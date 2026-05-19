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
    // Only known algorithms are considered secure
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

    // Compute missed indices
    std::vector<SessionUsage> result;
    for (auto& p : sessions) {
        auto& usage = p.second;
        // Simple consecutive check — in real impl would collect all indices
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

} // namespace progressive
