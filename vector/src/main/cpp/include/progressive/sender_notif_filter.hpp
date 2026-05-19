#ifndef PROGRESSIVE_SENDER_NOTIF_FILTER_HPP
#define PROGRESSIVE_SENDER_NOTIF_FILTER_HPP

#include <string>
#include <vector>
#include <unordered_map>

namespace progressive {

// ---- Per-Room Sender Notification Filter ----
// Allows users to control which senders trigger notifications per room.
//
// Room settings → Notifications → Filter by sender:
//   [✓] @alice:server → notify
//   [✗] @bob:server → mute
//   [✓] Everyone else → notify (default)

enum class SenderNotifMode {
    NotifyAll,       // default — notify for all senders
    NotifyMentions,  // only @mentions and highlights
    NotifyList,      // only notify senders in allowedList
    MuteList,        // mute senders in mutedList, notify others
};

struct SenderNotifSettings {
    std::string roomId;
    SenderNotifMode mode = SenderNotifMode::NotifyAll;
    std::vector<std::string> allowedSenders;  // whitelist for NotifyList mode
    std::vector<std::string> mutedSenders;    // blacklist for MuteList mode
    bool muteUnknownSenders = false;          // mute senders not in room member list
};

// Check if a notification should fire for a given sender in a room.
// @param settings  Per-room notification settings
// @param senderId  Message sender
// @param hasMention  True if this message @mentions the user
// @param hasHighlight  True if this message matches highlight keywords
// @return true if notification should fire
bool shouldNotifyForSender(
    const SenderNotifSettings& settings,
    const std::string& senderId,
    bool hasMention,
    bool hasHighlight);

// Add a sender to the muted list for a room.
void muteSender(SenderNotifSettings& settings, const std::string& senderId);

// Remove a sender from the muted list.
void unmuteSender(SenderNotifSettings& settings, const std::string& senderId);

// Add a sender to the allowed list (NotifyList mode).
void allowSender(SenderNotifSettings& settings, const std::string& senderId);

// Remove a sender from the allowed list.
void disallowSender(SenderNotifSettings& settings, const std::string& senderId);

// Check if a sender is muted in this room.
bool isSenderMuted(const SenderNotifSettings& settings, const std::string& senderId);

// Check if a sender is explicitly allowed in this room.
bool isSenderAllowed(const SenderNotifSettings& settings, const std::string& senderId);

// Get a human-readable mode description.
std::string senderNotifModeToString(SenderNotifMode mode);
SenderNotifMode senderNotifModeFromString(const std::string& mode);

// Format settings as JSON for Kotlin UI.
std::string senderNotifSettingsToJson(const SenderNotifSettings& settings);

// Parse settings from JSON.
SenderNotifSettings parseSenderNotifSettings(const std::string& json);

// Get default settings for a room.
SenderNotifSettings getDefaultSenderNotifSettings(const std::string& roomId);

} // namespace progressive

#endif // PROGRESSIVE_SENDER_NOTIF_FILTER_HPP
