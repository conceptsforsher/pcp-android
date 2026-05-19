#ifndef PROGRESSIVE_NOTIF_PRIORITY_HPP
#define PROGRESSIVE_NOTIF_PRIORITY_HPP

#include <string>

namespace progressive {

enum class NotifLevel { Silent, Low, Normal, High, Urgent };

struct NotifContent {
    std::string senderName;
    std::string body;
    std::string roomName;
    std::string roomId;
    bool isDirectMessage = false;
    bool isMention = false;       // @user ping
    bool isRoomMention = false;   // @room ping
    bool isKeywordMatch = false;  // custom notification keyword
    bool isCallInvite = false;
    bool isEncrypted = false;
};

struct NotifPriority {
    NotifLevel level = NotifLevel::Normal;
    bool shouldVibrate = false;
    bool shouldWakeScreen = false;
    int ledColor = 0x00000000;    // ARGB, 0 = no LED
    std::string soundUri;         // empty = default
    std::string category;         // Android notification category
};

// Compute notification priority from message content and user settings.
NotifPriority computeNotifPriority(const NotifContent& content,
    bool isBackground,             // app in background?
    bool doNotDisturb,             // DND mode active?
    bool isFavoriteRoom            // marked as favourite?
);

// Format notification title: "RoomName" or "SenderName (DM)"
std::string formatNotifTitle(const NotifContent& content);

// Format notification body: "Sender: message" or "message"
std::string formatNotifBody(const NotifContent& content, bool showSender);

// Truncate notification text to fit Android limit (usually ~40 chars for title).
std::string truncateForNotification(const std::string& text, int maxLen = 120);

// Check if text contains @room or @here mention.
bool isRoomMention(const std::string& body);

} // namespace progressive

#endif // PROGRESSIVE_NOTIF_PRIORITY_HPP
