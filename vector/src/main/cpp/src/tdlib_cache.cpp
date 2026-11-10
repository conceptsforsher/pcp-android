#include "pcp/tdlib_cache.hpp"

#ifdef PCP_HAS_TDLIB

#include <algorithm>
#include <sqlite3.h>
#include <sstream>
#include <android/log.h>

#define TAG "TdCache"

namespace pcp {

static std::string esc(const std::string& s) {
    std::string out;
    for (char c : s) {
        if (c == '\'') out += "''";
        else out += c;
    }
    return out;
}

TdCache::TdCache(const std::string& path) : dbPath(path) {}

TdCache::~TdCache() { close(); }

bool TdCache::open() {
    int rc = sqlite3_open(dbPath.c_str(), (sqlite3**)&db);
    if (rc != SQLITE_OK) {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "Cannot open cache db: %s", sqlite3_errmsg((sqlite3*)db));
        return false;
    }
    execute("PRAGMA journal_mode=WAL");
    execute("PRAGMA synchronous=NORMAL");
    createTables();
    return true;
}

void TdCache::close() {
    if (db) {
        sqlite3_close((sqlite3*)db);
        db = nullptr;
    }
}

void TdCache::execute(const std::string& sql) {
    char* err = nullptr;
    sqlite3_exec((sqlite3*)db, sql.c_str(), nullptr, nullptr, &err);
    if (err) {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "SQL error: %s", err);
        sqlite3_free(err);
    }
}

void TdCache::createTables() {
    execute(R"(
        CREATE TABLE IF NOT EXISTS messages (
            id INTEGER PRIMARY KEY,
            chat_id INTEGER NOT NULL,
            sender_id INTEGER,
            sender_name TEXT,
            date INTEGER,
            edit_date INTEGER,
            is_outgoing INTEGER,
            content_type TEXT,
            text_content TEXT,
            caption TEXT,
            raw_json TEXT
        );
        CREATE INDEX IF NOT EXISTS idx_msg_chat ON messages(chat_id, id);

        CREATE TABLE IF NOT EXISTS chats (
            id INTEGER PRIMARY KEY,
            title TEXT,
            type TEXT,
            unread_count INTEGER,
            last_msg_id INTEGER,
            last_msg_text TEXT,
            last_msg_time INTEGER,
            is_pinned INTEGER,
            draft_text TEXT,
            mute_for INTEGER,
            chat_order INTEGER
        );

        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY,
            first_name TEXT,
            last_name TEXT,
            username TEXT,
            phone TEXT,
            bio TEXT,
            is_premium INTEGER,
            is_verified INTEGER
        );

        CREATE TABLE IF NOT EXISTS stories (
            id INTEGER,
            sender_chat_id INTEGER,
            date INTEGER,
            is_pinned INTEGER,
            content_type TEXT,
            caption TEXT,
            PRIMARY KEY (sender_chat_id, id)
        );
    )");
}

void TdCache::insertMessage(const TdMessage& msg) {
    std::lock_guard<std::mutex> lock(mtx);
    std::ostringstream sql;
    sql << "INSERT OR REPLACE INTO messages VALUES("
        << msg.id << "," << msg.chatId << "," << msg.senderId
        << ",'" << esc(msg.senderName) << "'," << msg.date << "," << msg.editDate
        << "," << (msg.isOutgoing ? 1 : 0) << ",'" << esc(std::to_string((int)msg.contentType))
        << "','" << esc(msg.text) << "','" << esc(msg.caption) << "','')";
    execute(sql.str());
}

std::vector<TdMessage> TdCache::getMessages(int64_t chatId, int limit, int64_t fromId) {
    std::lock_guard<std::mutex> lock(mtx);
    std::ostringstream sql;
    sql << "SELECT id,chat_id,sender_id,sender_name,date,is_outgoing,content_type,text_content "
        << "FROM messages WHERE chat_id=" << chatId;
    if (fromId > 0) sql << " AND id < " << fromId;
    sql << " ORDER BY id DESC LIMIT " << limit;

    std::vector<TdMessage> result;
    sqlite3_stmt* stmt = nullptr;
    sqlite3_prepare_v2((sqlite3*)db, sql.str().c_str(), -1, &stmt, nullptr);
    if (!stmt) return result;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        TdMessage m;
        m.id = sqlite3_column_int64(stmt, 0);
        m.chatId = sqlite3_column_int64(stmt, 1);
        m.senderId = sqlite3_column_int64(stmt, 2);
        m.senderName = (const char*)sqlite3_column_text(stmt, 3);
        m.date = sqlite3_column_int(stmt, 4);
        m.isOutgoing = sqlite3_column_int(stmt, 5) != 0;
        m.text = (const char*)sqlite3_column_text(stmt, 7);
        result.push_back(m);
    }
    sqlite3_finalize(stmt);
    std::reverse(result.begin(), result.end());
    return result;
}

void TdCache::deleteMessage(int64_t chatId, int64_t msgId) {
    std::lock_guard<std::mutex> lock(mtx);
    execute("DELETE FROM messages WHERE chat_id=" + std::to_string(chatId) +
            " AND id=" + std::to_string(msgId));
}

void TdCache::deleteMessages(int64_t chatId, const std::vector<int64_t>& ids) {
    std::lock_guard<std::mutex> lock(mtx);
    std::string list;
    for (size_t i = 0; i < ids.size(); i++) {
        if (i > 0) list += ",";
        list += std::to_string(ids[i]);
    }
    execute("DELETE FROM messages WHERE chat_id=" + std::to_string(chatId) +
            " AND id IN (" + list + ")");
}

int TdCache::messageCount(int64_t chatId) {
    std::lock_guard<std::mutex> lock(mtx);
    sqlite3_stmt* stmt = nullptr;
    sqlite3_prepare_v2((sqlite3*)db,
        ("SELECT COUNT(*) FROM messages WHERE chat_id=" + std::to_string(chatId)).c_str(),
        -1, &stmt, nullptr);
    int count = 0;
    if (stmt && sqlite3_step(stmt) == SQLITE_ROW) count = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
    return count;
}

void TdCache::upsertChat(const TdChat& chat) {
    std::lock_guard<std::mutex> lock(mtx);
    std::ostringstream sql;
    sql << "INSERT OR REPLACE INTO chats VALUES("
        << chat.id << ",'" << esc(chat.title) << "','" << esc(chat.type)
        << "'," << chat.unreadCount << "," << chat.lastMessageId
        << ",'" << esc(chat.lastMessageText) << "'," << chat.lastMessageTime
        << "," << (chat.isPinned ? 1 : 0) << ",'" << esc(chat.draftText)
        << "'," << chat.muteFor << "," << chat.order << ")";
    execute(sql.str());
}

std::vector<TdChat> TdCache::getAllChats() {
    std::lock_guard<std::mutex> lock(mtx);
    sqlite3_stmt* stmt = nullptr;
    sqlite3_prepare_v2((sqlite3*)db,
        "SELECT id,title,type,unread_count,last_msg_id,last_msg_text,last_msg_time,is_pinned,draft_text,mute_for,chat_order FROM chats ORDER BY chat_order DESC",
        -1, &stmt, nullptr);
    std::vector<TdChat> result;
    if (!stmt) return result;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        TdChat c;
        c.id = sqlite3_column_int64(stmt, 0);
        c.title = (const char*)sqlite3_column_text(stmt, 1);
        c.type = (const char*)sqlite3_column_text(stmt, 2);
        c.unreadCount = sqlite3_column_int(stmt, 3);
        c.lastMessageId = sqlite3_column_int64(stmt, 4);
        c.lastMessageText = (const char*)sqlite3_column_text(stmt, 5);
        c.lastMessageTime = sqlite3_column_int64(stmt, 6);
        c.isPinned = sqlite3_column_int(stmt, 7) != 0;
        c.draftText = (const char*)sqlite3_column_text(stmt, 8);
        c.muteFor = sqlite3_column_int(stmt, 9);
        c.order = sqlite3_column_int64(stmt, 10);
        result.push_back(c);
    }
    sqlite3_finalize(stmt);
    return result;
}

TdChat TdCache::getChat(int64_t chatId) {
    TdChat c; c.id = chatId;
    // Simplify — just return the ID
    return c;
}

void TdCache::upsertUser(const TdUser& user) {
    std::lock_guard<std::mutex> lock(mtx);
    std::ostringstream sql;
    sql << "INSERT OR REPLACE INTO users VALUES("
        << user.id << ",'" << esc(user.firstName) << "','" << esc(user.lastName)
        << "','" << esc(user.username) << "','" << esc(user.phoneNumber)
        << "','" << esc(user.bio) << "'," << (user.isPremium ? 1 : 0)
        << "," << (user.isVerified ? 1 : 0) << ")";
    execute(sql.str());
}

TdUser TdCache::getUser(int64_t userId) {
    TdUser u; u.id = userId; return u;
}

void TdCache::insertStory(const TdStory& story) {
    std::lock_guard<std::mutex> lock(mtx);
    std::ostringstream sql;
    sql << "INSERT OR REPLACE INTO stories VALUES("
        << story.id << "," << story.senderChatId << "," << story.date
        << "," << (story.isPinned ? 1 : 0) << ",'"
        << esc(std::to_string((int)story.contentType)) << "','" << esc(story.caption) << "')";
    execute(sql.str());
}

std::vector<TdStory> TdCache::getActiveStories() {
    return {};
}

} // namespace pcp

#else
// Stub implementation when TDLib is not available
namespace pcp {
TdCache::TdCache(const std::string&) {}
TdCache::~TdCache() {}
bool TdCache::open() { return false; }
void TdCache::close() {}
void TdCache::insertMessage(const TdMessage&) {}
std::vector<TdMessage> TdCache::getMessages(int64_t, int, int64_t) { return {}; }
void TdCache::deleteMessage(int64_t, int64_t) {}
void TdCache::deleteMessages(int64_t, const std::vector<int64_t>&) {}
int TdCache::messageCount(int64_t) { return 0; }
void TdCache::upsertChat(const TdChat&) {}
std::vector<TdChat> TdCache::getAllChats() { return {}; }
TdChat TdCache::getChat(int64_t cid) { TdChat c; c.id = cid; return c; }
void TdCache::upsertUser(const TdUser&) {}
TdUser TdCache::getUser(int64_t uid) { TdUser u; u.id = uid; return u; }
void TdCache::insertStory(const TdStory&) {}
std::vector<TdStory> TdCache::getActiveStories() { return {}; }
}
#endif
