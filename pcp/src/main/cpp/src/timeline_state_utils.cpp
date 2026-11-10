#include "pcp/timeline_state_utils.hpp"
#include <sstream>

namespace pcp {
TimelineState parseTimelineState(const std::string& json) {
    TimelineState s;
    auto extract = [&](const std::string& key) -> std::string {
        auto p = json.find("\"" + key + "\":\""); if (p == std::string::npos) return "";
        p += key.size() + 4; auto e = json.find('"', p);
        return e != std::string::npos ? json.substr(p, e - p) : "";
    };
    s.prevBatch = extract("start"); s.nextBatch = extract("end");
    s.limited = json.find("\"limited\":true") != std::string::npos;
    s.reachedStart = s.prevBatch.empty();
    return s;
}
bool hasMoreEvents(const TimelineState& s, bool forward) { return forward ? !s.nextBatch.empty() : !s.prevBatch.empty(); }
std::string formatTimelinePagination(const TimelineState& s) {
    std::ostringstream os; os << s.eventCount << " events"; if (s.limited) os << " (limited)"; return os.str();
}
} // namespace pcp
