#include "progressive/room_permissions.hpp"
#include "progressive/json_parser.hpp"
#include <sstream>
#include <algorithm>

namespace progressive {

RoomPowerLevels parseRoomPowerLevels(const std::string& stateContentJson) {
    RoomPowerLevels pl;

    auto extractNum = [&](const std::string& key) -> int {
        auto val = parseJsonStringValue(stateContentJson, key);
        return val.empty() ? -1 : std::stoi(val);
    };

    int ud = extractNum("users_default");
    if (ud >= 0) pl.usersDefault = ud;
    int ed = extractNum("events_default");
    if (ed >= 0) pl.eventsDefault = ed;
    int sd = extractNum("state_default");
    if (sd >= 0) pl.stateDefault = sd;

    int ban = extractNum("ban");
    if (ban >= 0) pl.ban = ban;
    int kick = extractNum("kick");
    if (kick >= 0) pl.kick = kick;
    int redact = extractNum("redact");
    if (redact >= 0) pl.redact = redact;
    int invite = extractNum("invite");
    if (invite >= 0) pl.invite = invite;

    auto roomNotif = parseJsonStringValue(stateContentJson, "notifications.room");
    if (!roomNotif.empty()) pl.notificationsRoom = std::stoi(roomNotif);

    // Parse user overrides: "users": {"@alice:server": 100}
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
                    while (!valStr.empty() && valStr.front() == ' ') valStr.erase(0, 1);
                    pl.userOverrides[userId] = std::stoi(valStr);
                }
            }
            pos = end + 1;
        }
    }

    return pl;
}

int getUserPowerLevel(const RoomPowerLevels& pl, const std::string& userId) {
    auto it = pl.userOverrides.find(userId);
    if (it != pl.userOverrides.end()) return it->second;
    return pl.usersDefault;
}

int getRequiredLevel(const RoomPowerLevels& pl, const std::string& eventType, bool isState) {
    auto it = pl.eventOverrides.find(eventType);
    if (it != pl.eventOverrides.end()) return it->second;
    return isState ? pl.stateDefault : pl.eventsDefault;
}

RoomPermissions computePermissions(const RoomPowerLevels& pl, const std::string& myUserId) {
    RoomPermissions p;
    p.myUserId = myUserId;

    int myPL = getUserPowerLevel(pl, myUserId);

    // Messaging
    int msgPL = getRequiredLevel(pl, "m.room.message", false);
    p.canSendMessages = myPL >= msgPL;

    int imgPL = getRequiredLevel(pl, "m.room.message#image", false);
    p.canSendImages = myPL >= std::max(msgPL, imgPL);

    int vidPL = getRequiredLevel(pl, "m.room.message#video", false);
    p.canSendVideos = myPL >= std::max(msgPL, vidPL);

    int filePL = getRequiredLevel(pl, "m.room.message#file", false);
    p.canSendFiles = myPL >= std::max(msgPL, filePL);

    // Moderation
    p.canBan = myPL >= pl.ban;
    p.canKick = myPL >= pl.kick;
    p.canRedactOthers = myPL >= pl.redact;
    p.canInvite = myPL >= pl.invite;
    p.canNotifyEveryone = myPL >= pl.notificationsRoom;

    // Room management
    p.canChangeName      = myPL >= getRequiredLevel(pl, "m.room.name", true);
    p.canChangeTopic     = myPL >= getRequiredLevel(pl, "m.room.topic", true);
    p.canChangeAvatar    = myPL >= getRequiredLevel(pl, "m.room.avatar", true);
    p.canUpgradeRoom     = myPL >= getRequiredLevel(pl, "m.room.tombstone", true);
    p.canPinMessages     = myPL >= getRequiredLevel(pl, "m.room.pinned_events", true);
    p.canToggleEncryption = myPL >= getRequiredLevel(pl, "m.room.encryption", true);

    return p;
}

bool hasPower(const RoomPowerLevels& pl, const std::string& userId,
    const std::string& action, bool isState) {
    int userPL = getUserPowerLevel(pl, userId);
    int required = getRequiredLevel(pl, action, isState);
    return userPL >= required;
}

std::string getSuggestedRole(const RoomPowerLevels& pl, const std::string& userId) {
    int plvl = getUserPowerLevel(pl, userId);
    if (plvl >= 100) return "Admin";
    if (plvl >= 50) return "Moderator";
    return "Member";
}

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
