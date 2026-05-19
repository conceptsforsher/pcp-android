#include "progressive/event_classifier.hpp"
#include <algorithm>

namespace progressive {

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

} // namespace progressive
