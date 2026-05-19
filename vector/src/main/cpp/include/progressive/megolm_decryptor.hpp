#pragma once

#include <string>
#include <vector>
#include <map>
#include <cstdint>
#include <unordered_map>
#include <memory>

namespace progressive {

// ==== Megolm Decryptor ====
//
// Wraps libolm's inbound_group_session for Megolm message decryption.
// Manages imported room keys and decrypts Megolm-encrypted events.
// Opt-in via Labs: SETTINGS_LABS_NATIVE_CRYPTO

struct MegolmSession {
    void* session = nullptr;      // OlmInboundGroupSession*
    std::string sessionId;        // Unique session identifier
    std::string senderKey;        // Curve25519 key of sender
    uint32_t firstKnownIndex = 0; // First known message index
    bool valid = false;
};

// Parse session key (base64-encoded Megolm session export).
// Returns false if the key is invalid.
bool parseMegolmSessionKey(const std::string& keyBase64, std::vector<uint8_t>& sessionKey);

// Create an inbound Megolm session from a session key.
// sessionKey: raw bytes from room_key event
// Returns a valid MegolmSession or invalid on failure.
MegolmSession createInboundMegolmSession(const std::vector<uint8_t>& sessionKey);

// Destroy a Megolm session.
void destroyMegolmSession(MegolmSession& session);

// Decrypt a Megolm-encrypted message.
// session: the inbound Megolm session
// ciphertext: raw ciphertext bytes
// Returns: decrypted plaintext, or empty string on failure
std::string megolmDecrypt(MegolmSession& session, const std::string& ciphertext);

// Get the session ID (for matching events to sessions).
std::string getMegolmSessionId(const MegolmSession& session);

// Export a Megolm session (for key backup/sharing).
std::string exportMegolmSession(const MegolmSession& session);

// ============================================================================
// Megolm Inbound Session Model (ported from Kotlin)
// ============================================================================
//
// Original Kotlin (MegolmSessionData.kt + MegolmInboundSessionInfo):
//   data class MegolmSessionData(algorithm, sessionId, senderKey, roomId, sessionKey, ...)
//   data class MegolmInboundSessionInfo(sessionId, senderKey, ed25519Key, roomId, ...)

// ==== MegolmInboundSession ====
//
// Represents an active inbound Megolm session.
// Tracks sender, key chain, verification status, and session metadata.
//
// Original Kotlin (MegolmInboundSessionInfo):
//   Derived from MXMegolmSessionData, stored in crypto store per room.

struct MegolmInboundSession {
    std::string sessionId;                              // Unique session identifier
    std::string senderKey;                              // Curve25519 key of original sender
    std::string ed25519Key;                             // Claimed Ed25519 key of original sender
    std::string roomId;                                 // Room this session is for
    std::string algorithm;                              // "m.megolm.v1.aes-sha2"
    int64_t firstKnownIndex = 0;                        // First message index known for this session
    bool isInbound = true;                              // Always true for inbound sessions
    bool hasReceived = false;                           // Whether we've received any messages
    bool isVerified = false;                            // Whether sender is verified (cross-signing)
    bool hasBeenBackedUp = false;                       // Whether uploaded to key backup
    std::vector<std::string> forwardingCurve25519KeyChain; // Forwarded key chain
    bool sharedHistory = false;                         // MSC3061 shared history flag
    int64_t lastReceivedTs = 0;                         // Timestamp of last received message
    int64_t messageCount = 0;                           // Number of messages decrypted
};

// ==== MegolmOutboundSession ====
//
// Represents an active outbound Megolm session.
// Tracks message count, creation time, and sharing status.

struct MegolmOutboundSession {
    std::string sessionId;                              // Unique session identifier
    std::string roomId;                                 // Room this session is for
    int64_t messageCount = 0;                           // Number of messages sent
    int64_t creationTs = 0;                             // Creation epoch millis
    int sharedWithCount = 0;                            // Number of devices shared with
};

// ============================================================================
// Megolm Session Store
// ============================================================================
//
// Original Kotlin (CryptoStore + RoomKey logic):
//   Stores inbound/outbound Megolm sessions indexed by roomId + sessionId.
//   Provides lookup for decryption and session management.

class MegolmSessionStore {
public:
    // Get an inbound session by ID.
    MegolmInboundSession* getInboundSession(const std::string& sessionId);

    // Get all inbound sessions for a room.
    //
    // Original Kotlin (CryptoStore.getInboundMegolmSessions(roomId)):
    std::vector<MegolmInboundSession> getSessionsForRoom(const std::string& roomId) const;

    // Add a new inbound session.
    //
    // Original Kotlin: store imported room key as inbound session
    void addInboundSession(const MegolmInboundSession& session);

    // Add a new outbound session.
    void addOutboundSession(const MegolmOutboundSession& session);

    // Get an outbound session by ID.
    MegolmOutboundSession* getOutboundSession(const std::string& sessionId);

    // Get all outbound sessions for a room.
    std::vector<MegolmOutboundSession> getOutboundSessionsForRoom(const std::string& roomId) const;

    // Remove a session by ID.
    void removeSession(const std::string& sessionId);

    // Remove all sessions for a room.
    void clearRoom(const std::string& roomId);

    // Remove all sessions.
    void clearAll();

    // Get the best session for a room (by message count / recency).
    //
    // Original Kotlin: pick most recently used inbound session for sending
    MegolmInboundSession* getBestSession(const std::string& roomId);

    // Total inbound session count.
    size_t inboundCount() const;

    // Total outbound session count.
    size_t outboundCount() const;

private:
    std::unordered_map<std::string, MegolmInboundSession> inboundSessions_;   // sessionId -> session
    std::unordered_map<std::string, MegolmOutboundSession> outboundSessions_; // sessionId -> session
};

// ============================================================================
// Megolm Session Verification
// ============================================================================
//
// Original Kotlin (CryptoService.deviceWithIdentityKey + MXDeviceInfo):
//   Verifies that the sender's device is trusted via cross-signing or
//   direct verification. Used to decorate encrypted messages with trust level.

// Check if a Megolm session's sender is verified.
// Verification is based on:
//   - Device is verified (direct verification)
//   - User is cross-signed (SSSS cross-signing trust)
//   - Forwarding chain is trusted
//
// Original Kotlin (E2eeDecoration.kt + MXEventDecryptionResult.verificationState):
bool isMegolmSessionVerified(const MegolmInboundSession& session);

// ============================================================================
// Megolm Session Sharing Decision
// ============================================================================
//
// Original Kotlin (RoomHistoryVisibility.kt:55-56):
//   fun RoomHistoryVisibility.shouldShareHistory() =
//       this == WORLD_READABLE || this == SHARED
//
// Also: key sharing is controlled by history visibility, key forwarding
// settings, and device trust levels.

// Determine whether a Megolm session should be shared (key forwarding).
//
// Original Kotlin: checks room history visibility + key forwarding settings
//   - WORLD_READABLE / SHARED history -> always share
//   - INVITED / JOINED -> only share if key forwarding is enabled
//
// Parameters:
//   - historyVisibility: "world_readable", "shared", "invited", "joined"
//   - keyForwardingEnabled: from SETTINGS_CRYPTO_USE_KEY_FORWARDING
//   - encryptToDeviceOnly: from SETTINGS_ENCRYPTION_USE_ENCRYPTION_TO_DEVICE_ONLY
bool shouldShareMegolmSession(const std::string& historyVisibility,
                               bool keyForwardingEnabled,
                               bool encryptToDeviceOnly);

// ============================================================================
// Megolm Key Export/Import (ported from MXMegolmExportEncryption.kt)
// ============================================================================
//
// Original Kotlin (MXMegolmExportEncryption.kt:34-351):
//   Utility class for encrypting/decrypting Megolm session keys for
//   export to file. Uses:
//     - PBKDF2 with HMAC-SHA512 (key derivation from password)
//     - AES/CTR/NoPadding (encryption)
//     - HMAC-SHA256 (authentication)
//     - Base64 + ASCII armoring

// ==== MegolmKeyExportData ====
//
// Structure of an exported Megolm key file (after unpacking armor).
// Format: version | salt(16) | iv(16) | iterations(4) | ciphertext | hmac(32)

struct MegolmKeyExportData {
    std::vector<uint8_t> encryptedKey;    // ciphertext of the session key
    std::vector<uint8_t> iv;              // initialization vector (16 bytes)
    std::vector<uint8_t> mac;             // HMAC-SHA256 (32 bytes)
    std::vector<uint8_t> salt;            // PBKDF2 salt (16 bytes)
    int kdfRounds = 500000;               // PBKDF2 iteration count
};

// Decrypt a Megolm key export file.
//
// Original Kotlin (MXMegolmExportEncryption.kt:77-136):
//   fun decryptMegolmKeyFile(data: ByteArray, password: String): String
//
// The data is ASCII-armored (-----BEGIN/END MEGOLM SESSION DATA-----).
// Internally: version(1) | salt(16) | iv(16) | iterations(4) | ciphertext | hmac(32)
//
// Returns the decrypted session key data, or empty string on failure.
std::string decryptMegolmKeyExport(const std::string& armoredData, const std::string& password);

// Encrypt a Megolm key for export.
//
// Original Kotlin (MXMegolmExportEncryption.kt:149-209):
//   fun encryptMegolmKeyFile(data: String, password: String, kdfRounds: Int): ByteArray
//
// Returns ASCII-armored encrypted key data.
std::string encryptMegolmKeyExport(const std::string& sessionKeyData,
                                    const std::string& password,
                                    int kdfRounds = 500000);

// ==== Megolm Session Key Export to ASCII-armored format ====

// Convert a raw Megolm session key to the ASCII-armored export format.
// This wraps the raw key bytes with -----BEGIN/END MEGOLM SESSION DATA-----
// and Base64 encoding.
//
// Original Kotlin (MXMegolmExportEncryption.kt:280-302):
//   fun packMegolmKeyFile(data: ByteArray): ByteArray
std::string packMegolmKeyFile(const std::string& rawData);

// Extract raw data from an ASCII-armored Megolm key file.
//
// Original Kotlin (MXMegolmExportEncryption.kt:219-271):
//   fun unpackMegolmKeyFile(data: ByteArray): ByteArray?
std::string unpackMegolmKeyFile(const std::string& armoredData);

// ==== Megolm Export Header/Trailer Constants ====
//
// Original Kotlin (MXMegolmExportEncryption.kt:38-39):
//   const val HEADER_LINE = "-----BEGIN MEGOLM SESSION DATA-----"
//   const val TRAILER_LINE = "-----END MEGOLM SESSION DATA-----"

namespace MegolmExport {
    constexpr const char* HEADER_LINE = "-----BEGIN MEGOLM SESSION DATA-----";
    constexpr const char* TRAILER_LINE = "-----END MEGOLM SESSION DATA-----";
    constexpr int DEFAULT_ITERATION_COUNT = 500000;
    constexpr int LINE_LENGTH = 72 * 4 / 3;  // base64 line length
}

// ============================================================================
// Megolm Session Manager (existing, kept for backward compat)
// ============================================================================

class MegolmSessionManager {
public:
    // Add a session for a room/sender key pair.
    bool addSession(const std::string& roomId, const std::string& senderKey,
                    const std::string& sessionId, const std::string& sessionKeyBase64);

    // Find a matching session for an event.
    MegolmSession* findSession(const std::string& roomId, const std::string& senderKey,
                               const std::string& sessionId);

    // Remove all sessions for a room.
    void clearRoom(const std::string& roomId);

    // Remove all sessions.
    void clearAll();

    // Session count.
    int sessionCount() const { return (int)sessions_.size(); }

private:
    struct SessionKey {
        std::string roomId;
        std::string senderKey;
        std::string sessionId;
        bool operator==(const SessionKey& o) const {
            return roomId == o.roomId && senderKey == o.senderKey && sessionId == o.sessionId;
        }
    };
    struct SessionKeyHash {
        size_t operator()(const SessionKey& k) const {
            return std::hash<std::string>()(k.roomId + k.senderKey + k.sessionId);
        }
    };
    std::unordered_map<SessionKey, MegolmSession, SessionKeyHash> sessions_;
};

} // namespace progressive
