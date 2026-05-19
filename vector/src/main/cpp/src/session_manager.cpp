#include "progressive/session_manager.hpp"
#include "progressive/json_parser.hpp"
#include <sstream>
#include <algorithm>
#include <chrono>

namespace progressive {

// ---- LoginType helpers ----

std::string loginTypeToString(LoginType type) {
    switch (type) {
        case LoginType::PASSWORD:    return "PASSWORD";
        case LoginType::SSO:         return "SSO";
        case LoginType::UNSUPPORTED: return "UNSUPPORTED";
        case LoginType::CUSTOM:      return "CUSTOM";
        case LoginType::DIRECT:      return "DIRECT";
        case LoginType::UNKNOWN:     return "UNKNOWN";
        case LoginType::QR:          return "QR";
        default:                     return "UNKNOWN";
    }
}

LoginType loginTypeFromString(const std::string& name) {
    if (name == "PASSWORD")    return LoginType::PASSWORD;
    if (name == "SSO")         return LoginType::SSO;
    if (name == "UNSUPPORTED") return LoginType::UNSUPPORTED;
    if (name == "CUSTOM")      return LoginType::CUSTOM;
    if (name == "DIRECT")      return LoginType::DIRECT;
    if (name == "QR")          return LoginType::QR;
    return LoginType::UNKNOWN;
}

// ---- SessionCredentials serialization ----

// Original Kotlin: Credentials JSON serialization (Moshi-backed)
std::string buildSessionCredentials(const SessionCredentials& creds) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) {
            if (c == '"') out += "\\\"";
            else if (c == '\\') out += "\\\\";
            else out += c;
        }
        return out;
    };
    std::ostringstream json;
    json << R"({"user_id": ")" << esc(creds.userId) << R"(")";
    json << R"(,"access_token": ")" << esc(creds.accessToken) << R"(")";
    json << R"(,"device_id": ")" << esc(creds.deviceId) << R"(")";
    if (!creds.homeServer.empty()) {
        json << R"(,"home_server": ")" << esc(creds.homeServer) << R"(")";
    }
    if (!creds.refreshToken.empty()) {
        json << R"(,"refresh_token": ")" << esc(creds.refreshToken) << R"(")";
    }
    json << "}";
    return json.str();
}

SessionCredentials parseSessionCredentials(const std::string& json) {
    SessionCredentials creds;
    creds.userId      = parseJsonStringValue(json, "user_id");
    creds.accessToken = parseJsonStringValue(json, "access_token");
    creds.deviceId    = parseJsonStringValue(json, "device_id");
    creds.homeServer  = parseJsonStringValue(json, "home_server");
    creds.refreshToken = parseJsonStringValue(json, "refresh_token");
    return creds;
}

// ---- SessionParams serialization ----

// Original Kotlin: SessionParams JSON representation for C++ layer
std::string buildSessionParams(const SessionParams& params) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) {
            if (c == '"') out += "\\\"";
            else if (c == '\\') out += "\\\\";
            else out += c;
        }
        return out;
    };
    std::ostringstream json;
    json << R"({"userId": ")" << esc(params.userId) << R"(")";
    json << R"(,"accessToken": ")" << esc(params.accessToken) << R"(")";
    json << R"(,"deviceId": ")" << esc(params.deviceId) << R"(")";
    json << R"(,"homeServerUrl": ")" << esc(params.homeServerUrl) << R"(")";
    json << R"(,"homeServerUrlBase": ")" << esc(params.homeServerUrlBase) << R"(")";
    if (!params.identityServerUrl.empty()) {
        json << R"(,"identityServerUrl": ")" << esc(params.identityServerUrl) << R"(")";
    }
    json << R"(,"isTokenValid": )" << (params.isTokenValid ? "true" : "false");
    json << R"(,"isUserRegistered": )" << (params.isUserRegistered ? "true" : "false");
    json << R"(,"loginType": ")" << loginTypeToString(params.loginType) << R"(")";
    json << R"(,"creationTimestampMs": )" << params.creationTimestampMs;
    if (!params.homeServerCapabilities.empty()) {
        json << R"(,"homeServerCapabilities": )" << params.homeServerCapabilities;
    }
    if (!params.sessionId.empty()) {
        json << R"(,"sessionId": ")" << esc(params.sessionId) << R"(")";
    }
    if (!params.refreshToken.empty()) {
        json << R"(,"refreshToken": ")" << esc(params.refreshToken) << R"(")";
    }
    json << "}";
    return json.str();
}

SessionParams parseSessionParams(const std::string& json) {
    SessionParams params;
    params.userId         = parseJsonStringValue(json, "userId");
    params.accessToken    = parseJsonStringValue(json, "accessToken");
    params.deviceId       = parseJsonStringValue(json, "deviceId");
    params.homeServerUrl  = parseJsonStringValue(json, "homeServerUrl");
    params.homeServerUrlBase = parseJsonStringValue(json, "homeServerUrlBase");
    params.identityServerUrl = parseJsonStringValue(json, "identityServerUrl");
    params.homeServerCapabilities = parseJsonStringValue(json, "homeServerCapabilities");
    params.isTokenValid        = parseJsonBoolValue(json, "isTokenValid", true);
    params.isUserRegistered    = parseJsonBoolValue(json, "isUserRegistered", false);
    params.creationTimestampMs = parseJsonInt64Value(json, "creationTimestampMs", 0);
    params.sessionId          = parseJsonStringValue(json, "sessionId");
    params.refreshToken       = parseJsonStringValue(json, "refreshToken");

    auto lt = parseJsonStringValue(json, "loginType");
    if (!lt.empty()) {
        params.loginType = loginTypeFromString(lt);
    }

    return params;
}

// ---- SessionState helpers ----

std::string sessionStateToString(SessionState state) {
    switch (state) {
        case SessionState::UNINITIALIZED:  return "UNINITIALIZED";
        case SessionState::INITIALIZING:   return "INITIALIZING";
        case SessionState::INITIALIZED:    return "INITIALIZED";
        case SessionState::OPENED:         return "OPENED";
        case SessionState::STARTED:        return "STARTED";
        case SessionState::STOPPED:        return "STOPPED";
        case SessionState::DESTROYED:      return "DESTROYED";
        default:                           return "UNKNOWN";
    }
}

SessionState sessionStateFromString(const std::string& name) {
    if (name == "UNINITIALIZED") return SessionState::UNINITIALIZED;
    if (name == "INITIALIZING")  return SessionState::INITIALIZING;
    if (name == "INITIALIZED")   return SessionState::INITIALIZED;
    if (name == "OPENED")        return SessionState::OPENED;
    if (name == "STARTED")       return SessionState::STARTED;
    if (name == "STOPPED")       return SessionState::STOPPED;
    if (name == "DESTROYED")     return SessionState::DESTROYED;
    return SessionState::UNINITIALIZED;
}

// ---- SessionLifecycleState ----

// Original Kotlin: SessionLifecycleObserver lifecycle transitions
// Enforces valid state transitions.

bool SessionLifecycleState::canTransitionTo(SessionState target) const {
    switch (currentState) {
        case SessionState::UNINITIALIZED:
            return target == SessionState::INITIALIZING
                || target == SessionState::DESTROYED;
        case SessionState::INITIALIZING:
            return target == SessionState::INITIALIZED
                || target == SessionState::DESTROYED;
        case SessionState::INITIALIZED:
            return target == SessionState::OPENED
                || target == SessionState::DESTROYED;
        case SessionState::OPENED:
            return target == SessionState::STARTED
                || target == SessionState::STOPPED
                || target == SessionState::DESTROYED;
        case SessionState::STARTED:
            return target == SessionState::STOPPED
                || target == SessionState::DESTROYED;
        case SessionState::STOPPED:
            return target == SessionState::STARTED
                || target == SessionState::DESTROYED;
        case SessionState::DESTROYED:
            return false; // terminal state
        default:
            return false;
    }
}

bool SessionLifecycleState::transitionTo(SessionState target) {
    if (!canTransitionTo(target)) return false;

    auto nowMs = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();

    currentState = target;
    lastTransitionMs = nowMs;

    if (target == SessionState::INITIALIZING && initializationTimeMs == 0) {
        initializationTimeMs = nowMs;
    }
    if (target == SessionState::STARTED && startedTimeMs == 0) {
        startedTimeMs = nowMs;
    }

    return true;
}

// ---- Multi-Session Management ----

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
    if (session.isActive) out << " <- active";
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

// ---- Session Persistence ----

std::string serializeSession(const SessionPersistence& session) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) {
            if (c == '"') out += "\\\"";
            else out += c;
        }
        return out;
    };
    std::ostringstream json;
    json << R"({"userId": ")" << esc(session.userId) << R"(")";
    json << R"(,"accessToken": ")" << esc(session.accessToken) << R"(")";
    json << R"(,"homeServerUrl": ")" << esc(session.homeServerUrl) << R"(")";
    json << R"(,"deviceId": ")" << esc(session.deviceId) << R"(")";
    if (!session.refreshToken.empty()) {
        json << R"(,"refreshToken": ")" << esc(session.refreshToken) << R"(")";
    }
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
        std::string out;
        for (char c : s) {
            if (c == '"') out += "\\\"";
            else out += c;
        }
        return out;
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
