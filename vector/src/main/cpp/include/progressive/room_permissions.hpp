#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>

namespace progressive {

// ================================================================
// Room Permissions & Power Levels
//
// Faithful port from original Kotlin:
//   PowerLevelsContent.kt — ban, kick, invite, redact, events_default,
//     events, users_default, users, state_default, notifications
//   RoomPowerLevels.kt (api) — wraps PowerLevelsContent + RoomCreateContent
//   RoomPowerLevels.kt (internal) — getRoomPowerLevels(), createRoomPowerLevels()
//   UserPowerLevel.kt — Infinite, Value(value), User(0), Moderator(50),
//     Admin(100), SuperAdmin(150)
// ================================================================

// ---- Power Level Constants ----
// Original Kotlin: UserPowerLevel.User(0), Moderator(50), Admin(100), SuperAdmin(150)

namespace PowerLevelsConstants {
    constexpr int DEFAULT_PL = 0;       // User
    constexpr int MODERATOR_PL = 50;    // Moderator
    constexpr int ADMIN_PL = 100;       // Admin
    constexpr int SUPER_ADMIN_PL = 150; // SuperAdmin
    constexpr int INFINITE_PL = -1;     // Infinite/Creator (can do anything)
}

// ---- RoomPermissionLevel ----
//
// Original Kotlin: RoomPermissionLevel.kt
enum class RoomPermissionLevel {
    NONE = 0,
    USER = 10,
    MODERATOR = 50,
    ADMIN = 100
};

const char* roomPermissionLevelToString(RoomPermissionLevel level);
RoomPermissionLevel roomPermissionLevelFromString(const std::string& s);
RoomPermissionLevel roomPermissionLevelFromInt(int level);

// ---- Power Level Action ----
// Original Kotlin: implicit from check functions (banOrDefault, kickOrDefault, etc.)

enum class PowerLevelAction {
    BAN,            // Ban a user from the room
    KICK,           // Kick/remove a user
    INVITE,         // Invite a user to the room
    REDACT,         // Redact (delete) a message
    NOTIFY_ROOM,    // Send @room notification
};

const char* powerLevelActionToString(PowerLevelAction action);
PowerLevelAction powerLevelActionFromString(const std::string& action);

// ---- RoomPermissionCheck ----
//
// Original Kotlin: RoomPermissionCheck.kt
struct RoomPermissionCheck {
    std::string userId;
    std::string action;
    int requiredLevel = 0;
    int userLevel = 0;
    bool isAllowed = false;
    std::string reason;
};

// ---- RoomPermissionException ----
//
// Original Kotlin: RoomPermissionException.kt
struct RoomPermissionException {
    std::string action;
    std::string exceptionType;
    std::string details;
};

// ---- RoomBanInfo ----
//
// Original Kotlin: RoomBanInfo.kt
struct RoomBanInfo {
    std::string userId;
    std::string reason;
    std::string bannedBy;
    int64_t bannedAt = 0;
    int64_t expiresAt = 0;
};

// ---- RoomKickInfo ----
//
// Original Kotlin: RoomKickInfo.kt
struct RoomKickInfo {
    std::string userId;
    std::string reason;
    std::string kickedBy;
    int64_t kickedAt = 0;
};

// ---- PowerLevels (full Matrix spec struct) ----
// Original Kotlin: PowerLevelsContent.kt (9 fields + notifications)

struct PowerLevels {
    int usersDefault = PowerLevelsConstants::DEFAULT_PL;     // Default: 0
    int eventsDefault = PowerLevelsConstants::DEFAULT_PL;    // Default: 0
    int stateDefault = PowerLevelsConstants::MODERATOR_PL;   // Default: 50
    int ban = PowerLevelsConstants::MODERATOR_PL;            // Default: 50
    int kick = PowerLevelsConstants::MODERATOR_PL;           // Default: 50
    int redact = PowerLevelsConstants::MODERATOR_PL;         // Default: 50
    int invite = PowerLevelsConstants::DEFAULT_PL;           // Default: 0
    int notify = PowerLevelsConstants::MODERATOR_PL;         // Default: 50 (@room)
    std::unordered_map<std::string, int> users;   // userId → power level
    std::unordered_map<std::string, int> events;  // eventType → required power level
    bool valid = false;
};

// ---- RoomPowerLevels (wraps PowerLevelsContent + RoomCreateContent) ----
// Original Kotlin:
//   class RoomPowerLevels(
//       val powerLevelsContent: PowerLevelsContent?,
//       private val roomCreateContent: RoomCreateContentWithSender?,
//   )

struct RoomCreateContent;  // forward declaration (defined in create_room.hpp)

struct RoomPowerLevels {
    // The power levels content from the room state.
    PowerLevels powerLevelsContent;

    // The room create content (for infinite power level check).
    // Original Kotlin: shouldGiveInfinitePowerLevel(userId)
    //   checks roomCreateContent.explicitlyPrivilegeRoomCreators()
    //   and whether userId is in the creators list.
    const RoomCreateContent* roomCreateContent = nullptr;

    // The creators list (from room create + power levels 100 users).
    std::vector<std::string> creators;

    // Check if a user should be given infinite power level (creator privilege).
    // Original Kotlin: RoomPowerLevels.kt:shouldGiveInfinitePowerLevel(userId)
    bool shouldGiveInfinitePowerLevel(const std::string& userId) const;

    // Get a user's effective power level value.
    // Original Kotlin: getUserPowerLevel(userId): UserPowerLevel
    //   Returns the numeric value (or INFINITE_PL if creator privilege applies).
    int getUserPowerLevel(const std::string& userId) const;

    // Check if a user is allowed to send an event of a given type.
    // Original Kotlin: isUserAllowedToSend(userId, isState, eventType): Boolean
    //   val powerLevel = getUserPowerLevel(userId)
    //   val minimumPowerLevel = powerLevelsContent?.events?.get(eventType)
    //       ?: if (isState) stateDefaultOrDefault() else eventsDefaultOrDefault()
    //   powerLevel >= Value(minimumPowerLevel)
    bool isUserAllowedToSend(const std::string& userId, bool isState,
        const std::string& eventType) const;

    // Check if a user is allowed to send a state event.
    // Original Kotlin: isUserAllowedToSend(userId, isState=true, eventType)
    bool isUserAllowedToSendState(const std::string& userId,
        const std::string& eventType) const;

    // Check if a user is allowed to send a message event.
    bool isUserAllowedToSendMessage(const std::string& userId,
        const std::string& msgType = "m.room.message") const;

    // Get the required power level for a specific event type.
    // Original Kotlin: powerLevelsContent?.events?.get(eventType)
    //   ?: if (isState) stateDefault else eventsDefault
    int getEventPowerLevel(bool isState, const std::string& eventType) const;

    // Check if a user can invite others.
    // Original Kotlin: isUserAbleToInvite(userId): Boolean
    bool isUserAbleToInvite(const std::string& userId) const;

    // Check if a user can ban others.
    // Original Kotlin: isUserAbleToBan(userId): Boolean
    bool isUserAbleToBan(const std::string& userId) const;

    // Check if a user can kick others.
    // Original Kotlin: isUserAbleToKick(userId): Boolean
    bool isUserAbleToKick(const std::string& userId) const;

    // Check if a user can redact.
    // Original Kotlin: isUserAbleToRedact(userId): Boolean
    bool isUserAbleToRedact(const std::string& userId) const;

    // Check if a user can trigger a notification.
    // Original Kotlin: isUserAbleToTriggerNotification(userId, notificationKey): Boolean
    bool isUserAbleToTriggerNotification(const std::string& userId,
        const std::string& notificationKey = "room") const;

    // Get the suggested role name for a user.
    // Original Kotlin: getSuggestedRole(userId): Role
    std::string getSuggestedRole(const std::string& userId) const;
};

// ================================================================
// Functions (standalone)
// ================================================================

// Parse power levels from m.room.power_levels state event JSON.
// Original Kotlin: parse from state event content → PowerLevelsContent
PowerLevels parsePowerLevels(const std::string& stateContentJson);

// Serialize PowerLevels to JSON string.
// Original Kotlin: buildContent() for sending to server
std::string powerLevelsToJson(const PowerLevels& pl);

// Parse power levels from m.room.power_levels state event (legacy name).
// Alias kept for backward compatibility with existing code.
RoomPowerLevels parseRoomPowerLevels(const std::string& stateContentJson);

// ---- Room Permissions (computed from power levels) ----

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

// Compute room permissions from power levels.
RoomPermissions computePermissions(const RoomPowerLevels& pl, const std::string& myUserId);

// Compute permissions from raw PowerLevels struct.
RoomPermissions computePermissionsFromPowerLevels(const PowerLevels& pl, const std::string& myUserId);

// Get a user's effective power level.
int getUserPowerLevel(const RoomPowerLevels& pl, const std::string& userId);

// Get a user's effective power level from raw PowerLevels.
int getUserPowerLevel(const PowerLevels& pl, const std::string& userId);

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

// ---- Extended Permission Checks ----
//
// Original Kotlin: RoomPermissionChecks.kt

// Check if a user has permission for a specific action.
// Returns a full RoomPermissionCheck with details.
RoomPermissionCheck checkRoomPermission(const RoomPowerLevels& pl, const std::string& userId,
                                         const std::string& action);

// Get the minimum power level required for a given action from PowerLevels.
int getRequiredLevelForAction(const PowerLevels& pl, const std::string& action);

// Check if a user's power level is sufficient for an action.
bool isPowerLevelSufficient(const RoomPowerLevels& pl, const std::string& userId,
                            const std::string& action);

// ---- Ban/Kick Event Builders & Parsers ----
//
// Original Kotlin: BanEventBuilder.kt / KickEventBuilder.kt

// Build a ban event content JSON for m.room.member with membership=ban.
std::string buildBanEvent(const RoomBanInfo& banInfo);

// Parse a ban event from state event content JSON.
RoomBanInfo parseBanEvent(const std::string& stateContentJson);

// Check if a user is banned based on membership state JSON.
bool isUserBanned(const std::string& userId, const std::string& membershipStateJson);

// Build a kick event content JSON for m.room.member with membership=leave.
std::string buildKickEvent(const RoomKickInfo& kickInfo);

// ---- Individual Permission Check Functions ----
//
// Original Kotlin: RoomPermissionChecks.kt (individual helpers)

bool isUserAllowedToInvite(const RoomPowerLevels& pl, const std::string& userId);
bool isUserAllowedToKick(const RoomPowerLevels& pl, const std::string& userId);
bool isUserAllowedToBan(const RoomPowerLevels& pl, const std::string& userId);
bool isUserAllowedToRedact(const RoomPowerLevels& pl, const std::string& userId);
bool isUserAllowedToSetName(const RoomPowerLevels& pl, const std::string& userId);
bool isUserAllowedToSetTopic(const RoomPowerLevels& pl, const std::string& userId);
bool isUserAllowedToSetAvatar(const RoomPowerLevels& pl, const std::string& userId);
bool isUserAllowedToChangeJoinRules(const RoomPowerLevels& pl, const std::string& userId);
bool isUserAllowedToChangeHistoryVisibility(const RoomPowerLevels& pl, const std::string& userId);
bool isUserAllowedToChangeGuestAccess(const RoomPowerLevels& pl, const std::string& userId);

} // namespace progressive
