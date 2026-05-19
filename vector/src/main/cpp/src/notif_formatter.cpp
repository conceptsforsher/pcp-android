#include "progressive/notif_formatter.hpp"
#include <algorithm>
#include <sstream>

namespace progressive {

// ============================================================================
// NotificationStyle
// ============================================================================

std::string notificationStyleToString(NotificationStyle style) {
    switch (style) {
        case NotificationStyle::INBOX:       return "INBOX";
        case NotificationStyle::BIG_TEXT:    return "BIG_TEXT";
        case NotificationStyle::BIG_PICTURE: return "BIG_PICTURE";
        case NotificationStyle::MESSAGING:   return "MESSAGING";
        case NotificationStyle::MEDIA:       return "MEDIA";
        default:                             return "BIG_TEXT";
    }
}

NotificationStyle notificationStyleFromString(const std::string& str) {
    if (str == "INBOX")       return NotificationStyle::INBOX;
    if (str == "BIG_TEXT")    return NotificationStyle::BIG_TEXT;
    if (str == "BIG_PICTURE") return NotificationStyle::BIG_PICTURE;
    if (str == "MESSAGING")   return NotificationStyle::MESSAGING;
    if (str == "MEDIA")       return NotificationStyle::MEDIA;
    return NotificationStyle::BIG_TEXT;
}

// ============================================================================
// Manual JSON helpers
// ============================================================================

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

static std::string extractStr(const std::string& json, const std::string& key) {
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

static int64_t extractInt64(const std::string& json, const std::string& key) {
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

static int extractInt(const std::string& json, const std::string& key) {
    return static_cast<int>(extractInt64(json, key));
}

static bool extractBool(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return false;
    pos = json.find(':', pos);
    if (pos == std::string::npos) return false;
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n')) pos++;
    return json.compare(pos, 4, "true") == 0;
}

static std::string extractObj(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return "";
    pos = json.find(':', pos);
    if (pos == std::string::npos) return "";
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n')) pos++;
    if (pos >= json.size() || json[pos] != '{') return "";
    int depth = 1;
    size_t start = pos;
    pos++;
    while (pos < json.size() && depth > 0) {
        if (json[pos] == '{') depth++;
        else if (json[pos] == '}') depth--;
        pos++;
    }
    return json.substr(start, pos - start);
}

static std::vector<std::string> extractStrArray(const std::string& json, const std::string& key) {
    std::vector<std::string> result;
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return result;
    pos = json.find('[', pos);
    if (pos == std::string::npos) return result;
    size_t start = pos + 1;
    while (start < json.size() && json[start] != ']') {
        while (start < json.size() && (json[start] == ' ' || json[start] == '\t' || json[start] == '\n' || json[start] == ',')) start++;
        if (start >= json.size() || json[start] == ']') break;
        if (json[start] == '"') {
            start++;
            std::string value;
            while (start < json.size() && json[start] != '"') {
                if (json[start] == '\\' && start + 1 < json.size()) start++;
                value += json[start];
                start++;
            }
            start++; // skip closing "
            result.push_back(value);
        } else {
            start++;
        }
    }
    return result;
}

// ============================================================================
// NotifAction
// ============================================================================

std::string notifActionToJson(const NotifAction& action) {
    std::string json = "{";
    json += "\"actionId\":\"" + jsonEscape(action.actionId) + "\",";
    json += "\"title\":\"" + jsonEscape(action.title) + "\",";
    json += "\"icon\":\"" + jsonEscape(action.icon) + "\",";
    json += "\"intentUri\":\"" + jsonEscape(action.intentUri) + "\",";
    json += "\"intentAction\":\"" + jsonEscape(action.intentAction) + "\",";
    json += "\"intentData\":\"" + jsonEscape(action.intentData) + "\",";
    json += "\"isAuthenticationRequired\":" + std::string(action.isAuthenticationRequired ? "true" : "false") + ",";
    json += "\"isQuickReply\":" + std::string(action.isQuickReply ? "true" : "false");
    json += "}";
    return json;
}

NotifAction parseNotifAction(const std::string& json) {
    NotifAction action;
    action.actionId = extractStr(json, "actionId");
    action.title = extractStr(json, "title");
    action.icon = extractStr(json, "icon");
    action.intentUri = extractStr(json, "intentUri");
    action.intentAction = extractStr(json, "intentAction");
    action.intentData = extractStr(json, "intentData");
    action.isAuthenticationRequired = extractBool(json, "isAuthenticationRequired");
    action.isQuickReply = extractBool(json, "isQuickReply");
    return action;
}

// ============================================================================
// NotifPerson
// ============================================================================

std::string notifPersonToJson(const NotifPerson& person) {
    std::string json = "{";
    json += "\"name\":\"" + jsonEscape(person.name) + "\",";
    json += "\"key\":\"" + jsonEscape(person.key) + "\",";
    json += "\"uri\":\"" + jsonEscape(person.uri) + "\",";
    json += "\"isBot\":" + std::string(person.isBot ? "true" : "false") + ",";
    json += "\"isImportant\":" + std::string(person.isImportant ? "true" : "false");
    json += "}";
    return json;
}

NotifPerson parseNotifPerson(const std::string& json) {
    NotifPerson person;
    person.name = extractStr(json, "name");
    person.key = extractStr(json, "key");
    person.uri = extractStr(json, "uri");
    person.isBot = extractBool(json, "isBot");
    person.isImportant = extractBool(json, "isImportant");
    return person;
}

// ============================================================================
// NotifMessage
// ============================================================================

std::string notifMessageToJson(const NotifMessage& message) {
    std::string json = "{";
    json += "\"text\":\"" + jsonEscape(message.text) + "\",";
    json += "\"timestampMs\":" + std::to_string(message.timestampMs) + ",";
    json += "\"sender\":" + notifPersonToJson(message.sender) + ",";
    json += "\"dataMimeType\":\"" + jsonEscape(message.dataMimeType) + "\",";
    json += "\"dataUri\":\"" + jsonEscape(message.dataUri) + "\"";
    json += "}";
    return json;
}

NotifMessage parseNotifMessage(const std::string& json) {
    NotifMessage message;
    message.text = extractStr(json, "text");
    message.timestampMs = extractInt64(json, "timestampMs");
    message.sender = parseNotifPerson(extractObj(json, "sender"));
    message.dataMimeType = extractStr(json, "dataMimeType");
    message.dataUri = extractStr(json, "dataUri");
    return message;
}

// ============================================================================
// NotificationBuilder
// ============================================================================
//
// Original Kotlin (NotificationBuilder.kt):
//   Builds a full Android Notification object description as JSON.

std::string buildNotificationJson(const NotificationBuilder& builder) {
    std::string json = "{";

    // Channel & identity
    json += "\"channelId\":\"" + jsonEscape(builder.channelId) + "\",";
    json += "\"groupKey\":\"" + jsonEscape(builder.groupKey) + "\",";
    json += "\"groupSummary\":" + std::string(builder.groupSummary ? "true" : "false") + ",";

    // Content
    json += "\"title\":\"" + jsonEscape(builder.title) + "\",";
    json += "\"contentText\":\"" + jsonEscape(builder.contentText) + "\",";
    json += "\"subText\":\"" + jsonEscape(builder.subText) + "\",";
    json += "\"bigText\":\"" + jsonEscape(builder.bigText) + "\",";
    json += "\"ticker\":\"" + jsonEscape(builder.ticker) + "\",";
    json += "\"summaryText\":\"" + jsonEscape(builder.summaryText) + "\",";

    // Visual
    json += "\"smallIcon\":\"" + jsonEscape(builder.smallIcon) + "\",";
    json += "\"largeIconUri\":\"" + jsonEscape(builder.largeIconUri) + "\",";
    json += "\"color\":" + std::to_string(builder.color) + ",";
    json += "\"bigPictureUri\":\"" + jsonEscape(builder.bigPictureUri) + "\",";

    // Behavior
    json += "\"priority\":" + std::to_string(builder.priority) + ",";
    json += "\"category\":\"" + jsonEscape(builder.category) + "\",";
    json += "\"autoCancel\":" + std::string(builder.autoCancel ? "true" : "false") + ",";
    json += "\"ongoing\":" + std::string(builder.ongoing ? "true" : "false") + ",";
    json += "\"onlyAlertOnce\":" + std::string(builder.onlyAlertOnce ? "true" : "false") + ",";
    json += "\"timestampMs\":" + std::to_string(builder.timestampMs) + ",";

    // Style
    json += "\"style\":\"" + notificationStyleToString(builder.style) + "\",";

    // Actions array
    json += "\"actions\":[";
    for (size_t i = 0; i < builder.actions.size(); ++i) {
        if (i > 0) json += ",";
        json += notifActionToJson(builder.actions[i]);
    }
    json += "],";

    // Messages array
    json += "\"messages\":[";
    for (size_t i = 0; i < builder.messages.size(); ++i) {
        if (i > 0) json += ",";
        json += notifMessageToJson(builder.messages[i]);
    }
    json += "],";

    // Persons array
    json += "\"persons\":[";
    for (size_t i = 0; i < builder.persons.size(); ++i) {
        if (i > 0) json += ",";
        json += notifPersonToJson(builder.persons[i]);
    }
    json += "],";

    // Inbox lines array
    json += "\"inboxLines\":[";
    for (size_t i = 0; i < builder.inboxLines.size(); ++i) {
        if (i > 0) json += ",";
        json += "\"" + jsonEscape(builder.inboxLines[i]) + "\"";
    }
    json += "],";

    // Visibility & badge
    json += "\"visibility\":" + std::to_string(builder.visibility) + ",";
    json += "\"badgeNumber\":" + std::to_string(builder.badgeNumber);

    json += "}";
    return json;
}

NotificationBuilder parseNotificationBuilder(const std::string& json) {
    NotificationBuilder builder;
    builder.channelId = extractStr(json, "channelId");
    builder.groupKey = extractStr(json, "groupKey");
    builder.groupSummary = extractBool(json, "groupSummary");
    builder.title = extractStr(json, "title");
    builder.contentText = extractStr(json, "contentText");
    builder.subText = extractStr(json, "subText");
    builder.bigText = extractStr(json, "bigText");
    builder.ticker = extractStr(json, "ticker");
    builder.summaryText = extractStr(json, "summaryText");
    builder.smallIcon = extractStr(json, "smallIcon");
    builder.largeIconUri = extractStr(json, "largeIconUri");
    builder.color = extractInt(json, "color");
    builder.bigPictureUri = extractStr(json, "bigPictureUri");
    builder.priority = extractInt(json, "priority");
    builder.category = extractStr(json, "category");
    builder.autoCancel = extractBool(json, "autoCancel");
    builder.ongoing = extractBool(json, "ongoing");
    builder.onlyAlertOnce = extractBool(json, "onlyAlertOnce");
    builder.timestampMs = extractInt64(json, "timestampMs");
    builder.style = notificationStyleFromString(extractStr(json, "style"));
    builder.visibility = extractInt(json, "visibility");
    builder.badgeNumber = extractInt(json, "badgeNumber");

    // Parse actions, messages, persons, inboxLines as sub-arrays
    // Actions (simplified — parse individual action objects from actions array)
    {
        auto actPos = json.find("\"actions\"");
        if (actPos != std::string::npos) {
            actPos = json.find('[', actPos);
            if (actPos != std::string::npos) {
                size_t pos = actPos + 1;
                while (pos < json.size() && json[pos] != ']') {
                    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n' || json[pos] == ',')) pos++;
                    if (pos >= json.size() || json[pos] == ']') break;
                    if (json[pos] == '{') {
                        int depth = 1;
                        size_t start = pos;
                        pos++;
                        while (pos < json.size() && depth > 0) {
                            if (json[pos] == '{') depth++;
                            else if (json[pos] == '}') depth--;
                            pos++;
                        }
                        std::string actionJson = json.substr(start, pos - start);
                        builder.actions.push_back(parseNotifAction(actionJson));
                    } else {
                        pos++;
                    }
                }
            }
        }
    }

    // Messages
    {
        auto msgPos = json.find("\"messages\"");
        if (msgPos != std::string::npos) {
            msgPos = json.find('[', msgPos);
            if (msgPos != std::string::npos) {
                size_t pos = msgPos + 1;
                while (pos < json.size() && json[pos] != ']') {
                    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n' || json[pos] == ',')) pos++;
                    if (pos >= json.size() || json[pos] == ']') break;
                    if (json[pos] == '{') {
                        int depth = 1;
                        size_t start = pos;
                        pos++;
                        while (pos < json.size() && depth > 0) {
                            if (json[pos] == '{') depth++;
                            else if (json[pos] == '}') depth--;
                            pos++;
                        }
                        std::string msgJson = json.substr(start, pos - start);
                        builder.messages.push_back(parseNotifMessage(msgJson));
                    } else {
                        pos++;
                    }
                }
            }
        }
    }

    // Persons
    {
        auto perPos = json.find("\"persons\"");
        if (perPos != std::string::npos) {
            perPos = json.find('[', perPos);
            if (perPos != std::string::npos) {
                size_t pos = perPos + 1;
                while (pos < json.size() && json[pos] != ']') {
                    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n' || json[pos] == ',')) pos++;
                    if (pos >= json.size() || json[pos] == ']') break;
                    if (json[pos] == '{') {
                        int depth = 1;
                        size_t start = pos;
                        pos++;
                        while (pos < json.size() && depth > 0) {
                            if (json[pos] == '{') depth++;
                            else if (json[pos] == '}') depth--;
                            pos++;
                        }
                        std::string personJson = json.substr(start, pos - start);
                        builder.persons.push_back(parseNotifPerson(personJson));
                    } else {
                        pos++;
                    }
                }
            }
        }
    }

    // InboxLines (string array)
    builder.inboxLines = extractStrArray(json, "inboxLines");

    return builder;
}

// ============================================================================
// Notification Content Formatter
// ============================================================================
//
// Original Kotlin (NotificationContentFormatter.kt):
//   Formats event content into human-readable notification text.

std::string formatNotificationContent(const std::string& eventType,
                                       const std::string& senderName,
                                       const std::string& body,
                                       const std::string& roomName,
                                       int unreadCount) {
    // Original Kotlin: build notification body based on event type
    if (eventType == "m.room.message") {
        return senderName + ": " + body;
    } else if (eventType == "m.room.member") {
        return senderName + " sent an invitation";
    } else if (eventType == "m.call.invite") {
        return "Incoming call from " + senderName;
    } else if (eventType == "m.call.answer") {
        return senderName + " answered the call";
    } else if (eventType == "m.call.hangup") {
        return "Call ended";
    } else if (eventType == "m.sticker") {
        return senderName + " sent a sticker";
    } else if (eventType == "m.room.tombstone") {
        return roomName + " has been upgraded";
    } else if (eventType == "m.room.encrypted") {
        if (unreadCount > 1) {
            return std::to_string(unreadCount) + " new encrypted messages in " + roomName;
        }
        return senderName + " sent an encrypted message";
    } else {
        // Generic fallback
        if (unreadCount > 1) {
            return std::to_string(unreadCount) + " new events in " + roomName;
        }
        return senderName + ": " + body;
    }
}

std::string formatNotificationSummary(const std::vector<std::string>& senderNames,
                                       const std::string& roomName,
                                       int totalUnread) {
    std::ostringstream out;
    out << totalUnread << " new message";
    if (totalUnread != 1) out << "s";

    if (!senderNames.empty()) {
        out << " from ";
        size_t maxShow = std::min(senderNames.size(), size_t(3));
        for (size_t i = 0; i < maxShow; ++i) {
            if (i > 0) {
                out << (i == maxShow - 1 ? " and " : ", ");
            }
            out << senderNames[i];
        }
        if (senderNames.size() > 3) {
            out << " and " << (senderNames.size() - 3) << " others";
        }
    }

    if (!roomName.empty()) {
        out << " in " << roomName;
    }

    return out.str();
}

std::string formatSystemNotification(const std::string& eventType,
                                      const std::string& roomName) {
    if (eventType == "m.room.create") {
        return roomName.empty() ? "Room created" : roomName + " was created";
    } else if (eventType == "m.room.tombstone") {
        return roomName.empty() ? "Room has been upgraded" : roomName + " has been upgraded";
    } else if (eventType == "m.room.join_rules") {
        return roomName.empty() ? "Room join rules changed" : "Join rules changed in " + roomName;
    } else if (eventType == "m.room.power_levels") {
        return roomName.empty() ? "Power levels changed" : "Power levels changed in " + roomName;
    } else if (eventType == "m.room.name") {
        return roomName.empty() ? "Room name changed" : "Room name changed";
    } else if (eventType == "m.room.topic") {
        return roomName.empty() ? "Room topic changed" : "Room topic changed";
    } else if (eventType == "m.room.avatar") {
        return roomName.empty() ? "Room avatar changed" : "Room avatar changed";
    } else {
        return "System event in " + roomName;
    }
}

std::string formatGroupSummaryTitle(const std::string& appName,
                                     int totalNotifications) {
    std::ostringstream out;
    out << totalNotifications << " new notification";
    if (totalNotifications != 1) out << "s";
    if (!appName.empty()) out << " from " << appName;
    return out.str();
}

std::string formatRoomNotificationTitle(const std::string& roomName,
                                         int unreadCount) {
    if (unreadCount <= 0) return roomName;
    std::ostringstream out;
    out << roomName << " (" << unreadCount << ")";
    return out.str();
}

// ============================================================================
// MXC URL Builder (legacy from original)
// ============================================================================

std::string buildMxcDownloadUrl(const std::string& homeserver, const std::string& mxcUri) {
    auto parsed = parseMxcUri(mxcUri);
    if (!parsed.valid) return "";

    std::string url = homeserver;
    if (!url.empty() && url.back() == '/') url.pop_back();
    url += "/_matrix/media/r0/download/" + parsed.serverName + "/" + parsed.mediaId;
    return url;
}

std::string buildMxcThumbnailUrl(
    const std::string& homeserver,
    const std::string& mxcUri,
    int width, int height,
    const std::string& method)
{
    auto parsed = parseMxcUri(mxcUri);
    if (!parsed.valid) return "";

    std::string url = homeserver;
    if (!url.empty() && url.back() == '/') url.pop_back();
    url += "/_matrix/media/r0/thumbnail/" + parsed.serverName + "/" + parsed.mediaId;
    url += "?width=" + std::to_string(width);
    url += "&height=" + std::to_string(height);
    url += "&method=" + method;
    return url;
}

MxcUri parseMxcUri(const std::string& mxcUri) {
    MxcUri result;
    if (mxcUri.compare(0, 6, "mxc://") != 0) return result;

    size_t serverStart = 6;
    auto slashPos = mxcUri.find('/', serverStart);
    if (slashPos == std::string::npos) return result;

    result.serverName = mxcUri.substr(serverStart, slashPos - serverStart);
    result.mediaId = mxcUri.substr(slashPos + 1);
    result.valid = !result.serverName.empty() && !result.mediaId.empty();
    return result;
}

// ============================================================================
// Notification Formatting (legacy from original)
// ============================================================================

std::string formatTextNotification(
    const std::string& senderName,
    const std::string& body,
    const NotificationFormatConfig& config)
{
    std::string result;
    if (config.showSenderName && !senderName.empty()) {
        result += senderName + ": ";
    }

    if ((int)body.size() <= config.maxBodyLength) {
        result += body;
    } else {
        result += body.substr(0, config.maxBodyLength) + config.truncatedSuffix;
    }

    return result;
}

std::string formatImageNotification(const std::string& senderName) {
    return senderName.empty() ? "sent an image" : senderName + " sent an image";
}

std::string formatFileNotification(const std::string& senderName, const std::string& fileName) {
    std::string msg = senderName.empty() ? "sent a file" : senderName + " sent a file";
    if (!fileName.empty()) msg += ": " + fileName;
    return msg;
}

std::string formatVideoNotification(const std::string& senderName) {
    return senderName.empty() ? "sent a video" : senderName + " sent a video";
}

std::string formatAudioNotification(const std::string& senderName, bool isVoice) {
    std::string base = senderName.empty() ? "sent " : senderName + " sent ";
    return base + (isVoice ? "a voice message" : "an audio file");
}

std::string formatInviteNotification(const std::string& inviterName, const std::string& roomName) {
    return inviterName + " invited you to " + roomName;
}

std::string formatRoomNotification(int messageCount, const std::string& roomName) {
    return std::to_string(messageCount) + " new message" +
           (messageCount != 1 ? "s" : "") + " in " + roomName;
}

std::string formatStickerNotification(const std::string& senderName) {
    return senderName.empty() ? "sent a sticker" : senderName + " sent a sticker";
}

std::string formatLocationNotification(const std::string& senderName) {
    return senderName.empty() ? "shared their location" : senderName + " shared their location";
}

std::string formatPollNotification(const std::string& senderName, bool isStart) {
    return senderName.empty()
        ? (isStart ? "created a poll" : "ended a poll")
        : (senderName + (isStart ? " created a poll" : " ended a poll"));
}

NotificationText buildNotificationText(
    const std::string& msgType,
    const std::string& senderName,
    const std::string& body,
    const std::string& roomName,
    const std::string& fileName,
    const NotificationFormatConfig& config)
{
    NotificationText result;

    result.title = config.showRoomName ? roomName : "";

    if (msgType == "m.text" || msgType == "m.notice" || msgType == "m.emote") {
        result.body = formatTextNotification(senderName, body, config);
    } else if (msgType == "m.image") {
        result.body = formatImageNotification(senderName);
    } else if (msgType == "m.video") {
        result.body = formatVideoNotification(senderName);
    } else if (msgType == "m.audio") {
        result.body = formatAudioNotification(senderName, false);
    } else if (msgType == "m.file") {
        result.body = formatFileNotification(senderName, fileName);
    } else if (msgType == "m.sticker") {
        result.body = formatStickerNotification(senderName);
    } else if (msgType == "m.location") {
        result.body = formatLocationNotification(senderName);
    } else {
        result.body = formatTextNotification(senderName, body, config);
    }

    result.ticker = senderName + ": " + body.substr(0, std::min((int)body.size(), 80));

    return result;
}

} // namespace progressive
