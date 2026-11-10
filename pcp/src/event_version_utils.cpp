#include "pcp/event_version_utils.hpp"
namespace pcp {
int parseEventVersion(const std::string& json) {
    auto p = json.find("\"room_version\":\""); if (p == std::string::npos) return 1;
    p += 16; try { return std::stoi(json.substr(p)); } catch(...) { return 1; }
}
bool isEventVersionSupported(int v) { return v >= 1 && v <= 11; }
std::string getLatestEventVersion() { return "11"; }
}
