#include "progressive/notif_format.hpp"
#include <sstream>
#include <iomanip>
#include <cmath>

namespace progressive {

// ============================================================================
// NotifSound
// ============================================================================

std::string notifSoundToUri(NotifSound sound) {
    switch (sound) {
        case NotifSound::DEFAULT:    return "content://settings/system/notification_sound";
        case NotifSound::SILENT:     return "";
        case NotifSound::RING_ONCE:  return "content://settings/system/ringtone";
        case NotifSound::RING_REPEAT: return "content://settings/system/alarm_alert";
        case NotifSound::CUSTOM:     return "content://";
        default:                     return "";
    }
}

NotifSound notifSoundFromString(const std::string& str) {
    if (str == "DEFAULT")      return NotifSound::DEFAULT;
    if (str == "SILENT")       return NotifSound::SILENT;
    if (str == "RING_ONCE")    return NotifSound::RING_ONCE;
    if (str == "RING_REPEAT")  return NotifSound::RING_REPEAT;
    if (str == "CUSTOM")       return NotifSound::CUSTOM;
    return NotifSound::DEFAULT;
}

// ============================================================================
// NotifPriority
// ============================================================================

int notifPriorityToAndroidValue(NotifPriority priority) {
    return static_cast<int>(priority);
}

NotifPriority notifPriorityFromAndroidValue(int value) {
    switch (value) {
        case -2: return NotifPriority::MIN;
        case -1: return NotifPriority::LOW;
        case  0: return NotifPriority::DEFAULT;
        case  1: return NotifPriority::HIGH;
        case  2: return NotifPriority::MAX;
        default: return NotifPriority::DEFAULT;
    }
}

std::string notifPriorityToString(NotifPriority priority) {
    switch (priority) {
        case NotifPriority::DEFAULT: return "DEFAULT";
        case NotifPriority::HIGH:    return "HIGH";
        case NotifPriority::LOW:     return "LOW";
        case NotifPriority::MIN:     return "MIN";
        case NotifPriority::MAX:     return "MAX";
        default:                     return "DEFAULT";
    }
}

NotifPriority notifPriorityFromString(const std::string& str) {
    if (str == "DEFAULT") return NotifPriority::DEFAULT;
    if (str == "HIGH")    return NotifPriority::HIGH;
    if (str == "LOW")     return NotifPriority::LOW;
    if (str == "MIN")     return NotifPriority::MIN;
    if (str == "MAX")     return NotifPriority::MAX;
    return NotifPriority::DEFAULT;
}

// ============================================================================
// NotifImportance
// ============================================================================

std::string notifImportanceToChannelName(NotifImportance importance) {
    switch (importance) {
        case NotifImportance::NONE:    return "Miscellaneous";
        case NotifImportance::MIN:     return "Low priority";
        case NotifImportance::LOW:     return "Messages";
        case NotifImportance::DEFAULT: return "Notifications";
        case NotifImportance::HIGH:    return "Direct messages";
        case NotifImportance::MAX:     return "Calls";
        default:                       return "Notifications";
    }
}

int notifImportanceToAndroidValue(NotifImportance importance) {
    return static_cast<int>(importance);
}

NotifImportance notifImportanceFromAndroidValue(int value) {
    switch (value) {
        case 0: return NotifImportance::NONE;
        case 1: return NotifImportance::MIN;
        case 2: return NotifImportance::LOW;
        case 3: return NotifImportance::DEFAULT;
        case 4: return NotifImportance::HIGH;
        case 5: return NotifImportance::MAX;
        default: return NotifImportance::DEFAULT;
    }
}

std::string notifImportanceToString(NotifImportance importance) {
    switch (importance) {
        case NotifImportance::NONE:    return "NONE";
        case NotifImportance::MIN:     return "MIN";
        case NotifImportance::LOW:     return "LOW";
        case NotifImportance::DEFAULT: return "DEFAULT";
        case NotifImportance::HIGH:    return "HIGH";
        case NotifImportance::MAX:     return "MAX";
        default:                       return "DEFAULT";
    }
}

NotifImportance notifImportanceFromString(const std::string& str) {
    if (str == "NONE")    return NotifImportance::NONE;
    if (str == "MIN")     return NotifImportance::MIN;
    if (str == "LOW")     return NotifImportance::LOW;
    if (str == "DEFAULT") return NotifImportance::DEFAULT;
    if (str == "HIGH")    return NotifImportance::HIGH;
    if (str == "MAX")     return NotifImportance::MAX;
    return NotifImportance::DEFAULT;
}

// ============================================================================
// NotifVisibility
// ============================================================================

int notifVisibilityToAndroidValue(NotifVisibility visibility) {
    return static_cast<int>(visibility);
}

NotifVisibility notifVisibilityFromAndroidValue(int value) {
    switch (value) {
        case  1: return NotifVisibility::PUBLIC;
        case  0: return NotifVisibility::PRIVATE;
        case -1: return NotifVisibility::SECRET;
        default: return NotifVisibility::PRIVATE;
    }
}

std::string notifVisibilityToString(NotifVisibility visibility) {
    switch (visibility) {
        case NotifVisibility::PUBLIC:  return "PUBLIC";
        case NotifVisibility::PRIVATE: return "PRIVATE";
        case NotifVisibility::SECRET:  return "SECRET";
        default:                       return "PRIVATE";
    }
}

NotifVisibility notifVisibilityFromString(const std::string& str) {
    if (str == "PUBLIC")  return NotifVisibility::PUBLIC;
    if (str == "PRIVATE") return NotifVisibility::PRIVATE;
    if (str == "SECRET")  return NotifVisibility::SECRET;
    return NotifVisibility::PRIVATE;
}

// ============================================================================
// NotificationChannel
// ============================================================================

std::string NotificationChannel::effectiveSoundUri() const {
    if (sound == NotifSound::SILENT) return "";
    if (sound == NotifSound::CUSTOM) return customSoundUri;
    return notifSoundToUri(sound);
}

static std::string jsonEscape(const std::string& s) {
    std::string out;
    for (char c : s) {
        switch (c) {
            case '"':  out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\n': out += "\\n";  break;
            case '\r': out += "\\r";  break;
            case '\t': out += "\\t";  break;
            default:   out += c;      break;
        }
    }
    return out;
}

std::string buildNotificationChannel(const NotificationChannel& channel) {
    std::string json = "{";
    json += "\"channelId\":\"" + jsonEscape(channel.channelId) + "\",";
    json += "\"name\":\"" + jsonEscape(channel.name) + "\",";
    json += "\"importance\":\"" + notifImportanceToString(channel.importance) + "\",";
    json += "\"importanceValue\":" + std::to_string(notifImportanceToAndroidValue(channel.importance)) + ",";
    json += "\"sound\":\"" + notifSoundToUri(channel.sound) + "\",";
    json += "\"soundPolicy\":\"" + std::to_string(static_cast<int>(channel.sound)) + "\",";
    json += "\"enableVibration\":" + std::string(channel.enableVibration ? "true" : "false") + ",";
    json += "\"description\":\"" + jsonEscape(channel.description) + "\",";
    json += "\"lightColor\":" + std::to_string(channel.lightColor) + ",";
    json += "\"lockscreenVisibility\":\"" + notifVisibilityToString(channel.lockscreenVisibility) + "\",";
    json += "\"lockscreenVisibilityValue\":" + std::to_string(notifVisibilityToAndroidValue(channel.lockscreenVisibility)) + ",";
    json += "\"showBadge\":" + std::string(channel.showBadge ? "true" : "false") + ",";
    json += "\"bypassDnd\":" + std::string(channel.bypassDnd ? "true" : "false") + ",";
    json += "\"vibrationPatternMs\":" + std::to_string(channel.vibrationPatternMs);
    json += "}";
    return json;
}

std::string notificationChannelToJson(const NotificationChannel& channel) {
    return buildNotificationChannel(channel);
}

// Manual JSON extraction helpers
static std::string extractJsonStr(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return "";
    pos = json.find(':', pos);
    if (pos == std::string::npos) return "";
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n')) pos++;
    if (pos >= json.size() || json[pos] != '"') return "";
    pos++;
    size_t end = pos;
    while (end < json.size() && json[end] != '"') {
        if (json[end] == '\\') end++;
        end++;
    }
    return json.substr(pos, end - pos);
}

static int extractJsonInt(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return 0;
    pos = json.find(':', pos);
    if (pos == std::string::npos) return 0;
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n')) pos++;
    if (pos >= json.size()) return 0;
    bool neg = false;
    if (json[pos] == '-') { neg = true; pos++; }
    int val = 0;
    while (pos < json.size() && json[pos] >= '0' && json[pos] <= '9') {
        val = val * 10 + (json[pos] - '0');
        pos++;
    }
    return neg ? -val : val;
}

static int64_t extractJsonInt64(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return 0;
    pos = json.find(':', pos);
    if (pos == std::string::npos) return 0;
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n')) pos++;
    if (pos >= json.size()) return 0;
    bool neg = false;
    if (json[pos] == '-') { neg = true; pos++; }
    int64_t val = 0;
    while (pos < json.size() && json[pos] >= '0' && json[pos] <= '9') {
        val = val * 10 + (json[pos] - '0');
        pos++;
    }
    return neg ? -val : val;
}

static bool extractJsonBool(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return false;
    pos = json.find(':', pos);
    if (pos == std::string::npos) return false;
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n')) pos++;
    return json.compare(pos, 4, "true") == 0;
}

NotificationChannel parseNotificationChannel(const std::string& json) {
    NotificationChannel channel;
    channel.channelId = extractJsonStr(json, "channelId");
    channel.name = extractJsonStr(json, "name");
    channel.importance = notifImportanceFromString(extractJsonStr(json, "importance"));
    channel.customSoundUri = extractJsonStr(json, "sound");
    channel.sound = notifSoundFromString(extractJsonStr(json, "soundPolicy"));
    channel.enableVibration = extractJsonBool(json, "enableVibration");
    channel.description = extractJsonStr(json, "description");
    channel.lightColor = extractJsonInt(json, "lightColor");
    channel.lockscreenVisibility = notifVisibilityFromString(extractJsonStr(json, "lockscreenVisibility"));
    channel.showBadge = extractJsonBool(json, "showBadge");
    channel.bypassDnd = extractJsonBool(json, "bypassDnd");
    channel.vibrationPatternMs = extractJsonInt64(json, "vibrationPatternMs");
    return channel;
}

// ============================================================================
// Predefined channels
// ============================================================================
//
// Original Kotlin (NotificationUtils.kt / NotificationChannels.kt)

NotificationChannel createNoisyNotificationChannel() {
    NotificationChannel ch;
    ch.channelId = "DEFAULT_NOISY_NOTIFICATION_CHANNEL_ID";
    ch.name = "Noisy notifications";
    ch.importance = NotifImportance::HIGH;
    ch.sound = NotifSound::DEFAULT;
    ch.enableVibration = true;
    ch.description = "Notifications with sound for direct messages and mentions";
    ch.lightColor = 0xFF00FF00;    // Green
    ch.lockscreenVisibility = NotifVisibility::PUBLIC;
    ch.showBadge = true;
    ch.bypassDnd = false;
    return ch;
}

NotificationChannel createSilentNotificationChannel() {
    NotificationChannel ch;
    ch.channelId = "DEFAULT_SILENT_NOTIFICATION_CHANNEL_ID";
    ch.name = "Silent notifications";
    ch.importance = NotifImportance::LOW;
    ch.sound = NotifSound::SILENT;
    ch.enableVibration = false;
    ch.description = "Notifications without sound for general messages";
    ch.lightColor = 0;
    ch.lockscreenVisibility = NotifVisibility::PRIVATE;
    ch.showBadge = true;
    ch.bypassDnd = false;
    return ch;
}

NotificationChannel createCallNotificationChannel() {
    NotificationChannel ch;
    ch.channelId = "CALL_NOTIFICATION_CHANNEL_ID";
    ch.name = "Voice & video calls";
    ch.importance = NotifImportance::MAX;
    ch.sound = NotifSound::RING_ONCE;
    ch.enableVibration = true;
    ch.description = "Incoming voice and video call notifications";
    ch.lightColor = 0xFF0000FF;    // Blue
    ch.lockscreenVisibility = NotifVisibility::PUBLIC;
    ch.showBadge = true;
    ch.bypassDnd = true;
    ch.vibrationPatternMs = 0;     // Default vibrate pattern
    return ch;
}

NotificationChannel createBackgroundSyncChannel() {
    NotificationChannel ch;
    ch.channelId = "BACKGROUND_SYNC_CHANNEL_ID";
    ch.name = "Background sync";
    ch.importance = NotifImportance::MIN;
    ch.sound = NotifSound::SILENT;
    ch.enableVibration = false;
    ch.description = "Background synchronization service";
    ch.lightColor = 0;
    ch.lockscreenVisibility = NotifVisibility::SECRET;
    ch.showBadge = false;
    ch.bypassDnd = false;
    return ch;
}

// ============================================================================
// Channel registration helpers
// ============================================================================

std::string buildAllNotificationChannelsJson() {
    std::string json = "[";

    json += buildNotificationChannel(createNoisyNotificationChannel()) + ",";
    json += buildNotificationChannel(createSilentNotificationChannel()) + ",";
    json += buildNotificationChannel(createCallNotificationChannel()) + ",";
    json += buildNotificationChannel(createBackgroundSyncChannel());

    json += "]";
    return json;
}

std::string buildChannelRegistrationInfo(const NotificationChannel& channel) {
    std::string json = "{";
    json += "\"channelId\":\"" + jsonEscape(channel.channelId) + "\",";
    json += "\"name\":\"" + jsonEscape(channel.name) + "\",";
    json += "\"importance\":" + std::to_string(notifImportanceToAndroidValue(channel.importance)) + ",";
    json += "\"soundUri\":\"" + jsonEscape(channel.effectiveSoundUri()) + "\",";
    json += "\"vibration\":" + std::string(channel.enableVibration ? "true" : "false") + ",";
    json += "\"lightColor\":" + std::to_string(channel.lightColor);
    json += "}";
    return json;
}

// ============================================================================
// Notification Count Formatter (legacy)
// ============================================================================
// Original Kotlin (RoomSummaryFormatter.kt:18-24):
//   fun formatUnreadMessagesCounter(count: Int): String {
//       return if (count > 999) "${count / 1000}.${count % 1000 / 100}k"
//       else count.toString()
//   }

std::string formatUnreadCounter(int count) {
    if (count <= 0) return "0";
    if (count <= 999) return std::to_string(count);

    int thousands = count / 1000;
    int hundreds = (count % 1000) / 100;

    std::ostringstream out;
    out << thousands << "." << hundreds << "k";
    return out.str();
}

std::string formatNotificationCount(int count, int highlightCount) {
    if (count <= 0 && highlightCount <= 0) return "";

    std::ostringstream out;
    out << formatUnreadCounter(count);
    if (highlightCount > 0) out << "!";
    return out.str();
}

std::string formatThreadNotificationCount(int threadCount, int threadHighlightCount) {
    if (threadCount <= 0) return "";
    return formatNotificationCount(threadCount, threadHighlightCount);
}

std::string formatCombinedNotificationCount(int roomCount, int threadCount) {
    if (roomCount <= 0 && threadCount <= 0) return "";

    std::ostringstream out;
    out << formatUnreadCounter(roomCount);
    if (threadCount > 0) {
        out << " (" << formatUnreadCounter(threadCount) << ")";
    }
    return out.str();
}

int getTotalUnreadCount(int roomCount, int threadCount) {
    return std::max(0, roomCount) + std::max(0, threadCount);
}

std::string formatBadgeText(int totalCount) {
    if (totalCount <= 0) return "";
    if (totalCount <= 99) return std::to_string(totalCount);
    if (totalCount <= 999) return std::to_string(totalCount);
    return "999+";
}

} // namespace progressive
