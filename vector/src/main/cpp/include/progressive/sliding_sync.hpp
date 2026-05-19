#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>

namespace progressive {

// ==== Sliding Sync (MSC3575) Protocol Implementation ====
//
// Native C++ implementation of the Sliding Sync protocol.
// Replaces the Rust SDK's Simplified Sliding Sync used by Element X.
// 
// Ref: https://github.com/matrix-org/matrix-spec-proposals/pull/3575
//
// Protocol overview:
//   Client sends a SlidingSyncRequest with:
//     - lists: named sliding windows with ranges and sort orders
//     - room_subscriptions: rooms needing live timeline data
//     - pos: sync token (empty for initial sync)
//     - timeout: long-poll duration
//   Server responds with:
//     - pos: new sync token
//     - lists: per-list operations (SYNC, INSERT, DELETE, INVALIDATE)
//     - rooms: room data (name, avatar, unread, timeline, etc.)
//     - extensions: e2ee, to-device, account_data

// ==== Configuration ====

// Sort order for sliding sync lists
enum class SlidingSyncSort {
    BY_RECENCY = 0,              // "by_recency"
    BY_NAME = 1,                 // "by_name"
    BY_NOTIFICATION_LEVEL = 2,   // "by_notification_level"
    BY_PRIORITY = 3              // "by_priority" (favorites first)
};

// Required state to include per room
struct SlidingSyncRequiredState {
    std::string eventType;       // "m.room.name", "m.room.avatar", etc.
    std::string stateKey;        // "" for default, user_id for member events
};

// A single sliding window list config
struct SlidingSyncList {
    std::string name;                        // "rooms", "people", "spaces"
    int rangeStart = 0;                      // Start of visible range
    int rangeEnd = 19;                       // End of visible range (PAGE_SIZE-1)
    std::vector<SlidingSyncSort> sort;       // Sort order chain
    std::vector<SlidingSyncRequiredState> requiredState;
    bool includeOldRooms = false;            // Include left/archived rooms

    // Build JSON for this list entry
    std::string toJson() const;
};

// ==== Request ====

struct SlidingSyncRequest {
    std::vector<SlidingSyncList> lists;           // Named sliding window lists
    std::vector<std::string> roomSubscriptions;   // Room IDs needing live data
    std::string pos;                               // Sync token (empty = initial)
    int timeout = 30000;                           // Long-poll timeout in ms

    // Build the full JSON request body
    std::string toJson() const;
};

// ==== Response ====

// Operation types for sliding sync list updates
enum class SlidingSyncOp {
    SYNC = 0,        // Full sync of the window (range + room_ids)
    INSERT = 1,      // Insert rooms at a position
    DELETE = 2,      // Delete rooms at a position
    INVALIDATE = 3,  // Clear and re-sync the list
    UPDATE = 4       // Partial update of room data
};

// A single operation on a sliding window list
struct SlidingSyncListOp {
    SlidingSyncOp op = SlidingSyncOp::SYNC;
    int rangeStart = 0;
    int rangeEnd = 0;
    std::vector<std::string> roomIds;    // Room IDs affected
    int index = 0;                        // For INSERT/DELETE operations
};

// Per-list response
struct SlidingSyncListResponse {
    int count = 0;                       // Total rooms in this list
    std::vector<SlidingSyncListOp> ops;  // Operations to apply
};

// Room data from sliding sync
struct SlidingSyncRoom {
    std::string roomId;
    std::string name;
    std::string avatarUrl;
    std::string topic;
    bool isDirect = false;
    bool isSpace = false;
    bool isFavourite = false;
    int notificationCount = 0;
    int highlightCount = 0;
    int64_t timestamp = 0;               // Last activity timestamp
    bool initial = true;                 // First data for this room
    bool isEncrypted = false;
    std::string membership;              // "join", "invite", "leave"
    // Required state events (JSON strings)
    std::vector<std::string> requiredStateEvents;
    // Timeline events (recent messages)
    std::vector<std::string> timelineEvents;
};

// Extensions (E2EE, To-Device, Account Data)
struct SlidingSyncExtensions {
    std::string e2ee;                    // E2EE device updates
    std::string toDevice;                // To-device events
    std::string accountData;             // Account data changes
};

// Top-level response
struct SlidingSyncResponse {
    std::string pos;                                   // New sync token
    bool success = false;
    std::string errorMessage;
    std::unordered_map<std::string, SlidingSyncListResponse> lists;  // list_name → response
    std::unordered_map<std::string, SlidingSyncRoom> rooms;          // room_id → room data
    SlidingSyncExtensions extensions;
};

// ==== API Client ====

// Execute a sliding sync request.
// Sends POST /_matrix/client/unstable/org.matrix.msc3575/sync
//
// Original Element X: client.syncService().finish() → Rust SDK handles internally
SlidingSyncResponse slidingSync(const SlidingSyncRequest& request,
                                 const std::string& homeserverUrl,
                                 const std::string& accessToken);

// ==== Response Parsing ====

// Parse the JSON response from the sliding sync endpoint.
SlidingSyncResponse parseSlidingSyncResponse(const std::string& json);

// ==== Client State Management ====

// Track the sliding sync state for pagination and subscription management.
struct SlidingSyncState {
    std::string pos;                     // Current sync token
    std::vector<SlidingSyncList> lists;  // Active list configurations
    std::vector<std::string> subscribedRooms; // Currently subscribed rooms
    int pageSize = 20;
    int prefetchRange = 40;              // Rooms to preload beyond visible range

    // Move the visible window down (scroll down → load more)
    void scrollDown(int amount = 20);

    // Subscribe to newly visible rooms, unsubscribe from no-longer-visible rooms
    void updateSubscriptions(const std::vector<std::string>& visibleRoomIds);

    // Build a SlidingSyncRequest from current state
    SlidingSyncRequest buildRequest(int timeout = 30000) const;
};

} // namespace progressive
