#include "progressive/event_classifier.hpp"
#include <algorithm>

namespace progressive {

// ==== Enum to String Converters ====

const char* eventClassToString(EventClass ec) {
    switch (ec) {
        case EventClass::MESSAGE:          return "MESSAGE";
        case EventClass::STATE:            return "STATE";
        case EventClass::CALL:             return "CALL";
        case EventClass::POLL:             return "POLL";
        case EventClass::STICKER:          return "STICKER";
        case EventClass::ENCRYPTED:        return "ENCRYPTED";
        case EventClass::REACTION:         return "REACTION";
        case EventClass::REDACTION:        return "REDACTION";
        case EventClass::VERIFICATION:     return "VERIFICATION";
        case EventClass::LOCATION:         return "LOCATION";
        case EventClass::VOICE_BROADCAST:  return "VOICE_BROADCAST";
        case EventClass::WIDGET:           return "WIDGET";
        case EventClass::UNKNOWN:          return "UNKNOWN";
    }
    return "UNKNOWN";
}

EventClass eventClassFromString(const std::string& s) {
    if (s == "MESSAGE")           return EventClass::MESSAGE;
    if (s == "STATE")             return EventClass::STATE;
    if (s == "CALL")              return EventClass::CALL;
    if (s == "POLL")              return EventClass::POLL;
    if (s == "STICKER")           return EventClass::STICKER;
    if (s == "ENCRYPTED")         return EventClass::ENCRYPTED;
    if (s == "REACTION")          return EventClass::REACTION;
    if (s == "REDACTION")         return EventClass::REDACTION;
    if (s == "VERIFICATION")      return EventClass::VERIFICATION;
    if (s == "LOCATION")          return EventClass::LOCATION;
    if (s == "VOICE_BROADCAST")   return EventClass::VOICE_BROADCAST;
    if (s == "WIDGET")            return EventClass::WIDGET;
    return EventClass::UNKNOWN;
}

const char* eventImportanceToString(EventImportance ei) {
    switch (ei) {
        case EventImportance::HIGH:    return "HIGH";
        case EventImportance::NORMAL:  return "NORMAL";
        case EventImportance::LOW:     return "LOW";
        case EventImportance::MINIMAL: return "MINIMAL";
    }
    return "NORMAL";
}

EventImportance eventImportanceFromString(const std::string& s) {
    if (s == "HIGH")    return EventImportance::HIGH;
    if (s == "NORMAL")  return EventImportance::NORMAL;
    if (s == "LOW")     return EventImportance::LOW;
    if (s == "MINIMAL") return EventImportance::MINIMAL;
    return EventImportance::NORMAL;
}

const char* eventTextTypeToString(EventTextType ett) {
    switch (ett) {
        case EventTextType::TEXT:   return "TEXT";
        case EventTextType::EMOTE:  return "EMOTE";
        case EventTextType::NOTICE: return "NOTICE";
        case EventTextType::BOT:    return "BOT";
    }
    return "TEXT";
}

EventTextType eventTextTypeFromString(const std::string& s) {
    if (s == "TEXT")   return EventTextType::TEXT;
    if (s == "EMOTE")  return EventTextType::EMOTE;
    if (s == "NOTICE") return EventTextType::NOTICE;
    if (s == "BOT")    return EventTextType::BOT;
    return EventTextType::TEXT;
}

// ==== Manual JSON Helpers ====

static std::string extractJsonString(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return "";
    pos = json.find(':', pos);
    if (pos == std::string::npos) return "";
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    if (pos >= json.size() || json[pos] != '"') return "";
    pos++;
    size_t end = pos;
    while (end < json.size() && json[end] != '"') {
        if (json[end] == '\\') end++;
        end++;
    }
    return json.substr(pos, end - pos);
}

static int64_t extractJsonInt64(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return 0;
    pos = json.find(':', pos);
    if (pos == std::string::npos) return 0;
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    if (pos >= json.size()) return 0;
    int64_t val = 0;
    while (pos < json.size() && json[pos] >= '0' && json[pos] <= '9') {
        val = val * 10 + (json[pos] - '0');
        pos++;
    }
    return val;
}

static std::string extractJsonObject(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return "";
    pos = json.find(':', pos);
    if (pos == std::string::npos) return "";
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
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

static std::string jsonEscape(const std::string& s) {
    std::string out;
    for (char c : s) {
        if (c == '"') out += "\\\"";
        else if (c == '\\') out += "\\\\";
        else out += c;
    }
    return out;
}

// ==== Classification Functions ====

bool isStateEvent(const std::string& eventType) {
    return eventType == EventTypeStr::STATE_ROOM_NAME ||
           eventType == EventTypeStr::STATE_ROOM_TOPIC ||
           eventType == EventTypeStr::STATE_ROOM_AVATAR ||
           eventType == EventTypeStr::STATE_ROOM_MEMBER ||
           eventType == EventTypeStr::STATE_ROOM_CREATE ||
           eventType == EventTypeStr::STATE_ROOM_JOIN_RULES ||
           eventType == EventTypeStr::STATE_ROOM_GUEST_ACCESS ||
           eventType == EventTypeStr::STATE_ROOM_POWER_LEVELS ||
           eventType == EventTypeStr::STATE_ROOM_TOMBSTONE ||
           eventType == EventTypeStr::STATE_ROOM_CANONICAL_ALIAS ||
           eventType == EventTypeStr::STATE_ROOM_HISTORY_VISIBILITY ||
           eventType == EventTypeStr::STATE_ROOM_PINNED_EVENT ||
           eventType == EventTypeStr::STATE_ROOM_ENCRYPTION ||
           eventType == EventTypeStr::STATE_ROOM_SERVER_ACL ||
           eventType == EventTypeStr::STATE_ROOM_THIRD_PARTY_INVITE ||
           eventType == EventTypeStr::STATE_SPACE_CHILD ||
           eventType == EventTypeStr::STATE_SPACE_PARENT ||
           eventType == EventTypeStr::STATE_ROOM_WIDGET;
}

bool isCallEvent(const std::string& eventType) {
    // Original Kotlin (EventType.kt:122-131):
    //   fun isCallEvent(type: String): Boolean {
    //       return type == CALL_INVITE || type == CALL_CANDIDATES || ...
    //   }
    return eventType == EventTypeStr::CALL_INVITE ||
           eventType == EventTypeStr::CALL_CANDIDATES ||
           eventType == EventTypeStr::CALL_ANSWER ||
           eventType == EventTypeStr::CALL_HANGUP ||
           eventType == EventTypeStr::CALL_REJECT ||
           eventType == EventTypeStr::CALL_NEGOTIATE ||
           eventType == EventTypeStr::CALL_SELECT_ANSWER ||
           eventType == EventTypeStr::CALL_REPLACES;
}

bool isVerificationEvent(const std::string& eventType) {
    // Original Kotlin (EventType.kt:133-145):
    return eventType == EventTypeStr::KEY_VERIFICATION_REQUEST ||
           eventType == EventTypeStr::KEY_VERIFICATION_START ||
           eventType == EventTypeStr::KEY_VERIFICATION_ACCEPT ||
           eventType == EventTypeStr::KEY_VERIFICATION_KEY ||
           eventType == EventTypeStr::KEY_VERIFICATION_MAC ||
           eventType == EventTypeStr::KEY_VERIFICATION_CANCEL ||
           eventType == EventTypeStr::KEY_VERIFICATION_DONE ||
           eventType == EventTypeStr::KEY_VERIFICATION_READY;
}

bool isPollEvent(const std::string& eventType) {
    return eventType == EventTypeStr::POLL_START ||
           eventType == EventTypeStr::POLL_RESPONSE ||
           eventType == EventTypeStr::POLL_END;
}

bool isMediaMessageType(const std::string& msgType) {
    return msgType == MessageTypeStr::IMAGE ||
           msgType == MessageTypeStr::VIDEO ||
           msgType == MessageTypeStr::AUDIO ||
           msgType == MessageTypeStr::FILE;
}

bool isTextMessageType(const std::string& msgType) {
    return msgType == MessageTypeStr::TEXT ||
           msgType == MessageTypeStr::EMOTE ||
           msgType == MessageTypeStr::NOTICE;
}

std::vector<std::string> getAllEventTypes() {
    return {
        EventTypeStr::MESSAGE, EventTypeStr::STICKER, EventTypeStr::ENCRYPTED,
        EventTypeStr::TYPING, EventTypeStr::REDACTION, EventTypeStr::RECEIPT,
        EventTypeStr::REACTION,
        EventTypeStr::STATE_ROOM_NAME, EventTypeStr::STATE_ROOM_TOPIC, EventTypeStr::STATE_ROOM_AVATAR,
        EventTypeStr::STATE_ROOM_MEMBER, EventTypeStr::STATE_ROOM_CREATE,
        EventTypeStr::STATE_ROOM_JOIN_RULES, EventTypeStr::STATE_ROOM_GUEST_ACCESS,
        EventTypeStr::STATE_ROOM_POWER_LEVELS, EventTypeStr::STATE_ROOM_TOMBSTONE,
        EventTypeStr::STATE_ROOM_CANONICAL_ALIAS, EventTypeStr::STATE_ROOM_HISTORY_VISIBILITY,
        EventTypeStr::STATE_ROOM_PINNED_EVENT, EventTypeStr::STATE_ROOM_ENCRYPTION,
        EventTypeStr::STATE_ROOM_SERVER_ACL, EventTypeStr::STATE_SPACE_CHILD, EventTypeStr::STATE_SPACE_PARENT,
        EventTypeStr::CALL_INVITE, EventTypeStr::CALL_CANDIDATES, EventTypeStr::CALL_ANSWER,
        EventTypeStr::CALL_HANGUP, EventTypeStr::CALL_REJECT,
        EventTypeStr::KEY_VERIFICATION_REQUEST, EventTypeStr::KEY_VERIFICATION_START,
        EventTypeStr::KEY_VERIFICATION_ACCEPT, EventTypeStr::KEY_VERIFICATION_KEY,
        EventTypeStr::KEY_VERIFICATION_MAC, EventTypeStr::KEY_VERIFICATION_CANCEL,
        EventTypeStr::KEY_VERIFICATION_DONE, EventTypeStr::KEY_VERIFICATION_READY,
        EventTypeStr::POLL_START, EventTypeStr::POLL_RESPONSE, EventTypeStr::POLL_END,
    };
}

std::vector<std::string> getAllMessageTypes() {
    return {
        MessageTypeStr::TEXT, MessageTypeStr::EMOTE, MessageTypeStr::NOTICE,
        MessageTypeStr::IMAGE, MessageTypeStr::AUDIO, MessageTypeStr::VIDEO,
        MessageTypeStr::LOCATION, MessageTypeStr::FILE,
    };
}

std::string getEventTypeLabel(const std::string& eventType) {
    if (eventType == EventTypeStr::MESSAGE) return "Message";
    if (eventType == EventTypeStr::STICKER) return "Sticker";
    if (eventType == EventTypeStr::ENCRYPTED) return "Encrypted";
    if (eventType == EventTypeStr::TYPING) return "Typing";
    if (eventType == EventTypeStr::REDACTION) return "Redaction";
    if (eventType == EventTypeStr::RECEIPT) return "Read Receipt";
    if (eventType == EventTypeStr::REACTION) return "Reaction";
    if (eventType == EventTypeStr::STATE_ROOM_NAME) return "Room Name Change";
    if (eventType == EventTypeStr::STATE_ROOM_TOPIC) return "Topic Change";
    if (eventType == EventTypeStr::STATE_ROOM_AVATAR) return "Avatar Change";
    if (eventType == EventTypeStr::STATE_ROOM_MEMBER) return "Membership";
    if (eventType == EventTypeStr::STATE_ROOM_CREATE) return "Room Created";
    if (eventType == EventTypeStr::STATE_ROOM_JOIN_RULES) return "Join Rules";
    if (eventType == EventTypeStr::STATE_ROOM_POWER_LEVELS) return "Power Levels";
    if (eventType == EventTypeStr::STATE_ROOM_TOMBSTONE) return "Room Upgrade";
    if (eventType == EventTypeStr::STATE_ROOM_ENCRYPTION) return "Encryption";
    if (eventType == EventTypeStr::CALL_INVITE) return "Call Invite";
    if (eventType == EventTypeStr::CALL_HANGUP) return "Call Ended";
    if (isVerificationEvent(eventType)) return "Verification";
    if (isPollEvent(eventType)) return "Poll";
    return eventType;
}

std::string getMessageTypeLabel(const std::string& msgType) {
    if (msgType == MessageTypeStr::TEXT) return "Text";
    if (msgType == MessageTypeStr::EMOTE) return "Action";
    if (msgType == MessageTypeStr::NOTICE) return "Notice";
    if (msgType == MessageTypeStr::IMAGE) return "Image";
    if (msgType == MessageTypeStr::AUDIO) return "Audio";
    if (msgType == MessageTypeStr::VIDEO) return "Video";
    if (msgType == MessageTypeStr::LOCATION) return "Location";
    if (msgType == MessageTypeStr::FILE) return "File";
    if (msgType == MessageTypeStr::STICKER_LOCAL) return "Sticker";
    if (msgType == MessageTypeStr::CONFETTI) return "Confetti";
    if (msgType == MessageTypeStr::SNOWFALL) return "Snowfall";
    return msgType;
}

std::string routeEventForProcessing(const std::string& eventType, const std::string& msgType) {
    // Routing logic — determines which C++ module processes this event
    if (eventType == EventTypeStr::MESSAGE || eventType == EventTypeStr::STICKER ||
        eventType == EventTypeStr::ENCRYPTED) {
        if (isTextMessageType(msgType)) return "message_text";
        if (isMediaMessageType(msgType)) return "message_media";
        if (msgType == MessageTypeStr::LOCATION) return "message_location";
        if (msgType == MessageTypeStr::CONFETTI || msgType == MessageTypeStr::SNOWFALL) return "message_effect";
        return "message";
    }
    if (isStateEvent(eventType)) return "state";
    if (isCallEvent(eventType)) return "call";
    if (isVerificationEvent(eventType)) return "verification";
    if (eventType == EventTypeStr::REACTION) return "reaction";
    if (isPollEvent(eventType)) return "poll";
    if (eventType == EventTypeStr::REDACTION) return "redaction";
    if (eventType == EventTypeStr::RECEIPT) return "receipt";
    if (eventType == EventTypeStr::TYPING) return "typing";
    return "unknown";
}

// ==== Message Type Detection (from Event.kt:383-450) ====
// Original: fun Event.isTextMessage(): Boolean { return when (getMsgType()) { ... } }

bool isTextMessage(const std::string& msgType) {
    // Original: MSGTYPE_TEXT, MSGTYPE_EMOTE, MSGTYPE_NOTICE → true
    return msgType == MessageTypeStr::TEXT ||
           msgType == MessageTypeStr::EMOTE ||
           msgType == MessageTypeStr::NOTICE;
}

bool isImageMessage(const std::string& msgType) { return msgType == MessageTypeStr::IMAGE; }
bool isVideoMessage(const std::string& msgType) { return msgType == MessageTypeStr::VIDEO; }
bool isAudioMessage(const std::string& msgType) { return msgType == MessageTypeStr::AUDIO; }
bool isFileMessage(const std::string& msgType) { return msgType == MessageTypeStr::FILE; }
bool isLocationMessage(const std::string& msgType) { return msgType == MessageTypeStr::LOCATION; }

bool isAttachmentMessage(const std::string& msgType) {
    // Original: MSGTYPE_IMAGE, AUDIO, VIDEO, FILE → true
    return isImageMessage(msgType) || isAudioMessage(msgType) ||
           isVideoMessage(msgType) || isFileMessage(msgType);
}

bool supportsNotification(const std::string& eventType) {
    // Original: getClearType() in MESSAGE + POLL_START + POLL_END + BEACON_INFO + CALL_NOTIFY
    return eventType == EventTypeStr::MESSAGE ||
           isPollEvent(eventType) ||
           eventType == EventTypeStr::CALL_INVITE;
}

bool isContentReportable(const std::string& eventType) {
    return eventType == EventTypeStr::MESSAGE;
}

bool isInvitationEvent(const std::string& eventType, const std::string& contentJson) {
    // Original: type == STATE_ROOM_MEMBER && membership == INVITE
    if (eventType != EventTypeStr::STATE_ROOM_MEMBER) return false;
    return contentJson.find("\"membership\":\"invite\"") != std::string::npos ||
           contentJson.find("\"membership\": \"invite\"") != std::string::npos;
}

// ==== Relation Types (from RelationType.kt + RelationDefaultContent.kt) ====
bool isReplyRelation(const std::string& contentJson) {
    // Original: this?.inReplyTo?.eventId != null
    auto replyPos = contentJson.find("\"m.in_reply_to\"");
    if (replyPos == std::string::npos) return false;
    return contentJson.find("\"event_id\":\"", replyPos) != std::string::npos ||
           contentJson.find("\"event_id\": \"", replyPos) != std::string::npos;
}

bool shouldRenderInThread(const std::string& contentJson) {
    // Original: isFallingBack == false
    if (!isReplyRelation(contentJson)) return false;
    return contentJson.find("\"is_falling_back\": true") == std::string::npos &&
           contentJson.find("\"is_falling_back\":true") == std::string::npos;
}

// ==== classifyEvent — determine event class from type + content ====
//
// Original Kotlin: EventType.kt routing + TimelineEventController classification
// Routes an event to its display class based on type string and content hints.

EventClass classifyEvent(const std::string& eventType, const std::string& contentJson) {
    // Check redaction first — special handling
    if (isRedactionEvent(eventType)) return EventClass::REDACTION;

    // Check encrypted
    if (isEncryptedEvent(eventType)) return EventClass::ENCRYPTED;

    // Message-based events
    if (eventType == EventTypeStr::MESSAGE) {
        auto msgType = extractJsonString(contentJson, "msgtype");
        if (msgType == MessageTypeStr::LOCATION) return EventClass::LOCATION;
        return EventClass::MESSAGE;
    }

    // Sticker
    if (isStickerEvent(eventType)) return EventClass::STICKER;

    // Reaction
    if (isReactionEvent(eventType)) return EventClass::REACTION;

    // Poll
    if (isPollEvent(eventType)) return EventClass::POLL;

    // Call
    if (isCallEvent(eventType)) return EventClass::CALL;

    // Verification
    if (isVerificationEvent(eventType)) return EventClass::VERIFICATION;

    // State events
    if (isStateEvent(eventType)) {
        // Voice broadcast info is technically a state event
        if (eventType == EventTypeStr::STATE_ROOM_VOICE_BROADCAST_INFO)
            return EventClass::VOICE_BROADCAST;
        // Widget state
        if (eventType == EventTypeStr::STATE_ROOM_WIDGET)
            return EventClass::WIDGET;
        return EventClass::STATE;
    }

    // Check content for location sharing (m.beacon / m.beacon_info)
    if (eventType == "m.beacon" || eventType == "m.beacon_info")
        return EventClass::LOCATION;

    return EventClass::UNKNOWN;
}

// ==== getEventImportance — compute display priority ====
//
// Original Kotlin: NotifPriority logic + PushRuleEvaluator score mapping
// HIGH: @mentions, calls, invites, room tombstones
// NORMAL: regular messages, media, stickers, polls
// LOW: reactions, receipts, member joins/leaves
// MINIMAL: redundant memberships, m.typing, hidden events

EventImportance getEventImportance(const std::string& eventType, const std::string& contentJson,
    const std::string& myUserId, bool isDirectMention)
{
    // HIGH priority: direct mentions, calls, invites
    if (isDirectMention) return EventImportance::HIGH;
    if (isCallEvent(eventType)) return EventImportance::HIGH;
    if (eventType == EventTypeStr::STATE_ROOM_TOMBSTONE) return EventImportance::HIGH;

    // Check for membership invite
    if (eventType == EventTypeStr::STATE_ROOM_MEMBER) {
        auto membership = extractJsonString(contentJson, "membership");
        if (membership == "invite") return EventImportance::HIGH;
    }

    // NORMAL: regular messages, media, stickers, polls, state changes
    if (eventType == EventTypeStr::MESSAGE) return EventImportance::NORMAL;
    if (isPollEvent(eventType)) return EventImportance::NORMAL;
    if (isStickerEvent(eventType)) return EventImportance::NORMAL;
    if (isEncryptedEvent(eventType)) return EventImportance::NORMAL;

    // LOW: reactions, verification, join/leave memberships, widget
    if (isReactionEvent(eventType)) return EventImportance::LOW;
    if (isVerificationEvent(eventType)) return EventImportance::LOW;
    if (eventType == EventTypeStr::STATE_ROOM_MEMBER) return EventImportance::LOW;
    if (eventType == EventTypeStr::STATE_ROOM_WIDGET) return EventImportance::LOW;
    if (isStateEvent(eventType)) return EventImportance::LOW;

    // MINIMAL: typing, receipts, dummies, redundant membership updates
    if (eventType == EventTypeStr::TYPING) return EventImportance::MINIMAL;
    if (eventType == EventTypeStr::RECEIPT) return EventImportance::MINIMAL;
    if (eventType == EventTypeStr::DUMMY) return EventImportance::MINIMAL;

    return EventImportance::NORMAL;
}

// ==== isRedactedEvent — check if event content indicates redaction ====
//
// Original Kotlin: unsignedData?.redactedEvent != null

bool isRedactedEvent(const std::string& contentJson, const std::string& unsignedJson) {
    // Check unsigned data for redacted_because
    if (!unsignedJson.empty()) {
        if (unsignedJson.find("\"redacted_because\"") != std::string::npos) return true;
    }
    // Check content directly
    if (!contentJson.empty()) {
        if (contentJson.find("\"redacted_because\"") != std::string::npos) return true;
    }
    return false;
}

// ==== isReplacedEvent — check if event has been edited/replaced ====
//
// Original Kotlin: m.relates_to with rel_type == "m.replace"

bool isReplacedEvent(const std::string& contentJson) {
    if (contentJson.empty()) return false;

    // Check for m.relates_to with rel_type m.replace
    auto relPos = contentJson.find("\"m.relates_to\"");
    if (relPos == std::string::npos) return false;

    auto relObj = extractJsonObject(contentJson, "m.relates_to");
    if (relObj.empty()) return false;

    auto relType = extractJsonString(relObj, "rel_type");
    return relType == "m.replace";
}

// ==== getEventTextType — determine from msgtype in content ====
//
// Original Kotlin (MessageType.kt): msgtype routing

EventTextType getEventTextType(const std::string& eventType, const std::string& contentJson) {
    // Non-message events: return TEXT as default
    if (eventType != EventTypeStr::MESSAGE && !isEncryptedEvent(eventType))
        return EventTextType::TEXT;

    auto msgType = extractJsonString(contentJson, "msgtype");
    if (msgType == MessageTypeStr::EMOTE) return EventTextType::EMOTE;
    if (msgType == MessageTypeStr::NOTICE) return EventTextType::NOTICE;
    if (msgType == MessageTypeStr::TEXT && contentJson.find("\"m.relates_to\"") != std::string::npos) {
        // Could be a bot if rel_type is specific
        auto relObj = extractJsonObject(contentJson, "m.relates_to");
        auto relType = extractJsonString(relObj, "rel_type");
        if (relType == "m.bot" || relType == "org.matrix.bot")
            return EventTextType::BOT;
    }
    return EventTextType::TEXT;
}

// ==== shouldDisplayEvent — filter out hidden/irrelevant events ====
//
// Original Kotlin: DefaultTimeline + TimelineEventVisibilityFilter

bool shouldDisplayEvent(const std::string& eventType, const std::string& contentJson,
    const std::string& prevContentJson)
{
    // Always hide these event types from timeline
    if (eventType == EventTypeStr::TYPING) return false;
    if (eventType == EventTypeStr::RECEIPT) return false;
    if (eventType == EventTypeStr::DUMMY) return false;
    if (eventType == EventTypeStr::PRESENCE) return false;

    // m.room.member: hide redundant displayname/avatar-url only changes
    if (eventType == EventTypeStr::STATE_ROOM_MEMBER) {
        if (contentJson.empty()) return true; // can't determine, show it

        auto membership = extractJsonString(contentJson, "membership");
        // If membership is unchanged AND only displayname/avatar changed, hide
        if (!prevContentJson.empty()) {
            auto prevMembership = extractJsonString(prevContentJson, "membership");
            bool hasDisplayName = contentJson.find("\"displayname\"") != std::string::npos;
            bool prevHasDisplayName = prevContentJson.find("\"displayname\"") != std::string::npos;
            bool hasAvatar = contentJson.find("\"avatar_url\"") != std::string::npos;
            bool prevHasAvatar = prevContentJson.find("\"avatar_url\"") != std::string::npos;

            bool sameMembership = (membership == prevMembership || (membership.empty() && prevMembership.empty()));

            // Only avatar/displayname changed and no membership change
            if (sameMembership &&
                ((hasDisplayName != prevHasDisplayName) || (hasAvatar != prevHasAvatar)) &&
                membership.empty()) {
                return false; // redundant profile-only update
            }
        }

        // Show if there's a meaningful membership change
        if (!membership.empty()) return true;

        // Show if there's displayname/avatar content without prev content
        if (contentJson.find("\"displayname\"") != std::string::npos ||
            contentJson.find("\"avatar_url\"") != std::string::npos) {
            return true;
        }

        return false;
    }

    // Room key / forwarded room key — internal E2EE, not for display
    if (eventType == EventTypeStr::ROOM_KEY) return false;
    if (eventType == EventTypeStr::FORWARDED_ROOM_KEY) return false;
    if (eventType == EventTypeStr::ROOM_KEY_REQUEST) return false;

    // Secret request/send — internal, not for display
    if (eventType == EventTypeStr::REQUEST_SECRET) return false;
    if (eventType == EventTypeStr::SEND_SECRET) return false;

    // Default: display everything else
    return true;
}

// ==== isDisplayableEvent — check if event should appear in timeline ====
//
// Original Kotlin: TimelineEvent.isDisplayable()

bool isDisplayableEvent(const std::string& eventType, const std::string& contentJson,
    bool isRedacted, bool isEncrypted)
{
    // Redacted events are still displayable (show "Message removed")
    if (isRedacted) return true;

    // Encrypted events are displayable (show "encrypted message")
    if (isEncrypted) {
        // But not if content is empty (no decryption key)
        if (contentJson.empty() || contentJson == "{}") return false;
        return true;
    }

    // Check content has visible body
    if (eventType == EventTypeStr::MESSAGE) {
        auto body = extractJsonString(contentJson, "body");
        if (!body.empty()) return true;
        // Media messages might have url without body, still displayable
        if (contentJson.find("\"url\"") != std::string::npos) return true;
        return false;
    }

    // State events: always displayable if shouldDisplayEvent passed
    if (isStateEvent(eventType)) return true;

    // Sticker: always displayable
    if (isStickerEvent(eventType)) return true;

    // Reaction: always displayable
    if (isReactionEvent(eventType)) return true;

    // Poll: always displayable
    if (isPollEvent(eventType)) return true;

    // Call: always displayable
    if (isCallEvent(eventType)) return true;

    // Verification: always displayable
    if (isVerificationEvent(eventType)) return true;

    // Redaction: displayable (shows removal notice)
    if (isRedactionEvent(eventType)) return true;

    // Widget: displayable
    if (eventType == EventTypeStr::STATE_ROOM_WIDGET) return true;

    return false;
}

// ==== JSON Serialization ====

std::string eventClassToJson(EventClass ec) {
    return "\"" + std::string(eventClassToString(ec)) + "\"";
}

EventClass eventClassFromJson(const std::string& json) {
    // Expects "\"CLASS_NAME\"" or just "CLASS_NAME"
    std::string s = json;
    if (!s.empty() && s.front() == '"') s = s.substr(1);
    if (!s.empty() && s.back() == '"') s.pop_back();
    return eventClassFromString(s);
}

std::string eventImportanceToJson(EventImportance ei) {
    return "\"" + std::string(eventImportanceToString(ei)) + "\"";
}

EventImportance eventImportanceFromJson(const std::string& json) {
    std::string s = json;
    if (!s.empty() && s.front() == '"') s = s.substr(1);
    if (!s.empty() && s.back() == '"') s.pop_back();
    return eventImportanceFromString(s);
}

std::string eventTextTypeToJson(EventTextType ett) {
    return "\"" + std::string(eventTextTypeToString(ett)) + "\"";
}

EventTextType eventTextTypeFromJson(const std::string& json) {
    std::string s = json;
    if (!s.empty() && s.front() == '"') s = s.substr(1);
    if (!s.empty() && s.back() == '"') s.pop_back();
    return eventTextTypeFromString(s);
}

} // namespace progressive
