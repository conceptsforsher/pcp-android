#ifndef PROGRESSIVE_MESSAGE_LOCATION_HPP
#define PROGRESSIVE_MESSAGE_LOCATION_HPP

#include <string>
#include <cstdint>

namespace progressive {

// ---- Message Location in Timeline ----

struct TimelinePosition {
    std::string eventId;
    int index = -1;                // position in loaded timeline (-1 = not loaded)
    int totalLoaded = 0;           // how many events are loaded
    bool isLoaded = false;
    bool isFirstEvent = false;
    bool isLastEvent = false;
    int eventsBefore = 0;          // estimate of events before this one
    int eventsAfter = 0;           // estimate of events after this one
    double scrollPercent = 0.0;    // where in timeline (0 = oldest, 100 = newest)
};

// Compute timeline position from event index and total.
TimelinePosition computeTimelinePosition(const std::string& eventId, int index,
    int totalLoaded, int estimatedTotal);

// Estimate total events in a room from sync data.
int estimateTotalEvents(int loadedEvents, bool hasMoreBackward, bool hasMoreForward,
    int64_t roomCreateTs, int64_t oldestEventTs, double avgMsgPerDay);

// Check if event is within the loaded window.
bool isEventInWindow(int index, int totalLoaded);

// Get the distance between two events in the timeline.
int eventDistance(int indexA, int indexB);

// ---- Jump to Event ----

struct JumpTarget {
    std::string eventId;
    bool isLoaded = false;         // event is already in the timeline
    bool needsPagination = false;  // need to paginate to find it
    int estimatedPages = 0;        // how many pagination calls needed
    std::string direction;         // "forward" or "backward" to paginate
    std::string prevEventId;       // known event before the gap
    std::string nextEventId;       // known event after the gap
};

// Compute how to reach a target event from the current timeline state.
JumpTarget computeJumpTarget(const std::string& targetEventId,
    const std::vector<std::string>& loadedEventIds, bool hasMoreBackward,
    bool hasMoreForward, int pageSize);

// Estimate number of pagination requests needed.
int estimatePaginationRequests(int missingEvents, int pageSize);

} // namespace progressive

#endif // PROGRESSIVE_MESSAGE_LOCATION_HPP
