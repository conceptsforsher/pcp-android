#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>

namespace progressive {

// ==== Push Notification Builder ====
//
// Constructs Android notification objects from Matrix push data.
// Used by the F-Droid push receiver to display local notifications
// without Google Play Services (FCM).

struct PushNotificationData {
    std::string title;               // Room name or sender name
    std::string body;                // Message preview
    std::string ticker;              // Status bar text
    std::string roomId;              // Which room to open on tap
    std::string eventId;             // Which event to jump to
    std::string senderName;          // Who sent the message
    std::string senderAvatarUrl;     // Avatar to show
    std::string roomAvatarUrl;       // Room avatar
    std::string sound;               // "default" or custom URI
    int notificationId = 0;          // Unique ID for this notification
    bool shouldVibrate = true;       // Whether to vibrate
    int priority = 0;                // Android notification priority (0=default)
    int unreadCount = 0;             // Badge count
    int highlightCount = 0;          // Mentions count
    std::string category;            // "msg", "call", etc.
    std::string groupKey;            // For grouping multiple notifications
};

// ==== Group Key Builder ====

// Build a notification group key from room ID.
// All messages from the same room are grouped together.
inline std::string buildNotificationGroupKey(const std::string& roomId) {
    return "progressive_room_" + roomId;
}

// ==== Notification Sound ====

// Get the notification sound URI based on notification state.
enum class NotifSoundType { DEFAULT = 0, SILENT = 1, URGENT = 2 };

inline const char* notificationSoundUri(NotifSoundType type) {
    switch (type) {
        case NotifSoundType::SILENT: return "";
        case NotifSoundType::URGENT: return "default";
        case NotifSoundType::DEFAULT: return "default";
    }
    return "default";
}

// ==== Notification Priority ====

// Map Matrix notification state to Android priority.
// ALL_MESSAGES_NOISY → high priority + sound
// ALL_MESSAGES → default priority
// MENTIONS_ONLY → low priority (only when mentioned)
// MUTE → silent notification

inline int notificationPriority(int highlightCount, bool isNoisy, bool isMuted) {
    if (isMuted) return -2;          // PRIORITY_MIN — no sound/vibration
    if (highlightCount > 0) return 2; // PRIORITY_HIGH — heads-up
    if (isNoisy) return 1;            // PRIORITY_DEFAULT
    return 0;                         // PRIORITY_LOW
}

// ==== Notification Summary ====

// Build a summary notification for multiple messages.
// "5 new messages in 3 rooms"
inline std::string buildNotificationSummary(int totalMessages, int roomCount) {
    if (totalMessages <= 0) return "";
    std::string msg = std::to_string(totalMessages) + " new message" + (totalMessages > 1 ? "s" : "");
    if (roomCount > 1) msg += " in " + std::to_string(roomCount) + " rooms";
    return msg;
}

// ==== Push Rule Matching ====

// Check if an event matches a push rule condition.
// Per Matrix spec: event_match conditions support glob patterns.

inline bool matchPushCondition(const std::string& fieldValue, const std::string& pattern) {
    // Simple glob matching: * = any chars, ? = any char
    // Pattern is case-insensitive per spec
    if (pattern == "*") return true;

    std::string fv = fieldValue;
    std::string pat = pattern;

    // Case insensitive
    for (char& c : fv) c = tolower(c);
    for (char& c : pat) c = tolower(c);

    // If no special glob chars, do substring match (per Matrix spec for content.body)
    if (pat.find('*') == std::string::npos && pat.find('?') == std::string::npos) {
        return fv.find(pat) != std::string::npos;
    }

    // Convert glob to regex for matching
    // * → .*, ? → .
    std::string regex;
    for (char c : pat) {
        if (c == '*') regex += ".*";
        else if (c == '?') regex += ".";
        else if (c == '.' || c == '\\' || c == '+' || c == '[' || c == ']' ||
                 c == '(' || c == ')' || c == '{' || c == '}' || c == '^' || c == '$') {
            regex += '\\'; regex += c;
        } else {
            regex += c;
        }
    }

    // Simple regex match (std::regex not available on all NDK versions)
    // Use manual matching: the glob * stays as substring check for simple cases
    if (pat == "*" + fv || pat == fv + "*" || pat == "*" + fv + "*") {
        std::string search = pat;
        // Remove leading/trailing *
        if (!search.empty() && search[0] == '*') search = search.substr(1);
        if (!search.empty() && search.back() == '*') search.pop_back();
        return fv.find(search) != std::string::npos;
    }

    return false;
}

// ==== Device Notification Dedup ====

// Track which events have already been notified to avoid duplicates.
class NotificationDedup {
public:
    // Check if this event was already notified.
    bool isDuplicate(const std::string& eventId) {
        if (seen_.count(eventId)) return true;
        seen_.insert(eventId);
        if (seen_.size() > 500) {
            // Evict oldest entries
            auto it = seen_.begin();
            for (int i = 0; i < 100 && it != seen_.end(); i++) {
                it = seen_.erase(it);
            }
        }
        return false;
    }

    // Clear all tracked events.
    void clear() { seen_.clear(); }

private:
    std::unordered_set<std::string> seen_;
};

} // namespace progressive
