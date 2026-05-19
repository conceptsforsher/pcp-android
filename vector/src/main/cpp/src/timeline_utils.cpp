#include "progressive/timeline_utils.hpp"
#include "progressive/timeline_chunk.hpp"
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <unordered_set>

namespace progressive {

std::vector<std::string> mergeTimelineChunks(const std::vector<TimelineChunk>& chunks) {
    if (chunks.empty()) return {};

    auto sorted = chunks;
    sortChunksByPosition(sorted);

    std::vector<std::string> allEvents;
    for (const auto& chunk : sorted) {
        for (const auto& eid : chunk.eventIds) {
            // Avoid duplicates at chunk boundaries
            if (allEvents.empty() || allEvents.back() != eid) {
                allEvents.push_back(eid);
            }
        }
    }

    return allEvents;
}

std::vector<std::pair<std::string, std::string>> detectChunkGaps(
    const std::vector<TimelineChunk>& chunks) {
    std::vector<std::pair<std::string, std::string>> gaps;

    if (chunks.size() < 2) return gaps;

    auto sorted = chunks;
    sortChunksByPosition(sorted);

    for (size_t i = 1; i < sorted.size(); ++i) {
        const auto& prev = sorted[i - 1];
        const auto& curr = sorted[i];

        // Check if prev's last event matches curr's first event
        if (!prev.eventIds.empty() && !curr.eventIds.empty()) {
            if (prev.eventIds.back() != curr.eventIds.front()) {
                gaps.push_back({prev.eventIds.back(), curr.eventIds.front()});
            }
        }
    }

    return gaps;
}

void sortChunksByPosition(std::vector<TimelineChunk>& chunks) {
    std::sort(chunks.begin(), chunks.end(), [](const TimelineChunk& a, const TimelineChunk& b) {
        // Backward chunks first, then forward chunks
        if (a.isLastBackward != b.isLastBackward) return a.isLastBackward;
        if (a.isLastForward != b.isLastForward) return !a.isLastForward;
        return a.eventCount < b.eventCount;
    });
}

bool chunkContainsEvent(const TimelineChunk& chunk, const std::string& eventId) {
    for (const auto& eid : chunk.eventIds) {
        if (eid == eventId) return true;
    }
    return false;
}

int getTotalChunkEvents(const std::vector<TimelineChunk>& chunks) {
    int total = 0;
    for (const auto& c : chunks) total += c.eventCount;
    return total;
}

void sortByStreamOrder(std::vector<OrderedEvent>& events) {
    std::sort(events.begin(), events.end(), [](const OrderedEvent& a, const OrderedEvent& b) {
        if (a.streamOrder != b.streamOrder) return a.streamOrder < b.streamOrder;
        return a.originServerTs < b.originServerTs;
    });
}

void sortByTimestamp(std::vector<OrderedEvent>& events) {
    std::sort(events.begin(), events.end(), [](const OrderedEvent& a, const OrderedEvent& b) {
        return a.originServerTs < b.originServerTs;
    });
}

std::string computeOrderingKey(int64_t timestamp, int streamOrder) {
    std::ostringstream key;
    key << std::setfill('0') << std::setw(16) << timestamp
        << ":" << std::setw(10) << streamOrder;
    return key.str();
}

bool shouldAutoScroll(const LiveTimelineState& state, bool newEventIsFromMe) {
    if (newEventIsFromMe) return true;     // always scroll for own messages
    if (state.shouldJumpToBottom) return true;
    return false;
}

LiveTimelineState updateScrollState(const LiveTimelineState& state, int64_t nowMs) {
    LiveTimelineState updated = state;
    updated.lastUserScrollMs = nowMs;

    // If user hasn't scrolled in 30 seconds, resume auto-scroll
    if (nowMs - state.lastUserScrollMs > 30000) {
        updated.shouldJumpToBottom = true;
    }

    return updated;
}

bool hasScrolledAway(const LiveTimelineState& state, int visibleFirstIndex,
    int totalEvents, int thresholdFromEnd) {
    if (totalEvents <= 0) return false;
    int eventsFromEnd = totalEvents - visibleFirstIndex;
    return eventsFromEnd > thresholdFromEnd;
}

// ==== Loading Progress Indicator ====
LoadingProgress computeLoadingProgress(int loaded, int rendered) {
    LoadingProgress prog;
    prog.eventsLoaded = loaded;
    prog.eventsRendered = rendered;
    prog.eventsPending = loaded - rendered;
    if (prog.eventsPending < 0) prog.eventsPending = 0;
    prog.isLoading = prog.eventsPending > 0;

    // Label for spinner center: show pending count, cap at 99+
    if (prog.eventsPending > 99) prog.label = "99+";
    else if (prog.eventsPending > 0) prog.label = std::to_string(prog.eventsPending);
    else prog.label = "";

    return prog;
}

std::string loadingProgressToJson(const LoadingProgress& prog) {
    std::ostringstream json;
    json << R"({"eventsLoaded": )" << prog.eventsLoaded << ",";
    json << R"("eventsRendered": )" << prog.eventsRendered << ",";
    json << R"("eventsPending": )" << prog.eventsPending << ",";
    json << R"("isLoading": )" << (prog.isLoading ? "true" : "false") << ",";
    json << R"("label": ")" << prog.label << R"(")";
    json << "}";
    return json.str();
}

// ==== Internal JSON Helpers (manual parsing, no library dependency) ====

namespace {

// Original Kotlin: GSON deserialization; we parse manually for NDK compatibility
std::string jsonGetValue(const std::string& json, const std::string& key) {
    std::string search = "\"" + key + "\"";
    auto pos = json.find(search);
    if (pos == std::string::npos) return "";

    pos += search.size();
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n' || json[pos] == '\r')) pos++;
    if (pos < json.size() && json[pos] == ':') pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n' || json[pos] == '\r')) pos++;
    if (pos >= json.size()) return "";

    if (json[pos] == '"') {
        size_t end = pos + 1;
        while (end < json.size()) {
            if (json[end] == '\\') { end += 2; continue; }
            if (json[end] == '"') break;
            end++;
        }
        return json.substr(pos + 1, end - pos - 1);
    }
    if (json[pos] == '{' || json[pos] == '[') {
        char open = json[pos];
        char close = (open == '{') ? '}' : ']';
        int depth = 1;
        size_t end = pos + 1;
        while (end < json.size()) {
            if (json[end] == open) depth++;
            else if (json[end] == close) depth--;
            if (depth == 0) break;
            end++;
        }
        return json.substr(pos, end - pos + 1);
    }
    // Number, boolean, or null
    size_t end = pos;
    while (end < json.size() && json[end] != ',' && json[end] != '}' && json[end] != ']' &&
           json[end] != ' ' && json[end] != '\n' && json[end] != '\r' && json[end] != '\t') end++;
    return json.substr(pos, end - pos);
}

std::string jsonGetString(const std::string& json, const std::string& key) {
    return jsonGetValue(json, key);
}

int64_t jsonGetLong(const std::string& json, const std::string& key) {
    auto val = jsonGetValue(json, key);
    if (val.empty()) return 0;
    try { return std::stoll(val); } catch (...) { return 0; }
}

std::vector<std::string> jsonGetArray(const std::string& json, const std::string& key) {
    auto arr = jsonGetValue(json, key);
    std::vector<std::string> result;
    if (arr.empty() || arr[0] != '[') return result;

    size_t pos = 1;
    while (pos < arr.size()) {
        while (pos < arr.size() && (arr[pos] == ' ' || arr[pos] == '\n' || arr[pos] == '\r' || arr[pos] == '\t')) pos++;
        if (pos >= arr.size() || arr[pos] == ']') break;

        if (arr[pos] == '{') {
            size_t start = pos;
            int depth = 1;
            pos++;
            while (pos < arr.size() && depth > 0) {
                if (arr[pos] == '{') depth++;
                else if (arr[pos] == '}') depth--;
                pos++;
            }
            result.push_back(arr.substr(start, pos - start));
        } else if (arr[pos] == '"') {
            pos++;
            size_t start = pos;
            while (pos < arr.size() && !(arr[pos] == '"' && arr[pos-1] != '\\')) pos++;
            result.push_back(arr.substr(start, pos - start));
            if (pos < arr.size()) pos++;
        } else {
            size_t start = pos;
            while (pos < arr.size() && arr[pos] != ',' && arr[pos] != ']') pos++;
            result.push_back(arr.substr(start, pos - start));
        }

        while (pos < arr.size() && (arr[pos] == ' ' || arr[pos] == '\n' || arr[pos] == '\r' || arr[pos] == '\t')) pos++;
        if (pos < arr.size() && arr[pos] == ',') pos++;
    }
    return result;
}

} // anonymous namespace

// ==== Pagination Request Builders ====
// Original Kotlin: PaginationTask extends Task<Params, Result>
// Original Kotlin: roomAPI.getRoomMessagesFrom(roomId, from, direction.value, limit, filter)

std::string buildPaginationRequest(const PaginationOptions& opts) {
    std::ostringstream req;
    req << "from=" << opts.from;
    req << "&dir=" << opts.direction;
    req << "&limit=" << opts.limit;
    if (!opts.to.empty()) req << "&to=" << opts.to;
    if (!opts.filter.empty()) req << "&filter=" << opts.filter;
    return req.str();
}

PaginationResult parsePaginationResponse(const std::string& json) {
    // Original Kotlin: TokenChunkEventPersistor.insertInDb(receivedChunk, roomId, direction)
    // Response shape: {"start":"...", "end":"...", "chunk":[...], "state":[...]}
    PaginationResult result;
    result.prevToken = jsonGetString(json, "start");
    result.nextToken = jsonGetString(json, "end");
    result.eventJsons = jsonGetArray(json, "chunk");
    result.reachedEnd = result.eventJsons.empty();
    result.state = result.eventJsons.empty() ? PaginationState::REACHED_END : PaginationState::LOADED;
    return result;
}

// Original Kotlin: GetContextOfEventTask (fetches /context/{eventId})
std::string buildContextRequest(const std::string& /*roomId*/, const std::string& /*eventId*/, int limit) {
    std::ostringstream req;
    req << "limit=" << limit;
    return req.str();
}

ContextResult parseContextResponse(const std::string& json) {
    // Original Kotlin: /context response: start, end, events_before, event, events_after, state
    ContextResult result;
    result.prevToken = jsonGetString(json, "start");
    result.nextToken = jsonGetString(json, "end");
    result.eventsBeforeJson = jsonGetArray(json, "events_before");
    result.eventJson = jsonGetValue(json, "event");
    result.eventsAfterJson = jsonGetArray(json, "events_after");
    result.state = PaginationState::LOADED;
    return result;
}

// Original Kotlin: fetching a single event by ID
std::string buildEventByIdRequest(const std::string& /*roomId*/, const std::string& /*eventId*/) {
    return ""; // No query params needed for /event endpoint
}

std::string parseEventByIdResponse(const std::string& json) {
    // Original Kotlin: the response IS the event JSON object
    return json;
}

// ==== Event JSON Parser ====
// Original Kotlin: TimelineEventMapper maps Entity → TimelineEvent
// We parse the raw Matrix event JSON manually for NDK compatibility

TimelineEventData parseTimelineEventFromJson(const std::string& json) {
    // Original Kotlin: event.toEntity(roomId, SendState.SYNCED, ageLocalTs).copyToRealmOrIgnore()
    TimelineEventData ev;
    ev.eventId = jsonGetString(json, "event_id");
    ev.roomId = jsonGetString(json, "room_id");
    ev.senderId = jsonGetString(json, "sender");
    ev.type = jsonGetString(json, "type");
    ev.contentJson = jsonGetValue(json, "content");
    ev.originServerTs = jsonGetLong(json, "origin_server_ts");

    // Original Kotlin: val ageLocalTs = now - (event.unsignedData?.age ?: 0)
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    auto unsignedData = jsonGetValue(json, "unsigned");
    if (!unsignedData.empty()) {
        int64_t age = 0;
        try { age = std::stoll(jsonGetString(unsignedData, "age")); } catch (...) {}
        ev.ageLocalTs = now - age;
    } else {
        ev.ageLocalTs = ev.originServerTs;
    }

    ev.stateKey = jsonGetString(json, "state_key");
    ev.isState = !ev.stateKey.empty() ||
        (ev.type.find("m.room.") == 0 && ev.type.find("m.room.message") != 0 &&
         ev.type.find("m.room.encrypted") != 0);

    ev.redacts = jsonGetString(json, "redacts");

    // Original Kotlin: m.relates_to extraction from content
    auto contentJson = jsonGetValue(json, "content");
    if (!contentJson.empty()) {
        auto relatesTo = jsonGetValue(contentJson, "m.relates_to");
        if (!relatesTo.empty()) {
            ev.relationType = jsonGetString(relatesTo, "rel_type");
            ev.relatesToEventId = jsonGetString(relatesTo, "event_id");
        }
    }

    return ev;
}

// ==== Timeline Loading Strategy ====
// Original Kotlin: LoadTimelineStrategy.loadMore(count, direction, fetchOnServerIfNeeded)

LoadStrategy computeLoadStrategy(const TimelineLoadState& state, bool isLive) {
    // Original Kotlin: DefaultTimeline decides direction based on isLive and state
    if (isLive) {
        // Live timeline: only paginate backward (older events); forward comes from /sync
        if (state.hasMoreBackward && !state.isLoadingBackward) {
            return LoadStrategy::BACKWARD;
        }
    } else {
        // Past / permalink timeline: load backward first, then forward
        if (state.hasMoreBackward && !state.isLoadingBackward) {
            return LoadStrategy::BACKWARD;
        }
        if (state.hasMoreForward && !state.isLoadingForward) {
            return LoadStrategy::FORWARD;
        }
    }
    return LoadStrategy::BACKWARD;
}

bool shouldLoadMore(const TimelineLoadState& state, LoadStrategy dir) {
    // Original Kotlin: DefaultTimeline.hasMoreToLoad(direction)
    if (dir == LoadStrategy::FORWARD || dir == LoadStrategy::PERMALINK) {
        return state.hasMoreForward && !state.isLoadingForward;
    }
    if (dir == LoadStrategy::BACKWARD) {
        return state.hasMoreBackward && !state.isLoadingBackward;
    }
    return state.hasMoreBackward && !state.isLoadingBackward;
}

int getNextPageLimit(const TimelineLoadState& /*state*/, int defaultLimit) {
    // Original Kotlin: TimelineSettings.initialSize
    return std::min(defaultLimit, 100);
}

EstimateLoadedRange estimateLoadedRange(
    const std::vector<TimelineEventData>& events,
    int startIdx,
    int count)
{
    // Original Kotlin: range calculation from display indices
    EstimateLoadedRange range;
    if (events.empty()) return range;

    int endIdx = (count < 0) ? static_cast<int>(events.size()) : std::min(startIdx + count, static_cast<int>(events.size()));
    if (startIdx >= endIdx || startIdx < 0) return range;

    range.oldestTs = events[startIdx].originServerTs;
    range.newestTs = events[startIdx].originServerTs;
    range.oldestIndex = events[startIdx].displayIndex;
    range.newestIndex = events[startIdx].displayIndex;

    for (int i = startIdx; i < endIdx; i++) {
        if (events[i].originServerTs < range.oldestTs) {
            range.oldestTs = events[i].originServerTs;
            range.oldestIndex = events[i].displayIndex;
        }
        if (events[i].originServerTs > range.newestTs) {
            range.newestTs = events[i].originServerTs;
            range.newestIndex = events[i].displayIndex;
        }
    }
    return range;
}

// ==== Timeline Event Processing ====
// Original Kotlin: TimelineChunk builtItems() merging + dedup + displayIndex ordering

int TimelineEventProcessor::insertTimelineEvents(
    std::vector<TimelineEventData>& target,
    const std::vector<TimelineEventData>& incoming,
    bool dedup)
{
    // Original Kotlin: ChunkEntity.addTimelineEvent(eventEntity, direction, ...)
    int inserted = 0;

    std::unordered_set<std::string> known;
    if (dedup) {
        for (const auto& ev : target) {
            known.insert(ev.eventId);
        }
    }

    for (const auto& ev : incoming) {
        if (dedup && known.count(ev.eventId)) continue;

        auto it = std::lower_bound(target.begin(), target.end(), ev,
            [](const TimelineEventData& a, const TimelineEventData& b) {
                return a.displayIndex < b.displayIndex;
            });
        target.insert(it, ev);
        if (dedup) known.insert(ev.eventId);
        inserted++;
    }

    return inserted;
}

std::vector<TimelineEventData> TimelineEventProcessor::mergeTimelineEvents(
    const std::vector<TimelineEventData>& existing,
    const std::vector<TimelineEventData>& incoming,
    bool sortByDisplayIndex)
{
    // Original Kotlin: merge builtItems() from linked chunks (includesNext/includesPrev)
    std::vector<TimelineEventData> result;
    std::unordered_set<std::string> seen;

    for (const auto& ev : existing) {
        if (seen.insert(ev.eventId).second) {
            result.push_back(ev);
        }
    }
    for (const auto& ev : incoming) {
        if (seen.insert(ev.eventId).second) {
            result.push_back(ev);
        }
    }

    if (sortByDisplayIndex) {
        sortTimelineEvents(result);
    }

    return result;
}

void TimelineEventProcessor::sortTimelineEvents(std::vector<TimelineEventData>& events) {
    // Original Kotlin: displayIndex ordering in builtItems()
    std::sort(events.begin(), events.end(),
        [](const TimelineEventData& a, const TimelineEventData& b) {
            return a.displayIndex < b.displayIndex;
        });
}

// ==== Timeline Gap Detection ====
// Original Kotlin: chunk boundary gap detection in TimelineChunk.kt

TimelineGap getTimelineGap(
    const TimelineEventData& before,
    const TimelineEventData& after,
    int avgBatchSize)
{
    // Original Kotlin: detects continuity gaps between consecutive events
    TimelineGap gap;
    gap.beforeEventId = before.eventId;
    gap.afterEventId = after.eventId;

    int64_t tsDiff = after.originServerTs - before.originServerTs;
    if (tsDiff < 0) tsDiff = 0;
    gap.timeGapMs = tsDiff;

    // Heuristic: displayIndex skip indicates missing events
    int diDiff = after.displayIndex - before.displayIndex;
    if (diDiff > 1) {
        gap.missingCount = diDiff - 1;
    } else if (tsDiff > 60000) { // > 1min gap suggests missing events
        gap.missingCount = std::min(static_cast<int>(tsDiff / 30000), avgBatchSize);
    }

    return gap;
}

// ==== Timeline Range Calculation ====
// Original Kotlin: viewport range for UI rendering

TimelineRange computeTimelineRange(int visibleStart, int visibleEnd, int totalEvents) {
    // Original Kotlin: determines visible event range for RecyclerView / lazy list
    TimelineRange range;
    range.totalEvents = std::max(0, totalEvents);
    if (range.totalEvents == 0) return range;

    range.startIndex = std::max(0, visibleStart);
    if (visibleEnd < 0) visibleEnd = totalEvents - 1;
    range.endIndex = std::min(totalEvents - 1, visibleEnd);
    if (range.endIndex < range.startIndex) {
        range.endIndex = range.startIndex;
    }
    return range;
}

} // namespace progressive
