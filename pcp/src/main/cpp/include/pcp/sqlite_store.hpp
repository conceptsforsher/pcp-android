#pragma once
// ============================================================================
// sqlite_store.hpp — Full SQLite replacement for Kotlin Realm database
//
// Replaces ALL 47 Realm entity classes + 57 migration files + 30 query files.
// Tables mirror the Realm schema exactly for drop-in compatibility via JNI.
// ============================================================================

#include <string>
#include <vector>
#include <cstdint>
#include <functional>

struct sqlite3;
struct sqlite3_stmt;

namespace pcp {

// ==== Forward declarations ====
struct DbTimelineEvent;
struct DbRoomSummary;
struct DbChunk;
struct DbDraft;

// ============================================================================
// Full SQLite Store — replaces RealmSessionStoreMigration + all entities
// ============================================================================

class SqliteStore {
public:
    SqliteStore();
    ~SqliteStore();

    // Open/create database. Returns true on success.
    bool open(const std::string& dbPath, const std::string& key = "");

    // Close and clean up.
    void close();

    // ================================================================
    // Schema Management (replaces 57 MigrateSessionTo*.kt files)
    // ================================================================

    // Create ALL tables (idempotent — uses IF NOT EXISTS).
    bool createSchema();

    // Get current schema version (user_version pragma).
    int schemaVersion();

    // Migrate from current version to latest.
    // Returns true if migration succeeded or already at latest.
    bool migrateToLatest();

    // ================================================================
    // Transaction Support
    // ================================================================
    void beginTransaction();
    void commitTransaction();
    void rollbackTransaction();

    // ================================================================
    // Room Summary (replaces RoomSummaryEntity + RoomEntity)
    // ================================================================

    bool upsertRoomSummary(const DbRoomSummary& summary);
    DbRoomSummary getRoomSummary(const std::string& roomId);
    std::vector<DbRoomSummary> getAllRoomSummaries();
    bool deleteRoomSummary(const std::string& roomId);

    // Room tags (replaces RoomTagEntity)
    bool upsertRoomTag(const std::string& roomId, const std::string& tagName, double order);
    bool deleteRoomTag(const std::string& roomId, const std::string& tagName);
    std::vector<std::pair<std::string,double>> getRoomTags(const std::string& roomId);

    // Room member summaries (replaces RoomMemberSummaryEntity)
    bool upsertRoomMember(const std::string& roomId, const std::string& userId,
                          const std::string& displayName, const std::string& avatarUrl,
                          const std::string& membership);
    std::string getRoomMembersJson(const std::string& roomId);

    // ================================================================
    // Timeline Events (replaces EventEntity + TimelineEventEntity + ChunkEntity)
    // ================================================================

    bool insertEvent(const DbTimelineEvent& event);
    bool insertEvents(const std::vector<DbTimelineEvent>& events);
    bool deleteEvent(const std::string& eventId);
    DbTimelineEvent getEvent(const std::string& eventId);

    // Query events for a room with pagination
    std::vector<DbTimelineEvent> queryEvents(const std::string& roomId,
        int limit = 0, int offset = 0, bool ascending = true);

    // Count events in a room
    int countEvents(const std::string& roomId);

    // Get max display index
    int maxDisplayIndex(const std::string& roomId);

    // ================================================================
    // Chunks (replaces ChunkEntity + LoadTimelineStrategy)
    // ================================================================

    bool upsertChunk(const DbChunk& chunk);
    DbChunk getChunk(const std::string& roomId, int chunkIndex);
    std::vector<DbChunk> getChunks(const std::string& roomId);
    bool deleteChunks(const std::string& roomId);

    // ================================================================
    // Drafts (replaces DraftEntity + UserDraftsEntity)
    // ================================================================

    bool upsertDraft(const std::string& roomId, const std::string& userId,
                     const std::string& draftJson);
    std::string getDraft(const std::string& roomId, const std::string& userId);
    bool deleteDraft(const std::string& roomId, const std::string& userId);

    // ================================================================
    // Read Receipts + Read Markers (replaces ReadReceiptEntity, ReadMarkerEntity)
    // ================================================================

    bool upsertReadReceipt(const std::string& roomId, const std::string& eventId,
                           const std::string& userId, int64_t timestamp);
    bool upsertReadMarker(const std::string& roomId, const std::string& eventId);

    // ================================================================
    // Event Annotations (replaces EventAnnotationsSummaryEntity + children)
    // ================================================================

    bool upsertReaction(const std::string& eventId, const std::string& key,
                        const std::string& userId, int64_t timestamp);

    // ================================================================
    // Push Rules (replaces PushRuleEntity + PushRulesEntity + PushConditionEntity)
    // ================================================================

    bool upsertPushRule(const std::string& ruleId, const std::string& kind,
                        const std::string& actionsJson, const std::string& conditionsJson,
                        bool enabled, bool isDefault);
    std::string getAllPushRulesJson();

    // ================================================================
    // Filters (replaces FilterEntity + SyncFilterParamsEntity)
    // ================================================================

    bool upsertFilter(const std::string& userId, const std::string& filterId,
                      const std::string& filterJson);
    std::string getFilter(const std::string& userId, const std::string& filterId);

    // ================================================================
    // Breadcrumbs (replaces BreadcrumbsEntity)
    // ================================================================

    bool upsertBreadcrumb(const std::string& roomId, int index);
    std::vector<std::string> getBreadcrumbs();
    bool deleteBreadcrumb(const std::string& roomId);

    // ================================================================
    // Home Server Capabilities (replaces HomeServerCapabilitiesEntity)
    // ================================================================

    bool upsertHomeServerCapabilities(const std::string& capabilitiesJson);
    std::string getHomeServerCapabilities();

    // ================================================================
    // Ignored Users (replaces IgnoredUserEntity)
    // ================================================================

    bool upsertIgnoredUser(const std::string& userId);
    bool deleteIgnoredUser(const std::string& userId);
    std::vector<std::string> getIgnoredUsers();
    bool isIgnored(const std::string& userId);

    // ================================================================
    // Preview URL Cache (replaces PreviewUrlCacheEntity)
    // ================================================================

    bool upsertPreviewUrlCache(const std::string& url, const std::string& dataJson,
                               int64_t timestamp);
    std::string getPreviewUrlCache(const std::string& url, int64_t maxAgeMs = 86400000);

    // ================================================================
    // Sync State (replaces SyncEntity)
    // ================================================================

    bool upsertSyncState(const std::string& syncToken, int64_t lastSyncMs,
                         const std::string& filterId);
    std::string getSyncStateJson();

    // ================================================================
    // Space Hierarchy (replaces SpaceChildSummaryEntity + SpaceParentSummaryEntity)
    // ================================================================

    bool upsertSpaceChild(const std::string& parentId, const std::string& childId,
                          const std::string& order, bool suggested,
                          const std::string& viaServersJson);
    bool upsertSpaceParent(const std::string& childId, const std::string& parentId,
                           bool canonical, const std::string& viaServersJson);

    // ================================================================
    // User Account Data (replaces UserAccountDataEntity + RoomAccountDataEntity)
    // ================================================================

    bool upsertAccountData(const std::string& userId, const std::string& roomId,
                           const std::string& type, const std::string& contentJson);
    std::string getAccountDataJson(const std::string& userId, const std::string& roomId,
                                   const std::string& type);

    // ================================================================
    // 3PID (replaces PendingThreePidEntity + UserThreePidEntity)
    // ================================================================

    bool upsertThreePid(const std::string& userId, const std::string& medium,
                        const std::string& address, bool validated);

    // ================================================================
    // Pushers (replaces PusherEntity + PusherDataEntity)
    // ================================================================

    bool upsertPusher(const std::string& pushKey, const std::string& appId,
                      const std::string& appDisplayName, const std::string& deviceDisplayName,
                      const std::string& lang, const std::string& dataJson,
                      const std::string& profileTag);

    // ================================================================
    // Bulk Operations (for sync response processing)
    // ================================================================

    // Clear all data for a room (cascading delete)
    bool clearRoom(const std::string& roomId);

    // Clear entire database
    bool clearAll();

    // Export full database as JSON (for debugging/backup)
    std::string exportJson();

private:
    void* db_ = nullptr;  // sqlite3*

    // Helpers
    void exec(const std::string& sql);
    bool execSafe(const std::string& sql);
    bool tableExists(const std::string& name);

    // Migration steps (one per schema version)
    bool migrateV1();  // initial schema
    bool migrateV2();  // add thread support
    bool migrateV3();  // add space hierarchy
    bool migrateV4();  // add polls + location sharing
    bool migrateV5();  // add push rules
};

// ============================================================================
// Data Structs (mirroring Realm entities)
// ============================================================================

struct DbTimelineEvent {
    std::string eventId;
    std::string roomId;
    std::string type;              // EventType
    std::string stateKey;
    std::string senderId;
    std::string contentJson;
    std::string prevContentJson;
    std::string unsignedDataJson;
    std::string redacts;
    std::string decryptionResultJson;
    int64_t originServerTs = 0;
    int64_t ageLocalTs = 0;
    int displayIndex = 0;
    bool isRootThread = false;
    std::string rootThreadEventId;
    int numberOfThreads = 0;
    std::string threadSummaryJson;
    std::string sendState;         // UNSENT/SENDING/SENT/etc.
    std::string sendStateDetailsJson;
    bool sentByMe = false;
    bool isEncrypted = false;
    // Relation fields
    std::string relationType;
    std::string relatesToId;
    std::string relationKey;

    // Convenience
    bool hasRelation() const { return !relationType.empty(); }
    bool isReply() const { return relationType == "m.in_reply_to"; }
    bool isEdit() const { return relationType == "m.replace"; }
    bool isReaction() const { return relationType == "m.annotation"; }
};

struct DbRoomSummary {
    std::string roomId;
    std::string roomType;          // "m.space" or empty
    std::string displayName;
    std::string normalizedDisplayName;
    std::string avatarUrl;
    std::string name;              // m.room.name
    std::string topic;
    std::string canonicalAlias;
    std::string flatAliases;       // pipe-joined
    std::string membership;        // NONE/INVITE/JOIN/LEAVE/BAN
    std::string joinRules;         // PUBLIC/KNOCK/INVITE/PRIVATE
    std::string versioningState;   // NONE/UPGRADING_ROOM/UPGRADED_UPSTREAM

    int joinedMembersCount = 0;
    int invitedMembersCount = 0;

    bool isDirect = false;
    std::string directUserId;

    int notificationCount = 0;
    int highlightCount = 0;
    int threadNotificationCount = 0;
    int threadHighlightCount = 0;
    bool hasUnreadMessages = false;

    bool isEncrypted = false;
    std::string e2eAlgorithm;
    int64_t encryptionEventTs = 0;
    std::string roomEncryptionTrustLevelStr;

    bool isFavourite = false;
    bool isLowPriority = false;
    bool isServerNotice = false;

    int64_t lastActivityTime = 0;

    std::string readMarkerId;
    std::string inviterId;
    int breadcrumbsIndex = -1;
    bool hasFailedSending = false;
    bool isHiddenFromUser = false;

    std::string flattenParentIds;  // pipe-joined
    std::string directParentNamesJson;

    std::string heroesJson;        // JSON array of userIds
    std::string otherMemberIdsJson;

    // Latest previewable event
    std::string latestEventId;
    std::string latestEventJson;

    // Draft
    std::string draftJson;
};

struct DbChunk {
    std::string roomId;
    int chunkIndex = 0;
    std::string prevBatch;         // pagination token for "before"
    std::string nextBatch;         // pagination token for "after"
    bool isLastForward = true;
    bool isLastBackward = false;
    int numberOfEvents = 0;

    // Serialize chunk boundaries
    std::string toString() const {
        return roomId + "|" + std::to_string(chunkIndex);
    }
};

// ============================================================================
// Built-in schema version history (maps to Realm migration files)
// ============================================================================

constexpr int SQLITE_STORE_SCHEMA_VERSION = 5;

} // namespace pcp
