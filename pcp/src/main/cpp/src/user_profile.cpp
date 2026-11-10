#include "pcp/user_profile.hpp"
#include <sstream>
#include <chrono>

namespace pcp {

static std::string extractField(const std::string& json, const std::string& key) {
    auto p = json.find("\"" + key + "\":\"");
    if (p == std::string::npos) return "";
    p += key.size() + 4;
    auto e = json.find('"', p);
    if (e == std::string::npos) return "";
    return json.substr(p, e - p);
}

UserProfile parseUserProfile(const std::string& json, const std::string& userId) {
    UserProfile p;
    p.userId = userId.empty() ? extractField(json, "user_id") : userId;
    p.displayName = extractField(json, "displayname");
    if (p.displayName.empty()) p.displayName = extractField(json, "display_name");
    p.avatarUrl = extractField(json, "avatar_url");
    return p;
}

UserProfile parsePresenceEvent(const std::string& userId, const std::string& json) {
    UserProfile p;
    p.userId = userId;
    p.presence = extractField(json, "presence");
    if (p.presence.empty()) p.presence = "offline";
    p.statusMessage = extractField(json, "status_msg");
    p.isActive = json.find("\"currently_active\":true") != std::string::npos;
    
    auto agoPos = json.find("\"last_active_ago\":");
    if (agoPos != std::string::npos) {
        agoPos += 17;
        while (agoPos < json.size() && json[agoPos] == ' ') agoPos++;
        try { p.lastActiveAgoMs = std::stoll(json.substr(agoPos)); } catch(...) {}
    }
    return p;
}

std::string formatDisplayName(const std::string& displayName, const std::string& userId) {
    if (!displayName.empty()) return displayName;
    auto colon = userId.find(':');
    return colon != std::string::npos ? userId.substr(1, colon - 1) : userId;
}

std::string formatUserMention(const std::string& userId, const std::string& displayName) {
    std::string name = formatDisplayName(displayName, userId);
    return "@" + name;
}

std::string formatLastActive(int64_t agoMs) {
    if (agoMs < 0) return "Unknown";
    int64_t secs = agoMs / 1000;
    if (secs < 60) return "Active now";
    int64_t mins = secs / 60;
    if (mins < 60) return std::to_string(mins) + "m ago";
    int64_t hours = mins / 60;
    if (hours < 24) return std::to_string(hours) + "h ago";
    int64_t days = hours / 24;
    if (days < 7) return std::to_string(days) + "d ago";
    // Format as date
    int64_t timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count() - agoMs;
    return std::to_string(timestamp / 86400000) + "d ago";
}

std::string extractDisplayName(const std::string& memberEventJson, const std::string& userId) {
    auto dn = extractField(memberEventJson, "displayname");
    if (!dn.empty()) return dn;
    dn = extractField(memberEventJson, "display_name");
    if (!dn.empty()) return dn;
    // Fallback: extract from prev_content if this is a profile update
    auto prevPos = memberEventJson.find("\"prev_content\"");
    if (prevPos != std::string::npos) {
        auto prevSub = memberEventJson.substr(prevPos);
        dn = extractField(prevSub, "displayname");
        if (!dn.empty()) return dn;
    }
    return formatDisplayName("", userId);
}

} // namespace pcp
