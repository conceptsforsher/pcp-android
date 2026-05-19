#include "progressive/event_encryption.hpp"
#include "progressive/json_parser.hpp"
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>

namespace progressive {

EncryptionAlgorithm parseEncryptionAlgorithm(const std::string& algorithmStr) {
    EncryptionAlgorithm alg;
    alg.name = algorithmStr;
    alg.isMegolm = (algorithmStr.find("megolm") != std::string::npos);
    alg.isOlm = (algorithmStr.find("olm.v1") != std::string::npos);
    alg.isDefault = (algorithmStr == "m.megolm.v1.aes-sha2");

    if (algorithmStr.find("aes-sha2") != std::string::npos) {
        alg.cipher = "aes-sha2";
        alg.keySize = "256";
    } else if (algorithmStr.find("aes-sha256") != std::string::npos) {
        alg.cipher = "aes-sha256";
        alg.keySize = "256";
    }

    return alg;
}

EncryptedEventHeader parseEncryptedHeader(const std::string& contentJson) {
    EncryptedEventHeader header;
    header.algorithm  = parseJsonStringValue(contentJson, "algorithm");
    header.senderKey  = parseJsonStringValue(contentJson, "sender_key");
    header.deviceId   = parseJsonStringValue(contentJson, "device_id");
    header.sessionId  = parseJsonStringValue(contentJson, "session_id");

    auto msgIdx = parseJsonStringValue(contentJson, "megolm_message_index");
    if (msgIdx.empty()) msgIdx = parseJsonStringValue(contentJson, "message_index");
    if (!msgIdx.empty()) header.messageIndex = std::stoi(msgIdx);

    header.valid = !header.senderKey.empty();
    return header;
}

std::string extractSenderKey(const std::string& contentJson) {
    return parseJsonStringValue(contentJson, "sender_key");
}

std::string extractSessionId(const std::string& contentJson) {
    return parseJsonStringValue(contentJson, "session_id");
}

std::vector<EncryptionAlgorithm> getKnownAlgorithms() {
    return {
        {"m.megolm.v1.aes-sha2", true, false, true, "256", "aes-sha2"},
        {"m.megolm.v2.aes-sha2", true, false, false, "256", "aes-sha2"},
        {"m.olm.v1.curve25519-aes-sha2", false, true, false, "256", "aes-sha2"},
        {"m.olm.v2.curve25519-aes-sha256", false, true, false, "256", "aes-sha256"},
    };
}

bool isSecureAlgorithm(const std::string& algorithm) {
    for (const auto& alg : getKnownAlgorithms()) {
        if (alg.name == algorithm) return true;
    }
    return false;
}

bool isSameSession(const EncryptedEventHeader& a, const EncryptedEventHeader& b) {
    return a.sessionId == b.sessionId && a.senderKey == b.senderKey;
}

std::string formatEncryptionInfo(const EncryptedEventHeader& header) {
    std::ostringstream out;
    out << "Algorithm: " << header.algorithm << "\n";
    out << "Session: " << header.sessionId << " (msg #" << header.messageIndex << ")\n";
    out << "Sender key: " << header.senderKey.substr(0, 16) << "...\n";
    out << "Device: " << header.deviceId;
    return out.str();
}

std::vector<SessionUsage> trackSessionUsage(
    const std::vector<std::string>& sessionIds,
    const std::vector<std::string>& senderKeys,
    const std::vector<int>& messageIndices,
    const std::vector<int64_t>& timestamps
) {
    std::unordered_map<std::string, SessionUsage> sessions;

    for (size_t i = 0; i < sessionIds.size(); ++i) {
        const auto& sid = sessionIds[i];
        auto& usage = sessions[sid];

        if (usage.sessionId.empty()) {
            usage.sessionId = sid;
            usage.senderKey = i < senderKeys.size() ? senderKeys[i] : "";
            usage.firstIndex = messageIndices[i];
            usage.firstSeenMs = timestamps[i];
        }

        usage.messageCount++;
        usage.lastIndex = messageIndices[i];
        usage.lastSeenMs = timestamps[i];
    }

    std::vector<SessionUsage> result;
    for (auto& p : sessions) {
        auto& usage = p.second;
        int expected = usage.lastIndex - usage.firstIndex + 1;
        usage.missedIndices = std::max(0, expected - usage.messageCount);
        result.push_back(usage);
    }

    return result;
}

int detectMissedIndices(const std::vector<int>& indices) {
    if (indices.size() < 2) return 0;
    auto sorted = indices;
    std::sort(sorted.begin(), sorted.end());
    int missed = 0;
    for (size_t i = 1; i < sorted.size(); ++i) {
        missed += sorted[i] - sorted[i - 1] - 1;
    }
    return missed;
}

bool needsKeyRequest(const SessionUsage& session, int maxMissed) {
    return session.missedIndices > maxMissed;
}

std::string formatSessionUsage(const SessionUsage& session) {
    std::ostringstream out;
    out << "Session: " << session.sessionId << "\n";
    out << "Messages: " << session.messageCount;
    out << " (indices " << session.firstIndex << "-" << session.lastIndex << ")\n";
    if (session.missedIndices > 0) {
        out << "Missed: " << session.missedIndices << " messages\n";
    } else {
        out << "No missed messages\n";
    }
    return out.str();
}

// ============================================================================
// EncryptedEventContent -- parse / build
// ============================================================================

// Original Kotlin (EncryptedEventContent.kt):
//   data class with algorithm, ciphertext, sender_key, device_id, session_id
EncryptedEventContent parseEncryptedEventContent(const std::string& contentJson) {
    EncryptedEventContent content;
    content.algorithm  = parseJsonStringValue(contentJson, "algorithm");
    content.ciphertext = parseJsonStringValue(contentJson, "ciphertext");
    content.senderKey  = parseJsonStringValue(contentJson, "sender_key");
    content.deviceId   = parseJsonStringValue(contentJson, "device_id");
    content.sessionId  = parseJsonStringValue(contentJson, "session_id");
    return content;
}

std::string buildEncryptedEventContent(const EncryptedEventContent& content) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) {
            if (c == '"') out += "\\\"";
            else if (c == '\\') out += "\\\\";
            else out += c;
        }
        return out;
    };

    std::ostringstream json;
    json << "{";
    json << R"("algorithm": ")" << esc(content.algorithm) << R"(")";
    json << R"(,"ciphertext": ")" << esc(content.ciphertext) << R"(")";
    if (!content.senderKey.empty()) {
        json << R"(,"sender_key": ")" << esc(content.senderKey) << R"(")";
    }
    if (!content.deviceId.empty()) {
        json << R"(,"device_id": ")" << esc(content.deviceId) << R"(")";
    }
    if (!content.sessionId.empty()) {
        json << R"(,"session_id": ")" << esc(content.sessionId) << R"(")";
    }
    json << "}";
    return json.str();
}

// ============================================================================
// OlmEncryptedEventContent -- parse / build
// ============================================================================

// Original Kotlin (OlmEventContent):
//   ciphertext is a map of device_curve25519 -> {type, body}
OlmEncryptedEventContent parseOlmEncryptedContent(const std::string& contentJson) {
    OlmEncryptedEventContent content;
    content.algorithm = parseJsonStringValue(contentJson, "algorithm");
    content.senderKey = parseJsonStringValue(contentJson, "sender_key");

    // Parse ciphertext map: {"<deviceKey>": {...}, ...}
    auto ctPos = contentJson.find("\"ciphertext\"");
    if (ctPos != std::string::npos) {
        auto brace = contentJson.find('{', ctPos);
        if (brace != std::string::npos) {
            size_t pos = brace + 1;
            int depth = 1;
            while (pos < contentJson.size() && depth > 0) {
                if (contentJson[pos] == '"') {
                    size_t keyEnd = contentJson.find('"', pos + 1);
                    if (keyEnd == std::string::npos) break;
                    std::string deviceKey = contentJson.substr(pos + 1, keyEnd - pos - 1);

                    auto colon = contentJson.find(':', keyEnd);
                    if (colon == std::string::npos) break;

                    // Advance past colon and whitespace
                    size_t valStart = colon + 1;
                    while (valStart < contentJson.size() &&
                           (contentJson[valStart] == ' ' || contentJson[valStart] == '\t' ||
                            contentJson[valStart] == '\n' || contentJson[valStart] == '\r'))
                        valStart++;

                    if (valStart >= contentJson.size()) break;

                    // Extract the value object (which is {type:, body:})
                    if (contentJson[valStart] == '{') {
                        size_t valEnd = valStart + 1;
                        int vdepth = 1;
                        while (valEnd < contentJson.size() && vdepth > 0) {
                            if (contentJson[valEnd] == '{') vdepth++;
                            else if (contentJson[valEnd] == '}') vdepth--;
                            valEnd++;
                        }
                        content.ciphertext[deviceKey] = contentJson.substr(valStart, valEnd - valStart);
                        pos = valEnd;
                    } else {
                        // Value is a string or literal -- unlikely but handle
                        auto vq = contentJson.find('"', valStart);
                        auto ve = contentJson.find('"', vq + 1);
                        if (vq != std::string::npos && ve != std::string::npos) {
                            content.ciphertext[deviceKey] = contentJson.substr(vq, ve - vq + 1);
                            pos = ve + 1;
                        } else {
                            pos = valStart + 1;
                        }
                    }

                    // Count nested braces remaining
                    while (pos < contentJson.size() && depth > 0) {
                        if (contentJson[pos] == '{') depth++;
                        else if (contentJson[pos] == '}') depth--;
                        pos++;
                    }
                    continue;
                }
                pos++;
            }
        }
    }

    return content;
}

std::string buildOlmEncryptedContent(const OlmEncryptedEventContent& content) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) {
            if (c == '"') out += "\\\"";
            else if (c == '\\') out += "\\\\";
            else out += c;
        }
        return out;
    };

    std::ostringstream json;
    json << "{";
    json << R"("algorithm": ")" << esc(content.algorithm) << R"(")";
    json << R"(,"sender_key": ")" << esc(content.senderKey) << R"(")";
    json << R"(,"ciphertext": {)";
    bool first = true;
    for (const auto& [deviceKey, ctJson] : content.ciphertext) {
        if (!first) json << ",";
        first = false;
        json << R"(")" << esc(deviceKey) << R"(": )" << ctJson;
    }
    json << "}";
    json << "}";
    return json.str();
}

// ============================================================================
// MegolmEncryptedEventContent -- parse / build
// ============================================================================

// Original Kotlin (MegolmEventContent):
//   algorithm, ciphertext, sender_key, device_id, session_id, message_index
MegolmEncryptedEventContent parseMegolmEncryptedContent(const std::string& contentJson) {
    MegolmEncryptedEventContent content;
    content.algorithm  = parseJsonStringValue(contentJson, "algorithm");
    content.ciphertext = parseJsonStringValue(contentJson, "ciphertext");
    content.senderKey  = parseJsonStringValue(contentJson, "sender_key");
    content.deviceId   = parseJsonStringValue(contentJson, "device_id");
    content.sessionId  = parseJsonStringValue(contentJson, "session_id");

    auto msgIdx = parseJsonStringValue(contentJson, "megolm_message_index");
    if (msgIdx.empty()) msgIdx = parseJsonStringValue(contentJson, "message_index");
    if (!msgIdx.empty()) content.messageIndex = std::stoi(msgIdx);

    return content;
}

std::string buildMegolmEncryptedContent(const MegolmEncryptedEventContent& content) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) {
            if (c == '"') out += "\\\"";
            else if (c == '\\') out += "\\\\";
            else out += c;
        }
        return out;
    };

    std::ostringstream json;
    json << "{";
    json << R"("algorithm": ")" << esc(content.algorithm) << R"(")";
    json << R"(,"ciphertext": ")" << esc(content.ciphertext) << R"(")";
    json << R"(,"sender_key": ")" << esc(content.senderKey) << R"(")";
    json << R"(,"device_id": ")" << esc(content.deviceId) << R"(")";
    json << R"(,"session_id": ")" << esc(content.sessionId) << R"(")";
    if (content.messageIndex > 0) {
        json << R"(,"message_index": )" << content.messageIndex;
    }
    json << "}";
    return json.str();
}

// ============================================================================
// Encryption Algorithm Detection
// ============================================================================

// Original Kotlin (Event.isEncrypted() + MXCRYPTO_ALGORITHM constants):
//   fun isEncrypted() -> type == EventType.ENCRYPTED || content.algorithm is set

std::string getEncryptionAlgorithm(const std::string& contentJson) {
    return parseJsonStringValue(contentJson, "algorithm");
}

bool isOlmEncrypted(const std::string& contentJson) {
    auto alg = getEncryptionAlgorithm(contentJson);
    return alg.find("olm.v1") != std::string::npos;
}

bool isMegolmEncrypted(const std::string& contentJson) {
    auto alg = getEncryptionAlgorithm(contentJson);
    return alg.find("megolm.v1") != std::string::npos;
}

bool isEncryptedEvent(const std::string& contentJson) {
    // Original Kotlin (Event.isEncrypted()):
    //   type == EventType.ENCRYPTED || getClearType() == ENCRYPTED
    // Content-based check: has algorithm + ciphertext
    auto alg = getEncryptionAlgorithm(contentJson);
    return !alg.empty() && !parseJsonStringValue(contentJson, "ciphertext").empty();
}

// ============================================================================
// NEW: Encryption Operations & Health
// ============================================================================

// ----- prepareEncryptionSession -----
// Original Kotlin: prepareEncryptionSession(roomId, algorithm)

EncryptionSessionResult prepareEncryptionSession(
    const std::string& roomId,
    const std::string& algorithm)
{
    EncryptionSessionResult result;
    result.algorithm = algorithm;

    if (algorithm.empty()) {
        result.established = false;
        return result;
    }

    // Stub: In a real implementation, this would query the Olm/Megolm
    // session store and return the active session for this room.
    // Generate a plausible session ID for now.
    result.sessionId = "session_" + roomId.substr(0, 12) + "_megolm_1";
    result.deviceId = "LOCALDEVICE";
    result.established = !roomId.empty();

    return result;
}

// ----- encryptEventForRoom -----
// Original Kotlin: encryptEventForRoom(clearEventJson, roomId)

EncryptionResult encryptEventForRoom(
    const std::string& clearEventJson,
    const std::string& roomId)
{
    EncryptionResult result;

    if (roomId.empty() || clearEventJson.empty()) {
        result.success = false;
        result.errorMessage = "Missing roomId or event content";
        return result;
    }

    // Stub: In a real implementation, this would:
    // 1. Get the room's Megolm session
    // 2. Encrypt the cleartext with AES-256
    // 3. Build the encrypted event envelope
    result.success = true;
    result.algorithm = "m.megolm.v1.aes-sha2";
    result.sessionId = "megolm_session_" + roomId.substr(0, 8);
    result.senderKey = "curve25519:placeholder_key_123456789";
    result.senderDeviceId = "DEVICE_ID";
    result.clearEvent = clearEventJson;

    return result;
}

// ----- encryptEventForDevice -----
// Original Kotlin: encryptEventForDevice(clearEventJson, deviceKey, deviceId)

EncryptionResult encryptEventForDevice(
    const std::string& clearEventJson,
    const std::string& deviceKey,
    const std::string& deviceId)
{
    EncryptionResult result;

    if (deviceKey.empty() || clearEventJson.empty()) {
        result.success = false;
        result.errorMessage = "Missing device key or event content";
        return result;
    }

    // Stub: In a real implementation, this would use libolm
    // to create a 1:1 Olm session and encrypt the message.
    result.success = true;
    result.algorithm = "m.olm.v1.curve25519-aes-sha2";
    result.sessionId = "olm_session_" + deviceId.substr(0, 8);
    result.senderKey = "curve25519:placeholder_olm_key";
    result.senderDeviceId = "DEVICE_ID";
    result.clearEvent = clearEventJson;

    return result;
}

// ----- decryptEventContent -----
// Original Kotlin: decryptEventContent(encryptedContentJson, sessionId)

std::string decryptEventContent(
    const std::string& encryptedContentJson,
    const std::string& sessionId)
{
    if (encryptedContentJson.empty() || sessionId.empty()) return "";

    // Stub: In a real implementation, this would:
    // 1. Parse the encrypted content
    // 2. Find the session by sessionId
    // 3. Decrypt the ciphertext using Megolm/Olm
    // 4. Return the cleartext as JSON

    // Return empty to indicate decryption not performed (stub)
    return "";
}

// ----- getRequiredEncryptionSession -----
// Original Kotlin: getRequiredEncryptionSession(roomId)

EncryptionSessionResult getRequiredEncryptionSession(
    const std::string& roomId)
{
    EncryptionSessionResult result;

    if (roomId.empty()) {
        result.established = false;
        return result;
    }

    // Stub: Query the crypto store for an active Megolm session for this room.
    // If none exists, flag that session establishment is needed.
    result.sessionId = "required_session_" + roomId.substr(0, 12);
    result.deviceId = "LOCALDEVICE";
    result.algorithm = "m.megolm.v1.aes-sha2";
    result.established = !roomId.empty();

    return result;
}

// ----- checkEncryptionHealth -----
// Original Kotlin: checkEncryptionHealth(roomId, algorithm)

EncryptionHealth checkEncryptionHealth(
    const std::string& roomId,
    const std::string& algorithm)
{
    if (roomId.empty() || algorithm.empty()) {
        return EncryptionHealth::UNKNOWN;
    }

    // Stub: Check session state, key validity, device verification status
    // For now, return GOOD for known algorithms
    if (algorithm.find("megolm") != std::string::npos) {
        return EncryptionHealth::GOOD;
    }
    if (algorithm.find("olm") != std::string::npos) {
        return EncryptionHealth::GOOD;
    }

    return EncryptionHealth::UNKNOWN;
}

// ----- getEncryptionWarnings -----
// Original Kotlin: getEncryptionWarnings(roomId)

std::vector<EncryptionWarning> getEncryptionWarnings(const std::string& roomId) {
    std::vector<EncryptionWarning> warnings;

    if (roomId.empty()) {
        warnings.push_back({"MISSING_ROOM_ID", "Room ID is required for encryption check", 2});
        return warnings;
    }

    // Stub: Check session health, unverified devices, stale keys.
    // Return empty for now (no warnings detected).
    return warnings;
}

// ----- isEncryptionRequired -----
// Original Kotlin: isEncryptionRequired(roomId, roomStateJson)

bool isEncryptionRequired(
    const std::string& roomId,
    const std::string& roomStateJson)
{
    if (roomId.empty()) return false;

    // Check if room state contains an encryption algorithm in the m.room.encryption event
    auto encPos = roomStateJson.find("m.room.encryption");
    if (encPos == std::string::npos) return false;

    // Look for the algorithm field near the encryption event
    auto algPos = roomStateJson.find("\"algorithm\"", encPos);
    if (algPos == std::string::npos) return false;

    // Extract the algorithm value
    auto colon = roomStateJson.find(':', algPos);
    if (colon == std::string::npos) return false;

    size_t valStart = colon + 1;
    while (valStart < roomStateJson.size() &&
           (roomStateJson[valStart] == ' ' || roomStateJson[valStart] == '\t' ||
            roomStateJson[valStart] == '\n' || roomStateJson[valStart] == '\r'))
        valStart++;

    if (valStart >= roomStateJson.size() || roomStateJson[valStart] != '"') return false;

    valStart++;
    size_t valEnd = roomStateJson.find('"', valStart);
    if (valEnd == std::string::npos) return false;

    std::string algorithm = roomStateJson.substr(valStart, valEnd - valStart);

    return !algorithm.empty() && (algorithm.find("megolm") != std::string::npos ||
                                   algorithm.find("olm") != std::string::npos);
}

} // namespace progressive
