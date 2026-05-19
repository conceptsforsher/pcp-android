#include "progressive/account_data_utils.hpp"
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

} // namespace progressive
