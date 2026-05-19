#ifndef PROGRESSIVE_NOTIF_SETTINGS_HPP
#define PROGRESSIVE_NOTIF_SETTINGS_HPP

#include <string>
#include <vector>

namespace progressive {

// ---- Notification Settings ----

enum class NotifMode {
    All,          // notify for every message
    Mentions,     // only @mentions
    None,         // mute completely
    Default       // inherit from room defaults
};

struct RoomNotifSettings {
    std::string roomId;
    NotifMode mode = NotifMode::Default;
    bool isEncryptedRoom = false;     // encrypted DMs use Mentions by default
    bool isDirectChat = false;
    bool hasMention = false;
    bool hasKeyword = false;
};

struct NotifDecision {
    bool shouldNotify = true;
    bool shouldHighlight = false;  // noisy notification
    bool shouldVibrate = false;
    std::string soundName;         // empty = default
    std::string reason;            // why this decision was made
};

// Compute notification decision based on room settings and message content.
NotifDecision computeNotifDecision(
    const RoomNotifSettings& roomSettings,
    bool isMention,
    bool isRoomMention,
    bool isKeywordMatch,
    bool isInvite,
    bool isCall,
    bool isDirectChat,
    const std::string& senderId,
    const std::string& myUserId
);

// Check if a room's notification mode should be upgraded.
bool shouldUpgradeToMentions(const RoomNotifSettings& settings);

// Format notification mode as human-readable text.
std::string formatNotifMode(NotifMode mode);

// Parse notification mode from Matrix push rule action.
NotifMode parseNotifMode(const std::string& action);

// Build notification settings JSON for room account data.
std::string buildRoomNotifSettingsBody(NotifMode mode);

// Check if notification mode would change behavior.
bool isNotifModeDifferent(NotifMode oldMode, NotifMode newMode);

// Get the recommended default mode for a room type.
NotifMode getDefaultModeForRoom(bool isDirect, bool isEncrypted);

} // namespace progressive

#endif // PROGRESSIVE_NOTIF_SETTINGS_HPP
