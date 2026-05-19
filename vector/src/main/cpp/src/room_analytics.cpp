#include "progressive/room_analytics.hpp"
#include <sstream>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>

namespace progressive {

void sortByJoinDate(std::vector<UserStats>& users) {
    std::sort(users.begin(), users.end(), [](const UserStats& a, const UserStats& b) {
        return a.firstSeenMs < b.firstSeenMs;
    });
}

std::vector<UserStats> filterByServer(const std::vector<UserStats>& users, const std::string& server) {
    if (server.empty()) return users;
    std::vector<UserStats> result;
    for (const auto& u : users) {
        if (u.serverName == server) result.push_back(u);
    }
    return result;
}

RoomAnalytics computeRoomAnalytics(const std::vector<CacheEvent>& events) {
    RoomAnalytics analytics;
    std::unordered_map<std::string, UserStats> userMap;
    std::unordered_set<std::string> servers;

    for (const auto& e : events) {
        if (e.senderId.empty()) continue;

        auto& us = userMap[e.senderId];
        if (us.userId.empty()) {
            us.userId = e.senderId;
            us.displayName = e.senderName;
            us.serverName = extractServerName(e.senderId);
            us.firstSeenMs = e.timestamp;
        }

        servers.insert(us.serverName);

        // Count messages
        if (e.eventType == "m.room.message" || !e.body.empty()) {
            us.messageCount++;
            us.avgMessageLength += e.body.size();
        }

        // Track join/leave events
        if (e.eventType == "m.room.member" && !e.stateKey.empty()) {
            JoinLeaveEvent jle;
            jle.eventId = e.eventId;
            jle.type = e.membership;
            jle.timestamp = e.timestamp;
            analytics.userJoinHistory[e.stateKey].push_back(jle);

            if (e.membership == "join" && e.stateKey == e.senderId) {
                us.firstSeenMs = std::min(us.firstSeenMs, e.timestamp);
            }
        }

        if (e.timestamp > us.lastSeenMs) us.lastSeenMs = e.timestamp;
        analytics.totalMessages++;
    }

    // Compute averages
    for (auto& [_, us] : userMap) {
        if (us.messageCount > 0) {
            us.avgMessageLength /= us.messageCount;
        }
        analytics.topPosters.push_back(us);
    }

    // Sort by message count
    std::sort(analytics.topPosters.begin(), analytics.topPosters.end(),
        [](const UserStats& a, const UserStats& b) {
            return a.messageCount > b.messageCount;
        }
    );

    analytics.totalUsers = static_cast<int>(userMap.size());
    analytics.servers.assign(servers.begin(), servers.end());
    std::sort(analytics.servers.begin(), analytics.servers.end());

    return analytics;
}

std::vector<UserStats> computeAvgMessageLengths(const std::vector<CacheEvent>& events) {
    std::unordered_map<std::string, UserStats> userMap;

    for (const auto& e : events) {
        if (e.senderId.empty() || e.body.empty()) continue;
        auto& us = userMap[e.senderId];
        if (us.userId.empty()) {
            us.userId = e.senderId;
            us.displayName = e.senderName;
        }
        us.messageCount++;
        us.avgMessageLength += e.body.size();
    }

    std::vector<UserStats> result;
    for (auto& [_, us] : userMap) {
        if (us.messageCount > 0) {
            us.avgMessageLength /= us.messageCount;
        }
        result.push_back(us);
    }
    std::sort(result.begin(), result.end(), [](const UserStats& a, const UserStats& b) {
        return a.avgMessageLength > b.avgMessageLength;
    });
    return result;
}

std::string analyticsToJson(const RoomAnalytics& a) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    std::ostringstream json;
    json << "{";
    json << R"("roomId": ")" << esc(a.roomId) << R"(",)";
    json << R"("totalMessages": )" << a.totalMessages << ",";
    json << R"("totalUsers": )" << a.totalUsers << ",";
    json << R"("servers": [)";
    for (size_t i = 0; i < a.servers.size(); ++i) {
        if (i > 0) json << ",";
        json << R"(")" << esc(a.servers[i]) << R"(")";
    }
    json << "],";
    json << R"("topPosters": [)";
    for (size_t i = 0; i < a.topPosters.size(); ++i) {
        if (i > 0) json << ",";
        const auto& u = a.topPosters[i];
        json << R"({"userId": ")" << esc(u.userId) << R"(")";
        json << R"(,"displayName": ")" << esc(u.displayName) << R"(")";
        json << R"(,"serverName": ")" << esc(u.serverName) << R"(")";
        json << R"(,"messageCount": )" << u.messageCount;
        json << R"(,"avgMessageLength": )" << u.avgMessageLength;
        json << R"(,"firstSeenMs": )" << u.firstSeenMs;
        json << R"(,"lastSeenMs": )" << u.lastSeenMs << "}";
    }
    json << "]";
    // Join history
    json << R"(,"userJoinHistory": {)";
    size_t hi = 0;
    for (const auto& [userId, events] : a.userJoinHistory) {
        if (hi++ > 0) json << ",";
        json << R"(")" << esc(userId) << R"(": [)";
        for (size_t j = 0; j < events.size(); ++j) {
            if (j > 0) json << ",";
            json << R"({"eventId": ")" << esc(events[j].eventId) << R"(")";
            json << R"(,"type": ")" << events[j].type << R"(")";
            json << R"(,"timestamp": )" << events[j].timestamp << "}";
        }
        json << "]";
    }
    json << "}}";
    return json.str();
}

} // namespace progressive
