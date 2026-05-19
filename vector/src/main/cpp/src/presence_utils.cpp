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

} // namespace progressive
