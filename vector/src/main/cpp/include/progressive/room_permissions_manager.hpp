#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>

namespace progressive {

// ================================================================
// Room Permissions Manager — power levels, moderation, bans, kicks
//
// Faithful port from Element Android original sources:
//   PowerLevelsContent.kt — ban (50), kick (50), invite (0), redact (50),
//     events_default (0), events (map), users_default (0), users (map),
//     state_default (50), notifications (map), setUserPowerLevel(),
//     notificationLevel(key), NOTIFICATIONS_ROOM_KEY
//   UserPowerLevel.kt — Infinite, Value(value), User(0),
//     Moderator(50), Admin(100), SuperAdmin(150)
//
// Matrix spec: m.room.power_levels state event
// Controls who can do what in the room.
//
// Covers:
//   1. Power level parsing and management
//   2. Can-do checks (ban, kick, invite, redact, send message, send state)
//   3. User power level management
//   4. Role detection (User/Moderator/Admin)
//   5. @room notification permission
//   6. Build power levels content
// ================================================================

// ---- Role Constants ----
// Original: UserPowerLevel.User(0), Moderator(50), Admin(100), SuperAdmin(150)

struct PowerLevel {
    static constexpr int USER = 0;
    static constexpr int MODERATOR = 50;
    static constexpr int ADMIN = 100;
    static constexpr int SUPER_ADMIN = 150;
    static constexpr int INFINITE = -1; // Special: can do anything
};

// ---- Role ----

enum class RoomRole {
    USER = 0,           // Power 0 — default
    MODERATOR = 1,      // Power 50 — can kick, ban, redact
    ADMIN = 2,          // Power 100 — can change power levels
    SUPER_ADMIN = 3,    // Power 150+
    CUSTOM = 4,         // Non-standard power level
};

const char* roomRoleToString(RoomRole role);
RoomRole powerLevelToRole(int powerLevel);

// ---- Power Levels Content ----
// Original: PowerLevelsContent.kt (9 fields)

struct PowerLevelsContent {
    int ban = PowerLevel::MODERATOR;              // Default: 50
    int kick = PowerLevel::MODERATOR;             // Default: 50
    int invite = PowerLevel::USER;                // Default: 0
    int redact = PowerLevel::MODERATOR;           // Default: 50
    int eventsDefault = PowerLevel::USER;         // Default: 0
    std::unordered_map<std::string, int> events;  // event_type → power
    int usersDefault = PowerLevel::USER;          // Default: 0
    std::unordered_map<std::string, int> users;   // user_id → power
    int stateDefault = PowerLevel::MODERATOR;     // Default: 50
    std::unordered_map<std::string, int> notifications; // notification_key → power
    bool valid = false;

    // Original: setUserPowerLevel(userId, powerLevel)
    void setUserPower(const std::string& userId, int powerLevel);

    // Get user's power level (or usersDefault if not set).
    int getUserPower(const std::string& userId) const;

    // Original: notificationLevel(key)
    int notificationLevel(const std::string& key) const;

    // Get the role for a user.
    RoomRole getUserRole(const std::string& userId) const;
};

// ================================+===============================
// Room Permissions Manager
// ================================================================

class RoomPermissionsManager {
public:
    RoomPermissionsManager();

    // ====== Permission Checks ======
    // Original: banOrDefault(), kickOrDefault(), etc.

    // Check if a user can ban.
    bool canBan(int userPowerLevel) const;

    // Check if a user can kick.
    bool canKick(int userPowerLevel) const;

    // Check if a user can invite.
    bool canInvite(int userPowerLevel) const;

    // Check if a user can redact (delete messages).
    bool canRedact(int userPowerLevel) const;

    // Check if a user can send messages.
    bool canSendMessage(int userPowerLevel) const;

    // Check if a user can send state events.
    bool canSendState(int userPowerLevel) const;

    // Check if a user can send a specific event type.
    bool canSendEvent(int userPowerLevel, const std::string& eventType) const;

    // Check if a user can change power levels (needs ADMIN).
    bool canChangePowerLevels(int userPowerLevel) const;

    // Check if a user can send @room notification.
    bool canNotifyRoom(int userPowerLevel) const;

    // Check if a user can change room settings.
    bool canChangeSettings(int userPowerLevel) const;

    // ====== Power Level Content ======
    // Original: Parse/parse m.room.power_levels state event

    // Parse power levels from state event content JSON.
    PowerLevelsContent parsePowerLevels(const std::string& contentJson);

    // Build power levels state event content JSON.
    std::string buildPowerLevelsContent(const PowerLevelsContent& pl);

    // Get the current power levels for the room.
    PowerLevelsContent getPowerLevels() const;

    // Set the power levels for the room.
    void setPowerLevels(const PowerLevelsContent& pl);

    // Update power for a specific user.
    void setUserPower(const std::string& userId, int powerLevel);

    // ====== Role Management ======

    // Get the human-readable role name.
    std::string getRoleLabel(RoomRole role) const;
    std::string getRoleLabel(int powerLevel) const;

    // Get a description of what a power level can do.
    std::string getPowerDescription(int powerLevel) const;

    // Build a human-readable power level diff.
    std::string formatPowerLevelChange(const std::string& userId,
                                        int oldPower, int newPower) const;

    // ====== Moderation Actions ======

    // Build kick request body.
    std::string buildKickRequest(const std::string& userId, const std::string& reason = "");

    // Build ban request body.
    std::string buildBanRequest(const std::string& userId, const std::string& reason = "");

    // Build invite request body.
    std::string buildInviteRequest(const std::string& userId);

    // Build unban request body.
    std::string buildUnbanRequest(const std::string& userId);

    // ====== Serialization ======

    std::string powerLevelsToJson(const PowerLevelsContent& pl) const;
    std::string roleToJson(const std::string& userId, int powerLevel) const;

private:
    PowerLevelsContent powerLevels_;

    static std::string extractStr(const std::string& json, const std::string& key);
    static int extractInt(const std::string& json, const std::string& key);
};

} // namespace progressive
