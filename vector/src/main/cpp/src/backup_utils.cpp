#include "progressive/backup_utils.hpp"
#include "progressive/key_backup.hpp"
#include "progressive/json_parser.hpp"
#include <sstream>
#include <algorithm>
#include <regex>

namespace progressive {

BackupInfo parseBackupInfo(const std::string& apiResponseJson) {
    BackupInfo info;

    info.version   = parseJsonStringValue(apiResponseJson, "version");
    info.algorithm = parseJsonStringValue(apiResponseJson, "algorithm");
    info.authData  = parseJsonStringValue(apiResponseJson, "auth_data");

    auto count     = parseJsonStringValue(apiResponseJson, "count");
    if (!count.empty()) info.totalKeys = std::stoi(count);

    auto etag      = parseJsonStringValue(apiResponseJson, "etag");
    if (!etag.empty()) info.backedUpKeys = std::stoi(etag);

    // Check if verified
    auto verified = parseJsonStringValue(apiResponseJson, "verified");
    info.verified = (verified == "true");

    auto trusted = parseJsonStringValue(apiResponseJson, "trusted");
    info.trusted = (trusted == "true");

    return info;
}

std::string formatBackupStats(const BackupInfo& info) {
    std::ostringstream out;
    out << "Key Backup: " << info.version << "\n";
    out << "Algorithm: " << info.algorithm << "\n";
    out << "Progress: " << info.backedUpKeys << "/" << info.totalKeys
        << " (" << static_cast<int>(computeBackupProgress(info)) << "%)\n";
    out << "Verified: " << (info.verified ? "Yes" : "No") << "\n";
    out << "Trusted: " << (info.trusted ? "Yes" : "No") << "\n";
    return out.str();
}

double computeBackupProgress(const BackupInfo& info) {
    if (info.totalKeys <= 0) return 0.0;
    return (info.backedUpKeys * 100.0) / info.totalKeys;
}

bool needsBackupAttention(const BackupInfo& info, double minProgress) {
    if (!info.verified && !info.trusted) return true;
    if (computeBackupProgress(info) < minProgress) return true;
    return false;
}

std::string buildCreateBackupBody(const std::string& algorithm, const std::string& authData) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << "{";
    json << R"("algorithm": ")" << esc(algorithm) << R"(",)";
    json << R"("auth_data": )" << authData;
    json << "}";
    return json.str();
}

bool isValidRecoveryKey(const std::string& key) {
    // Recovery keys are base58, start with "Es" and are ~48 chars
    if (key.size() < 40 || key.size() > 60) return false;
    if (key.substr(0, 2) != "Es") return false;
    for (char c : key) {
        if (!std::isalnum(static_cast<unsigned char>(c)) && c != ' ') return false;
    }
    return true;
}

std::string extractDefaultSecretKey(const std::string& accountDataJson) {
    // Parse m.secret_storage.default_key from account data
    auto content = parseJsonStringValue(accountDataJson, "content");
    if (content.empty()) return {};

    std::string searchIn = "{" + content + "}";
    return parseJsonStringValue(searchIn, "key");
}

SecretInfo extractSecret(const std::string& secretEventJson, const std::string& secretId) {
    SecretInfo info;
    info.secretId = secretId;

    auto content = parseJsonStringValue(secretEventJson, "content");
    if (content.empty()) return info;

    std::string searchIn = "{" + content + "}";
    auto encrypted = parseJsonStringValue(searchIn, "encrypted");
    if (encrypted.empty()) {
        // Try extracting from the specific secret key
        auto secretData = parseJsonStringValue(searchIn, secretId);
        if (!secretData.empty()) {
            std::string inner = "{" + secretData + "}";
            info.encryptedContent = parseJsonStringValue(inner, "encrypted");
        }
    } else {
        info.encryptedContent = encrypted;
    }

    info.found = !info.encryptedContent.empty();
    return info;
}

std::string buildStoreSecretBody(const std::string& secretId, const std::string& encryptedContent) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << "{";
    json << R"("encrypted": {)";
    json << R"(")" << esc(secretId) << R"(": {)";
    json << R"("encrypted": ")" << esc(encryptedContent) << R"(")";
    json << "}}}";
    return json.str();
}

bool hasCrossSigningSecrets(const std::string& accountDataJson) {
    return accountDataJson.find("m.cross_signing.master") != std::string::npos;
}

std::string validateAndFormatRecoveryKey(const std::string& rawKey) {
    std::string clean;
    for (char c : rawKey) if (c != ' ') clean += c;

    std::ostringstream os;
    if (!isValidRecoveryKey(rawKey)) {
        os << R"({"valid":false,"formatted":"","error":"Invalid recovery key format"})";
        return os.str();
    }

    std::string formatted;
    for (size_t i = 0; i < clean.size(); i++) {
        if (i > 0 && i % 4 == 0) formatted += ' ';
        formatted += clean[i];
    }

    os << R"({"valid":true,"formatted":")" << formatted << R"(","error":""})";
    return os.str();
}

} // namespace progressive
