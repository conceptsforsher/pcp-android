#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// ============================================================================
// NotificationStyle — determines the expanded notification layout
// ============================================================================
//
// Original Kotlin (NotificationStyle.kt):
//   enum class NotificationStyle { INBOX, BIG_TEXT, BIG_PICTURE, MESSAGING, MEDIA }

enum class NotificationStyle {
    INBOX,        // Multiple summary lines
    BIG_TEXT,     // Expanded text preview
    BIG_PICTURE,  // Large image preview
    MESSAGING,    // Android 7+ MessagingStyle with per-message bubbles
    MEDIA         // Media playback controls
};

std::string notificationStyleToString(NotificationStyle style);
NotificationStyle notificationStyleFromString(const std::string& str);

// ============================================================================
// NotifAction — notification action button
// ============================================================================
//
// Original Kotlin (NotifAction.kt):
//   data class NotifAction(
//       val actionId: String,
//       val title: String,
//       val icon: Int,
//       val intent: Intent?,
//       val isAuthenticationRequired: Boolean
//   )

struct NotifAction {
    std::string actionId;                    // "REPLY", "MARK_READ", "QUICK_REPLY"
    std::string title;                       // "Reply", "Mark as Read"
    std::string icon;                        // Icon resource name (e.g. "ic_reply_24dp")
    std::string intentUri;                   // Intent URI for Android PendingIntent
    std::string intentAction;                // Action string for the intent
    std::string intentData;                  // Data URI for the intent
    bool isAuthenticationRequired = false;   // Require device unlock
    bool isQuickReply = false;               // Opens inline reply input

    bool isValid() const { return !actionId.empty() && !title.empty(); }
};

// Convert NotifAction to/from JSON.
std::string notifActionToJson(const NotifAction& action);
NotifAction parseNotifAction(const std::string& json);

// ============================================================================
// NotifPerson — sender information for MessagingStyle
// ============================================================================
//
// Original Kotlin (NotifPerson.kt / Person class in Android):
//   data class NotifPerson(
//       val name: String,
//       val key: String,
//       val uri: String?,
//       val isBot: Boolean,
//       val isImportant: Boolean
//   )

struct NotifPerson {
    std::string name;            // Display name
    std::string key;             // Unique key (MXID)
    std::string uri;             // Avatar URI or content URI
    bool isBot = false;
    bool isImportant = false;

    bool isValid() const { return !name.empty() && !key.empty(); }
};

std::string notifPersonToJson(const NotifPerson& person);
NotifPerson parseNotifPerson(const std::string& json);

// ============================================================================
// NotifMessage — individual message within MessagingStyle
// ============================================================================
//
// Original Kotlin (NotifMessage.kt):
//   data class NotifMessage(
//       val text: String,
//       val timestampMs: Long,
//       val sender: NotifPerson?,
//       val dataMimeType: String?,
//       val dataUri: String?
//   )

struct NotifMessage {
    std::string text;                  // Message body text
    int64_t timestampMs = 0;          // UTC timestamp in milliseconds
    NotifPerson sender;               // Who sent this message
    std::string dataMimeType;         // MIME type for data URI (e.g. "image/jpeg")
    std::string dataUri;              // Data URI for large icon/attachment

    bool hasAttachment() const { return !dataMimeType.empty() && !dataUri.empty(); }
};

std::string notifMessageToJson(const NotifMessage& message);
NotifMessage parseNotifMessage(const std::string& json);

// ============================================================================
// NotificationBuilder — full notification specification
// ============================================================================
//
// Original Kotlin (NotificationBuilder.kt):
//   data class NotificationBuilder(
//       val channelId: String,
//       val title: String,
//       val contentText: String,
//       val subText: String?,
//       val smallIcon: Int,
//       val largeIconUri: String?,
//       val color: Int,
//       val priority: Int,
//       val category: String?,
//       val groupKey: String?,
//       val groupSummary: Boolean,
//       val autoCancel: Boolean,
//       val actions: List<NotifAction>,
//       val style: NotificationStyle?,
//       val messages: List<NotifMessage>,
//       val persons: List<NotifPerson>,
//       val bigPictureUri: String?,
//       val bigText: String?,
//       val inboxLines: List<String>,
//       val visibility: Int,
//       val timestampMs: Long
//   )

struct NotificationBuilder {
    // Channel & identity
    std::string channelId;
    std::string groupKey;          // Group notifications by room
    bool groupSummary = false;

    // Content
    std::string title;             // Primary title (room name)
    std::string contentText;       // Body text
    std::string subText;           // Secondary text (sender name)
    std::string bigText;           // Expanded text content
    std::string ticker;            // Status bar ticker text
    std::string summaryText;       // Group summary text

    // Visual
    std::string smallIcon;         // Icon resource name
    std::string largeIconUri;      // Avatar or content URI for large icon
    int color = 0;                 // ARGB accent color
    std::string bigPictureUri;     // Hero image for big picture style

    // Behavior
    int priority = 0;              // Android notification priority
    std::string category;          // Notification.CATEGORY_*
    bool autoCancel = true;
    bool ongoing = false;          // Cannot be dismissed
    bool onlyAlertOnce = false;
    int64_t timestampMs = 0;      // When the event occurred

    // Style
    NotificationStyle style = NotificationStyle::BIG_TEXT;

    // Content lists
    std::vector<NotifAction> actions;
    std::vector<NotifMessage> messages;
    std::vector<NotifPerson> persons;
    std::vector<std::string> inboxLines;

    // Visibility
    int visibility = 0;            // Notification.VISIBILITY_*

    // Badge
    int badgeNumber = 0;           // Badge count on app icon

    bool hasActions() const { return !actions.empty(); }
    bool hasMessages() const { return !messages.empty(); }
    bool hasPersons() const { return !persons.empty(); }
    bool hasInboxLines() const { return !inboxLines.empty(); }
};

// Build the full notification JSON for JNI bridge.
std::string buildNotificationJson(const NotificationBuilder& builder);

// Parse a NotificationBuilder from JSON.
NotificationBuilder parseNotificationBuilder(const std::string& json);

// ============================================================================
// Notification Content Formatter
// ============================================================================
//
// Original Kotlin (NotificationContentFormatter.kt):
//   Formats event content into human-readable notification text.

// Format notification content from event type and body.
// Returns a descriptive string for different event types.
std::string formatNotificationContent(const std::string& eventType,
                                       const std::string& senderName,
                                       const std::string& body,
                                       const std::string& roomName,
                                       int unreadCount = 1);

// Format a summary notification for multiple messages.
// "5 new messages from Alice, Bob, and 3 others in Room Name"
std::string formatNotificationSummary(const std::vector<std::string>& senderNames,
                                       const std::string& roomName,
                                       int totalUnread);

// Format a "content-less" notification (e.g. call ended, room created).
std::string formatSystemNotification(const std::string& eventType,
                                      const std::string& roomName);

// Format notification title for group summary.
std::string formatGroupSummaryTitle(const std::string& appName,
                                     int totalNotifications);

// Format notification title for single room.
std::string formatRoomNotificationTitle(const std::string& roomName,
                                         int unreadCount);

// ============================================================================
// MXC URL Builder & Thumbnail (legacy from original)
// ============================================================================
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

// ============================================================================
// Notification Text Formatter (legacy from original)
// ============================================================================
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
