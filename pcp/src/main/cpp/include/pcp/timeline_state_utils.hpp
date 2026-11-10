#pragma once
#include <string>
#include <vector>
#include <cstdint>

namespace pcp {

struct TimelineState {
    std::string prevBatch;      // pagination back token
    std::string nextBatch;      // pagination forward token
    int eventCount = 0;
    bool limited = false;       // timeline was limited
    bool reachedStart = false;  // no more events backward
};

// Parse timeline state from /messages response
TimelineState parseTimelineState(const std::string& json);

// Check if timeline has more events in given direction
bool hasMoreEvents(const TimelineState& state, bool forward);

// Format pagination status
std::string formatTimelinePagination(const TimelineState& state);

} // namespace pcp
