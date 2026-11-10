#pragma once
#include <string>
#include <cstdint>
#include <vector>

namespace pcp {

// ==== Matrix Message Type Constants ====
// Ported from: org.matrix.android.sdk.api.session.room.model.message.MessageType.kt

namespace MessageType {
    constexpr const char* TEXT = "m.text";
    constexpr const char* EMOTE = "m.emote";
    constexpr const char* NOTICE = "m.notice";
    constexpr const char* IMAGE = "m.image";
    constexpr const char* AUDIO = "m.audio";
    constexpr const char* VIDEO = "m.video";
    constexpr const char* LOCATION = "m.location";
    constexpr const char* FILE = "m.file";
    constexpr const char* STICKER = "org.matrix.android.sdk.sticker";
    constexpr const char* POLL_START = "org.matrix.android.sdk.poll.start";
    constexpr const char* POLL_RESPONSE = "org.matrix.android.sdk.poll.response";
    constexpr const char* POLL_END = "org.matrix.android.sdk.poll.end";
    constexpr const char* VERIFICATION_REQUEST = "m.key.verification.request";
    constexpr const char* BEACON_INFO = "org.matrix.android.sdk.beacon.info";
    constexpr const char* BEACON_LOCATION = "org.matrix.android.sdk.beacon.location.data";
    constexpr const char* VOICE_BROADCAST = "io.element.voicebroadcast.info";
}

// ==== Event Type Constants ====
// Ported from: org.matrix.android.sdk.api.session.events.model.EventType.kt

namespace EventType {
    constexpr const char* ROOM_MESSAGE = "m.room.message";
    constexpr const char* ROOM_MEMBER = "m.room.member";
    constexpr const char* ROOM_NAME = "m.room.name";
    constexpr const char* ROOM_TOPIC = "m.room.topic";
    constexpr const char* ROOM_AVATAR = "m.room.avatar";
    constexpr const char* ROOM_CREATE = "m.room.create";
    constexpr const char* ROOM_JOIN_RULES = "m.room.join_rules";
    constexpr const char* ROOM_POWER_LEVELS = "m.room.power_levels";
    constexpr const char* ROOM_TOMBSTONE = "m.room.tombstone";
    constexpr const char* ROOM_ENCRYPTION = "m.room.encryption";
    constexpr const char* ROOM_GUEST_ACCESS = "m.room.guest_access";
    constexpr const char* ROOM_HISTORY_VISIBILITY = "m.room.history_visibility";
    constexpr const char* ROOM_CANONICAL_ALIAS = "m.room.canonical_alias";
    constexpr const char* TYPING = "m.typing";
    constexpr const char* PRESENCE = "m.presence";
    constexpr const char* RECEIPT = "m.receipt";
    constexpr const char* REACTION = "m.reaction";
    constexpr const char* STICKER = "m.sticker";
    constexpr const char* CALL_INVITE = "m.call.invite";
    constexpr const char* CALL_ANSWER = "m.call.answer";
    constexpr const char* CALL_HANGUP = "m.call.hangup";
    constexpr const char* POLL_START = "m.poll.start";
    constexpr const char* POLL_RESPONSE = "m.poll.response";
    constexpr const char* POLL_END = "m.poll.end";
    constexpr const char* VOICE_BROADCAST_START = "io.element.voice_broadcast_start";
    constexpr const char* BEACON_INFO = "org.matrix.msc3488.beacon_info";
    constexpr const char* BEACON_LOCATION = "org.matrix.msc3672.beacon";
}

// ==== Message Content Structure ====
// Ported from: org.matrix.android.sdk.api.session.room.model.message.MessageContent

struct MessageContentInfo {
    std::string msgType;       // "m.text", "m.image", etc.
    std::string body;          // Plain text body
    std::string formattedBody; // HTML formatted body (may be empty)
    std::string format;        // "org.matrix.custom.html"
    std::string url;           // MXC URL for media messages
    std::string filename;      // Original filename for file messages
    std::string infoMimeType;  // MIME type from info block
    int infoWidth = 0;         // Image/video width
    int infoHeight = 0;        // Image/video height
    int infoSize = 0;          // File size in bytes
    int infoDuration = 0;      // Audio/video duration in ms
    std::string thumbnailUrl;  // Thumbnail MXC URL
    std::string geoUri;        // geo: URI for location messages
    std::string relatesToEventId; // Related event ID (replies, edits, reactions)
    std::string relatesToType;    // "m.annotation", "m.replace", "m.reference"
    bool valid = false;
};

// Parse message content from a Matrix event JSON.
// Input: {"content":{"msgtype":"m.text","body":"Hello"}} or the content sub-object
// Original Kotlin: Content.toModel<MessageContent>()
MessageContentInfo parseMessageContent(const std::string& json);

// Extract the msgtype from event JSON content.
// Original Kotlin: content["msgtype"] as? String
std::string extractMsgType(const std::string& json);

// Check if a message type represents media (image, audio, video, file).
// Original Kotlin: when(msgType) { MSGTYPE_IMAGE, MSGTYPE_AUDIO, ... -> true }
bool isMediaMessage(const std::string& msgType);

// Check if a message type is downloadable (has an MXC URL to download).
bool isDownloadableMessage(const std::string& msgType);

// Get human-readable label for a message type.
// Original Kotlin: MessageType human-readable labels used in timeline/notifications
std::string getMessageTypeLabel(const std::string& msgType);

// Check if the event type represents a message that can be displayed in timeline.
// Original Kotlin: EventType used in timeline display
bool isDisplayableEvent(const std::string& eventType);

// Check if the event is a state event (room configuration).
// Original Kotlin: Event.isStateEvent()

// Extract the event_id from event JSON.
std::string extractEventId(const std::string& json);

// Extract the room_id from event JSON.
std::string extractRoomId(const std::string& json);

// Extract the sender (user_id) from event JSON.
std::string extractSenderId(const std::string& json);

// Extract the origin_server_ts (timestamp in ms) from event JSON.
int64_t extractTimestamp(const std::string& json);

// Build a minimal message event JSON.
std::string buildMessageEvent(const std::string& roomId, const std::string& body, 
                               const std::string& msgType = "m.text");

} // namespace pcp
