#include "progressive/event_display.hpp"
#include <sstream>

namespace progressive {

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
    if (eventType == "m.room.redaction" || eventType == "m.room.redaction")
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
        out << " — " << reason;
    }

    return out.str();
}

} // namespace progressive
