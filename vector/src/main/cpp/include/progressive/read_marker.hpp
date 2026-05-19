#ifndef PROGRESSIVE_READ_MARKER_HPP
#define PROGRESSIVE_READ_MARKER_HPP

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

} // namespace progressive

#endif // PROGRESSIVE_READ_MARKER_HPP
