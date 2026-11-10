#pragma once
#include <string>

namespace pcp {

std::string buildMatrixLink(const std::string& type, const std::string& id);
std::string buildRoomPermalink(const std::string& roomId, const std::string& viaServer = "");
std::string buildUserPermalink(const std::string& userId);
std::string buildEventPermalink(const std::string& roomId, const std::string& eventId, const std::string& viaServer = "");
bool parsePermalink(const std::string& url, std::string& type, std::string& id);
bool isPermalink(const std::string& url);
std::string formatPermalink(const std::string& url, const std::string& label);

} // namespace pcp
