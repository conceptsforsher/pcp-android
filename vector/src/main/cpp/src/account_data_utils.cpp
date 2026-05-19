#include "progressive/account_data_utils.hpp"
#include <algorithm>
#include <sstream>

namespace progressive {

// ==== Helper: extract JSON string array for a key ====

static std::vector<std::string> extractJsonStringArray(const std::string& json, const std::string& key) {
    std::vector<std::string> result;
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return result;
    pos = json.find('[', pos);
    if (pos == std::string::npos) return result;
    pos++;
    while (pos < json.size()) {
        while (pos < json.size() && (json[pos] == ' ' || json[pos] == ',' || json[pos] == '\n')) pos++;
        if (pos >= json.size() || json[pos] == ']') break;
        if (json[pos] == '"') {
            pos++;
            size_t end = pos;
            while (end < json.size() && json[end] != '"') end++;
            result.push_back(json.substr(pos, end - pos));
            pos = end + 1;
        }
    }
    return result;
}

// ==== Helper: extract a single JSON string field value ====

static std::string extractJsonStringField(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return "";
    pos = json.find(':', pos);
    if (pos == std::string::npos) return "";
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\n')) pos++;
    if (pos >= json.size() || json[pos] != '"') return "";
    pos++;
    size_t end = pos;
    while (end < json.size() && json[end] != '"') end++;
    return json.substr(pos, end - pos);
}

// ==== Helper: extract a JSON integer field value ====

static int64_t extractJsonLongField(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return -1;
    pos = json.find(':', pos);
    if (pos == std::string::npos) return -1;
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\n')) pos++;
    if (pos >= json.size()) return -1;
    bool neg = false;
    if (json[pos] == '-') { neg = true; pos++; }
    if (pos >= json.size() || json[pos] < '0' || json[pos] > '9') return -1;
    int64_t val = 0;
    while (pos < json.size() && json[pos] >= '0' && json[pos] <= '9') {
        val = val * 10 + (json[pos] - '0');
        pos++;
    }
    return neg ? -val : val;
}

// ==== Helper: extract a JSON boolean field value ====

static bool extractJsonBoolField(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return false;
    pos = json.find(':', pos);
    if (pos == std::string::npos) return false;
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\n')) pos++;
    return pos + 4 <= json.size() && json.substr(pos, 4) == "true";
}

// ==== Direct Message Map ====

DirectMessageMap parseDirectMessageMap(const std::string& json) {
    DirectMessageMap result;
    if (json.empty()) return result;

    size_t pos = 1; // Skip opening {
    while (pos < json.size()) {
        while (pos < json.size() && (json[pos] == ' ' || json[pos] == ',' || json[pos] == '\n')) pos++;
        if (pos >= json.size() || json[pos] == '}') break;
        if (json[pos] == '"') {
            pos++;
            size_t keyEnd = pos;
            while (keyEnd < json.size() && json[keyEnd] != '"') keyEnd++;
            std::string userId = json.substr(pos, keyEnd - pos);
            pos = keyEnd + 1;
            while (pos < json.size() && json[pos] != ':') pos++;
            pos++;
            while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\n')) pos++;
            if (pos < json.size() && json[pos] == '[') {
                pos++;
                std::vector<std::string> rooms;
                while (pos < json.size()) {
                    while (pos < json.size() && (json[pos] == ' ' || json[pos] == ',' || json[pos] == '\n')) pos++;
                    if (pos >= json.size() || json[pos] == ']') break;
                    if (json[pos] == '"') {
                        pos++;
                        size_t end = pos;
                        while (end < json.size() && json[end] != '"') end++;
                        rooms.push_back(json.substr(pos, end - pos));
                        pos = end + 1;
                    }
                }
                result[userId] = rooms;
            }
        }
    }
    return result;
}

std::string buildDirectMessageMapJson(const DirectMessageMap& map) {
    std::ostringstream os;
    os << "{";
    bool firstUser = true;
    for (auto it = map.begin(); it != map.end(); ++it) {
        const auto& uid = it->first;
        const auto& rooms = it->second;
        if (!firstUser) os << ",";
        firstUser = false;
        os << "\"" << uid << "\":[";
        bool firstRoom = true;
        for (const auto& rid : rooms) {
            if (!firstRoom) os << ",";
            firstRoom = false;
            os << "\"" << rid << "\"";
        }
        os << "]";
    }
    os << "}";
    return os.str();
}

// ==== Ignored Users ====

std::vector<std::string> parseIgnoredUsers(const std::string& json) {
    std::vector<std::string> result;
    auto pos = json.find("\"ignored_users\"");
    if (pos == std::string::npos) return result;
    pos = json.find('{', pos);
    if (pos == std::string::npos) return result;
    // Parse keys of the ignored_users object
    int depth = 1;
    pos++;
    while (pos < json.size() && depth > 0) {
        if (json[pos] == '{') depth++;
        else if (json[pos] == '}') depth--;
        else if (json[pos] == '"' && depth == 1) {
            pos++;
            size_t end = pos;
            while (end < json.size() && json[end] != '"') end++;
            std::string key = json.substr(pos, end - pos);
            if (!key.empty() && key[0] == '@') result.push_back(key);
            pos = end;
        }
        pos++;
    }
    return result;
}

std::string buildIgnoredUsersJson(const std::vector<std::string>& userIds) {
    std::ostringstream os;
    os << R"({"ignored_users":{)";
    for (size_t i = 0; i < userIds.size(); i++) {
        if (i > 0) os << ",";
        os << "\"" << userIds[i] << "\":{}";
    }
    os << "}}";
    return os.str();
}

// ==== Identity Server ====

IdentityServerContent parseIdentityServerContent(const std::string& json) {
    IdentityServerContent content;
    content.baseUrl = extractJsonStringField(json, "base_url");
    return content;
}

std::string buildIdentityServerContent(const IdentityServerContent& content) {
    return R"({"base_url":")" + content.baseUrl + R"("})";
}

// ==== Accepted Terms ====

AcceptedTermsContent parseAcceptedTermsContent(const std::string& json) {
    AcceptedTermsContent content;
    content.acceptedTerms = extractJsonStringArray(json, "accepted");
    return content;
}

std::string buildAcceptedTermsContent(const AcceptedTermsContent& content) {
    std::ostringstream os;
    os << R"({"accepted":[)";
    for (size_t i = 0; i < content.acceptedTerms.size(); i++) {
        if (i > 0) os << ",";
        os << "\"" << content.acceptedTerms[i] << "\"";
    }
    os << "]}";
    return os.str();
}

// ==== Breadcrumbs ====

std::vector<std::string> parseBreadcrumbs(const std::string& json) {
    return extractJsonStringArray(json, "recent_rooms");
}

std::string addBreadcrumb(const std::string& currentJson, const std::string& roomId) {
    auto current = parseBreadcrumbs(currentJson);

    // Remove existing entry if present
    current.erase(std::remove(current.begin(), current.end(), roomId), current.end());

    // Insert at front
    current.insert(current.begin(), roomId);

    // Limit to 20
    if (current.size() > 20) current.resize(20);

    std::ostringstream os;
    os << R"({"recent_rooms":[)";
    for (size_t i = 0; i < current.size(); i++) {
        if (i > 0) os << ",";
        os << "\"" << current[i] << "\"";
    }
    os << "]}";
    return os.str();
}

// ==== Room Account Data ====

RoomAccountData parseRoomAccountData(const std::string& json) {
    RoomAccountData data;
    data.roomId    = extractJsonStringField(json, "roomId");
    data.type      = extractJsonStringField(json, "type");

    // Extract raw content object
    auto pos = json.find("\"content\"");
    if (pos != std::string::npos) {
        pos = json.find(':', pos);
        if (pos != std::string::npos) {
            pos++;
            while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\n')) pos++;
            if (pos < json.size() && json[pos] == '{') {
                int depth = 0;
                size_t start = pos;
                do {
                    if (json[pos] == '{') depth++;
                    else if (json[pos] == '}') depth--;
                    pos++;
                } while (pos < json.size() && depth > 0);
                data.content = json.substr(start, pos - start);
            }
        }
    }
    return data;
}

std::string buildRoomAccountData(const RoomAccountData& data) {
    std::ostringstream os;
    os << "{\"roomId\":\"" << data.roomId
       << "\",\"type\":\"" << data.type
       << "\",\"content\":" << data.content << "}";
    return os.str();
}

// ==== Account Data Event Builders ====

std::string buildUserAccountDataEvent(const std::string& type, const std::string& contentJson) {
    // Original Kotlin (UserAccountDataEvent): {"type":"m.direct","content":{...}}
    std::ostringstream os;
    os << "{\"type\":\"" << type << "\",\"content\":" << contentJson << "}";
    return os.str();
}

std::string buildRoomAccountDataEvent(const std::string& roomId, const std::string& type,
                                      const std::string& contentJson) {
    // Original Kotlin (RoomAccountDataEvent): {"roomId":"...","type":"...","content":{...}}
    std::ostringstream os;
    os << "{\"roomId\":\"" << roomId
       << "\",\"type\":\"" << type
       << "\",\"content\":" << contentJson << "}";
    return os.str();
}

// ==== User Profile ====

// Original Kotlin (User.kt:37-42):
//   fun fromJson(userId: String, json: JsonDict) = User(
//       userId = userId,
//       displayName = json[ProfileService.DISPLAY_NAME_KEY] as? String,
//       avatarUrl = json[ProfileService.AVATAR_URL_KEY] as? String)

UserProfileContent parseUserProfileContent(const std::string& json) {
    UserProfileContent content;
    content.displayName = extractJsonStringField(json, "displayname");
    content.avatarUrl   = extractJsonStringField(json, "avatar_url");
    return content;
}

std::string buildUserProfileContent(const UserProfileContent& content) {
    std::ostringstream os;
    os << "{";
    bool first = true;
    if (!content.displayName.empty()) {
        os << "\"displayname\":\"" << content.displayName << "\"";
        first = false;
    }
    if (!content.avatarUrl.empty()) {
        if (!first) os << ",";
        os << "\"avatar_url\":\"" << content.avatarUrl << "\"";
    }
    os << "}";
    return os.str();
}

// ==== Presence ====

// Original Kotlin (PresenceEnum.kt:23-38):
//   enum class PresenceEnum(val value: String) { ONLINE("online"), OFFLINE("offline"),
//       UNAVAILABLE("unavailable"), BUSY("org.matrix.msc3026.busy") }

PresenceEnum presenceEnumFromString(const std::string& s) {
    if (s == "online")      return PresenceEnum::ONLINE;
    if (s == "offline")     return PresenceEnum::OFFLINE;
    if (s == "unavailable") return PresenceEnum::UNAVAILABLE;
    if (s == "org.matrix.msc3026.busy" || s == "busy") return PresenceEnum::BUSY;
    return PresenceEnum::OFFLINE;
}

std::string presenceEnumToString(PresenceEnum p) {
    switch (p) {
        case PresenceEnum::ONLINE:      return "online";
        case PresenceEnum::OFFLINE:     return "offline";
        case PresenceEnum::UNAVAILABLE: return "unavailable";
        case PresenceEnum::BUSY:        return "busy";
    }
    return "offline";
}

// Original Kotlin (UserPresence.kt:19-24):
//   data class UserPresence(val lastActiveAgo: Long? = null,
//       val statusMessage: String? = null, val isCurrentlyActive: Boolean? = null,
//       val presence: PresenceEnum = PresenceEnum.OFFLINE)

UserPresenceContent parseUserPresenceContent(const std::string& json) {
    UserPresenceContent content;
    content.lastActiveAgo     = extractJsonLongField(json, "last_active_ago");
    content.statusMessage     = extractJsonStringField(json, "status_msg");
    content.isCurrentlyActive  = extractJsonBoolField(json, "currently_active");
    auto presenceStr          = extractJsonStringField(json, "presence");
    if (!presenceStr.empty()) {
        content.presence = presenceEnumFromString(presenceStr);
    }
    return content;
}

std::string buildUserPresenceContent(const UserPresenceContent& content) {
    std::ostringstream os;
    os << "{\"presence\":\"" << presenceEnumToString(content.presence) << "\"";
    if (content.lastActiveAgo >= 0) {
        os << ",\"last_active_ago\":" << content.lastActiveAgo;
    }
    if (!content.statusMessage.empty()) {
        os << ",\"status_msg\":\"" << content.statusMessage << "\"";
    }
    if (content.isCurrentlyActive) {
        os << ",\"currently_active\":true";
    }
    os << "}";
    return os.str();
}

} // namespace progressive
