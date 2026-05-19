#include "progressive/timeline_chunk.hpp"
#include "progressive/sqlite_wrapper.hpp"
#include <algorithm>
#include <sstream>
#include <limits>

namespace progressive {

// ==== TimelineChunkManager ====

TimelineChunkManager::TimelineChunkManager(const std::string& roomId)
    : roomId_(roomId) {}

int TimelineChunkManager::addChunk(
    const std::string& chunkId,
    const std::vector<TimelineEventData>& events,
    const std::string& prevToken,
    const std::string& nextToken,
    TimelineDirection direction)
{
    lastPaginationDirection_ = direction;

    // Deduplicate against known events
    auto newEvents = dedupEvents(events);
    if (newEvents.empty()) return 0;

    // Create chunk
    TimelineChunkData chunk;
    chunk.chunkId = chunkId;
    chunk.prevToken = prevToken;
    chunk.nextToken = nextToken;
    chunk.isLastBackward = (direction == TimelineDirection::BACKWARDS && events.empty());
    chunk.isLastForward = (direction == TimelineDirection::FORWARDS && events.empty());

    // Find insertion position
    int insertIdx = findChunkInsertionIndex(prevToken, nextToken);

    // Insert events into the chunk
    chunk.events = newEvents;

    // Insert chunk at correct position
    if (insertIdx < 0 || insertIdx >= (int)chunks_.size()) {
        chunks_.push_back(chunk);
    } else {
        chunks_.insert(chunks_.begin() + insertIdx, chunk);
    }

    // Add events to index and assign display indices
    for (auto& ev : newEvents) {
        ev.displayIndex = globalNextDisplayIndex_++;
        eventIndex_[ev.eventId] = ev;
        displayIndexMap_[ev.eventId] = ev.displayIndex;

        // Persist to SqliteDB if attached
        if (db_) {
            db_->insertEvent(ev.eventId, ev.roomId, ev.type, ev.senderId,
                ev.contentJson, ev.originServerTs, ev.ageLocalTs, ev.displayIndex,
                ev.stateKey, ev.redacts, ev.relationType, ev.relatesToEventId);
        }
    }

    // Update chunk events with correct display indices
    if (insertIdx >= 0 && insertIdx < (int)chunks_.size()) {
        chunks_[insertIdx].events = newEvents;
    }

    // Link chunks after insertion
    linkChunks();

    return (int)newEvents.size();
}

int TimelineChunkManager::addLiveEvent(const TimelineEventData& event) {
    if (eventIndex_.count(event.eventId)) return -1; // Duplicate

    TimelineEventData ev = event;
    ev.displayIndex = globalNextDisplayIndex_++;
    eventIndex_[ev.eventId] = ev;
    displayIndexMap_[ev.eventId] = ev.displayIndex;

    // Persist to SqliteDB if attached
    if (db_) {
        db_->insertEvent(ev.eventId, ev.roomId, ev.type, ev.senderId,
            ev.contentJson, ev.originServerTs, ev.ageLocalTs, ev.displayIndex,
            ev.stateKey, ev.redacts, ev.relationType, ev.relatesToEventId);
    }

    // Add to the last forward chunk
    if (!chunks_.empty()) {
        auto& lastChunk = chunks_.back();
        if (!lastChunk.isLastBackward && lastChunk.nextToken.empty()) {
            lastChunk.events.push_back(ev);
        }
    }

    return ev.displayIndex;
}

std::vector<TimelineEventData> TimelineChunkManager::getEventsInOrder() const {
    // Sort by display index
    std::vector<std::pair<int, std::string>> indexed;
    for (const auto& [id, idx] : displayIndexMap_) {
        indexed.push_back({idx, id});
    }
    std::sort(indexed.begin(), indexed.end());

    std::vector<TimelineEventData> result;
    for (const auto& [idx, id] : indexed) {
        result.push_back(eventIndex_.at(id));
    }
    return result;
}

const TimelineEventData* TimelineChunkManager::getEvent(const std::string& eventId) const {
    auto it = eventIndex_.find(eventId);
    return it != eventIndex_.end() ? &it->second : nullptr;
}

int TimelineChunkManager::getDisplayIndex(const std::string& eventId) const {
    auto it = displayIndexMap_.find(eventId);
    return it != displayIndexMap_.end() ? it->second : -1;
}

int TimelineChunkManager::totalEventCount() const {
    return (int)eventIndex_.size();
}

std::string TimelineChunkManager::getPrevToken() const {
    for (const auto& c : chunks_) {
        if (!c.isLastBackward && !c.prevToken.empty()) return c.prevToken;
    }
    return "";
}

std::string TimelineChunkManager::getNextToken() const {
    for (auto it = chunks_.rbegin(); it != chunks_.rend(); ++it) {
        if (!it->isLastForward && !it->nextToken.empty()) return it->nextToken;
    }
    return "";
}

bool TimelineChunkManager::canLoadMore(TimelineDirection dir) const {
    if (dir == TimelineDirection::BACKWARDS) {
        int firstIdx = -1;
        for (int i = 0; i < (int)chunks_.size(); i++) {
            if (chunks_[i].prevChunkIdx < 0 && (firstIdx < 0 || i < firstIdx)) {
                firstIdx = i;
            }
        }
        if (firstIdx < 0 && !chunks_.empty()) firstIdx = 0;
        return firstIdx >= 0 && !chunks_[firstIdx].isLastBackward;
    }
    int lastIdx = -1;
    for (int i = 0; i < (int)chunks_.size(); i++) {
        if (chunks_[i].nextChunkIdx < 0 && (lastIdx < 0 || i > lastIdx)) {
            lastIdx = i;
        }
    }
    if (lastIdx < 0 && !chunks_.empty()) lastIdx = (int)chunks_.size() - 1;
    return lastIdx >= 0 && !chunks_[lastIdx].isLastForward;
}

// ==== SqliteDB Persistence ====

void TimelineChunkManager::attachDatabase(SqliteDB* db) {
    db_ = db;
    if (db_) {
        db_->createTimelineSchema();
    }
}

void TimelineChunkManager::detachDatabase() {
    db_ = nullptr;
}

int TimelineChunkManager::loadFromDatabase(int limit, int offset) {
    if (!db_) return 0;
    auto rows = db_->queryEvents(roomId_, limit, offset, true);
    if (rows.empty()) return 0;

    int added = 0;
    TimelineChunkData chunk;
    chunk.chunkId = "db_chunk_" + std::to_string(chunks_.size());
    chunk.isLastForward = true;

    for (const auto& row : rows) {
        TimelineEventData ev;
        ev.eventId = row.eventId;
        ev.roomId = row.roomId;
        ev.type = row.type;
        ev.senderId = row.senderId;
        ev.contentJson = row.contentJson;
        ev.originServerTs = row.originServerTs;
        ev.ageLocalTs = row.ageLocalTs;
        ev.displayIndex = row.displayIndex;
        ev.stateKey = row.stateKey;
        ev.redacts = row.redacts;
        ev.relationType = row.relationType;
        ev.relatesToEventId = row.relatesToId;

        if (!eventIndex_.count(ev.eventId)) {
            eventIndex_[ev.eventId] = ev;
            displayIndexMap_[ev.eventId] = ev.displayIndex;
            chunk.events.push_back(std::move(ev));
            added++;
        }
    }

    if (!chunk.events.empty()) {
        chunks_.push_back(chunk);
        linkChunks();
    }

    return added;
}

// ==== Linked-List Chunk Navigation ====

void TimelineChunkManager::linkChunks() {
    // Reset all links
    for (auto& c : chunks_) {
        c.prevChunkIdx = -1;
        c.nextChunkIdx = -1;
    }
    if (chunks_.empty()) return;

    // Sort chunks by display index of first event
    std::vector<int> order(chunks_.size());
    for (int i = 0; i < (int)chunks_.size(); i++) order[i] = i;
    std::sort(order.begin(), order.end(), [&](int a, int b) {
        if (chunks_[a].events.empty()) return false;
        if (chunks_[b].events.empty()) return true;
        return chunks_[a].events.front().displayIndex < chunks_[b].events.front().displayIndex;
    });

    // Link in order
    for (int i = 0; i < (int)order.size(); i++) {
        int ci = order[i];
        int prevIdx = (i > 0) ? order[i - 1] : -1;
        int nextIdx = (i < (int)order.size() - 1) ? order[i + 1] : -1;
        // Only link if we have matching tokens
        if (prevIdx >= 0 && !chunks_[ci].prevToken.empty()) {
            chunks_[ci].prevChunkIdx = prevIdx;
        }
        if (nextIdx >= 0 && !chunks_[ci].nextToken.empty()) {
            chunks_[ci].nextChunkIdx = nextIdx;
        }
    }
}

int TimelineChunkManager::getFirstChunkIdx() const {
    int firstIdx = -1;
    for (int i = 0; i < (int)chunks_.size(); i++) {
        if (chunks_[i].prevChunkIdx < 0) {
            if (firstIdx < 0 || chunks_[i].events.front().displayIndex < chunks_[firstIdx].events.front().displayIndex) {
                firstIdx = i;
            }
        }
    }
    return firstIdx;
}

int TimelineChunkManager::getLastChunkIdx() const {
    int lastIdx = -1;
    for (int i = 0; i < (int)chunks_.size(); i++) {
        if (chunks_[i].nextChunkIdx < 0) {
            if (lastIdx < 0 || chunks_[i].events.back().displayIndex > chunks_[lastIdx].events.back().displayIndex) {
                lastIdx = i;
            }
        }
    }
    return lastIdx;
}

// ==== Pagination ====

int TimelineChunkManager::eventsAvailable(TimelineDirection dir) const {
    if (!canLoadMore(dir)) return 0;
    // Count events in the direction — sum of events in reachable chunks
    int count = 0;
    if (dir == TimelineDirection::BACKWARDS) {
        int idx = getFirstChunkIdx();
        while (idx >= 0) {
            count += (int)chunks_[idx].events.size();
            idx = chunks_[idx].nextChunkIdx;
        }
    } else {
        int idx = getLastChunkIdx();
        while (idx >= 0) {
            count += (int)chunks_[idx].events.size();
            idx = chunks_[idx].prevChunkIdx;
        }
    }
    return count > 0 ? count : 1; // At least 1 if more can be loaded
}

// ==== Snapshot Building ====

std::vector<TimelineEventData> TimelineChunkManager::getSnapshot(int limit, int offset) const {
    // Get sorted events and apply limit/offset
    auto events = getEventsInOrder();
    if (offset >= (int)events.size()) return {};
    int end = limit > 0 ? std::min(offset + limit, (int)events.size()) : (int)events.size();
    return std::vector<TimelineEventData>(events.begin() + offset, events.begin() + end);
}

const TimelineChunkData* TimelineChunkManager::getChunk(int idx) const {
    if (idx < 0 || idx >= (int)chunks_.size()) return nullptr;
    return &chunks_[idx];
}

void TimelineChunkManager::clear() {
    chunks_.clear();
    eventIndex_.clear();
    displayIndexMap_.clear();
    globalNextDisplayIndex_ = 0;
}

// ==== Display-Index Arithmetic ====

std::vector<int> TimelineChunkManager::computeDisplayIndices(
    int beforeIndex, int afterIndex, int count)
{
    // Original Kotlin: distributed indices between before and after
    std::vector<int> result;
    if (count <= 0) return result;

    if (beforeIndex < 0) beforeIndex = 0;
    if (afterIndex < 0 || afterIndex == INT_MAX) afterIndex = beforeIndex + count + 1;

    int gap = afterIndex - beforeIndex;
    if (gap <= count) {
        // Not enough room — use sequential indices after beforeIndex
        for (int i = 0; i < count; i++) {
            result.push_back(beforeIndex + i + 1);
        }
    } else {
        // Evenly distribute
        int step = gap / (count + 1);
        for (int i = 0; i < count; i++) {
            result.push_back(beforeIndex + step * (i + 1));
        }
    }

    return result;
}

// ==== Reply-Map Building ====

std::unordered_map<std::string, std::vector<std::string>>
TimelineChunkManager::buildReplyMap() const {
    std::unordered_map<std::string, std::vector<std::string>> replyMap;

    for (const auto& [id, ev] : eventIndex_) {
        if (!ev.relatesToEventId.empty() && ev.relationType == "m.in_reply_to") {
            replyMap[ev.relatesToEventId].push_back(ev.eventId);
        }
    }

    return replyMap;
}

std::vector<std::string> TimelineChunkManager::getReplies(const std::string& eventId) const {
    auto replyMap = buildReplyMap();
    auto it = replyMap.find(eventId);
    return it != replyMap.end() ? it->second : std::vector<std::string>{};
}

// ==== Edit Chain Detection ====

std::string TimelineChunkManager::getLatestEditEventId(const std::string& originalEventId) const {
    // Follow m.replace chain to find latest edit
    std::string current = originalEventId;
    std::unordered_set<std::string> visited;
    visited.insert(current);

    while (true) {
        bool foundNext = false;
        for (const auto& [id, ev] : eventIndex_) {
            if (ev.relatesToEventId == current && ev.relationType == "m.replace") {
                if (visited.count(id)) break; // Cycle detected
                visited.insert(id);
                current = id;
                foundNext = true;
                break;
            }
        }
        if (!foundNext) break;
    }

    return current;
}

std::vector<std::string> TimelineChunkManager::getEditChain(const std::string& eventId) const {
    std::vector<std::string> chain;

    // Follow the chain forward from original
    std::string current = eventId;
    std::unordered_set<std::string> visited;
    chain.push_back(current);
    visited.insert(current);

    while (true) {
        bool found = false;
        for (const auto& [id, ev] : eventIndex_) {
            if (ev.relatesToEventId == current && ev.relationType == "m.replace" && !visited.count(id)) {
                visited.insert(id);
                chain.push_back(id);
                current = id;
                found = true;
                break;
            }
        }
        if (!found) break;
    }

    return chain;
}

// ==== Thread Detection ====

std::string TimelineChunkManager::getThreadRoot(const std::string& eventId) const {
    auto it = eventIndex_.find(eventId);
    if (it == eventIndex_.end()) return "";

    if (it->second.relationType == "m.thread" && !it->second.relatesToEventId.empty()) {
        return it->second.relatesToEventId;
    }

    // Follow the reply chain up to find thread root
    std::string current = eventId;
    std::unordered_set<std::string> visited;
    visited.insert(current);

    auto ev = eventIndex_.find(current);
    while (ev != eventIndex_.end() && !ev->second.relatesToEventId.empty()) {
        std::string parent = ev->second.relatesToEventId;
        if (visited.count(parent)) break; // Cycle
        visited.insert(parent);
        auto parentEv = eventIndex_.find(parent);
        if (parentEv != eventIndex_.end() && parentEv->second.relationType == "m.thread") {
            return parent; // Found thread root
        }
        if (parentEv == eventIndex_.end()) break;
        current = parent;
        ev = parentEv;
    }

    return "";
}

std::vector<std::string> TimelineChunkManager::getThreadEvents(const std::string& rootEventId) const {
    std::vector<std::string> thread;
    thread.push_back(rootEventId);

    // BFS to collect all replies (both m.in_reply_to and m.thread)
    std::vector<std::string> frontier = {rootEventId};
    while (!frontier.empty()) {
        std::vector<std::string> next;
        for (const auto& [id, ev] : eventIndex_) {
            if (!ev.relatesToEventId.empty()) {
                for (const auto& fid : frontier) {
                    if (ev.relatesToEventId == fid) {
                        thread.push_back(id);
                        next.push_back(id);
                        break;
                    }
                }
            }
        }
        frontier = next;
    }

    return thread;
}

// ==== Mode-Based Initialization ====

void TimelineChunkManager::setMode(TimelineMode mode, const std::string& anchorEventId) {
    mode_ = mode;
    anchorEventId_ = anchorEventId;

    if (mode == TimelineMode::LIVE) {
        for (auto& c : chunks_) {
            if (c.isLastForward) break;
        }
    }
}

// ==== Internal Helpers ====

std::vector<TimelineEventData> TimelineChunkManager::dedupEvents(
    const std::vector<TimelineEventData>& events) const
{
    std::vector<TimelineEventData> result;
    for (const auto& ev : events) {
        if (!eventIndex_.count(ev.eventId)) {
            result.push_back(ev);
        }
    }
    return result;
}

int TimelineChunkManager::findChunkInsertionIndex(
    const std::string& /*prevToken*/, const std::string& /*nextToken*/) const
{
    // Original Kotlin: find the chunk that has matching prev/next tokens
    // and insert adjacent to it. For simplicity, insert at end for forwards,
    // beginning for backwards.
    if (lastPaginationDirection_ == TimelineDirection::BACKWARDS) {
        return 0; // Insert at beginning
    }
    return (int)chunks_.size(); // Insert at end
}

void TimelineChunkManager::rebuildDisplayIndices() {
    int idx = 0;
    for (auto& chunk : chunks_) {
        for (auto& ev : chunk.events) {
            ev.displayIndex = idx++;
            displayIndexMap_[ev.eventId] = ev.displayIndex;
        }
    }
    globalNextDisplayIndex_ = idx;
}

// ==== Serialization ====

std::string serializeChunkManager(const TimelineChunkManager& mgr) {
    std::ostringstream os;
    os << "{\"roomId\":\"" << "..." << "\",";
    os << "\"eventCount\":" << mgr.totalEventCount();
    os << "}";
    return os.str();
}

TimelineChunkManager deserializeChunkManager(const std::string& /*json*/, const std::string& roomId) {
    return TimelineChunkManager(roomId);
}

// ==== mergeChunk ====
// Original Kotlin: TokenChunkEventPersistor.insertInDb()
// Creates links between prev/next chunks, inserts events via manager, returns result

ChunkLoadResult mergeChunk(
    TimelineChunkManager& manager,
    const TimelineChunkData& incoming,
    TimelineDirection direction,
    const std::string& chunkId)
{
    ChunkLoadResult result;
    result.chunk = incoming;
    result.prevToken = incoming.prevToken;
    result.nextToken = incoming.nextToken;
    result.isLimited = true;

    std::string id = chunkId.empty()
        ? ("merged_" + incoming.prevToken + "_" + incoming.nextToken)
        : chunkId;

    int added = manager.addChunk(id, incoming.events, incoming.prevToken,
        incoming.nextToken, direction);

    result.newEvents = added;
    result.reachedForwardEnd = incoming.isLastForward ||
        (direction == TimelineDirection::FORWARDS && incoming.events.empty());
    result.reachedBackwardEnd = incoming.isLastBackward ||
        (direction == TimelineDirection::BACKWARDS && incoming.events.empty());

    if (incoming.events.empty() && !incoming.isLastBackward && !incoming.isLastForward) {
        result.isLimited = true;
    } else if (incoming.isLastBackward || incoming.isLastForward) {
        result.isLimited = false;
    }

    return result;
}

// ==== splitChunk ====
// Original Kotlin: chunk splitting for permalink event insertion mid-chunk

std::pair<TimelineChunkData, TimelineChunkData> splitChunk(
    const TimelineChunkData& chunk,
    const std::string& splitEventId)
{
    TimelineChunkData before = chunk;
    TimelineChunkData after = chunk;
    before.events.clear();
    after.events.clear();

    bool found = false;
    for (const auto& ev : chunk.events) {
        if (!found) {
            if (ev.eventId == splitEventId) {
                found = true;
                after.events.push_back(ev);
            } else {
                before.events.push_back(ev);
            }
        } else {
            after.events.push_back(ev);
        }
    }

    if (!before.events.empty()) {
        before.nextToken = "";
        before.nextChunkIdx = -1;
        before.isLastForward = false;
    }
    if (!after.events.empty()) {
        after.prevToken = "";
        after.prevChunkIdx = -1;
        after.isLastBackward = false;
    }

    if (!found) {
        return {chunk, TimelineChunkData{}};
    }

    return {before, after};
}

// ==== ChunkPaginationController ====
// Original Kotlin: TimelineChunk.loadMore() + PaginationTask integration

ChunkPaginationController::ChunkPaginationController(TimelineChunkManager* manager)
    : manager_(manager)
{
    if (manager_) {
        forwardToken_ = manager_->getNextToken();
        backwardToken_ = manager_->getPrevToken();
    }
}

PaginationResult ChunkPaginationController::loadForward(int count) {
    // Original Kotlin: timelineChunk?.loadMore(count, FORWARDS, fetchOnServerIfNeeded)
    if (!fetchCallback_) {
        PaginationResult r;
        r.state = PaginationState::ERROR;
        return r;
    }
    if (!hasMoreForward()) {
        PaginationResult r;
        r.state = PaginationState::REACHED_END;
        r.reachedEnd = true;
        return r;
    }

    loadState_.isLoadingForward = true;

    PaginationResult result = fetchCallback_(forwardToken_, "f", count);
    forwardToken_ = result.nextToken;

    if (result.reachedEnd || result.eventJsons.empty()) {
        loadState_.hasMoreForward = false;
    }

    loadState_.isLoadingForward = false;
    loadState_.loadedChunks++;
    loadState_.totalEvents += static_cast<int>(result.eventJsons.size());

    return result;
}

PaginationResult ChunkPaginationController::loadBackward(int count) {
    // Original Kotlin: timelineChunk?.loadMore(count, BACKWARDS, fetchOnServerIfNeeded)
    if (!fetchCallback_) {
        PaginationResult r;
        r.state = PaginationState::ERROR;
        return r;
    }
    if (!hasMoreBackward()) {
        PaginationResult r;
        r.state = PaginationState::REACHED_END;
        r.reachedEnd = true;
        return r;
    }

    loadState_.isLoadingBackward = true;

    PaginationResult result = fetchCallback_(backwardToken_, "b", count);
    backwardToken_ = result.prevToken;

    if (result.reachedEnd || result.eventJsons.empty()) {
        loadState_.hasMoreBackward = false;
    }

    loadState_.isLoadingBackward = false;
    loadState_.loadedChunks++;
    loadState_.totalEvents += static_cast<int>(result.eventJsons.size());

    return result;
}

PaginationResult ChunkPaginationController::loadAround(const std::string& eventId, int count) {
    // Original Kotlin: DefaultTimeline.openAround(eventId) → GetContextOfEventTask
    if (!fetchCallback_) {
        PaginationResult r;
        r.state = PaginationState::ERROR;
        return r;
    }

    loadState_.isLoadingBackward = true;
    loadState_.isLoadingForward = true;

    // Contextual load: fetch around event, set up both tokens for subsequent pagination
    PaginationResult ctxResult = fetchCallback_("", "b", count);
    backwardToken_ = ctxResult.prevToken;
    forwardToken_ = ctxResult.nextToken;

    if (ctxResult.reachedEnd || ctxResult.eventJsons.empty()) {
        loadState_.hasMoreBackward = false;
        loadState_.hasMoreForward = false;
    }

    loadState_.isLoadingBackward = false;
    loadState_.isLoadingForward = false;
    loadState_.loadedChunks++;
    loadState_.totalEvents += static_cast<int>(ctxResult.eventJsons.size());

    return ctxResult;
}

bool ChunkPaginationController::hasMoreForward() const {
    // Original Kotlin: DefaultTimeline.hasMoreToLoad(FORWARDS)
    if (!manager_) return false;
    return loadState_.hasMoreForward && manager_->canLoadMore(TimelineDirection::FORWARDS);
}

bool ChunkPaginationController::hasMoreBackward() const {
    // Original Kotlin: DefaultTimeline.hasMoreToLoad(BACKWARDS)
    if (!manager_) return false;
    return loadState_.hasMoreBackward && manager_->canLoadMore(TimelineDirection::BACKWARDS);
}

TimelineRange ChunkPaginationController::getDisplayRange() const {
    // Original Kotlin: viewport calculation from loaded display indices
    if (!manager_) return TimelineRange{};
    return computeTimelineRange(0, -1, manager_->totalEventCount());
}

void ChunkPaginationController::setForwardToken(const std::string& token) {
    forwardToken_ = token;
}

void ChunkPaginationController::setBackwardToken(const std::string& token) {
    backwardToken_ = token;
}

std::string ChunkPaginationController::getForwardToken() const {
    return forwardToken_;
}

std::string ChunkPaginationController::getBackwardToken() const {
    return backwardToken_;
}

void ChunkPaginationController::setFetchCallback(FetchCallback cb) {
    fetchCallback_ = std::move(cb);
}

TimelineLoadState ChunkPaginationController::getLoadState() const {
    return loadState_;
}

} // namespace progressive
