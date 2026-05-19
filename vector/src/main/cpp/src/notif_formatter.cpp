#include "progressive/notif_formatter.hpp"
#include <algorithm>

namespace progressive {

// ==== MXC URL Builder ====

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

// ==== Notification Formatting ====

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

// ==== Build Full Notification ====

NotificationText buildNotificationText(
    const std::string& msgType,
    const std::string& senderName,
    const std::string& body,
    const std::string& roomName,
    const std::string& fileName,
    const NotificationFormatConfig& config)
{
    NotificationText result;

    // Title: room name
    result.title = config.showRoomName ? roomName : "";

    // Body: message-type-specific preview
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

    // Ticker: short version for status bar
    result.ticker = senderName + ": " + body.substr(0, std::min((int)body.size(), 80));

    return result;
}

} // namespace progressive
