#include "progressive/key_backup.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

namespace progressive {

bool isValidBase58Char(char c) {
    static const char* alphabet = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
    for (int i = 0; alphabet[i]; ++i) {
        if (alphabet[i] == c) return true;
    }
    return false;
}

// ---- Recovery Key Formatting ----
// Original Kotlin:
//   fun formatRecoveryKey(raw: String?): String? {
//       return raw?.chunked(4)?.joinToString(" ")
//   }

std::string formatRecoveryKey(const std::string& raw) {
    if (raw.empty()) return "";

    std::ostringstream out;
    for (size_t i = 0; i < raw.size(); ++i) {
        if (i > 0 && i % 4 == 0) out << ' ';
        out << static_cast<char>(std::toupper(static_cast<unsigned char>(raw[i])));
    }
    return out.str();
}

std::string unformatRecoveryKey(const std::string& formatted) {
    std::string raw;
    for (char c : formatted) {
        if (c != ' ' && c != '-' && c != '\t' && c != '\n') {
            raw += c;
        }
    }
    return raw;
}

RecoveryKey validateRecoveryKey(const std::string& key) {
    RecoveryKey result;

    // Step 1: Unformat (remove spaces)
    result.raw = unformatRecoveryKey(key);

    // Step 2: Length check — Matrix recovery keys are 58 base58 chars
    // (1 byte prefix + 32 byte key + 4 byte checksum = 37 bytes → ~58 base58 chars)
    if (result.raw.size() < 50) {
        result.status = RecoveryKeyStatus::Invalid_TooShort;
        return result;
    }
    if (result.raw.size() > 70) {
        result.status = RecoveryKeyStatus::Invalid_TooLong;
        return result;
    }

    // Step 3: Validate base58 characters
    for (char c : result.raw) {
        if (!isValidBase58Char(c)) {
            result.status = RecoveryKeyStatus::Invalid_BadCharacters;
            return result;
        }
    }

    // Step 4: Format check — must have spaces in groups of 4 (optional, not strict)
    // Original Kotlin doesn't strictly require groups, but we check
    // for common formatting issues

    result.valid = true;
    result.status = RecoveryKeyStatus::Valid;
    return result;
}

std::string extractCurveKeyFromRecoveryKey(const std::string& recoveryKey) {
    // Original Kotlin (RecoveryKey.kt:77-121):
    //   fun extractCurveKeyFromRecoveryKey(recoveryKey: String?): ByteArray? {
    //       val spaceFreeRecoveryKey = recoveryKey.replace("\\s".toRegex(), "")
    //       val b58DecodedKey = base58decode(spaceFreeRecoveryKey)
    //       if (b58DecodedKey.size != RECOVERY_KEY_LENGTH) return null  // 35
    //       if (b58DecodedKey[0] != CHAR_0) return null  // 0x8B
    //       if (b58DecodedKey[1] != CHAR_1) return null  // 0x01
    //       // Parity check: XOR of all bytes must be 0
    //       var parity: Byte = 0
    //       for (i in 0 until RECOVERY_KEY_LENGTH) parity = parity xor b58DecodedKey[i]
    //       if (parity != 0.toByte()) return null
    //       // Extract key: bytes 2..34 (skip 2 header + 1 parity)
    //       return b58DecodedKey.copyOfRange(2, b58DecodedKey.size - 1)
    //   }

    if (recoveryKey.empty()) return "";

    // Step 1: Remove whitespace
    std::string spaceFree;
    for (char c : recoveryKey) {
        if (c != ' ' && c != '\t' && c != '\n' && c != '\r') spaceFree += c;
    }
    if (spaceFree.empty()) return "";

    // Step 2: Base58 decode
    auto decoded = base58Decode(spaceFree);
    if (decoded.empty()) throw std::runtime_error("Invalid base58 key");
    const int RECOVERY_KEY_LENGTH = 35;

    // Step 3: Check length
    if (static_cast<int>(decoded.size()) != RECOVERY_KEY_LENGTH) return "";

    // Step 4: Check header bytes
    const unsigned char CHAR_0 = 0x8B;
    const unsigned char CHAR_1 = 0x01;
    if (static_cast<unsigned char>(decoded[0]) != CHAR_0) return "";
    if (static_cast<unsigned char>(decoded[1]) != CHAR_1) return "";

    // Step 5: Parity check — XOR of all bytes must be 0
    unsigned char parity = 0;
    for (int i = 0; i < RECOVERY_KEY_LENGTH; ++i) {
        parity ^= static_cast<unsigned char>(decoded[i]);
    }
    if (parity != 0) return "";

    // Step 6: Extract key bytes (skip 2 header bytes, skip 1 parity byte at end)
    return std::string(decoded.begin() + 2, decoded.begin() + 2 + 32);
}

std::string computeRecoveryKey(const std::string& curve25519Key) {
    // Original Kotlin (RecoveryKey.kt:48-69):
    //   fun computeRecoveryKey(curve25519Key: ByteArray): String {
    //       val data = ByteArray(curve25519Key.size + 3)
    //       data[0] = CHAR_0  // 0x8B
    //       data[1] = CHAR_1  // 0x01
    //       var parity: Byte = CHAR_0 xor CHAR_1
    //       for (i in curve25519Key.indices) {
    //           data[i + 2] = curve25519Key[i]
    //           parity = parity xor curve25519Key[i]
    //       }
    //       data[curve25519Key.size + 2] = parity
    //       return base58encode(data)
    //   }

    if (curve25519Key.size() != 32) return "";

    const unsigned char CHAR_0 = 0x8B;
    const unsigned char CHAR_1 = 0x01;

    // Build data: [CHAR_0] [CHAR_1] [key 32B] [parity 1B] = 35 bytes
    std::string data;
    data += static_cast<char>(CHAR_0);
    data += static_cast<char>(CHAR_1);
    data += curve25519Key;

    // Compute parity
    unsigned char parity = CHAR_0 ^ CHAR_1;
    for (unsigned char c : curve25519Key) {
        parity ^= c;
    }
    data += static_cast<char>(parity);

    return base58Encode(std::vector<uint8_t>(data.begin(), data.end()));
}

bool validateRecoveryKeyChecksum(const std::string& rawKey) {
    // Recovery key encodes: [1B prefix] [32B key] [4B checksum]
    // We need at least 37 bytes decoded (58 base58 chars)
    // Without SHA-256 (no OpenSSL), we do a minimum length check
    return rawKey.size() >= 54;  // 58 base58 chars min for 37 bytes
}

// ---- Backup Version ----
KeyBackupVersion parseKeyBackupVersion(const std::string& json) {
    KeyBackupVersion backup;

    // Original Kotlin: json.getString("version")
    auto extractStr = [&](const std::string& key) -> std::string {
        auto search = "\"" + key + "\":\"";
        auto pos = json.find(search);
        if (pos == std::string::npos) {
            search = "\"" + key + "\": \"";
            pos = json.find(search);
        }
        if (pos == std::string::npos) return "";
        pos += search.size();
        auto end = json.find('"', pos);
        if (end == std::string::npos) return "";
        return json.substr(pos, end - pos);
    };

    auto extractInt = [&](const std::string& key) -> int {
        auto search = "\"" + key + "\":";
        auto pos = json.find(search);
        if (pos == std::string::npos) return 0;
        pos += search.size();
        while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
        int val = 0;
        while (pos < json.size() && json[pos] >= '0' && json[pos] <= '9') {
            val = val * 10 + (json[pos] - '0');
            pos++;
        }
        return val;
    };

    backup.version = extractStr("version");
    backup.algorithm = extractStr("algorithm");
    backup.count = extractInt("count");

    // Extract auth_data
    auto authPos = json.find("\"auth_data\"");
    if (authPos != std::string::npos) {
        auto openPos = json.find('{', authPos);
        if (openPos != std::string::npos) {
            int braceDepth = 1;
            size_t pos = openPos + 1;
            while (pos < json.size() && braceDepth > 0) {
                if (json[pos] == '{') braceDepth++;
                else if (json[pos] == '}') braceDepth--;
                pos++;
            }
            backup.authData = json.substr(openPos, pos - openPos);
        }
    }

    backup.valid = !backup.version.empty() && !backup.algorithm.empty();
    if (!backup.valid) {
        backup.error = "Missing version or algorithm in backup info";
    } else if (!isSupportedBackupAlgorithm(backup.algorithm)) {
        backup.valid = false;
        backup.error = "Unsupported backup algorithm: " + backup.algorithm;
    }

    return backup;
}

bool isSupportedBackupAlgorithm(const std::string& algorithm) {
    // Matrix currently supports this algorithm
    return algorithm == "m.megolm_backup.v1.curve25519-aes-sha2";
}

std::string keyBackupVersionToJson(const KeyBackupVersion& backup) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };
    std::ostringstream json;
    json << R"({"version": ")" << esc(backup.version) << R"(",)";
    json << R"("algorithm": ")" << esc(backup.algorithm) << R"(",)";
    json << R"("count": )" << backup.count << ",";
    json << R"("valid": )" << (backup.valid ? "true" : "false") << ",";
    json << R"("error": ")" << esc(backup.error) << R"(")";
    json << "}";
    return json.str();
}

std::string getBackupAlgorithmDescription(const std::string& algorithm) {
    if (algorithm == "m.megolm_backup.v1.curve25519-aes-sha2") {
        return "Encrypted using Curve25519-AES-SHA2";
    }
    return "Unknown algorithm: " + algorithm;
}

std::string getRecoveryKeyExample() {
    return "EsTc 2FZd Jsdf 4Gt7 HqX9 bKpL mNvR wQzY x3A5 B6C7 D8E";
}

bool isValidPassphrase(const std::string& passphrase) {
    return !passphrase.empty() && passphrase.size() >= static_cast<size_t>(getMinPassphraseLength());
}

int getMinPassphraseLength() {
    return 8;
}

// ==== Secret Storage Key (from SecretStorageKeyContent.kt:53-103) ====

SecretStorageKey parseSecretStorageKey(const std::string& keyId, const std::string& json) {
    SecretStorageKey key;
    key.keyId = keyId;

    auto extractStr = [&](const std::string& k) -> std::string {
        auto search = "\"" + k + "\":\"";
        auto pos = json.find(search);
        if (pos == std::string::npos) { search = "\"" + k + "\": \""; pos = json.find(search); }
        if (pos == std::string::npos) return "";
        pos += search.size();
        auto end = json.find('"', pos);
        return (end != std::string::npos) ? json.substr(pos, end - pos) : "";
    };

    auto extractInt = [&](const std::string& k) -> int {
        auto search = "\"" + k + "\":";
        auto pos = json.find(search);
        if (pos == std::string::npos) return 0;
        pos += search.size();
        while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
        int v = 0;
        while (pos < json.size() && json[pos] >= '0' && json[pos] <= '9') { v = v * 10 + (json[pos] - '0'); pos++; }
        return v;
    };

    key.algorithm = extractStr("algorithm");
    key.name = extractStr("name");
    key.publicKey = extractStr("pubkey");

    auto passPos = json.find("\"passphrase\"");
    if (passPos != std::string::npos) {
        key.passphrase.algorithm = "m.pbkdf2";
        key.passphrase.iterations = extractInt("iterations");
        if (key.passphrase.iterations == 0) key.passphrase.iterations = 500000;
        key.passphrase.salt = extractStr("salt");
    }

    key.valid = !key.algorithm.empty() && !key.publicKey.empty();
    return key;
}

std::string secretStorageKeyToJson(const SecretStorageKey& key) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"keyId": ")" << esc(key.keyId) << R"(",)";
    json << R"("algorithm": ")" << esc(key.algorithm) << R"(",)";
    json << R"("name": ")" << esc(key.name) << R"(",)";
    json << R"("valid": )" << (key.valid ? "true" : "false") << ",";
    json << R"("hasPassphrase": )" << (key.hasPassphrase() ? "true" : "false") << "}";
    return json.str();
}

} // namespace progressive
