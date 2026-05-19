#include "progressive/account_export.hpp"
#include "progressive/hash_utils.hpp"
#include <sstream>
#include <cstring>
#include <vector>

namespace progressive {

std::string accountToJson(const AccountData& data) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };
    std::ostringstream json;
    json << "{";
    json << R"("userId": ")" << esc(data.userId) << R"(",)";
    json << R"("accessToken": ")" << esc(data.accessToken) << R"(",)";
    json << R"("refreshToken": ")" << esc(data.refreshToken) << R"(",)";
    json << R"("homeServerUrl": ")" << esc(data.homeServerUrl) << R"(",)";
    json << R"("deviceId": ")" << esc(data.deviceId) << R"(",)";
    json << R"("deviceName": ")" << esc(data.deviceName) << R"(",)";
    json << R"("displayName": ")" << esc(data.displayName) << R"(",)";
    json << R"("avatarUrl": ")" << esc(data.avatarUrl) << R"(",)";
    json << R"("includeCache": )" << (data.includeCache ? "true" : "false");
    json << "}";
    return json.str();
}

AccountData jsonToAccount(const std::string& json) {
    AccountData data;
    // Manual string parsing (no nlohmann/json dependency)
    auto getStr = [&](const std::string& key) -> std::string {
        auto search = "\"" + key + "\": \"";
        auto pos = json.find(search);
        if (pos == std::string::npos) return "";
        pos += search.size();
        auto end = json.find('"', pos);
        return json.substr(pos, end - pos);
    };
    data.userId = getStr("userId");
    data.accessToken = getStr("accessToken");
    data.refreshToken = getStr("refreshToken");
    data.homeServerUrl = getStr("homeServerUrl");
    data.deviceId = getStr("deviceId");
    data.deviceName = getStr("deviceName");
    data.displayName = getStr("displayName");
    data.avatarUrl = getStr("avatarUrl");
    if (json.find("\"includeCache\": true") != std::string::npos) data.includeCache = true;
    return data;
}

std::string encryptAccountData(const AccountData& data, const std::string& passphrase) {
    auto json = accountToJson(data);
    // XOR with passphrase-derived key
    for (size_t i = 0; i < json.size(); ++i) {
        json[i] ^= passphrase[i % passphrase.size()];
    }
    auto bytes = std::vector<uint8_t>(json.begin(), json.end());
    return progressive::base64Encode(bytes);
}

AccountData decryptAccountData(const std::string& encrypted, const std::string& passphrase) {
    auto decodedBytes = progressive::base64Decode(encrypted);
    std::string decoded(decodedBytes.begin(), decodedBytes.end());
    if (decoded.empty()) return {};

    for (size_t i = 0; i < decoded.size(); ++i) {
        decoded[i] ^= passphrase[i % passphrase.size()];
    }

    auto data = jsonToAccount(decoded);
    // Basic validation: must have userId and accessToken
    if (data.userId.empty() || data.accessToken.empty() || data.homeServerUrl.empty()) {
        return {};
    }
    return data;
}

} // namespace progressive
