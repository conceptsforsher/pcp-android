#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// ---- Event Type Classifier ----
// Faithful port from original Kotlin:
//   org.matrix.android.sdk.api.session.events.model.EventType.kt (146 lines)
//   org.matrix.android.sdk.api.session.room.model.message.MessageType.kt (52 lines)
//
// Provides ALL Matrix event type constants and classification functions.
// This is the central routing table — every incoming event passes through here.

// ==== Event Type Constants (from EventType.kt) ====
namespace EventTypeStr {
    constexpr const char* MISSING = "org.matrix.android.sdk.missing_type";
    constexpr const char* PRESENCE = "m.presence";
    constexpr const char* MESSAGE = "m.room.message";
    constexpr const char* STICKER = "m.sticker";
    constexpr const char* ENCRYPTED = "m.room.encrypted";
    constexpr const char* TYPING = "m.typing";
    constexpr const char* REDACTION = "m.room.redaction";
    constexpr const char* RECEIPT = "m.receipt";
    constexpr const char* ROOM_KEY = "m.room_key";
    constexpr const char* FORWARDED_ROOM_KEY = "m.forwarded_room_key";
    constexpr const char* ROOM_KEY_REQUEST = "m.room_key_request";

    // Room State
    constexpr const char* STATE_ROOM_NAME = "m.room.name";
    constexpr const char* STATE_ROOM_TOPIC = "m.room.topic";
    constexpr const char* STATE_ROOM_AVATAR = "m.room.avatar";
    constexpr const char* STATE_ROOM_MEMBER = "m.room.member";
    constexpr const char* STATE_ROOM_CREATE = "m.room.create";
    constexpr const char* STATE_ROOM_JOIN_RULES = "m.room.join_rules";
    constexpr const char* STATE_ROOM_GUEST_ACCESS = "m.room.guest_access";
    constexpr const char* STATE_ROOM_POWER_LEVELS = "m.room.power_levels";
    constexpr const char* STATE_ROOM_TOMBSTONE = "m.room.tombstone";
    constexpr const char* STATE_ROOM_CANONICAL_ALIAS = "m.room.canonical_alias";
    constexpr const char* STATE_ROOM_HISTORY_VISIBILITY = "m.room.history_visibility";
    constexpr const char* STATE_ROOM_PINNED_EVENT = "m.room.pinned_events";
    constexpr const char* STATE_ROOM_ENCRYPTION = "m.room.encryption";
    constexpr const char* STATE_ROOM_SERVER_ACL = "m.room.server_acl";
    constexpr const char* STATE_ROOM_THIRD_PARTY_INVITE = "m.room.third_party_invite";
    constexpr const char* STATE_SPACE_CHILD = "m.space.child";
    constexpr const char* STATE_SPACE_PARENT = "m.space.parent";
    constexpr const char* STATE_ROOM_WIDGET = "m.widget";

    // Call
    constexpr const char* CALL_INVITE = "m.call.invite";
    constexpr const char* CALL_CANDIDATES = "m.call.candidates";
    constexpr const char* CALL_ANSWER = "m.call.answer";
    constexpr const char* CALL_HANGUP = "m.call.hangup";
    constexpr const char* CALL_REJECT = "m.call.reject";
    constexpr const char* CALL_NEGOTIATE = "m.call.negotiate";
    constexpr const char* CALL_SELECT_ANSWER = "m.call.select_answer";
    constexpr const char* CALL_REPLACES = "m.call.replaces";

    // Verification
    constexpr const char* KEY_VERIFICATION_REQUEST = "m.key.verification.request";
    constexpr const char* KEY_VERIFICATION_START = "m.key.verification.start";
    constexpr const char* KEY_VERIFICATION_ACCEPT = "m.key.verification.accept";
    constexpr const char* KEY_VERIFICATION_KEY = "m.key.verification.key";
    constexpr const char* KEY_VERIFICATION_MAC = "m.key.verification.mac";
    constexpr const char* KEY_VERIFICATION_CANCEL = "m.key.verification.cancel";
    constexpr const char* KEY_VERIFICATION_DONE = "m.key.verification.done";
    constexpr const char* KEY_VERIFICATION_READY = "m.key.verification.ready";

    // Secrets
    constexpr const char* REQUEST_SECRET = "m.secret.request";
    constexpr const char* SEND_SECRET = "m.secret.send";

    // Relations
    constexpr const char* REACTION = "m.reaction";

    // Polls
    constexpr const char* POLL_START = "m.poll.start";
    constexpr const char* POLL_RESPONSE = "m.poll.response";
    constexpr const char* POLL_END = "m.poll.end";

    // Voice Broadcast
    constexpr const char* STATE_ROOM_VOICE_BROADCAST_INFO = "io.element.voice_broadcast_info";

    // Other
    constexpr const char* DUMMY = "m.dummy";
}

// ==== Message Type Constants (from MessageType.kt) ====
namespace MessageTypeStr {
    constexpr const char* TEXT = "m.text";
    constexpr const char* EMOTE = "m.emote";
    constexpr const char* NOTICE = "m.notice";
    constexpr const char* IMAGE = "m.image";
    constexpr const char* AUDIO = "m.audio";
    constexpr const char* VIDEO = "m.video";
    constexpr const char* LOCATION = "m.location";
    constexpr const char* FILE = "m.file";
    constexpr const char* STICKER_LOCAL = "org.matrix.android.sdk.sticker";
    constexpr const char* POLL_START_LOCAL = "org.matrix.android.sdk.poll.start";
    constexpr const char* POLL_RESPONSE_LOCAL = "org.matrix.android.sdk.poll.response";
    constexpr const char* POLL_END_LOCAL = "org.matrix.android.sdk.poll.end";
    constexpr const char* CONFETTI = "nic.custom.confetti";
    constexpr const char* SNOWFALL = "io.element.effect.snowfall";
}

// ==== EventClass enum ====
//
// Original Kotlin: Event type routing — determines which renderer/bubble to use.
// Derived from EventType.kt classification + TimelineEventController display logic.

enum class EventClass {
    MESSAGE = 0,
    STATE = 1,
    CALL = 2,
    POLL = 3,
    STICKER = 4,
    ENCRYPTED = 5,
    REACTION = 6,
    REDACTION = 7,
    VERIFICATION = 8,
    LOCATION = 9,
    VOICE_BROADCAST = 10,
    WIDGET = 11,
    UNKNOWN = 12
};

const char* eventClassToString(EventClass ec);
EventClass eventClassFromString(const std::string& s);

// ==== EventImportance enum ====
//
// Original Kotlin: NotifPriority / display priority — higher = more prominent in timeline.
// HIGH: direct mentions, room invites, calls
// NORMAL: regular messages
// LOW: reactions, read receipts, typing
// MINIMAL: hidden events, redundant state

enum class EventImportance {
    HIGH = 0,
    NORMAL = 1,
    LOW = 2,
    MINIMAL = 3
};

const char* eventImportanceToString(EventImportance ei);
EventImportance eventImportanceFromString(const std::string& s);

// ==== EventTextType enum ====
//
// Original Kotlin (MessageType.kt:27-36): msgtype → display category
// TEXT: m.text — regular message
// EMOTE: m.emote — /me action
// NOTICE: m.notice — server/system notice
// BOT: m.bot or from bot options — bot message

enum class EventTextType {
    TEXT = 0,
    EMOTE = 1,
    NOTICE = 2,
    BOT = 3
};

const char* eventTextTypeToString(EventTextType ett);
EventTextType eventTextTypeFromString(const std::string& s);

// ==== Classification Functions (from EventType.kt:122-145) ====

// Check if event type is a room state event (m.room.*).
bool isStateEvent(const std::string& eventType);

// Check if event type is a message event (m.room.message).
inline bool isMessageEvent(const std::string& t) { return t == EventTypeStr::MESSAGE; }

// Check if event type is encrypted.
inline bool isEncryptedEvent(const std::string& t) { return t == EventTypeStr::ENCRYPTED; }

// Check if event type is a reaction.
inline bool isReactionEvent(const std::string& t) { return t == EventTypeStr::REACTION; }

// Check if event type is a sticker.
inline bool isStickerEvent(const std::string& t) { return t == EventTypeStr::STICKER; }

// Check if event type is a redaction.
inline bool isRedactionEvent(const std::string& t) { return t == EventTypeStr::REDACTION; }

// Check if event type is a call event.
bool isCallEvent(const std::string& eventType);

// Check if event type is a key verification event.
bool isVerificationEvent(const std::string& eventType);

// Check if event type is a poll event.
bool isPollEvent(const std::string& eventType);

// Check if event type is a membership change.
inline bool isMembershipEvent(const std::string& t) { return t == EventTypeStr::STATE_ROOM_MEMBER; }

// Check if event type is a room tombstone (upgrade).
inline bool isTombstoneEvent(const std::string& t) { return t == EventTypeStr::STATE_ROOM_TOMBSTONE; }

// Check if message type is a media type (needs thumbnail/upload handling).
bool isMediaMessageType(const std::string& msgType);

// Check if message type is a text-based type (text, emote, notice).
bool isTextMessageType(const std::string& msgType);

// Get all known event types.
std::vector<std::string> getAllEventTypes();

// Get all known message types.
std::vector<std::string> getAllMessageTypes();

// Get a human-readable label for an event type.
std::string getEventTypeLabel(const std::string& eventType);

// Get a human-readable label for a message type.
std::string getMessageTypeLabel(const std::string& msgType);

// Determine the processing category for an event (routing to C++ modules).
std::string routeEventForProcessing(const std::string& eventType, const std::string& msgType = "");

// ---- Message Type Detection (from Event.kt:378-509) ----
// Original: fun Event.isTextMessage(): Boolean = when (getMsgType()) { MSGTYPE_TEXT, EMOTE, NOTICE → true }

bool isTextMessage(const std::string& msgType);
bool isImageMessage(const std::string& msgType);
bool isVideoMessage(const std::string& msgType);
bool isAudioMessage(const std::string& msgType);
bool isFileMessage(const std::string& msgType);
bool isLocationMessage(const std::string& msgType);
bool isAttachmentMessage(const std::string& msgType);     // image|audio|video|file

// Check if event supports notifications.
bool supportsNotification(const std::string& eventType);

// Check if event content is reportable.
bool isContentReportable(const std::string& eventType);

// Check if event is an invitation.
bool isInvitationEvent(const std::string& eventType, const std::string& contentJson);

// ---- Relation Types (from RelationType.kt 36L) ----
namespace RelationType {
}

// Check if event is a reply (has m.in_reply_to with event_id).
bool isReplyRelation(const std::string& contentJson);

// Check if event should render in thread (is reply AND isFallingBack==false).
bool shouldRenderInThread(const std::string& contentJson);

// ==== classifyEvent — determine event class from type + content ====
//
// Original Kotlin: EventType.kt routing + TimelineEventController classification
// Routes an event to its display class based on type string and content hints.
EventClass classifyEvent(const std::string& eventType, const std::string& contentJson);

// ==== getEventImportance — compute display priority ====
//
// Original Kotlin: NotifPriority logic + PushRuleEvaluator score mapping
// HIGH: @mentions, calls, invites, room tombstones
// NORMAL: regular messages, media, stickers, polls
// LOW: reactions, receipts, member joins/leaves
// MINIMAL: redundant memberships, m.typing, hidden events
EventImportance getEventImportance(const std::string& eventType, const std::string& contentJson,
    const std::string& myUserId = "", bool isDirectMention = false);

// ==== isRedactedEvent — check if event content indicates redaction ====
//
// Original Kotlin: unsignedData?.redactedEvent != null
// Checks for "redacted_because" in unsigned data JSON or redacted_because event_id.
bool isRedactedEvent(const std::string& contentJson, const std::string& unsignedJson);

// ==== isReplacedEvent — check if event has been edited/replaced ====
//
// Original Kotlin: m.relates_to with rel_type == "m.replace"
// An edit event replaces prior content — check if this event has a substitute.
bool isReplacedEvent(const std::string& contentJson);

// ==== getEventTextType — determine from msgtype in content ====
//
// Original Kotlin (MessageType.kt): msgtype routing
// Extracts msgtype from content JSON and classifies as TEXT/EMOTE/NOTICE/BOT.
EventTextType getEventTextType(const std::string& eventType, const std::string& contentJson);

// ==== shouldDisplayEvent — filter out hidden/irrelevant events ====
//
// Original Kotlin: DefaultTimeline + TimelineEventVisibilityFilter
// Filters out: redundant m.room.member (displayname/avatar-url only changes),
// m.receipt, m.typing, dummy events, non-displayable encrypted events without keys.
bool shouldDisplayEvent(const std::string& eventType, const std::string& contentJson,
    const std::string& prevContentJson = "");

// ==== isDisplayableEvent — check if event should appear in timeline ====
//
// Original Kotlin: TimelineEvent.isDisplayable()
// Returns true if the event has visible content: non-redacted messages,
// state events with meaningful change, reactions, stickers, etc.
bool isDisplayableEvent(const std::string& eventType, const std::string& contentJson,
    bool isRedacted = false, bool isEncrypted = false);

// ==== JSON Serialization ====

std::string eventClassToJson(EventClass ec);
EventClass eventClassFromJson(const std::string& json);
std::string eventImportanceToJson(EventImportance ei);
EventImportance eventImportanceFromJson(const std::string& json);
std::string eventTextTypeToJson(EventTextType ett);
EventTextType eventTextTypeFromJson(const std::string& json);

} // namespace progressive
