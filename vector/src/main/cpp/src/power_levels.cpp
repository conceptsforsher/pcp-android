#include "progressive/power_levels.hpp"
#include <sstream>

namespace progressive {

// Helper: extract integer value for a JSON key
static int getInt(const std::string& json, const std::string& key, int defaultVal = 0) {
    auto search = "\"" + key + "\":";
    auto pos = json.find(search);
    if (pos == std::string::npos) return defaultVal;
    pos += search.size();
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    // Skip negative sign if present
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

PowerLevels parsePowerLevels(const std::string& json) {
    PowerLevels pl;
    pl.rawJson = json;

    pl.usersDefault = getInt(json, "users_default", 0);
    pl.eventsDefault = getInt(json, "events_default", 0);
    pl.stateDefault = getInt(json, "state_default", 50);
    pl.inviteLevel = getInt(json, "invite", 0);
    pl.kickLevel = getInt(json, "kick", 50);
    pl.banLevel = getInt(json, "ban", 50);
    pl.redactLevel = getInt(json, "redact", 50);

    // notifications → room
    auto notifPos = json.find("\"notifications\"");
    if (notifPos != std::string::npos) {
        auto roomPos = json.find("\"room\"", notifPos);
        if (roomPos != std::string::npos) {
            auto colonPos = json.find(':', roomPos);
            if (colonPos != std::string::npos) {
                colonPos++;
                while (colonPos < json.size() && (json[colonPos] == ' ' || json[colonPos] == '\t')) colonPos++;
                int val = 0;
                while (colonPos < json.size() && json[colonPos] >= '0' && json[colonPos] <= '9') {
                    val = val * 10 + (json[colonPos] - '0');
                    colonPos++;
                }
                pl.notificationsRoomLevel = val;
            }
        }
    }

    // Parse "users" object: {"@alice:server": 100, "@bob:server": 50, ...}
    auto usersPos = json.find("\"users\"");
    if (usersPos != std::string::npos) {
        auto openPos = json.find('{', usersPos);
        if (openPos != std::string::npos) {
            int braceDepth = 1;
            size_t pos = openPos + 1;
            while (pos < json.size() && braceDepth > 0) {
                if (json[pos] == '"') {
                    // Found a key
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
                    pl.userOverrides.push_back({userId, level});
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

    // Parse "events" object
    auto eventsPos = json.find("\"events\"");
    if (eventsPos != std::string::npos) {
        auto openPos = json.find('{', eventsPos);
        if (openPos != std::string::npos) {
            int braceDepth = 1;
            size_t pos = openPos + 1;
            while (pos < json.size() && braceDepth > 0) {
                if (json[pos] == '"') {
                    size_t keyEnd = json.find('"', pos + 1);
                    if (keyEnd == std::string::npos) break;
                    std::string eventType = json.substr(pos + 1, keyEnd - pos - 1);
                    auto colon = json.find(':', keyEnd);
                    if (colon == std::string::npos) break;
                    size_t valPos = colon + 1;
                    while (valPos < json.size() && (json[valPos] == ' ' || json[valPos] == '\t')) valPos++;
                    int level = 0;
                    while (valPos < json.size() && json[valPos] >= '0' && json[valPos] <= '9') {
                        level = level * 10 + (json[valPos] - '0');
                        valPos++;
                    }
                    pl.eventOverrides.push_back({eventType, level});
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

int getUserPowerLevel(const PowerLevels& pl, const std::string& userId) {
    for (const auto& u : pl.userOverrides) {
        if (u.userId == userId) return u.level;
    }
    return pl.usersDefault;
}

UserPermissions computeUserPermissions(const PowerLevels& pl, const std::string& userId) {
    UserPermissions perms;
    perms.userId = userId;
    perms.powerLevel = getUserPowerLevel(pl, userId);
    int plv = perms.powerLevel;

    // State events default
    perms.canSendState = plv >= pl.stateDefault;

    // Specific state events: check event overrides first, fall back to state_default
    auto getEventLevel = [&](const std::string& eventType) -> int {
        for (const auto& e : pl.eventOverrides) {
            if (e.eventType == eventType) return e.level;
        }
        return pl.stateDefault;
    };

    perms.canChangeName = plv >= getEventLevel("m.room.name");
    perms.canChangeTopic = plv >= getEventLevel("m.room.topic");
    perms.canChangeAvatar = plv >= getEventLevel("m.room.avatar");
    perms.canChangeCanonicalAlias = plv >= getEventLevel("m.room.canonical_alias");
    perms.canChangeHistoryVisibility = plv >= getEventLevel("m.room.history_visibility");
    perms.canChangeJoinRules = plv >= getEventLevel("m.room.join_rules");
    perms.canChangeGuestAccess = plv >= getEventLevel("m.room.guest_access");
    perms.canChangePowerLevels = plv >= getEventLevel("m.room.power_levels");
    perms.canChangeServerACL = plv >= getEventLevel("m.room.server_acl");

    // Messages
    perms.canSendMessages = plv >= pl.eventsDefault;

    // Redaction
    perms.canSendRedacted = plv >= pl.redactLevel;  // simplified: own messages always, others check below
    perms.canRedactOthers = plv >= pl.redactLevel;

    // Moderation
    perms.canInvite = plv >= pl.inviteLevel;
    perms.canKick = plv >= pl.kickLevel;
    perms.canBan = plv >= pl.banLevel;

    // Notifications
    perms.canNotifyRoom = plv >= pl.notificationsRoomLevel;

    // Roles
    perms.isAdmin = plv >= pl.stateDefault;
    perms.isOwner = plv >= 100;

    return perms;
}

bool isValidPowerLevels(const PowerLevels& pl) {
    return !pl.rawJson.empty() && pl.rawJson.find("users") != std::string::npos;
}

std::string formatPowerLevel(int level, const PowerLevels& pl) {
    if (level >= 100) return "Admin (Owner)";
    if (level >= pl.stateDefault) return "Admin";
    if (level >= 50 && pl.stateDefault == 50) return "Moderator";
    if (level > pl.usersDefault) return "Custom (" + std::to_string(level) + ")";
    return "Default (" + std::to_string(level) + ")";
}

std::string getUserRole(const UserPermissions& perms) {
    if (perms.isOwner) return "Owner";
    if (perms.isAdmin) return "Admin";
    if (perms.canKick || perms.canBan) return "Moderator";
    return "Member";
}

std::string permissionsToJson(const UserPermissions& perms) {
    std::ostringstream json;
    json << R"({"userId": ")" << perms.userId << R"(",)";
    json << R"("powerLevel": )" << perms.powerLevel << ",";
    json << R"("role": ")" << getUserRole(perms) << R"(",)";
    json << R"("canSendState": )" << (perms.canSendState ? "true" : "false") << ",";
    json << R"("canSendMessages": )" << (perms.canSendMessages ? "true" : "false") << ",";
    json << R"("canChangeName": )" << (perms.canChangeName ? "true" : "false") << ",";
    json << R"("canChangeTopic": )" << (perms.canChangeTopic ? "true" : "false") << ",";
    json << R"("canChangeAvatar": )" << (perms.canChangeAvatar ? "true" : "false") << ",";
    json << R"("canInvite": )" << (perms.canInvite ? "true" : "false") << ",";
    json << R"("canKick": )" << (perms.canKick ? "true" : "false") << ",";
    json << R"("canBan": )" << (perms.canBan ? "true" : "false") << ",";
    json << R"("canRedactOthers": )" << (perms.canRedactOthers ? "true" : "false") << ",";
    json << R"("isAdmin": )" << (perms.isAdmin ? "true" : "false") << ",";
    json << R"("isOwner": )" << (perms.isOwner ? "true" : "false") << "}";
    return json.str();
}

} // namespace progressive
