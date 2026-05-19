#ifndef PROGRESSIVE_USER_STATUS_HPP
#define PROGRESSIVE_USER_STATUS_HPP

#include <string>
#include <cstdint>

namespace progressive {

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

#endif // PROGRESSIVE_USER_STATUS_HPP
