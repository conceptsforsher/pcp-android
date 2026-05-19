#include "progressive/sqlite_wrapper.hpp"
#include <sqlite3.h>
#include <android/log.h>
#include <cstring>
#include <sstream>

#define LOG_TAG "SqliteDB"
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

namespace progressive {

// ==== SQLite Database Implementation ====

SqliteDB SqliteDB::open(const std::string& path) {
    SqliteDB db;
    sqlite3* handle = nullptr;
    int rc = sqlite3_open(path.c_str(), &handle);
    if (rc != SQLITE_OK) {
        LOGE("Cannot open database %s: %s", path.c_str(), sqlite3_errmsg(handle));
        sqlite3_close(handle);
        return db;
    }
    db.db_ = static_cast<void*>(handle);
    sqlite3_exec(handle, "PRAGMA journal_mode=WAL", nullptr, nullptr, nullptr);
    sqlite3_exec(handle, "PRAGMA synchronous=NORMAL", nullptr, nullptr, nullptr);
    sqlite3_exec(handle, "PRAGMA cache_size=-8000", nullptr, nullptr, nullptr);
    LOGE("SqliteDB: opened %s", path.c_str());
    return db;
}

SqliteDB::~SqliteDB() {
    if (db_) {
        sqlite3_close(static_cast<sqlite3*>(db_));
    }
}

bool SqliteDB::execute(const std::string& sql) {
    if (!db_) return false;
    char* errMsg = nullptr;
    int rc = sqlite3_exec(static_cast<sqlite3*>(db_), sql.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        LOGE("execute failed: %s", errMsg ? errMsg : "unknown");
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

bool SqliteDB::createTimelineSchema() {
    if (!db_) return false;
    const char* sql = R"SQL(
        CREATE TABLE IF NOT EXISTS events (
            event_id TEXT PRIMARY KEY,
            room_id TEXT NOT NULL,
            type TEXT,
            sender_id TEXT,
            content_json TEXT,
            origin_server_ts INTEGER,
            age_local_ts INTEGER,
            display_index INTEGER,
            state_key TEXT DEFAULT '',
            redacts TEXT DEFAULT '',
            relation_type TEXT DEFAULT '',
            relates_to_id TEXT DEFAULT ''
        );
        CREATE INDEX IF NOT EXISTS idx_events_room ON events(room_id, display_index);
        CREATE INDEX IF NOT EXISTS idx_events_relates ON events(relates_to_id);
        CREATE TABLE IF NOT EXISTS rooms (
            room_id TEXT PRIMARY KEY,
            display_name TEXT DEFAULT '',
            avatar_url TEXT DEFAULT '',
            topic TEXT DEFAULT '',
            membership TEXT DEFAULT '',
            notification_count INTEGER DEFAULT 0,
            highlight_count INTEGER DEFAULT 0,
            last_activity_ms INTEGER DEFAULT 0,
            is_direct INTEGER DEFAULT 0,
            is_space INTEGER DEFAULT 0,
            is_favourite INTEGER DEFAULT 0,
            is_encrypted INTEGER DEFAULT 0
        );
    )SQL";
    return execute(sql);
}

int SqliteDB::schemaVersion() {
    if (!db_) return 0;
    sqlite3_stmt* stmt = nullptr;
    sqlite3_prepare_v2(static_cast<sqlite3*>(db_), "PRAGMA user_version", -1, &stmt, nullptr);
    int version = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) version = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
    return version;
}

void SqliteDB::setSchemaVersion(int version) {
    if (!db_) return;
    std::ostringstream os;
    os << "PRAGMA user_version = " << version;
    execute(os.str());
}

bool SqliteDB::insertEvent(
    const std::string& eventId, const std::string& roomId,
    const std::string& type, const std::string& senderId,
    const std::string& contentJson, int64_t originServerTs,
    int64_t ageLocalTs, int displayIndex,
    const std::string& stateKey, const std::string& redacts,
    const std::string& relationType, const std::string& relatesToId)
{
    if (!db_) return false;
    const char* sql = R"SQL(
        INSERT OR REPLACE INTO events
        (event_id, room_id, type, sender_id, content_json,
         origin_server_ts, age_local_ts, display_index,
         state_key, redacts, relation_type, relates_to_id)
        VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
    )SQL";
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(static_cast<sqlite3*>(db_), sql, -1, &stmt, nullptr) != SQLITE_OK) return false;
    sqlite3_bind_text(stmt, 1, eventId.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, roomId.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, type.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, senderId.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, contentJson.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int64(stmt, 6, originServerTs);
    sqlite3_bind_int64(stmt, 7, ageLocalTs);
    sqlite3_bind_int(stmt, 8, displayIndex);
    sqlite3_bind_text(stmt, 9, stateKey.empty() ? nullptr : stateKey.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 10, redacts.empty() ? nullptr : redacts.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 11, relationType.empty() ? nullptr : relationType.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 12, relatesToId.empty() ? nullptr : relatesToId.c_str(), -1, SQLITE_STATIC);
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return rc == SQLITE_DONE;
}

bool SqliteDB::deleteEvent(const std::string& eventId) {
    if (!db_) return false;
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(static_cast<sqlite3*>(db_),
        "DELETE FROM events WHERE event_id = ?", -1, &stmt, nullptr) != SQLITE_OK) return false;
    sqlite3_bind_text(stmt, 1, eventId.c_str(), -1, SQLITE_TRANSIENT);
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return rc == SQLITE_DONE;
}

std::vector<SqliteDB::EventRow> SqliteDB::queryEvents(
    const std::string& roomId, int limit, int offset, bool ascending)
{
    std::vector<EventRow> results;
    if (!db_) return results;
    std::ostringstream os;
    os << "SELECT event_id, room_id, type, sender_id, content_json,"
          " origin_server_ts, age_local_ts, display_index, state_key,"
          " redacts, relation_type, relates_to_id FROM events"
          " WHERE room_id = ? ORDER BY display_index "
       << (ascending ? "ASC" : "DESC")
       << " LIMIT ? OFFSET ?";
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(static_cast<sqlite3*>(db_), os.str().c_str(), -1, &stmt, nullptr) != SQLITE_OK) return results;
    sqlite3_bind_text(stmt, 1, roomId.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, limit > 0 ? limit : 999999);
    sqlite3_bind_int(stmt, 3, offset);
    auto safe = [](sqlite3_stmt* s, int c) -> std::string {
        auto t = reinterpret_cast<const char*>(sqlite3_column_text(s, c));
        return t ? std::string(t) : std::string();
    };
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        EventRow r;
        r.eventId = safe(stmt, 0); r.roomId = safe(stmt, 1);
        r.type = safe(stmt, 2); r.senderId = safe(stmt, 3);
        r.contentJson = safe(stmt, 4);
        r.originServerTs = sqlite3_column_int64(stmt, 5);
        r.ageLocalTs = sqlite3_column_int64(stmt, 6);
        r.displayIndex = sqlite3_column_int(stmt, 7);
        r.stateKey = safe(stmt, 8); r.redacts = safe(stmt, 9);
        r.relationType = safe(stmt, 10); r.relatesToId = safe(stmt, 11);
        results.push_back(std::move(r));
    }
    sqlite3_finalize(stmt);
    return results;
}

SqliteDB::EventRow SqliteDB::queryEvent(const std::string& eventId) {
    EventRow r;
    if (!db_) return r;
    sqlite3_stmt* stmt = nullptr;
    sqlite3_prepare_v2(static_cast<sqlite3*>(db_),
        "SELECT event_id, room_id, type, sender_id, content_json,"
        " origin_server_ts, age_local_ts, display_index, state_key,"
        " redacts, relation_type, relates_to_id FROM events WHERE event_id = ?",
        -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, eventId.c_str(), -1, SQLITE_TRANSIENT);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        auto safe = [](sqlite3_stmt* s, int c) -> std::string {
            auto t = reinterpret_cast<const char*>(sqlite3_column_text(s, c));
            return t ? std::string(t) : std::string();
        };
        r.eventId = safe(stmt, 0); r.roomId = safe(stmt, 1);
        r.type = safe(stmt, 2); r.senderId = safe(stmt, 3);
        r.contentJson = safe(stmt, 4);
        r.originServerTs = sqlite3_column_int64(stmt, 5);
        r.ageLocalTs = sqlite3_column_int64(stmt, 6);
        r.displayIndex = sqlite3_column_int(stmt, 7);
        r.stateKey = safe(stmt, 8); r.redacts = safe(stmt, 9);
        r.relationType = safe(stmt, 10); r.relatesToId = safe(stmt, 11);
    }
    sqlite3_finalize(stmt);
    return r;
}

int SqliteDB::countEvents(const std::string& roomId) {
    if (!db_) return 0;
    sqlite3_stmt* stmt = nullptr;
    sqlite3_prepare_v2(static_cast<sqlite3*>(db_),
        "SELECT COUNT(*) FROM events WHERE room_id = ?", -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, roomId.c_str(), -1, SQLITE_TRANSIENT);
    int cnt = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) cnt = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
    return cnt;
}

int SqliteDB::maxDisplayIndex(const std::string& roomId) {
    if (!db_) return 0;
    sqlite3_stmt* stmt = nullptr;
    sqlite3_prepare_v2(static_cast<sqlite3*>(db_),
        "SELECT COALESCE(MAX(display_index), 0) FROM events WHERE room_id = ?", -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, roomId.c_str(), -1, SQLITE_TRANSIENT);
    int m = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) m = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
    return m;
}

bool SqliteDB::upsertRoom(
    const std::string& roomId, const std::string& displayName,
    const std::string& avatarUrl, const std::string& topic,
    const std::string& membership, int notificationCount,
    int highlightCount, int64_t lastActivityMs,
    bool isDirect, bool isSpace, bool isFavourite, bool isEncrypted)
{
    if (!db_) return false;
    // Try update first
    const char* updateSql = R"SQL(
        UPDATE rooms SET display_name=?, avatar_url=?, topic=?, membership=?,
        notification_count=?, highlight_count=?, last_activity_ms=?,
        is_direct=?, is_space=?, is_favourite=?, is_encrypted=?
        WHERE room_id=?
    )SQL";
    sqlite3_stmt* stmt = nullptr;
    sqlite3_prepare_v2(static_cast<sqlite3*>(db_), updateSql, -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, displayName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, avatarUrl.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, topic.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, membership.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 5, notificationCount);
    sqlite3_bind_int(stmt, 6, highlightCount);
    sqlite3_bind_int64(stmt, 7, lastActivityMs);
    sqlite3_bind_int(stmt, 8, isDirect ? 1 : 0);
    sqlite3_bind_int(stmt, 9, isSpace ? 1 : 0);
    sqlite3_bind_int(stmt, 10, isFavourite ? 1 : 0);
    sqlite3_bind_int(stmt, 11, isEncrypted ? 1 : 0);
    sqlite3_bind_text(stmt, 12, roomId.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_step(stmt);
    int changes = sqlite3_changes(static_cast<sqlite3*>(db_));
    sqlite3_finalize(stmt);
    if (changes > 0) return true;
    // Insert
    const char* insertSql = R"SQL(
        INSERT INTO rooms (room_id, display_name, avatar_url, topic, membership,
        notification_count, highlight_count, last_activity_ms,
        is_direct, is_space, is_favourite, is_encrypted)
        VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
    )SQL";
    sqlite3_prepare_v2(static_cast<sqlite3*>(db_), insertSql, -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, roomId.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, displayName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, avatarUrl.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, topic.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, membership.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 6, notificationCount);
    sqlite3_bind_int(stmt, 7, highlightCount);
    sqlite3_bind_int64(stmt, 8, lastActivityMs);
    sqlite3_bind_int(stmt, 9, isDirect ? 1 : 0);
    sqlite3_bind_int(stmt, 10, isSpace ? 1 : 0);
    sqlite3_bind_int(stmt, 11, isFavourite ? 1 : 0);
    sqlite3_bind_int(stmt, 12, isEncrypted ? 1 : 0);
    bool ok = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return ok;
}

std::vector<SqliteDB::RoomRow> SqliteDB::queryRooms() {
    std::vector<RoomRow> results;
    if (!db_) return results;
    sqlite3_stmt* stmt = nullptr;
    sqlite3_prepare_v2(static_cast<sqlite3*>(db_),
        "SELECT room_id, display_name, avatar_url, topic, membership,"
        " notification_count, highlight_count, last_activity_ms,"
        " is_direct, is_space, is_favourite, is_encrypted"
        " FROM rooms ORDER BY last_activity_ms DESC",
        -1, &stmt, nullptr);
    auto safe = [](sqlite3_stmt* s, int c) -> std::string {
        auto t = reinterpret_cast<const char*>(sqlite3_column_text(s, c));
        return t ? std::string(t) : std::string();
    };
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        RoomRow r;
        r.roomId = safe(stmt, 0); r.displayName = safe(stmt, 1);
        r.avatarUrl = safe(stmt, 2); r.topic = safe(stmt, 3);
        r.membership = safe(stmt, 4);
        r.notificationCount = sqlite3_column_int(stmt, 5);
        r.highlightCount = sqlite3_column_int(stmt, 6);
        r.lastActivityMs = sqlite3_column_int64(stmt, 7);
        r.isDirect = sqlite3_column_int(stmt, 8) != 0;
        r.isSpace = sqlite3_column_int(stmt, 9) != 0;
        r.isFavourite = sqlite3_column_int(stmt, 10) != 0;
        r.isEncrypted = sqlite3_column_int(stmt, 11) != 0;
        results.push_back(std::move(r));
    }
    sqlite3_finalize(stmt);
    return results;
}

void SqliteDB::beginTransaction() { execute("BEGIN TRANSACTION"); }
void SqliteDB::commitTransaction() { execute("COMMIT"); }
void SqliteDB::rollbackTransaction() { execute("ROLLBACK"); }

// ==== SQL Helpers ====

std::string sqlEscape(const std::string& s) {
    std::string result;
    for (char c : s) {
        if (c == '\'') result += "''";
        else result += c;
    }
    return result;
}

std::string buildInsertSql(
    const std::string& table,
    const std::vector<std::string>& columns,
    const std::vector<std::string>& values)
{
    std::string sql = "INSERT OR REPLACE INTO " + table + " (";
    for (size_t i = 0; i < columns.size(); i++) {
        if (i > 0) sql += ", ";
        sql += columns[i];
    }
    sql += ") VALUES (";
    for (size_t i = 0; i < values.size(); i++) {
        if (i > 0) sql += ", ";
        sql += values[i];
    }
    sql += ")";
    return sql;
}

} // namespace progressive
