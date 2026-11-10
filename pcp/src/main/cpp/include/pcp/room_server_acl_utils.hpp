#pragma once
#include <string>
#include <vector>
namespace pcp {
bool isServerAllowed(const std::string& serverName, const std::string& aclJson);
std::vector<std::string> parseAllowedServers(const std::string& aclJson);
std::string buildAclEvent(const std::vector<std::string>& allowedServers, bool allowIpLiterals=false);
}
