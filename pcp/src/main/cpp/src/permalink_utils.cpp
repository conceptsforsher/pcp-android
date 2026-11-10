#include "pcp/permalink_utils.hpp"
#include <regex>

namespace pcp {

std::string buildMatrixLink(const std::string& type, const std::string& id) {
    return "https://matrix.to/#/" + type + "/" + id;
}

std::string buildRoomPermalink(const std::string& roomId, const std::string& viaServer) {
    std::string url = "https://matrix.to/#/" + roomId;
    if (!viaServer.empty()) {
        url += "?via=" + viaServer;
    }
    return url;
}

std::string buildUserPermalink(const std::string& userId) {
    return "https://matrix.to/#/" + userId;
}

std::string buildEventPermalink(const std::string& roomId, const std::string& eventId, const std::string& viaServer) {
    std::string url = "https://matrix.to/#/" + roomId + "/" + eventId;
    if (!viaServer.empty()) {
        url += "?via=" + viaServer;
    }
    return url;
}

bool parsePermalink(const std::string& url, std::string& type, std::string& id) {
    const std::string prefix = "https://matrix.to/#/";
    if (url.rfind(prefix, 0) != 0) return false;

    std::string rest = url.substr(prefix.size());
    auto queryPos = rest.find('?');
    if (queryPos != std::string::npos) {
        rest = rest.substr(0, queryPos);
    }

    if (rest.empty()) return false;

    if (rest[0] == '@') {
        type = "user";
        id = rest;
        return true;
    } else if (rest[0] == '!') {
        auto slash = rest.find('/');
        if (slash != std::string::npos) {
            type = "event";
            id = rest.substr(slash + 1);
        } else {
            type = "room";
            id = rest;
        }
        return true;
    } else if (rest[0] == '#') {
        type = "room_alias";
        id = rest;
        return true;
    }

    return false;
}

bool isPermalink(const std::string& url) {
    return url.rfind("https://matrix.to/#/", 0) == 0;
}

std::string formatPermalink(const std::string& url, const std::string& label) {
    return "[" + label + "](" + url + ")";
}

} // namespace pcp
