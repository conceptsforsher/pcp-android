#ifndef PROGRESSIVE_EVENT_ENCRYPTION_HPP
#define PROGRESSIVE_EVENT_ENCRYPTION_HPP

#include <string>
#include <vector>

namespace progressive {

// ---- Event Encryption Utilities ----

struct EncryptionAlgorithm {
    std::string name;              // "m.megolm.v1.aes-sha2", "m.olm.v1.curve25519-aes-sha2"
    bool isMegolm = false;         // group encryption
    bool isOlm = false;            // 1:1 encryption
    bool isDefault = false;
    std::string keySize;           // "256"
    std::string cipher;            // "aes-sha2"
};

struct EncryptedEventHeader {
    std::string algorithm;
    std::string senderKey;
    std::string deviceId;
    std::string sessionId;
    int messageIndex = 0;
    bool valid = false;
};

struct EncryptedContent {
    std::string ciphertext;
    std::string senderKey;
    std::string deviceId;
    std::string sessionId;
    int messageIndex = 0;
};

// Parse encryption algorithm from room state or event.
EncryptionAlgorithm parseEncryptionAlgorithm(const std::string& algorithmStr);

// Parse encrypted event content header.
EncryptedEventHeader parseEncryptedHeader(const std::string& contentJson);

// Extract sender key from encrypted event.
std::string extractSenderKey(const std::string& contentJson);

// Extract session ID from encrypted event.
std::string extractSessionId(const std::string& contentJson);

// Get the list of known encryption algorithms.
std::vector<EncryptionAlgorithm> getKnownAlgorithms();

// Check if an algorithm is considered secure.
bool isSecureAlgorithm(const std::string& algorithm);

// Check if two encrypted events use the same session.
bool isSameSession(const EncryptedEventHeader& a, const EncryptedEventHeader& b);

// Format encryption info for debug display.
std::string formatEncryptionInfo(const EncryptedEventHeader& header);

// ---- Olm/Megolm Session Tracking ----

struct SessionUsage {
    std::string sessionId;
    std::string senderKey;
    int messageCount = 0;
    int firstIndex = 0;
    int lastIndex = 0;
    int missedIndices = 0;         // gaps in message sequence
    int64_t firstSeenMs = 0;
    int64_t lastSeenMs = 0;
    bool isActive = true;
};

// Track session usage across multiple encrypted events.
std::vector<SessionUsage> trackSessionUsage(
    const std::vector<std::string>& sessionIds,
    const std::vector<std::string>& senderKeys,
    const std::vector<int>& messageIndices,
    const std::vector<int64_t>& timestamps
);

// Detect missed message indices in a session.
int detectMissedIndices(const std::vector<int>& indices);

// Check if a session needs key re-request.
bool needsKeyRequest(const SessionUsage& session, int maxMissed = 5);

// Format session usage summary.
std::string formatSessionUsage(const SessionUsage& session);

} // namespace progressive

#endif // PROGRESSIVE_EVENT_ENCRYPTION_HPP
