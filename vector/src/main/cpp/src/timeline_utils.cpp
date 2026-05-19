#include "progressive/timeline_utils.hpp"
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>

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

} // namespace progressive
