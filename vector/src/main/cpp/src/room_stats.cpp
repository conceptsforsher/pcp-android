#include "progressive/room_stats.hpp"
#include <sstream>
#include <unordered_set>
#include <algorithm>
#include <ctime>

namespace progressive {

RoomStats computeRoomStats(
    const std::string& roomId,
    const std::vector<std::string>& eventTypes,
    const std::vector<std::string>& msgTypes,
    const std::vector<std::string>& senders,
    const std::vector<int64_t>& timestamps,
    const std::vector<bool>& encrypted,
    const std::vector<bool>& redacted,
    const std::vector<std::string>& bodies
) {
    RoomStats stats;
    stats.roomId = roomId;
    stats.totalEvents = static_cast<int>(eventTypes.size());

    std::unordered_set<std::string> uniqueSendersSet;
    int64_t totalChars = 0;
    int bodiesWithText = 0;

    for (size_t i = 0; i < eventTypes.size(); ++i) {
        const auto& et = i < eventTypes.size() ? eventTypes[i] : "";

        if (et == "m.room.message" || et == "m.room.encrypted") {
            const auto& mt = i < msgTypes.size() ? msgTypes[i] : "";
            if (mt == "m.text" || mt.empty()) stats.textMessages++;
            else if (mt == "m.image") stats.imageMessages++;
            else if (mt == "m.video") stats.videoMessages++;
            else if (mt == "m.file") stats.fileMessages++;
            else if (mt == "m.audio") stats.audioMessages++;
            else if (mt == "m.emote") stats.emoteMessages++;
        }

        if (et == "m.reaction") stats.reactionEvents++;
        if (et == "m.room.member") stats.memberEvents++;

        if (i < encrypted.size() && encrypted[i]) stats.encryptedEvents++;
        if (i < redacted.size() && redacted[i]) stats.redactedEvents++;

        if (i < senders.size()) uniqueSendersSet.insert(senders[i]);

        const auto& body = i < bodies.size() ? bodies[i] : "";
        if (!body.empty()) {
            totalChars += body.size();
            bodiesWithText++;
        }
    }

    stats.uniqueSenders = static_cast<int>(uniqueSendersSet.size());
    stats.avgMessageLength = bodiesWithText > 0 ? static_cast<int>(totalChars / bodiesWithText) : 0;
    stats.totalBodyChars = totalChars;

    if (!timestamps.empty()) {
        stats.firstEventTs = *std::min_element(timestamps.begin(), timestamps.end());
        stats.lastEventTs = *std::max_element(timestamps.begin(), timestamps.end());
        stats.messagesPerDay = computeMessagesPerDay(stats.totalEvents, stats.firstEventTs, stats.lastEventTs);

        auto [hour, count] = findBusiestHour(timestamps);
        std::ostringstream hourStr;
        hourStr << (hour < 10 ? "0" : "") << hour << ":00"
                << "-" << (hour + 1 < 10 ? "0" : "") << (hour + 1) << ":00";
        stats.busiestHour = hourStr.str();
        stats.busiestHourCount = count;
    }

    return stats;
}

double computeMessagesPerDay(int eventCount, int64_t firstTs, int64_t lastTs) {
    if (firstTs <= 0 || lastTs <= firstTs) return 0.0;
    double days = static_cast<double>(lastTs - firstTs) / (1000.0 * 86400.0);
    if (days < 1.0) days = 1.0;
    return static_cast<double>(eventCount) / days;
}

std::pair<int, int> findBusiestHour(const std::vector<int64_t>& timestamps) {
    std::vector<int> hourCounts(24, 0);
    for (int64_t ts : timestamps) {
        if (ts <= 0) continue;
        time_t t = ts / 1000;
        struct tm result;
        gmtime_r(&t, &result);
        hourCounts[result.tm_hour]++;
    }

    int maxHour = 0, maxCount = 0;
    for (int h = 0; h < 24; ++h) {
        if (hourCounts[h] > maxCount) {
            maxCount = hourCounts[h];
            maxHour = h;
        }
    }
    return {maxHour, maxCount};
}

std::string roomStatsToJson(const RoomStats& stats) {
    std::ostringstream json;
    json << "{";
    json << R"("roomId": ")" << stats.roomId << R"(",)";
    json << R"("totalEvents": )" << stats.totalEvents << ",";
    json << R"("textMessages": )" << stats.textMessages << ",";
    json << R"("imageMessages": )" << stats.imageMessages << ",";
    json << R"("videoMessages": )" << stats.videoMessages << ",";
    json << R"("fileMessages": )" << stats.fileMessages << ",";
    json << R"("audioMessages": )" << stats.audioMessages << ",";
    json << R"("reactionEvents": )" << stats.reactionEvents << ",";
    json << R"("memberEvents": )" << stats.memberEvents << ",";
    json << R"("encryptedEvents": )" << stats.encryptedEvents << ",";
    json << R"("messagesPerDay": )" << stats.messagesPerDay << ",";
    json << R"("uniqueSenders": )" << stats.uniqueSenders << ",";
    json << R"("busiestHour": ")" << stats.busiestHour << R"(",)";
    json << R"("avgMessageLength": )" << stats.avgMessageLength;
    json << "}";
    return json.str();
}

std::string roomStatsToText(const RoomStats& stats) {
    std::ostringstream out;
    out << "Room Statistics\n";
    out << "===============\n";
    out << "Total events: " << stats.totalEvents << "\n";
    out << "Text: " << stats.textMessages << "  Images: " << stats.imageMessages
        << "  Video: " << stats.videoMessages << "  Files: " << stats.fileMessages << "\n";
    out << "Reactions: " << stats.reactionEvents << "  Members: " << stats.memberEvents << "\n";
    out << "Encrypted: " << stats.encryptedEvents << "  Redacted: " << stats.redactedEvents << "\n";
    out << "Unique senders: " << stats.uniqueSenders << "\n";
    out << "Messages/day: " << stats.messagesPerDay << "\n";
    out << "Busiest hour: " << stats.busiestHour << " (" << stats.busiestHourCount << " msgs)\n";
    out << "Avg message length: " << stats.avgMessageLength << " chars\n";
    return out.str();
}

} // namespace progressive
