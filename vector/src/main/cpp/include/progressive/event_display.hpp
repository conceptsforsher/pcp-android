#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// ---- Event Display Utilities ----
//
// Original Kotlin: TimelineEventDecorator, TimelineEventDisplayUtils,
//   MessageDisplayFormatter, EventDisplayInfoBuilder

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

// ==== EventGroupingRule enum ====
//
// Original Kotlin: TimelineEventGroupingRule
// Controls when events are visually grouped together in the timeline.
// ALWAYS: group regardless of sender/time (e.g. reactions under parent)
// WITH_SAME_SENDER: group when same sender (default for messages)
// NEVER: never group (e.g. state events, calls)
// WITHIN_TIMEFRAME: group if within a time window (e.g. 5 min)

enum class EventGroupingRule {
    ALWAYS = 0,
    WITH_SAME_SENDER = 1,
    NEVER = 2,
    WITHIN_TIMEFRAME = 3
};

const char* eventGroupingRuleToString(EventGroupingRule r);
EventGroupingRule eventGroupingRuleFromString(const std::string& s);

// ==== Reply Info ====
//
// Original Kotlin: InReplyToData / ReplyToInfo
// Contains the data needed to render a reply preview in a message bubble.

struct ReplyInfo {
    std::string eventId;           // the event being replied to
    std::string senderId;
    std::string senderName;
    std::string body;              // preview body of replied message
    bool isRedacted = false;       // if replied event was redacted
};

// ==== Reaction Display ====
//
// Original Kotlin: ReactionAggregatedSummary + ReactionDisplayItem

struct ReactionDisplayItem {
    std::string key;              // emoji or reaction text (e.g. "👍")
    int count = 0;
    bool addedByMe = false;
};

// ==== Shield State ====
//
// Original Kotlin: ShieldState / E2EE decoration state
// RED: untrusted device / unknown key
// BLACK: blacklisted / unverified with warning
// NONE: no shield (unencrypted)
// GRAY: encrypted but not cross-signed

enum class ShieldState {
    NONE = 0,
    GREEN = 1,     // verified / cross-signed
    GRAY = 2,      // encrypted but not verified
    RED = 3,       // unverified / warning
    BLACK = 4      // blacklisted
};

const char* shieldStateToString(ShieldState s);
ShieldState shieldStateFromString(const std::string& s);

// ==== EventDisplayInfo -- full display-ready event data ====
//
// Original Kotlin: TimelineEvent display fields + decorator output
// Provides everything the UI needs to render one event bubble/cell.

struct EventDisplayInfo {
    std::string eventId;
    std::string body;                    // plain text body
    std::string formattedBody;           // HTML formatted body (or empty)
    std::string senderName;              // resolved display name
    std::string senderAvatar;            // resolved avatar URL
    std::string timestamp;               // formatted relative/absolute time
    std::string eventType;               // raw Matrix event type
    bool isEncrypted = false;            // event is e2ee
    bool isEdited = false;               // event has been edited (m.replace)
    bool isThreaded = false;             // event is in a thread
    ShieldState shieldState = ShieldState::NONE; // E2EE verification decoration
    std::vector<ReactionDisplayItem> reactionKeys; // aggregated reactions
    ReplyInfo replyInfo;                 // in-reply-to data (if reply)
    int64_t originServerTs = 0;          // raw timestamp for sorting
    std::string senderId;                // raw user ID
    bool isFromMe = false;               // sent by current user
    bool isStateEvent = false;           // m.room.* state event
};

// ==== Group Info ====
//
// Original Kotlin: EventGroupInfo -- metadata for visual grouping decisions

struct EventGroupInfo {
    bool isGroupedWithPrevious = false;  // should merge with prior event
    bool isGroupedWithNext = false;      // next event will merge with this one
    bool showSender = true;              // show sender name/avatar
    bool showTimestamp = true;           // show standalone timestamp
    EventGroupingRule rule = EventGroupingRule::WITH_SAME_SENDER;
};

// ==== Existing Functions ====

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

// Get an event type icon emoji.
std::string getEventTypeIcon(DisplayEventType type);

// Format event body for room list preview.
std::string formatEventPreview(const DisplayEvent& event, bool showSender);

// Get the default display name for a member event.
std::string formatMemberEvent(const std::string& senderName, const std::string& membership,
    const std::string& targetName, const std::string& reason = "");

// ==== formatEventForDisplay -- full display formatting ====
//
// Original Kotlin: TimelineEventDecorator.decorate(timelineEvent)
// Takes raw event data and produces a fully formatted EventDisplayInfo.
EventDisplayInfo formatEventForDisplay(
    const std::string& eventId,
    const std::string& eventType,
    const std::string& contentJson,
    const std::string& senderId,
    const std::string& senderName,
    const std::string& senderAvatar,
    int64_t originServerTs,
    const std::string& unsignedJson,
    bool isEncrypted,
    bool isFromMe,
    const std::vector<ReactionDisplayItem>& reactions = {});

// ==== getEventSenderDisplayName -- resolve sender name ====
//
// Original Kotlin: RoomMemberDataSource.getDisplayName(userId) ?: userId
// Falls back to userId if no display name is set.
std::string getEventSenderDisplayName(const std::string& userId,
    const std::string& rawDisplayName);

// ==== getEventAvatarUrl -- resolve avatar from member state ====
//
// Original Kotlin: RoomMemberDataSource.getAvatarUrl(userId) ?: ""
std::string getEventAvatarUrl(const std::string& userId,
    const std::string& rawAvatarUrl);

// ==== formatEventTimestamp -- relative time formatting ====
//
// Original Kotlin: DateUtils.formatRelativeTime(originServerTs)
// Produces: "just now", "5m ago", "Yesterday 3:14 PM", "May 13, 2025"
std::string formatEventTimestamp(int64_t epochMs, int64_t nowMs = 0);

// ==== getEventGroupInfo -- determine if event should be grouped with previous ====
//
// Original Kotlin: TimelineChunk.grouping logic
// Returns group info for this event relative to the previous one.
EventGroupInfo getEventGroupInfo(
    const std::string& eventType,
    const std::string& senderId,
    const std::string& prevSenderId,
    int64_t originServerTs,
    int64_t prevOriginServerTs);

// ==== shouldGroupEvents -- grouping decision ====
//
// Original Kotlin: EventGroupingPolicy.shouldGroup(a, b)
// Determines if two events should be visually grouped together.
bool shouldGroupEvents(const std::string& currentType, const std::string& currentSender,
    int64_t currentTs, const std::string& prevType, const std::string& prevSender,
    int64_t prevTs, int64_t mergeWindowMs = 300000);

// ==== isEventContinuation -- same sender within time window ====
//
// Original Kotlin: ContinuationDetector.isContinuation()
// Returns true if event is by the same sender within the merge window.
bool isEventContinuation(const std::string& currentSender, int64_t currentTs,
    const std::string& prevSender, int64_t prevTs, int64_t mergeWindowMs = 300000);

// ==== formatEventReactions -- format reaction chips ====
//
// Original Kotlin: ReactionRenderer.renderReactions(reactions)
// Produces a string like "👍 3 😄 1 ❤️ 2"
std::string formatEventReactions(const std::vector<ReactionDisplayItem>& reactions);

// ==== getReactionDisplayList -- ordered list of reactions for display ====
//
// Original Kotlin: ReactionAggregatedSummary sorted by count + firstTimestamp
// Returns reactions sorted by count descending, then by firstTimestamp.
std::vector<ReactionDisplayItem> getReactionDisplayList(
    const std::vector<ReactionDisplayItem>& rawReactions);

// ==== JSON Serialization ====

std::string displayEventToJson(const DisplayEvent& ev);
DisplayEvent displayEventFromJson(const std::string& json);
std::string eventDisplayInfoToJson(const EventDisplayInfo& info);
EventDisplayInfo eventDisplayInfoFromJson(const std::string& json);
std::string replyInfoToJson(const ReplyInfo& ri);
ReplyInfo replyInfoFromJson(const std::string& json);
std::string reactionDisplayItemToJson(const ReactionDisplayItem& ri);
ReactionDisplayItem reactionDisplayItemFromJson(const std::string& json);
std::string eventGroupingRuleToJson(EventGroupingRule r);
EventGroupingRule eventGroupingRuleFromJson(const std::string& json);
std::string shieldStateToJson(ShieldState s);
ShieldState shieldStateFromJson(const std::string& json);
std::string eventGroupInfoToJson(const EventGroupInfo& gi);
EventGroupInfo eventGroupInfoFromJson(const std::string& json);

// ============================================================================
// NEW: Extended Display Features
// ============================================================================

// Original Kotlin: EventDisplayMode, shouldShowTimelineAvatar, computeSenderDisplayColor,
//   formatMessageReplyPreview, truncateMessageBody, getEventAccessibilityLabel,
//   MessagePreviewOptions, formatMessagePreview, EventSelectionInfo,
//   formatEditionHistory, EventEditInfo

// Display verbosity mode for timeline events.
enum class EventDisplayMode {
    DEFAULT = 0,   // normal: body + sender + timestamp
    MINIMAL = 1,   // compact: body only
    DETAILED = 2,  // verbose: body + sender + timestamp + eventId + type
    RAW = 3        // show raw JSON content
};

// Decision whether to show the sender's avatar in a timeline bubble.
// Original Kotlin: shouldShowTimelineAvatar(event, previousEvent)
bool shouldShowTimelineAvatar(
    const std::string& senderId,
    const std::string& prevSenderId,
    bool isStateEvent
);

// Decision whether to show the sender's name above a message.
// Original Kotlin: shouldShowSenderName(event, previousEvent)
bool shouldShowSenderName(
    const std::string& senderId,
    const std::string& prevSenderId,
    int64_t timestampMs,
    int64_t prevTimestampMs,
    bool isDirectRoom
);

// Compute a deterministic display color from a userId.
// Uses a simple hash to produce a hue value.
// Original Kotlin: computeSenderDisplayColor(userId)
std::string computeSenderDisplayColor(const std::string& userId);

// Format a reply preview fallback text when the original message is unavailable.
// Original Kotlin: formatMessageReplyPreview(originalBody, originalSender)
std::string formatMessageReplyPreview(
    const std::string& originalBody,
    const std::string& originalSender,
    bool isRedacted
);

// Truncate a long message body to a maximum length.
// Appends "..." if truncated.
// Original Kotlin: truncateMessageBody(body, maxLen)
std::string truncateMessageBody(const std::string& body, int maxLen = 200);

// Build an accessibility (screen reader) label for an event.
// Original Kotlin: getEventAccessibilityLabel(event)
std::string getEventAccessibilityLabel(
    const std::string& senderName,
    const std::string& body,
    const std::string& timestamp,
    bool isFromMe,
    bool isEncrypted,
    bool isEdited,
    const std::vector<ReactionDisplayItem>& reactions
);

// Options controlling message preview formatting.
// Original Kotlin: MessagePreviewOptions
struct MessagePreviewOptions {
    int maxLines = 2;
    bool showFormatting = false;  // preserve markdown hints
    bool showEmoji = true;
    bool showSender = true;
};

// Format a message preview with configurable options.
// Original Kotlin: formatMessagePreview(body, options)
std::string formatMessagePreview(
    const std::string& body,
    const std::string& senderName,
    const MessagePreviewOptions& options
);

// Selection state of an event in multi-select mode.
// Original Kotlin: EventSelectionInfo
struct EventSelectionInfo {
    bool isSelected = false;
    bool isMultiSelectMode = false;
    bool isHighlighted = false;
};

// Format edit history for an edited event.
// Returns something like "Edited 3 times" or "Edited at 14:32".
// Original Kotlin: formatEditionHistory(editCount)
std::string formatEditionHistory(int editCount);

// Information about an event's edit history.
// Original Kotlin: EventEditInfo
struct EventEditInfo {
    int editCount = 0;
    int64_t lastEditTs = 0;
    std::string originalEventId;  // the first (original) event in the edit chain
};

} // namespace progressive
