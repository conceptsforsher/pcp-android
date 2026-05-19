#include "progressive/ram_monitor.hpp"
#include <sstream>
#include <fstream>

namespace progressive {

MemoryInfo getMemoryInfo() {
    MemoryInfo info;

    // Read /proc/self/status for process memory
    std::ifstream status("/proc/self/status");
    if (status.is_open()) {
        std::string line;
        while (std::getline(status, line)) {
            if (line.rfind("VmRSS:", 0) == 0) {
                // VmRSS:    123456 kB
                auto pos = line.find(':');
                if (pos != std::string::npos) {
                    auto val = line.substr(pos + 1);
                    // Trim and parse
                    while (!val.empty() && (val.front() == ' ' || val.front() == '\t')) val.erase(0, 1);
                    auto kbPos = val.find("kB");
                    if (kbPos != std::string::npos) val = val.substr(0, kbPos);
                    while (!val.empty() && val.back() == ' ') val.pop_back();
                    info.processRssKb = std::stoll(val);
                }
            }
            if (line.rfind("VmSize:", 0) == 0) {
                auto pos = line.find(':');
                if (pos != std::string::npos) {
                    auto val = line.substr(pos + 1);
                    while (!val.empty() && (val.front() == ' ' || val.front() == '\t')) val.erase(0, 1);
                    auto kbPos = val.find("kB");
                    if (kbPos != std::string::npos) val = val.substr(0, kbPos);
                    while (!val.empty() && val.back() == ' ') val.pop_back();
                    info.processVssKb = std::stoll(val);
                }
            }
        }
    }

    // Read /proc/meminfo for system RAM
    std::ifstream mem("/proc/meminfo");
    if (mem.is_open()) {
        std::string line;
        while (std::getline(mem, line)) {
            if (line.rfind("MemTotal:", 0) == 0) {
                auto pos = line.find(':');
                if (pos != std::string::npos) {
                    auto val = line.substr(pos + 1);
                    while (!val.empty() && (val.front() == ' ' || val.front() == '\t')) val.erase(0, 1);
                    auto kbPos = val.find("kB");
                    if (kbPos != std::string::npos) val = val.substr(0, kbPos);
                    while (!val.empty() && val.back() == ' ') val.pop_back();
                    info.totalRamKb = std::stoll(val);
                }
            }
            if (line.rfind("MemAvailable:", 0) == 0) {
                auto pos = line.find(':');
                if (pos != std::string::npos) {
                    auto val = line.substr(pos + 1);
                    while (!val.empty() && (val.front() == ' ' || val.front() == '\t')) val.erase(0, 1);
                    auto kbPos = val.find("kB");
                    if (kbPos != std::string::npos) val = val.substr(0, kbPos);
                    while (!val.empty() && val.back() == ' ') val.pop_back();
                    info.availableRamKb = std::stoll(val);
                }
            }
        }
    }

    info.usedRamKb = info.totalRamKb - info.availableRamKb;
    if (info.totalRamKb > 0) {
        info.usagePercent = static_cast<int>((info.processRssKb * 100) / info.totalRamKb);
    }

    return info;
}

std::string memoryInfoToJson(const MemoryInfo& info) {
    std::ostringstream json;
    json << "{";
    json << R"("rssKb": )" << info.processRssKb << ",";
    json << R"("rssMb": )" << (info.processRssKb / 1024) << ",";
    json << R"("totalKb": )" << info.totalRamKb << ",";
    json << R"("usagePercent": )" << info.usagePercent;
    json << "}";
    return json.str();
}

std::string formatMemoryLabel(int64_t rssKb) {
    std::ostringstream out;
    out << (rssKb / 1024) << " MB";
    return out.str();
}

} // namespace progressive
