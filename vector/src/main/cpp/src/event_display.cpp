#include "progressive/event_display.hpp"
#include "progressive/json_parser.hpp"
#include <sstream>
#include <ctime>
#include <algorithm>
#include <chrono>
#include <unordered_map>

namespace progressive {

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

static bool extractJsonBool(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return false;
    pos = json.find(':', pos);
    if (pos == std::string::npos) return false;
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    return json.compare(pos, 4, "true") == 0;
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

static int64_t parseJsonInt(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return 0;
    pos = json.find(':', pos);
    if (pos == std::string::npos) return 0;
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    if (pos >= json.size()) return 0;
    int64_t val = 0;
    bool neg = false;
    if (json[pos] == '-') { neg = true; pos++; }
    while (pos < json.size() && json[pos] >= '0' && json[pos] <= '9') {
        val = val * 10 + (json[pos] - '0');
        pos++;
    }
    return neg ? -val : val;
}

// ==== Enum to String Converters ====

const char* eventGroupingRuleToString(EventGroupingRule r) {
    switch (r) {
        case EventGroupingRule::ALWAYS:           return "ALWAYS";
        case EventGroupingRule::WITH_SAME_SENDER:  return "WITH_SAME_SENDER";
        case EventGroupingRule::NEVER:             return "NEVER";
        case EventGroupingRule::WITHIN_TIMEFRAME:  return "WITHIN_TIMEFRAME";
    }
    return "WITH_SAME_SENDER";
}

EventGroupingRule eventGroupingRuleFromString(const std::string& s) {
    if (s == "ALWAYS")            return EventGroupingRule::ALWAYS;
    if (s == "WITH_SAME_SENDER")  return EventGroupingRule::WITH_SAME_SENDER;
    if (s == "NEVER")             return EventGroupingRule::NEVER;
    if (s == "WITHIN_TIMEFRAME")  return EventGroupingRule::WITHIN_TIMEFRAME;
    return EventGroupingRule::WITH_SAME_SENDER;
}

const char* shieldStateToString(ShieldState s) {
    switch (s) {
        case ShieldState::NONE:  return "NONE";
        case ShieldState::GREEN: return "GREEN";
        case ShieldState::GRAY:  return "GRAY";
        case ShieldState::RED:   return "RED";
        case ShieldState::BLACK: return "BLACK";
    }
    return "NONE";
}

ShieldState shieldStateFromString(const std::string& s) {
    if (s == "GREEN") return ShieldState::GREEN;
    if (s == "GRAY")  return ShieldState::GRAY;
    if (s == "RED")   return ShieldState::RED;
    if (s == "BLACK") return ShieldState::BLACK;
    return ShieldState::NONE;
}

// ==== Existing Functions ====

DisplayEventType classifyEvent(const std::string& eventType, const std::string& msgType) {
    if (eventType == "m.room.message" || eventType == "m.room.encrypted") {
        if (msgType == "m.text") return DisplayEventType::Message;
        if (msgType == "m.emote") return DisplayEventType::Emote;
        if (msgType == "m.image") return DisplayEventType::Image;
        if (msgType == "m.video") return DisplayEventType::Video;
        if (msgType == "m.audio") return DisplayEventType::Audio;
        if (msgType == "m.file") return DisplayEventType::File;
        if (msgType == "m.sticker" || eventType.find("sticker") != std::string::npos)
            return DisplayEventType::Sticker;
        if (msgType == "m.location") return DisplayEventType::Location;
        if (msgType == "m.poll" || eventType.find("poll") != std::string::npos)
            return DisplayEventType::Poll;
        return DisplayEventType::Message;
    }
    if (eventType == "m.reaction") return DisplayEventType::Reaction;
    if (eventType == "m.room.member") return DisplayEventType::MemberEvent;
    if (eventType == "m.room.name") return DisplayEventType::RoomName;
    if (eventType == "m.room.topic") return DisplayEventType::RoomTopic;
    if (eventType == "m.room.avatar") return DisplayEventType::RoomAvatar;
    if (eventType == "m.room.encryption") return DisplayEventType::Encryption;
    if (eventType == "m.room.redaction")
        return DisplayEventType::Redaction;
    if (eventType.find("m.call.") == 0) return DisplayEventType::Call;
    if (eventType == "m.widget") return DisplayEventType::Widget;
    if (eventType == "m.room.server_notice") return DisplayEventType::Notice;
    return DisplayEventType::Unknown;
}

bool isContinuation(const std::string& currentSender, const std::string& previousSender,
    int64_t currentTs, int64_t previousTs, int64_t mergeWindowMs) {
    if (currentSender.empty() || previousSender.empty()) return false;
    if (currentSender != previousSender) return false;
    if (currentTs <= 0 || previousTs <= 0) return false;
    return (currentTs - previousTs) <= mergeWindowMs;
}

bool shouldShowTimestamp(const std::string& currentSender, int64_t currentTs,
    int64_t previousTs, bool showAll) {
    if (showAll) return true;
    if (previousTs <= 0) return true;
    // Show timestamp if more than 5 minutes since last message
    return (currentTs - previousTs) >= 300000;
}

bool shouldShowAvatar(const std::string& currentSender, const std::string& previousSender,
    bool isLastInGroup) {
    if (currentSender != previousSender) return true;
    return isLastInGroup;
}

std::string getEventTypeDescription(DisplayEventType type) {
    switch (type) {
        case DisplayEventType::Message:    return "Message";
        case DisplayEventType::Emote:      return "Emote";
        case DisplayEventType::Notice:     return "Server notice";
        case DisplayEventType::Image:      return "Image";
        case DisplayEventType::Video:      return "Video";
        case DisplayEventType::Audio:      return "Audio";
        case DisplayEventType::File:       return "File";
        case DisplayEventType::Sticker:    return "Sticker";
        case DisplayEventType::Location:   return "Location";
        case DisplayEventType::Poll:       return "Poll";
        case DisplayEventType::Reaction:   return "Reaction";
        case DisplayEventType::MemberEvent: return "Membership change";
        case DisplayEventType::RoomName:   return "Room name changed";
        case DisplayEventType::RoomTopic:  return "Topic changed";
        case DisplayEventType::RoomAvatar: return "Avatar changed";
        case DisplayEventType::Encryption: return "Encryption changed";
        case DisplayEventType::Call:       return "Call";
        case DisplayEventType::Widget:     return "Widget";
        case DisplayEventType::Redaction:  return "Message removed";
        default:                           return "Unknown";
    }
}

std::string getEventTypeIcon(DisplayEventType type) {
    switch (type) {
        case DisplayEventType::Message:  return "\xF0\x9F\x92\xAC"; // 💬
        case DisplayEventType::Image:    return "\xF0\x9F\x93\xB7"; // 📷
        case DisplayEventType::Video:    return "\xF0\x9F\x8E\xA5"; // 🎥
        case DisplayEventType::Audio:    return "\xF0\x9F\x8E\xB5"; // 🎵
        case DisplayEventType::File:     return "\xF0\x9F\x93\x84"; // 📄
        case DisplayEventType::Sticker:  return "\xF0\x9F\x96\xBC"; // 🖼
        case DisplayEventType::Location: return "\xF0\x9F\x93\x8D"; // 📍
        case DisplayEventType::Poll:     return "\xF0\x9F\x93\x8A"; // 📊
        case DisplayEventType::Reaction: return "\xE2\x9D\xA4";     // ❤
        case DisplayEventType::MemberEvent: return "\xF0\x9F\x91\xA4"; // 👤
        case DisplayEventType::Call:     return "\xF0\x9F\x93\x9E"; // 📞
        case DisplayEventType::Redaction: return "\xE2\x9D\x8C";    // ❌
        case DisplayEventType::Notice:   return "\xE2\x84\xB9";     // ℹ
        default:                         return "\xF0\x9F\x93\x9D"; // 📝
    }
}

std::string formatEventPreview(const DisplayEvent& event, bool showSender) {
    std::ostringstream out;
    if (showSender && !event.senderName.empty()) {
        out << event.senderName << ": ";
    }

    switch (event.type) {
        case DisplayEventType::Image:
            out << "[Image]";
            if (!event.body.empty()) out << " " << event.body;
            break;
        case DisplayEventType::Video:
            out << "[Video]";
            if (!event.body.empty()) out << " " << event.body;
            break;
        case DisplayEventType::Audio:
            out << "[Audio]";
            break;
        case DisplayEventType::File:
            out << "[File]";
            break;
        case DisplayEventType::Sticker:
            out << "[Sticker]";
            break;
        case DisplayEventType::Location:
            out << "[Location]";
            break;
        case DisplayEventType::Poll:
            out << "[Poll: " << (event.body.size() > 30 ? event.body.substr(0, 27) + "..." : event.body) << "]";
            break;
        case DisplayEventType::MemberEvent:
            out << event.body;
            break;
        case DisplayEventType::Redaction:
            out << "[Message removed]";
            break;
        case DisplayEventType::Encryption:
            out << "[Encryption changed]";
            break;
        default:
            out << (event.body.size() > 60 ? event.body.substr(0, 57) + "..." : event.body);
            break;
    }

    return out.str();
}

std::string formatMemberEvent(const std::string& senderName, const std::string& membership,
    const std::string& targetName, const std::string& reason) {
    std::ostringstream out;

    if (membership == "join") {
        out << senderName << " joined the room";
    } else if (membership == "leave") {
        out << senderName << " left the room";
    } else if (membership == "invite") {
        out << senderName << " invited " << targetName;
    } else if (membership == "ban") {
        out << senderName << " banned " << targetName;
    } else if (membership == "knock") {
        out << senderName << " requested to join";
    } else {
        out << senderName << " changed membership to " << membership;
    }

    if (!reason.empty()) {
        out << " -- " << reason;
    }

    return out.str();
}

// ==== formatEventForDisplay -- full display formatting ====
//
// Original Kotlin: TimelineEventDecorator.decorate(timelineEvent)

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
    const std::vector<ReactionDisplayItem>& reactions)
{
    EventDisplayInfo info;
    info.eventId = eventId;
    info.eventType = eventType;
    info.senderId = senderId;
    info.senderName = senderName;
    info.senderAvatar = senderAvatar;
    info.originServerTs = originServerTs;
    info.isEncrypted = isEncrypted;
    info.isFromMe = isFromMe;
    info.reactionKeys = reactions;

    // Resolve body
    if (isEncrypted && contentJson.find("\"body\"") == std::string::npos) {
        info.body = "Encrypted message";
        info.formattedBody = "";
    } else {
        info.body = extractJsonString(contentJson, "body");
        info.formattedBody = extractJsonString(contentJson, "formatted_body");
    }

    // Format timestamp
    info.timestamp = formatEventTimestamp(originServerTs, 0);

    // Check for edit (m.new_content or m.replace relation)
    if (contentJson.find("\"m.new_content\"") != std::string::npos) {
        info.isEdited = true;
    } else {
        auto relObj = extractJsonObject(contentJson, "m.relates_to");
        if (!relObj.empty()) {
            auto relType = extractJsonString(relObj, "rel_type");
            if (relType == "m.replace") info.isEdited = true;
        }
    }

    // Check for thread (m.relates_to with is_falling_back and rel_type m.thread)
    auto relObj = extractJsonObject(contentJson, "m.relates_to");
    if (!relObj.empty()) {
        auto relType = extractJsonString(relObj, "rel_type");
        bool isFallingBack = extractJsonBool(relObj, "is_falling_back");
        if (relType == "m.thread" && !isFallingBack) {
            info.isThreaded = true;
        }
    }

    // Extract reply info
    auto replyObj = extractJsonObject(contentJson, "m.in_reply_to");
    if (!replyObj.empty()) {
        info.replyInfo.eventId = extractJsonString(replyObj, "event_id");
        // Sender and body of reply are typically resolved from the original event
        // by the caller. Here we set what we can from the content.
    }

    // Check for state events
    info.isStateEvent = (eventType.find("m.room.") == 0 &&
                         eventType != "m.room.message" &&
                         eventType != "m.room.encrypted" &&
                         eventType != "m.room.redaction");

    // Default shield state
    if (isEncrypted) {
        info.shieldState = ShieldState::GRAY;
    } else {
        info.shieldState = ShieldState::NONE;
    }

    return info;
}

// ==== getEventSenderDisplayName -- resolve sender name ====
//
// Original Kotlin: RoomMemberDataSource.getDisplayName(userId) ?: userId

std::string getEventSenderDisplayName(const std::string& userId,
    const std::string& rawDisplayName)
{
    if (!rawDisplayName.empty()) return rawDisplayName;
    if (!userId.empty()) return userId;
    return "Unknown";
}

// ==== getEventAvatarUrl -- resolve avatar from member state ====
//
// Original Kotlin: RoomMemberDataSource.getAvatarUrl(userId) ?: ""

std::string getEventAvatarUrl(const std::string& userId,
    const std::string& rawAvatarUrl)
{
    if (!rawAvatarUrl.empty()) return rawAvatarUrl;
    return "";
}

// ==== formatEventTimestamp -- relative time formatting ====
//
// Original Kotlin: DateUtils.formatRelativeTime(originServerTs)
// Produces: "just now", "5m ago", "Yesterday 3:14 PM", "May 13, 2025"

std::string formatEventTimestamp(int64_t epochMs, int64_t nowMs) {
    if (epochMs <= 0) return "";

    if (nowMs <= 0) {
        nowMs = static_cast<int64_t>(time(nullptr)) * 1000;
    }

    int64_t diffMs = nowMs - epochMs;
    if (diffMs < 0) diffMs = 0;

    int64_t diffSec = diffMs / 1000;
    int64_t diffMin = diffSec / 60;
    int64_t diffHour = diffMin / 60;
    int64_t diffDay = diffHour / 24;

    if (diffSec < 60) return "just now";
    if (diffMin < 60) return std::to_string(diffMin) + "m ago";
    if (diffHour < 6) return std::to_string(diffHour) + "h ago";

    time_t t = epochMs / 1000;
    struct tm result;
    gmtime_r(&t, &result);

    time_t now = nowMs / 1000;
    struct tm nowTm;
    gmtime_r(&now, &nowTm);

    char timeBuf[16];
    strftime(timeBuf, sizeof(timeBuf), "%l:%M %p", &result);

    // Today: show time only with "Today"
    if (nowTm.tm_year == result.tm_year &&
        nowTm.tm_mon == result.tm_mon &&
        nowTm.tm_mday == result.tm_mday) {
        return "Today " + std::string(timeBuf);
    }

    // Yesterday
    time_t yesterday = now - 86400;
    struct tm yestTm;
    gmtime_r(&yesterday, &yestTm);
    if (yestTm.tm_year == result.tm_year &&
        yestTm.tm_mon == result.tm_mon &&
        yestTm.tm_mday == result.tm_mday) {
        return "Yesterday " + std::string(timeBuf);
    }

    // This week: day name + time
    if (diffDay < 7 && nowTm.tm_year == result.tm_year) {
        char dayBuf[16];
        strftime(dayBuf, sizeof(dayBuf), "%A", &result);
        return std::string(dayBuf) + " " + std::string(timeBuf);
    }

    // This year: month day + time
    if (nowTm.tm_year == result.tm_year) {
        char dateBuf[16];
        strftime(dateBuf, sizeof(dateBuf), "%b %d", &result);
        return std::string(dateBuf) + " " + std::string(timeBuf);
    }

    // Older: full date + time
    char fullBuf[32];
    strftime(fullBuf, sizeof(fullBuf), "%b %d, %Y", &result);
    return std::string(fullBuf) + " " + std::string(timeBuf);
}

// ==== getEventGroupInfo -- determine if event should be grouped with previous ====
//
// Original Kotlin: TimelineChunk.grouping logic

EventGroupInfo getEventGroupInfo(
    const std::string& eventType,
    const std::string& senderId,
    const std::string& prevSenderId,
    int64_t originServerTs,
    int64_t prevOriginServerTs)
{
    EventGroupInfo gi;

    // State events and calls are never grouped
    if (eventType.find("m.room.") == 0 &&
        eventType != "m.room.message" &&
        eventType != "m.room.encrypted") {
        gi.rule = EventGroupingRule::NEVER;
        gi.showSender = true;
        gi.showTimestamp = true;
        gi.isGroupedWithPrevious = false;
        return gi;
    }

    // Non-message events (reactions, etc.)
    if (eventType == "m.reaction" || eventType == "m.sticker") {
        gi.rule = EventGroupingRule::ALWAYS;
        gi.isGroupedWithPrevious = (prevSenderId == senderId);
        gi.showSender = false;
        gi.showTimestamp = false;
        return gi;
    }

    // Messages: group by same sender within time window
    gi.rule = EventGroupingRule::WITH_SAME_SENDER;

    if (prevSenderId.empty() || senderId.empty()) {
        gi.isGroupedWithPrevious = false;
        gi.showSender = true;
        gi.showTimestamp = true;
        return gi;
    }

    if (senderId == prevSenderId && prevOriginServerTs > 0 && originServerTs > 0) {
        int64_t diff = originServerTs - prevOriginServerTs;
        if (diff >= 0 && diff <= 300000) {
            gi.isGroupedWithPrevious = true;
            gi.showSender = false;
            gi.showTimestamp = false;
        } else {
            gi.isGroupedWithPrevious = false;
            gi.showSender = true;
            gi.showTimestamp = true;
        }
    } else {
        gi.isGroupedWithPrevious = false;
        gi.showSender = true;
        gi.showTimestamp = true;
    }

    return gi;
}

// ==== shouldGroupEvents -- grouping decision ====
//
// Original Kotlin: EventGroupingPolicy.shouldGroup(a, b)

bool shouldGroupEvents(const std::string& currentType, const std::string& currentSender,
    int64_t currentTs, const std::string& prevType, const std::string& prevSender,
    int64_t prevTs, int64_t mergeWindowMs)
{
    // State events, calls, and redactions never group
    auto isStateLike = [](const std::string& t) -> bool {
        return t.find("m.room.") == 0 &&
               t != "m.room.message" &&
               t != "m.room.encrypted";
    };

    if (isStateLike(currentType) || isStateLike(prevType)) return false;
    if (currentType.find("m.call.") == 0 || prevType.find("m.call.") == 0) return false;
    if (currentType == "m.room.redaction" || prevType == "m.room.redaction") return false;

    // Reactions always group with parent
    if (currentType == "m.reaction" || prevType == "m.reaction") return true;

    // Must have same sender
    if (currentSender != prevSender) return false;
    if (currentSender.empty() || prevSender.empty()) return false;

    // Within time window
    if (currentTs <= 0 || prevTs <= 0) return false;
    int64_t diff = currentTs - prevTs;
    return diff >= 0 && diff <= mergeWindowMs;
}

// ==== isEventContinuation -- same sender within time window ====
//
// Original Kotlin: ContinuationDetector.isContinuation()

bool isEventContinuation(const std::string& currentSender, int64_t currentTs,
    const std::string& prevSender, int64_t prevTs, int64_t mergeWindowMs)
{
    if (currentSender.empty() || prevSender.empty()) return false;
    if (currentSender != prevSender) return false;
    if (currentTs <= 0 || prevTs <= 0) return false;
    int64_t diff = currentTs - prevTs;
    return diff >= 0 && diff <= mergeWindowMs;
}

// ==== formatEventReactions -- format reaction chips ====
//
// Original Kotlin: ReactionRenderer.renderReactions(reactions)

std::string formatEventReactions(const std::vector<ReactionDisplayItem>& reactions) {
    if (reactions.empty()) return "";

    std::ostringstream out;
    for (size_t i = 0; i < reactions.size(); i++) {
        if (i > 0) out << "  ";
        out << reactions[i].key;
        if (reactions[i].count > 1) {
            out << " " << reactions[i].count;
        }
    }
    return out.str();
}

// ==== getReactionDisplayList -- ordered list of reactions for display ====
//
// Original Kotlin: ReactionAggregatedSummary sorted by count + firstTimestamp

std::vector<ReactionDisplayItem> getReactionDisplayList(
    const std::vector<ReactionDisplayItem>& rawReactions)
{
    auto sorted = rawReactions;
    std::sort(sorted.begin(), sorted.end(),
        [](const ReactionDisplayItem& a, const ReactionDisplayItem& b) {
            if (a.count != b.count) return a.count > b.count;
            return a.key < b.key;
        });
    return sorted;
}

// ==== JSON Serialization ====

std::string displayEventToJson(const DisplayEvent& ev) {
    std::ostringstream j;
    j << "{";
    j << "\"eventId\":\"" << jsonEscape(ev.eventId) << "\",";
    j << "\"senderName\":\"" << jsonEscape(ev.senderName) << "\",";
    j << "\"senderId\":\"" << jsonEscape(ev.senderId) << "\",";
    j << "\"type\":" << static_cast<int>(ev.type) << ",";
    j << "\"body\":\"" << jsonEscape(ev.body) << "\",";
    j << "\"formattedBody\":\"" << jsonEscape(ev.formattedBody) << "\",";
    j << "\"timestamp\":\"" << jsonEscape(ev.timestamp) << "\",";
    j << "\"originServerTs\":" << ev.originServerTs << ",";
    j << "\"isEncrypted\":" << (ev.isEncrypted ? "true" : "false") << ",";
    j << "\"isRedacted\":" << (ev.isRedacted ? "true" : "false") << ",";
    j << "\"isContinuation\":" << (ev.isContinuation ? "true" : "false") << ",";
    j << "\"isFromMe\":" << (ev.isFromMe ? "true" : "false") << ",";
    j << "\"showTimestamp\":" << (ev.showTimestamp ? "true" : "false") << ",";
    j << "\"showAvatar\":" << (ev.showAvatar ? "true" : "false") << ",";
    j << "\"showSender\":" << (ev.showSender ? "true" : "false") << ",";
    j << "\"thumbnailUrl\":\"" << jsonEscape(ev.thumbnailUrl) << "\",";
    j << "\"mxcUrl\":\"" << jsonEscape(ev.mxcUrl) << "\",";
    j << "\"imgWidth\":" << ev.imgWidth << ",";
    j << "\"imgHeight\":" << ev.imgHeight;
    j << "}";
    return j.str();
}

DisplayEvent displayEventFromJson(const std::string& json) {
    DisplayEvent ev;
    ev.eventId = extractJsonString(json, "eventId");
    ev.senderName = extractJsonString(json, "senderName");
    ev.senderId = extractJsonString(json, "senderId");
    ev.type = static_cast<DisplayEventType>(static_cast<int>(extractJsonInt64(json, "type")));
    ev.body = extractJsonString(json, "body");
    ev.formattedBody = extractJsonString(json, "formattedBody");
    ev.timestamp = extractJsonString(json, "timestamp");
    ev.originServerTs = extractJsonInt64(json, "originServerTs");
    ev.isEncrypted = extractJsonBool(json, "isEncrypted");
    ev.isRedacted = extractJsonBool(json, "isRedacted");
    ev.isContinuation = extractJsonBool(json, "isContinuation");
    ev.isFromMe = extractJsonBool(json, "isFromMe");
    ev.showTimestamp = extractJsonBool(json, "showTimestamp");
    ev.showAvatar = extractJsonBool(json, "showAvatar");
    ev.showSender = extractJsonBool(json, "showSender");
    ev.thumbnailUrl = extractJsonString(json, "thumbnailUrl");
    ev.mxcUrl = extractJsonString(json, "mxcUrl");
    ev.imgWidth = static_cast<int>(extractJsonInt64(json, "imgWidth"));
    ev.imgHeight = static_cast<int>(extractJsonInt64(json, "imgHeight"));
    return ev;
}

std::string eventDisplayInfoToJson(const EventDisplayInfo& info) {
    std::ostringstream j;
    j << "{";
    j << "\"eventId\":\"" << jsonEscape(info.eventId) << "\",";
    j << "\"body\":\"" << jsonEscape(info.body) << "\",";
    j << "\"formattedBody\":\"" << jsonEscape(info.formattedBody) << "\",";
    j << "\"senderName\":\"" << jsonEscape(info.senderName) << "\",";
    j << "\"senderAvatar\":\"" << jsonEscape(info.senderAvatar) << "\",";
    j << "\"timestamp\":\"" << jsonEscape(info.timestamp) << "\",";
    j << "\"eventType\":\"" << jsonEscape(info.eventType) << "\",";
    j << "\"isEncrypted\":" << (info.isEncrypted ? "true" : "false") << ",";
    j << "\"isEdited\":" << (info.isEdited ? "true" : "false") << ",";
    j << "\"isThreaded\":" << (info.isThreaded ? "true" : "false") << ",";
    j << "\"shieldState\":\"" << shieldStateToString(info.shieldState) << "\",";
    j << "\"originServerTs\":" << info.originServerTs << ",";
    j << "\"senderId\":\"" << jsonEscape(info.senderId) << "\",";
    j << "\"isFromMe\":" << (info.isFromMe ? "true" : "false") << ",";
    j << "\"isStateEvent\":" << (info.isStateEvent ? "true" : "false") << ",";
    j << "\"reactionKeys\":[";
    for (size_t i = 0; i < info.reactionKeys.size(); i++) {
        if (i > 0) j << ",";
        j << reactionDisplayItemToJson(info.reactionKeys[i]);
    }
    j << "],";
    j << "\"replyInfo\":" << replyInfoToJson(info.replyInfo);
    j << "}";
    return j.str();
}

EventDisplayInfo eventDisplayInfoFromJson(const std::string& json) {
    EventDisplayInfo info;
    info.eventId = extractJsonString(json, "eventId");
    info.body = extractJsonString(json, "body");
    info.formattedBody = extractJsonString(json, "formattedBody");
    info.senderName = extractJsonString(json, "senderName");
    info.senderAvatar = extractJsonString(json, "senderAvatar");
    info.timestamp = extractJsonString(json, "timestamp");
    info.eventType = extractJsonString(json, "eventType");
    info.isEncrypted = extractJsonBool(json, "isEncrypted");
    info.isEdited = extractJsonBool(json, "isEdited");
    info.isThreaded = extractJsonBool(json, "isThreaded");
    info.shieldState = shieldStateFromString(extractJsonString(json, "shieldState"));
    info.originServerTs = extractJsonInt64(json, "originServerTs");
    info.senderId = extractJsonString(json, "senderId");
    info.isFromMe = extractJsonBool(json, "isFromMe");
    info.isStateEvent = extractJsonBool(json, "isStateEvent");
    info.replyInfo = replyInfoFromJson(extractJsonObject(json, "replyInfo"));

    auto arrPos = json.find("\"reactionKeys\"");
    if (arrPos != std::string::npos) {
        arrPos = json.find('[', arrPos);
        if (arrPos != std::string::npos) {
            arrPos++;
            while (arrPos < json.size()) {
                while (arrPos < json.size() && (json[arrPos] == ' ' || json[arrPos] == ',' || json[arrPos] == '\n')) arrPos++;
                if (arrPos >= json.size() || json[arrPos] == ']') break;
                if (json[arrPos] == '{') {
                    int d = 1;
                    size_t start = arrPos;
                    arrPos++;
                    while (arrPos < json.size() && d > 0) {
                        if (json[arrPos] == '{') d++; else if (json[arrPos] == '}') d--;
                        arrPos++;
                    }
                    info.reactionKeys.push_back(reactionDisplayItemFromJson(json.substr(start, arrPos - start)));
                }
            }
        }
    }

    return info;
}

std::string replyInfoToJson(const ReplyInfo& ri) {
    std::ostringstream j;
    j << "{";
    j << "\"eventId\":\"" << jsonEscape(ri.eventId) << "\",";
    j << "\"senderId\":\"" << jsonEscape(ri.senderId) << "\",";
    j << "\"senderName\":\"" << jsonEscape(ri.senderName) << "\",";
    j << "\"body\":\"" << jsonEscape(ri.body) << "\",";
    j << "\"isRedacted\":" << (ri.isRedacted ? "true" : "false");
    j << "}";
    return j.str();
}

ReplyInfo replyInfoFromJson(const std::string& json) {
    ReplyInfo ri;
    ri.eventId = extractJsonString(json, "eventId");
    ri.senderId = extractJsonString(json, "senderId");
    ri.senderName = extractJsonString(json, "senderName");
    ri.body = extractJsonString(json, "body");
    ri.isRedacted = extractJsonBool(json, "isRedacted");
    return ri;
}

std::string reactionDisplayItemToJson(const ReactionDisplayItem& ri) {
    std::ostringstream j;
    j << "{";
    j << "\"key\":\"" << jsonEscape(ri.key) << "\",";
    j << "\"count\":" << ri.count << ",";
    j << "\"addedByMe\":" << (ri.addedByMe ? "true" : "false");
    j << "}";
    return j.str();
}

ReactionDisplayItem reactionDisplayItemFromJson(const std::string& json) {
    ReactionDisplayItem ri;
    ri.key = extractJsonString(json, "key");
    ri.count = static_cast<int>(extractJsonInt64(json, "count"));
    ri.addedByMe = extractJsonBool(json, "addedByMe");
    return ri;
}

std::string eventGroupingRuleToJson(EventGroupingRule r) {
    return "\"" + std::string(eventGroupingRuleToString(r)) + "\"";
}

EventGroupingRule eventGroupingRuleFromJson(const std::string& json) {
    std::string s = json;
    if (!s.empty() && s.front() == '"') s = s.substr(1);
    if (!s.empty() && s.back() == '"') s.pop_back();
    return eventGroupingRuleFromString(s);
}

std::string shieldStateToJson(ShieldState s) {
    return "\"" + std::string(shieldStateToString(s)) + "\"";
}

ShieldState shieldStateFromJson(const std::string& json) {
    std::string s = json;
    if (!s.empty() && s.front() == '"') s = s.substr(1);
    if (!s.empty() && s.back() == '"') s.pop_back();
    return shieldStateFromString(s);
}

std::string eventGroupInfoToJson(const EventGroupInfo& gi) {
    std::ostringstream j;
    j << "{";
    j << "\"isGroupedWithPrevious\":" << (gi.isGroupedWithPrevious ? "true" : "false") << ",";
    j << "\"isGroupedWithNext\":" << (gi.isGroupedWithNext ? "true" : "false") << ",";
    j << "\"showSender\":" << (gi.showSender ? "true" : "false") << ",";
    j << "\"showTimestamp\":" << (gi.showTimestamp ? "true" : "false") << ",";
    j << "\"rule\":\"" << eventGroupingRuleToString(gi.rule) << "\"";
    j << "}";
    return j.str();
}

EventGroupInfo eventGroupInfoFromJson(const std::string& json) {
    EventGroupInfo gi;
    gi.isGroupedWithPrevious = extractJsonBool(json, "isGroupedWithPrevious");
    gi.isGroupedWithNext = extractJsonBool(json, "isGroupedWithNext");
    gi.showSender = extractJsonBool(json, "showSender");
    gi.showTimestamp = extractJsonBool(json, "showTimestamp");
    gi.rule = eventGroupingRuleFromString(extractJsonString(json, "rule"));
    return gi;
}

// ============================================================================
// NEW: Extended Display Features
// ============================================================================

// ----- shouldShowTimelineAvatar -----
// Original Kotlin: TimelineEventDecorator.shouldShowAvatar(event, prevEvent)
// Show avatar when: different sender than previous, or state event

bool shouldShowTimelineAvatar(
    const std::string& senderId,
    const std::string& prevSenderId,
    bool isStateEvent)
{
    if (isStateEvent) return true;
    return senderId != prevSenderId;
}

// ----- shouldShowSenderName -----
// Original Kotlin: TimelineEventDecorator.shouldShowSender(event, prevEvent)
// Show sender name in DMs always; in rooms when different sender or after time gap

bool shouldShowSenderName(
    const std::string& senderId,
    const std::string& prevSenderId,
    int64_t timestampMs,
    int64_t prevTimestampMs,
    bool isDirectRoom)
{
    if (isDirectRoom) return false; // DM rooms typically hide names
    if (senderId != prevSenderId) return true;
    if (prevTimestampMs <= 0 || timestampMs <= 0) return true;
    // Show name if > 5 minutes gap
    return (timestampMs - prevTimestampMs) > 300000;
}

// ----- computeSenderDisplayColor -----
// Original Kotlin: computeSenderDisplayColor(userId)
// Deterministic hue from userId using a simple hash

std::string computeSenderDisplayColor(const std::string& userId) {
    // Simple DJB2 hash
    unsigned long hash = 5381;
    for (char c : userId) {
        hash = ((hash << 5) + hash) + static_cast<unsigned char>(c);
    }
    int hue = static_cast<int>(hash % 360);

    // Map hue to a pastel-ish color using HSL->RGB
    // Use fixed saturation 65% and lightness 55% for readability
    auto hslToRgb = [](float h, float s, float l) -> std::string {
        auto hue2rgb = [](float p, float q, float t) -> float {
            if (t < 0.0f) t += 1.0f;
            if (t > 1.0f) t -= 1.0f;
            if (t < 1.0f / 6.0f) return p + (q - p) * 6.0f * t;
            if (t < 1.0f / 2.0f) return q;
            if (t < 2.0f / 3.0f) return p + (q - p) * (2.0f / 3.0f - t) * 6.0f;
            return p;
        };

        float hf = h / 360.0f;
        float q = l < 0.5f ? l * (1.0f + s) : l + s - l * s;
        float p = 2.0f * l - q;
        int r = static_cast<int>(hue2rgb(p, q, hf + 1.0f / 3.0f) * 255);
        int g = static_cast<int>(hue2rgb(p, q, hf) * 255);
        int b = static_cast<int>(hue2rgb(p, q, hf - 1.0f / 3.0f) * 255);

        char buf[8];
        snprintf(buf, sizeof(buf), "#%02X%02X%02X", r, g, b);
        return std::string(buf);
    };

    return hslToRgb(static_cast<float>(hue), 0.65f, 0.55f);
}

// ----- formatMessageReplyPreview -----
// Original Kotlin: formatMessageReplyPreview(body, sender, isRedacted)

std::string formatMessageReplyPreview(
    const std::string& originalBody,
    const std::string& originalSender,
    bool isRedacted)
{
    if (isRedacted) return "Original message was removed";

    std::string truncated = originalBody.size() > 80
        ? originalBody.substr(0, 77) + "..."
        : originalBody;

    return "> " + originalSender + ": " + truncated;
}

// ----- truncateMessageBody -----
// Original Kotlin: truncateMessageBody(body, maxLen)

std::string truncateMessageBody(const std::string& body, int maxLen) {
    if (static_cast<int>(body.size()) <= maxLen) return body;
    return body.substr(0, maxLen - 3) + "...";
}

// ----- getEventAccessibilityLabel -----
// Original Kotlin: getEventAccessibilityLabel(event)

std::string getEventAccessibilityLabel(
    const std::string& senderName,
    const std::string& body,
    const std::string& timestamp,
    bool isFromMe,
    bool isEncrypted,
    bool isEdited,
    const std::vector<ReactionDisplayItem>& reactions)
{
    std::ostringstream label;

    if (isFromMe) {
        label << "You sent";
    } else {
        label << senderName << " sent";
    }

    if (isEncrypted) {
        label << " an encrypted message";
    } else {
        label << " a message: " << body;
    }

    if (isEdited) {
        label << " (edited)";
    }

    if (!timestamp.empty()) {
        label << " at " << timestamp;
    }

    if (!reactions.empty()) {
        label << ". Reactions: ";
        for (size_t i = 0; i < reactions.size(); i++) {
            if (i > 0) label << ", ";
            label << reactions[i].key;
            if (reactions[i].count > 1) {
                label << " " << reactions[i].count;
            }
        }
    }

    return label.str();
}

// ----- formatMessagePreview -----
// Original Kotlin: formatMessagePreview(body, senderName, options)

std::string formatMessagePreview(
    const std::string& body,
    const std::string& senderName,
    const MessagePreviewOptions& options)
{
    std::ostringstream out;

    if (options.showSender && !senderName.empty()) {
        out << senderName << ": ";
    }

    if (options.maxLines <= 0) {
        out << body;
    } else {
        // Count newlines and truncate
        int lineCount = 1;
        std::string result;
        for (size_t i = 0; i < body.size(); i++) {
            if (body[i] == '\n' && (i > 0 || result.size() > 0)) {
                lineCount++;
                if (lineCount > options.maxLines) {
                    result += "...";
                    break;
                }
            }
            if (lineCount <= options.maxLines) {
                result += body[i];
            }
        }
        if (result.size() > 200) {
            result = result.substr(0, 197) + "...";
        }
        out << result;
    }

    return out.str();
}

// ----- formatEditionHistory -----
// Original Kotlin: formatEditionHistory(editCount)

std::string formatEditionHistory(int editCount) {
    if (editCount <= 0) return "";
    if (editCount == 1) return "Edited 1 time";
    return "Edited " + std::to_string(editCount) + " times";
}

} // namespace progressive
