#include "pcp/room_encryption_event_utils.hpp"
#include <sstream>

namespace pcp {

RoomEncryptionConfig parseRoomEncryption(const std::string& json) {
    RoomEncryptionConfig c;
    auto extract = [&](const std::string& key) -> std::string {
        auto p = json.find("\"" + key + "\":\""); if (p == std::string::npos) return "";
        p += key.size() + 4; auto e = json.find('"', p);
        return e != std::string::npos ? json.substr(p, e - p) : "";
    };
    c.algorithm = extract("algorithm"); return c;
}
std::string buildRoomEncryptionEvent(const RoomEncryptionConfig& cfg) {
    std::ostringstream os;
    os << R"({"algorithm":")" << cfg.algorithm << R"(")";
    os << R"(,"rotation_period_ms":)" << cfg.rotationPeriodMs;
    os << R"(,"rotation_period_msgs":)" << cfg.rotationPeriodMsgs << "}";
    return os.str();
}
std::string formatEncryptionConfig(const RoomEncryptionConfig& cfg) {
    return "Encrypted with " + cfg.algorithm;
}
} // namespace pcp
