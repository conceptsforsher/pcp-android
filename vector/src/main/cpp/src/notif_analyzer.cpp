#include "progressive/notif_analyzer.hpp"
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include <ctime>

namespace progressive {

NotifAnalytics analyzeNotifications(const std::vector<NotifEvent>& events) {
    NotifAnalytics a;
    a.byHour.resize(24, 0);
    a.byDay.resize(7, 0);

    if (events.empty()) return a;

    std::unordered_map<std::string, int> roomCount;
    std::unordered_map<std::string, int> senderCount;

    a.firstSeenMs = events[0].timestampMs;
    a.lastSeenMs = events[0].timestampMs;

    for (const auto& e : events) {
        a.totalNotifications++;

        // Classify
        if (e.type == "message") a.messages++;
        else if (e.type == "mention") a.mentions++;
        else if (e.type == "invite") a.invites++;
        else if (e.type == "call") a.calls++;
        else if (e.type == "reaction") a.reactions++;

        if (e.isNoisy) a.noisyCount++;
        if (e.isHighlight) a.highlightCount++;

        // Time distribution
        if (e.timestampMs > 0) {
            time_t ts = e.timestampMs / 1000;
            struct tm result;
            gmtime_r(&ts, &result);
            a.byHour[result.tm_hour]++;
            a.byDay[result.tm_wday]++;

            if (e.timestampMs < a.firstSeenMs) a.firstSeenMs = e.timestampMs;
            if (e.timestampMs > a.lastSeenMs) a.lastSeenMs = e.timestampMs;
        }

        roomCount[e.roomName.empty() ? e.roomId : e.roomName]++;
        senderCount[e.senderName]++;
    }

    // Peak hour
    for (int i = 0; i < 24; ++i) {
        if (a.byHour[i] > a.byHour[a.peakHour]) a.peakHour = i;
    }

    // Peak day
    for (int i = 0; i < 7; ++i) {
        if (a.byDay[i] > a.byDay[a.peakDay]) a.peakDay = i;
    }

    // Top rooms
    for (const auto& p : roomCount) a.topRooms.push_back(p);
    std::sort(a.topRooms.begin(), a.topRooms.end(),
        [](const auto& x, const auto& y) { return x.second > y.second; });
    if (a.topRooms.size() > 5) a.topRooms.resize(5);

    // Top senders
    for (const auto& p : senderCount) a.topSenders.push_back(p);
    std::sort(a.topSenders.begin(), a.topSenders.end(),
        [](const auto& x, const auto& y) { return x.second > y.second; });
    if (a.topSenders.size() > 5) a.topSenders.resize(5);

    // Rates
    if (a.lastSeenMs > a.firstSeenMs) {
        double hours = static_cast<double>(a.lastSeenMs - a.firstSeenMs) / (1000.0 * 3600.0);
        if (hours > 0) {
            a.avgPerHour = a.totalNotifications / hours;
            a.avgPerDay = a.avgPerHour * 24.0;
        }
    }

    return a;
}

std::string classifyNotifEvent(const std::string& body, bool isMention,
    bool isInvite, bool isCall, bool isReaction) {
    if (isCall) return "call";
    if (isInvite) return "invite";
    if (isMention) return "mention";
    if (isReaction) return "reaction";
    return "message";
}

std::pair<int, int> suggestQuietHours(const NotifAnalytics& a) {
    // Find the least busy contiguous 8-hour window
    int bestStart = 22;
    int bestScore = INT32_MAX;

    for (int start = 0; start < 24; ++start) {
        int score = 0;
        for (int h = 0; h < 8; ++h) {
            score += a.byHour[(start + h) % 24];
        }
        if (score < bestScore) {
            bestScore = score;
            bestStart = start;
        }
    }

    return {bestStart, (bestStart + 8) % 24};
}

std::string getBusiestDayName(const NotifAnalytics& a) {
    static const char* days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    return days[a.peakDay];
}

std::string notifAnalyticsToJson(const NotifAnalytics& a) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << "{";
    json << R"("total": )" << a.totalNotifications << ",";
    json << R"("messages": )" << a.messages << ",";
    json << R"("mentions": )" << a.mentions << ",";
    json << R"("noisy": )" << a.noisyCount << ",";
    json << R"("peakHour": )" << a.peakHour << ",";
    json << R"("avgPerHour": )" << a.avgPerHour << ",";
    json << R"("avgPerDay": )" << a.avgPerDay << ",";
    json << R"("busiestDay": ")" << getBusiestDayName(a) << R"(",)";
    json << R"("topRooms": [)";
    for (size_t i = 0; i < a.topRooms.size(); ++i) {
        if (i > 0) json << ",";
        json << R"({"name": ")" << esc(a.topRooms[i].first) << R"(")";
        json << R"(,"count": )" << a.topRooms[i].second << "}";
    }
    json << "]}";
    return json.str();
}

std::string notifAnalyticsToText(const NotifAnalytics& a) {
    std::ostringstream out;
    out << "Notification Analytics\n";
    out << "======================\n";
    out << "Total: " << a.totalNotifications << " (msgs: " << a.messages
        << ", mentions: " << a.mentions << ", noisy: " << a.noisyCount << ")\n";
    out << "Avg: " << a.avgPerHour << "/hour, " << a.avgPerDay << "/day\n";
    out << "Peak hour: " << a.peakHour << ":00, Busiest day: " << getBusiestDayName(a) << "\n";
    out << "Top rooms:\n";
    for (const auto& p : a.topRooms) {
        out << "  " << p.first << ": " << p.second << "\n";
    }
    return out.str();
}

} // namespace progressive
