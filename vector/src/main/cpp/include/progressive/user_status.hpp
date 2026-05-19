#pragma once

#include <string>
#include <cstdint>
#include <vector>
#include <map>

namespace progressive {

// ==== User Status Type ====
// Original Kotlin: user presence/status types

enum class UserStatusType {
    ONLINE,
    OFFLINE,
    AWAY,
    BUSY,
    CUSTOM     // Custom status message set by user
};

// ==== User Status Message ====
// Original Kotlin: custom status with emoji and auto-reply

struct UserStatusMessage {
    std::string message;       // Status text (e.g. "In a meeting")
    std::string emoji;         // Status emoji (e.g. "💼")
    bool isAutoReply = false;  // Auto-reply mode enabled
};

// ==== User Status Info ====
// Original Kotlin: full user status with expiry tracking

struct UserStatusInfo {
    std::string userId;           // Matrix user ID
    UserStatusType status = UserStatusType::OFFLINE;
    UserStatusMessage message;    // Custom status message (if CUSTOM type)
    int64_t lastChangedMs = 0;  // Last status change timestamp
    int64_t expiresAtMs = 0;    // 0 = never expires

    // Convenience check
    bool isActive() const { return status == UserStatusType::ONLINE; }
    bool hasCustomMessage() const { return status == UserStatusType::CUSTOM && !message.message.empty(); }
};

// ==== User Status Preset ====
// Original Kotlin: predefined statuses (like Element Web presets)

struct UserStatusPreset {
    std::string label;    // "Available", "Away", etc.
    std::string emoji;    // "🟢", "🟡", etc.
    UserStatusType type;

    static std::vector<UserStatusPreset> getPresets();
    static UserStatusPreset getPreset(UserStatusType type);
};

// Get an emoji icon for a status type.
// Original Kotlin: status icon for UI indicators
std::string getStatusIcon(UserStatusType type);

// Get a color string for a status indicator.
// Returns "#RRGGBB" hex color.
// Original Kotlin: color-coded presence indicators
std::string getStatusColor(UserStatusType type);

// ==== User Activity Status ====
// Original Kotlin: derived from multiple presence/activity signals

struct UserActivityStatus {
    bool isActive = false;        // User is currently active
    int64_t lastTypingTs = 0;   // Last typing indicator timestamp
    int64_t lastReadTs = 0;     // Last read receipt timestamp
    int64_t lastPresenceTs = 0; // Last presence update timestamp

    // Compute derived "active" state from combined signals.
    bool isRecentlyActive(int64_t nowMs, int64_t thresholdMs = 300000) const {
        if ((nowMs - lastTypingTs) < thresholdMs) return true;
        if ((nowMs - lastPresenceTs) < thresholdMs) return true;
        return false;
    }
};

// Compute activity status from multiple signals.
// Original Kotlin: derive user status from event streams
UserActivityStatus computeUserActivityStatus(
    bool isCurrentlyTyping,
    int64_t lastTypingTs,
    int64_t lastReadTs,
    int64_t lastPresenceTs);

// ---- User Status / Custom Status Messages ----
// Like Element Web: users can set a custom status with emoji.
// Stored as account data event: im.vector.user_status
// Displayed in profile, room list (DM), and timeline header.

struct UserStatus {
    std::string status;           // "In a meeting", "On vacation", "🎮 Gaming"
    std::string emoji;            // "🎮" or "" for no emoji
    int64_t setAtMs = 0;         // when the status was set (epoch ms)
    bool isSet = false;           // user has a status
    bool isExpired = false;       // status has timed out (optional expiry)
    int64_t expiresAtMs = 0;     // 0 = never expires

    // Format for display: "🎮 Gaming" or just "Gaming"
    std::string displayText() const;
    // Format short: "🎮" or ""
    std::string emojiOnly() const;

    // Check if empty (no status or blank)
    bool isEmpty() const;
};

// Build the m.user_status event content JSON for setting a status.
// Original Kotlin: setUserStatus() builds the event
std::string setUserStatus(const std::string& userId, const UserStatusMessage& msg, int64_t nowMs);

// Parse m.user_status event JSON.
// Format: {"userId":"@alice:matrix.org","status":"online","message":"...","emoji":"💼"}
// Original Kotlin: parse user status event from timeline
UserStatusInfo parseUserStatusEvent(const std::string& eventJson);

// Format user status as human-readable description.
// "Alice is away"
// "Alice is busy (In a meeting)"
std::string formatUserStatus(const UserStatusInfo& info);

// Parse im.vector.user_status account data JSON.
// Format: {"status": "In a meeting", "emoji": "💼", "setAt": 1715700000000}
UserStatus parseUserStatus(const std::string& accountDataJson);

// Build the account data JSON for setting a status.
std::string buildUserStatusJson(const std::string& status, const std::string& emoji, int64_t nowMs);

// Check if a status has expired (if expiry is set).
bool isStatusExpired(const UserStatus& status);

// Format status for display in different contexts.
std::string formatStatusForProfile(const UserStatus& status);
std::string formatStatusForRoomList(const UserStatus& status);   // DM preview
std::string formatStatusForTimeline(const UserStatus& status);   // under name

// Parse multiple statuses from presence + user_status events.
// Returns the best available status display.
UserStatus resolveBestStatus(const UserStatus& customStatus, bool isOnline, int64_t lastActiveMs);

// Get a default status based on presence.
std::string getPresenceStatusText(bool isOnline, int64_t lastActiveMs);

// Format status as JSON for Kotlin UI.
std::string userStatusToJson(const UserStatus& status);

// Suggested status presets (like Element Web).
std::vector<std::string> getStatusSuggestions();

} // namespace progressive
