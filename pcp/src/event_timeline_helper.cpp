#include "pcp/event_timeline_helper.hpp"
#include <sstream>
#include <chrono>
#include <ctime>

namespace pcp {

std::vector<TimelineEventMeta> computeTimelineGroups(
    const std::vector<std::string>& eventIds,
    const std::vector<std::string>& senderIds,
    const std::vector<int64_t>& timestampsMs,
    int64_t maxGapMs) {
    
    std::vector<TimelineEventMeta> result;
    if (eventIds.empty()) return result;
    
    int groupIdx = 0;
    std::string lastSender;
    int64_t lastTs = 0;
    
    for (size_t i = 0; i < eventIds.size(); i++) {
        TimelineEventMeta m;
        m.eventId = eventIds[i];
        m.senderId = i < senderIds.size() ? senderIds[i] : "";
        m.groupIndex = groupIdx;
        
        bool newGroup = (i == 0 || senderIds[i] != lastSender ||
                         (timestampsMs[i] - lastTs > maxGapMs));
        if (newGroup) {
            groupIdx = 0;
            m.showSender = true;
        } else {
            groupIdx++;
            m.showSender = false;
        }
        m.groupIndex = groupIdx;
        m.isGrouped = !newGroup;
        lastSender = senderIds[i];
        lastTs = timestampsMs[i];
        
        result.push_back(m);
    }
    
    // Set position (FIRST/MIDDLE/LAST/SINGLE)
    for (size_t i = 0; i < result.size(); i++) {
        bool prevSame = i > 0 && result[i].senderId == result[i-1].senderId;
        bool nextSame = i + 1 < result.size() && result[i].senderId == result[i+1].senderId;
        if (!prevSame && !nextSame) result[i].position = TimelineEventPosition::SINGLE;
        else if (!prevSame && nextSame) result[i].position = TimelineEventPosition::FIRST;
        else if (prevSame && nextSame) result[i].position = TimelineEventPosition::MIDDLE;
        else result[i].position = TimelineEventPosition::LAST;
        
        // Update group size
        int gs = 1;
        for (size_t j = i + 1; j < result.size() && result[j].senderId == result[i].senderId; j++) gs++;
        result[i].groupSize = gs;
    }
    
    return result;
}

bool shouldShowDateSeparator(int64_t prevTs, int64_t currentTs) {
    time_t prev = prevTs / 1000, curr = currentTs / 1000;
    struct tm tm_prev, tm_curr;
    localtime_r(&prev, &tm_prev);
    localtime_r(&curr, &tm_curr);
    return tm_prev.tm_mday != tm_curr.tm_mday ||
           tm_prev.tm_mon != tm_curr.tm_mon ||
           tm_prev.tm_year != tm_curr.tm_year;
}

bool shouldShowTimeSeparator(int64_t prevTs, int64_t currentTs, int64_t maxGapMs) {
    return (currentTs - prevTs) > maxGapMs;
}

std::string formatTimelineHeader(int64_t ms) {
    time_t t = ms / 1000;
    struct tm tm;
    localtime_r(&t, &tm);
    char buf[64];
    strftime(buf, sizeof(buf), "%B %d, %Y", &tm);
    return buf;
}

std::string formatTimelineTime(int64_t ms) {
    time_t t = ms / 1000;
    struct tm tm;
    localtime_r(&t, &tm);
    char buf[16];
    strftime(buf, sizeof(buf), "%H:%M", &tm);
    return buf;
}

std::string getRelativeTime(int64_t ms) {
    int64_t now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    int64_t diff = now - ms;
    if (diff < 60000) return "just now";
    if (diff < 3600000) return std::to_string(diff / 60000) + " min ago";
    if (diff < 86400000) return std::to_string(diff / 3600000) + "h ago";
    if (diff < 172800000) return "yesterday";
    return formatTimelineHeader(ms);
}

} // namespace pcp
