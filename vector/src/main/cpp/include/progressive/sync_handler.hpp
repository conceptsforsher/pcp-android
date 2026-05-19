#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include "progressive/sync_models.hpp"

namespace progressive {

// ==== SyncErrorRecovery ====
//
// Original Kotlin: sync/SyncErrorRecovery.kt
enum class SyncErrorRecovery {
    RETRY,       // Retry the sync request after backoff
    RESET_CACHE, // Clear local cache and re-sync
    RE_LOGIN,    // Clear session and require re-login
    CONTINUE     // Ignore the error and continue
};

const char* syncErrorRecoveryToString(SyncErrorRecovery r);
SyncErrorRecovery syncErrorRecoveryFromString(const std::string& s);

// ==== SyncErrorMessage ====
//
// Original Kotlin: sync/SyncErrorMessage.kt
struct SyncErrorMessage {
    std::string error;
    SyncErrorRecovery recovery = SyncErrorRecovery::RETRY;
    std::string message;
};

// ==== SyncTokenManager ====
//
// Original Kotlin: sync/SyncTokenManager.kt
struct SyncTokenManager {
    std::string currentToken;
    std::string prevToken;
    bool isInitialSync = true;

    bool isValid() const;
};

// ==== SyncScheduler ====
//
// Original Kotlin: sync/SyncScheduler.kt
struct SyncScheduler {
    int64_t intervalMs = 30000;
    int64_t backoffMs = 0;
    bool isScheduled = false;

    int64_t computeBackoff() const;
};

// ==== SyncFilterManager ====
//
// Original Kotlin: sync/SyncFilterManager.kt
struct SyncFilterManager {
    std::string currentFilterId;
    std::string buildFilterParams;

    bool applyFilter(const std::string& filterId);
};

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

// ---- Sync Error Handling ----
//
// Original Kotlin: SyncErrorHandler.kt

// Handle a sync error and return the recommended recovery action with message.
SyncErrorMessage handleSyncError(const std::string& errorJson, int httpStatus);

// Classify an HTTP error into a recovery category.
// Original Kotlin: classifySyncError()
SyncErrorRecovery classifySyncError(int httpStatus, const std::string& errorBody);

// ---- Sync Filter Functions ----
//
// Original Kotlin: SyncFilterBuilder.kt

// Build a sync filter JSON for the /sync request.
// roomTypes: list of room types to include (e.g. "m.space")
// eventTypes: list of event types to include
// limit: timeline event limit per room
std::string buildSyncFilter(const std::vector<std::string>& roomTypes,
                            const std::vector<std::string>& eventTypes,
                            int limit = 20);

// Parse the filter creation response from the server.
// Returns the filter ID assigned by the server.
// Original Kotlin: SyncFilterParser.parse()
std::string parseSyncFilterResponse(const std::string& responseJson);

// ---- Sync Scheduling ----
//
// Original Kotlin: sync/SyncScheduler.kt

// Schedule the next sync with exponential backoff on error.
// Returns the computed interval in milliseconds.
int64_t scheduleNextSync(SyncScheduler& scheduler, bool isError = false);

// Compute the appropriate sync interval based on app activity.
// Original Kotlin: computeSyncInterval()
int64_t computeSyncInterval(bool isActive, bool hasPendingEvents);

// ---- Sync Token Management ----
//
// Original Kotlin: sync/SyncTokenManager.kt

// Rotate the sync token: move current to previous, set new current.
void rotateSyncToken(SyncTokenManager& manager, const std::string& newToken);

// Check if a sync token string is valid (non-empty and well-formed).
bool isTokenValid(const std::string& token);

} // namespace progressive
