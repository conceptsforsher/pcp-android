#ifndef PROGRESSIVE_ACCOUNT_EXPORT_HPP
#define PROGRESSIVE_ACCOUNT_EXPORT_HPP

#include <string>

namespace progressive {

struct AccountData {
    std::string userId;
    std::string accessToken;
    std::string refreshToken;
    std::string homeServerUrl;
    std::string deviceId;
    std::string deviceName;
    std::string displayName;
    std::string avatarUrl;
    bool includeCache = false;   // export cached timeline data too
};

// Serialize account data to an encrypted JSON blob.
// Uses a simple XOR + base64 scheme (sufficient for local file storage).
// Key: user-chosen passphrase.
std::string encryptAccountData(const AccountData& data, const std::string& passphrase);

// Decrypt an encrypted account blob back to AccountData.
// Returns empty AccountData on failure (wrong passphrase or corruption).
AccountData decryptAccountData(const std::string& encrypted, const std::string& passphrase);

// Serialize account to JSON (before encryption).
std::string accountToJson(const AccountData& data);
AccountData jsonToAccount(const std::string& json);

// Simple base64 encode/decode (no external deps).
// NOTE: base64 functions are now in hash_utils.hpp
// std::string base64Encode(const std::string& input);
// std::string base64Decode(const std::string& input);

} // namespace progressive

#endif // PROGRESSIVE_ACCOUNT_EXPORT_HPP
