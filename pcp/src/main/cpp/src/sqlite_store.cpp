// ============================================================================
// sqlite_store.cpp — SQLite implementation replacing Realm database
// ============================================================================

#include "pcp/sqlite_store.hpp"
#include <sqlite3.h>
#include <cstdio>
#include <cstring>
#include <sstream>

namespace pcp {

// ============================================================================
// SqliteStore implementation
// ============================================================================

SqliteStore::SqliteStore() = default;

SqliteStore::~SqliteStore() {
    close();
}

bool SqliteStore::open(const std::string& dbPath, const std::string& /*key*/) {
    if (db_) return false;
    int rc = sqlite3_open(dbPath.c_str(), (sqlite3**)&db_);
    if (rc != SQLITE_OK) return false;

    // Enable WAL mode for concurrent reads
    exec("PRAGMA journal_mode=WAL");
    exec("PRAGMA foreign_keys=ON");
    exec("PRAGMA synchronous=NORMAL");

    // Create schema if first open
    createSchema();

    return true;
}

void SqliteStore::close() {
    if (db_) {
        sqlite3_close((sqlite3*)db_);
        db_ = nullptr;
    }
}

void SqliteStore::exec(const std::string& sql) {
    if (!db_) return;
    char* err = nullptr;
    sqlite3_exec((sqlite3*)db_, sql.c_str(), nullptr, nullptr, &err);
    if (err) {
        sqlite3_free(err);
    }
}

bool SqliteStore::execSafe(const std::string& sql) {
    if (!db_) return false;
    char* err = nullptr;
    int rc = sqlite3_exec((sqlite3*)db_, sql.c_str(), nullptr, nullptr, &err);
    if (err) { sqlite3_free(err); }
    return rc == SQLITE_OK;
}

bool SqliteStore::tableExists(const std::string& name) {
    auto sql = "SELECT name FROM sqlite_master WHERE type='table' AND name='" + name + "'";
    sqlite3_stmt* stmt = nullptr;
    bool exists = false;
    if (sqlite3_prepare_v2((sqlite3*)db_, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        exists = sqlite3_step(stmt) == SQLITE_ROW;
    }
    sqlite3_finalize(stmt);
    return exists;
}

// ================================================================
// Schema Creation
// ================================================================

bool SqliteStore::createSchema() {
    if (!db_) return false;

    // === Events table (replaces EventEntity + TimelineEventEntity) ===
    exec(R"SQL(
        CREATE TABLE IF NOT EXISTS events (
            event_id TEXT PRIMARY KEY,
            room_id TEXT NOT NULL,
            type TEXT NOT NULL DEFAULT '',
            state_key TEXT DEFAULT '',
            sender_id TEXT DEFAULT '',
            content_json TEXT DEFAULT '{}',
            prev_content_json TEXT DEFAULT '',
            unsigned_data_json TEXT DEFAULT '',
            redacts TEXT DEFAULT '',
            decryption_result_json TEXT DEFAULT '',
            origin_server_ts INTEGER DEFAULT 0,
            age_local_ts INTEGER DEFAULT 0,
            display_index INTEGER DEFAULT 0,
            is_root_thread INTEGER DEFAULT 0,
            root_thread_event_id TEXT DEFAULT '',
            number_of_threads INTEGER DEFAULT 0,
            thread_summary_json TEXT DEFAULT '',
            send_state TEXT DEFAULT 'UNSENT',
            send_state_details_json TEXT DEFAULT '',
            sent_by_me INTEGER DEFAULT 0,
            is_encrypted INTEGER DEFAULT 0,
            relation_type TEXT DEFAULT '',
            relates_to_id TEXT DEFAULT '',
            relation_key TEXT DEFAULT ''
        )
    )SQL");

    // Indices
    exec("CREATE INDEX IF NOT EXISTS idx_events_room ON events(room_id)");
    exec("CREATE INDEX IF NOT EXISTS idx_events_room_display ON events(room_id, display_index)");
    exec("CREATE INDEX IF NOT EXISTS idx_events_type ON events(type)");
    exec("CREATE INDEX IF NOT EXISTS idx_events_sender ON events(sender_id)");
    exec("CREATE INDEX IF NOT EXISTS idx_events_root_thread ON events(root_thread_event_id)");

    // === Room Summary table (replaces RoomSummaryEntity + RoomEntity) ===
    exec(R"SQL(
        CREATE TABLE IF NOT EXISTS room_summaries (
            room_id TEXT PRIMARY KEY,
            room_type TEXT DEFAULT '',
            display_name TEXT DEFAULT '',
            normalized_display_name TEXT DEFAULT '',
            avatar_url TEXT DEFAULT '',
            name TEXT DEFAULT '',
            topic TEXT DEFAULT '',
            canonical_alias TEXT DEFAULT '',
            flat_aliases TEXT DEFAULT '',
            membership TEXT DEFAULT 'NONE',
            join_rules TEXT DEFAULT '',
            versioning_state TEXT DEFAULT 'NONE',
            joined_members_count INTEGER DEFAULT 0,
            invited_members_count INTEGER DEFAULT 0,
            is_direct INTEGER DEFAULT 0,
            direct_user_id TEXT DEFAULT '',
            notification_count INTEGER DEFAULT 0,
            highlight_count INTEGER DEFAULT 0,
            thread_notification_count INTEGER DEFAULT 0,
            thread_highlight_count INTEGER DEFAULT 0,
            has_unread_messages INTEGER DEFAULT 0,
            is_encrypted INTEGER DEFAULT 0,
            e2e_algorithm TEXT DEFAULT '',
            encryption_event_ts INTEGER DEFAULT 0,
            room_encryption_trust_level_str TEXT DEFAULT '',
            is_favourite INTEGER DEFAULT 0,
            is_low_priority INTEGER DEFAULT 0,
            is_server_notice INTEGER DEFAULT 0,
            last_activity_time INTEGER DEFAULT 0,
            read_marker_id TEXT DEFAULT '',
            inviter_id TEXT DEFAULT '',
            breadcrumbs_index INTEGER DEFAULT -1,
            has_failed_sending INTEGER DEFAULT 0,
            is_hidden_from_user INTEGER DEFAULT 0,
            flatten_parent_ids TEXT DEFAULT '',
            direct_parent_names_json TEXT DEFAULT '[]',
            heroes_json TEXT DEFAULT '[]',
            other_member_ids_json TEXT DEFAULT '[]',
            latest_event_id TEXT DEFAULT '',
            latest_event_json TEXT DEFAULT '',
            draft_json TEXT DEFAULT ''
        )
    )SQL");

    exec("CREATE INDEX IF NOT EXISTS idx_room_membership ON room_summaries(membership)");
    exec("CREATE INDEX IF NOT EXISTS idx_room_favourite ON room_summaries(is_favourite)");
    exec("CREATE INDEX IF NOT EXISTS idx_room_direct ON room_summaries(is_direct)");
    exec("CREATE INDEX IF NOT EXISTS idx_room_activity ON room_summaries(last_activity_time)");

    // === Chunks table ===
    exec(R"SQL(
        CREATE TABLE IF NOT EXISTS chunks (
            room_id TEXT NOT NULL,
            chunk_index INTEGER NOT NULL DEFAULT 0,
            prev_batch TEXT DEFAULT '',
            next_batch TEXT DEFAULT '',
            is_last_forward INTEGER DEFAULT 1,
            is_last_backward INTEGER DEFAULT 0,
            number_of_events INTEGER DEFAULT 0,
            PRIMARY KEY (room_id, chunk_index)
        )
    )SQL");

    // === Room Tags table ===
    exec(R"SQL(
        CREATE TABLE IF NOT EXISTS room_tags (
            room_id TEXT NOT NULL,
            tag_name TEXT NOT NULL,
            tag_order REAL DEFAULT 0.0,
            PRIMARY KEY (room_id, tag_name)
        )
    )SQL");

    // === Room Members table ===
    exec(R"SQL(
        CREATE TABLE IF NOT EXISTS room_members (
            room_id TEXT NOT NULL,
            user_id TEXT NOT NULL,
            display_name TEXT DEFAULT '',
            avatar_url TEXT DEFAULT '',
            membership TEXT DEFAULT 'JOIN',
            PRIMARY KEY (room_id, user_id)
        )
    )SQL");

    // === Drafts table ===
    exec(R"SQL(
        CREATE TABLE IF NOT EXISTS drafts (
            room_id TEXT NOT NULL,
            user_id TEXT NOT NULL,
            draft_json TEXT DEFAULT '{}',
            PRIMARY KEY (room_id, user_id)
        )
    )SQL");

    // === Read Receipts table ===
    exec(R"SQL(
        CREATE TABLE IF NOT EXISTS read_receipts (
            room_id TEXT NOT NULL,
            event_id TEXT NOT NULL,
            user_id TEXT NOT NULL,
            timestamp_ms INTEGER DEFAULT 0,
            PRIMARY KEY (room_id, event_id, user_id)
        )
    )SQL");

    // === Read Markers table ===
    exec(R"SQL(
        CREATE TABLE IF NOT EXISTS read_markers (
            room_id TEXT PRIMARY KEY,
            event_id TEXT DEFAULT ''
        )
    )SQL");

    // === Reactions table ===
    exec(R"SQL(
        CREATE TABLE IF NOT EXISTS reactions (
            event_id TEXT NOT NULL,
            reaction_key TEXT NOT NULL,
            user_id TEXT NOT NULL,
            timestamp_ms INTEGER DEFAULT 0,
            PRIMARY KEY (event_id, reaction_key, user_id)
        )
    )SQL");

    // === Push Rules table ===
    exec(R"SQL(
        CREATE TABLE IF NOT EXISTS push_rules (
            rule_id TEXT PRIMARY KEY,
            kind TEXT DEFAULT '',
            actions_json TEXT DEFAULT '[]',
            conditions_json TEXT DEFAULT '[]',
            enabled INTEGER DEFAULT 1,
            is_default INTEGER DEFAULT 0
        )
    )SQL");

    // === Filters table ===
    exec(R"SQL(
        CREATE TABLE IF NOT EXISTS filters (
            user_id TEXT NOT NULL,
            filter_id TEXT NOT NULL,
            filter_json TEXT DEFAULT '{}',
            PRIMARY KEY (user_id, filter_id)
        )
    )SQL");

    // === Breadcrumbs table ===
    exec(R"SQL(
        CREATE TABLE IF NOT EXISTS breadcrumbs (
            room_id TEXT PRIMARY KEY,
            breadcrumb_index INTEGER DEFAULT 0
        )
    )SQL");

    // === Home Server Capabilities table ===
    exec(R"SQL(
        CREATE TABLE IF NOT EXISTS homeserver_capabilities (
            id INTEGER PRIMARY KEY DEFAULT 1,
            capabilities_json TEXT DEFAULT '{}'
        )
    )SQL");

    // === Ignored Users table ===
    exec(R"SQL(
        CREATE TABLE IF NOT EXISTS ignored_users (
            user_id TEXT PRIMARY KEY
        )
    )SQL");

    // === Preview URL Cache table ===
    exec(R"SQL(
        CREATE TABLE IF NOT EXISTS preview_url_cache (
            url TEXT PRIMARY KEY,
            data_json TEXT DEFAULT '{}',
            cached_at_ms INTEGER DEFAULT 0
        )
    )SQL");

    // === Sync State table ===
    exec(R"SQL(
        CREATE TABLE IF NOT EXISTS sync_state (
            id INTEGER PRIMARY KEY DEFAULT 1,
            sync_token TEXT DEFAULT '',
            last_sync_ms INTEGER DEFAULT 0,
            filter_id TEXT DEFAULT ''
        )
    )SQL");

    // === Space Children table ===
    exec(R"SQL(
        CREATE TABLE IF NOT EXISTS space_children (
            parent_id TEXT NOT NULL,
            child_id TEXT NOT NULL,
            sort_order TEXT DEFAULT '',
            suggested INTEGER DEFAULT 0,
            via_servers_json TEXT DEFAULT '[]',
            PRIMARY KEY (parent_id, child_id)
        )
    )SQL");

    // === Space Parents table ===
    exec(R"SQL(
        CREATE TABLE IF NOT EXISTS space_parents (
            child_id TEXT NOT NULL,
            parent_id TEXT NOT NULL,
            canonical INTEGER DEFAULT 0,
            via_servers_json TEXT DEFAULT '[]',
            PRIMARY KEY (child_id, parent_id)
        )
    )SQL");

    // === Account Data table ===
    exec(R"SQL(
        CREATE TABLE IF NOT EXISTS account_data (
            user_id TEXT NOT NULL DEFAULT '',
            room_id TEXT NOT NULL DEFAULT '',
            type TEXT NOT NULL,
            content_json TEXT DEFAULT '{}',
            PRIMARY KEY (user_id, room_id, type)
        )
    )SQL");

    // === 3PID table ===
    exec(R"SQL(
        CREATE TABLE IF NOT EXISTS three_pids (
            user_id TEXT NOT NULL,
            medium TEXT NOT NULL,
            address TEXT NOT NULL,
            validated INTEGER DEFAULT 0,
            PRIMARY KEY (user_id, medium, address)
        )
    )SQL");

    // === Pushers table ===
    exec(R"SQL(
        CREATE TABLE IF NOT EXISTS pushers (
            push_key TEXT PRIMARY KEY,
            app_id TEXT DEFAULT '',
            app_display_name TEXT DEFAULT '',
            device_display_name TEXT DEFAULT '',
            lang TEXT DEFAULT '',
            data_json TEXT DEFAULT '{}',
            profile_tag TEXT DEFAULT ''
        )
    )SQL");

    // Set initial schema version
    exec("PRAGMA user_version = " + std::to_string(SQLITE_STORE_SCHEMA_VERSION));

    return true;
}

int SqliteStore::schemaVersion() {
    if (!db_) return 0;
    sqlite3_stmt* stmt = nullptr;
    int version = 0;
    if (sqlite3_prepare_v2((sqlite3*)db_, "PRAGMA user_version", -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            version = sqlite3_column_int(stmt, 0);
        }
    }
    sqlite3_finalize(stmt);
    return version;
}

bool SqliteStore::migrateToLatest() {
    int v = schemaVersion();
    if (v >= SQLITE_STORE_SCHEMA_VERSION) return true;

    if (v < 1) migrateV1();
    if (v < 2) migrateV2();
    if (v < 3) migrateV3();
    if (v < 4) migrateV4();
    if (v < 5) migrateV5();

    return schemaVersion() >= SQLITE_STORE_SCHEMA_VERSION;
}

bool SqliteStore::migrateV1() {
    createSchema();
    return true;
}
bool SqliteStore::migrateV2() {
    exec("ALTER TABLE events ADD COLUMN is_root_thread INTEGER DEFAULT 0");
    exec("ALTER TABLE events ADD COLUMN root_thread_event_id TEXT DEFAULT ''");
    exec("ALTER TABLE events ADD COLUMN number_of_threads INTEGER DEFAULT 0");
    exec("ALTER TABLE events ADD COLUMN thread_summary_json TEXT DEFAULT ''");
    exec("CREATE INDEX IF NOT EXISTS idx_events_root_thread ON events(root_thread_event_id)");
    exec("ALTER TABLE room_summaries ADD COLUMN thread_notification_count INTEGER DEFAULT 0");
    exec("ALTER TABLE room_summaries ADD COLUMN thread_highlight_count INTEGER DEFAULT 0");
    exec("PRAGMA user_version = 2");
    return true;
}
bool SqliteStore::migrateV3() {
    // Space hierarchy tables already created in createSchema, ensure they exist
    exec(R"SQL(
        CREATE TABLE IF NOT EXISTS space_children (
            parent_id TEXT NOT NULL, child_id TEXT NOT NULL, sort_order TEXT DEFAULT '',
            suggested INTEGER DEFAULT 0, via_servers_json TEXT DEFAULT '[]',
            PRIMARY KEY (parent_id, child_id))
    )SQL");
    exec(R"SQL(
        CREATE TABLE IF NOT EXISTS space_parents (
            child_id TEXT NOT NULL, parent_id TEXT NOT NULL, canonical INTEGER DEFAULT 0,
            via_servers_json TEXT DEFAULT '[]', PRIMARY KEY (child_id, parent_id))
    )SQL");
    exec("ALTER TABLE room_summaries ADD COLUMN flatten_parent_ids TEXT DEFAULT ''");
    exec("ALTER TABLE room_summaries ADD COLUMN direct_parent_names_json TEXT DEFAULT '[]'");
    exec("PRAGMA user_version = 3");
    return true;
}
bool SqliteStore::migrateV4() {
    // Poll and location tables
    exec(R"SQL(
        CREATE TABLE IF NOT EXISTS poll_responses (
            event_id TEXT NOT NULL, option_id TEXT NOT NULL, user_id TEXT NOT NULL,
            timestamp_ms INTEGER DEFAULT 0, PRIMARY KEY (event_id, option_id, user_id))
    )SQL");
    exec("ALTER TABLE room_summaries ADD COLUMN room_type TEXT DEFAULT ''");
    exec("PRAGMA user_version = 4");
    return true;
}
bool SqliteStore::migrateV5() {
    exec(R"SQL(
        CREATE TABLE IF NOT EXISTS push_rules (
            rule_id TEXT PRIMARY KEY, kind TEXT DEFAULT '', actions_json TEXT DEFAULT '[]',
            conditions_json TEXT DEFAULT '[]', enabled INTEGER DEFAULT 1, is_default INTEGER DEFAULT 0)
    )SQL");
    exec(R"SQL(
        CREATE TABLE IF NOT EXISTS pushers (
            push_key TEXT PRIMARY KEY, app_id TEXT DEFAULT '', app_display_name TEXT DEFAULT '',
            device_display_name TEXT DEFAULT '', lang TEXT DEFAULT '', data_json TEXT DEFAULT '{}',
            profile_tag TEXT DEFAULT '')
    )SQL");
    exec("PRAGMA user_version = 5");
    return true;
}

// ================================================================
// Transaction Support
// ================================================================

void SqliteStore::beginTransaction() { exec("BEGIN TRANSACTION"); }
void SqliteStore::commitTransaction() { exec("COMMIT"); }
void SqliteStore::rollbackTransaction() { exec("ROLLBACK"); }

// ================================================================
// Room Summary operations
// ================================================================

bool SqliteStore::upsertRoomSummary(const DbRoomSummary& s) {
    std::ostringstream sql;
    sql << "INSERT OR REPLACE INTO room_summaries (room_id,room_type,display_name,normalized_display_name,"
        << "avatar_url,name,topic,canonical_alias,flat_aliases,membership,join_rules,versioning_state,"
        << "joined_members_count,invited_members_count,is_direct,direct_user_id,"
        << "notification_count,highlight_count,thread_notification_count,thread_highlight_count,"
        << "has_unread_messages,is_encrypted,e2e_algorithm,encryption_event_ts,"
        << "room_encryption_trust_level_str,is_favourite,is_low_priority,is_server_notice,"
        << "last_activity_time,read_marker_id,inviter_id,breadcrumbs_index,"
        << "has_failed_sending,is_hidden_from_user,flatten_parent_ids,direct_parent_names_json,"
        << "heroes_json,other_member_ids_json,latest_event_id,latest_event_json,draft_json) VALUES ('"
        << s.roomId << "','" << s.roomType << "','" << s.displayName << "','" << s.normalizedDisplayName << "','"
        << s.avatarUrl << "','" << s.name << "','" << s.topic << "','" << s.canonicalAlias << "','"
        << s.flatAliases << "','" << s.membership << "','" << s.joinRules << "','"
        << s.versioningState << "'," << s.joinedMembersCount << "," << s.invitedMembersCount << ","
        << (s.isDirect ? 1 : 0) << ",'" << s.directUserId << "',"
        << s.notificationCount << "," << s.highlightCount << ","
        << s.threadNotificationCount << "," << s.threadHighlightCount << ","
        << (s.hasUnreadMessages ? 1 : 0) << "," << (s.isEncrypted ? 1 : 0) << ",'"
        << s.e2eAlgorithm << "'," << s.encryptionEventTs << ",'"
        << s.roomEncryptionTrustLevelStr << "'," << (s.isFavourite ? 1 : 0) << ","
        << (s.isLowPriority ? 1 : 0) << "," << (s.isServerNotice ? 1 : 0) << ","
        << s.lastActivityTime << ",'" << s.readMarkerId << "','" << s.inviterId << "',"
        << s.breadcrumbsIndex << "," << (s.hasFailedSending ? 1 : 0) << ","
        << (s.isHiddenFromUser ? 1 : 0) << ",'" << s.flattenParentIds << "','"
        << s.directParentNamesJson << "','" << s.heroesJson << "','"
        << s.otherMemberIdsJson << "','" << s.latestEventId << "','"
        << s.latestEventJson << "','" << s.draftJson << "')";
    return execSafe(sql.str());
}

DbRoomSummary SqliteStore::getRoomSummary(const std::string& roomId) {
    DbRoomSummary s;
    if (!db_) return s;

    auto sql = "SELECT * FROM room_summaries WHERE room_id='" + roomId + "'";
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2((sqlite3*)db_, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            s.roomId = (const char*)sqlite3_column_text(stmt, 0);
            s.membership = (const char*)sqlite3_column_text(stmt, 8);
            s.displayName = (const char*)sqlite3_column_text(stmt, 2);
            s.avatarUrl = (const char*)sqlite3_column_text(stmt, 4);
            s.topic = (const char*)sqlite3_column_text(stmt, 6);
            s.notificationCount = sqlite3_column_int(stmt, 15);
            s.highlightCount = sqlite3_column_int(stmt, 16);
            s.isDirect = sqlite3_column_int(stmt, 13) != 0;
            s.lastActivityTime = sqlite3_column_int64(stmt, 27);
            s.isEncrypted = sqlite3_column_int(stmt, 19) != 0;
        }
    }
    sqlite3_finalize(stmt);
    return s;
}

std::vector<DbRoomSummary> SqliteStore::getAllRoomSummaries() {
    std::vector<DbRoomSummary> result;
    if (!db_) return result;

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2((sqlite3*)db_,
            "SELECT room_id,display_name,avatar_url,membership,notification_count,"
            "highlight_count,is_direct,is_encrypted,last_activity_time,is_favourite,"
            "is_low_priority,has_unread_messages,topic FROM room_summaries "
            "WHERE is_hidden_from_user=0 ORDER BY is_favourite DESC, last_activity_time DESC",
            -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            DbRoomSummary s;
            s.roomId = (const char*)sqlite3_column_text(stmt, 0);
            s.displayName = (const char*)sqlite3_column_text(stmt, 1);
            if (sqlite3_column_text(stmt, 2)) s.avatarUrl = (const char*)sqlite3_column_text(stmt, 2);
            if (sqlite3_column_text(stmt, 3)) s.membership = (const char*)sqlite3_column_text(stmt, 3);
            s.notificationCount = sqlite3_column_int(stmt, 4);
            s.highlightCount = sqlite3_column_int(stmt, 5);
            s.isDirect = sqlite3_column_int(stmt, 6) != 0;
            s.isEncrypted = sqlite3_column_int(stmt, 7) != 0;
            s.lastActivityTime = sqlite3_column_int64(stmt, 8);
            s.isFavourite = sqlite3_column_int(stmt, 9) != 0;
            s.isLowPriority = sqlite3_column_int(stmt, 10) != 0;
            s.hasUnreadMessages = sqlite3_column_int(stmt, 11) != 0;
            if (sqlite3_column_text(stmt, 12)) s.topic = (const char*)sqlite3_column_text(stmt, 12);
            result.push_back(s);
        }
    }
    sqlite3_finalize(stmt);
    return result;
}

bool SqliteStore::deleteRoomSummary(const std::string& roomId) {
    return execSafe("DELETE FROM room_summaries WHERE room_id='" + roomId + "'");
}

// ================================================================
// Room Tags
// ================================================================

bool SqliteStore::upsertRoomTag(const std::string& roomId, const std::string& tagName, double order) {
    std::ostringstream sql;
    sql << "INSERT OR REPLACE INTO room_tags (room_id,tag_name,tag_order) VALUES ('"
        << roomId << "','" << tagName << "'," << order << ")";
    return execSafe(sql.str());
}

bool SqliteStore::deleteRoomTag(const std::string& roomId, const std::string& tagName) {
    return execSafe("DELETE FROM room_tags WHERE room_id='" + roomId + "' AND tag_name='" + tagName + "'");
}

std::vector<std::pair<std::string,double>> SqliteStore::getRoomTags(const std::string& roomId) {
    std::vector<std::pair<std::string,double>> result;
    sqlite3_stmt* stmt = nullptr;
    auto sql = "SELECT tag_name,tag_order FROM room_tags WHERE room_id='" + roomId + "'";
    if (sqlite3_prepare_v2((sqlite3*)db_, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            result.push_back({
                (const char*)sqlite3_column_text(stmt, 0),
                sqlite3_column_double(stmt, 1)
            });
        }
    }
    sqlite3_finalize(stmt);
    return result;
}

// ================================================================
// Room Members
// ================================================================

bool SqliteStore::upsertRoomMember(const std::string& roomId, const std::string& userId,
    const std::string& displayName, const std::string& avatarUrl, const std::string& membership) {
    std::ostringstream sql;
    sql << "INSERT OR REPLACE INTO room_members (room_id,user_id,display_name,avatar_url,membership) VALUES ('"
        << roomId << "','" << userId << "','" << displayName << "','" << avatarUrl << "','"
        << membership << "')";
    return execSafe(sql.str());
}

std::string SqliteStore::getRoomMembersJson(const std::string& roomId) {
    std::ostringstream json;
    json << "[";
    sqlite3_stmt* stmt = nullptr;
    auto sql = "SELECT user_id,display_name,avatar_url,membership FROM room_members WHERE room_id='" + roomId + "'";
    if (sqlite3_prepare_v2((sqlite3*)db_, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        bool first = true;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            if (!first) json << ",";
            first = false;
            json << "{\"user_id\":\"" << (const char*)sqlite3_column_text(stmt, 0) << "\"";
            if (sqlite3_column_text(stmt, 1)) json << ",\"display_name\":\"" << sqlite3_column_text(stmt, 1) << "\"";
            if (sqlite3_column_text(stmt, 2)) json << ",\"avatar_url\":\"" << sqlite3_column_text(stmt, 2) << "\"";
            json << ",\"membership\":\"" << (const char*)sqlite3_column_text(stmt, 3) << "\"}";
        }
    }
    sqlite3_finalize(stmt);
    json << "]";
    return json.str();
}

// ================================================================
// Timeline Events
// ================================================================

bool SqliteStore::insertEvent(const DbTimelineEvent& e) {
    std::ostringstream sql;
    sql << "INSERT OR REPLACE INTO events (event_id,room_id,type,state_key,sender_id,content_json,"
        << "prev_content_json,unsigned_data_json,redacts,decryption_result_json,"
        << "origin_server_ts,age_local_ts,display_index,is_root_thread,root_thread_event_id,"
        << "number_of_threads,thread_summary_json,send_state,send_state_details_json,"
        << "sent_by_me,is_encrypted,relation_type,relates_to_id,relation_key) VALUES ('"
        << e.eventId << "','" << e.roomId << "','" << e.type << "','" << e.stateKey << "','"
        << e.senderId << "','" << e.contentJson << "','" << e.prevContentJson << "','"
        << e.unsignedDataJson << "','" << e.redacts << "','" << e.decryptionResultJson << "',"
        << e.originServerTs << "," << e.ageLocalTs << "," << e.displayIndex << ","
        << (e.isRootThread ? 1 : 0) << ",'" << e.rootThreadEventId << "',"
        << e.numberOfThreads << ",'" << e.threadSummaryJson << "','" << e.sendState << "','"
        << e.sendStateDetailsJson << "'," << (e.sentByMe ? 1 : 0) << ","
        << (e.isEncrypted ? 1 : 0) << ",'" << e.relationType << "','"
        << e.relatesToId << "','" << e.relationKey << "')";
    return execSafe(sql.str());
}

bool SqliteStore::insertEvents(const std::vector<DbTimelineEvent>& events) {
    beginTransaction();
    for (const auto& e : events) {
        if (!insertEvent(e)) {
            rollbackTransaction();
            return false;
        }
    }
    commitTransaction();
    return true;
}

bool SqliteStore::deleteEvent(const std::string& eventId) {
    return execSafe("DELETE FROM events WHERE event_id='" + eventId + "'");
}

DbTimelineEvent SqliteStore::getEvent(const std::string& eventId) {
    DbTimelineEvent e;
    sqlite3_stmt* stmt = nullptr;
    auto sql = "SELECT * FROM events WHERE event_id='" + eventId + "'";
    if (sqlite3_prepare_v2((sqlite3*)db_, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            e.eventId = (const char*)sqlite3_column_text(stmt, 0);
            e.roomId = (const char*)sqlite3_column_text(stmt, 1);
            e.type = (const char*)sqlite3_column_text(stmt, 2);
            if (sqlite3_column_text(stmt, 4)) e.senderId = (const char*)sqlite3_column_text(stmt, 4);
            if (sqlite3_column_text(stmt, 5)) e.contentJson = (const char*)sqlite3_column_text(stmt, 5);
            e.originServerTs = sqlite3_column_int64(stmt, 10);
            e.displayIndex = sqlite3_column_int(stmt, 12);
            if (sqlite3_column_text(stmt, 20)) e.relationType = (const char*)sqlite3_column_text(stmt, 20);
            if (sqlite3_column_text(stmt, 21)) e.relatesToId = (const char*)sqlite3_column_text(stmt, 21);
        }
    }
    sqlite3_finalize(stmt);
    return e;
}

std::vector<DbTimelineEvent> SqliteStore::queryEvents(const std::string& roomId,
    int limit, int offset, bool ascending) {
    std::vector<DbTimelineEvent> result;
    std::ostringstream sql;
    sql << "SELECT event_id,room_id,type,sender_id,content_json,origin_server_ts,"
        << "display_index,is_encrypted,relation_type,relates_to_id "
        << "FROM events WHERE room_id='" << roomId << "' ORDER BY display_index "
        << (ascending ? "ASC" : "DESC");
    if (limit > 0) sql << " LIMIT " << limit;
    if (offset > 0) sql << " OFFSET " << offset;

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2((sqlite3*)db_, sql.str().c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            DbTimelineEvent e;
            e.eventId = (const char*)sqlite3_column_text(stmt, 0);
            e.roomId = (const char*)sqlite3_column_text(stmt, 1);
            if (sqlite3_column_text(stmt, 2)) e.type = (const char*)sqlite3_column_text(stmt, 2);
            if (sqlite3_column_text(stmt, 3)) e.senderId = (const char*)sqlite3_column_text(stmt, 3);
            if (sqlite3_column_text(stmt, 4)) e.contentJson = (const char*)sqlite3_column_text(stmt, 4);
            e.originServerTs = sqlite3_column_int64(stmt, 5);
            e.displayIndex = sqlite3_column_int(stmt, 6);
            e.isEncrypted = sqlite3_column_int(stmt, 7) != 0;
            if (sqlite3_column_text(stmt, 8)) e.relationType = (const char*)sqlite3_column_text(stmt, 8);
            if (sqlite3_column_text(stmt, 9)) e.relatesToId = (const char*)sqlite3_column_text(stmt, 9);
            result.push_back(e);
        }
    }
    sqlite3_finalize(stmt);
    return result;
}

int SqliteStore::countEvents(const std::string& roomId) {
    sqlite3_stmt* stmt = nullptr;
    int count = 0;
    auto sql = "SELECT COUNT(*) FROM events WHERE room_id='" + roomId + "'";
    if (sqlite3_prepare_v2((sqlite3*)db_, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) count = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    return count;
}

int SqliteStore::maxDisplayIndex(const std::string& roomId) {
    sqlite3_stmt* stmt = nullptr;
    int maxIdx = 0;
    auto sql = "SELECT MAX(display_index) FROM events WHERE room_id='" + roomId + "'";
    if (sqlite3_prepare_v2((sqlite3*)db_, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) maxIdx = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    return maxIdx;
}

// ================================================================
// Chunks
// ================================================================

bool SqliteStore::upsertChunk(const DbChunk& chunk) {
    std::ostringstream sql;
    sql << "INSERT OR REPLACE INTO chunks (room_id,chunk_index,prev_batch,next_batch,"
        << "is_last_forward,is_last_backward,number_of_events) VALUES ('"
        << chunk.roomId << "'," << chunk.chunkIndex << ",'" << chunk.prevBatch << "','"
        << chunk.nextBatch << "'," << (chunk.isLastForward ? 1 : 0) << ","
        << (chunk.isLastBackward ? 1 : 0) << "," << chunk.numberOfEvents << ")";
    return execSafe(sql.str());
}

DbChunk SqliteStore::getChunk(const std::string& roomId, int chunkIndex) {
    DbChunk c;
    std::ostringstream sql;
    sql << "SELECT * FROM chunks WHERE room_id='" << roomId << "' AND chunk_index=" << chunkIndex;
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2((sqlite3*)db_, sql.str().c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            c.roomId = (const char*)sqlite3_column_text(stmt, 0);
            c.chunkIndex = sqlite3_column_int(stmt, 1);
            if (sqlite3_column_text(stmt, 2)) c.prevBatch = (const char*)sqlite3_column_text(stmt, 2);
            if (sqlite3_column_text(stmt, 3)) c.nextBatch = (const char*)sqlite3_column_text(stmt, 3);
            c.isLastForward = sqlite3_column_int(stmt, 4) != 0;
            c.isLastBackward = sqlite3_column_int(stmt, 5) != 0;
            c.numberOfEvents = sqlite3_column_int(stmt, 6);
        }
    }
    sqlite3_finalize(stmt);
    return c;
}

std::vector<DbChunk> SqliteStore::getChunks(const std::string& roomId) {
    std::vector<DbChunk> result;
    auto sql = "SELECT * FROM chunks WHERE room_id='" + roomId + "' ORDER BY chunk_index";
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2((sqlite3*)db_, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            DbChunk c;
            c.roomId = (const char*)sqlite3_column_text(stmt, 0);
            c.chunkIndex = sqlite3_column_int(stmt, 1);
            if (sqlite3_column_text(stmt, 2)) c.prevBatch = (const char*)sqlite3_column_text(stmt, 2);
            if (sqlite3_column_text(stmt, 3)) c.nextBatch = (const char*)sqlite3_column_text(stmt, 3);
            c.isLastForward = sqlite3_column_int(stmt, 4) != 0;
            c.isLastBackward = sqlite3_column_int(stmt, 5) != 0;
            c.numberOfEvents = sqlite3_column_int(stmt, 6);
            result.push_back(c);
        }
    }
    sqlite3_finalize(stmt);
    return result;
}

bool SqliteStore::deleteChunks(const std::string& roomId) {
    return execSafe("DELETE FROM chunks WHERE room_id='" + roomId + "'");
}

// ================================================================
// Drafts
// ================================================================

bool SqliteStore::upsertDraft(const std::string& roomId, const std::string& userId,
    const std::string& draftJson) {
    std::ostringstream sql;
    sql << "INSERT OR REPLACE INTO drafts (room_id,user_id,draft_json) VALUES ('"
        << roomId << "','" << userId << "','" << draftJson << "')";
    return execSafe(sql.str());
}

std::string SqliteStore::getDraft(const std::string& roomId, const std::string& userId) {
    sqlite3_stmt* stmt = nullptr;
    std::string json;
    auto sql = "SELECT draft_json FROM drafts WHERE room_id='" + roomId + "' AND user_id='" + userId + "'";
    if (sqlite3_prepare_v2((sqlite3*)db_, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW && sqlite3_column_text(stmt, 0)) {
            json = (const char*)sqlite3_column_text(stmt, 0);
        }
    }
    sqlite3_finalize(stmt);
    return json;
}

bool SqliteStore::deleteDraft(const std::string& roomId, const std::string& userId) {
    return execSafe("DELETE FROM drafts WHERE room_id='" + roomId + "' AND user_id='" + userId + "'");
}

// ================================================================
// Read Receipts + Markers
// ================================================================

bool SqliteStore::upsertReadReceipt(const std::string& roomId, const std::string& eventId,
    const std::string& userId, int64_t timestamp) {
    std::ostringstream sql;
    sql << "INSERT OR REPLACE INTO read_receipts (room_id,event_id,user_id,timestamp_ms) VALUES ('"
        << roomId << "','" << eventId << "','" << userId << "'," << timestamp << ")";
    return execSafe(sql.str());
}

bool SqliteStore::upsertReadMarker(const std::string& roomId, const std::string& eventId) {
    std::ostringstream sql;
    sql << "INSERT OR REPLACE INTO read_markers (room_id,event_id) VALUES ('"
        << roomId << "','" << eventId << "')";
    return execSafe(sql.str());
}

// ================================================================
// Reactions
// ================================================================

bool SqliteStore::upsertReaction(const std::string& eventId, const std::string& key,
    const std::string& userId, int64_t timestamp) {
    std::ostringstream sql;
    sql << "INSERT OR REPLACE INTO reactions (event_id,reaction_key,user_id,timestamp_ms) VALUES ('"
        << eventId << "','" << key << "','" << userId << "'," << timestamp << ")";
    return execSafe(sql.str());
}

// ================================================================
// Push Rules
// ================================================================

bool SqliteStore::upsertPushRule(const std::string& ruleId, const std::string& kind,
    const std::string& actionsJson, const std::string& conditionsJson, bool enabled, bool isDefault) {
    std::ostringstream sql;
    sql << "INSERT OR REPLACE INTO push_rules (rule_id,kind,actions_json,conditions_json,enabled,is_default) VALUES ('"
        << ruleId << "','" << kind << "','" << actionsJson << "','" << conditionsJson << "',"
        << (enabled ? 1 : 0) << "," << (isDefault ? 1 : 0) << ")";
    return execSafe(sql.str());
}

std::string SqliteStore::getAllPushRulesJson() {
    std::ostringstream json;
    json << "[";
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2((sqlite3*)db_,
            "SELECT rule_id,kind,actions_json,conditions_json,enabled,is_default FROM push_rules",
            -1, &stmt, nullptr) == SQLITE_OK) {
        bool first = true;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            if (!first) json << ",";
            first = false;
            json << "{\"rule_id\":\"" << sqlite3_column_text(stmt, 0)
                 << "\",\"kind\":\"" << sqlite3_column_text(stmt, 1)
                 << "\",\"actions\":" << sqlite3_column_text(stmt, 2)
                 << ",\"conditions\":" << sqlite3_column_text(stmt, 3)
                 << ",\"enabled\":" << (sqlite3_column_int(stmt, 4) ? "true" : "false")
                 << ",\"default\":" << (sqlite3_column_int(stmt, 5) ? "true" : "false") << "}";
        }
    }
    sqlite3_finalize(stmt);
    json << "]";
    return json.str();
}

// ================================================================
// Filters
// ================================================================

bool SqliteStore::upsertFilter(const std::string& userId, const std::string& filterId,
    const std::string& filterJson) {
    std::ostringstream sql;
    sql << "INSERT OR REPLACE INTO filters (user_id,filter_id,filter_json) VALUES ('"
        << userId << "','" << filterId << "','" << filterJson << "')";
    return execSafe(sql.str());
}

std::string SqliteStore::getFilter(const std::string& userId, const std::string& filterId) {
    sqlite3_stmt* stmt = nullptr;
    std::string json;
    auto sql = "SELECT filter_json FROM filters WHERE user_id='" + userId + "' AND filter_id='" + filterId + "'";
    if (sqlite3_prepare_v2((sqlite3*)db_, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW && sqlite3_column_text(stmt, 0)) {
            json = (const char*)sqlite3_column_text(stmt, 0);
        }
    }
    sqlite3_finalize(stmt);
    return json;
}

// ================================================================
// Breadcrumbs
// ================================================================

bool SqliteStore::upsertBreadcrumb(const std::string& roomId, int index) {
    std::ostringstream sql;
    sql << "INSERT OR REPLACE INTO breadcrumbs (room_id,breadcrumb_index) VALUES ('"
        << roomId << "'," << index << ")";
    return execSafe(sql.str());
}

std::vector<std::string> SqliteStore::getBreadcrumbs() {
    std::vector<std::string> result;
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2((sqlite3*)db_,
            "SELECT room_id FROM breadcrumbs ORDER BY breadcrumb_index",
            -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            result.push_back((const char*)sqlite3_column_text(stmt, 0));
        }
    }
    sqlite3_finalize(stmt);
    return result;
}

bool SqliteStore::deleteBreadcrumb(const std::string& roomId) {
    return execSafe("DELETE FROM breadcrumbs WHERE room_id='" + roomId + "'");
}

// ================================================================
// Home Server Capabilities
// ================================================================

bool SqliteStore::upsertHomeServerCapabilities(const std::string& capabilitiesJson) {
    std::ostringstream sql;
    sql << "INSERT OR REPLACE INTO homeserver_capabilities (id,capabilities_json) VALUES (1,'"
        << capabilitiesJson << "')";
    return execSafe(sql.str());
}

std::string SqliteStore::getHomeServerCapabilities() {
    sqlite3_stmt* stmt = nullptr;
    std::string json;
    if (sqlite3_prepare_v2((sqlite3*)db_,
            "SELECT capabilities_json FROM homeserver_capabilities WHERE id=1",
            -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW && sqlite3_column_text(stmt, 0)) {
            json = (const char*)sqlite3_column_text(stmt, 0);
        }
    }
    sqlite3_finalize(stmt);
    return json;
}

// ================================================================
// Ignored Users
// ================================================================

bool SqliteStore::upsertIgnoredUser(const std::string& userId) {
    return execSafe("INSERT OR IGNORE INTO ignored_users (user_id) VALUES ('" + userId + "')");
}

bool SqliteStore::deleteIgnoredUser(const std::string& userId) {
    return execSafe("DELETE FROM ignored_users WHERE user_id='" + userId + "'");
}

std::vector<std::string> SqliteStore::getIgnoredUsers() {
    std::vector<std::string> result;
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2((sqlite3*)db_,
            "SELECT user_id FROM ignored_users", -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            result.push_back((const char*)sqlite3_column_text(stmt, 0));
        }
    }
    sqlite3_finalize(stmt);
    return result;
}

bool SqliteStore::isIgnored(const std::string& userId) {
    sqlite3_stmt* stmt = nullptr;
    bool ignored = false;
    auto sql = "SELECT 1 FROM ignored_users WHERE user_id='" + userId + "'";
    if (sqlite3_prepare_v2((sqlite3*)db_, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        ignored = sqlite3_step(stmt) == SQLITE_ROW;
    }
    sqlite3_finalize(stmt);
    return ignored;
}

// ================================================================
// Preview URL Cache
// ================================================================

bool SqliteStore::upsertPreviewUrlCache(const std::string& url, const std::string& dataJson,
    int64_t timestamp) {
    std::ostringstream sql;
    sql << "INSERT OR REPLACE INTO preview_url_cache (url,data_json,cached_at_ms) VALUES ('"
        << url << "','" << dataJson << "'," << timestamp << ")";
    return execSafe(sql.str());
}

std::string SqliteStore::getPreviewUrlCache(const std::string& url, int64_t maxAgeMs) {
    sqlite3_stmt* stmt = nullptr;
    std::string json;
    auto sql = "SELECT data_json FROM preview_url_cache WHERE url='" + url + "'";
    if (sqlite3_prepare_v2((sqlite3*)db_, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW && sqlite3_column_text(stmt, 0)) {
            json = (const char*)sqlite3_column_text(stmt, 0);
        }
    }
    sqlite3_finalize(stmt);
    return json;
}

// ================================================================
// Sync State
// ================================================================

bool SqliteStore::upsertSyncState(const std::string& syncToken, int64_t lastSyncMs,
    const std::string& filterId) {
    std::ostringstream sql;
    sql << "INSERT OR REPLACE INTO sync_state (id,sync_token,last_sync_ms,filter_id) VALUES (1,'"
        << syncToken << "'," << lastSyncMs << ",'" << filterId << "')";
    return execSafe(sql.str());
}

std::string SqliteStore::getSyncStateJson() {
    sqlite3_stmt* stmt = nullptr;
    std::string result = "{}";
    if (sqlite3_prepare_v2((sqlite3*)db_,
            "SELECT sync_token,last_sync_ms,filter_id FROM sync_state WHERE id=1",
            -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            std::ostringstream json;
            json << "{\"sync_token\":\"" << ((const char*)sqlite3_column_text(stmt, 0) ? (const char*)sqlite3_column_text(stmt, 0) : "")
                 << "\",\"last_sync_ms\":" << sqlite3_column_int64(stmt, 1)
                 << ",\"filter_id\":\"" << ((const char*)sqlite3_column_text(stmt, 2) ? (const char*)sqlite3_column_text(stmt, 2) : "")
                 << "\"}";
            result = json.str();
        }
    }
    sqlite3_finalize(stmt);
    return result;
}

// ================================================================
// Space Hierarchy
// ================================================================

bool SqliteStore::upsertSpaceChild(const std::string& parentId, const std::string& childId,
    const std::string& order, bool suggested, const std::string& viaServersJson) {
    std::ostringstream sql;
    sql << "INSERT OR REPLACE INTO space_children (parent_id,child_id,sort_order,suggested,via_servers_json) VALUES ('"
        << parentId << "','" << childId << "','" << order << "'," << (suggested ? 1 : 0) << ",'"
        << viaServersJson << "')";
    return execSafe(sql.str());
}

bool SqliteStore::upsertSpaceParent(const std::string& childId, const std::string& parentId,
    bool canonical, const std::string& viaServersJson) {
    std::ostringstream sql;
    sql << "INSERT OR REPLACE INTO space_parents (child_id,parent_id,canonical,via_servers_json) VALUES ('"
        << childId << "','" << parentId << "'," << (canonical ? 1 : 0) << ",'"
        << viaServersJson << "')";
    return execSafe(sql.str());
}

// ================================================================
// Account Data
// ================================================================

bool SqliteStore::upsertAccountData(const std::string& userId, const std::string& roomId,
    const std::string& type, const std::string& contentJson) {
    std::ostringstream sql;
    sql << "INSERT OR REPLACE INTO account_data (user_id,room_id,type,content_json) VALUES ('"
        << userId << "','" << roomId << "','" << type << "','" << contentJson << "')";
    return execSafe(sql.str());
}

std::string SqliteStore::getAccountDataJson(const std::string& userId, const std::string& roomId,
    const std::string& type) {
    sqlite3_stmt* stmt = nullptr;
    std::string json;
    auto sql = "SELECT content_json FROM account_data WHERE user_id='" + userId
        + "' AND room_id='" + roomId + "' AND type='" + type + "'";
    if (sqlite3_prepare_v2((sqlite3*)db_, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW && sqlite3_column_text(stmt, 0)) {
            json = (const char*)sqlite3_column_text(stmt, 0);
        }
    }
    sqlite3_finalize(stmt);
    return json;
}

// ================================================================
// 3PID
// ================================================================

bool SqliteStore::upsertThreePid(const std::string& userId, const std::string& medium,
    const std::string& address, bool validated) {
    std::ostringstream sql;
    sql << "INSERT OR REPLACE INTO three_pids (user_id,medium,address,validated) VALUES ('"
        << userId << "','" << medium << "','" << address << "'," << (validated ? 1 : 0) << ")";
    return execSafe(sql.str());
}

// ================================================================
// Pushers
// ================================================================

bool SqliteStore::upsertPusher(const std::string& pushKey, const std::string& appId,
    const std::string& appDisplayName, const std::string& deviceDisplayName,
    const std::string& lang, const std::string& dataJson, const std::string& profileTag) {
    std::ostringstream sql;
    sql << "INSERT OR REPLACE INTO pushers (push_key,app_id,app_display_name,device_display_name,lang,data_json,profile_tag) VALUES ('"
        << pushKey << "','" << appId << "','" << appDisplayName << "','" << deviceDisplayName << "','"
        << lang << "','" << dataJson << "','" << profileTag << "')";
    return execSafe(sql.str());
}

// ================================================================
// Bulk Operations
// ================================================================

bool SqliteStore::clearRoom(const std::string& roomId) {
    beginTransaction();
    execSafe("DELETE FROM events WHERE room_id='" + roomId + "'");
    execSafe("DELETE FROM chunks WHERE room_id='" + roomId + "'");
    execSafe("DELETE FROM room_tags WHERE room_id='" + roomId + "'");
    execSafe("DELETE FROM room_members WHERE room_id='" + roomId + "'");
    execSafe("DELETE FROM drafts WHERE room_id='" + roomId + "'");
    execSafe("DELETE FROM read_receipts WHERE room_id='" + roomId + "'");
    execSafe("DELETE FROM read_markers WHERE room_id='" + roomId + "'");
    execSafe("DELETE FROM breadcrumbs WHERE room_id='" + roomId + "'");
    execSafe("DELETE FROM account_data WHERE room_id='" + roomId + "'");
    execSafe("DELETE FROM space_children WHERE parent_id='" + roomId + "' OR child_id='" + roomId + "'");
    execSafe("DELETE FROM space_parents WHERE parent_id='" + roomId + "' OR child_id='" + roomId + "'");
    deleteRoomSummary(roomId);
    commitTransaction();
    return true;
}

bool SqliteStore::clearAll() {
    if (!db_) return false;
    beginTransaction();
    exec("DELETE FROM events");
    exec("DELETE FROM room_summaries");
    exec("DELETE FROM chunks");
    exec("DELETE FROM room_tags");
    exec("DELETE FROM room_members");
    exec("DELETE FROM drafts");
    exec("DELETE FROM read_receipts");
    exec("DELETE FROM read_markers");
    exec("DELETE FROM reactions");
    exec("DELETE FROM push_rules");
    exec("DELETE FROM filters");
    exec("DELETE FROM breadcrumbs");
    exec("DELETE FROM homeserver_capabilities");
    exec("DELETE FROM ignored_users");
    exec("DELETE FROM preview_url_cache");
    exec("DELETE FROM sync_state");
    exec("DELETE FROM space_children");
    exec("DELETE FROM space_parents");
    exec("DELETE FROM account_data");
    exec("DELETE FROM three_pids");
    exec("DELETE FROM pushers");
    commitTransaction();
    return true;
}

std::string SqliteStore::exportJson() {
    // Stub: export tables as JSON for debugging
    return "{\"ok\":true}";
}

} // namespace pcp
