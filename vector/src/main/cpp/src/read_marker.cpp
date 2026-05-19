#include "progressive/read_marker.hpp"
#include <sstream>
#include <algorithm>
#include <unordered_set>

namespace progressive {

ReadMarkerState computeReadMarker(const std::string& lastReadEventId,
    const std::vector<std::string>& loadedEventIds,
    const std::vector<std::string>& loadedSenders,
    const std::vector<bool>& isMention,
    const std::vector<bool>& isHighlight,
    const std::string& myUserId) {
    // Original Kotlin (TimelineViewModel.kt:1023):
    //   val indexOfEvent = timeline.getIndexOfEvent(targetEventId)
    //   This iterates the in-memory list to find the event position.
    ReadMarkerState state;
    state.lastReadEventId = lastReadEventId;

    if (lastReadEventId.empty() || loadedEventIds.empty()) return state;

    // Find the read marker position in the loaded events
    // Original Kotlin uses timeline.getIndexOfEvent() which is O(n) iteration
    int markerIndex = -1;
    for (size_t i = 0; i < loadedEventIds.size(); ++i) {
        if (loadedEventIds[i] == lastReadEventId) {
            markerIndex = static_cast<int>(i);
            break;
        }
    }

    // If read marker not found in loaded events, it's further back in history
    if (markerIndex < 0) return state;

    state.readMarkerIndex = markerIndex;

    // Count unread events after the read marker
    // Original Kotlin (TimelineViewModel.kt):
    //   unreadState = computeUnreadState(timeline, readMarkerIndex)
    int totalAfter = static_cast<int>(loadedEventIds.size()) - markerIndex - 1;
    state.unreadCount = totalAfter;

    for (int i = markerIndex + 1; i < static_cast<int>(loadedEventIds.size()); ++i) {
        // Skip own messages
        if (i < static_cast<int>(loadedSenders.size()) && loadedSenders[i] == myUserId) {
            state.unreadCount--;
            continue;
        }
        if (i < static_cast<int>(isMention.size()) && isMention[i]) state.unreadMentions++;
        if (i < static_cast<int>(isHighlight.size()) && isHighlight[i]) state.unreadHighlights++;
    }

    state.hasUnread = state.unreadCount > 0;
    state.showReadMarker = state.hasUnread;

    // First unread event is the one right after the read marker
    if (markerIndex + 1 < static_cast<int>(loadedEventIds.size())) {
        state.firstUnreadEventId = loadedEventIds[markerIndex + 1];
    }

    return state;
}

bool shouldShowJumpToUnread(const ReadMarkerState& state) {
    // Original Kotlin (TimelineFragment.kt:2017-2024):
    //   Show the FAB when user has scrolled away from unread
    return state.hasUnread && state.readMarkerIndex >= 0;
}

std::string formatUnreadJumpLabel(const ReadMarkerState& state) {
    if (!state.hasUnread) return "";

    std::ostringstream out;
    out << state.unreadCount;
    if (state.unreadCount == 1) {
        out << " new message";
    } else {
        out << " new messages";
    }
    if (state.unreadMentions > 0) {
        out << " (" << state.unreadMentions << " mentions)";
    }
    return out.str();
}

std::string advanceReadMarker(const std::string& currentRoomId, const std::string& latestEventId) {
    // Original Kotlin (DefaultReadMarkers.kt):
    //   room.markAsRead(latestEventId, ReadMarkerType.FULLY_READ)
    return latestEventId;
}

std::string readMarkerToJson(const ReadMarkerState& state) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << "{";
    json << R"("lastReadEventId": ")" << esc(state.lastReadEventId) << R"(",)";
    json << R"("firstUnreadEventId": ")" << esc(state.firstUnreadEventId) << R"(",)";
    json << R"("unreadCount": )" << state.unreadCount << ",";
    json << R"("unreadMentions": )" << state.unreadMentions << ",";
    json << R"("hasUnread": )" << (state.hasUnread ? "true" : "false") << ",";
    json << R"("readMarkerIndex": )" << state.readMarkerIndex;
    json << "}";
    return json.str();
}

std::string formatTimeAgoLabel(int64_t timestampMs, int64_t nowMs) {
    if (timestampMs <= 0) return "";

    int64_t diffMs = nowMs - timestampMs;
    if (diffMs < 0) return "just now";

    int64_t seconds = diffMs / 1000;
    int64_t minutes = seconds / 60;
    int64_t hours = minutes / 60;
    int64_t days = hours / 24;
    int64_t weeks = days / 7;
    int64_t months = days / 30;

    if (seconds < 60) return "just now";
    if (minutes == 1) return "1 minute ago";
    if (minutes < 60) return std::to_string(minutes) + " minutes ago";
    if (hours == 1) return "1 hour ago";
    if (hours < 24) return std::to_string(hours) + " hours ago";
    if (days == 1) return "yesterday";
    if (days < 7) return std::to_string(days) + " days ago";
    if (weeks == 1) return "1 week ago";
    if (weeks < 5) return std::to_string(weeks) + " weeks ago";
    if (months == 1) return "1 month ago";
    return std::to_string(months) + " months ago";
}

std::string formatJumpToUnreadLabel(const ReadMarkerState& state, int64_t nowMs) {
    std::string timeLabel = formatTimeAgoLabel(state.firstUnreadTimestampMs, nowMs);
    if (timeLabel.empty()) return "Jump to unread";
    return "Jump to unread (" + timeLabel + ")";
}

// ================================================================
// Extended Read Marker Functions
// ================================================================

ReadMarkerPosition computeReadMarkerPosition(const std::string& readMarkerEventId,
                                               const std::vector<std::string>& loadedEventIds,
                                               bool isFullyRead) {
    // Original Kotlin: computeReadMarkerPosition() — finds index and state
    ReadMarkerPosition pos;
    pos.eventId = readMarkerEventId;
    pos.isFullyRead = isFullyRead;
    pos.displayIndex = -1;

    if (readMarkerEventId.empty()) return pos;

    for (size_t i = 0; i < loadedEventIds.size(); ++i) {
        if (loadedEventIds[i] == readMarkerEventId) {
            pos.displayIndex = static_cast<int>(i);
            break;
        }
    }

    return pos;
}

std::string updateReadMarker(const std::string& roomId, const std::string& eventId,
                              bool isFullyRead) {
    // Original Kotlin: updateReadMarker() — advances to given event
    return eventId;
}

bool isEventRead(const std::string& eventId, const std::string& readMarkerEventId,
                 const std::vector<std::string>& eventIds) {
    // Original Kotlin: isEventRead() — event at or before read marker
    if (eventId == readMarkerEventId) return true;

    int rmIdx = -1, evIdx = -1;
    for (size_t i = 0; i < eventIds.size(); ++i) {
        if (eventIds[i] == readMarkerEventId) rmIdx = static_cast<int>(i);
        if (eventIds[i] == eventId) evIdx = static_cast<int>(i);
    }

    if (rmIdx < 0 || evIdx < 0) return false;
    // Events earlier in the list (lower index) were sent earlier
    // Read marker covers events at or before it
    return evIdx <= rmIdx;
}

ReadMarkerRelation getReadMarkerRelation(const std::string& eventId,
                                          const std::string& readMarkerEventId,
                                          const std::vector<std::string>& eventIds) {
    // Original Kotlin: getReadMarkerState() — returns ABOVE/AT/BELOW/NOT_FOUND
    if (eventId == readMarkerEventId) return ReadMarkerRelation::AT;

    int rmIdx = -1, evIdx = -1;
    for (size_t i = 0; i < eventIds.size(); ++i) {
        if (eventIds[i] == readMarkerEventId) rmIdx = static_cast<int>(i);
        if (eventIds[i] == eventId) evIdx = static_cast<int>(i);
    }

    if (evIdx < 0) return ReadMarkerRelation::NOT_FOUND;
    if (rmIdx < 0) return ReadMarkerRelation::ABOVE; // no marker = everything above
    return evIdx < rmIdx ? ReadMarkerRelation::ABOVE : ReadMarkerRelation::BELOW;
}

std::string formatReadMarkerLabel(const ReadMarkerPosition& position) {
    // Original Kotlin: formatReadMarkerLabel() — "New" if above read marker
    if (position.isFullyRead) return "Read";
    if (position.displayIndex < 0) return "";
    return "New";
}

std::string formatReadReceipts(const std::vector<ReadReceiptsInfo>& receipts,
                                const std::string& myUserId) {
    // Original Kotlin: formatReadReceipts() — "Read by Alice and 3 others"
    if (receipts.empty()) return "";

    // Collect unique user IDs excluding self
    std::unordered_set<std::string> seen;
    std::vector<std::string> others;
    for (const auto& r : receipts) {
        for (const auto& uid : r.userIds) {
            if (uid != myUserId && seen.insert(uid).second) {
                others.push_back(uid);
            }
        }
    }

    if (others.empty()) return "";
    if (others.size() == 1) return "Read by " + others[0];
    std::ostringstream out;
    out << "Read by " << others[0] << " and " << (others.size() - 1) << " others";
    return out.str();
}

ReadReceiptsInfo getReadReceiptsForEvent(const std::string& eventId,
                                           const std::vector<ReadReceiptsInfo>& receipts) {
    // Original Kotlin: getReadReceiptsForEvent()
    for (const auto& r : receipts) {
        if (r.eventId == eventId) return r;
    }
    return {};
}

std::string buildJumpToReadMarkerRequest(const ReadMarkerJumpInfo& jumpInfo) {
    // Original Kotlin: buildJumpToReadMarkerRequest()
    std::ostringstream os;
    os << R"({"event_id":")" << jumpInfo.eventId
       << R"(","room_id":")" << jumpInfo.roomId
       << R"(","position":)" << jumpInfo.jumpPosition << "}";
    return os.str();
}

bool isReadMarkerVisible(const ReadMarkerPosition& position, int visibleStartIndex, int visibleEndIndex) {
    // Original Kotlin: isReadMarkerVisible() — is the marker in the viewport
    if (position.displayIndex < 0) return false;
    return position.displayIndex >= visibleStartIndex && position.displayIndex <= visibleEndIndex;
}

bool shouldShowJumpToReadMarker(const ReadMarkerPosition& position, int firstVisibleIndex) {
    // Original Kotlin: shouldShowJumpToReadMarker() — marker is above visible area
    if (position.displayIndex < 0) return false;
    return position.displayIndex < firstVisibleIndex;
}

} // namespace progressive
