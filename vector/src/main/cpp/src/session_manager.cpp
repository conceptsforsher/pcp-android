#include "progressive/session_manager.hpp"
#include "progressive/json_parser.hpp"
#include <sstream>
#include <algorithm>
#include <chrono>

namespace progressive {

SessionList computeSessionList(const std::vector<SessionInfo>& sessions,
    const std::string& activeUserId) {
    SessionList list;
    list.sessions = sessions;
    list.activeUserId = activeUserId;

    for (auto& s : list.sessions) {
        s.isActive = (s.userId == activeUserId);
        list.totalUnread += s.unreadCount;
        list.totalHighlights += s.highlightCount;
    }

    assignSessionIndices(list.sessions);
    sortSessions(list.sessions);

    return list;
}

void assignSessionIndices(std::vector<SessionInfo>& sessions) {
    for (size_t i = 0; i < sessions.size(); ++i) {
        sessions[i].sessionIndex = static_cast<int>(i) + 1;
    }
}

void sortSessions(std::vector<SessionInfo>& sessions) {
    std::sort(sessions.begin(), sessions.end(), [](const SessionInfo& a, const SessionInfo& b) {
        if (a.isActive != b.isActive) return a.isActive;
        if (a.highlightCount != b.highlightCount) return a.highlightCount > b.highlightCount;
        return a.lastSyncMs > b.lastSyncMs;
    });
}

std::string formatSessionBadge(const SessionInfo& session) {
    if (session.highlightCount > 0) return "!";
    if (session.unreadCount > 0) {
        return session.unreadCount > 99 ? "99+" : std::to_string(session.unreadCount);
    }
    return "";
}

std::string formatSessionInfo(const SessionInfo& session) {
    std::ostringstream out;
    out << session.displayName;
    if (!session.homeServer.empty()) {
        out << " @" << session.homeServer;
    }
    if (!session.displayName.empty() && session.userId != session.displayName) {
        out << " (" << session.userId << ")";
    }
    auto badge = formatSessionBadge(session);
    if (!badge.empty()) {
        out << " [" << badge << "]";
    }
    if (session.isActive) out << " ← active";
    return out.str();
}

bool hasPendingNotifications(const SessionList& list) {
    return list.totalHighlights > 0;
}

std::string getRecommendedSession(const SessionList& list, const std::string& excludeUserId) {
    const SessionInfo* best = nullptr;
    for (const auto& s : list.sessions) {
        if (s.userId == excludeUserId) continue;
        if (!best) best = &s;
        else if (s.highlightCount > best->highlightCount) best = &s;
        else if (s.highlightCount == best->highlightCount && s.lastSyncMs > best->lastSyncMs) best = &s;
    }
    return best ? best->userId : "";
}

std::string serializeSession(const SessionPersistence& session) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"userId": ")" << esc(session.userId) << R"(")";
    json << R"(,"accessToken": ")" << esc(session.accessToken) << R"(")";
    json << R"(,"homeServerUrl": ")" << esc(session.homeServerUrl) << R"(")";
    json << R"(,"deviceId": ")" << esc(session.deviceId) << R"(")";
    json << R"(,"lastUsedMs": )" << session.lastUsedMs;
    json << R"(,"isActive": )" << (session.isActive ? "true" : "false") << "}";
    return json.str();
}

SessionPersistence deserializeSession(const std::string& data) {
    SessionPersistence session;
    session.userId        = parseJsonStringValue(data, "userId");
    session.accessToken   = parseJsonStringValue(data, "accessToken");
    session.refreshToken  = parseJsonStringValue(data, "refreshToken");
    session.homeServerUrl = parseJsonStringValue(data, "homeServerUrl");
    session.deviceId      = parseJsonStringValue(data, "deviceId");

    auto lastUsed = parseJsonStringValue(data, "lastUsedMs");
    if (!lastUsed.empty()) session.lastUsedMs = std::stoll(lastUsed);

    auto active = parseJsonStringValue(data, "isActive");
    session.isActive = (active == "true");

    return session;
}

bool needsTokenRefresh(const SessionPersistence& session, int64_t tokenExpiryMs) {
    if (tokenExpiryMs <= 0) return false;
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    return (tokenExpiryMs - now) < 60000; // less than 1 minute
}

std::string sessionListToJson(const SessionList& list) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"activeUserId": ")" << esc(list.activeUserId) << R"(")";
    json << R"(,"totalUnread": )" << list.totalUnread << ",";
    json << R"(,"totalHighlights": )" << list.totalHighlights << ",";
    json << R"("sessions": [)";
    for (size_t i = 0; i < list.sessions.size(); ++i) {
        if (i > 0) json << ",";
        const auto& s = list.sessions[i];
        json << R"({"userId": ")" << esc(s.userId) << R"(")";
        json << R"(,"displayName": ")" << esc(s.displayName) << R"(")";
        json << R"(,"unreadCount": )" << s.unreadCount << ",";
        json << R"(,"highlightCount": )" << s.highlightCount << ",";
        json << R"(,"sessionIndex": )" << s.sessionIndex << ",";
        json << R"(,"isActive": )" << (s.isActive ? "true" : "false") << "}";
    }
    json << "]}";
    return json.str();
}

} // namespace progressive
