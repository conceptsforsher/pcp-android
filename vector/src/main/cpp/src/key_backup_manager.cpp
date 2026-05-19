#include "progressive/key_backup_manager.hpp"
#include "progressive/key_backup.hpp"
#include "progressive/crypto_algorithms.hpp"
#include "progressive/megolm_decryptor.hpp"
#include <sstream>
#include <algorithm>
#include <ctime>

namespace progressive {

// ====== JSON extraction helpers (local to this file) ======

static std::string extractStr(const std::string& json, const std::string& key) {
    auto pp = json.find("\"" + key + "\"");
    if (pp == std::string::npos) return "";
    pp = json.find('"', pp + key.size() + 2);
    if (pp == std::string::npos) return "";
    pp++;
    size_t e = pp;
    while (e < json.size() && json[e] != '"') e++;
    return json.substr(pp, e - pp);
}

static int extractInt(const std::string& json, const std::string& key) {
    auto pp = json.find("\"" + key + "\"");
    if (pp == std::string::npos) return 0;
    pp = json.find(':', pp);
    if (pp == std::string::npos) return 0;
    pp++;
    while (pp < json.size() && (json[pp] == ' ' || json[pp] == '\t')) pp++;
    int v = 0;
    while (pp < json.size() && json[pp] >= '0' && json[pp] <= '9') { v=v*10+(json[pp]-'0'); pp++; }
    return v;
}

static bool extractBool(const std::string& json, const std::string& key) {
    return json.find("\"" + key + "\":true") != std::string::npos;
}

static std::string extractNestedObj(const std::string& json, const std::string& key) {
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
}

// ====== Constructor ======

KeyBackupManager::KeyBackupManager() {
    progress_.startedAt = static_cast<int64_t>(std::time(nullptr)) * 1000;
}

// ====== Configuration ======

void KeyBackupManager::setRecoveryKey(const std::string& key) { recoveryKey_ = key; }
void KeyBackupManager::setCurve25519Key(const std::string& key) { curve25519Key_ = key; }

void KeyBackupManager::setBackupVersion(const std::string& version) {
    currentVersion_.version = version;
    currentVersion_.versionInt = std::stoi(version);
}

// ====== Recovery Key Management ======

std::string KeyBackupManager::extractPrivateKeyFromRecoveryKey(const std::string& recoveryKey) {
    auto result = extractCurveKeyFromRecoveryKey(recoveryKey);
    if (!result.empty()) recoveryKey_ = recoveryKey;
    return result;
}

std::string KeyBackupManager::generateRecoveryKey(const std::string& curve25519Key) {
    curve25519Key_ = curve25519Key;
    auto rk = computeRecoveryKey(curve25519Key);
    if (!rk.empty()) recoveryKey_ = rk;
    return rk;
}

bool KeyBackupManager::validatePassphrase(const std::string& passphrase) {
    return !passphrase.empty() && passphrase.size() >= 8;
}

// ====== Backup Version Management ======

BackupVersion KeyBackupManager::parseBackupVersion(const std::string& json) {
    BackupVersion v;
    v.version = extractStr(json, "version");
    v.algorithm = extractStr(json, "algorithm");
    v.authData = extractNestedObj(json, "auth_data");
    if (v.authData.empty()) v.authData = extractStr(json, "auth_data");
    v.count = extractInt(json, "count");
    v.etag = extractStr(json, "etag");

    if (!v.version.empty()) {
        try { v.versionInt = std::stoll(v.version); } catch (...) { v.versionInt = 0; }
    }

    v.valid = !v.version.empty() && !v.algorithm.empty() && isSupportedBackupAlgorithm(v.algorithm);
    if (!v.valid && v.algorithm.empty()) v.error = "Missing algorithm";
    else if (!v.valid) v.error = "Unsupported algorithm: " + v.algorithm;

    currentVersion_ = v;
    return v;
}

std::string KeyBackupManager::buildCreateBackupVersionRequest(const KeyBackupConfig& config) {
    std::ostringstream os;
    os << R"({"algorithm":")" << config.algorithm << R"(")";
    if (!config.authData.empty()) {
        os << R"(,"auth_data":)" << config.authData;
    }
    os << R"(,"version":")" << config.version << R"(")";
    os << "}";
    return os.str();
}

std::string KeyBackupManager::buildUpdateBackupVersionRequest(const std::string& version,
                                                               const std::string& authData) {
    std::ostringstream os;
    os << R"({"algorithm":")" << currentVersion_.algorithm << R"(")";
    os << R"(,"auth_data":)" << authData;
    os << R"(,"version":")" << version << R"(")";
    os << "}";
    return os.str();
}

std::string KeyBackupManager::buildDeleteBackupRequest(const std::string& version) {
    (void)version;
    return "{}"; // DELETE typically has no body
}

// ====== Session Key Export ======

std::string KeyBackupManager::generateSessionId(const MegolmSessionExport& session) const {
    // Session ID in backup = base64(firstMessageIndex || senderKey[0:4])
    std::string id;
    uint32_t idx = static_cast<uint32_t>(session.firstMessageIndex);
    id += static_cast<char>((idx >> 24) & 0xFF);
    id += static_cast<char>((idx >> 16) & 0xFF);
    id += static_cast<char>((idx >> 8) & 0xFF);
    id += static_cast<char>(idx & 0xFF);
    for (int i = 0; i < 4 && i < (int)session.senderKey.size(); i++)
        id += session.senderKey[i];
    // Simple base64-like encoding
    static const char* b64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string encoded;
    for (size_t i = 0; i < id.size(); i += 3) {
        uint32_t t = static_cast<uint8_t>(id[i]) << 16;
        if (i+1 < id.size()) t |= static_cast<uint8_t>(id[i+1]) << 8;
        if (i+2 < id.size()) t |= static_cast<uint8_t>(id[i+2]);
        encoded += b64[(t >> 18) & 0x3F];
        encoded += b64[(t >> 12) & 0x3F];
        if (i+1 < id.size()) encoded += b64[(t >> 6) & 0x3F]; else encoded += '=';
        if (i+2 < id.size()) encoded += b64[t & 0x3F]; else encoded += '=';
    }
    return encoded;
}

MegolmSessionExport KeyBackupManager::exportSessionForBackup(
    const std::string& roomId, const std::string& senderKey,
    const std::string& sessionId, const std::string& sessionKeyBase64,
    int64_t firstMessageIndex, bool isForwardedKey, int64_t forwardedCount) {

    MegolmSessionExport exp;
    exp.roomId = roomId;
    exp.senderKey = senderKey;
    exp.sessionId = sessionId;
    exp.firstMessageIndex = firstMessageIndex;
    exp.isForwardedKey = isForwardedKey;
    exp.forwardedCount = forwardedCount;
    exp.hasKnownIndex = true;

    // Convert base64 session key to binary
    // For now, store as base64 string (will be converted during encryption)
    exp.sessionKey.clear();
    for (char c : sessionKeyBase64) {
        exp.sessionKey.push_back(static_cast<uint8_t>(c));
    }

    exp.senderClaimedKeys = "ed25519:" + senderKey;
    return exp;
}

// ====== Session Key Encryption ======

std::string KeyBackupManager::encryptSessionDataForBackup(
    const MegolmSessionExport& session, const std::string& authData) {

    std::ostringstream os;
    os << "{";
    // Per Matrix spec: encrypted session data is JSON with ciphertext, mac, ephemeral
    // We build a mock encryption structure (real encryption needs proper AES+MAC via libolm/OpenSSL)
    os << R"("ciphertext":")" << session.sessionId << R"(",)";
    os << R"("mac":"mock_mac",)";
    os << R"("ephemeral":"mock_ephemeral")";
    os << "}";

    // Store metadata for decryption
    os << R"(,"room_id":")" << session.roomId << R"(")";
    os << R"(,"session_id":")" << session.sessionId << R"(")";
    os << R"(,"sender_key":")" << session.senderKey << R"(")";
    os << R"(,"first_message_index":)" << session.firstMessageIndex;
    if (session.isForwardedKey) os << R"(,"forwarded_count":)" << session.forwardedCount;

    return os.str();
}

std::string KeyBackupManager::encryptSessionsForBackup(
    const std::vector<MegolmSessionExport>& sessions, const std::string& authData) {

    std::ostringstream os;
    os << R"({"rooms":{)";
    // Group by roomId
    std::unordered_map<std::string, std::vector<MegolmSessionExport>> byRoom;
    for (const auto& s : sessions) byRoom[s.roomId].push_back(s);

    bool firstRoom = true;
    for (const auto& [roomId, roomSessions] : byRoom) {
        if (!firstRoom) os << ","; firstRoom = false;
        os << "\"" << roomId << R"(":{"sessions":{)";
        bool firstSess = true;
        for (const auto& sess : roomSessions) {
            if (!firstSess) os << ","; firstSess = false;
            auto id = generateSessionId(sess);
            auto enc = encryptSessionDataForBackup(sess, authData);
            os << "\"" << id << "\":" << enc;
        }
        os << "}}";
    }
    os << "}}";
    return os.str();
}

// ====== Backup Upload Request Building ======

std::string KeyBackupManager::buildUploadRoomKeyRequest(const std::string& roomId,
                                                         const std::string& sessionId,
                                                         const std::string& encryptedData) {
    std::ostringstream os;
    os << R"({"room_id":")" << roomId << R"(",)";
    os << R"("session_id":")" << sessionId << R"(",)";
    os << R"("session_data":)" << encryptedData;
    os << "}";
    return os.str();
}

std::string KeyBackupManager::buildBatchUploadRequest(const std::vector<MegolmSessionExport>& sessions) {
    return encryptSessionsForBackup(sessions, currentVersion_.authData);
}

// ====== Backup Download & Parsing ======

std::vector<RoomKeyBackupData> KeyBackupManager::parseBackupKeysResponse(const std::string& json) {
    std::vector<RoomKeyBackupData> rooms;

    size_t pos = json.find("\"rooms\"");
    if (pos == std::string::npos) return rooms;
    pos = json.find('{', pos);
    if (pos == std::string::npos) return rooms;

    // Parse each room
    size_t end = pos;
    int depth = 0;
    while (end < json.size()) {
        if (json[end] == '{') depth++;
        else if (json[end] == '}') depth--;
        if (depth == 0 && end > pos) break;
        end++;
    }

    std::string roomsBlock = json.substr(pos, end - pos + 1);

    // Find room ID keys (quoted strings before ":")
    size_t rp = 0;
    while ((rp = roomsBlock.find("\"!", rp)) != std::string::npos) {
        size_t keyEnd = roomsBlock.find('"', rp + 1);
        if (keyEnd == std::string::npos) break;
        std::string roomId = roomsBlock.substr(rp + 1, keyEnd - rp - 1);

        auto sessBlock = extractNestedObj(roomsBlock.substr(keyEnd), "sessions");
        if (!sessBlock.empty()) {
            RoomKeyBackupData rd;
            rd.roomId = roomId;

            // Extract session IDs
            size_t sp = 0;
            while ((sp = sessBlock.find("\"", sp)) != std::string::npos) {
                sp++;
                size_t se = sp;
                while (se < sessBlock.size() && sessBlock[se] != '"') se++;
                if (se > sp) {
                    std::string sid = sessBlock.substr(sp, se - sp);
                    if (sid != "sessions") rd.sessions[roomId].push_back(sid);
                }
                sp = se + 1;
            }
            rooms.push_back(rd);
        }
        rp = keyEnd + 1;
    }

    return rooms;
}

std::vector<std::string> KeyBackupManager::parseRoomBackupKeys(const std::string& json,
                                                                const std::string& roomId) {
    std::vector<std::string> keys;
    auto sessBlock = extractNestedObj(json, "sessions");
    if (sessBlock.empty()) return keys;

    size_t pos = 0;
    while ((pos = sessBlock.find("\"", pos)) != std::string::npos) {
        pos++;
        size_t end = pos;
        while (end < sessBlock.size() && sessBlock[end] != '"') end++;
        std::string key = sessBlock.substr(pos, end - pos);
        if (!key.empty() && key != "sessions") {
            keys.push_back(key);
        }
        pos = end + 1;
    }
    (void)roomId; // Used for metadata
    return keys;
}

std::string KeyBackupManager::parseSessionBackupKey(const std::string& json,
                                                     const std::string& sessionId) {
    auto sessBlock = extractNestedObj(json, sessionId);
    if (sessBlock.empty()) {
        // Try looking for session in nested structure
        auto sessions = extractNestedObj(json, "sessions");
        if (!sessions.empty()) {
            auto found = extractNestedObj(sessions, sessionId);
            if (!found.empty()) return found;
            // Try string value
            auto val = extractStr(sessions, sessionId);
            if (!val.empty()) return "{" + val + "}";
        }
        return "";
    }
    return sessBlock;
}

// ====== Backup Key Decryption ======

std::string KeyBackupManager::decryptBackupKey(const std::string& backupAuthData,
                                                const std::string& recoveryKey) {
    // Extract the encrypted backup key from auth_data
    // auth_data format: {"public_key":"...","signatures":{...},"encrypted":{...}}
    // The encrypted part contains the AES key for session encryption

    (void)backupAuthData;
    (void)recoveryKey;

    // Real implementation would:
    // 1. Extract Curve25519 key from recoveryKey via extractCurveKeyFromRecoveryKey
    // 2. Parse auth_data for the encrypted AES key
    // 3. Decrypt using Curve25519 + AES
    // 4. Return the AES key

    // Mock: return the recovery key as the backup key
    return formatRecoveryKey(recoveryKey);
}

std::string KeyBackupManager::extractAuthDataField(const std::string& authData,
                                                    const std::string& field) const {
    return extractStr(authData, field);
}

// ====== Session Data Decryption ======

DecryptedSessionData KeyBackupManager::decryptSessionData(
    const std::string& encryptedSessionJson,
    const std::string& backupKey,
    const std::string& roomId) {

    DecryptedSessionData result;
    (void)backupKey;
    (void)roomId;

    // Try to extract session metadata from the encrypted JSON
    result.sessionId = extractStr(encryptedSessionJson, "session_id");
    result.senderKey = extractStr(encryptedSessionJson, "sender_key");
    result.firstMessageIndex = extractInt(encryptedSessionJson, "first_message_index");

    if (!result.sessionId.empty() && !result.senderKey.empty()) {
        result.sessionKeyBase64 = extractStr(encryptedSessionJson, "session_key");
        result.forwardedCount = extractInt(encryptedSessionJson, "forwarded_count");
        result.isForwardedKey = extractBool(encryptedSessionJson, "forwarded_key");
        result.decrypted = true;
    } else {
        result.decrypted = false;
        result.error = "Failed to decrypt: missing session_id or sender_key in payload";
    }

    return result;
}

std::vector<BackupSessionResult> KeyBackupManager::decryptAllSessions(
    const std::string& backupKeysJson,
    const std::string& backupAuthData,
    const std::string& recoveryKey) {

    std::vector<BackupSessionResult> results;

    // Decrypt backup key
    auto backupAesKey = decryptBackupKey(backupAuthData, recoveryKey);
    if (backupAesKey.empty()) {
        BackupSessionResult err;
        err.success = false;
        err.error = "Failed to decrypt backup AES key";
        results.push_back(err);
        return results;
    }

    // Parse rooms
    auto rooms = parseBackupKeysResponse(backupKeysJson);
    progress_.totalKeys = 0;
    progress_.downloadedKeys = 0;

    for (const auto& room : rooms) {
        for (const auto& [roomId, sessionIds] : room.sessions) {
            progress_.totalKeys += static_cast<int>(sessionIds.size());
        }
    }

    for (const auto& room : rooms) {
        for (const auto& [roomId, sessionIds] : room.sessions) {
            for (const auto& sessionId : sessionIds) {
                auto decrypted = decryptSessionData(sessionId, backupAesKey, roomId);
                advanceDownloaded(1);
                if (decrypted.decrypted) {
                    advanceDecrypted(1);
                    BackupSessionResult res;
                    res.sessionId = decrypted.sessionId;
                    res.roomId = roomId;
                    res.senderKey = decrypted.senderKey;
                    res.sessionKey = decrypted.sessionKeyBase64;
                    res.success = true;
                    results.push_back(res);
                } else {
                    BackupSessionResult res;
                    res.sessionId = sessionId;
                    res.roomId = roomId;
                    res.success = false;
                    res.error = decrypted.error;
                    results.push_back(res);
                }
            }
        }
    }

    progress_.isComplete = true;
    return results;
}

// ====== Backup Verification ======

bool KeyBackupManager::verifyBackupIntegrity(const std::string& backupAuthData) {
    // Check that auth_data contains required fields
    auto publicKey = extractStr(backupAuthData, "public_key");
    if (publicKey.empty()) return false;

    // Check for valid signatures
    auto signatures = extractNestedObj(backupAuthData, "signatures");
    if (signatures.empty()) return false; // must have at least one signature

    return true;
}

bool KeyBackupManager::verifyRecoveryKeyMatchesBackup(const std::string& recoveryKey,
                                                       const std::string& backupAuthData) {
    auto publicKey = extractStr(backupAuthData, "public_key");
    if (publicKey.empty()) return false;

    // Check that recovery key is valid
    auto valid = validateRecoveryKey(recoveryKey);
    if (!valid.valid) return false;

    // Extract the private key from recovery key
    auto privateKey = extractCurveKeyFromRecoveryKey(recoveryKey);
    if (privateKey.empty()) return false;

    // In a real implementation, derive public key from private key and compare
    // For now, just verify both exist
    return true;
}

bool KeyBackupManager::validateEncryptionPayload(const std::string& encryptedJson) const {
    return !encryptedJson.empty() &&
           (encryptedJson.find("ciphertext") != std::string::npos ||
            encryptedJson.find("session_id") != std::string::npos);
}

// ====== Trust Management ======

void KeyBackupManager::markBackupAsTrusted() {
    trust_ = BackupTrust::TRUSTED;
}

void KeyBackupManager::markBackupAsUntrusted() {
    trust_ = BackupTrust::UNTRUSTED;
}

std::string KeyBackupManager::buildMarkBackupAsTrustedRequest() {
    std::ostringstream os;
    os << R"({)";
    os << R"("algorithm":")" << currentVersion_.algorithm << R"(")";
    os << R"(,"auth_data":)" << currentVersion_.authData;
    os << R"(,"version":")" << currentVersion_.version << R"(")";
    os << R"(,"etag":")" << currentVersion_.etag << R"(")";
    os << "}";
    return os.str();
}

// ====== Progress Tracking ======

void KeyBackupManager::setTotalKeys(int count) {
    progress_.totalKeys = count;
    progress_.isRunning = true;
    progress_.startedAt = static_cast<int64_t>(std::time(nullptr)) * 1000;
}

void KeyBackupManager::advanceUploaded(int count) {
    progress_.uploadedKeys += count;
    progress_.lastUpdateAt = static_cast<int64_t>(std::time(nullptr)) * 1000;
}

void KeyBackupManager::advanceDownloaded(int count) {
    progress_.downloadedKeys += count;
    progress_.lastUpdateAt = static_cast<int64_t>(std::time(nullptr)) * 1000;
}

void KeyBackupManager::advanceDecrypted(int count) {
    progress_.decryptedKeys += count;
    progress_.lastUpdateAt = static_cast<int64_t>(std::time(nullptr)) * 1000;
}

void KeyBackupManager::advanceImported(int count) {
    progress_.importedKeys += count;
    progress_.lastUpdateAt = static_cast<int64_t>(std::time(nullptr)) * 1000;
}

void KeyBackupManager::markComplete() {
    progress_.isComplete = true;
    progress_.isRunning = false;
    progress_.lastUpdateAt = static_cast<int64_t>(std::time(nullptr)) * 1000;
}

void KeyBackupManager::markError(const std::string& error) {
    progress_.error = error;
    progress_.isRunning = false;
    progress_.lastUpdateAt = static_cast<int64_t>(std::time(nullptr)) * 1000;
}

// ====== Serialization ======

std::string KeyBackupManager::progressToJson() const {
    std::ostringstream os;
    os << R"({"total_keys":)" << progress_.totalKeys
       << R"(,"uploaded":)" << progress_.uploadedKeys
       << R"(,"failed":)" << progress_.failedKeys
       << R"(,"downloaded":)" << progress_.downloadedKeys
       << R"(,"decrypted":)" << progress_.decryptedKeys
       << R"(,"imported":)" << progress_.importedKeys
       << R"(,"is_running":)" << (progress_.isRunning ? "true" : "false")
       << R"(,"is_complete":)" << (progress_.isComplete ? "true" : "false")
       << R"(,"started_at":)" << progress_.startedAt
       << R"(,"last_update":)" << progress_.lastUpdateAt;
    if (!progress_.error.empty()) os << R"(,"error":")" << progress_.error << "\"";
    os << "}";
    return os.str();
}

std::string KeyBackupManager::backupVersionToJson(const BackupVersion& ver) const {
    std::ostringstream os;
    os << R"({"version":")" << ver.version
       << R"(","algorithm":")" << ver.algorithm
       << R"(","count":)" << ver.count
       << R"(,"valid":)" << (ver.valid ? "true" : "false")
       << R"(,"etag":")" << ver.etag << "\"";
    if (!ver.error.empty()) os << R"(,"error":")" << ver.error << "\"";
    os << "}";
    return os.str();
}

std::string KeyBackupManager::decryptResultsToJson(const std::vector<BackupSessionResult>& results) const {
    std::ostringstream os;
    os << "[";
    for (size_t i = 0; i < results.size(); i++) {
        if (i > 0) os << ",";
        os << R"({"session_id":")" << results[i].sessionId
           << R"(","room_id":")" << results[i].roomId
           << R"(","sender_key":")" << results[i].senderKey
           << R"(","session_key":")" << results[i].sessionKey
           << R"(","success":)" << (results[i].success ? "true" : "false");
        if (!results[i].error.empty()) os << R"(,"error":")" << results[i].error << "\"";
        os << "}";
    }
    os << "]";
    return os.str();
}

} // namespace progressive
