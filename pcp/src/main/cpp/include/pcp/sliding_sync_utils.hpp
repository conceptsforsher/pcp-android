#pragma once
#include <string>
#include <vector>
#include <cstdint>

namespace pcp {

struct SlidingSyncRange {
    int start = 0;
    int end = 20;
};

struct SlidingSyncRoom {
    std::string roomId;
    std::string name;
    int notificationCount = 0;
    int highlightCount = 0;
    int64_t lastActivityMs = 0;
    bool isDirect = false;
    std::vector<std::string> requiredStateKeys;
};

// Build sliding sync request with ranges and filters
std::string buildSlidingSyncRequest(const std::string& pos = "",
                                      const std::vector<SlidingSyncRange>& ranges = {{0,20}});

// Parse sliding sync room list from response
std::vector<std::string> parseSlidingSyncRooms(const std::string& json);

// Get sliding sync subscription key for a room
std::string buildSubscriptionKey(const std::string& roomId, int timelineLimit = 20);

// Check if more rooms needed (scrolled to end)
bool needsMoreRooms(int loadedCount, int totalCount);

// Format sliding sync range parameter
std::string formatRangeParam(const SlidingSyncRange& range);

} // namespace pcp
