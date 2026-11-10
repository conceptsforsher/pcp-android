#include "pcp/session_info_utils.hpp"
#include <sstream>

namespace pcp {
std::string buildWhoamiRequest() { return "{}"; }
SessionInfo parseWhoamiResponse(const std::string& json) {
    SessionInfo s;
    auto extract = [&](const std::string& key) -> std::string {
        auto p = json.find("\"" + key + "\":\""); if (p == std::string::npos) return "";
        p += key.size() + 4; auto e = json.find('"', p);
        return e != std::string::npos ? json.substr(p, e - p) : "";
    };
    s.userId = extract("user_id"); s.deviceId = extract("device_id"); return s;
}
std::string formatSessionDebug(const SessionInfo& i) {
    std::ostringstream os; os << i.userId << " @ " << i.homeServer << " [" << i.deviceId << "]"; return os.str();
}
} // namespace pcp
