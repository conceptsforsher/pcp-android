#include "progressive/session_manager_full.hpp"
#include <sstream>
#include <algorithm>
#include <ctime>

namespace progressive {

// ====== Enum conversions ======

const char* sessionLoginTypeToString(SessionLoginType type) {
    switch (type) {
        case SessionLoginType::PASSWORD: return "password";
        case SessionLoginType::SSO: return "sso";
        case SessionLoginType::TOKEN: return "token";
        case SessionLoginType::OIDC: return "oidc";
        default: return "unknown";
    }
}

SessionLoginType sessionLoginTypeFromString(const std::string& s) {
    if (s == "password") return SessionLoginType::PASSWORD;
    if (s == "sso") return SessionLoginType::SSO;
    if (s == "token") return SessionLoginType::TOKEN;
    if (s == "oidc") return SessionLoginType::OIDC;
    return SessionLoginType::UNKNOWN;
}

const char* sessionStateToString(SessionLifecycleState state) {
    switch (state) {
        case SessionLifecycleState::CREATED: return "created";
        case SessionLifecycleState::OPENING: return "opening";
        case SessionLifecycleState::OPENED: return "opened";
        case SessionLifecycleState::CLOSING: return "closing";
        case SessionLifecycleState::CLOSED: return "closed";
        case SessionLifecycleState::ERROR: return "error";
        default: return "unknown";
    }
}

// ====== Session Credentials ======
// Original: sessionId() = md5("$userId|$deviceId")

std::string SessionCredentials::computeSessionId() const {
    return SessionManager::computeSessionId(userId, deviceId);
}

std::string SessionCredentials::extractHomeServer() const {
    if (!userId.empty() && userId[0] == '@') {
        auto colon = userId.find(':');
        if (colon != std::string::npos) return userId.substr(colon + 1);
    }
    return homeServer;
}

// ====== Session ID Computation ======
// Original: md5("$userId|$deviceId")
// We use a simple DJB2 hash to avoid external crypto dependency

std::string SessionManager::simpleHash(const std::string& input) {
    uint32_t hash = 5381;
    for (char c : input) hash = ((hash << 5) + hash) + static_cast<uint8_t>(c);
    // Convert to hex
    static const char* hex = "0123456789abcdef";
    std::string result;
    for (int i = 28; i >= 0; i -= 4) result += hex[(hash >> i) & 0xF];
    return result;
}

std::string SessionManager::computeSessionId(const std::string& userId, const std::string& deviceId) {
    return simpleHash(userId + "|" + deviceId);
}

// ====== JSON helpers ======

static std::string extractStr(const std::string& json, const std::string& key) {
    auto pp = json.find("\"" + key + "\"");
    if (pp == std::string::npos) return "";
    pp = json.find('"', pp + key.size() + 2);
    if (pp == std::string::npos) return "";
    pp++;
    size_t e = pp;
    while (e < json.size() && json[e] != '"') e++;
    return json.substr(pp, e - pp);
}

static int64_t extractInt(const std::string& json, const std::string& key) {
    auto pp = json.find("\"" + key + "\"");
    if (pp == std::string::npos) return 0;
    pp = json.find(':', pp);
    if (pp == std::string::npos) return 0;
    pp++;
    while (pp < json.size() && (json[pp] == ' ' || json[pp] == '\t')) pp++;
    int64_t v = 0;
    while (pp < json.size() && json[pp] >= '0' && json[pp] <= '9') { v=v*10+(json[pp]-'0'); pp++; }
    return v;
}

// ====== Constructor ======

SessionManager::SessionManager() {}

SavedSessionInfo* SessionManager::findSession(const std::string& sessionId) {
    auto it = sessions_.find(sessionId);
    return (it != sessions_.end()) ? &it->second : nullptr;
}

const SavedSessionInfo* SessionManager::findSession(const std::string& sessionId) const {
    auto it = sessions_.find(sessionId);
    return (it != sessions_.end()) ? &it->second : nullptr;
}

// ====== Session Lifecycle ======

std::string SessionManager::createSession(const SessionCredentials& creds, const HomeServerConfig& config,
                                           SessionLoginType loginType, std::string& error) {
    if (creds.userId.empty()) { error = "Missing userId"; return ""; }
    if (creds.accessToken.empty()) { error = "Missing accessToken"; return ""; }
    if (creds.deviceId.empty()) { error = "Missing deviceId"; return ""; }

    auto sessionId = creds.computeSessionId();
    if (hasSession(sessionId)) { error = "Session already exists: " + sessionId; return ""; }

    SavedSessionInfo info;
    info.sessionId = sessionId;
    info.userId = creds.userId;
    info.deviceId = creds.deviceId;
    info.homeServerUrl = config.homeServerUrl;
    info.homeServerUrlBase = config.homeServerUrlBase;
    info.identityServerUrl = config.identityServerUrl;
    info.loginType = loginType;
    info.state = SessionLifecycleState::CREATED;
    info.isOpenable = !creds.accessToken.empty();
    info.createdAtMs = static_cast<int64_t>(std::time(nullptr)) * 1000;

    sessions_[sessionId] = info;
    return sessionId;
}

bool SessionManager::openSession(const std::string& sessionId, std::string& error) {
    auto* s = findSession(sessionId);
    if (!s) { error = "Session not found: " + sessionId; return false; }
    if (!s->isOpenable) { error = "Session not openable (no token)"; return false; }

    s->state = SessionLifecycleState::OPENED;
    s->isSyncing = true;
    updateSyncTimestamp(sessionId);
    return true;
}

bool SessionManager::closeSession(const std::string& sessionId) {
    auto* s = findSession(sessionId);
    if (!s) return false;

    s->state = SessionLifecycleState::CLOSED;
    s->isSyncing = false;

    if (s->isActive) {
        s->isActive = false;
        activeSessionId_.clear();
    }
    return true;
}

bool SessionManager::removeSession(const std::string& sessionId) {
    if (sessionId == activeSessionId_) activeSessionId_.clear();
    return sessions_.erase(sessionId) > 0;
}

void SessionManager::clearAllSessions() {
    sessions_.clear();
    activeSessionId_.clear();
}

// ====== Active Session ======
// Original: ActiveSessionHolder.setActiveSession(session)

bool SessionManager::setActiveSession(const std::string& sessionId) {
    auto* s = findSession(sessionId);
    if (!s) return false;

    // Deactivate current
    if (!activeSessionId_.empty()) {
        auto* current = findSession(activeSessionId_);
        if (current) current->isActive = false;
    }

    s->isActive = true;
    activeSessionId_ = sessionId;
    return true;
}

void SessionManager::clearActiveSession() {
    if (!activeSessionId_.empty()) {
        auto* s = findSession(activeSessionId_);
        if (s) s->isActive = false;
        activeSessionId_.clear();
    }
}

bool SessionManager::hasActiveSession() const {
    return !activeSessionId_.empty() && hasSession(activeSessionId_);
}

bool SessionManager::getActiveSession(SavedSessionInfo& out) const {
    auto* s = findSession(activeSessionId_);
    if (!s) return false;
    out = *s;
    return true;
}

bool SessionManager::getSession(const std::string& sessionId, SavedSessionInfo& out) const {
    auto* s = findSession(sessionId);
    if (!s) return false;
    out = *s;
    return true;
}

bool SessionManager::getSessionByUser(const std::string& userId, SavedSessionInfo& out) const {
    for (const auto& [id, info] : sessions_) {
        if (info.userId == userId) {
            out = info;
            return true;
        }
    }
    return false;
}

// ====== Queries ======

std::vector<SavedSessionInfo> SessionManager::getAllSessions() const {
    std::vector<SavedSessionInfo> result;
    for (const auto& [id, info] : sessions_) result.push_back(info);
    // Sort: active first, then by last sync (descending)
    std::sort(result.begin(), result.end(), [](const SavedSessionInfo& a, const SavedSessionInfo& b) {
        if (a.isActive != b.isActive) return a.isActive;
        return a.lastSyncMs > b.lastSyncMs;
    });
    return result;
}

bool SessionManager::hasSession(const std::string& sessionId) const {
    return sessions_.find(sessionId) != sessions_.end();
}

bool SessionManager::hasUserSession(const std::string& userId) const {
    for (const auto& [id, info] : sessions_) {
        if (info.userId == userId) return true;
    }
    return false;
}

// ====== Token Management ======

bool SessionManager::updateAccessToken(const std::string& sessionId, const std::string& newToken) {
    auto* s = findSession(sessionId);
    if (!s) return false;
    s->isOpenable = !newToken.empty();
    return true;
}

bool SessionManager::updateRefreshToken(const std::string& sessionId, const std::string& newRefreshToken) {
    return findSession(sessionId) != nullptr;
}

void SessionManager::invalidateToken(const std::string& sessionId) {
    auto* s = findSession(sessionId);
    if (s) {
        s->isOpenable = false;
        s->state = SessionLifecycleState::ERROR;
    }
}

bool SessionManager::isTokenExpired(const std::string& sessionId) const {
    auto* s = findSession(sessionId);
    if (!s) return true;
    if (s->tokenExpiresAtMs <= 0) return false;
    auto now = static_cast<int64_t>(std::time(nullptr)) * 1000;
    return now >= s->tokenExpiresAtMs;
}

// ====== Metadata ======

void SessionManager::setDisplayName(const std::string& sessionId, const std::string& name) {
    auto* s = findSession(sessionId);
    if (s) s->displayName = name;
}

void SessionManager::setAvatarUrl(const std::string& sessionId, const std::string& url) {
    auto* s = findSession(sessionId);
    if (s) s->avatarUrl = url;
}

void SessionManager::updateSyncTimestamp(const std::string& sessionId) {
    auto* s = findSession(sessionId);
    if (s) s->lastSyncMs = static_cast<int64_t>(std::time(nullptr)) * 1000;
}

// ====== Push Rules ======

void SessionManager::setPushRule(const std::string& sessionId, const std::string& rule) {
    auto* s = findSession(sessionId);
    if (s) s->pushRuleGlobal = rule;
}

// ====== Serialization ======

std::string SessionManager::sessionToJson(const SavedSessionInfo& session) const {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    std::ostringstream os;
    os << R"({"session_id":")" << esc(session.sessionId)
       << R"(","user_id":")" << esc(session.userId)
       << R"(","device_id":")" << esc(session.deviceId)
       << R"(","display_name":")" << esc(session.displayName)
       << R"(","avatar_url":")" << esc(session.avatarUrl)
       << R"(","home_server":")" << esc(session.homeServerUrl)
       << R"(","state":")" << sessionStateToString(session.state)
       << R"(","is_active":)" << (session.isActive ? "true" : "false")
       << R"(,"is_syncing":)" << (session.isSyncing ? "true" : "false")
       << R"(,"is_openable":)" << (session.isOpenable ? "true" : "false")
       << R"(,"login_type":")" << sessionLoginTypeToString(session.loginType)
       << R"(","created_at":)" << session.createdAtMs
       << R"(,"last_sync":)" << session.lastSyncMs
       << R"(,"push_rule":")" << esc(session.pushRuleGlobal)
       << R"(")";
    os << "}";
    return os.str();
}

std::string SessionManager::allSessionsToJson() const {
    auto sessions = getAllSessions();
    std::ostringstream os; os << "[";
    for (size_t i = 0; i < sessions.size(); i++) {
        if (i > 0) os << ","; os << sessionToJson(sessions[i]);
    }
    os << "]";
    return os.str();
}

std::string SessionManager::paramsToJson(const SavedSessionParams& params) const {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    std::ostringstream os;
    os << R"({"session_id":")" << esc(params.sessionId())
       << R"(","user_id":")" << esc(params.userId())
       << R"(","device_id":")" << esc(params.deviceId())
       << R"(","home_server_url":")" << esc(params.homeServerUrl())
       << R"(","home_server_url_base":")" << esc(params.homeServerUrlBase())
       << R"(","identity_server_url":")" << esc(params.defaultIdentityServerUrl())
       << R"(","is_token_valid":)" << (params.isTokenValid ? "true" : "false")
       << R"(,"login_type":")" << sessionLoginTypeToString(params.loginType)
       << R"(")";
    os << "}";
    return os.str();
}

} // namespace progressive
