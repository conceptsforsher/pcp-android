#ifndef PROGRESSIVE_TIMELINE_UTILS_HPP
#define PROGRESSIVE_TIMELINE_UTILS_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// ---- Timeline Utilities ----

struct TimelineChunk {
    std::string startToken;        // pagination token (start)
    std::string endToken;          // pagination token (end)
    std::vector<std::string> eventIds;
    int eventCount = 0;
    bool hasMoreBackward = false;
    bool hasMoreForward = false;
    bool isLastBackward = false;
    bool isLastForward = false;
};

// Merge overlapping timeline chunks into a single continuous timeline.
std::vector<std::string> mergeTimelineChunks(const std::vector<TimelineChunk>& chunks);

// Detect gaps between consecutive chunks.
std::vector<std::pair<std::string, std::string>> detectChunkGaps(
    const std::vector<TimelineChunk>& chunks);

// Sort chunks by their position in the timeline (backward to forward).
void sortChunksByPosition(std::vector<TimelineChunk>& chunks);

// Check if a chunk contains a specific event.
bool chunkContainsEvent(const TimelineChunk& chunk, const std::string& eventId);

// Get the total event count across all chunks.
int getTotalChunkEvents(const std::vector<TimelineChunk>& chunks);

// ---- Event Ordering ----

struct OrderedEvent {
    std::string eventId;
    int64_t originServerTs = 0;
    int streamOrder = 0;           // from sync order
    int depth = 0;                 // thread depth
    std::string orderingKey;       // composite key for sorting
};

// Sort events by stream order (sync order), falling back to timestamp.
void sortByStreamOrder(std::vector<OrderedEvent>& events);

// Sort events by timestamp only.
void sortByTimestamp(std::vector<OrderedEvent>& events);

// Compute an ordering key that combines timestamp and stream position.
std::string computeOrderingKey(int64_t timestamp, int streamOrder);

// ---- Live Timeline State ----

struct LiveTimelineState {
    bool isLive = true;            // showing latest events
    bool isPaused = false;         // user scrolled up
    int unreadCount = 0;           // events below visible area
    int unreadMentions = 0;
    std::string lastReadEventId;
    std::string firstUnreadEventId;
    int64_t lastUserScrollMs = 0;
    bool shouldJumpToBottom = true; // auto-scroll on new messages
};

// Check if the timeline should auto-scroll to bottom.
bool shouldAutoScroll(const LiveTimelineState& state, bool newEventIsFromMe);

// Update live timeline state after user scroll.
LiveTimelineState updateScrollState(const LiveTimelineState& state, int64_t nowMs);

// Check if user has scrolled away from live (paused state).
bool hasScrolledAway(const LiveTimelineState& state, int visibleFirstIndex,
    int totalEvents, int thresholdFromEnd = 3);

// ---- Loading Progress Indicator ---
// Optional Labs feature: shows how many events loaded vs rendered in loading spinner.

struct LoadingProgress {
    int eventsLoaded = 0;      // total fetched from server
    int eventsRendered = 0;    // already displayed in UI
    int eventsPending = 0;     // loaded but not yet rendered
    bool isLoading = false;    // fetch in progress
    std::string label;         // e.g. "12" for the spinner center
};

LoadingProgress computeLoadingProgress(int loaded, int rendered);
std::string loadingProgressToJson(const LoadingProgress& prog);

} // namespace progressive

#endif // PROGRESSIVE_TIMELINE_UTILS_HPP
