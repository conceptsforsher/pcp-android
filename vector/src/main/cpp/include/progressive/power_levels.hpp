#ifndef PROGRESSIVE_POWER_LEVELS_HPP
#define PROGRESSIVE_POWER_LEVELS_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// ---- Room Power Levels / Permissions ----
// Ported from: org.matrix.android.sdk.api.session.room.model.PowerLevelsContent.kt
//              im.vector.app.features.roomprofile.permissions.RoomPermissions.kt
//              org.matrix.android.sdk.api.session.room.powerlevels.PowerLevelsHelper.kt

struct PowerLevels {
    // Default power levels (from spec: users_default = 0, events_default = 0, state_default = 50)
    int usersDefault = 0;
    int eventsDefault = 0;
    int stateDefault = 50;
    int inviteLevel = 0;
    int kickLevel = 50;
    int banLevel = 50;
    int redactLevel = 50;
    int notificationsRoomLevel = 50;

    // Per-user overrides: userId → power level
    struct UserOverride {
        std::string userId;
        int level = 0;
    };
    std::vector<UserOverride> userOverrides;

    // Per-event overrides: eventType → required level
    struct EventOverride {
        std::string eventType;
        int level = 0;
    };
    std::vector<EventOverride> eventOverrides;

    // Raw JSON for passing back to Kotlin
    std::string rawJson;
};

struct UserPermissions {
    std::string userId;
    int powerLevel = 0;            // user's power level in the room

    // State event permissions
    bool canSendState = false;     // default: PL >= stateDefault (50)
    bool canChangeName = false;    // m.room.name → requires appropriate level
    bool canChangeTopic = false;   // m.room.topic
    bool canChangeAvatar = false;  // m.room.avatar
    bool canChangeCanonicalAlias = false;
    bool canChangeHistoryVisibility = false;
    bool canChangeJoinRules = false;
    bool canChangeGuestAccess = false;
    bool canChangePowerLevels = false;
    bool canChangeServerACL = false;

    // Message permissions
    bool canSendMessages = false;  // default: PL >= eventsDefault (0)
    bool canSendRedacted = false;  // can redact own messages

    // Moderation
    bool canInvite = false;        // PL >= inviteLevel
    bool canKick = false;          // PL >= kickLevel
    bool canBan = false;           // PL >= banLevel
    bool canRedactOthers = false;  // PL >= redactLevel

    // Room notifications
    bool canNotifyRoom = false;    // PL >= notifications.room

    // Admin
    bool isAdmin = false;          // PL >= stateDefault (50 typically)
    bool isOwner = false;          // PL == 100
};

// Parse power_levels event content JSON.
// Original Kotlin (PowerLevelsContent.kt):
//   data class PowerLevelsContent(...)
PowerLevels parsePowerLevels(const std::string& json);

// Get a user's power level in the room.
int getUserPowerLevel(const PowerLevels& pl, const std::string& userId);

// Compute all permissions for a user in the room.
// Original Kotlin (PowerLevelsHelper.kt):
//   fun getUserPermissions(userId: String): UserPermissions
UserPermissions computeUserPermissions(const PowerLevels& pl, const std::string& userId);

// Check if a power level event is valid (has required fields).
bool isValidPowerLevels(const PowerLevels& pl);

// Format power level as a display string ("Admin", "Moderator", "Default", etc.).
std::string formatPowerLevel(int level, const PowerLevels& pl);

// Get a human-readable role name for the user.
std::string getUserRole(const UserPermissions& perms);

// Format user permissions as JSON for the Kotlin UI.
std::string permissionsToJson(const UserPermissions& perms);

} // namespace progressive

#endif // PROGRESSIVE_POWER_LEVELS_HPP
