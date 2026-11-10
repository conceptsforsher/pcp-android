#include "pcp/event_id_utils.hpp"
#include <regex>

namespace pcp {

bool validateEventId(const std::string& eventId) {
    if (eventId.empty() || eventId[0] != '$') return false;
    // Covers legacy ($alphanum:server), V3 ($base64), and V4 ($base64urlsafe)
    // Optional :server suffix for legacy compatibility
    std::regex eventIdRegex(
        R"(^\$[0-9A-Za-z+/_=\-]+(?::[0-9A-Za-z.\-]+(?::\d{2,5})?)?$)"
    );
    return std::regex_match(eventId, eventIdRegex);
}

std::string extractServerFromEventId(const std::string& eventId) {
    auto pos = eventId.find(':');
    if (pos != std::string::npos && pos + 1 < eventId.size()) {
        return eventId.substr(pos + 1);
    }
    return "";
}

int compareEventIds(const std::string& eid1, const std::string& eid2) {
    if (eid1 == eid2) return 0;
    // Compare server first, then full ID
    auto server1 = extractServerFromEventId(eid1);
    auto server2 = extractServerFromEventId(eid2);
    if (server1 != server2) {
        return server1 < server2 ? -1 : 1;
    }
    return eid1 < eid2 ? -1 : 1;
}

std::string parseEventId(const std::string& input) {
    std::regex eventIdRegex(
        R"(\$[0-9A-Za-z+/_=\-]+(?::[0-9A-Za-z.\-]+(?::\d{2,5})?)?)"
    );
    std::smatch m;
    if (std::regex_search(input, m, eventIdRegex)) {
        return m[0].str();
    }
    return "";
}

std::string buildEventId(const std::string& serverName) {
    return "$event:" + serverName;
}

} // namespace pcp
