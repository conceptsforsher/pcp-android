#pragma once

#include <string>
#include <cstdint>

namespace progressive {

// ==== MXC URL Builder & Thumbnail ====
//
// Matrix Content (MXC) URLs specify media on a Matrix homeserver.
// Format: mxc://<server-name>/<media-id>
//
// The client constructs HTTP download URLs from MXC URIs.
// Thumbnails are generated server-side with resize parameters.

// Build the download URL for an MXC URI.
// homeserver: "https://matrix.example.org"
// mxcUri: "mxc://matrix.example.org/abc123"
// Returns: "https://matrix.example.org/_matrix/media/r0/download/matrix.example.org/abc123"
std::string buildMxcDownloadUrl(const std::string& homeserver, const std::string& mxcUri);

// Build the thumbnail URL with resize parameters.
// width/height: desired dimensions (0 = auto-scale)
// method: "crop" or "scale"
std::string buildMxcThumbnailUrl(
    const std::string& homeserver,
    const std::string& mxcUri,
    int width, int height,
    const std::string& method = "scale"
);

// Parse an MXC URI into server name and media ID.
// "mxc://matrix.example.org/abc123" → server="matrix.example.org", mediaId="abc123"
struct MxcUri {
    std::string serverName;
    std::string mediaId;
    bool valid = false;
};
MxcUri parseMxcUri(const std::string& mxcUri);

// ==== Notification Text Formatter ====
//
// Builds human-readable notification text from Matrix events.
// Used for push notification content and room list previews.

// Notification format configuration
struct NotificationFormatConfig {
    int maxBodyLength = 120;         // Truncate message body
    bool showSenderName = true;      // Include sender name in notification
    bool showRoomName = true;        // Include room name
    std::string truncatedSuffix = "...";
};

// Format a text message for notification display.
// Example output: "Alice: Hello, how are you doing..."
std::string formatTextNotification(
    const std::string& senderName,
    const std::string& body,
    const NotificationFormatConfig& config = {}
);

// Format an image notification.
// Example output: "Alice sent an image"
std::string formatImageNotification(const std::string& senderName);

// Format a file notification.
// Example output: "Alice sent a file: document.pdf"
std::string formatFileNotification(const std::string& senderName,
                                   const std::string& fileName = "");

// Format a video notification.
std::string formatVideoNotification(const std::string& senderName);

// Format an audio/voice message notification.
std::string formatAudioNotification(const std::string& senderName, bool isVoice = false);

// Format an invitation notification.
// Example output: "Alice invited you to My Room"
std::string formatInviteNotification(const std::string& inviterName,
                                     const std::string& roomName);

// Format a room-level notification (multiple messages).
// Example output: "3 new messages in My Room"
std::string formatRoomNotification(int messageCount, const std::string& roomName);

// Format a sticker notification.
std::string formatStickerNotification(const std::string& senderName);

// Format a location notification.
std::string formatLocationNotification(const std::string& senderName);

// Format a poll notification.
std::string formatPollNotification(const std::string& senderName, bool isStart = true);

// Build the full notification title + body.
struct NotificationText {
    std::string title;     // Room name + optional sender summary
    std::string body;      // Message preview
    std::string ticker;    // Short version for status bar
};

// Build complete notification text for a single event.
NotificationText buildNotificationText(
    const std::string& msgType,          // "m.text", "m.image", etc.
    const std::string& senderName,
    const std::string& body,
    const std::string& roomName,
    const std::string& fileName = "",
    const NotificationFormatConfig& config = {}
);

} // namespace progressive
