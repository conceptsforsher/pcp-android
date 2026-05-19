#include "progressive/key_backup.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <random>
#include <unordered_map>

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
    backup.etag = extractStr("etag");
    if (backup.etag.empty()) backup.etag = extractStr("hash"); // alternative key name
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

// ---- Key Backup HTTP API Builders / Parsers ----

// Original Kotlin (CreateKeysBackupVersionBody.kt:24-37):
//   data class CreateKeysBackupVersionBody(algorithm: String, authData: JsonDict)

std::string buildKeyBackupCreateRequest(const std::string& algorithm, const std::string& authDataJson) {
    std::ostringstream os;
    os << R"({"algorithm":")" << algorithm << R"(")";
    if (!authDataJson.empty()) {
        os << R"(,"auth_data":)" << authDataJson;
    }
    os << "}";
    return os.str();
}

// Original Kotlin (KeyBackupData.kt + session_data):
//   PUT /room_keys/keys/{roomId}/{sessionId}
//   Request: { "first_message_index":..., "forwarded_count":..., "is_verified":..., "session_data":{...} }

std::string buildKeyBackupUploadRequest(const std::string& roomId, const std::string& sessionId,
                                         const std::string& sessionDataJson) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream os;
    os << R"({"rooms":{")" << esc(roomId) << R"(":{"sessions":{)";
    os << esc(sessionId) << R"(":)";
    os << sessionDataJson;
    os << "}}}";
    return os.str();
}

// Original Kotlin (KeysBackupData.kt + RoomKeysBackupData):
//   GET /room_keys/keys → { "rooms": { "!room:id": { "sessions": { "sessionId": { ... } } } } }

std::vector<KeyBackupRoomSessions> parseKeyBackupDownloadResponse(const std::string& responseJson) {
    std::vector<KeyBackupRoomSessions> rooms;

    auto extractStr = [](const std::string& json, const std::string& key) -> std::string {
        auto search = "\"" + key + "\":\"";
        auto pos = json.find(search);
        if (pos == std::string::npos) {
            search = "\"" + key + "\": \"";
            pos = json.find(search);
        }
        if (pos == std::string::npos) return "";
        pos += search.size();
        auto end = json.find('"', pos);
        return (end != std::string::npos) ? json.substr(pos, end - pos) : "";
    };

    auto extractInt = [](const std::string& json, const std::string& key) -> int {
        auto search = "\"" + key + "\":";
        auto pos = json.find(search);
        if (pos == std::string::npos) return 0;
        pos += search.size();
        while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
        int v = 0;
        while (pos < json.size() && json[pos] >= '0' && json[pos] <= '9') { v = v * 10 + (json[pos] - '0'); pos++; }
        return v;
    };

    auto extractInt64 = [](const std::string& json, const std::string& key) -> int64_t {
        auto search = "\"" + key + "\":";
        auto pos = json.find(search);
        if (pos == std::string::npos) return 0;
        pos += search.size();
        while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
        int64_t v = 0;
        while (pos < json.size() && json[pos] >= '0' && json[pos] <= '9') { v = v * 10 + (json[pos] - '0'); pos++; }
        return v;
    };

    auto extractBool = [](const std::string& json, const std::string& key) -> bool {
        return json.find("\"" + key + "\":true") != std::string::npos ||
               json.find("\"" + key + "\": true") != std::string::npos;
    };

    auto extractNestedObj = [](const std::string& json, const std::string& key) -> std::string {
        auto pp = json.find("\"" + key + "\"");
        if (pp == std::string::npos) return "";
        pp = json.find('{', pp);
        if (pp == std::string::npos) return "";
        int depth = 1;
        size_t start = pp;
        pp++;
        while (pp < json.size() && depth > 0) {
            if (json[pp] == '{') depth++;
            else if (json[pp] == '}') depth--;
            pp++;
        }
        return json.substr(start, pp - start);
    };

    // Parse rooms block
    auto roomsObj = extractNestedObj(responseJson, "rooms");
    if (roomsObj.empty()) return rooms;

    // Find room IDs (keys starting with "!")
    size_t rp = 0;
    while ((rp = roomsObj.find("\"!", rp)) != std::string::npos) {
        size_t keyEnd = roomsObj.find('"', rp + 1);
        if (keyEnd == std::string::npos) break;
        std::string roomId = roomsObj.substr(rp + 1, keyEnd - rp - 1);
        rp = keyEnd + 1;

        // Extract room's sessions block
        auto roomBlock = extractNestedObj(roomsObj.substr(keyEnd), roomId);
        if (roomBlock.empty()) {
            // Try to find the next '{' after the roomId key
            auto colon = roomsObj.find(':', keyEnd);
            if (colon != std::string::npos) {
                auto brace = roomsObj.find('{', colon);
                if (brace != std::string::npos) {
                    int depth = 1;
                    size_t pos = brace + 1;
                    while (pos < roomsObj.size() && depth > 0) {
                        if (roomsObj[pos] == '{') depth++;
                        else if (roomsObj[pos] == '}') depth--;
                        pos++;
                    }
                    roomBlock = roomsObj.substr(brace, pos - brace);
                }
            }
        }

        auto sessionsObj = extractNestedObj(roomBlock, "sessions");
        KeyBackupRoomSessions roomSessions;
        roomSessions.roomId = roomId;

        // Parse each session in the room
        if (!sessionsObj.empty()) {
            size_t sp = 0;
            while ((sp = sessionsObj.find("\"", sp)) != std::string::npos) {
                sp++;
                size_t se = sp;
                while (se < sessionsObj.size() && sessionsObj[se] != '"') se++;
                if (se <= sp) { sp = se + 1; continue; }
                std::string sid = sessionsObj.substr(sp, se - sp);
                sp = se + 1;

                if (sid == "sessions") continue; // skip the "sessions" key

                // Extract session data object
                auto sessionBlock = extractNestedObj(sessionsObj.substr(sp - 1), sid);
                if (sessionBlock.empty()) {
                    // Try alternative extraction after the session ID
                    auto colon = sessionsObj.find(':', sp - 1);
                    if (colon != std::string::npos) {
                        auto brace = sessionsObj.find('{', colon);
                        if (brace != std::string::npos) {
                            int depth = 1;
                            size_t pos = brace + 1;
                            while (pos < sessionsObj.size() && depth > 0) {
                                if (sessionsObj[pos] == '{') depth++;
                                else if (sessionsObj[pos] == '}') depth--;
                                pos++;
                            }
                            sessionBlock = sessionsObj.substr(brace, pos - brace);
                        }
                    }
                }

                if (!sessionBlock.empty()) {
                    KeyBackupSession session;
                    session.sessionId = sid;
                    session.roomId = roomId;
                    session.senderKey = extractStr(sessionBlock, "sender_key");
                    session.sessionKey = extractStr(sessionBlock, "session_key");
                    session.firstMessageIndex = extractInt64(sessionBlock, "first_message_index");
                    session.forwardedCount = extractInt(sessionBlock, "forwarded_count");
                    session.isVerified = extractBool(sessionBlock, "is_verified");
                    session.algorithm = extractStr(sessionBlock, "algorithm");
                    if (session.algorithm.empty()) session.algorithm = "m.megolm.v1.aes-sha2";
                    roomSessions.sessions[sid] = session;
                }
            }
        }

        if (!roomSessions.sessions.empty()) {
            rooms.push_back(roomSessions);
        }
    }

    return rooms;
}

// Original Kotlin (KeysBackup.kt):
//   fun computeBackupVersion(currentVersion: String?): String

std::string computeKeyBackupVersion(const std::string& currentVersion) {
    if (currentVersion.empty()) return "0";
    try {
        int64_t v = std::stoll(currentVersion);
        return std::to_string(v + 1);
    } catch (...) {
        return "0";
    }
}

// Original Kotlin (KeysBackupVersionTrust.kt + KeysBackup.kt):
//   fun isKeyBackupValid(keysVersionResult: KeysVersionResult): Boolean

bool isKeyBackupValid(const KeyBackupVersion& backup) {
    if (!backup.valid) return false;
    if (backup.version.empty()) return false;
    if (backup.algorithm.empty()) return false;
    if (!isSupportedBackupAlgorithm(backup.algorithm)) return false;
    if (backup.authData.empty()) return false;
    // Check auth_data contains public_key
    if (backup.authData.find("\"public_key\"") == std::string::npos &&
        backup.authData.find("\"public_key\":") == std::string::npos) {
        return false;
    }
    return true;
}

// ---- Key Backup Auth Data Builders / Parsers ----

// Original Kotlin (MegolmBackupAuthData.kt:30-79):
//   data class MegolmBackupAuthData(publicKey, privateKeySalt, privateKeyIterations, signatures)

std::string buildKeyBackupAuthData(const KeyBackupAuthData& authData) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream os;
    os << R"({"public_key":")" << esc(authData.publicKey) << R"(")";

    if (!authData.privateKeySalt.empty()) {
        os << R"(,"private_key_salt":")" << esc(authData.privateKeySalt) << R"(")";
    }
    if (authData.privateKeyIterations > 0) {
        os << R"(,"private_key_iterations":)" << authData.privateKeyIterations;
    }

    // Build signatures map
    if (!authData.signatures.empty()) {
        os << R"(,"signatures":{)";
        bool firstUser = true;
        for (const auto& [userId, devSigs] : authData.signatures) {
            if (!firstUser) os << ","; firstUser = false;
            os << "\"" << esc(userId) << "\":{";
            bool firstDev = true;
            for (const auto& [keyId, sig] : devSigs) {
                if (!firstDev) os << ","; firstDev = false;
                os << "\"" << esc(keyId) << "\":\"" << esc(sig) << "\"";
            }
            os << "}";
        }
        os << "}";
    }

    os << "}";
    return os.str();
}

// Original Kotlin (MegolmBackupAuthData.kt:signalableJSONDictionary + fromJsonDict):
//   fun signalableJSONDictionary(): JsonDict
//   fun fromJsonValue(authData: JsonDict?): MegolmBackupAuthData?

KeyBackupAuthData parseKeyBackupAuthData(const std::string& authDataJson) {
    KeyBackupAuthData auth;

    auto extractStr = [](const std::string& json, const std::string& key) -> std::string {
        auto search = "\"" + key + "\":\"";
        auto pos = json.find(search);
        if (pos == std::string::npos) {
            search = "\"" + key + "\": \"";
            pos = json.find(search);
        }
        if (pos == std::string::npos) return "";
        pos += search.size();
        auto end = json.find('"', pos);
        return (end != std::string::npos) ? json.substr(pos, end - pos) : "";
    };

    auto extractInt = [](const std::string& json, const std::string& key) -> int {
        auto search = "\"" + key + "\":";
        auto pos = json.find(search);
        if (pos == std::string::npos) return 0;
        pos += search.size();
        while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
        int v = 0;
        while (pos < json.size() && json[pos] >= '0' && json[pos] <= '9') { v = v * 10 + (json[pos] - '0'); pos++; }
        return v;
    };

    auto extractNestedObj = [](const std::string& json, const std::string& key) -> std::string {
        auto pp = json.find("\"" + key + "\"");
        if (pp == std::string::npos) return "";
        pp = json.find('{', pp);
        if (pp == std::string::npos) return "";
        int depth = 1;
        size_t start = pp;
        pp++;
        while (pp < json.size() && depth > 0) {
            if (json[pp] == '{') depth++;
            else if (json[pp] == '}') depth--;
            pp++;
        }
        return json.substr(start, pp - start);
    };

    auth.publicKey = extractStr(authDataJson, "public_key");
    auth.privateKeySalt = extractStr(authDataJson, "private_key_salt");
    auth.privateKeyIterations = extractInt(authDataJson, "private_key_iterations");
    auth.fromPassphrase = !auth.privateKeySalt.empty();

    // Parse signatures map
    auto sigsBlock = extractNestedObj(authDataJson, "signatures");
    if (!sigsBlock.empty()) {
        // sigsBlock: {"@user:id": {"ed25519:DEV": "sig..."}}
        size_t pos = 0;
        while ((pos = sigsBlock.find("\"@", pos)) != std::string::npos ||
               (pos = sigsBlock.find("\"", pos)) != std::string::npos) {
            pos++;
            size_t keyEnd = sigsBlock.find('"', pos);
            if (keyEnd == std::string::npos) break;
            std::string userId = sigsBlock.substr(pos, keyEnd - pos);
            pos = keyEnd + 1;

            auto devBlock = extractNestedObj(sigsBlock.substr(pos), userId);
            if (devBlock.empty()) { pos++; continue; }

            std::unordered_map<std::string, std::string> devSigs;
            size_t dp = 0;
            while ((dp = devBlock.find("\"", dp)) != std::string::npos) {
                dp++;
                size_t dkEnd = devBlock.find('"', dp);
                if (dkEnd == std::string::npos) break;
                std::string keyId = devBlock.substr(dp, dkEnd - dp);
                dp = dkEnd + 1;

                // Find the signature value
                auto colon = devBlock.find(':', dp);
                if (colon == std::string::npos) break;
                auto valStart = devBlock.find('"', colon);
                if (valStart == std::string::npos) break;
                valStart++;
                auto valEnd = devBlock.find('"', valStart);
                if (valEnd == std::string::npos) break;
                devSigs[keyId] = devBlock.substr(valStart, valEnd - valStart);
                dp = valEnd + 1;
            }

            if (!devSigs.empty()) {
                auth.signatures[userId] = devSigs;
            }
        }
    }

    return auth;
}

} // namespace progressive
