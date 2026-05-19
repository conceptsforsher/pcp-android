#include "progressive/account_utils.hpp"
#include "progressive/json_parser.hpp"
#include <sstream>
#include <regex>
#include <algorithm>
#include <cctype>

namespace progressive {

PasswordValidation validateAccountPassword(const std::string& password, const std::string& username, int minLength) {
    PasswordValidation result;

    if (password.size() < static_cast<size_t>(minLength)) {
        result.tooShort = true;
        result.errorMessage = "Password must be at least " + std::to_string(minLength) + " characters.";
        return result;
    }

    bool hasUpper = false, hasLower = false, hasDigit = false, hasSpecial = false;
    for (char c : password) {
        if (std::isupper(static_cast<unsigned char>(c))) hasUpper = true;
        else if (std::islower(static_cast<unsigned char>(c))) hasLower = true;
        else if (std::isdigit(static_cast<unsigned char>(c))) hasDigit = true;
        else hasSpecial = true;
    }

    result.noUpperCase = !hasUpper;
    result.noLowerCase = !hasLower;
    result.noDigit = !hasDigit;
    result.noSpecialChar = !hasSpecial;

    // Check if password contains username
    if (!username.empty()) {
        auto lowerPass = password;
        auto lowerUser = username;
        std::transform(lowerPass.begin(), lowerPass.end(), lowerPass.begin(), ::tolower);
        std::transform(lowerUser.begin(), lowerUser.end(), lowerUser.begin(), ::tolower);
        result.matchesUsername = (lowerPass.find(lowerUser) != std::string::npos);
    }

    bool hasAll = hasUpper && hasLower && hasDigit && hasSpecial && !result.matchesUsername;
    if (!hasAll) {
        std::ostringstream msg;
        msg << "Password must contain: ";
        if (!hasUpper) msg << "uppercase, ";
        if (!hasLower) msg << "lowercase, ";
        if (!hasDigit) msg << "digit, ";
        if (!hasSpecial) msg << "special character";
        result.errorMessage = msg.str();
        return result;
    }

    result.valid = true;
    return result;
}

std::string buildPasswordChangeBody(const PasswordChange& change, const std::string& authSession) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << "{";
    json << R"("new_password": ")" << esc(change.newPassword) << R"(")";
    if (!change.oldPassword.empty())
        json << R"(,"old_password": ")" << esc(change.oldPassword) << R"(")";
    if (change.logoutOtherDevices)
        json << R"(,"logout_devices": true)";
    if (!authSession.empty())
        json << R"(,"auth": )" << authSession;
    json << "}";
    return json.str();
}

std::string buildDeactivateBody(const std::string& authSession) {
    if (authSession.empty()) return "{}";
    return R"({"auth": )" + authSession + "}";
}

AccountInfo parseAccountInfo(const std::string& apiResponseJson) {
    AccountInfo info;
    info.userId      = parseJsonStringValue(apiResponseJson, "user_id");
    info.deviceId    = parseJsonStringValue(apiResponseJson, "device_id");
    info.isDeactivated = apiResponseJson.find("\"deactivated\": true") != std::string::npos;
    return info;
}

std::string formatAccountInfo(const AccountInfo& info) {
    std::ostringstream out;
    out << "User: " << info.userId << "\n";
    if (!info.displayName.empty()) out << "Name: " << info.displayName << "\n";
    out << "Server: " << info.homeServer << "\n";
    out << "Device: " << info.deviceId << "\n";
    return out.str();
}

bool isValidDisplayName(const std::string& name, int maxLen) {
    if (name.empty() || static_cast<int>(name.size()) > maxLen) return false;
    bool hasNonSpace = false;
    for (char c : name) {
        if (!std::isspace(static_cast<unsigned char>(c))) hasNonSpace = true;
        if (static_cast<unsigned char>(c) < 32) return false; // no control chars
    }
    return hasNonSpace;
}

bool isValidAvatarUrl(const std::string& url) {
    return url.rfind("mxc://", 0) == 0 && url.size() > 6;
}

ThreePidValidation validateThreePid(const std::string& input, bool isEmail) {
    ThreePidValidation result;
    result.address = input;

    if (isEmail) {
        result.medium = "email";
        std::regex emailRe(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
        if (!std::regex_match(input, emailRe)) {
            result.invalidFormat = true;
            result.errorMessage = "Invalid email address.";
            return result;
        }
    } else {
        result.medium = "msisdn";
        std::regex phoneRe(R"(^\+\d{7,15}$)");
        if (!std::regex_match(input, phoneRe)) {
            result.invalidFormat = true;
            result.errorMessage = "Invalid phone number. Use +1234567890 format.";
            return result;
        }
    }

    result.valid = true;
    return result;
}

std::string buildThreePidRequestBody(const ThreePidRequest& req) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"client_secret": ")" << esc(req.clientSecret) << R"(")";
    json << R"(,"send_attempt": )" << req.sendAttempt;
    if (!req.idServer.empty())
        json << R"(,"id_server": ")" << esc(req.idServer) << R"(")";
    if (!req.homeServer.empty())
        json << R"(,"next_link": ")" << esc(req.homeServer) << R"(")";
    json << "}";
    return json.str();
}

std::string buildThreePidBindBody(const std::string& clientSecret, const std::string& sid,
    const std::string& idServer) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"client_secret": ")" << esc(clientSecret) << R"(")";
    json << R"(,"sid": ")" << esc(sid) << R"(")";
    if (!idServer.empty())
        json << R"(,"id_server": ")" << esc(idServer) << R"(")";
    json << "}";
    return json.str();
}

std::string parseThreePidSid(const std::string& responseJson) {
    return parseJsonStringValue(responseJson, "sid");
}

} // namespace progressive
