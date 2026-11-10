#include "pcp/room_power_level_utils.hpp"

namespace pcp {

static int extractInt(const std::string& json, const std::string& key, int defaultVal = 0) {
    auto search = "\"" + key + "\":";
    auto pos = json.find(search);
    if (pos == std::string::npos) return defaultVal;
    pos += search.size();
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    bool neg = false;
    if (pos < json.size() && json[pos] == '-') { neg = true; pos++; }
    if (pos >= json.size() || json[pos] < '0' || json[pos] > '9') return defaultVal;
    int val = 0;
    while (pos < json.size() && json[pos] >= '0' && json[pos] <= '9') {
        val = val * 10 + (json[pos] - '0');
        pos++;
    }
    return neg ? -val : val;
}

PowerLevelInfo parsePowerLevelInfo(const std::string& json) {
    PowerLevelInfo pl;

    pl.usersDefault = extractInt(json, "users_default", 0);
    pl.eventsDefault = extractInt(json, "events_default", 0);
    pl.stateDefault = extractInt(json, "state_default", 50);
    pl.inviteLevel = extractInt(json, "invite", 0);
    pl.kickLevel = extractInt(json, "kick", 50);
    pl.banLevel = extractInt(json, "ban", 50);
    pl.redactLevel = extractInt(json, "redact", 50);

    auto usersPos = json.find("\"users\"");
    if (usersPos != std::string::npos) {
        auto openPos = json.find('{', usersPos);
        if (openPos != std::string::npos) {
            int braceDepth = 1;
            size_t pos = openPos + 1;
            while (pos < json.size() && braceDepth > 0) {
                if (json[pos] == '"') {
                    size_t keyEnd = json.find('"', pos + 1);
                    if (keyEnd == std::string::npos) break;
                    std::string userId = json.substr(pos + 1, keyEnd - pos - 1);
                    auto colon = json.find(':', keyEnd);
                    if (colon == std::string::npos) break;
                    size_t valPos = colon + 1;
                    while (valPos < json.size() && (json[valPos] == ' ' || json[valPos] == '\t')) valPos++;
                    int level = 0;
                    while (valPos < json.size() && json[valPos] >= '0' && json[valPos] <= '9') {
                        level = level * 10 + (json[valPos] - '0');
                        valPos++;
                    }
                    pl.users[userId] = level;
                    pos = valPos;
                } else if (json[pos] == '{') {
                    braceDepth++;
                    pos++;
                } else if (json[pos] == '}') {
                    braceDepth--;
                    pos++;
                } else {
                    pos++;
                }
            }
        }
    }

    return pl;
}

int getUserLevel(const PowerLevelInfo& pl, const std::string& userId) {
    auto it = pl.users.find(userId);
    if (it != pl.users.end()) {
        return it->second;
    }
    return pl.usersDefault;
}

bool canUserBan(const PowerLevelInfo& pl, const std::string& userId) {
    return getUserLevel(pl, userId) >= pl.banLevel;
}

bool canUserKick(const PowerLevelInfo& pl, const std::string& userId) {
    return getUserLevel(pl, userId) >= pl.kickLevel;
}

bool canUserRedact(const PowerLevelInfo& pl, const std::string& userId) {
    return getUserLevel(pl, userId) >= pl.redactLevel;
}

bool canUserInvite(const PowerLevelInfo& pl, const std::string& userId) {
    return getUserLevel(pl, userId) >= pl.inviteLevel;
}

bool canUserChangeState(const PowerLevelInfo& pl, const std::string& userId) {
    return getUserLevel(pl, userId) >= pl.stateDefault;
}

bool isUserAdmin(const PowerLevelInfo& pl, const std::string& userId) {
    return getUserLevel(pl, userId) >= pl.stateDefault;
}

std::string formatPowerLevel(int level) {
    if (level >= 100) return "Admin (Owner)";
    if (level >= 50) return "Moderator";
    if (level > 0) return "Custom (" + std::to_string(level) + ")";
    return "Default";
}

std::string formatUserRole(const PowerLevelInfo& pl, const std::string& userId) {
    int level = getUserLevel(pl, userId);
    if (level >= 100) return "Owner";
    if (level >= 50) return "Admin";
    if (level >= pl.kickLevel) return "Moderator";
    return "Member";
}

PowerLevelInfo getDefaultPowerLevels() {
    PowerLevelInfo pl;
    pl.usersDefault  = 0;
    pl.eventsDefault = 0;
    pl.stateDefault  = 50;
    pl.inviteLevel   = 0;
    pl.kickLevel     = 50;
    pl.banLevel      = 50;
    pl.redactLevel   = 50;
    return pl;
}

std::string formatPermissionDenied(const std::string& reason) {
    if (reason.empty()) return "Permission denied.";
    return "Permission denied: " + reason;
}

std::string formatSuggestedRole(int powerLevel) {
    if (powerLevel >= 150) return "SuperAdmin";
    if (powerLevel >= 100) return "Admin";
    if (powerLevel >= 50)  return "Moderator";
    return "User";
}

} // namespace pcp
