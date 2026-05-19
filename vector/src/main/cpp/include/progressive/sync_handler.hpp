#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "progressive/sync_models.hpp"

namespace progressive {

// ---- SyncHandlerResult ----

// Original Kotlin: SyncResponseHandler.kt — aggregated handler result
struct SyncHandlerResult {
    int roomsProcessed = 0;
    int eventsProcessed = 0;
    std::vector<std::string> errors;
    std::string aggregatorJson;  // serialized SyncResponsePostTreatmentAggregator
};

// Original Kotlin: SyncResponseHandler.handleResponse()
// Main handler — processes the full /sync v2 response:
//   - Rooms (joined, invited, left)
//   - Presence events
//   - Account data
//   - To-device events
//   - Device lists / OTK counts
SyncHandlerResult processSyncResponse(const SyncResponse& response);

// ---- AccountDataSyncResult ----

// Original Kotlin: UserAccountDataSyncHandler.kt
struct AccountDataSyncResult {
    std::string type;           // e.g. "m.direct", "m.push_rules"
    int totalEvents = 0;
    int processedEvents = 0;
};

// Original Kotlin: handle() from UserAccountDataSyncHandler
// Processes the account_data block from /sync:
//   - m.direct (DM room associations)
//   - m.push_rules
//   - m.ignored_user_list
//   - m.breadcrumbs
//   - Generic account data (stored as-is)
std::vector<AccountDataSyncResult> processAccountDataSync(
    const std::vector<UserAccountDataEvent>& events);

// ---- Room Account Data ----

// Original Kotlin: RoomSyncAccountDataHandler.kt + RoomSyncAccountData
// Process per-room account data events (tags, fully_read markers).
// Returns number of room account data events processed.
int processRoomAccountData(const std::string& roomId,
                           const std::vector<Event>& accountDataEvents);

// ---- Room Tag Info ----

// Original Kotlin: RoomTagHandler.kt
struct RoomTagInfo {
    std::string roomId;
    std::string tagName;
    double tagOrder = 0.0;
};

// Original Kotlin: RoomTagHandler.handle()
// Extract tag info from a room tag account data event.
// Returns a list of RoomTagInfo for the given room and tag content JSON.
std::vector<RoomTagInfo> processRoomTags(const std::string& roomId, const std::string& tagContentJson);

// ---- Fully Read Marker ----

// Original Kotlin: RoomFullyReadHandler.kt
// Extract the fully read event ID from a m.fully_read account data event.
// Returns the eventId, or empty string if not found.
std::string processFullyReadMarker(const std::string& fullyReadContentJson);

// ---- Typing Users ----

// Original Kotlin: RoomTypingUsersHandler.kt
// Process typing users ephemeral event data for a room.
// Returns filtered list of typing user IDs (minus ignored users and self).
std::vector<std::string> processTypingUsers(const std::string& roomId,
                                             const std::vector<std::string>& typingUserIds,
                                             const std::string& selfUserId,
                                             const std::vector<std::string>& ignoredUserIds);

// ---- Sync Post Treatment ----

// Original Kotlin: SyncResponsePostTreatmentAggregator.kt
struct SyncPostTreatmentResult {
    int shieldUpdates = 0;       // rooms needing shield re-evaluation
    int spaceUpdates = 0;        // space hierarchy updates
    int summaryUpdates = 0;      // room summary updates
    std::vector<std::string> roomsWithMembershipChanges;
};

// Original Kotlin: SyncResponsePostTreatmentAggregator
// Compute post-sync treatment actions from the aggregator state.
SyncPostTreatmentResult computeSyncPostTreatment(const SyncHandlerResult& handlerResult);

} // namespace progressive
