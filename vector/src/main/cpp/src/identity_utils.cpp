#include "progressive/identity_utils.hpp"
#include "progressive/matrix_patterns.hpp"
#include <sstream>
#include <regex>
#include <algorithm>
#include <cctype>

namespace progressive {

ResolvedId resolveMatrixId(const std::string& input) {
    ResolvedId result;
    result.input = input;
    if (input.empty()) return result;

    // @user:server
    if (input[0] == '@' && isUserId(input)) {
        result.resolved = input;
        result.type = "user";
        result.valid = true;
        return result;
    }

    // !room:server
    if (input[0] == '!' && isRoomId(input)) {
        result.resolved = input;
        result.type = "room";
        result.valid = true;
        return result;
    }

    // #alias:server
    if (input[0] == '#' && isRoomAlias(input)) {
        result.resolved = input;
        result.type = "alias";
        result.valid = true;
        return result;
    }

    // $event
    if (input[0] == '$' && isEventId(input)) {
        result.resolved = input;
        result.type = "event";
        result.valid = true;
        return result;
    }

    // matrix.to URL
    if (isMatrixToPermalink(input)) {
        auto parsed = parseMatrixToPermalink(input);
        if (parsed.valid) {
            result.resolved = parsed.userId.empty() ? parsed.roomId : parsed.userId;
            result.type = parsed.type;
            result.valid = true;
            return result;
        }
    }

    // Email
    if (isEmail(input)) {
        result.resolved = input;
        result.type = "email";
        result.valid = true;
        return result;
    }

    // Phone
    if (isMsisdn(input)) {
        result.resolved = input;
        result.type = "phone";
        result.valid = true;
        return result;
    }

    return result;
}

IdentityThreePid parseThreePid(const std::string& input) {
    IdentityThreePid pid;
    if (input.empty()) return pid;

    if (isEmail(input)) {
        pid.medium = "email";
        pid.address = input;
        pid.valid = true;
        return pid;
    }

    if (isMsisdn(input)) {
        pid.medium = "msisdn";
        pid.address = input;
        pid.valid = true;
        return pid;
    }

    return pid;
}

bool isEmail(const std::string& input) {
    std::regex re(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    return std::regex_match(input, re);
}

bool isMsisdn(const std::string& input) {
    // E.164: + followed by 7-15 digits
    std::regex re(R"(^\+\d{7,15}$)");
    return std::regex_match(input, re);
}

std::string formatThreePid(const IdentityThreePid& pid) {
    if (!pid.valid) return "";
    if (pid.medium == "email") return pid.address;
    if (pid.medium == "msisdn") return pid.address;
    return pid.address;
}

bool isAmbiguousName(const std::string& name1, const std::string& name2) {
    if (name1.empty() || name2.empty()) return false;
    auto n1 = name1;
    auto n2 = name2;
    std::transform(n1.begin(), n1.end(), n1.begin(), ::tolower);
    std::transform(n2.begin(), n2.end(), n2.begin(), ::tolower);
    return n1 == n2;
}

bool isValidDisplayName(const std::string& name) {
    if (name.empty() || name.size() > 100) return false;
    // Must not be only whitespace
    bool hasNonSpace = false;
    for (char c : name) {
        if (!std::isspace(static_cast<unsigned char>(c))) hasNonSpace = true;
    }
    return hasNonSpace;
}

std::string getIdentityInitials(const std::string& displayName, int maxChars) {
    std::string result;
    bool takeNext = true;
    for (char c : displayName) {
        if (takeNext && !std::isspace(static_cast<unsigned char>(c))) {
            result += std::toupper(static_cast<unsigned char>(c));
            takeNext = false;
            if (static_cast<int>(result.size()) >= maxChars) break;
        }
        if (std::isspace(static_cast<unsigned char>(c))) takeNext = true;
    }
    return result;
}

bool isCanonicalAlias(const std::string& alias, const std::string& expectedRoomId) {
    return !alias.empty() && expectedRoomId.find(alias) != std::string::npos;
}

std::string extractAliasLocalpart(const std::string& alias) {
    if (alias.empty() || alias[0] != '#') return {};
    auto colon = alias.find(':');
    if (colon == std::string::npos) return alias.substr(1);
    return alias.substr(1, colon - 1);
}

std::vector<std::string> suggestAliases(const std::string& roomName, int maxResults) {
    std::vector<std::string> aliases;

    // Lowercase, replace spaces
    auto name = roomName;
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);
    for (char& c : name) {
        if (std::isspace(static_cast<unsigned char>(c)) || c == '.') c = '-';
    }
    // Remove non-allowed chars
    std::string cleaned;
    for (char c : name) {
        if (std::isalnum(static_cast<unsigned char>(c)) || c == '-' || c == '_') {
            cleaned += c;
        }
    }

    if (!cleaned.empty()) {
        aliases.push_back("#" + cleaned);
        if (static_cast<int>(aliases.size()) >= maxResults) return aliases;
        aliases.push_back("#" + cleaned + "-chat");
        if (static_cast<int>(aliases.size()) >= maxResults) return aliases;
        aliases.push_back("#" + cleaned + "-room");
    }

    return aliases;
}

} // namespace progressive
