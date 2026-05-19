#include "progressive/backup_utils.hpp"
#include "progressive/key_backup.hpp"
#include "progressive/json_parser.hpp"
#include <sstream>
#include <algorithm>
#include <random>
#include <cctype>

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

// ---- Backup Recovery Key (higher-level, structured) ----
// Original Kotlin (RecoveryKey.kt + BackupRecoveryKey.kt):
//   fun computeRecoveryKey(curve25519Key: ByteArray): String
//   fun fromBase58(key: String): BackupRecoveryKey

BackupRecoveryKey generateRecoveryKey() {
    BackupRecoveryKey result;
    result.algorithm = "m.megolm_backup.v1.curve25519-aes-sha2";
    result.fromPassphrase = false;

    // Generate 32 random bytes for Curve25519 private key
    std::vector<uint8_t> keyBytes(32);
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint8_t> dist(0, 255);
    for (int i = 0; i < 32; ++i) {
        keyBytes[i] = dist(gen);
    }

    // Clamp Curve25519 key (ensure valid: clear low 3 bits, set bit 254, clear bit 255)
    keyBytes[0] &= 0xF8;
    keyBytes[31] &= 0x7F;
    keyBytes[31] |= 0x40;

    result.keyBytes = std::string(keyBytes.begin(), keyBytes.end());

    // Compute recovery key from the Curve25519 key using the low-level function
    result.keyBase58 = computeRecoveryKey(result.keyBytes);

    return result;
}

// Original Kotlin (RecoveryKey.kt:isValidRecoveryKey + extractCurveKeyFromRecoveryKey):
//   fun isValidRecoveryKey(recoveryKey: String?): Boolean { return extractCurveKeyFromRecoveryKey(recoveryKey) != null }
//   fun extractCurveKeyFromRecoveryKey(recoveryKey: String?): ByteArray? { ... }

RecoveryKeyInfo validateRecoveryKeyInfo(const BackupRecoveryKey& key) {
    RecoveryKeyInfo info;
    info.keyBase58 = key.keyBase58;

    // Remove spaces for validation
    std::string clean;
    for (char c : key.keyBase58) {
        if (c != ' ' && c != '-') clean += c;
    }

    // Format for display
    std::string formatted;
    for (size_t i = 0; i < clean.size(); ++i) {
        if (i > 0 && i % 4 == 0) formatted += ' ';
        formatted += clean[i];
    }
    info.format = formatted;

    // Check if key is empty
    if (clean.empty()) {
        info.isComplete = false;
        info.errorMessage = "Key is empty";
        return info;
    }

    // Validate base58 characters
    for (char c : clean) {
        if (!isValidBase58Char(c)) {
            info.isComplete = false;
            info.errorMessage = "Invalid character in recovery key: '" + std::string(1, c) + "'";
            return info;
        }
    }

    // Try to extract the curve key (validates header and parity)
    auto curveKey = extractCurveKeyFromRecoveryKey(clean);
    if (curveKey.empty()) {
        // Check if this might be a partial/incomplete key
        if (clean.size() < 50) {
            info.isComplete = false;
            info.missingChars = std::string(58 - clean.size(), '?');
            info.errorMessage = "Recovery key is incomplete (" + std::to_string(clean.size()) + "/58 chars)";
        } else {
            info.isComplete = false;
            info.hasValidChecksum = false;
            info.errorMessage = "Invalid recovery key (header or parity check failed)";
        }
        return info;
    }

    // Key is valid and complete
    info.isComplete = true;
    info.hasValidChecksum = true;

    return info;
}

// Original Kotlin (KeysBackupSetupSharedViewModel.kt):
//   fun formatRecoveryKey(raw: String): String { return raw.chunked(4).joinToString(" ") }

std::string formatRecoveryKeyDisplay(const BackupRecoveryKey& key) {
    std::string clean;
    for (char c : key.keyBase58) {
        if (c != ' ' && c != '-') clean += static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    }

    if (clean.empty()) return "";

    std::ostringstream out;
    for (size_t i = 0; i < clean.size(); ++i) {
        if (i > 0 && i % 4 == 0) out << ' ';
        out << clean[i];
    }
    return out.str();
}

// Original Kotlin (RecoveryKey.kt:extractCurveKeyFromRecoveryKey + fromBase58):
//   fun extractCurveKeyFromRecoveryKey(recoveryKey: String?): ByteArray?
//   fun fromBase58(key: String): BackupRecoveryKey

BackupRecoveryKey parseRecoveryKey(const std::string& input) {
    BackupRecoveryKey result;
    result.algorithm = "m.megolm_backup.v1.curve25519-aes-sha2";
    result.fromPassphrase = false;

    if (input.empty()) return result;

    // Step 1: Clean the input — remove spaces, dashes, convert to raw base58
    std::string clean;
    for (char c : input) {
        if (c != ' ' && c != '-' && c != '\t' && c != '\n' && c != '\r') {
            clean += c;
        }
    }

    if (clean.empty()) return result;

    // Step 2: Store the cleaned key for base58 representation
    result.keyBase58 = clean;

    // Step 3: Extract the Curve25519 private key using low-level utility
    auto extractedKey = extractCurveKeyFromRecoveryKey(clean);
    if (!extractedKey.empty()) {
        result.keyBytes = extractedKey;
        // Recompute the proper base58 representation
        result.keyBase58 = computeRecoveryKey(result.keyBytes);
    }
    // If extraction failed, keyBytes remains empty (invalid key)

    return result;
}

} // namespace progressive
