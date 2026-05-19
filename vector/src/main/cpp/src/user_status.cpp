#include "progressive/user_status.hpp"
#include <sstream>
#include <ctime>
#include <vector>

namespace progressive {

// ==== UserStatus Methods ====

std::string UserStatus::displayText() const {
    if (!isSet || status.empty()) return "";
    if (!emoji.empty()) return emoji + " " + status;
    return status;
}

std::string UserStatus::emojiOnly() const { return emoji; }

bool UserStatus::isEmpty() const { return !isSet || status.empty(); }

// ==== JSON Parsing ====
// Format: {"status":"In a meeting","emoji":"💼","setAt":1715700000000}

UserStatus parseUserStatus(const std::string& accountDataJson) {
    UserStatus s;

    auto extractStr = [&](const std::string& key) -> std::string {
        auto search = "\"" + key + "\":\"";
        auto pos = accountDataJson.find(search);
        if (pos == std::string::npos) {
            search = "\"" + key + "\": \"";
            pos = accountDataJson.find(search);
        }
        if (pos == std::string::npos) return "";
        pos += search.size();
        auto end = accountDataJson.find('"', pos);
        return (end != std::string::npos) ? accountDataJson.substr(pos, end - pos) : "";
    };

    auto extractInt64 = [&](const std::string& key) -> int64_t {
        auto search = "\"" + key + "\":";
        auto pos = accountDataJson.find(search);
        if (pos == std::string::npos) return 0;
        pos += search.size();
        while (pos < accountDataJson.size() && (accountDataJson[pos] == ' ' || accountDataJson[pos] == '\t')) pos++;
        int64_t val = 0;
        while (pos < accountDataJson.size() && accountDataJson[pos] >= '0' && accountDataJson[pos] <= '9') {
            val = val * 10 + (accountDataJson[pos] - '0'); pos++;
        }
        return val;
    };

    s.status = extractStr("status");
    s.emoji = extractStr("emoji");
    s.setAtMs = extractInt64("setAt");
    s.expiresAtMs = extractInt64("expiresAt");
    s.isSet = !s.status.empty();

    if (s.isSet && s.expiresAtMs > 0) {
        // Get current time for expiry check
        auto now = std::time(nullptr) * 1000;
        s.isExpired = now > s.expiresAtMs;
    }

    return s;
}

std::string buildUserStatusJson(const std::string& status, const std::string& emoji, int64_t nowMs) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"status": ")" << esc(status) << R"(")";
    if (!emoji.empty()) json << R"(,"emoji": ")" << esc(emoji) << R"(")";
    json << R"(,"setAt": )" << nowMs;
    json << "}";
    return json.str();
}

bool isStatusExpired(const UserStatus& status) {
    return status.isExpired;
}

// ==== Formatting ====

std::string formatStatusForProfile(const UserStatus& status) {
    return status.displayText();
}

std::string formatStatusForRoomList(const UserStatus& status) {
    if (!status.emoji.empty()) return status.emoji + " " + status.status;
    return status.status;
}

std::string formatStatusForTimeline(const UserStatus& status) {
    return status.emojiOnly();  // Just emoji under the name
}

UserStatus resolveBestStatus(const UserStatus& customStatus, bool isOnline, int64_t lastActiveMs) {
    if (customStatus.isSet && !customStatus.isExpired) return customStatus;

    // Fallback to presence-based status
    UserStatus presenceStatus;
    presenceStatus.isSet = true;
    presenceStatus.status = getPresenceStatusText(isOnline, lastActiveMs);
    if (isOnline) presenceStatus.emoji = "🟢";
    else if (presenceStatus.status == "Away") presenceStatus.emoji = "🟡";
    else presenceStatus.emoji = "⚫";
    return presenceStatus;
}

std::string getPresenceStatusText(bool isOnline, int64_t lastActiveMs) {
    if (isOnline) return "Online";

    // Calculate time since last activity
    auto now = std::time(nullptr) * 1000;
    auto inactiveMs = now - lastActiveMs;
    auto inactiveMin = inactiveMs / 60000;

    if (inactiveMin < 5) return "Online";
    if (inactiveMin < 30) return "Away";
    if (inactiveMin < 120) return "Away for " + std::to_string(inactiveMin) + "m";
    if (inactiveMin < 1440) return "Away for " + std::to_string(inactiveMin / 60) + "h";
    return "Offline";
}

std::string userStatusToJson(const UserStatus& status) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"isSet": )" << (status.isSet ? "true" : "false") << ",";
    json << R"("status": ")" << esc(status.status) << R"(",)";
    json << R"("emoji": ")" << esc(status.emoji) << R"(",)";
    json << R"("displayText": ")" << esc(status.displayText()) << R"(",)";
    json << R"("isEmpty": )" << (status.isEmpty() ? "true" : "false") << ",";
    json << R"("isExpired": )" << (status.isExpired ? "true" : "false") << "}";
    return json.str();
}

std::vector<std::string> getStatusSuggestions() {
    // Common status presets (like Element Web)
    return {
        "🎮 Gaming",
        "💼 In a meeting",
        "🍽️ Lunch break",
        "✈️ On vacation",
        "🤒 Sick",
        "📚 Studying",
        "🎧 Listening to music",
        "💤 Sleeping",
        "🚗 Commuting",
        "🏠 Working from home"
    };
}

} // namespace progressive
