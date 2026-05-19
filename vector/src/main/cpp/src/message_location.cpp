#include "progressive/message_location.hpp"
#include <vector>
#include <sstream>
#include <algorithm>
#include <chrono>

namespace progressive {

TimelinePosition computeTimelinePosition(const std::string& eventId, int index,
    int totalLoaded, int estimatedTotal) {
    TimelinePosition pos;
    pos.eventId = eventId;
    pos.index = index;
    pos.totalLoaded = totalLoaded;
    pos.isLoaded = (index >= 0 && index < totalLoaded);

    if (pos.isLoaded && totalLoaded > 0) {
        pos.isFirstEvent = (index == 0);
        pos.isLastEvent = (index == totalLoaded - 1);
        pos.scrollPercent = (index * 100.0) / totalLoaded;
    }

    if (estimatedTotal > 0) {
        pos.eventsBefore = std::max(0, index);
        pos.eventsAfter = std::max(0, estimatedTotal - index - 1);
        if (estimatedTotal > 0) {
            pos.scrollPercent = (index * 100.0) / estimatedTotal;
        }
    }

    return pos;
}

int estimateTotalEvents(int loadedEvents, bool hasMoreBackward, bool hasMoreForward,
    int64_t roomCreateTs, int64_t oldestEventTs, double avgMsgPerDay) {
    if (loadedEvents <= 0) return 0;
    if (!hasMoreBackward && !hasMoreForward) return loadedEvents; // complete history

    // Estimate from time range
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();

    int64_t totalMs = now - (roomCreateTs > 0 ? roomCreateTs : oldestEventTs);
    if (totalMs <= 0) return loadedEvents;

    double days = totalMs / (1000.0 * 86400.0);
    int estimated = static_cast<int>(days * avgMsgPerDay);

    return std::max(loadedEvents, estimated);
}

bool isEventInWindow(int index, int totalLoaded) {
    return index >= 0 && index < totalLoaded;
}

int eventDistance(int indexA, int indexB) {
    return std::abs(indexA - indexB);
}

JumpTarget computeJumpTarget(const std::string& targetEventId,
    const std::vector<std::string>& loadedEventIds, bool hasMoreBackward,
    bool hasMoreForward, int pageSize) {
    JumpTarget target;
    target.eventId = targetEventId;

    // Check if already loaded
    for (size_t i = 0; i < loadedEventIds.size(); ++i) {
        if (loadedEventIds[i] == targetEventId) {
            target.isLoaded = true;
            return target;
        }
    }

    // Not loaded — need pagination
    target.needsPagination = true;

    if (hasMoreBackward && !loadedEventIds.empty()) {
        target.direction = "backward";
        target.prevEventId = loadedEventIds.front();
        target.estimatedPages = estimatePaginationRequests(1000, pageSize);
    } else if (hasMoreForward && !loadedEventIds.empty()) {
        target.direction = "forward";
        target.nextEventId = loadedEventIds.back();
        target.estimatedPages = estimatePaginationRequests(1000, pageSize);
    } else {
        target.direction = "backward";
        target.estimatedPages = 1;
    }

    return target;
}

int estimatePaginationRequests(int missingEvents, int pageSize) {
    if (pageSize <= 0) return 1;
    return (missingEvents + pageSize - 1) / pageSize;
}

} // namespace progressive
