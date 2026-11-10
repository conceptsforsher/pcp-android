#include "pcp/room_alias_utils.hpp"
#include <regex>
#include <cctype>
#include <algorithm>

namespace pcp {

bool isValidAlias(const std::string& alias) {
    if (alias.empty() || alias[0] != '#') return false;
    std::regex aliasRegex(R"(^#[A-Za-z0-9._%#@=+\-]+:[A-Za-z0-9.\-]+(?::\d{2,5})?$)");
    return std::regex_match(alias, aliasRegex);
}

bool isAliasUrl(const std::string& url) {
    std::regex urlRegex(R"(https://matrix\.to/#/#[A-Za-z0-9._%#@=+\-]+:[A-Za-z0-9.\-]+)", std::regex::icase);
    return std::regex_search(url, urlRegex);
}

std::string extractServerFromAlias(const std::string& alias) {
    auto pos = alias.find(':');
    if (pos != std::string::npos && pos + 1 < alias.size()) {
        return alias.substr(pos + 1);
    }
    return "";
}

std::string normalizeAlias(const std::string& alias) {
    std::string result;
    if (!alias.empty() && alias[0] != '#') {
        result = "#" + alias;
    } else {
        result = alias;
    }
    auto colonPos = result.find(':');
    if (colonPos != std::string::npos) {
        std::string localPart = result.substr(0, colonPos);
        std::transform(localPart.begin(), localPart.end(), localPart.begin(), ::tolower);
        result = localPart + result.substr(colonPos);
    } else {
        std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    }
    return result;
}

std::string formatAlias(const std::string& alias) {
    return normalizeAlias(alias);
}

std::string buildAliasJoinUrl(const std::string& alias) {
    return "https://matrix.to/#/" + alias;
}

std::vector<std::string> parseRoomAliases(const std::string& stateJson) {
    std::vector<std::string> aliases;
    std::regex aliasRegex(R"(#[A-Za-z0-9._%#@=+\-]+:[A-Za-z0-9.\-]+(?::\d{2,5})?)");
    std::sregex_iterator it(stateJson.begin(), stateJson.end(), aliasRegex);
    std::sregex_iterator end;
    for (; it != end; ++it) {
        aliases.push_back(it->str());
    }
    return aliases;
}

std::string buildAliasResolveRequest(const std::string& alias) {
    return R"({"uri":")" + alias + R"("})";
}

RoomAliasInfo parseAliasResolve(const std::string& json, const std::string& alias) {
    RoomAliasInfo info;
    info.alias = alias;

    // room_id pattern: "room_id":"!xxx:yyy"
    std::regex roomIdRegex(R"regex("room_id"\s*:\s*"(![^"]+)")regex");
    std::smatch m;
    if (std::regex_search(json, m, roomIdRegex)) {
        info.roomId = m[1].str();
    }

    info.server = extractServerFromAlias(alias);
    info.servers.push_back(info.server);

    // servers array
    std::regex serversRegex(R"regex("servers"\s*:\s*\[([^\]]*)\])regex");
    if (std::regex_search(json, m, serversRegex)) {
        std::string serverList = m[1].str();
        std::regex serverRegex(R"regex("([^"]+)")regex");
        std::sregex_iterator sit(serverList.begin(), serverList.end(), serverRegex);
        std::sregex_iterator send;
        for (; sit != send; ++sit) {
            std::string srv = (*sit)[1].str();
            if (srv != info.server) {
                info.servers.push_back(srv);
            }
        }
    }

    return info;
}

std::string buildAliasCreateRequest(const std::string& alias) {
    auto server = extractServerFromAlias(alias);
    return R"({"room_alias_name":")" + server + R"("})";
}

} // namespace pcp
