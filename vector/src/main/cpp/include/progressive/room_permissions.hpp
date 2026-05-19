#ifndef PROGRESSIVE_ROOM_PERMISSIONS_HPP
#define PROGRESSIVE_ROOM_PERMISSIONS_HPP

#include <string>
#include <vector>
#include <unordered_map>

namespace progressive {

// ---- Power Levels & Permissions ----

struct RoomPowerLevels {
    int usersDefault = 0;
    int eventsDefault = 0;
    int stateDefault = 50;
    int ban = 50;
    int kick = 50;
    int redact = 50;
    int invite = 50;
    int notificationsRoom = 50;
    std::unordered_map<std::string, int> userOverrides;  // userId → PL
    std::unordered_map<std::string, int> eventOverrides; // eventType → PL
};

struct RoomPermissions {
    std::string myUserId;

    // Messaging
    bool canSendMessages = true;
    bool canSendImages = true;
    bool canSendVideos = true;
    bool canSendFiles = true;

    // Moderation
    bool canBan = false;
    bool canKick = false;
    bool canRedactOwn = true;
    bool canRedactOthers = false;
    bool canInvite = false;

    // Room management
    bool canChangeName = false;
    bool canChangeTopic = false;
    bool canChangeAvatar = false;
    bool canUpgradeRoom = false;

    // Special
    bool canNotifyEveryone = false;   // @room
    bool canPinMessages = false;
    bool canCreatePolls = false;
    bool canToggleEncryption = false;
};

// Parse power levels from m.room.power_levels state event.
RoomPowerLevels parseRoomPowerLevels(const std::string& stateContentJson);

// Compute room permissions from power levels.
RoomPermissions computePermissions(const RoomPowerLevels& pl, const std::string& myUserId);

// Get a user's effective power level.
int getUserPowerLevel(const RoomPowerLevels& pl, const std::string& userId);

// Get the required level for an event type.
int getRequiredLevel(const RoomPowerLevels& pl, const std::string& eventType, bool isState);

// Check if a user has sufficient power for an action.
bool hasPower(const RoomPowerLevels& pl, const std::string& userId,
    const std::string& action, bool isState = false);

// Get the suggested role name: Creator, Admin, Moderator, Default.
std::string getSuggestedRole(const RoomPowerLevels& pl, const std::string& userId);

// Format permissions summary as text.
std::string formatPermissionsSummary(const RoomPermissions& perms);

// Format permissions as JSON.
std::string permissionsToJson(const RoomPermissions& perms);

} // namespace progressive

#endif // PROGRESSIVE_ROOM_PERMISSIONS_HPP
