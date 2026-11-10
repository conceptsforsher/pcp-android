#pragma once
#include <string>
#include <vector>
namespace pcp {
struct InviteTarget { std::string value; bool isEmail=false; bool isValid=false; };
InviteTarget parseInviteTarget(const std::string& input);
std::string buildMultiInviteRequest(const std::vector<std::string>& userIds, const std::string& reason="");
std::string formatInviteList(const std::vector<std::string>& invitees);
}
