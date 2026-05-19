#include "progressive/username_validator.hpp"
#include <sstream>
#include <cctype>
#include <algorithm>
#include <unordered_set>

namespace progressive {

UsernameValidation validateUsername(const std::string& username) {
    UsernameValidation result;
    result.username = username;
    result.length = static_cast<int>(username.size());

    if (username.empty()) {
        result.errorMessage = "Username cannot be empty.";
        return result;
    }

    if (username.size() > 255) {
        result.errorMessage = "Username is too long (max 255 characters).";
        return result;
    }

    // Must start with a letter
    if (!std::isalpha(static_cast<unsigned char>(username[0]))) {
        result.errorMessage = "Username must start with a letter.";
        return result;
    }

    // Check allowed characters
    for (char c : username) {
        if (!std::islower(static_cast<unsigned char>(c)) &&
            !std::isdigit(static_cast<unsigned char>(c)) &&
            c != '_' && c != '.' && c != '-' && c != '=' && c != '/') {
            result.errorMessage = std::string("Invalid character '") + c + "' in username.";
            return result;
        }
    }

    // Check reserved names
    if (isReservedUsername(username)) {
        result.errorMessage = "This username is reserved.";
        return result;
    }

    result.valid = true;
    result.sanitized = sanitizeUsername(username);
    return result;
}

std::string sanitizeUsername(const std::string& raw) {
    std::string result;
    for (char c : raw) {
        char lower = std::tolower(static_cast<unsigned char>(c));
        if (std::islower(static_cast<unsigned char>(lower)) ||
            std::isdigit(static_cast<unsigned char>(lower)) ||
            lower == '_' || lower == '.' || lower == '-' || lower == '=' || lower == '/') {
            result += lower;
        } else if (lower == ' ') {
            result += '_';
        }
    }
    // Ensure starts with letter
    if (!result.empty() && !std::isalpha(static_cast<unsigned char>(result[0]))) {
        result = "u" + result;
    }
    if (result.size() > 255) result = result.substr(0, 255);
    return result;
}

bool isReservedUsername(const std::string& username) {
    static const std::unordered_set<std::string> reserved = {
        "admin", "administrator", "matrix", "support", "help",
        "root", "system", "nobody", "element", "riot", "vector",
        "server", "homeserver", "moderator", "mod", "bot",
        "service", "abuse", "security", "postmaster", "hostmaster",
        "webmaster", "info", "test", "null", "undefined"
    };
    auto lower = username;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return reserved.find(lower) != reserved.end();
}

std::string suggestUsername(const std::string& displayName, int maxAttempts) {
    if (displayName.empty()) return "user";

    // Try underscored version
    auto sanitized = sanitizeUsername(displayName);
    if (sanitized.size() >= 3 && !isReservedUsername(sanitized)) return sanitized;

    // Try replacing spaces with dots
    auto dotted = sanitized;
    size_t pos = 0;
    while ((pos = dotted.find('_', pos)) != std::string::npos) {
        dotted[pos] = '.';
        pos++;
    }
    if (dotted.size() >= 3 && !isReservedUsername(dotted)) return dotted;

    // Try dashes
    auto dashed = sanitized;
    pos = 0;
    while ((pos = dashed.find('_', pos)) != std::string::npos) {
        dashed[pos] = '-';
        pos++;
    }
    if (dashed.size() >= 3 && !isReservedUsername(dashed)) return dashed;

    // Fallback: add number suffix
    for (int i = 1; i <= maxAttempts; ++i) {
        auto candidate = sanitized + std::to_string(i);
        if (!isReservedUsername(candidate)) return candidate;
    }

    return sanitized + "1";
}

bool isBotUsername(const std::string& username) {
    auto lower = username;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return lower.rfind("bot") == lower.size() - 3 ||
           lower.rfind("bridge") == lower.size() - 6 ||
           lower.find("_bot") != std::string::npos ||
           lower.find("-bot") != std::string::npos;
}

std::string usernameToDisplayName(const std::string& username) {
    std::string result;
    bool capitalize = true;
    for (char c : username) {
        if (c == '_' || c == '.' || c == '-') {
            result += ' ';
            capitalize = true;
        } else if (capitalize) {
            result += std::toupper(static_cast<unsigned char>(c));
            capitalize = false;
        } else {
            result += c;
        }
    }
    return result;
}

} // namespace progressive
