#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <functional>
#include <utility>

#include "progressive/timeline_utils.hpp"

namespace progressive {

class SqliteDB; // Forward declaration

// ==== Native Timeline Chunk Engine ====
//
// C++ replacement for TimelineChunk.kt — the core pagination engine.
// Manages doubly-linked timeline chunks with display-index arithmetic.
// Opt-in via Labs: SETTINGS_LABS_NATIVE_TIMELINE
// Original Kotlin/Realm code remains as fallback.

// Timeline open mode — mirrors Kotlin's Mode sealed interface.
enum class TimelineMode {
    LIVE,         // Follow live stream (default)
    PERMALINK,    // Open around a specific event
    THREAD        // Thread timeline for a root event
};

// A single event in the timeline.
struct TimelineEventData {
    std::string eventId;
    std::string roomId;
    std::string senderId;
    std::string type;            // "m.room.message", etc.
    std::string contentJson;     // Raw event content
    int64_t originServerTs = 0;  // Server timestamp
    int64_t ageLocalTs = 0;      // Local timestamp when received
    int displayIndex = 0;        // Position in current chunk
    bool isState = false;        // Is this a state event?
    std::string stateKey;        // State key if state event
    std::string redacts;         // Event ID being redacted
    std::string relationType;    // "m.replace", "m.annotation", etc.
    std::string relatesToEventId; // Event ID this relates to
    bool isEncrypted = false;
};

// A chunk is a page of events with pagination tokens.
struct TimelineChunkData {
    std::string chunkId;
    std::string prevToken;       // Token for older events
    std::string nextToken;       // Token for newer events
    std::vector<TimelineEventData> events;
    bool isLastBackward = false; // Reached the beginning of history
    bool isLastForward = false;  // Reached the end of live stream
    int nextDisplayIndex = 0;    // Display index to assign to next inserted event

    // Linked-list references (indices into chunks_ vector, -1 = none)
    int prevChunkIdx = -1;       // Older chunk
    int nextChunkIdx = -1;       // Newer chunk
};

// Direction of pagination.
enum class TimelineDirection { FORWARDS = 0, BACKWARDS = 1 };

// ==== Chunk Manager ====

class TimelineChunkManager {
public:
    // Initialize with a room ID. Defaults to LIVE mode.
    TimelineChunkManager(const std::string& roomId);

    // ==== Mode-Based Initialization ====

    // Set the timeline mode and optionally an anchor event.
    // openLive(): follow live stream (isLastForward chunk)
    // openAtEvent(eventId): open around a specific event (permalink mode)
    void setMode(TimelineMode mode, const std::string& anchorEventId = "");
    TimelineMode getMode() const { return mode_; }
    std::string getAnchorEventId() const { return anchorEventId_; }

    // Add a chunk from pagination response.
    // Returns the number of new events added (after dedup).
    int addChunk(const std::string& chunkId,
                 const std::vector<TimelineEventData>& events,
                 const std::string& prevToken,
                 const std::string& nextToken,
                 TimelineDirection direction);

    // Add a single live event from /sync.
    // Returns display index assigned or -1 if duplicate.
    int addLiveEvent(const TimelineEventData& event);

    // Get events in display order (oldest first).
    std::vector<TimelineEventData> getEventsInOrder() const;

    // Get a specific event by ID.
    const TimelineEventData* getEvent(const std::string& eventId) const;

    // Get the display index of an event.
    int getDisplayIndex(const std::string& eventId) const;

    // Get the total event count.
    int totalEventCount() const;

    // Get pagination tokens for loading more.
    std::string getPrevToken() const;
    std::string getNextToken() const;

    // Check if more events can be loaded in a direction.
    bool canLoadMore(TimelineDirection dir) const;

    // ==== SqliteDB Persistence ====
    //
    // Attach a SqliteDB for persistent event storage.
    // When set, events are automatically persisted on addChunk/addLiveEvent
    // and loaded via loadFromDatabase().

    // Attach a database for persistence.
    void attachDatabase(SqliteDB* db);

    // Detach the database (returns to memory-only mode).
    void detachDatabase();

    // Load events from database for this room.
    int loadFromDatabase(int limit = 100, int offset = 0);

    // ==== Linked-List Chunk Navigation ====

    // Link chunks after insertion — rebuilds prevChunkIdx/nextChunkIdx.
    void linkChunks();

    // Get the first (oldest) chunk index, or -1 if empty.
    int getFirstChunkIdx() const;

    // Get the last (newest) chunk index, or -1 if empty.
    int getLastChunkIdx() const;

    // Get chunk count.
    int chunkCount() const { return (int)chunks_.size(); }

    // ==== Pagination (loadMore delegation) ====

    // How many more events could be loaded in a direction.
    // Returns 0 if no more events can be loaded, or a suggested count.
    int eventsAvailable(TimelineDirection dir) const;

    // ==== Snapshot Building ====

    // Get all events in display order (oldest first) across all linked chunks.
    // For room timeline display. Equivalent to Kotlin builtItems().
    std::vector<TimelineEventData> getSnapshot(int limit = 0, int offset = 0) const;

    // Get events from a specific chunk by index.
    const TimelineChunkData* getChunk(int idx) const;

    // Clear all chunks (for room change).
    void clear();

    // ==== Display-Index Arithmetic ====
    //
    // Original Kotlin: TimelineChunk.displayIndex arithmetic
    // When inserting events between two chunks, computes the display indices
    // for the gap so that ordering is preserved without renumbering everything.

    // Compute display indices for a gap of `count` events between two indices.
    // beforeIndex: display index of last event before the gap (-1 if none)
    // afterIndex: display index of first event after the gap (INT_MAX if none)
    // Returns: list of display indices, evenly distributed in the gap
    static std::vector<int> computeDisplayIndices(
        int beforeIndex, int afterIndex, int count
    );

    // ==== Reply-Map Building ====
    //
    // Original Kotlin: builds a map of event_id → list of reply event IDs.
    // Used for threading and reply chain detection.

    // Build the reply map from current events.
    // Returns: eventId → list of events that reply to it.
    std::unordered_map<std::string, std::vector<std::string>> buildReplyMap() const;

    // Get all events that reply to a specific event.
    std::vector<std::string> getReplies(const std::string& eventId) const;

    // ==== Edit Chain Detection ====
    //
    // Find the latest version of an edited event.
    // Follows the m.replace relation chain to find the most recent edit.

    std::string getLatestEditEventId(const std::string& originalEventId) const;

    // Get all edit versions of an event (including original).
    std::vector<std::string> getEditChain(const std::string& eventId) const;

    // ==== Thread Detection ====

    // Find the root event of a thread.
    // Returns the root event ID, or empty if this is a root.
    std::string getThreadRoot(const std::string& eventId) const;

    // Get all events in a thread (including root).
    std::vector<std::string> getThreadEvents(const std::string& rootEventId) const;

private:
    std::string roomId_;
    TimelineMode mode_ = TimelineMode::LIVE;
    std::string anchorEventId_;
    SqliteDB* db_ = nullptr;       // Optional persistent storage
    std::vector<TimelineChunkData> chunks_;
    std::unordered_map<std::string, TimelineEventData> eventIndex_; // eventId → event data
    std::unordered_map<std::string, int> displayIndexMap_; // eventId → display index
    int globalNextDisplayIndex_ = 0;
    TimelineDirection lastPaginationDirection_ = TimelineDirection::BACKWARDS;

    // Internal: insert events into the chunk list in correct position
    void insertEvents(const std::vector<TimelineEventData>& events, const std::string& chunkId);

    // Internal: deduplicate events (skip already-known event IDs)
    std::vector<TimelineEventData> dedupEvents(const std::vector<TimelineEventData>& events) const;

    // Internal: find the chunk a chunk should be inserted relative to
    int findChunkInsertionIndex(const std::string& prevToken, const std::string& nextToken) const;

    // Internal: rebuild display indices after insertion
    void rebuildDisplayIndices();
};

// ==== Serialization ====

// Serialize the chunk manager state to JSON for persistence.
std::string serializeChunkManager(const TimelineChunkManager& mgr);

// Restore chunk manager from persisted state.
TimelineChunkManager deserializeChunkManager(const std::string& json, const std::string& roomId);

// ---- Chunk Load Result (from TokenChunkEventPersistor.kt) ----
// Original Kotlin: TokenChunkEventPersistor.Result (SHOULD_FETCH_MORE, REACHED_END, SUCCESS)
// Original Kotlin: insertInDb returns Result after chunk linking

struct ChunkLoadResult {
    TimelineChunkData chunk;
    int newEvents = 0;           // number of new events added in this load
    std::string prevToken;       // pagination token for older events
    std::string nextToken;       // pagination token for newer events
    bool isLimited = false;      // Original Kotlin: receivedChunk.hasMore() — server has more
    bool reachedForwardEnd = false;
    bool reachedBackwardEnd = false;
};

// Merge an incoming chunk into an existing chunk list via TimelineChunkManager.
// Handles dedup, linking, and token propagation.
// Original Kotlin: TokenChunkEventPersistor.insertInDb() — chunk linking logic
ChunkLoadResult mergeChunk(
    TimelineChunkManager& manager,
    const TimelineChunkData& incoming,
    TimelineDirection direction,
    const std::string& chunkId = "");

// Split a chunk into two at a specific event. The split event goes to the second chunk.
// Returns (beforeChunk, afterChunk). beforeChunk contains events before the split point.
// afterChunk contains the split event and events after it.
// Original Kotlin: used for permalink insertion when an event arrives mid-chunk
std::pair<TimelineChunkData, TimelineChunkData> splitChunk(
    const TimelineChunkData& chunk,
    const std::string& splitEventId);

// ---- Chunk Pagination Controller ----
// Manages chunk-based pagination (forward/backward/around).
// Wraps TimelineChunkManager with pagination token management and fetch delegation.
// Original Kotlin: TimelineChunk.loadMore() + PaginationTask + FetchTokenAndPaginateTask
// Original Kotlin: DefaultTimeline routes loadMore() → strategy.loadMore() → timelineChunk.loadMore()

class ChunkPaginationController {
public:
    explicit ChunkPaginationController(TimelineChunkManager* manager);

    // Load more events in the given direction. Returns paginated result.
    // Updates internal pagination tokens on success.
    // Original Kotlin: timelineChunk?.loadMore(count, direction, fetchOnServerIfNeeded)
    PaginationResult loadForward(int count = 20);
    PaginationResult loadBackward(int count = 20);

    // Open the timeline around a specific event (permalink mode).
    // Loads context around the event, then can page in both directions.
    // Original Kotlin: DefaultTimeline.openAround(eventId) + GetContextOfEventTask
    PaginationResult loadAround(const std::string& eventId, int count = 20);

    // Check if more events can be loaded in each direction.
    // Original Kotlin: DefaultTimeline.hasMoreToLoad(direction)
    bool hasMoreForward() const;
    bool hasMoreBackward() const;

    // Get the display range of currently loaded events.
    // Original Kotlin: used for UI viewport / RecyclerView range calculation
    TimelineRange getDisplayRange() const;

    // Set/get pagination tokens (used to resume pagination after context load).
    void setForwardToken(const std::string& token);
    void setBackwardToken(const std::string& token);
    std::string getForwardToken() const;
    std::string getBackwardToken() const;

    // Fetch callback: called when the controller needs to fetch events from the server.
    // Receives (from_token, direction_str, limit) and returns a PaginationResult.
    // Original Kotlin: PaginationTask.execute(Params(roomId, from, direction, limit))
    using FetchCallback = std::function<PaginationResult(
        const std::string& from, const std::string& direction, int limit)>;
    void setFetchCallback(FetchCallback cb);

    // Get current load state for external monitoring.
    // Original Kotlin: DefaultTimeline.getPaginationState(direction)
    TimelineLoadState getLoadState() const;

private:
    TimelineChunkManager* manager_;
    std::string forwardToken_;
    std::string backwardToken_;
    FetchCallback fetchCallback_;
    TimelineLoadState loadState_;
};

} // namespace progressive
