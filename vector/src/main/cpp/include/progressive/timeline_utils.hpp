#ifndef PROGRESSIVE_TIMELINE_UTILS_HPP
#define PROGRESSIVE_TIMELINE_UTILS_HPP

#include <string>
#include <vector>
#include <cstdint>
#include <functional>

// Forward declaration from timeline_chunk.hpp
namespace progressive { struct TimelineEventData; }

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

// ---- Pagination Models (from PaginationTask.kt + PaginationDirection.kt) ----
// Original Kotlin: PaginationDirection enum (FORWARDS="f", BACKWARDS="b")
// Original Kotlin: PaginationTask.Params (roomId, from, direction, limit)

enum class PaginationState { IDLE, LOADING, LOADED, ERROR, REACHED_END };

struct PaginationOptions {
    std::string from;       // pagination token
    std::string to;         // pagination token
    int limit = 20;
    std::string direction;  // Original Kotlin: PaginationDirection.value ("b" or "f")
    std::string filter;     // Original Kotlin: filterRepository.getRoomFilterBody()
};

struct PaginationResult {
    std::vector<std::string> eventJsons;  // raw event JSON strings from /messages chunk
    std::string prevToken;   // start token, for loading older events
    std::string nextToken;   // end token, for loading newer events
    bool reachedEnd = false; // Original Kotlin: REACHED_END / SHOULD_FETCH_MORE / SUCCESS
    PaginationState state = PaginationState::IDLE;
};

// Build query string for GET /_matrix/client/r0/rooms/{roomId}/messages.
// Original Kotlin: roomAPI.getRoomMessagesFrom(roomId, from, direction.value, limit, filter)
std::string buildPaginationRequest(const PaginationOptions& opts);

// Parse /messages response JSON. Extracts start, end, chunk[], state[].
// Original Kotlin: TokenChunkEventPersistor.insertInDb(receivedChunk, ...)
PaginationResult parsePaginationResponse(const std::string& json);

// Result from /rooms/{roomId}/context/{eventId} query.
// Original Kotlin: GetContextOfEventTask returns events_before, event, events_after
struct ContextResult {
    std::vector<std::string> eventsBeforeJson;
    std::string eventJson;               // the anchor event
    std::vector<std::string> eventsAfterJson;
    std::string prevToken;
    std::string nextToken;
    PaginationState state = PaginationState::IDLE;
};

// Build query string for GET /rooms/{roomId}/context/{eventId}.
// Original Kotlin: GetContextOfEventTask.Params(roomId, eventId, limit)
std::string buildContextRequest(const std::string& roomId, const std::string& eventId, int limit = 20);

// Parse /context response JSON.
// Original Kotlin: deserialized via GSON, we parse manually
ContextResult parseContextResponse(const std::string& json);

// Build query string for GET /rooms/{roomId}/event/{eventId}.
// Original Kotlin: used when fetching a single known event
std::string buildEventByIdRequest(const std::string& roomId, const std::string& eventId);

// Parse /event response JSON. Returns the raw event JSON string.
// Original Kotlin: single Event deserialization
std::string parseEventByIdResponse(const std::string& json);

// Parse a single event JSON string into TimelineEventData.
// TimelineEventData is defined in timeline_chunk.hpp.
TimelineEventData parseTimelineEventFromJson(const std::string& json);

// ---- Timeline Loading Strategy (from LoadTimelineStrategy.kt) ----
// Original Kotlin: LoadTimelineStrategy.Mode (Live, Permalink:originEventId, Thread:rootThreadEventId)
// Original Kotlin: LoadTimelineStrategy.loadMore(count, direction, fetchOnServerIfNeeded)

enum class LoadStrategy {
    FORWARD,      // load newer events (forwards pagination)
    BACKWARD,     // load older events (backwards pagination)
    CONTEXTUAL,   // load around an anchor event
    PERMALINK     // open at a specific event
};

struct TimelineLoadState {
    bool hasMoreForward = true;
    bool hasMoreBackward = true;
    bool isLoadingForward = false;
    bool isLoadingBackward = false;
    int loadedChunks = 0;
    int totalEvents = 0;
};

// Decide which direction to load based on current state.
// Original Kotlin: DefaultTimeline.loadMore(direction) + strategy.loadMore()
LoadStrategy computeLoadStrategy(const TimelineLoadState& state, bool isLive);

// Check if more events available in the given direction.
// Original Kotlin: DefaultTimeline.hasMoreToLoad(direction)
bool shouldLoadMore(const TimelineLoadState& state, LoadStrategy dir);

// Compute optimal page size for next request.
// Original Kotlin: TimelineSettings.initialSize, passed as 'count' to loadMore()
int getNextPageLimit(const TimelineLoadState& state, int defaultLimit = 20);

// Estimated loaded range for viewport calculation.
// Original Kotlin: used in TimelineChunk for displayIndex range calculation
struct EstimateLoadedRange {
    int64_t oldestTs = 0;
    int64_t newestTs = 0;
    int oldestIndex = 0;
    int newestIndex = 0;
};

// Estimate the loaded range from a set of events.
// Original Kotlin: finds oldest/newest timestamps and display indices
EstimateLoadedRange estimateLoadedRange(
    const std::vector<TimelineEventData>& events,
    int startIdx = 0,
    int count = -1);

// ---- Timeline Event Processing ----
// Original Kotlin: TimelineChunk.kt — builtItems(), merge logic, dedup, sort by displayIndex

struct TimelineEventProcessor {
    // Insert events into target in correct position (by display index).
    // Returns number of new (non-duplicate) events inserted.
    // Original Kotlin: ChunkEntity.addTimelineEvent(eventEntity, direction, ...)
    static int insertTimelineEvents(
        std::vector<TimelineEventData>& target,
        const std::vector<TimelineEventData>& incoming,
        bool dedup = true);

    // Merge new events with existing, deduplicating by eventId,
    // then sorting by display index. Returns combined sorted list.
    // Original Kotlin: merge of builtItems() from multiple linked chunks
    static std::vector<TimelineEventData> mergeTimelineEvents(
        const std::vector<TimelineEventData>& existing,
        const std::vector<TimelineEventData>& incoming,
        bool sortByDisplayIndex = true);

    // Sort events by displayIndex ascending.
    // Original Kotlin: TimelineChunk.displayIndex ordering
    static void sortTimelineEvents(
        std::vector<TimelineEventData>& events);
};

// Gap between two consecutive events.
// Original Kotlin: gap detection at chunk boundaries
struct TimelineGap {
    std::string beforeEventId;
    std::string afterEventId;
    int missingCount = 0;
    int64_t timeGapMs = 0;
};

// Find a gap between two events.
// Original Kotlin: determines if events are consecutive or have missing events between them
TimelineGap getTimelineGap(
    const TimelineEventData& before,
    const TimelineEventData& after,
    int avgBatchSize = 20);

// Visible range info.
// Original Kotlin: viewport calculation for rendered timeline items
struct TimelineRange {
    int startIndex = 0;
    int endIndex = 0;
    int totalEvents = 0;
};

// Compute visible range from loaded events and viewport.
// Original Kotlin: determines which events to render based on scroll position
TimelineRange computeTimelineRange(
    int visibleStart,
    int visibleEnd,
    int totalEvents);

} // namespace progressive

#endif // PROGRESSIVE_TIMELINE_UTILS_HPP
