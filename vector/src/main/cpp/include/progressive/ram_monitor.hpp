#ifndef PROGRESSIVE_RAM_MONITOR_HPP
#define PROGRESSIVE_RAM_MONITOR_HPP

#include <string>
#include <cstdint>

namespace progressive {

struct MemoryInfo {
    int64_t totalRamKb = 0;      // total system RAM
    int64_t availableRamKb = 0;  // free + cached
    int64_t usedRamKb = 0;
    int64_t processRssKb = 0;    // this process RSS (resident set size)
    int64_t processVssKb = 0;    // virtual memory size
    int usagePercent = 0;        // 0-100 relative to total
};

// Get current memory info for the app process.
MemoryInfo getMemoryInfo();

// Format memory info as JSON for the statusbar.
std::string memoryInfoToJson(const MemoryInfo& info);

// Format a short label for the statusbar: "42 MB"
std::string formatMemoryLabel(int64_t rssKb);

} // namespace progressive

#endif // PROGRESSIVE_RAM_MONITOR_HPP
