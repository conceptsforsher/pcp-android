#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// ---- Read Marker & Unread Count ----
// Ported from: im.vector.app.features.home.room.detail.TimelineViewModel.kt
//              (read marker positioning logic, ~50 lines of index math)
//              org.matrix.android.sdk.api.session.room.read.ReadMarkers.kt
//              (server-side read marker management)

struct ReadMarkerState {
    std::string roomId;
    std::string lastReadEventId;
    std::string firstUnreadEventId;
    int64_t firstUnreadTimestampMs = 0;  // when the first unread event was sent
    int unreadCount = 0;
    int unreadMentions = 0;
    int unreadHighlights = 0;
    bool hasUnread = false;
    bool showReadMarker = false;
    int readMarkerIndex = -1;
};

// ---- Read Marker Position ----
// Original Kotlin: ReadMarkerPosition data class

struct ReadMarkerPosition {
    std::string eventId;
    int displayIndex = -1;
    bool isFullyRead = false;
};

// ---- Read Marker Relation ----
// Original Kotlin: describes whether an event is above, at, or below the read marker

enum class ReadMarkerRelation {
    ABOVE = 0,
    AT = 1,
    BELOW = 2,
    NOT_FOUND = 3,
};

// ---- Read Receipts Info ----
// Original Kotlin: ReadReceiptsInfo data class

struct ReadReceiptsInfo {
    std::string eventId;
    std::vector<std::string> userIds;
    int64_t timestamp = 0;
};

// ---- Read Marker Jump Info ----
// Original Kotlin: ReadMarkerJumpInfo data class

struct ReadMarkerJumpInfo {
    std::string eventId;
    std::string roomId;
    int jumpPosition = -1;
};

// Compute read marker position in the loaded event list.
// Original Kotlin (TimelineViewModel.kt:1023-1035):
//   val indexOfEvent = timeline.getIndexOfEvent(targetEventId)
//   if (indexOfEvent == null) timeline.restartWithEventId(targetEventId)
//   setState { copy(highlightedEventId = targetEventId) }
ReadMarkerState computeReadMarker(const std::string& lastReadEventId,
    const std::vector<std::string>& loadedEventIds,
    const std::vector<std::string>& loadedSenders,
    const std::vector<bool>& isMention,
    const std::vector<bool>& isHighlight,
    const std::string& myUserId);

// Check if we should show the "jump to first unread" button.
bool shouldShowJumpToUnread(const ReadMarkerState& state);

// Format unread count for the jump button: "14 new messages".
std::string formatUnreadJumpLabel(const ReadMarkerState& state);

// Format a "time ago" label for the jump button.
// "Jump to unread (3 hours ago)", "Jump to unread (5 days ago)"
// Returns empty string if firstUnreadTimestampMs is 0.
std::string formatTimeAgoLabel(int64_t timestampMs, int64_t nowMs);

// Format the complete jump button text with optional time suffix.
// Returns "Jump to unread (3 hours ago)" or "Jump to unread" if no timestamp.
std::string formatJumpToUnreadLabel(const ReadMarkerState& state, int64_t nowMs);

// Update read marker after sending own message (auto-advance).
std::string advanceReadMarker(const std::string& currentRoomId, const std::string& latestEventId);

// Format read marker state as JSON for the Kotlin UI layer.
std::string readMarkerToJson(const ReadMarkerState& state);

// ---- Extended Read Marker Functions ----

// Compute the position of the read marker in the event list.
// Original Kotlin: computeReadMarkerPosition()
ReadMarkerPosition computeReadMarkerPosition(const std::string& readMarkerEventId,
                                               const std::vector<std::string>& loadedEventIds,
                                               bool isFullyRead);

// Update the read marker to a new event ID.
// Original Kotlin: updateReadMarker()
std::string updateReadMarker(const std::string& roomId, const std::string& eventId,
                              bool isFullyRead);

// Check if an event has been read (is at or below the read marker).
// Original Kotlin: isEventRead()
bool isEventRead(const std::string& eventId, const std::string& readMarkerEventId,
                 const std::vector<std::string>& eventIds);

// Get the relation of an event to the read marker.
// Original Kotlin: getReadMarkerState()
ReadMarkerRelation getReadMarkerRelation(const std::string& eventId,
                                          const std::string& readMarkerEventId,
                                          const std::vector<std::string>& eventIds);

// Format the read marker label for the UI ("New", "Read").
// Original Kotlin: formatReadMarkerLabel()
std::string formatReadMarkerLabel(const ReadMarkerPosition& position);

// Format read receipts for display.
// Original Kotlin: formatReadReceipts()
std::string formatReadReceipts(const std::vector<ReadReceiptsInfo>& receipts,
                                const std::string& myUserId);

// Get read receipts for a specific event.
// Original Kotlin: getReadReceiptsForEvent()
ReadReceiptsInfo getReadReceiptsForEvent(const std::string& eventId,
                                           const std::vector<ReadReceiptsInfo>& receipts);

// Build a request to jump to the read marker.
// Original Kotlin: buildJumpToReadMarkerRequest()
std::string buildJumpToReadMarkerRequest(const ReadMarkerJumpInfo& jumpInfo);

// Check if the read marker is visible in the current viewport.
// Original Kotlin: isReadMarkerVisible()
bool isReadMarkerVisible(const ReadMarkerPosition& position, int visibleStartIndex, int visibleEndIndex);

// Check if the "jump to read marker" button should be shown.
// Original Kotlin: shouldShowJumpToReadMarker()
bool shouldShowJumpToReadMarker(const ReadMarkerPosition& position, int firstVisibleIndex);

} // namespace progressive
