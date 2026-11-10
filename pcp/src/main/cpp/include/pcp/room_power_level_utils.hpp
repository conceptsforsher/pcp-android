#pragma once
#include <string>
#include <unordered_map>

namespace pcp {

struct PowerLevelInfo {
    int usersDefault = 0;
    int eventsDefault = 0;
    int stateDefault = 50;
    int inviteLevel = 0;
    int kickLevel = 50;
    int banLevel = 50;
    int redactLevel = 50;
    std::unordered_map<std::string, int> users;
};

PowerLevelInfo parsePowerLevelInfo(const std::string& json);
int getUserLevel(const PowerLevelInfo& pl, const std::string& userId);
bool canUserBan(const PowerLevelInfo& pl, const std::string& userId);
bool canUserKick(const PowerLevelInfo& pl, const std::string& userId);
bool canUserRedact(const PowerLevelInfo& pl, const std::string& userId);
bool canUserInvite(const PowerLevelInfo& pl, const std::string& userId);
bool canUserChangeState(const PowerLevelInfo& pl, const std::string& userId);
bool isUserAdmin(const PowerLevelInfo& pl, const std::string& userId);
std::string formatPowerLevel(int level);
std::string formatUserRole(const PowerLevelInfo& pl, const std::string& userId);

// Return a PowerLevelInfo with Matrix specification defaults:
//   users_default = 0, events_default = 0, state_default = 50,
//   invite = 0, kick = 50, ban = 50, redact = 50
PowerLevelInfo getDefaultPowerLevels();

// Format a permission-denied message.
// If reason is empty, returns "Permission denied."
// Otherwise returns "Permission denied: <reason>"
std::string formatPermissionDenied(const std::string& reason);

// Format a role suggestion string from a power level value.
std::string formatSuggestedRole(int powerLevel);

} // namespace pcp
