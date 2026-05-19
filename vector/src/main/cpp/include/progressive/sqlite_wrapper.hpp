#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <utility>

namespace progressive {

// ==== SQLite Wrapper — Native Database Layer ====
//
// Thin C++ wrapper around SQLite for Matrix data storage.
// Replaces Realm in the progressive native timeline engine.
// Uses NDK's built-in SQLite (android.database.sqlite or standalone).
// Opt-in via Labs: SETTINGS_LABS_NATIVE_TIMELINE

// ==== Database Handle ====

class SqliteDB {
public:
    // Open or create a database file.
    static SqliteDB open(const std::string& path);

    // Move semantics (unique ownership of DB handle)
    SqliteDB(SqliteDB&& other) noexcept : db_(other.db_) { other.db_ = nullptr; }
    SqliteDB& operator=(SqliteDB&& other) noexcept {
        if (this != &other) {
            std::swap(db_, other.db_);
        }
        return *this;
    }

    // Non-copyable
    SqliteDB(const SqliteDB&) = delete;
    SqliteDB& operator=(const SqliteDB&) = delete;

    // Close and clean up.
    ~SqliteDB();

    bool isOpen() const { return db_ != nullptr; }

    // Execute a SQL statement (no result rows).
    // Returns true on success.
    bool execute(const std::string& sql);

    // ==== Schema Management ====

    // Create the timeline events table if not exists.
    bool createTimelineSchema();

    // Get the database version (user_version pragma).
    int schemaVersion();

    // Set the database version.
    void setSchemaVersion(int version);

    // ==== Event Storage ====

    // Insert or replace a timeline event.
    bool insertEvent(
        const std::string& eventId,
        const std::string& roomId,
        const std::string& type,
        const std::string& senderId,
        const std::string& contentJson,
        int64_t originServerTs,
        int64_t ageLocalTs,
        int displayIndex,
        const std::string& stateKey = "",
        const std::string& redacts = "",
        const std::string& relationType = "",
        const std::string& relatesToId = ""
    );

    // Delete an event by ID.
    bool deleteEvent(const std::string& eventId);

    // ==== Event Querying ====

    // Query events for a room, ordered by displayIndex.
    // limit: max events to return (0 = all)
    // offset: skip first N events
    struct EventRow {
        std::string eventId, roomId, type, senderId, contentJson;
        int64_t originServerTs, ageLocalTs;
        int displayIndex;
        std::string stateKey, redacts, relationType, relatesToId;
    };

    std::vector<EventRow> queryEvents(
        const std::string& roomId,
        int limit = 0,
        int offset = 0,
        bool ascending = true
    );

    // Find an event by ID.
    EventRow queryEvent(const std::string& eventId);

    // Count events in a room.
    int countEvents(const std::string& roomId);

    // Get the last display index in a room.
    int maxDisplayIndex(const std::string& roomId);

    // ==== Room Storage ====

    // Insert or update a room summary.
    bool upsertRoom(
        const std::string& roomId,
        const std::string& displayName,
        const std::string& avatarUrl,
        const std::string& topic,
        const std::string& membership,
        int notificationCount,
        int highlightCount,
        int64_t lastActivityMs,
        bool isDirect,
        bool isSpace,
        bool isFavourite,
        bool isEncrypted
    );

    // Query all rooms (for room list).
    struct RoomRow {
        std::string roomId, displayName, avatarUrl, topic, membership;
        int notificationCount, highlightCount;
        int64_t lastActivityMs;
        bool isDirect, isSpace, isFavourite, isEncrypted;
    };

    std::vector<RoomRow> queryRooms();

    // ==== Transaction Support ====

    void beginTransaction();
    void commitTransaction();
    void rollbackTransaction();

private:
    void* db_ = nullptr; // sqlite3* handle
    SqliteDB() = default;
};

// ==== SQL Builder Helpers ====

// Escape a string for SQL (single quotes → doubled).
std::string sqlEscape(const std::string& s);

// Build an INSERT OR REPLACE statement.
std::string buildInsertSql(
    const std::string& table,
    const std::vector<std::string>& columns,
    const std::vector<std::string>& values
);

} // namespace progressive
