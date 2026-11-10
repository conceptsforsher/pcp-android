#include "pcp/event_context_utils.hpp"
#include <sstream>

namespace pcp {
EventContext parseEventContext(const std::string& json) {
    EventContext ctx;
    auto extract = [&](const std::string& key) -> std::string {
        auto p = json.find("\"" + key + "\":\""); if (p == std::string::npos) return "";
        p += key.size() + 4; auto e = json.find('"', p);
        return e != std::string::npos ? json.substr(p, e - p) : "";
    };
    ctx.eventId = extract("event_id");
    size_t pos = 0; while ((pos = json.find("\"event_id\":\"", pos)) != std::string::npos) {
        pos += 12; auto end = json.find('"', pos); if (end == std::string::npos) break;
        ctx.beforeEvents.push_back(json.substr(pos, end - pos)); pos = end + 1;
    }
    return ctx;
}
std::string buildContextRequest(int limit) { return R"({"limit":)" + std::to_string(limit) + "}"; }
std::string formatContextSummary(const EventContext& ctx) {
    return "Event with " + std::to_string(ctx.beforeEvents.size()) + " before";
}
} // namespace pcp
