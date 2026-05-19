#include "progressive/presence_utils.hpp"
#include <sstream>
#include <chrono>
#include <algorithm>

namespace progressive {

PresenceInfo parsePresence(const std::string& userId, const std::string& apiResponseJson) {
    PresenceInfo info;
    info.userId = userId;

    auto presenceStr = apiResponseJson.substr(
        apiResponseJson.find("\"presence\":\"") + 12,
        apiResponseJson.find('"', apiResponseJson.find("\"presence\":\"") + 12) -
            apiResponseJson.find("\"presence\":\"") - 12
    );

    // Parse manually due to nested JSON
    auto extract = [&](const std::string& key) -> std::string {
        auto search = '"' + key + '"';
        auto pos = apiResponseJson.find(search);
        if (pos == std::string::npos) return {};
        pos = apiResponseJson.find(':', pos);
        if (pos == std::string::npos) return {};
        ++pos;
        while (pos < apiResponseJson.size() && apiResponseJson[pos] == ' ') ++pos;
        if (pos >= apiResponseJson.size()) return {};
        if (apiResponseJson[pos] == '"') {
            ++pos;
            auto end = apiResponseJson.find('"', pos);
            if (end == std::string::npos) return {};
            return apiResponseJson.substr(pos, end - pos);
        }
        auto end = pos;
        while (end < apiResponseJson.size() && apiResponseJson[end] != ',' && apiResponseJson[end] != '}' && apiResponseJson[end] != ' ') ++end;
        return apiResponseJson.substr(pos, end - pos);
    };

    auto ps = extract("presence");
    if (ps == "online") info.presence = Presence::Online;
    else if (ps == "unavailable") info.presence = Presence::Unavailable;
    else if (ps == "offline") info.presence = Presence::Offline;
    else info.presence = Presence::Unknown;

    auto la = extract("last_active_ago");
    if (!la.empty()) info.lastActiveAgoMs = std::stoll(la);

    info.statusMessage = extract("status_msg");

    return info;
}

std::string formatPresence(Presence presence) {
    switch (presence) {
        case Presence::Online:      return "Online";
        case Presence::Unavailable: return "Away";
        case Presence::Offline:     return "Offline";
        default:                    return "Unknown";
    }
}

std::string formatPresenceWithTime(Presence presence, int64_t lastActiveAgoMs) {
    if (presence == Presence::Online) return "Online";
    if (presence == Presence::Unavailable) {
        int minutes = static_cast<int>(lastActiveAgoMs / 60000);
        if (minutes < 1) return "Away";
        if (minutes < 60) return "Away " + std::to_string(minutes) + "m";
        return "Away " + std::to_string(minutes / 60) + "h";
    }
    if (presence == Presence::Offline) {
        int hours = static_cast<int>(lastActiveAgoMs / 3600000);
        if (hours < 1) return "Offline";
        if (hours < 24) return "Offline " + std::to_string(hours) + "h";
        return "Offline " + std::to_string(hours / 24) + "d";
    }
    return "Unknown";
}

std::string getPresenceIndicator(Presence presence) {
    switch (presence) {
        case Presence::Online:      return "\xF0\x9F\x9F\xA2"; // 🟢
        case Presence::Unavailable: return "\xF0\x9F\x9F\xA1"; // 🟡
        case Presence::Offline:     return "\xE2\x9A\xAB";     // ⚫
        default:                    return "\xE2\x9A\xAA";     // ⚪
    }
}

bool isPresenceStale(int64_t lastUpdatedMs) {
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    return (now - lastUpdatedMs) > 5 * 60 * 1000; // 5 minutes
}

std::string formatStatusMessage(const std::string& message, int maxLen) {
    if (message.empty()) return "";
    if (static_cast<int>(message.size()) <= maxLen) return message;
    return message.substr(0, maxLen - 3) + "...";
}

std::string formatPresenceLine(const PresenceInfo& info) {
    std::ostringstream out;
    out << getPresenceIndicator(info.presence) << " ";
    out << formatPresenceWithTime(info.presence, info.lastActiveAgoMs);
    if (!info.statusMessage.empty()) {
        out << " — " << formatStatusMessage(info.statusMessage, 60);
    }
    return out.str();
}

// ---- UserActivityTimer ----

int64_t UserActivityTimer::now() const {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
}

void UserActivityTimer::start() {
    startMs_ = now();
    totalPaused_ = 0;
    isRunning_ = true;
    isPaused_ = false;
}

void UserActivityTimer::pause() {
    if (!isRunning_ || isPaused_) return;
    pauseStart_ = now();
    isPaused_ = true;
}

void UserActivityTimer::resume() {
    if (!isRunning_ || !isPaused_) return;
    totalPaused_ += now() - pauseStart_;
    isPaused_ = false;
}

void UserActivityTimer::stop() {
    isRunning_ = false;
    isPaused_ = false;
}

int64_t UserActivityTimer::elapsedMs() const {
    if (!isRunning_) return 0;
    int64_t current = now();
    int64_t elapsed = current - startMs_ - totalPaused_;
    if (isPaused_) elapsed -= (current - pauseStart_);
    return elapsed;
}

std::string UserActivityTimer::elapsedFormatted() const {
    int64_t ms = elapsedMs();
    if (ms < 0) return "0s";
    int64_t sec = ms / 1000;
    int hours = sec / 3600;
    int minutes = (sec % 3600) / 60;
    int seconds = sec % 60;
    std::ostringstream out;
    if (hours > 0) out << hours << "h ";
    if (minutes > 0) out << minutes << "m ";
    out << seconds << "s";
    return out.str();
}

// ============ Presence API JSON builders ============

// ---- JSON escape helper ----
namespace {

std::string escJson(const std::string& s) {
    std::string out;
    out.reserve(s.size() + 8);
    for (char c : s) {
        switch (c) {
            case '"':  out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\n': out += "\\n"; break;
            case '\r': out += "\\r"; break;
            case '\t': out += "\\t"; break;
            default:   out += c;
        }
    }
    return out;
}

// Manual JSON value extractor (matches existing project patterns)
std::string extractJsonVal(const std::string& json, const std::string& key) {
    auto search = '"' + key + '"';
    auto pos = json.find(search);
    if (pos == std::string::npos) return {};
    pos = json.find(':', pos);
    if (pos == std::string::npos) return {};
    ++pos;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n')) ++pos;
    if (pos >= json.size()) return {};
    if (json[pos] == '"') {
        ++pos;
        auto end = pos;
        while (end < json.size()) {
            if (json[end] == '"') break;
            if (json[end] == '\\') ++end;
            ++end;
        }
        if (end >= json.size()) return {};
        std::string val;
        val.reserve(end - pos);
        for (size_t i = pos; i < end; ++i) {
            if (json[i] == '\\' && i + 1 < end) {
                switch (json[i + 1]) {
                    case '"':  val += '"';  ++i; break;
                    case '\\': val += '\\'; ++i; break;
                    case 'n':  val += '\n'; ++i; break;
                    case 'r':  val += '\r'; ++i; break;
                    case 't':  val += '\t'; ++i; break;
                    default:   val += json[i];
                }
            } else {
                val += json[i];
            }
        }
        return val;
    }
    auto end = pos;
    while (end < json.size() && json[end] != ',' && json[end] != '}' && json[end] != ']' && !std::isspace(json[end])) ++end;
    return json.substr(pos, end - pos);
}

// Extract raw JSON object value (for nested objects)
std::string extractJsonObj(const std::string& json, const std::string& key) {
    auto search = '"' + key + '"';
    auto pos = json.find(search);
    if (pos == std::string::npos) return {};
    pos = json.find(':', pos);
    if (pos == std::string::npos) return {};
    ++pos;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n')) ++pos;
    if (pos >= json.size()) return {};
    if (json[pos] == '{') {
        int depth = 0;
        auto end = pos;
        while (end < json.size()) {
            if (json[end] == '{') ++depth;
            else if (json[end] == '}') { --depth; if (depth == 0) { ++end; break; } }
            ++end;
        }
        return json.substr(pos, end - pos);
    }
    if (json[pos] == '[') {
        int depth = 0;
        auto end = pos;
        while (end < json.size()) {
            if (json[end] == '[') ++depth;
            else if (json[end] == ']') { --depth; if (depth == 0) { ++end; break; } }
            else if (json[end] == '"') { ++end; while (end < json.size() && json[end] != '"') { if (json[end] == '\\') ++end; ++end; } }
            ++end;
        }
        return json.substr(pos, end - pos);
    }
    return {};
}

} // anonymous namespace

// Original Kotlin: PUT /presence/{userId}/status
std::string buildPresenceEvent(UserPresence presence, const std::string& statusMsg) {
    std::ostringstream os;
    os << "{";
    os << "\"presence\":\"" << userPresenceToString(presence) << "\"";
    if (!statusMsg.empty()) {
        os << ",\"status_msg\":\"" << escJson(statusMsg) << "\"";
    }
    os << "}";
    return os.str();
}

// Original Kotlin: POST /presence/list/{userId}
std::string buildPresenceList(const std::vector<std::string>& inviteUserIds,
                               const std::vector<std::string>& dropUserIds) {
    std::ostringstream os;
    os << "{";

    bool first = true;

    if (!inviteUserIds.empty()) {
        os << "\"invite\":[";
        for (size_t i = 0; i < inviteUserIds.size(); ++i) {
            if (i > 0) os << ",";
            os << "\"" << escJson(inviteUserIds[i]) << "\"";
        }
        os << "]";
        first = false;
    }

    if (!dropUserIds.empty()) {
        if (!first) os << ",";
        os << "\"drop\":[";
        for (size_t i = 0; i < dropUserIds.size(); ++i) {
            if (i > 0) os << ",";
            os << "\"" << escJson(dropUserIds[i]) << "\"";
        }
        os << "]";
    }

    os << "}";
    return os.str();
}

// Original Kotlin: parses presence event JSON (GET /presence or sync presence)
PresenceUserInfo parsePresenceEvent(const std::string& userId, const std::string& json) {
    PresenceUserInfo info;
    info.userId = userId;

    auto ps = extractJsonVal(json, "presence");
    info.presence = userPresenceFromString(ps);

    auto la = extractJsonVal(json, "last_active_ago");
    if (!la.empty()) info.lastActiveAgoMs = std::stoll(la);

    auto sm = extractJsonVal(json, "status_msg");
    info.statusMsg = sm;

    auto ca = extractJsonVal(json, "currently_active");
    info.currentlyActive = (ca == "true");

    // May also appear as "displayname" / "avatar_url" in some contexts
    info.displayName = extractJsonVal(json, "displayname");
    info.avatarUrl = extractJsonVal(json, "avatar_url");

    return info;
}

// Original Kotlin: parses POST /presence/list response (array of presence events)
std::vector<PresenceUserInfo> parsePresenceList(const std::string& json) {
    std::vector<PresenceUserInfo> result;

    // Find the first object directly or navigate through the JSON
    auto body = json;

    // Try to find an array at top level
    size_t pos = 0;
    while (pos < body.size() && body[pos] != '[' && body[pos] != '{') ++pos;

    if (pos < body.size() && body[pos] == '{') {
        // Single presence event
        auto userId = extractJsonVal(body, "user_id");
        if (userId.empty()) userId = extractJsonVal(body, "userId");
        result.push_back(parsePresenceEvent(userId.empty() ? "" : userId, body));
        return result;
    }

    if (pos < body.size() && body[pos] == '[') {
        // Array: [{...}, {...}]
        ++pos;
        while (pos < body.size()) {
            while (pos < body.size() && (body[pos] == ' ' || body[pos] == '\t' || body[pos] == ',' || body[pos] == '\n')) ++pos;
            if (pos >= body.size() || body[pos] == ']') break;
            if (body[pos] == '{') {
                int depth = 0;
                size_t end = pos;
                while (end < body.size()) {
                    if (body[end] == '{') ++depth;
                    else if (body[end] == '}') { --depth; if (depth == 0) { ++end; break; } }
                    ++end;
                }
                auto objJson = body.substr(pos, end - pos);
                auto userId = extractJsonVal(objJson, "user_id");
                if (userId.empty()) userId = extractJsonVal(objJson, "userId");
                result.push_back(parsePresenceEvent(userId.empty() ? "" : userId, objJson));
                pos = end;
            } else {
                ++pos;
            }
        }
    }

    return result;
}

// Original Kotlin: format "last active" / "last seen" human-readable string
std::string formatLastActiveTime(int64_t lastActiveAgoMs, bool currentlyActive) {
    if (currentlyActive) return "active now";

    if (lastActiveAgoMs < 0) return "unknown";

    int64_t sec = lastActiveAgoMs / 1000;
    int64_t minutes = sec / 60;
    int64_t hours = minutes / 60;
    int64_t days = hours / 24;

    if (minutes < 1) return "just now";
    if (minutes == 1) return "1m ago";
    if (minutes < 60) return std::to_string(minutes) + "m ago";
    if (hours == 1) return "1h ago";
    if (hours < 24) return std::to_string(hours) + "h ago";
    if (days == 1) return "1d ago";
    if (days < 7) return std::to_string(days) + "d ago";
    if (days < 14) return "1w ago";
    if (days < 60) return std::to_string(days / 7) + "w ago";
    if (days < 365) return std::to_string(days / 30) + "mo ago";
    return std::to_string(days / 365) + "y ago";
}

// ---- PresenceSyncResult ----

PresenceSyncResult processPresenceSync(const std::vector<Event>& events) {
    PresenceSyncResult result;
    result.totalUsers = static_cast<int>(events.size());

    for (const auto& event : events) {
        if (event.type.empty()) continue;
        // Filter for m.presence events
        if (event.type.find("presence") == std::string::npos && event.type.find("m.presence") == std::string::npos) continue;

        std::string userId = event.senderId;
        if (userId.empty()) userId = event.stateKey;

        auto info = parsePresenceEvent(userId, event.contentJson);
        if (!info.userId.empty() || !userId.empty()) {
            if (info.userId.empty()) info.userId = userId;
            result.usersChanged.push_back(std::move(info));
        }
    }

    return result;
}

// ---- Update Presence Info ----

bool updatePresenceInfo(std::unordered_map<std::string, PresenceUserInfo>& localState,
                        const PresenceUserInfo& incoming) {
    if (incoming.userId.empty()) return false;

    auto it = localState.find(incoming.userId);
    if (it == localState.end()) {
        localState[incoming.userId] = incoming;
        return true;
    }

    const auto& existing = it->second;
    bool changed = (existing.presence != incoming.presence) ||
                   (existing.lastActiveAgoMs != incoming.lastActiveAgoMs) ||
                   (existing.statusMsg != incoming.statusMsg) ||
                   (existing.currentlyActive != incoming.currentlyActive);

    if (changed) {
        localState[incoming.userId] = incoming;
    }
    return changed;
}

// ---- Presence List Request ----

std::string buildPresenceListRequest(const std::vector<std::string>& userIds) {
    std::ostringstream os;
    os << "{";
    os << "\"user_ids\":[";
    for (size_t i = 0; i < userIds.size(); ++i) {
        if (i > 0) os << ",";
        os << "\"" << escJson(userIds[i]) << "\"";
    }
    os << "]}";
    return os.str();
}

// ---- Presence List Response ----

PresenceListResponse parsePresenceListResponse(const std::string& json) {
    PresenceListResponse response;

    auto body = json;
    size_t pos = 0;
    while (pos < body.size() && (body[pos] == ' ' || body[pos] == '\t' || body[pos] == '\n')) ++pos;

    if (pos < body.size() && body[pos] == '[') {
        ++pos;
        while (pos < body.size()) {
            while (pos < body.size() && (body[pos] == ' ' || body[pos] == '\t' || body[pos] == ',' || body[pos] == '\n')) ++pos;
            if (pos >= body.size() || body[pos] == ']') break;
            if (body[pos] == '{') {
                int depth = 1;
                size_t end = pos;
                ++end;
                while (end < body.size() && depth > 0) {
                    if (body[end] == '{') ++depth;
                    else if (body[end] == '}') --depth;
                    ++end;
                }
                auto objJson = body.substr(pos, end - pos);
                auto userId = extractJsonVal(objJson, "user_id");
                if (userId.empty()) userId = extractJsonVal(objJson, "userId");
                auto info = parsePresenceEvent(userId, objJson);
                response.presenceList[info.userId.empty() ? userId : info.userId] = std::move(info);
                pos = end;
            } else {
                ++pos;
            }
        }
    }

    return response;
}

// ---- Set Presence Request ----

// Original Kotlin: PUT /presence/{userId}/status
std::string buildSetPresenceRequest(UserPresence presence, const std::string& statusMsg) {
    return buildPresenceEvent(presence, statusMsg);
}

// ---- PresenceUpdateInfo ----

std::string formatPresenceTimestamp(const PresenceUpdateInfo& info) {
    if (info.currentlyActive) return "Active now";

    if (info.lastActiveAgoMs < 0) return "Inactive";

    int64_t sec = info.lastActiveAgoMs / 1000;
    int64_t minutes = sec / 60;
    int64_t hours = minutes / 60;

    if (minutes < 1) return "Active now";
    if (minutes < 60) return "Active " + std::to_string(minutes) + "m ago";
    if (hours < 24) return "Active " + std::to_string(hours) + "h ago";
    return "Inactive";
}

bool isCurrentlyActive(const PresenceUpdateInfo& info) {
    if (info.currentlyActive) return true;
    // Consider active if last activity was within 5 minutes (300000 ms)
    return info.lastActiveAgoMs >= 0 && info.lastActiveAgoMs < 300000;
}

} // namespace progressive
