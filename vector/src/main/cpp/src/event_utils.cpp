#include "progressive/event_utils.hpp"
#include "progressive/json_parser.hpp"
#include <algorithm>
#include <sstream>
#include <chrono>
#include <ctime>
#include <unordered_set>
#include <cmath>

namespace progressive {

// ==== Event Summary ====

std::string formatEventSummary(
    const std::string& eventType,
    const std::string& msgType,
    const std::string& senderName,
    const std::string& body,
    const std::string& membership,
    const std::string& displayName,
    bool isRedacted,
    bool isEncrypted)
{
    if (isRedacted) return "Message removed";

    if (isEncrypted) return senderName.empty() ? "encrypted message" : senderName + ": encrypted message";

    // State events: membership changes
    if (eventType == "m.room.member") {
        std::string prefix = senderName.empty() ? "Someone" : senderName;
        if (membership == "join") {
            return prefix + " joined the room";
        } else if (membership == "invite") {
            return prefix + " was invited";
        } else if (membership == "leave" || membership == "ban") {
            return prefix + " left the room";
        } else if (!displayName.empty()) {
            return prefix + " changed their name to " + displayName;
        }
    }

    // Room state changes
    if (eventType == "m.room.name") {
        return senderName.empty() ? "Room name changed" : senderName + " changed the room name";
    }
    if (eventType == "m.room.topic") {
        return senderName.empty() ? "Topic changed" : senderName + " changed the topic";
    }
    if (eventType == "m.room.avatar") {
        return senderName.empty() ? "Avatar changed" : senderName + " changed the room avatar";
    }
    if (eventType == "m.room.create") {
        return "Room created";
    }
    if (eventType == "m.room.tombstone") {
        return "Room upgraded";
    }
    if (eventType == "m.room.join_rules") {
        return senderName.empty() ? "Join rules changed" : senderName + " changed join rules";
    }
    if (eventType == "m.room.encryption") {
        return senderName.empty() ? "Encryption enabled" : senderName + " enabled encryption";
    }
    if (eventType == "m.room.power_levels") {
        return senderName.empty() ? "Power levels changed" : senderName + " changed power levels";
    }

    // Messages
    if (eventType == "m.room.message" || eventType == "m.sticker") {
        // Format: "Sender: preview" or "Sender sent X"
        if (msgType == "m.text" || msgType == "m.notice") {
            std::string preview = body.size() > 50 ? body.substr(0, 50) + "..." : body;
            return senderName.empty() ? preview : senderName + ": " + preview;
        } else if (msgType == "m.emote") {
            std::string preview = body.size() > 50 ? body.substr(0, 50) + "..." : body;
            return senderName.empty() ? preview : "* " + senderName + " " + preview;
        } else if (msgType == "m.image") {
            return senderName.empty() ? "sent an image" : senderName + " sent an image";
        } else if (msgType == "m.video") {
            return senderName.empty() ? "sent a video" : senderName + " sent a video";
        } else if (msgType == "m.audio") {
            return senderName.empty() ? "sent an audio file" : senderName + " sent a voice message";
        } else if (msgType == "m.file") {
            return senderName.empty() ? "sent a file" : senderName + " sent a file";
        } else if (msgType == "m.location") {
            return senderName.empty() ? "shared their location" : senderName + " shared their location";
        }
    }

    // Sticker
    if (eventType == "m.sticker") {
        return senderName.empty() ? "sent a sticker" : senderName + " sent a sticker";
    }

    // Poll
    if (eventType == "m.poll.start" || eventType == "org.matrix.msc3381.poll.start") {
        return senderName.empty() ? "created a poll" : senderName + " created a poll";
    }
    if (eventType == "m.poll.end" || eventType == "org.matrix.msc3381.poll.end") {
        return senderName.empty() ? "ended a poll" : senderName + " ended a poll";
    }

    // Call
    if (eventType == "m.call.invite") {
        return senderName.empty() ? "started a call" : senderName + " started a call";
    }
    if (eventType == "m.call.hangup") {
        return "Call ended";
    }

    // Key verification
    if (eventType == "m.key.verification.request") {
        return senderName.empty() ? "verification request" : senderName + " wants to verify";
    }
    if (eventType == "m.key.verification.done") {
        return "Verification complete";
    }

    // Beacon / Live location
    if (eventType == "m.beacon_info") {
        return senderName.empty() ? "Live location" : senderName + " is sharing live location";
    }

    // Fallback: show the body if available
    if (!body.empty()) {
        std::string preview = body.size() > 50 ? body.substr(0, 50) + "..." : body;
        return senderName.empty() ? preview : senderName + ": " + preview;
    }

    return senderName.empty() ? "sent a message" : senderName + " sent a message";
}

// ==== Read Marker ====

ReadMarkerPosition calculateReadMarker(
    const std::vector<std::string>& eventIds,
    const std::string& readEventId,
    const std::vector<std::string>& highlightIds)
{
    ReadMarkerPosition result;

    if (eventIds.empty()) return result;

    // Find the position of the read event
    int readIndex = -1;
    if (!readEventId.empty()) {
        for (int i = (int)eventIds.size() - 1; i >= 0; i--) {
            if (eventIds[i] == readEventId) {
                readIndex = i;
                break;
            }
        }
    }

    if (readIndex < 0) {
        // Nothing read -- everything is unread
        result.eventIndex = -1;
        result.unreadCount = (int)eventIds.size();
        result.allRead = false;
    } else if (readIndex == (int)eventIds.size() - 1) {
        // Last event is read -- everything read
        result.eventIndex = -1;
        result.unreadCount = 0;
        result.allRead = true;
    } else {
        result.eventIndex = readIndex;
        result.unreadCount = (int)eventIds.size() - readIndex - 1;
        result.allRead = false;
    }

    // Count highlights after the read marker
    for (const auto& id : highlightIds) {
        int idx = -1;
        for (int i = 0; i < (int)eventIds.size(); i++) {
            if (eventIds[i] == id) { idx = i; break; }
        }
        if (idx > readIndex) result.highlightCount++;
    }

    return result;
}

// ==== Typing Indicator ====

std::string formatTypingIndicator(
    const std::vector<std::string>& typingUserNames,
    int maxNamesShown)
{
    if (typingUserNames.empty()) return "";

    int count = (int)typingUserNames.size();

    if (count == 1) {
        return typingUserNames[0] + " is typing...";
    }

    if (count == 2) {
        return typingUserNames[0] + " and " + typingUserNames[1] + " are typing...";
    }

    int shown = std::min(count, maxNamesShown);
    std::string result;
    for (int i = 0; i < shown; i++) {
        if (i > 0) {
            result += (i == shown - 1 && count <= maxNamesShown) ? " and " : ", ";
        }
        result += typingUserNames[i];
    }

    int remaining = count - shown;
    if (remaining > 0) {
        result += " and " + std::to_string(remaining) + " other";
        if (remaining > 1) result += "s";
    }

    result += (count == 1) ? " is typing..." : " are typing...";
    return result;
}

// ==== Power Level Capabilities ====

PowerLevelCapabilities calculateCapabilities(
    int userPowerLevel,
    int eventsDefault,
    int stateDefault,
    int inviteLevel,
    int kickLevel,
    int banLevel,
    int redactLevel,
    int notifyRoomLevel,
    const std::unordered_map<std::string, int>& eventTypeLevels)
{
    PowerLevelCapabilities c;

    c.isOwner = userPowerLevel >= 100;
    c.isModerator = userPowerLevel >= 50;

    c.canSendMessages = userPowerLevel >= eventsDefault;
    c.canSendState = userPowerLevel >= stateDefault;

    c.canInvite = userPowerLevel >= inviteLevel;
    c.canKick = userPowerLevel >= kickLevel;
    c.canBan = userPowerLevel >= banLevel;
    c.canRedact = userPowerLevel >= redactLevel;
    c.canRedactOthers = userPowerLevel >= 50; // Moderator+

    c.canChangeRoomName = userPowerLevel >= stateDefault;
    c.canChangeRoomTopic = userPowerLevel >= stateDefault;
    c.canChangeRoomAvatar = userPowerLevel >= stateDefault;
    c.canChangeJoinRules = userPowerLevel >= stateDefault;
    c.canChangeHistoryVisibility = userPowerLevel >= stateDefault;
    c.canChangeGuestAccess = userPowerLevel >= stateDefault;

    c.canNotifyEveryone = userPowerLevel >= notifyRoomLevel;

    // Check specific event type overrides
    auto checkOverridden = [&](const std::string& type) -> bool {
        auto it = eventTypeLevels.find(type);
        if (it != eventTypeLevels.end()) return userPowerLevel >= it->second;
        return userPowerLevel >= eventsDefault;
    };

    c.canSendMessages = checkOverridden("m.room.message");
    c.canSendState = std::max(userPowerLevel >= stateDefault, userPowerLevel >= 50);

    return c;
}

// ==== Member Event Notice Formatter ====

std::string formatMemberNotice(
    const std::string& membership,
    const std::string& prevMembership,
    const std::string& senderId,
    const std::string& senderName,
    const std::string& targetUserId,
    const std::string& targetDisplayName,
    const std::string& reason,
    bool isDirectMessage,
    bool sentByCurrentUser)
{
    bool sameUser = (senderId == targetUserId);
    std::string target = (sentByCurrentUser && sameUser) ? "You" : targetDisplayName;
    std::string sender = sentByCurrentUser ? "You" : senderName;
    std::string room = isDirectMessage ? "chat" : "room";

    // Membership change events
    if (membership == "join") {
        if (prevMembership == "invite") {
            if (sameUser) return target + " accepted the invitation to this " + room;
            return target + " accepted the invitation from " + sender;
        }
        if (prevMembership == "ban") {
            return sender + " unbanned " + target;
        }
        if (prevMembership == "knock") {
            return target + " was accepted into the " + room;
        }
        return target + " joined the " + room;
    }
    if (membership == "invite") {
        if (sentByCurrentUser) return "You invited " + target + (reason.empty() ? "" : ": " + reason);
        if (sameUser) return "You were invited to this " + room + (reason.empty() ? "" : ": " + reason);
        return sender + " invited " + target + (reason.empty() ? "" : ": " + reason);
    }
    if (membership == "ban") {
        if (sameUser) return target + " was banned from the " + room + (reason.empty() ? "" : ": " + reason);
        return sender + " banned " + target + (reason.empty() ? "" : ": " + reason);
    }
    if (membership == "leave") {
        if (sentByCurrentUser && sameUser) return "You left the " + room;
        if (sameUser) return target + " left the " + room;
        // Kicked by someone else
        return target + " was kicked by " + sender + (reason.empty() ? "" : ": " + reason);
    }
    if (membership == "knock") {
        if (sameUser) return "You requested to join this " + room + (reason.empty() ? "" : ": " + reason);
        return target + " requested to join the " + room + (reason.empty() ? "" : ": " + reason);
    }

    // Profile change events (no membership change)
    if (membership == "displayname") {
        if (prevMembership == "avatar") return target + " changed their profile picture and display name to " + senderName;
        return target + " changed their display name to " + senderName;
    }
    if (membership == "avatar") {
        if (prevMembership == "displayname") return target + " changed their display name and profile picture";
        return target + " changed their profile picture";
    }

    // Third-party invite
    if (membership == "third_party_invite") {
        return sender + " sent an email invitation to join the " + room;
    }

    return target + " (" + membership + ")";
}

// ==== Call Event Notice Formatter ====

std::string formatCallNotice(
    const std::string& eventType, bool isVideo,
    const std::string& senderName, bool sentByCurrentUser)
{
    std::string who = sentByCurrentUser ? "You" : senderName;
    std::string callType = isVideo ? "video call" : "voice call";

    if (eventType == "m.call.invite") return who + " placed a " + callType;
    if (eventType == "m.call.answer") return who + " answered the call";
    if (eventType == "m.call.hangup") return who + " ended the call";
    if (eventType == "m.call.reject") return who + " declined the call";
    return who + " (" + eventType + ")";
}

// ==== Redaction Notice ====

std::string formatRedactionNotice(const std::string& reason, bool redactedBySameUser, bool isStateEvent) {
    if (isStateEvent) return "This event is no longer available";
    if (redactedBySameUser) return reason.empty() ? "You removed this message" : "You removed this message: " + reason;
    return reason.empty() ? "Message removed" : "Message removed: " + reason;
}

// ==== Edit Annotation ====

std::string annotateEdited(const std::string& body, bool isEdited) {
    if (!isEdited) return body;
    return body + " (edited)";
}

// ==== Room State Notice Formatters ====

std::string formatRoomNameNotice(const std::string& senderName, const std::string& newName, bool sentByCurrentUser) {
    std::string who = sentByCurrentUser ? "You" : senderName;
    if (newName.empty()) return who + " removed the room name";
    return who + " changed the room name to " + newName;
}

std::string formatRoomTopicNotice(const std::string& senderName, const std::string& newTopic, bool sentByCurrentUser) {
    std::string who = sentByCurrentUser ? "You" : senderName;
    if (newTopic.empty()) return who + " removed the topic";
    return who + " changed the topic to: " + newTopic;
}

std::string formatRoomAvatarNotice(const std::string& senderName, bool isRemoved, bool sentByCurrentUser) {
    std::string who = sentByCurrentUser ? "You" : senderName;
    return isRemoved ? who + " removed the room avatar" : who + " changed the room avatar";
}

std::string formatRoomCreateNotice(const std::string& senderName, const std::string& predecessorRoomId, bool isDirect, bool sentByCurrentUser) {
    std::string who = sentByCurrentUser ? "You" : senderName;
    std::string type = isDirect ? "chat" : "room";
    if (!predecessorRoomId.empty()) return who + " upgraded the " + type;
    return who + " created the " + type;
}

std::string formatRoomTombstoneNotice(const std::string& senderName, const std::string& replacementRoom, bool sentByCurrentUser) {
    std::string who = sentByCurrentUser ? "You" : senderName;
    if (!replacementRoom.empty()) return who + " upgraded the room to " + replacementRoom;
    return "This room has been replaced";
}

std::string formatRoomEncryptionNotice(const std::string& senderName, bool isEnabled, bool sentByCurrentUser) {
    std::string who = sentByCurrentUser ? "You" : senderName;
    return isEnabled ? who + " enabled encryption" : who + " disabled encryption";
}

std::string formatPowerLevelNotice(const std::string& senderName, bool sentByCurrentUser) {
    std::string who = sentByCurrentUser ? "You" : senderName;
    return who + " changed power levels";
}

std::string formatPowerLevelDiff(const std::string& senderName,
    const std::unordered_map<std::string, int>& oldLevels,
    const std::unordered_map<std::string, int>& newLevels,
    const std::unordered_map<std::string, std::string>& userNames,
    bool sentByCurrentUser)
{
    std::string who = sentByCurrentUser ? "You" : senderName;
    std::string result = who + " changed power levels";

    // Helper: power level -> role name
    auto roleName = [](int pl) -> std::string {
        if (pl >= 100) return "Admin";
        if (pl >= 50) return "Moderator";
        if (pl >= 0) return "User";
        return "Custom (" + std::to_string(pl) + ")";
    };

    // Find changed users
    std::vector<std::string> diffs;
    for (const auto& kv : newLevels) {
        const auto& uid = kv.first;
        int newPl = kv.second;
        auto oit = oldLevels.find(uid);
        int oldPl = (oit != oldLevels.end()) ? oit->second : 0;
        if (oldPl == newPl) continue; // no change

        auto nit = userNames.find(uid);
        std::string name = (nit != userNames.end()) ? nit->second : uid;
        std::string from = roleName(oldPl);
        std::string to = roleName(newPl);

        if (oldPl == 0 && newPl > 0) diffs.push_back(name + " as " + to);
        else diffs.push_back(name + " from " + from + " to " + to);
    }

    if (!diffs.empty()) {
        result += ": ";
        for (size_t i = 0; i < diffs.size(); i++) {
            if (i > 0) result += ", ";
            result += diffs[i];
        }
    }

    return result;
}

std::string formatJoinRulesNotice(const std::string& senderName, const std::string& newRule, bool sentByCurrentUser) {
    std::string who = sentByCurrentUser ? "You" : senderName;
    return who + " changed join rules to " + newRule;
}

std::string formatHistoryVisibilityNotice(const std::string& senderName, const std::string& newVisibility, bool sentByCurrentUser) {
    std::string who = sentByCurrentUser ? "You" : senderName;
    return who + " changed history visibility to " + newVisibility;
}

std::string formatGuestAccessNotice(const std::string& senderName, bool guestsAllowed, bool sentByCurrentUser) {
    std::string who = sentByCurrentUser ? "You" : senderName;
    return guestsAllowed ? who + " allowed guests to join" : who + " restricted guest access";
}

// ============================================================================
// NEW: Event Batching, Sorting, Filtering & Statistics
// ============================================================================

// ----- JSON helpers (local, NDK 21 compatible, no regex) -----

static std::string localExtractJsonString(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return "";
    pos = json.find(':', pos);
    if (pos == std::string::npos) return "";
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n')) pos++;
    if (pos >= json.size() || json[pos] != '"') return "";
    pos++;
    std::string out;
    while (pos < json.size() && json[pos] != '"') {
        if (json[pos] == '\\' && pos + 1 < json.size()) {
            pos++;
            out += json[pos];
        } else {
            out += json[pos];
        }
        pos++;
    }
    return out;
}

static int64_t localExtractJsonInt64(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return 0;
    pos = json.find(':', pos);
    if (pos == std::string::npos) return 0;
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n')) pos++;
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

static bool localExtractJsonBool(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return false;
    pos = json.find(':', pos);
    if (pos == std::string::npos) return false;
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n')) pos++;
    return json.compare(pos, 4, "true") == 0;
}

// ----- Sort Event List -----
// Original Kotlin: sortEventsBy(sortOrder)

std::vector<std::string> sortEventList(
    const std::vector<std::string>& events,
    EventListSortOrder order)
{
    auto sorted = events;

    switch (order) {
        case EventListSortOrder::CHRONOLOGICAL: {
            std::sort(sorted.begin(), sorted.end(),
                [](const std::string& a, const std::string& b) {
                    return localExtractJsonInt64(a, "origin_server_ts") <
                           localExtractJsonInt64(b, "origin_server_ts");
                });
            break;
        }
        case EventListSortOrder::REVERSE_CHRONOLOGICAL: {
            std::sort(sorted.begin(), sorted.end(),
                [](const std::string& a, const std::string& b) {
                    return localExtractJsonInt64(a, "origin_server_ts") >
                           localExtractJsonInt64(b, "origin_server_ts");
                });
            break;
        }
        case EventListSortOrder::BY_SENDER: {
            std::sort(sorted.begin(), sorted.end(),
                [](const std::string& a, const std::string& b) {
                    std::string sa = localExtractJsonString(a, "sender");
                    std::string sb = localExtractJsonString(b, "sender");
                    if (sa != sb) return sa < sb;
                    return localExtractJsonInt64(a, "origin_server_ts") <
                           localExtractJsonInt64(b, "origin_server_ts");
                });
            break;
        }
        case EventListSortOrder::BY_TYPE: {
            std::sort(sorted.begin(), sorted.end(),
                [](const std::string& a, const std::string& b) {
                    std::string ta = localExtractJsonString(a, "type");
                    std::string tb = localExtractJsonString(b, "type");
                    if (ta != tb) return ta < tb;
                    return localExtractJsonInt64(a, "origin_server_ts") <
                           localExtractJsonInt64(b, "origin_server_ts");
                });
            break;
        }
    }

    return sorted;
}

// ----- Group Events By Sender -----
// Original Kotlin: groupEventsBySender(events)

std::vector<std::pair<std::string, std::vector<std::string>>> groupEventsBySender(
    const std::vector<std::string>& events)
{
    std::vector<std::pair<std::string, std::vector<std::string>>> groups;

    std::string currentSender;
    std::vector<std::string> currentGroup;

    for (const auto& ev : events) {
        std::string sender = localExtractJsonString(ev, "sender");

        if (sender != currentSender && !currentGroup.empty()) {
            groups.emplace_back(currentSender, currentGroup);
            currentGroup.clear();
        }

        currentSender = sender;
        currentGroup.push_back(ev);
    }

    if (!currentGroup.empty()) {
        groups.emplace_back(currentSender, currentGroup);
    }

    return groups;
}

// ----- Group Events By Day -----
// Original Kotlin: groupEventsByDay(events)

std::vector<EventDay> groupEventsByDay(
    const std::vector<std::string>& events,
    int64_t nowMs)
{
    if (nowMs <= 0) {
        nowMs = static_cast<int64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count());
    }

    int64_t dayMs = 86400000LL;
    std::vector<EventDay> days;

    for (const auto& ev : events) {
        int64_t ts = localExtractJsonInt64(ev, "origin_server_ts");
        int64_t localDayStart = nowMs / dayMs * dayMs;
        int64_t evDayStart = ts / dayMs * dayMs;
        int64_t diffDays = (localDayStart - evDayStart) / dayMs;

        std::string dateStr;
        {
            time_t t = ts / 1000;
            struct tm result;
            gmtime_r(&t, &result);
            char buf[16];
            strftime(buf, sizeof(buf), "%F", &result);
            dateStr = buf;
        }

        std::string label;
        if (diffDays == 0) {
            label = "Today";
        } else if (diffDays == 1) {
            label = "Yesterday";
        } else {
            time_t t = ts / 1000;
            struct tm result;
            gmtime_r(&t, &result);
            char buf[16];
            strftime(buf, sizeof(buf), "%b %d", &result);
            label = buf;
        }

        // Find or create the day bucket
        bool found = false;
        for (auto& day : days) {
            if (day.date == dateStr) {
                day.events.push_back(ev);
                found = true;
                break;
            }
        }
        if (!found) {
            EventDay day;
            day.date = dateStr;
            day.label = label;
            day.events.push_back(ev);
            days.push_back(day);
        }
    }

    return days;
}

// ----- Filter Events By Type -----
// Original Kotlin: filterEventsByType(events, types)

std::vector<std::string> filterEventsByType(
    const std::vector<std::string>& events,
    const std::vector<std::string>& types)
{
    if (types.empty()) return events;

    std::vector<std::string> filtered;
    for (const auto& ev : events) {
        std::string t = localExtractJsonString(ev, "type");
        for (const auto& wanted : types) {
            if (t == wanted) {
                filtered.push_back(ev);
                break;
            }
        }
    }
    return filtered;
}

// ----- Filter Events Excluding Redacted -----
// Original Kotlin: filterEventsExcludingRedacted(events)

std::vector<std::string> filterEventsExcludingRedacted(
    const std::vector<std::string>& events)
{
    std::vector<std::string> filtered;
    for (const auto& ev : events) {
        auto redactPos = ev.find("\"redacted_because\"");
        if (redactPos != std::string::npos) {
            // Check if it's actually set and not in a nested structure accidentally
            auto colon = ev.find(':', redactPos);
            if (colon != std::string::npos) {
                for (size_t i = colon + 1; i < ev.size(); i++) {
                    if (ev[i] == '{' || ev[i] == '[') break;
                    if (ev[i] == 'n' || ev[i] == 'N') continue; // skip "null"
                    if (ev[i] != ' ' && ev[i] != '\t' && ev[i] != '\n') {
                        // non-null redacted_because -> skip
                        goto skipEvent;
                    }
                }
            }
        }
        filtered.push_back(ev);
        skipEvent:;
    }
    return filtered;
}

// ----- Get Event Age (ms) -----
// Original Kotlin: getEventAgeMs(event)

int64_t getEventAgeMs(const std::string& eventJson, int64_t nowMs) {
    if (nowMs <= 0) {
        nowMs = static_cast<int64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count());
    }
    int64_t ts = localExtractJsonInt64(eventJson, "origin_server_ts");
    if (ts <= 0) return 0;
    return nowMs - ts;
}

// ----- Is Recent Event -----
// Original Kotlin: isRecentEvent(event, thresholdMs)

bool isRecentEvent(const std::string& eventJson, int64_t thresholdMs, int64_t nowMs) {
    int64_t age = getEventAgeMs(eventJson, nowMs);
    return age >= 0 && age <= thresholdMs;
}

// ----- Format Event Count -----
// Original Kotlin: formatEventCount(count)

std::string formatEventCount(int count) {
    if (count == 1) return "1 message";
    return std::to_string(count) + " messages";
}

// ----- Compute Event Stats -----
// Original Kotlin: computeEventStats(events)

EventStats computeEventStats(const std::vector<std::string>& events) {
    EventStats stats;
    stats.totalEvents = static_cast<int>(events.size());

    std::unordered_set<std::string> senders;

    for (const auto& ev : events) {
        std::string type = localExtractJsonString(ev, "type");

        // Message vs state
        if (type == "m.room.message" || type == "m.sticker" ||
            type.find("m.poll.") == 0 || type.find("m.call.") == 0) {
            stats.messageCount++;
        } else if (type.find("m.room.") == 0 &&
                   type != "m.room.message" &&
                   type != "m.room.encrypted" &&
                   type != "m.room.redaction") {
            stats.stateEventCount++;
        }

        // Encrypted
        if (type == "m.room.encrypted" ||
            localExtractJsonString(ev, "algorithm").find("megolm") != std::string::npos ||
            localExtractJsonString(ev, "algorithm").find("olm") != std::string::npos) {
            stats.encryptedCount++;
        }

        // Redacted
        if (localExtractJsonString(ev, "redacted_because").find("redacts") != std::string::npos) {
            stats.redactedCount++;
        }

        // Unique senders
        std::string sender = localExtractJsonString(ev, "sender");
        if (!sender.empty()) {
            senders.insert(sender);
        }
    }

    stats.senderCount = static_cast<int>(senders.size());
    return stats;
}

// ----- Search Event Text -----
// Original Kotlin: searchEventText(events, query)

std::vector<EventSearchResult> searchEventText(
    const std::vector<std::string>& events,
    const std::string& query,
    int maxResults)
{
    std::vector<EventSearchResult> results;

    if (query.empty()) return results;

    std::string qLower = query;
    for (auto& c : qLower) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));

    for (const auto& ev : events) {
        if (static_cast<int>(results.size()) >= maxResults) break;

        std::string body = localExtractJsonString(ev, "body");
        // Fallback: search content nested object
        if (body.empty()) {
            auto cp = ev.find("\"content\"");
            if (cp != std::string::npos) {
                auto ob = ev.find('{', cp);
                if (ob != std::string::npos) {
                    auto ib = localExtractJsonString(ev.substr(ob), "body");
                    if (!ib.empty()) body = ib;
                }
            }
        }

        std::string bodyLower = body;
        for (auto& c : bodyLower) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));

        auto pos = bodyLower.find(qLower);
        if (pos != std::string::npos) {
            EventSearchResult res;
            res.eventId = localExtractJsonString(ev, "event_id");
            res.roomId = localExtractJsonString(ev, "room_id");
            res.timestamp = localExtractJsonInt64(ev, "origin_server_ts");

            // Extract match text with context
            size_t start = (pos > 30) ? pos - 30 : 0;
            size_t len = std::min(body.size() - start, size_t(80));
            res.matchText = body.substr(pos, query.size());
            res.matchContext = body.substr(start, len);

            results.push_back(res);
        }
    }

    return results;
}

// ============================================================================
// Event JSON Field Extractors
// ============================================================================

// Original Kotlin: extract* from Event JSON

std::string extractEventType(const std::string& eventJson) {
    return localExtractJsonString(eventJson, "type");
}

std::string extractEventSender(const std::string& eventJson) {
    return localExtractJsonString(eventJson, "sender");
}

std::string extractEventRoomId(const std::string& eventJson) {
    return localExtractJsonString(eventJson, "room_id");
}

int64_t extractEventTimestamp(const std::string& eventJson) {
    return localExtractJsonInt64(eventJson, "origin_server_ts");
}

} // namespace progressive
