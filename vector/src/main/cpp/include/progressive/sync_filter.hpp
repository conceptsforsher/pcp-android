#pragma once

// progressive/sync_filter.hpp
//
// Port of Matrix filter models and builders from Kotlin:
//   org.matrix.android.sdk.internal.session.filter.Filter.kt
//   org.matrix.android.sdk.internal.session.filter.RoomFilter.kt
//   org.matrix.android.sdk.internal.session.filter.RoomEventFilter.kt
//   org.matrix.android.sdk.internal.session.filter.EventFilter.kt
//   org.matrix.android.sdk.internal.session.filter.FilterResponse.kt
//   org.matrix.android.sdk.internal.session.filter.FilterFactory.kt
//   org.matrix.android.sdk.internal.session.filter.FilterUtil.kt
//   org.matrix.android.sdk.internal.sync.filter.SyncFilterBuilder.kt
//   org.matrix.android.sdk.api.session.sync.filter.SyncFilterParams.kt
//
// Matrix spec reference:
//   https://matrix.org/docs/spec/client_server/r0.3.0.html#post-matrix-client-r0-user-userid-filter

#include <string>
#include <vector>
#include <optional>

namespace progressive {

// ---- Filter Constants ----
// Original Kotlin: implicit via default parameters
constexpr int DEFAULT_LIMIT = 20;
constexpr const char* EVENT_FORMAT_CLIENT = "client";
constexpr const char* EVENT_FORMAT_FEDERATION = "federation";

// Default event fields to include per spec
inline const std::vector<std::string> DEFAULT_EVENT_FIELDS = {
    "type",
    "content",
    "sender",
    "event_id",
    "origin_server_ts",
    "unsigned",
    "room_id",
    "state_key",
    "prev_content",
    "redacts"
};

// ---- EventFilter ----
// Original Kotlin: EventFilter.kt (59 lines)
// Represents base event filter per spec:
//   https://matrix.org/docs/spec/client_server/r0.3.0.html#post-matrix-client-r0-user-userid-filter
struct EventFilter {
    std::optional<int> limit;                                  // max events to return
    std::optional<std::vector<std::string>> notSenders;        // sender IDs to exclude
    std::optional<std::vector<std::string>> senders;           // sender IDs to include
    std::optional<std::vector<std::string>> types;             // event types to include
    std::optional<std::vector<std::string>> notTypes;          // event types to exclude

    bool hasData() const;
    std::string toJson() const;
};

// ---- RoomEventFilter ----
// Original Kotlin: RoomEventFilter.kt (105 lines)
// Represents room-level event filter per spec.
struct RoomEventFilter {
    std::optional<int> limit;
    std::optional<std::vector<std::string>> notSenders;
    std::optional<std::vector<std::string>> senders;
    std::optional<std::vector<std::string>> types;
    std::optional<std::vector<std::string>> notTypes;
    std::optional<std::vector<std::string>> rooms;             // room IDs to include
    std::optional<std::vector<std::string>> notRooms;          // room IDs to exclude
    std::optional<bool> containsUrl;                           // filter by url presence in content
    std::optional<std::string> url;                            // filter by specific url
    std::optional<bool> lazyLoadMembers;                       // enable lazy member loading
    std::optional<bool> includeRedundantMembers;               // include redundant member events
    // Original Kotlin: enableUnreadThreadNotifications
    std::optional<bool> unreadThreadNotifications;             // MSC3773 thread notification opt-in

    bool hasData() const;
    std::string toJson() const;
};

// ---- StateFilter ----
// Original Kotlin: RoomEventFilter (state uses same structure per spec)
// StateFilter mirrors RoomEventFilter except lacks url field.
struct StateFilter {
    std::optional<int> limit;
    std::optional<std::vector<std::string>> notSenders;
    std::optional<std::vector<std::string>> senders;
    std::optional<std::vector<std::string>> types;
    std::optional<std::vector<std::string>> notTypes;
    std::optional<bool> lazyLoadMembers;
    std::optional<bool> includeRedundantMembers;
    std::optional<bool> unreadThreadNotifications;
    std::optional<std::vector<std::string>> rooms;
    std::optional<std::vector<std::string>> notRooms;
    std::optional<bool> containsUrl;

    bool hasData() const;
    std::string toJson() const;
};

// ---- RoomFilter ----
// Original Kotlin: RoomFilter.kt (70 lines)
// Represents room-level filter per spec.
struct RoomFilter {
    std::optional<std::vector<std::string>> rooms;             // room IDs to include
    std::optional<std::vector<std::string>> notRooms;          // room IDs to exclude
    std::optional<RoomEventFilter> ephemeral;                  // ephemeral events filter (typing, receipts)
    std::optional<StateFilter> state;                          // state events filter
    std::optional<RoomEventFilter> timeline;                   // timeline events filter
    std::optional<RoomEventFilter> accountData;                // per-room account data filter
    std::optional<bool> includeLeave;                          // include rooms user has left
    std::optional<bool> unreadThreadNotifications;             // unread thread notifications in room

    bool hasData() const;
    std::string toJson() const;
};

// ---- FilterBody ----
// Original Kotlin: Filter.kt (58 lines) — named FilterBody to avoid ambiguity
// Represents the full filter body for POST /user/{userId}/filter
struct FilterBody {
    std::optional<std::vector<std::string>> eventFields;       // fields to include in events
    std::optional<std::string> eventFormat;                    // "client" or "federation"
    std::optional<EventFilter> presence;                       // presence updates filter
    std::optional<EventFilter> accountData;                    // global account data filter
    std::optional<RoomFilter> room;                            // room-specific filter

    bool hasData() const;
    std::string toJson() const;
};

// ---- FilterResponse ----
// Original Kotlin: FilterResponse.kt (33 lines)
// Response from POST /user/{userId}/filter
struct FilterResponse {
    std::string filterId;
};

// ---- SyncFilterParams ----
// Original Kotlin: SyncFilterParams.kt (25 lines)
// Parameters used to construct a sync filter.
struct SyncFilterParams {
    std::optional<bool> lazyLoadMembersForStateEvents;
    std::optional<bool> lazyLoadMembersForMessageEvents;
    std::optional<bool> useThreadNotifications;
    std::vector<std::string> listOfSupportedEventTypes;
    std::vector<std::string> listOfSupportedStateEventTypes;
};

// ============================================================
//  Filter Builders (JSON)
// ============================================================

// Build full POST filter JSON from FilterBody.
std::string buildFilterBody(const FilterBody& filter);

// Build room filter JSON from RoomFilter.
std::string buildRoomFilter(const RoomFilter& filter);

// Build room event filter JSON from RoomEventFilter.
std::string buildRoomEventFilter(const RoomEventFilter& filter);

// Build state filter JSON from StateFilter.
std::string buildStateFilter(const StateFilter& filter);

// Build base event filter JSON from EventFilter.
std::string buildEventFilter(const EventFilter& filter);

// Convenience builder: wraps a RoomEventFilter in a FilterBody
// with only a timeline room filter. Useful for /sync timeline-only filtering.
std::string buildTimelineFilter(const RoomEventFilter& timelineFilter);

// Build a FilterBody from SyncFilterParams (from SyncFilterBuilder.kt:57-61).
FilterBody buildFilterBodyFromParams(
    const SyncFilterParams& params,
    bool canUseThreadReadReceiptsAndNotifications = false);

// ============================================================
//  Filter Parsers
// ============================================================

// Parse POST filter response JSON, extracting filter_id.
// Input: {"filter_id": "abc123"}
FilterResponse parseFilterResponse(const std::string& json);

// Parse full filter body JSON into FilterBody struct.
FilterBody parseFilterBody(const std::string& json);

// ============================================================
//  Lazy Loading
// ============================================================

// Original Kotlin: FilterUtil.enableLazyLoading() (111 lines)
// Returns a FilterBody with lazy_load_members enabled/disabled on room.state.
FilterBody buildLazyLoadingFilter(bool useLazyLoading = true);

// Check if a filter body has lazy loading enabled on room.state.
bool isLazyLoadingEnabled(const FilterBody& filter);

// Check if a room event filter has lazy loading enabled.
bool isLazyLoadingEnabled(const RoomEventFilter& filter);

// ============================================================
//  Unread Thread Notifications (MSC3773)
// ============================================================

// Build a FilterBody with unread_thread_notifications enabled
// per MSC3773 (unread thread notification counts).
FilterBody buildUnreadThreadFilter(bool enable = true);

// ============================================================
//  Default Filters & Utilities
// ============================================================

// Original Kotlin: FilterFactory.createDefaultFilter()
// Returns a filter with lazy loading enabled on room.state.
FilterBody getDefaultFilter();

// Original Kotlin: FilterFactory.createDefaultRoomFilter()
// Returns RoomEventFilter with lazy_load_members=true.
RoomEventFilter getDefaultRoomEventFilter();

// Original Kotlin: FilterUtil — check if filter has any active filtering.
bool hasActiveFiltering(const FilterBody& filter);

// Convert FilterBody to JSON string (alias for buildFilterBody).
std::string filterBodyToJson(const FilterBody& filter);

// Convert FilterResponse to JSON string.
std::string filterResponseToJson(const FilterResponse& response);

} // namespace progressive
