#include "progressive/event_timeline.hpp"
#include "progressive/event_classifier.hpp"
#include <sstream>
#include <algorithm>
#include <ctime>
#include <unordered_set>

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

// ---- Timeline Gap Detection ----

double computeAvgIntervalMs(const std::vector<int64_t>& timestamps) {
    if (timestamps.size() < 2) return 0.0;
    int64_t totalInterval = 0;
    for (size_t i = 1; i < timestamps.size(); ++i) {
        totalInterval += timestamps[i] - timestamps[i - 1];
    }
    return static_cast<double>(totalInterval) / (timestamps.size() - 1);
}

int estimateMissingEvents(int64_t gapDurationMs, double avgIntervalMs) {
    if (avgIntervalMs <= 0) return 0;
    return static_cast<int>(gapDurationMs / avgIntervalMs);
}

TimelineStats analyzeTimeline(const std::string& roomId,
    const std::vector<std::string>& eventIds,
    const std::vector<int64_t>& timestamps,
    bool hasMoreBackward, bool hasMoreForward,
    int64_t maxGapMs
) {
    TimelineStats stats;
    stats.roomId = roomId;
    stats.totalEvents = static_cast<int>(eventIds.size());
    stats.hasMoreBackward = hasMoreBackward;
    stats.hasMoreForward = hasMoreForward;

    if (eventIds.size() < 2) return stats;

    stats.coverageStartMs = timestamps.front();
    stats.coverageEndMs = timestamps.back();

    double avgInterval = computeAvgIntervalMs(timestamps);

    for (size_t i = 1; i < timestamps.size(); ++i) {
        int64_t gapDuration = timestamps[i] - timestamps[i - 1];
        if (gapDuration > maxGapMs) {
            TimelineGap gap;
            gap.gapStartMs = timestamps[i - 1];
            gap.gapEndMs = timestamps[i];
            gap.gapDurationMs = gapDuration;
            gap.missingEventsEstimate = estimateMissingEvents(gapDuration, avgInterval);
            gap.prevEventId = i > 0 ? eventIds[i - 1] : "";
            gap.nextEventId = eventIds[i];
            stats.gaps.push_back(gap);
        }
    }

    stats.totalGaps = static_cast<int>(stats.gaps.size());

    if (stats.coverageEndMs > stats.coverageStartMs) {
        int64_t totalMs = stats.coverageEndMs - stats.coverageStartMs;
        int64_t gapMs = 0;
        for (const auto& g : stats.gaps) gapMs += g.gapDurationMs;
        stats.coveragePercent = 100.0 * (1.0 - static_cast<double>(gapMs) / totalMs);
    }

    return stats;
}

// ---- Event Grouping ----

std::vector<EventGroup> groupEventsByDate(const std::vector<int64_t>& timestamps) {
    std::vector<EventGroup> groups;

    for (int64_t ts : timestamps) {
        if (ts <= 0) continue;
        time_t t = ts / 1000;
        struct tm result;
        gmtime_r(&t, &result);

        char keyBuf[16];
        strftime(keyBuf, sizeof(keyBuf), "%Y-%m-%d", &result);
        std::string key(keyBuf);

        if (groups.empty() || groups.back().groupKey != key) {
            EventGroup group;
            group.groupKey = key;
            group.startMs = ts;
            group.label = formatGroupLabel(ts);
            groups.push_back(group);
        }
        auto& last = groups.back();
        last.eventCount++;
        last.endMs = ts;
    }

    return groups;
}

std::string formatGroupLabel(int64_t timestampMs) {
    if (timestampMs <= 0) return "";
    time_t now = time(nullptr);
    time_t ts = timestampMs / 1000;
    struct tm nowTm, tsTm;
    gmtime_r(&now, &nowTm);
    gmtime_r(&ts, &tsTm);

    if (nowTm.tm_year == tsTm.tm_year && nowTm.tm_mon == tsTm.tm_mon && nowTm.tm_mday == tsTm.tm_mday) {
        return "Today";
    }

    time_t yesterday = now - 86400;
    struct tm yestTm;
    gmtime_r(&yesterday, &yestTm);
    if (yestTm.tm_year == tsTm.tm_year && yestTm.tm_mon == tsTm.tm_mon && yestTm.tm_mday == tsTm.tm_mday) {
        return "Yesterday";
    }

    char buf[32];
    if (nowTm.tm_year == tsTm.tm_year) {
        strftime(buf, sizeof(buf), "%B %d", &tsTm);
    } else {
        strftime(buf, sizeof(buf), "%B %d, %Y", &tsTm);
    }
    return std::string(buf);
}

std::string formatGroupLabel(const EventGroup& group) {
    return group.label;
}

// ---- Read Marker ----

ReadMarker computeReadMarker(
    const std::string& lastReadEventId,
    const std::vector<std::string>& eventIds,
    const std::vector<int64_t>& timestamps,
    const std::vector<bool>& isMention,
    const std::string& myUserId
) {
    ReadMarker marker;
    marker.eventId = lastReadEventId;

    size_t readPos = 0;
    for (size_t i = 0; i < eventIds.size(); ++i) {
        if (eventIds[i] == lastReadEventId) {
            readPos = i;
            if (i < timestamps.size()) marker.positionMs = timestamps[i];
            break;
        }
    }

    for (size_t i = readPos + 1; i < eventIds.size(); ++i) {
        marker.unreadCount++;
        if (i < isMention.size() && isMention[i]) {
            marker.hasUnreadMentions = true;
        }
    }

    return marker;
}

std::string timelineStatsToJson(const TimelineStats& stats) {
    std::ostringstream json;
    json << "{";
    json << R"("roomId": ")" << jsonEscape(stats.roomId) << R"(",)";
    json << R"("totalEvents": )" << stats.totalEvents << ",";
    json << R"("totalGaps": )" << stats.totalGaps << ",";
    json << R"("coveragePercent": )" << stats.coveragePercent << ",";
    json << R"("gaps": [)";
    for (size_t i = 0; i < stats.gaps.size(); ++i) {
        if (i > 0) json << ",";
        const auto& g = stats.gaps[i];
        json << R"({"gapDurationMs": )" << g.gapDurationMs;
        json << R"(,"missingEstimate": )" << g.missingEventsEstimate;
        json << R"(,"prevEventId": ")" << jsonEscape(g.prevEventId) << R"(")";
        json << R"(,"nextEventId": ")" << jsonEscape(g.nextEventId) << R"(")" << "}";
    }
    json << "]}";
    return json.str();
}

// ==== Enum to String ====

const char* insertModeToString(InsertMode m) {
    switch (m) {
        case InsertMode::APPEND:  return "APPEND";
        case InsertMode::PREPEND: return "PREPEND";
        case InsertMode::REPLACE: return "REPLACE";
    }
    return "APPEND";
}

InsertMode insertModeFromString(const std::string& s) {
    if (s == "APPEND")  return InsertMode::APPEND;
    if (s == "PREPEND") return InsertMode::PREPEND;
    if (s == "REPLACE") return InsertMode::REPLACE;
    return InsertMode::APPEND;
}

// ==== buildTimelineSnapshot — build display-ready timeline from events ====
//
// Original Kotlin: DefaultTimeline.strategy.buildSnapshot()

TimelineSnapshot buildTimelineSnapshot(
    const std::vector<std::string>& eventJsons,
    const std::string& myUserId,
    bool hasMoreForward,
    bool hasMoreBackward,
    const std::string& readMarkerEventId,
    const std::string& fullyReadEventId)
{
    TimelineSnapshot snap;
    snap.hasMoreForward = hasMoreForward;
    snap.hasMoreBackward = hasMoreBackward;
    snap.readMarkerEventId = readMarkerEventId;
    snap.fullyReadEventId = fullyReadEventId;

    for (const auto& json : eventJsons) {
        if (json.empty() || json == "{}") continue;

        std::string eventId = extractJsonString(json, "event_id");
        if (eventId.empty()) continue;

        std::string eventType = extractJsonString(json, "type");
        std::string contentJson = extractJsonObject(json, "content");
        std::string senderId = extractJsonString(json, "sender");
        int64_t originTs = extractJsonInt64(json, "origin_server_ts");
        std::string unsignedJson = extractJsonObject(json, "unsigned");
        bool isEncrypted = (eventType == "m.room.encrypted");
        bool isFromMe = (senderId == myUserId);

        // Resolve sender display name from content or defaults
        std::string senderName = senderId;
        std::string senderAvatar = "";

        EventDisplayInfo info = formatEventForDisplay(
            eventId, eventType, contentJson, senderId, senderName, senderAvatar,
            originTs, unsignedJson, isEncrypted, isFromMe);

        // Ensure senderName is populated
        if (info.senderName.empty()) info.senderName = senderId;
        if (info.senderAvatar.empty()) info.senderAvatar = senderAvatar;

        snap.events.push_back(info);
    }

    snap.totalEventCount = static_cast<int>(snap.events.size());

    if (!snap.events.empty()) {
        snap.oldestEventId = snap.events.front().eventId;
        snap.newestEventId = snap.events.back().eventId;
        snap.oldestTimestamp = snap.events.front().originServerTs;
        snap.newestTimestamp = snap.events.back().originServerTs;
    }

    return snap;
}

// ==== computeTimelineDisplayItems — convert raw events to display items with grouping ====
//
// Original Kotlin: TimelineChunk.builtItems() → displayItems

std::vector<TimelineDisplayItem> computeTimelineDisplayItems(
    const std::vector<EventDisplayInfo>& events,
    const std::string& readMarkerEventId)
{
    std::vector<TimelineDisplayItem> items;
    if (events.empty()) return items;

    bool foundReadMarker = false;

    for (size_t i = 0; i < events.size(); i++) {
        TimelineDisplayItem item;
        item.event = events[i];
        item.displayInfo = events[i];
        item.index = static_cast<int>(i);

        // Determine grouping with previous event
        if (i > 0) {
            const auto& prev = events[i - 1];
            auto groupInfo = getEventGroupInfo(
                events[i].eventType, events[i].senderId, prev.senderId,
                events[i].originServerTs, prev.originServerTs);
            item.isGrouped = groupInfo.isGroupedWithPrevious;
            item.showSender = groupInfo.showSender;
            item.showTimestamp = groupInfo.showTimestamp;
        } else {
            item.isGrouped = false;
            item.showSender = true;
            item.showTimestamp = true;
        }

        // Determine group position (0=first, 1=middle, 2=last, 3=single)
        bool hasNextSameSender = false;
        if (i + 1 < events.size()) {
            const auto& next = events[i + 1];
            hasNextSameSender = (next.senderId == events[i].senderId);
        }

        if (item.isGrouped && hasNextSameSender) {
            item.groupPosition = 1;  // middle
        } else if (item.isGrouped && !hasNextSameSender) {
            item.groupPosition = 2;  // last
        } else if (!item.isGrouped && hasNextSameSender) {
            item.groupPosition = 0;  // first
        } else {
            item.groupPosition = 3;  // single
        }

        // Check for read marker position
        if (!foundReadMarker && !readMarkerEventId.empty() &&
            events[i].eventId == readMarkerEventId) {
            foundReadMarker = true;
        }
        item.isReadMarker = (!foundReadMarker && !readMarkerEventId.empty() &&
                             i == 0) ||
                            (foundReadMarker && i > 0 &&
                             events[i - 1].eventId == readMarkerEventId);

        // Check for new day separator
        if (i > 0) {
            time_t cur = events[i].originServerTs / 1000;
            time_t prev = events[i - 1].originServerTs / 1000;
            struct tm curTm, prevTm;
            gmtime_r(&cur, &curTm);
            gmtime_r(&prev, &prevTm);
            if (curTm.tm_year != prevTm.tm_year ||
                curTm.tm_mon != prevTm.tm_mon ||
                curTm.tm_mday != prevTm.tm_mday) {
                item.isNewDay = true;
                item.dateSeparator = formatTimelineDate(events[i].originServerTs);
            }
        } else {
            item.isNewDay = true;
            if (events[i].originServerTs > 0) {
                item.dateSeparator = formatTimelineDate(events[i].originServerTs);
            }
        }

        items.push_back(item);
    }

    return items;
}

// ==== insertDateSeparators — add date separators between days ====
//
// Original Kotlin: DateSeparatorProcessor.insertDateHeaders(items)

std::vector<TimelineDisplayItem> insertDateSeparators(
    const std::vector<TimelineDisplayItem>& items)
{
    std::vector<TimelineDisplayItem> result;

    for (const auto& item : items) {
        if (item.isNewDay && !item.dateSeparator.empty()) {
            TimelineDisplayItem sep;
            sep.dateSeparator = item.dateSeparator;
            sep.isNewDay = true;
            sep.event.eventId = "__date_sep__";
            result.push_back(sep);
        }
        result.push_back(item);
    }

    return result;
}

// ==== formatTimelineDate — date headings ====
//
// Original Kotlin: DateFormatter.formatForTimelineHeader(ts)
// Returns: "Today", "Yesterday", "Monday", "12 Jan 2023"

std::string formatTimelineDate(int64_t epochMs) {
    if (epochMs <= 0) return "";

    time_t t = epochMs / 1000;
    struct tm tmVal;
    gmtime_r(&t, &tmVal);

    time_t now = time(nullptr);
    struct tm nowTm;
    gmtime_r(&now, &nowTm);

    // Today
    if (nowTm.tm_year == tmVal.tm_year &&
        nowTm.tm_mon == tmVal.tm_mon &&
        nowTm.tm_mday == tmVal.tm_mday) {
        return "Today";
    }

    // Yesterday
    time_t yesterday = now - 86400;
    struct tm yestTm;
    gmtime_r(&yesterday, &yestTm);
    if (yestTm.tm_year == tmVal.tm_year &&
        yestTm.tm_mon == tmVal.tm_mon &&
        yestTm.tm_mday == tmVal.tm_mday) {
        return "Yesterday";
    }

    // This week: day name
    int64_t diffSec = static_cast<int64_t>(difftime(now, t));
    int64_t diffDay = diffSec / 86400;
    if (diffDay < 7 && nowTm.tm_year == tmVal.tm_year) {
        char dayBuf[16];
        strftime(dayBuf, sizeof(dayBuf), "%A", &tmVal);
        return std::string(dayBuf);
    }

    // This year: 12 Jan
    if (nowTm.tm_year == tmVal.tm_year) {
        char buf[16];
        strftime(buf, sizeof(buf), "%d %b", &tmVal);
        return std::string(buf);
    }

    // Older: 12 Jan 2023
    char buf[32];
    strftime(buf, sizeof(buf), "%d %b %Y", &tmVal);
    return std::string(buf);
}

// ==== getTimelineSections — group into sections ====
//
// Original Kotlin: TimelineSectionGrouper.group(items)
// Groups events into: new messages (after read marker), today, yesterday, older.

std::vector<TimelineSection> getTimelineSections(
    const std::vector<TimelineDisplayItem>& items)
{
    std::vector<TimelineSection> sections;
    if (items.empty()) return sections;

    TimelineSection current;
    std::string currentDateLabel;
    int unreadStart = -1;

    // Find read marker to split new messages
    for (size_t i = 0; i < items.size(); i++) {
        if (items[i].isReadMarker) {
            unreadStart = static_cast<int>(i);
            break;
        }
    }

    // New messages section (if unread)
    if (unreadStart >= 0 && unreadStart < static_cast<int>(items.size())) {
        TimelineSection newMsgs;
        newMsgs.title = "New Messages";
        newMsgs.isNewMessages = true;
        for (size_t i = static_cast<size_t>(unreadStart); i < items.size(); i++) {
            newMsgs.events.push_back(items[i].event);
        }
        if (!newMsgs.events.empty()) {
            sections.push_back(newMsgs);
        }
    }

    // Date-based sections for events before the read marker
    int endIdx = unreadStart >= 0 ? unreadStart : static_cast<int>(items.size());
    for (int i = 0; i < endIdx; i++) {
        std::string dateLabel;
        if (items[i].originServerTs > 0 || items[i].event.originServerTs > 0) {
            int64_t ts = items[i].event.originServerTs > 0 ?
                         items[i].event.originServerTs : items[i].originServerTs;
            if (ts > 0) dateLabel = formatTimelineDate(ts);
        }

        if (currentDateLabel != dateLabel) {
            if (!current.events.empty()) {
                sections.push_back(current);
            }
            current = TimelineSection();
            current.title = dateLabel;
            currentDateLabel = dateLabel;
        }
        current.events.push_back(items[i].event);
    }
    if (!current.events.empty()) {
        sections.push_back(current);
    }

    return sections;
}

// ==== isReadMarkerPosition — compute where read marker goes ====
//
// Original Kotlin: ReadMarkersTracker.getReadMarkerPosition()

int isReadMarkerPosition(
    const std::vector<std::string>& eventIds,
    const std::string& readMarkerEventId)
{
    if (readMarkerEventId.empty()) return -1;

    for (int i = 0; i < static_cast<int>(eventIds.size()); i++) {
        if (eventIds[i] == readMarkerEventId) {
            // Read marker goes after the last read event
            if (i + 1 < static_cast<int>(eventIds.size())) return i + 1;
            return i; // Last event is the read marker
        }
    }

    // Read marker not in loaded events — place at start (nothing read)
    return 0;
}

// ==== computeInsertMode — determine insert mode from pagination direction ====
//
// Original Kotlin: PaginationDirection enum mapping
// direction="b" (backwards) → PREPEND
// direction="f" (forwards) → APPEND
// initial load / eventId lookup → REPLACE

InsertMode computeInsertMode(const std::string& direction,
    bool isInitialLoad)
{
    if (isInitialLoad) return InsertMode::REPLACE;
    if (direction == "b") return InsertMode::PREPEND;
    if (direction == "f") return InsertMode::APPEND;
    return InsertMode::APPEND;
}

// ==== JSON Serialization ====

std::string timelineSnapshotToJson(const TimelineSnapshot& ts) {
    std::ostringstream j;
    j << "{";
    j << "\"events\":[";
    for (size_t i = 0; i < ts.events.size(); i++) {
        if (i > 0) j << ",";
        j << eventDisplayInfoToJson(ts.events[i]);
    }
    j << "],";
    j << "\"hasMoreForward\":" << (ts.hasMoreForward ? "true" : "false") << ",";
    j << "\"hasMoreBackward\":" << (ts.hasMoreBackward ? "true" : "false") << ",";
    j << "\"totalEventCount\":" << ts.totalEventCount << ",";
    j << "\"oldestEventId\":\"" << jsonEscape(ts.oldestEventId) << "\",";
    j << "\"newestEventId\":\"" << jsonEscape(ts.newestEventId) << "\",";
    j << "\"readMarkerEventId\":\"" << jsonEscape(ts.readMarkerEventId) << "\",";
    j << "\"fullyReadEventId\":\"" << jsonEscape(ts.fullyReadEventId) << "\",";
    j << "\"oldestTimestamp\":" << ts.oldestTimestamp << ",";
    j << "\"newestTimestamp\":" << ts.newestTimestamp;
    j << "}";
    return j.str();
}

TimelineSnapshot timelineSnapshotFromJson(const std::string& json) {
    TimelineSnapshot ts;
    ts.hasMoreForward = extractJsonBool(json, "hasMoreForward");
    ts.hasMoreBackward = extractJsonBool(json, "hasMoreBackward");
    ts.totalEventCount = static_cast<int>(extractJsonInt64(json, "totalEventCount"));
    ts.oldestEventId = extractJsonString(json, "oldestEventId");
    ts.newestEventId = extractJsonString(json, "newestEventId");
    ts.readMarkerEventId = extractJsonString(json, "readMarkerEventId");
    ts.fullyReadEventId = extractJsonString(json, "fullyReadEventId");
    ts.oldestTimestamp = extractJsonInt64(json, "oldestTimestamp");
    ts.newestTimestamp = extractJsonInt64(json, "newestTimestamp");

    auto arrPos = json.find("\"events\"");
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
                        if (json[arrPos] == '{') d++;
                        else if (json[arrPos] == '}') d--;
                        arrPos++;
                    }
                    ts.events.push_back(eventDisplayInfoFromJson(json.substr(start, arrPos - start)));
                }
            }
        }
    }

    return ts;
}

std::string timelineDisplayItemToJson(const TimelineDisplayItem& item) {
    std::ostringstream j;
    j << "{";
    j << "\"event\":" << eventDisplayInfoToJson(item.event) << ",";
    j << "\"displayInfo\":" << eventDisplayInfoToJson(item.displayInfo) << ",";
    j << "\"isGrouped\":" << (item.isGrouped ? "true" : "false") << ",";
    j << "\"groupPosition\":" << item.groupPosition << ",";
    j << "\"showSender\":" << (item.showSender ? "true" : "false") << ",";
    j << "\"showTimestamp\":" << (item.showTimestamp ? "true" : "false") << ",";
    j << "\"isReadMarker\":" << (item.isReadMarker ? "true" : "false") << ",";
    j << "\"isNewDay\":" << (item.isNewDay ? "true" : "false") << ",";
    j << "\"dateSeparator\":\"" << jsonEscape(item.dateSeparator) << "\",";
    j << "\"index\":" << item.index;
    j << "}";
    return j.str();
}

TimelineDisplayItem timelineDisplayItemFromJson(const std::string& json) {
    TimelineDisplayItem item;
    item.event = eventDisplayInfoFromJson(extractJsonObject(json, "event"));
    item.displayInfo = eventDisplayInfoFromJson(extractJsonObject(json, "displayInfo"));
    item.isGrouped = extractJsonBool(json, "isGrouped");
    item.groupPosition = static_cast<int>(extractJsonInt64(json, "groupPosition"));
    item.showSender = extractJsonBool(json, "showSender");
    item.showTimestamp = extractJsonBool(json, "showTimestamp");
    item.isReadMarker = extractJsonBool(json, "isReadMarker");
    item.isNewDay = extractJsonBool(json, "isNewDay");
    item.dateSeparator = extractJsonString(json, "dateSeparator");
    item.index = static_cast<int>(extractJsonInt64(json, "index"));
    return item;
}

std::string timelineDateSeparatorToJson(const TimelineDateSeparator& ds) {
    std::ostringstream j;
    j << "{";
    j << "\"dateString\":\"" << jsonEscape(ds.dateString) << "\",";
    j << "\"position\":" << ds.position;
    j << "}";
    return j.str();
}

TimelineDateSeparator timelineDateSeparatorFromJson(const std::string& json) {
    TimelineDateSeparator ds;
    ds.dateString = extractJsonString(json, "dateString");
    ds.position = static_cast<int>(extractJsonInt64(json, "position"));
    return ds;
}

std::string timelineSectionToJson(const TimelineSection& ts) {
    std::ostringstream j;
    j << "{";
    j << "\"title\":\"" << jsonEscape(ts.title) << "\",";
    j << "\"isNewMessages\":" << (ts.isNewMessages ? "true" : "false") << ",";
    j << "\"events\":[";
    for (size_t i = 0; i < ts.events.size(); i++) {
        if (i > 0) j << ",";
        j << eventDisplayInfoToJson(ts.events[i]);
    }
    j << "]}";
    return j.str();
}

TimelineSection timelineSectionFromJson(const std::string& json) {
    TimelineSection ts;
    ts.title = extractJsonString(json, "title");
    ts.isNewMessages = extractJsonBool(json, "isNewMessages");

    auto arrPos = json.find("\"events\"");
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
                        if (json[arrPos] == '{') d++;
                        else if (json[arrPos] == '}') d--;
                        arrPos++;
                    }
                    ts.events.push_back(eventDisplayInfoFromJson(json.substr(start, arrPos - start)));
                }
            }
        }
    }

    return ts;
}

std::string insertModeToJson(InsertMode m) {
    return "\"" + std::string(insertModeToString(m)) + "\"";
}

InsertMode insertModeFromJson(const std::string& json) {
    std::string s = json;
    if (!s.empty() && s.front() == '"') s = s.substr(1);
    if (!s.empty() && s.back() == '"') s.pop_back();
    return insertModeFromString(s);
}

} // namespace progressive
