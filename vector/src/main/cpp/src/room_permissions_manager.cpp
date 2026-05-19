#include "progressive/room_permissions_manager.hpp"
#include <sstream>
#include <algorithm>

namespace progressive {

// ====== Enum ======

const char* roomRoleToString(RoomRole role) {
    switch (role) {
        case RoomRole::USER: return "User";
        case RoomRole::MODERATOR: return "Moderator";
        case RoomRole::ADMIN: return "Admin";
        case RoomRole::SUPER_ADMIN: return "Super Admin";
        case RoomRole::CUSTOM: return "Custom";
    }
    return "User";
}

RoomRole powerLevelToRole(int powerLevel) {
    if (powerLevel >= PowerLevel::SUPER_ADMIN) return RoomRole::SUPER_ADMIN;
    if (powerLevel >= PowerLevel::ADMIN) return RoomRole::ADMIN;
    if (powerLevel >= PowerLevel::MODERATOR) return RoomRole::MODERATOR;
    if (powerLevel >= PowerLevel::USER) return RoomRole::USER;
    return RoomRole::CUSTOM;
}

// ====== PowerLevelsContent ======
// Original: setUserPowerLevel(userId, powerLevel)

void PowerLevelsContent::setUserPower(const std::string& userId, int powerLevel) {
    if (powerLevel == usersDefault) {
        users.erase(userId);
    } else {
        users[userId] = powerLevel;
    }
}

int PowerLevelsContent::getUserPower(const std::string& userId) const {
    auto it = users.find(userId);
    return (it != users.end()) ? it->second : usersDefault;
}

// Original: notificationLevel(key)
int PowerLevelsContent::notificationLevel(const std::string& key) const {
    auto it = notifications.find(key);
    if (it != notifications.end()) return it->second;
    if (key == "room") return PowerLevel::MODERATOR;
    return PowerLevel::USER;
}

RoomRole PowerLevelsContent::getUserRole(const std::string& userId) const {
    return powerLevelToRole(getUserPower(userId));
}

// ====== JSON helpers ======

std::string RoomPermissionsManager::extractStr(const std::string& json, const std::string& key) {
    auto pp = json.find("\"" + key + "\"");
    if (pp == std::string::npos) return "";
    pp = json.find('"', pp + key.size() + 2);
    if (pp == std::string::npos) return "";
    pp++;
    size_t e = pp;
    while (e < json.size() && json[e] != '"') e++;
    return json.substr(pp, e - pp);
}

int RoomPermissionsManager::extractInt(const std::string& json, const std::string& key) {
    auto pp = json.find("\"" + key + "\"");
    if (pp == std::string::npos) return 0;
    pp = json.find(':', pp);
    if (pp == std::string::npos) return 0;
    pp++;
    while (pp < json.size() && (json[pp] == ' ' || json[pp] == '\t')) pp++;
    int v = 0;
    while (pp < json.size() && json[pp] >= '0' && json[pp] <= '9') { v=v*10+(json[pp]-'0'); pp++; }
    return v;
}

// ====== Constructor ======

RoomPermissionsManager::RoomPermissionsManager() {}

// ====== Permission Checks ======
// Original: banOrDefault(), kickOrDefault(), etc.

bool RoomPermissionsManager::canBan(int userPowerLevel) const {
    return userPowerLevel >= powerLevels_.ban;
}
bool RoomPermissionsManager::canKick(int userPowerLevel) const {
    return userPowerLevel >= powerLevels_.kick;
}
bool RoomPermissionsManager::canInvite(int userPowerLevel) const {
    return userPowerLevel >= powerLevels_.invite;
}
bool RoomPermissionsManager::canRedact(int userPowerLevel) const {
    return userPowerLevel >= powerLevels_.redact;
}
bool RoomPermissionsManager::canSendMessage(int userPowerLevel) const {
    return userPowerLevel >= powerLevels_.eventsDefault;
}
bool RoomPermissionsManager::canSendState(int userPowerLevel) const {
    return userPowerLevel >= powerLevels_.stateDefault;
}
bool RoomPermissionsManager::canSendEvent(int userPowerLevel, const std::string& eventType) const {
    auto it = powerLevels_.events.find(eventType);
    int required = (it != powerLevels_.events.end()) ? it->second : powerLevels_.eventsDefault;
    return userPowerLevel >= required;
}
bool RoomPermissionsManager::canChangePowerLevels(int userPowerLevel) const {
    return userPowerLevel >= PowerLevel::ADMIN;
}
bool RoomPermissionsManager::canNotifyRoom(int userPowerLevel) const {
    return userPowerLevel >= powerLevels_.notificationLevel("room");
}
bool RoomPermissionsManager::canChangeSettings(int userPowerLevel) const {
    return userPowerLevel >= PowerLevel::MODERATOR;
}

// ====== Power Level Content ======

PowerLevelsContent RoomPermissionsManager::parsePowerLevels(const std::string& contentJson) {
    PowerLevelsContent pl;
    pl.ban = extractInt(contentJson, "ban");
    if (pl.ban == 0) pl.ban = PowerLevel::MODERATOR;
    pl.kick = extractInt(contentJson, "kick");
    if (pl.kick == 0) pl.kick = PowerLevel::MODERATOR;
    pl.invite = extractInt(contentJson, "invite");
    if (pl.invite == 0) pl.invite = PowerLevel::USER;
    pl.redact = extractInt(contentJson, "redact");
    if (pl.redact == 0) pl.redact = PowerLevel::MODERATOR;
    pl.eventsDefault = extractInt(contentJson, "events_default");
    if (pl.eventsDefault == 0) pl.eventsDefault = PowerLevel::USER;
    pl.usersDefault = extractInt(contentJson, "users_default");
    if (pl.usersDefault == 0) pl.usersDefault = PowerLevel::USER;
    pl.stateDefault = extractInt(contentJson, "state_default");
    if (pl.stateDefault == 0) pl.stateDefault = PowerLevel::MODERATOR;

    // Parse users map
    size_t pos = contentJson.find("\"users\"");
    if (pos != std::string::npos) {
        pos = contentJson.find('{', pos);
        if (pos != std::string::npos) {
            pos++;
            while (pos < contentJson.size() && contentJson[pos] != '}') {
                while (pos < contentJson.size() && (contentJson[pos] == ' ' || contentJson[pos] == ',')) pos++;
                if (pos >= contentJson.size() || contentJson[pos] == '}') break;
                if (contentJson[pos] == '"') {
                    pos++;
                    size_t e = pos;
                    while (e < contentJson.size() && contentJson[e] != '"') e++;
                    std::string userId = contentJson.substr(pos, e - pos);
                    pos = e + 1;
                    pos = contentJson.find(':', pos) + 1;
                    while (pos < contentJson.size() && (contentJson[pos] == ' ' || contentJson[pos] == '\t')) pos++;
                    int pwr = 0;
                    while (pos < contentJson.size() && contentJson[pos] >= '0' && contentJson[pos] <= '9') {
                        pwr = pwr * 10 + (contentJson[pos] - '0'); pos++;
                    }
                    pl.users[userId] = pwr;
                } else pos++;
            }
        }
    }

    pl.valid = true;
    powerLevels_ = pl;
    return pl;
}

std::string RoomPermissionsManager::buildPowerLevelsContent(const PowerLevelsContent& pl) {
    std::ostringstream os;
    os << R"({"ban":)" << pl.ban
       << R"(,"kick":)" << pl.kick
       << R"(,"invite":)" << pl.invite
       << R"(,"redact":)" << pl.redact
       << R"(,"events_default":)" << pl.eventsDefault
       << R"(,"users_default":)" << pl.usersDefault
       << R"(,"state_default":)" << pl.stateDefault;

    // Users
    os << R"(,"users":{)";
    bool first = true;
    for (const auto& [uid, pwr] : pl.users) {
        if (!first) os << ","; first = false;
        os << "\"" << uid << "\":" << pwr;
    }
    os << "}";

    // Events
    if (!pl.events.empty()) {
        os << R"(,"events":{)";
        first = true;
        for (const auto& [etype, pwr] : pl.events) {
            if (!first) os << ","; first = false;
            os << "\"" << etype << "\":" << pwr;
        }
        os << "}";
    }

    os << "}";
    return os.str();
}

PowerLevelsContent RoomPermissionsManager::getPowerLevels() const { return powerLevels_; }
void RoomPermissionsManager::setPowerLevels(const PowerLevelsContent& pl) { powerLevels_ = pl; }
void RoomPermissionsManager::setUserPower(const std::string& userId, int powerLevel) {
    powerLevels_.setUserPower(userId, powerLevel);
}

// ====== Role Management ======

std::string RoomPermissionsManager::getRoleLabel(RoomRole role) const {
    return roomRoleToString(role);
}

std::string RoomPermissionsManager::getRoleLabel(int powerLevel) const {
    return getRoleLabel(powerLevelToRole(powerLevel));
}

std::string RoomPermissionsManager::getPowerDescription(int powerLevel) const {
    std::ostringstream os;
    os << "Power " << powerLevel << " (" << getRoleLabel(powerLevel) << ")";

    bool canDo = false;
    os << " — can:";
    if (powerLevel >= PowerLevel::USER) { os << " read"; canDo = true; }
    if (canSendMessage(powerLevel)) { os << ", send messages"; }
    if (canInvite(powerLevel)) { os << ", invite"; }
    if (canKick(powerLevel)) { os << ", kick"; }
    if (canBan(powerLevel)) { os << ", ban"; }
    if (canRedact(powerLevel)) { os << ", redact"; }
    if (canNotifyRoom(powerLevel)) { os << ", @room"; }
    if (canChangePowerLevels(powerLevel)) { os << ", manage power levels"; }

    return os.str();
}

std::string RoomPermissionsManager::formatPowerLevelChange(const std::string& userId,
                                                             int oldPower, int newPower) const {
    std::ostringstream os;
    os << userId << " changed from " << getRoleLabel(oldPower)
       << " (" << oldPower << ") to " << getRoleLabel(newPower) << " (" << newPower << ")";
    return os.str();
}

// ====== Moderation Actions ======

std::string RoomPermissionsManager::buildKickRequest(const std::string& userId, const std::string& reason) {
    std::ostringstream os;
    os << R"({"user_id":")" << userId << R"(")";
    if (!reason.empty()) os << R"(,"reason":")" << reason << R"(")";
    os << "}";
    return os.str();
}

std::string RoomPermissionsManager::buildBanRequest(const std::string& userId, const std::string& reason) {
    std::ostringstream os;
    os << R"({"user_id":")" << userId << R"(")";
    if (!reason.empty()) os << R"(,"reason":")" << reason << R"(")";
    os << "}";
    return os.str();
}

std::string RoomPermissionsManager::buildInviteRequest(const std::string& userId) {
    return R"({"user_id":")" + userId + R"("})";
}

std::string RoomPermissionsManager::buildUnbanRequest(const std::string& userId) {
    return R"({"user_id":")" + userId + R"("})";
}

// ====== Serialization ======

std::string RoomPermissionsManager::powerLevelsToJson(const PowerLevelsContent& pl) const {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    std::ostringstream os;
    os << R"({"ban":)" << pl.ban << R"(,"kick":)" << pl.kick
       << R"(,"invite":)" << pl.invite << R"(,"redact":)" << pl.redact
       << R"(,"events_default":)" << pl.eventsDefault
       << R"(,"state_default":)" << pl.stateDefault
       << R"(,"users_default":)" << pl.usersDefault
       << R"(,"user_count":)" << static_cast<int>(pl.users.size())
       << R"(,"event_count":)" << static_cast<int>(pl.events.size())
       << R"(,"room_notification_level":)" << pl.notificationLevel("room")
       << "}";
    return os.str();
}

std::string RoomPermissionsManager::roleToJson(const std::string& userId, int powerLevel) const {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    auto role = powerLevelToRole(powerLevel);
    std::ostringstream os;
    os << R"({"user_id":")" << esc(userId)
       << R"(","power":)" << powerLevel
       << R"(,"role":")" << roomRoleToString(role)
       << R"(","can_ban":)" << (canBan(powerLevel) ? "true" : "false")
       << R"(,"can_kick":)" << (canKick(powerLevel) ? "true" : "false")
       << R"(,"can_invite":)" << (canInvite(powerLevel) ? "true" : "false")
       << R"(,"can_redact":)" << (canRedact(powerLevel) ? "true" : "false")
       << R"(,"can_send_msg":)" << (canSendMessage(powerLevel) ? "true" : "false")
       << R"(,"can_send_state":)" << (canSendState(powerLevel) ? "true" : "false")
       << R"(,"can_notify_room":)" << (canNotifyRoom(powerLevel) ? "true" : "false")
       << R"(,"can_change_pl":)" << (canChangePowerLevels(powerLevel) ? "true" : "false")
       << R"(,"description":")" << esc(getPowerDescription(powerLevel)) << R"(")"
       << "}";
    return os.str();
}

} // namespace progressive
