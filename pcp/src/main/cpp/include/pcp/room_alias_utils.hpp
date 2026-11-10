#pragma once
#include <string>
#include <vector>

namespace pcp {

struct RoomAliasInfo {
    std::string alias;
    std::string server;
    std::string roomId;
    std::vector<std::string> servers;
};

bool isValidAlias(const std::string& alias);
bool isAliasUrl(const std::string& url);
std::string extractServerFromAlias(const std::string& alias);
std::string normalizeAlias(const std::string& alias);
std::string formatAlias(const std::string& alias);
std::string buildAliasJoinUrl(const std::string& alias);
std::vector<std::string> parseRoomAliases(const std::string& stateJson);
std::string buildAliasResolveRequest(const std::string& alias);
RoomAliasInfo parseAliasResolve(const std::string& json, const std::string& alias);
std::string buildAliasCreateRequest(const std::string& alias);

} // namespace pcp
