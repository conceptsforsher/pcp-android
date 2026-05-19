#include "progressive/room_permissions.hpp"
#include "progressive/json_parser.hpp"
#include "progressive/create_room.hpp"
#include <sstream>
#include <algorithm>

namespace progressive {

// ============================================================================
// PowerLevelAction enum conversions
// ============================================================================

// Original Kotlin: PowerLevelAction (implicit in check functions)
const char* powerLevelActionToString(PowerLevelAction action) {
    switch (action) {
        case PowerLevelAction::BAN:         return "ban";
        case PowerLevelAction::KICK:        return "kick";
        case PowerLevelAction::INVITE:      return "invite";
        case PowerLevelAction::REDACT:      return "redact";
        case PowerLevelAction::NOTIFY_ROOM: return "notify_room";
    }
    return "";
}

PowerLevelAction powerLevelActionFromString(const std::string& action) {
    if (action == "ban")         return PowerLevelAction::BAN;
    if (action == "kick")        return PowerLevelAction::KICK;
    if (action == "invite")      return PowerLevelAction::INVITE;
    if (action == "redact")      return PowerLevelAction::REDACT;
    if (action == "notify_room") return PowerLevelAction::NOTIFY_ROOM;
    return PowerLevelAction::BAN;
}

// ============================================================================
// RoomPermissionLevel enum conversions
// ============================================================================

// Original Kotlin: RoomPermissionLevel.kt
const char* roomPermissionLevelToString(RoomPermissionLevel level) {
    switch (level) {
        case RoomPermissionLevel::NONE:      return "none";
        case RoomPermissionLevel::USER:      return "user";
        case RoomPermissionLevel::MODERATOR: return "moderator";
        case RoomPermissionLevel::ADMIN:     return "admin";
    }
    return "none";
}

RoomPermissionLevel roomPermissionLevelFromString(const std::string& s) {
    if (s == "none")      return RoomPermissionLevel::NONE;
    if (s == "user")      return RoomPermissionLevel::USER;
    if (s == "moderator") return RoomPermissionLevel::MODERATOR;
    if (s == "admin")     return RoomPermissionLevel::ADMIN;
    return RoomPermissionLevel::NONE;
}

RoomPermissionLevel roomPermissionLevelFromInt(int level) {
    if (level <= static_cast<int>(RoomPermissionLevel::NONE))  return RoomPermissionLevel::NONE;
    if (level <= static_cast<int>(RoomPermissionLevel::USER))  return RoomPermissionLevel::USER;
    if (level <= static_cast<int>(RoomPermissionLevel::MODERATOR)) return RoomPermissionLevel::MODERATOR;
    return RoomPermissionLevel::ADMIN;
}

// ============================================================================
// PowerLevels JSON parsing
// ============================================================================

// Original Kotlin (PowerLevelsContent.kt):
//   Parse m.room.power_levels state event content JSON → PowerLevelsContent
PowerLevels parsePowerLevels(const std::string& stateContentJson) {
    PowerLevels pl;

    auto extractNum = [&](const std::string& key, int defaultVal = -1) -> int {
        auto val = parseJsonStringValue(stateContentJson, key);
        return val.empty() ? defaultVal : std::stoi(val);
    };

    auto extractBool = [&](const std::string& key) -> bool {
        return parseJsonBoolValue(stateContentJson, key, false);
    };

    int ud = extractNum("users_default");
    if (ud >= 0) pl.usersDefault = ud;

    int ed = extractNum("events_default");
    if (ed >= 0) pl.eventsDefault = ed;

    int sd = extractNum("state_default");
    if (sd >= 0) pl.stateDefault = sd;

    int b = extractNum("ban");
    if (b >= 0) pl.ban = b;

    int k = extractNum("kick");
    if (k >= 0) pl.kick = k;

    int r = extractNum("redact");
    if (r >= 0) pl.redact = r;

    int iv = extractNum("invite");
    if (iv >= 0) pl.invite = iv;

    int nf = extractNum("notify");
    if (nf >= 0) pl.notify = nf;

    // Parse user overrides: "users": {"@alice:server": 100, ...}
    auto usersJson = parseJsonStringValue(stateContentJson, "users");
    if (!usersJson.empty()) {
        std::string wrapped = "{" + usersJson + "}";
        size_t pos = 0;
        while (true) {
            pos = wrapped.find('"', pos);
            if (pos == std::string::npos) break;
            ++pos;
            auto end = wrapped.find('"', pos);
            if (end == std::string::npos) break;
            std::string userId = wrapped.substr(pos, end - pos);

            auto colon = wrapped.find(':', end);
            if (colon != std::string::npos) {
                auto valEnd = wrapped.find_first_of(",}", colon);
                if (valEnd != std::string::npos) {
                    auto valStr = wrapped.substr(colon + 1, valEnd - colon - 1);
                    // Trim leading space
                    while (!valStr.empty() && valStr.front() == ' ') valStr.erase(0, 1);
                    while (!valStr.empty() && valStr.back() == ' ') valStr.pop_back();
                    if (!valStr.empty()) {
                        pl.users[userId] = std::stoi(valStr);
                    }
                }
            }
            pos = end + 1;
        }
    }

    // Parse event overrides: "events": {"m.room.topic": 50, ...}
    auto eventsJson = parseJsonStringValue(stateContentJson, "events");
    if (!eventsJson.empty()) {
        std::string wrapped = "{" + eventsJson + "}";
        size_t pos = 0;
        while (true) {
            pos = wrapped.find('"', pos);
            if (pos == std::string::npos) break;
            ++pos;
            auto end = wrapped.find('"', pos);
            if (end == std::string::npos) break;
            std::string eventType = wrapped.substr(pos, end - pos);

            auto colon = wrapped.find(':', end);
            if (colon != std::string::npos) {
                auto valEnd = wrapped.find_first_of(",}", colon);
                if (valEnd != std::string::npos) {
                    auto valStr = wrapped.substr(colon + 1, valEnd - colon - 1);
                    while (!valStr.empty() && valStr.front() == ' ') valStr.erase(0, 1);
                    while (!valStr.empty() && valStr.back() == ' ') valStr.pop_back();
                    if (!valStr.empty()) {
                        pl.events[eventType] = std::stoi(valStr);
                    }
                }
            }
            pos = end + 1;
        }
    }

    pl.valid = true;
    return pl;
}

// ============================================================================
// PowerLevels → JSON serialization
// ============================================================================

// Original Kotlin (PowerLevelsContent.kt): serialize PowerLevelsContent to JSON
std::string powerLevelsToJson(const PowerLevels& pl) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) {
            if (c == '"') out += "\\\"";
            else if (c == '\\') out += "\\\\";
            else out += c;
        }
        return out;
    };

    std::ostringstream json;
    json << "{";
    json << R"("users_default": )" << pl.usersDefault;
    json << R"(,"events_default": )" << pl.eventsDefault;
    json << R"(,"state_default": )" << pl.stateDefault;
    json << R"(,"ban": )" << pl.ban;
    json << R"(,"kick": )" << pl.kick;
    json << R"(,"redact": )" << pl.redact;
    json << R"(,"invite": )" << pl.invite;
    json << R"(,"notify": )" << pl.notify;

    // Users override map
    if (!pl.users.empty()) {
        json << R"(,"users": {)";
        bool first = true;
        for (const auto& [uid, level] : pl.users) {
            if (!first) json << ",";
            first = false;
            json << R"(")" << esc(uid) << R"(": )" << level;
        }
        json << "}";
    }

    // Events override map
    if (!pl.events.empty()) {
        json << R"(,"events": {)";
        bool first = true;
        for (const auto& [etype, level] : pl.events) {
            if (!first) json << ",";
            first = false;
            json << R"(")" << esc(etype) << R"(": )" << level;
        }
        json << "}";
    }

    json << "}";
    return json.str();
}

// ============================================================================
// RoomPowerLevels — member function implementations
// ============================================================================

// Original Kotlin (RoomPowerLevels.kt): check creator privilege
bool RoomPowerLevels::shouldGiveInfinitePowerLevel(const std::string& userId) const {
    if (!roomCreateContent) return false;

    // Original Kotlin: roomCreateContent.explicitlyPrivilegeRoomCreators()
    //   && userId is in the creators list (any user with PL >= 100 at creation)
    for (const auto& creator : creators) {
        if (creator == userId) return true;
    }
    return false;
}

// Original Kotlin (RoomPowerLevels.kt): getUserPowerLevel(userId) → int
int RoomPowerLevels::getUserPowerLevel(const std::string& userId) const {
    // Check for creator privilege (infinite)
    if (shouldGiveInfinitePowerLevel(userId)) return PowerLevelsConstants::INFINITE_PL;

    // Check explicit user override
    auto it = powerLevelsContent.users.find(userId);
    if (it != powerLevelsContent.users.end()) return it->second;

    // Return default
    return powerLevelsContent.usersDefault;
}

// Original Kotlin (RoomPowerLevels.kt): isUserAllowedToSend(userId, isState, eventType)
bool RoomPowerLevels::isUserAllowedToSend(const std::string& userId, bool isState,
    const std::string& eventType) const {
    int userPL = getUserPowerLevel(userId);

    // Infinite power can do anything
    if (userPL == PowerLevelsConstants::INFINITE_PL) return true;

    int minimumPL = getEventPowerLevel(isState, eventType);
    return userPL >= minimumPL;
}

// Original Kotlin: isUserAllowedToSend(userId, isState=true, eventType)
bool RoomPowerLevels::isUserAllowedToSendState(const std::string& userId,
    const std::string& eventType) const {
    return isUserAllowedToSend(userId, true, eventType);
}

// Original Kotlin: message send check
bool RoomPowerLevels::isUserAllowedToSendMessage(const std::string& userId,
    const std::string& msgType) const {
    return isUserAllowedToSend(userId, false, msgType);
}

// Original Kotlin (RoomPowerLevels.kt): getEventPowerLevel(isState, eventType)
int RoomPowerLevels::getEventPowerLevel(bool isState, const std::string& eventType) const {
    // Check explicit event type override
    auto it = powerLevelsContent.events.find(eventType);
    if (it != powerLevelsContent.events.end()) return it->second;

    // Check message subtype (e.g., m.room.message#image)
    // If no direct match, check the base type
    auto hashPos = eventType.find('#');
    if (hashPos != std::string::npos) {
        std::string baseType = eventType.substr(0, hashPos);
        auto baseIt = powerLevelsContent.events.find(baseType);
        if (baseIt != powerLevelsContent.events.end()) return baseIt->second;
    }

    // Fallback to defaults
    return isState ? powerLevelsContent.stateDefault : powerLevelsContent.eventsDefault;
}

bool RoomPowerLevels::isUserAbleToInvite(const std::string& userId) const {
    int userPL = getUserPowerLevel(userId);
    if (userPL == PowerLevelsConstants::INFINITE_PL) return true;
    return userPL >= powerLevelsContent.invite;
}

bool RoomPowerLevels::isUserAbleToBan(const std::string& userId) const {
    int userPL = getUserPowerLevel(userId);
    if (userPL == PowerLevelsConstants::INFINITE_PL) return true;
    return userPL >= powerLevelsContent.ban;
}

bool RoomPowerLevels::isUserAbleToKick(const std::string& userId) const {
    int userPL = getUserPowerLevel(userId);
    if (userPL == PowerLevelsConstants::INFINITE_PL) return true;
    return userPL >= powerLevelsContent.kick;
}

bool RoomPowerLevels::isUserAbleToRedact(const std::string& userId) const {
    int userPL = getUserPowerLevel(userId);
    if (userPL == PowerLevelsConstants::INFINITE_PL) return true;
    return userPL >= powerLevelsContent.redact;
}

bool RoomPowerLevels::isUserAbleToTriggerNotification(const std::string& userId,
    const std::string& notificationKey) const {
    int userPL = getUserPowerLevel(userId);
    if (userPL == PowerLevelsConstants::INFINITE_PL) return true;
    // Check notifications."room" or notifications."notificationKey"
    auto nit = powerLevelsContent.events.find("m.room.notifications." + notificationKey);
    int required = powerLevelsContent.notify;
    if (nit != powerLevelsContent.events.end()) required = nit->second;
    return userPL >= required;
}

std::string RoomPowerLevels::getSuggestedRole(const std::string& userId) const {
    int plvl = getUserPowerLevel(userId);
    if (plvl == PowerLevelsConstants::INFINITE_PL) return "Creator";
    if (plvl >= PowerLevelsConstants::SUPER_ADMIN_PL) return "Super Admin";
    if (plvl >= PowerLevelsConstants::ADMIN_PL) return "Admin";
    if (plvl >= PowerLevelsConstants::MODERATOR_PL) return "Moderator";
    return "Member";
}

// ============================================================================
// Legacy parseRoomPowerLevels — returns RoomPowerLevels wrapper
// ============================================================================

RoomPowerLevels parseRoomPowerLevels(const std::string& stateContentJson) {
    RoomPowerLevels pl;
    pl.powerLevelsContent = parsePowerLevels(stateContentJson);

    // Populate creators list: users with PL >= ADMIN_PL (100)
    for (const auto& [uid, level] : pl.powerLevelsContent.users) {
        if (level >= PowerLevelsConstants::ADMIN_PL) {
            pl.creators.push_back(uid);
        }
    }

    return pl;
}

// ============================================================================
// Compute permissions
// ============================================================================

// Original Kotlin (RoomPowerLevels.kt): compute capabilities per user
RoomPermissions computePermissions(const RoomPowerLevels& pl, const std::string& myUserId) {
    RoomPermissions p;
    p.myUserId = myUserId;

    int myPL = pl.getUserPowerLevel(myUserId);

    // Messaging
    int msgPL = pl.getEventPowerLevel(false, "m.room.message");
    p.canSendMessages = myPL >= msgPL || myPL == PowerLevelsConstants::INFINITE_PL;

    int imgPL = pl.getEventPowerLevel(false, "m.room.message#image");
    p.canSendImages = myPL >= std::max(msgPL, imgPL) || myPL == PowerLevelsConstants::INFINITE_PL;

    int vidPL = pl.getEventPowerLevel(false, "m.room.message#video");
    p.canSendVideos = myPL >= std::max(msgPL, vidPL) || myPL == PowerLevelsConstants::INFINITE_PL;

    int filePL = pl.getEventPowerLevel(false, "m.room.message#file");
    p.canSendFiles = myPL >= std::max(msgPL, filePL) || myPL == PowerLevelsConstants::INFINITE_PL;

    // Moderation
    p.canBan = myPL >= pl.powerLevelsContent.ban || myPL == PowerLevelsConstants::INFINITE_PL;
    p.canKick = myPL >= pl.powerLevelsContent.kick || myPL == PowerLevelsConstants::INFINITE_PL;
    p.canRedactOthers = myPL >= pl.powerLevelsContent.redact || myPL == PowerLevelsConstants::INFINITE_PL;
    p.canInvite = myPL >= pl.powerLevelsContent.invite || myPL == PowerLevelsConstants::INFINITE_PL;
    p.canNotifyEveryone = myPL >= pl.powerLevelsContent.notify || myPL == PowerLevelsConstants::INFINITE_PL;

    // Room management
    p.canChangeName   = myPL >= pl.getEventPowerLevel(true, "m.room.name") || myPL == PowerLevelsConstants::INFINITE_PL;
    p.canChangeTopic  = myPL >= pl.getEventPowerLevel(true, "m.room.topic") || myPL == PowerLevelsConstants::INFINITE_PL;
    p.canChangeAvatar = myPL >= pl.getEventPowerLevel(true, "m.room.avatar") || myPL == PowerLevelsConstants::INFINITE_PL;
    p.canUpgradeRoom  = myPL >= pl.getEventPowerLevel(true, "m.room.tombstone") || myPL == PowerLevelsConstants::INFINITE_PL;
    p.canPinMessages  = myPL >= pl.getEventPowerLevel(true, "m.room.pinned_events") || myPL == PowerLevelsConstants::INFINITE_PL;
    p.canToggleEncryption = myPL >= pl.getEventPowerLevel(true, "m.room.encryption") || myPL == PowerLevelsConstants::INFINITE_PL;

    // Polls
    p.canCreatePolls = myPL >= pl.getEventPowerLevel(false, "m.poll") || myPL == PowerLevelsConstants::INFINITE_PL;

    return p;
}

RoomPermissions computePermissionsFromPowerLevels(const PowerLevels& pl, const std::string& myUserId) {
    RoomPowerLevels rpl;
    rpl.powerLevelsContent = pl;
    return computePermissions(rpl, myUserId);
}

// ============================================================================
// Standalone functions
// ============================================================================

int getUserPowerLevel(const RoomPowerLevels& pl, const std::string& userId) {
    return pl.getUserPowerLevel(userId);
}

int getUserPowerLevel(const PowerLevels& pl, const std::string& userId) {
    auto it = pl.users.find(userId);
    if (it != pl.users.end()) return it->second;
    return pl.usersDefault;
}

int getRequiredLevel(const RoomPowerLevels& pl, const std::string& eventType, bool isState) {
    return pl.getEventPowerLevel(isState, eventType);
}

bool hasPower(const RoomPowerLevels& pl, const std::string& userId,
    const std::string& action, bool isState) {
    int userPL = pl.getUserPowerLevel(userId);
    int required = pl.getEventPowerLevel(isState, action);
    return userPL >= required || userPL == PowerLevelsConstants::INFINITE_PL;
}

std::string getSuggestedRole(const RoomPowerLevels& pl, const std::string& userId) {
    return pl.getSuggestedRole(userId);
}

// ============================================================================
// Extended Permission Checks
// ============================================================================

// Original Kotlin: checkRoomPermission()
RoomPermissionCheck checkRoomPermission(const RoomPowerLevels& pl, const std::string& userId,
                                         const std::string& action) {
    RoomPermissionCheck check;
    check.userId = userId;
    check.action = action;
    check.userLevel = pl.getUserPowerLevel(userId);
    check.requiredLevel = getRequiredLevelForAction(pl.powerLevelsContent, action);
    check.isAllowed = (check.userLevel >= check.requiredLevel ||
                       check.userLevel == PowerLevelsConstants::INFINITE_PL);
    if (!check.isAllowed) {
        std::ostringstream reason;
        reason << "User " << userId << " (PL " << check.userLevel
               << ") lacks permission for " << action
               << " (requires PL " << check.requiredLevel << ")";
        check.reason = reason.str();
    }
    return check;
}

// Original Kotlin: getRequiredLevelForAction()
int getRequiredLevelForAction(const PowerLevels& pl, const std::string& action) {
    if (action == "ban")                       return pl.ban;
    if (action == "kick")                      return pl.kick;
    if (action == "invite")                    return pl.invite;
    if (action == "redact")                    return pl.redact;
    if (action == "notify_room")               return pl.notify;

    // Check event-level overrides
    auto stateIt = pl.events.find(action);
    if (stateIt != pl.events.end()) return stateIt->second;

    // Check message subtype (e.g. m.room.message#image)
    auto hashPos = action.find('#');
    if (hashPos != std::string::npos) {
        std::string baseType = action.substr(0, hashPos);
        auto baseIt = pl.events.find(baseType);
        if (baseIt != pl.events.end()) return baseIt->second;
    }

    return pl.stateDefault;
}

// Original Kotlin: isPowerLevelSufficient()
bool isPowerLevelSufficient(const RoomPowerLevels& pl, const std::string& userId,
                            const std::string& action) {
    int userPL = pl.getUserPowerLevel(userId);
    int required = getRequiredLevelForAction(pl.powerLevelsContent, action);
    return userPL >= required || userPL == PowerLevelsConstants::INFINITE_PL;
}

// ============================================================================
// Ban/Kick Event Builders & Parsers
// ============================================================================

namespace {

std::string jExtractStr(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return "";
    pos = json.find(':', pos);
    if (pos == std::string::npos) return "";
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n' || json[pos] == '\r')) pos++;
    if (pos >= json.size() || json[pos] != '"') return "";
    pos++;
    size_t end = pos;
    while (end < json.size() && json[end] != '"') {
        if (json[end] == '\\') end++;
        end++;
    }
    return json.substr(pos, end - pos);
}

std::string jEscape(const std::string& s) {
    std::string out;
    for (char c : s) {
        if (c == '"') out += "\\\"";
        else if (c == '\\') out += "\\\\";
        else out += c;
    }
    return out;
}

} // anonymous namespace

// Original Kotlin: buildBanEvent()
std::string buildBanEvent(const RoomBanInfo& banInfo) {
    std::ostringstream json;
    json << "{";
    json << "\"membership\":\"ban\"";
    if (!banInfo.reason.empty()) {
        json << ",\"reason\":\"" << jEscape(banInfo.reason) << "\"";
    }
    json << "}";
    return json.str();
}

// Original Kotlin: parseBanEvent()
RoomBanInfo parseBanEvent(const std::string& stateContentJson) {
    RoomBanInfo info;
    info.reason = jExtractStr(stateContentJson, "reason");
    return info;
}

// Original Kotlin: isUserBanned()
bool isUserBanned(const std::string& userId, const std::string& membershipStateJson) {
    (void)userId;
    auto membership = jExtractStr(membershipStateJson, "membership");
    return membership == "ban";
}

// Original Kotlin: buildKickEvent()
std::string buildKickEvent(const RoomKickInfo& kickInfo) {
    std::ostringstream json;
    json << "{";
    json << "\"membership\":\"leave\"";
    if (!kickInfo.reason.empty()) {
        json << ",\"reason\":\"" << jEscape(kickInfo.reason) << "\"";
    }
    json << "}";
    return json.str();
}

// ============================================================================
// Individual Permission Check Functions
// ============================================================================

// Original Kotlin: isUserAllowedToInvite()
bool isUserAllowedToInvite(const RoomPowerLevels& pl, const std::string& userId) {
    return pl.isUserAbleToInvite(userId);
}

// Original Kotlin: isUserAllowedToKick()
bool isUserAllowedToKick(const RoomPowerLevels& pl, const std::string& userId) {
    return pl.isUserAbleToKick(userId);
}

// Original Kotlin: isUserAllowedToBan()
bool isUserAllowedToBan(const RoomPowerLevels& pl, const std::string& userId) {
    return pl.isUserAbleToBan(userId);
}

// Original Kotlin: isUserAllowedToRedact()
bool isUserAllowedToRedact(const RoomPowerLevels& pl, const std::string& userId) {
    return pl.isUserAbleToRedact(userId);
}

// Original Kotlin: isUserAllowedToSetName()
bool isUserAllowedToSetName(const RoomPowerLevels& pl, const std::string& userId) {
    return isPowerLevelSufficient(pl, userId, "m.room.name");
}

// Original Kotlin: isUserAllowedToSetTopic()
bool isUserAllowedToSetTopic(const RoomPowerLevels& pl, const std::string& userId) {
    return isPowerLevelSufficient(pl, userId, "m.room.topic");
}

// Original Kotlin: isUserAllowedToSetAvatar()
bool isUserAllowedToSetAvatar(const RoomPowerLevels& pl, const std::string& userId) {
    return isPowerLevelSufficient(pl, userId, "m.room.avatar");
}

// Original Kotlin: isUserAllowedToChangeJoinRules()
bool isUserAllowedToChangeJoinRules(const RoomPowerLevels& pl, const std::string& userId) {
    return isPowerLevelSufficient(pl, userId, "m.room.join_rules");
}

// Original Kotlin: isUserAllowedToChangeHistoryVisibility()
bool isUserAllowedToChangeHistoryVisibility(const RoomPowerLevels& pl, const std::string& userId) {
    return isPowerLevelSufficient(pl, userId, "m.room.history_visibility");
}

// Original Kotlin: isUserAllowedToChangeGuestAccess()
bool isUserAllowedToChangeGuestAccess(const RoomPowerLevels& pl, const std::string& userId) {
    return isPowerLevelSufficient(pl, userId, "m.room.guest_access");
}

// ============================================================================
// Formatting
// ============================================================================

std::string formatPermissionsSummary(const RoomPermissions& perms) {
    std::ostringstream out;
    out << "Permissions for " << perms.myUserId << ":\n";
    out << "  Messages: " << (perms.canSendMessages ? "Yes" : "No") << "\n";
    out << "  Ban/Kick: " << (perms.canBan ? "Yes" : "No") << "/" << (perms.canKick ? "Yes" : "No") << "\n";
    out << "  Invite: " << (perms.canInvite ? "Yes" : "No") << "\n";
    out << "  Redact others: " << (perms.canRedactOthers ? "Yes" : "No") << "\n";
    out << "  @room: " << (perms.canNotifyEveryone ? "Yes" : "No") << "\n";
    out << "  Pin messages: " << (perms.canPinMessages ? "Yes" : "No") << "\n";
    out << "  Change settings: " << (perms.canChangeName ? "Yes" : "No") << "\n";
    return out.str();
}

std::string permissionsToJson(const RoomPermissions& perms) {
    std::ostringstream json;
    json << "{";
    json << R"("canSendMessages": )" << (perms.canSendMessages ? "true" : "false") << ",";
    json << R"("canBan": )" << (perms.canBan ? "true" : "false") << ",";
    json << R"("canKick": )" << (perms.canKick ? "true" : "false") << ",";
    json << R"("canInvite": )" << (perms.canInvite ? "true" : "false") << ",";
    json << R"("canRedactOthers": )" << (perms.canRedactOthers ? "true" : "false") << ",";
    json << R"("canNotifyEveryone": )" << (perms.canNotifyEveryone ? "true" : "false") << ",";
    json << R"("canPinMessages": )" << (perms.canPinMessages ? "true" : "false") << ",";
    json << R"("canChangeName": )" << (perms.canChangeName ? "true" : "false");
    json << "}";
    return json.str();
}

} // namespace progressive
