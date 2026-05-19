#ifndef PROGRESSIVE_EVENT_DISPLAY_HPP
#define PROGRESSIVE_EVENT_DISPLAY_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// ---- Event Display Utilities ----

enum class DisplayEventType {
    Message,       // regular text message
    Emote,         // /me message
    Notice,        // server notice
    Image,         // image attachment
    Video,         // video attachment
    Audio,         // audio file
    File,          // generic file
    Sticker,       // sticker
    Location,      // location share
    Poll,          // poll
    Reaction,      // reaction
    MemberEvent,   // join/leave/invite
    RoomName,      // room name change
    RoomTopic,     // topic change
    RoomAvatar,    // avatar change
    Encryption,    // encryption enabled/disabled
    Call,          // VoIP call
    Widget,        // widget added/removed
    Redaction,     // message redacted
    Unknown
};

struct DisplayEvent {
    std::string eventId;
    std::string senderName;
    std::string senderId;
    DisplayEventType type = DisplayEventType::Unknown;
    std::string body;               // text to display
    std::string formattedBody;      // HTML if available
    std::string timestamp;          // formatted time
    int64_t originServerTs = 0;
    bool isEncrypted = false;
    bool isRedacted = false;
    bool isContinuation = false;    // same sender as previous
    bool isFromMe = false;
    bool showTimestamp = true;
    bool showAvatar = true;
    bool showSender = true;
    std::string thumbnailUrl;       // for images/video
    std::string mxcUrl;
    int imgWidth = 0;
    int imgHeight = 0;
};

struct ContinuationState {
    std::string previousSenderId;
    int64_t previousTimestampMs = 0;
    int eventsInGroup = 0;
};

// Classify event type from Matrix event type string.
DisplayEventType classifyEvent(const std::string& eventType, const std::string& msgType);

// Compute continuation: should this event merge with previous?
bool isContinuation(const std::string& currentSender, const std::string& previousSender,
    int64_t currentTs, int64_t previousTs, int64_t mergeWindowMs = 300000);

// Compute whether to show timestamp for this event.
bool shouldShowTimestamp(const std::string& currentSender, int64_t currentTs,
    int64_t previousTs, bool showAll = false);

// Compute whether to show avatar for this event.
bool shouldShowAvatar(const std::string& currentSender, const std::string& previousSender,
    bool isLastInGroup);

// Get a human-readable event type description.
std::string getEventTypeDescription(DisplayEventType type);

// Get an event type icon emoji: 💬, 📷, 🎥, etc.
std::string getEventTypeIcon(DisplayEventType type);

// Format event body for room list preview.
std::string formatEventPreview(const DisplayEvent& event, bool showSender);

// Get the default display name for a member event.
std::string formatMemberEvent(const std::string& senderName, const std::string& membership,
    const std::string& targetName, const std::string& reason = "");

} // namespace progressive

#endif // PROGRESSIVE_EVENT_DISPLAY_HPP
