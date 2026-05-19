#ifndef PROGRESSIVE_SYNC_FILTER_HPP
#define PROGRESSIVE_SYNC_FILTER_HPP

#include <string>
#include <vector>
#include "progressive/sync_utils.hpp"
#include <optional>

namespace progressive {

// ---- Sync Filter Builder ----
// Faithful port from original Kotlin:
//   org.matrix.android.sdk.api.session.sync.filter.SyncFilterParams.kt (25 lines)
//   org.matrix.android.sdk.internal.sync.filter.SyncFilterBuilder.kt (119 lines)
//
// SyncFilterBuilder creates a Matrix sync filter JSON that tells the server
// what data to include/exclude during sync. This reduces bandwidth and
// processing time by filtering out unneeded events.
//
// Key concepts from the original:
//   - lazyLoadMembers: don't include full member info with events
//   - useThreadNotifications: unread count includes thread replies
//   - threadNotifications capability: only used if server supports it
//   - orNullIfEmpty(): skip filter sections with no data
//   - Filter → RoomFilter → RoomEventFilter hierarchy

// Sync filter parameters (from SyncFilterParams.kt)
struct SyncFilterParams {
    std::optional<bool> lazyLoadMembersForStateEvents;     // lazy-load for state events
    std::optional<bool> lazyLoadMembersForMessageEvents;   // lazy-load for message events
    std::optional<bool> useThreadNotifications;            // count thread notifications
    std::vector<std::string> listOfSupportedEventTypes;    // timeline event types to include
    std::vector<std::string> listOfSupportedStateEventTypes; // state event types to include
};

// Room event filter (from RoomEventFilter)
struct RoomEventFilter {
    std::optional<bool> lazyLoadMembers;
    std::optional<bool> enableUnreadThreadNotifications;
    std::vector<std::string> types;     // event types or state types

    bool hasData() const;
    std::string toJson() const;
};

// Room filter (from RoomFilter)
struct RoomFilter {
    RoomEventFilter timeline;   // timeline events filter
    RoomEventFilter state;      // state events filter

    bool hasData() const;
    std::string toJson() const;
};

// Sync filter (from Filter)
struct RoomSyncFilter {
    RoomFilter room;            // room-specific filter
    std::vector<std::string> eventFields;  // fields to include in events
    std::string eventFormat;    // "client" or "federation"

    bool hasData() const;
    std::string toJson() const;
};

// Build a sync filter from parameters.
// Original Kotlin (SyncFilterBuilder.kt:build):
//   fun build(homeServerCapabilities: HomeServerCapabilities): Filter {
//       return Filter(room = buildRoomFilter(homeServerCapabilities))
//   }
SyncFilter buildSyncFilter(
    const SyncFilterParams& params,
    bool canUseThreadReadReceiptsAndNotifications = false
);

// Get the default sync filter (minimal data, lazy-load members).
SyncFilter getDefaultSyncFilter();

// Check if a sync filter has any active filtering (non-empty).
bool hasActiveFiltering(const SyncFilter& filter);

// Format filter as JSON for the sync request.
std::string syncFilterToJson(const SyncFilter& filter);

} // namespace progressive

#endif // PROGRESSIVE_SYNC_FILTER_HPP
