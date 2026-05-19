#include "progressive/room_encryption.hpp"
#include "progressive/json_parser.hpp"
#include <sstream>
#include <chrono>
#include <cmath>

namespace progressive {

EncryptionConfig parseEncryptionConfig(const std::string& stateContentJson) {
    EncryptionConfig config;
    config.algorithm = parseJsonStringValue(stateContentJson, "algorithm");

    auto rotMs = parseJsonStringValue(stateContentJson, "rotation_period_ms");
    if (!rotMs.empty()) config.rotationPeriodMs = std::stoi(rotMs);

    auto rotMsg = parseJsonStringValue(stateContentJson, "rotation_period_msgs");
    if (!rotMsg.empty()) config.rotationPeriodMessages = std::stoi(rotMsg);

    config.enabled = !config.algorithm.empty();
    config.isDefaultAlgorithm = (config.algorithm == "m.megolm.v1.aes-sha2" ||
                                  config.algorithm.empty());

    // Compute next rotation time if rotation period is set
    if (config.rotationPeriodMs > 0) {
        config.nextRotationMs = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count() + config.rotationPeriodMs;
    }

    return config;
}

bool isRoomEncrypted(const std::string& stateContentJson) {
    auto algo = parseJsonStringValue(stateContentJson, "algorithm");
    return !algo.empty();
}

std::string getDefaultEncryptionAlgorithm() {
    return "m.megolm.v1.aes-sha2";
}

bool requiresDeviceVerification(const std::string& algorithm) {
    return algorithm == "m.megolm.v1.aes-sha2";
}

EncryptionStatus computeEncryptionStatus(
    const std::string& algorithm,
    const std::vector<bool>& deviceVerified,
    const std::vector<bool>& deviceBlacklisted
) {
    EncryptionStatus status;
    status.algorithm = algorithm;
    status.isEncrypted = !algorithm.empty();
    status.totalDevices = static_cast<int>(deviceVerified.size());

    for (size_t i = 0; i < deviceVerified.size(); ++i) {
        if (i < deviceBlacklisted.size() && deviceBlacklisted[i]) {
            status.blacklistedCount++;
            status.hasBlacklistedDevices = true;
        } else if (deviceVerified[i]) {
            status.verifiedCount++;
        } else {
            status.unverifiedCount++;
            status.hasUnverifiedDevices = true;
        }
    }

    status.isVerified = !status.hasUnverifiedDevices && !status.hasBlacklistedDevices && status.verifiedCount > 0;

    if (status.hasBlacklistedDevices) status.trustLevel = "Warning";
    else if (status.isVerified) status.trustLevel = "Verified";
    else if (status.hasUnverifiedDevices) status.trustLevel = "Warning";
    else status.trustLevel = "Unknown";

    return status;
}

std::string formatEncryptionStatus(const EncryptionStatus& status) {
    std::ostringstream out;
    out << "Encryption: " << (status.isEncrypted ? "Enabled" : "Disabled") << "\n";
    if (status.isEncrypted) {
        out << "Algorithm: " << status.algorithm << "\n";
        out << "Trust: " << status.trustLevel << "\n";
        out << "Devices: " << status.verifiedCount << " verified, "
            << status.unverifiedCount << " unverified";
        if (status.blacklistedCount > 0)
            out << ", " << status.blacklistedCount << " blacklisted";
        out << "\n";
    }
    return out.str();
}

std::string formatEncryptionBadge(const EncryptionStatus& status) {
    if (!status.isEncrypted) return "\xF0\x9F\x94\x93"; // 🔓 open lock
    if (status.isVerified)   return "\xF0\x9F\x94\x92"; // 🔒 locked
    if (status.hasUnverifiedDevices) return "\xE2\x9A\xA0"; // ⚠ warning
    return "\xF0\x9F\x94\x90"; // 🔐 locked with key
}

std::string buildEncryptionContent(const EncryptionConfig& config) {
    std::ostringstream json;
    json << "{";
    json << R"("algorithm": ")" << config.algorithm << R"(")";
    if (config.rotationPeriodMs > 0)
        json << R"(,"rotation_period_ms": )" << config.rotationPeriodMs;
    if (config.rotationPeriodMessages > 0)
        json << R"(,"rotation_period_msgs": )" << config.rotationPeriodMessages;
    json << "}";
    return json.str();
}

bool isRotationDue(const EncryptionConfig& config, int messageCount, int64_t sessionStartMs) {
    if (config.rotationPeriodMessages > 0 && messageCount >= config.rotationPeriodMessages)
        return true;

    if (config.rotationPeriodMs > 0 && sessionStartMs > 0) {
        auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
        if (now - sessionStartMs >= config.rotationPeriodMs) return true;
    }

    return false;
}

// ==== RoomEncryptionEventContent Parsing ====
//
// Original Kotlin (RoomEncryption.kt parse):
//   {"algorithm":"m.megolm.v1.aes-sha2",
//    "rotation_period_ms":604800000,
//    "rotation_period_msgs":100,
//    "session_key_bits":256}

RoomEncryptionEventContent parseEncryptionEvent(const std::string& stateContentJson) {
    RoomEncryptionEventContent content;
    content.algorithm = parseJsonStringValue(stateContentJson, "algorithm");

    auto rotMs = parseJsonStringValue(stateContentJson, "rotation_period_ms");
    if (!rotMs.empty()) {
        try { content.rotationPeriodMs = std::stoll(rotMs); } catch (...) {}
    }

    auto rotMsgs = parseJsonStringValue(stateContentJson, "rotation_period_msgs");
    if (!rotMsgs.empty()) {
        try { content.rotationPeriodMsgs = std::stoll(rotMsgs); } catch (...) {}
    }

    auto sessionBits = parseJsonStringValue(stateContentJson, "session_key_bits");
    if (!sessionBits.empty()) {
        try { content.sessionKeyBits = std::stoi(sessionBits); } catch (...) {}
    }

    return content;
}

// ==== Build Encryption Event JSON ====
//
// Original Kotlin (RoomEncryption.kt buildEvent):
//   {"type":"m.room.encryption","content":{"algorithm":"m.megolm.v1.aes-sha2",...},"state_key":""}

std::string buildEncryptionEvent(const RoomEncryptionEventContent& content) {
    std::ostringstream json;
    json << "{";
    json << R"("type":"m.room.encryption",)";
    json << R"("content":{)";
    json << R"("algorithm":")" << content.algorithm << R"(")";
    if (content.rotationPeriodMs > 0)
        json << R"(,"rotation_period_ms":)" << content.rotationPeriodMs;
    if (content.rotationPeriodMsgs > 0)
        json << R"(,"rotation_period_msgs":)" << content.rotationPeriodMsgs;
    if (content.sessionKeyBits > 0)
        json << R"(,"session_key_bits":)" << content.sessionKeyBits;
    json << R"(},)";
    json << R"("state_key":"")";
    json << "}";
    return json.str();
}

// ==== Get Encryption Algorithm ====
//
// Original Kotlin (RoomEncryption.kt getAlgorithm):
//   Extract the algorithm string from encryption content

std::string getEncryptionAlgorithm(const std::string& stateContentJson) {
    return parseJsonStringValue(stateContentJson, "algorithm");
}

// ==== Should Rotate Session ====
//
// Original Kotlin (RoomEncryption.kt shouldRotate):
//   Check both message count and time-based rotation thresholds

bool shouldRotateSession(
    const RoomEncryptionEventContent& content,
    int messageCount,
    int64_t sessionStartMs)
{
    if (!content.hasRotation()) return false;

    // Check message count threshold
    if (content.rotationPeriodMsgs > 0 && messageCount >= content.rotationPeriodMsgs)
        return true;

    // Check time-based threshold
    if (content.rotationPeriodMs > 0 && sessionStartMs > 0) {
        auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
        if (now - sessionStartMs >= content.rotationPeriodMs) return true;
    }

    return false;
}

// ==== EncryptionState Helpers ====
//
// Original Kotlin (RoomEncryption.kt state mapping)

std::string encryptionStateToString(EncryptionState state) {
    switch (state) {
        case EncryptionState::NOT_ENCRYPTED:     return "Not Encrypted";
        case EncryptionState::ENCRYPTING:         return "Encrypting...";
        case EncryptionState::ENCRYPTED:          return "Encrypted";
        case EncryptionState::DECRYPTION_ERROR:   return "Decryption Error";
    }
    return "Unknown";
}

EncryptionState determineEncryptionState(
    const std::string& stateContentJson,
    bool hasSessionKeys)
{
    // Original Kotlin: if no algorithm in content → NOT_ENCRYPTED
    auto algo = parseJsonStringValue(stateContentJson, "algorithm");
    if (algo.empty()) {
        return EncryptionState::NOT_ENCRYPTED;
    }

    // Original Kotlin: if encrypted but no keys → ENCRYPTING
    if (!hasSessionKeys) {
        return EncryptionState::ENCRYPTING;
    }

    return EncryptionState::ENCRYPTED;
}

// ==== Format Rotation Recommendation ====
//
// Original Kotlin: Provide a human-readable rotation timeline

std::string formatRotationRecommendation(
    const RoomEncryptionEventContent& content,
    int currentMessageCount,
    int64_t sessionStartMs)
{
    std::ostringstream out;

    if (!content.hasRotation()) {
        out << "Session key rotation is not configured. "
            << "The session will not be automatically rotated.";
        return out.str();
    }

    out << "Session key rotation: ";

    if (content.rotationPeriodMsgs > 0) {
        int remaining = content.rotationPeriodMsgs - currentMessageCount;
        if (remaining <= 0) {
            out << "rotation due now (message threshold reached)";
        } else {
            out << remaining << " messages until rotation";
        }
    }

    if (content.rotationPeriodMs > 0) {
        if (content.rotationPeriodMsgs > 0) out << ", ";
        auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
        int64_t elapsed = now - sessionStartMs;
        int64_t remainingMs = content.rotationPeriodMs - elapsed;
        if (remainingMs <= 0) {
            out << "rotation due now (time threshold reached)";
        } else {
            int64_t remainingSec = remainingMs / 1000;
            int64_t remainingMin = remainingSec / 60;
            int64_t remainingHr = remainingMin / 60;
            if (remainingHr > 0) {
                out << "approx. " << remainingHr << " hours until rotation";
            } else if (remainingMin > 0) {
                out << "approx. " << remainingMin << " minutes until rotation";
            } else {
                out << "approx. " << remainingSec << " seconds until rotation";
            }
        }
    }

    return out.str();
}

} // namespace progressive
