#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>

namespace progressive {

// ================================================================
// Cross-Signing Manager — full cross-signing setup & trust chain
//
// Faithful port from Element Android original sources:
//   CrossSigningService.kt — isCrossSigningInitialized,
//     checkTrustFromPrivateKeys, getUserCrossSigningKeys,
//     getMyCrossSigningKeys, trustUser, markMyMasterKeyAsTrusted,
//     trustDevice, checkDeviceTrust, checkSelfTrust
//   MXCrossSigningInfo.kt — userId, crossSigningKeys[],
//     isTrusted(), masterKey(), userKey(), selfSigningKey()
//   CryptoCrossSigningKey.kt — userId, usages[], keys{},
//     signatures{}, isMasterKey/isSelfSigningKey/isUserKey,
//     addSignatureAndCopy, copyForSignature, Builder
//   CSM_KeyUsage — MASTER("master"), SELF_SIGNING("self_signing"),
//     USER_SIGNING("user_signing")
//   DeviceTrustLevel.kt — crossSigningVerified, locallyVerified
//   UserIdentity.kt — identity verified state
//
// Covers:
//   1. Three-key cross-signing (MSK/USK/SSK)
//   2. Key generation and upload
//   3. Trust chain verification
//   4. Device signing/trust
//   5. User trust marking
//   6. Self-trust checking
//   7. Private key import from backup
// ================================================================

// ---- Key Usage ----
// Original: CSM_KeyUsage enum (MASTER, SELF_SIGNING, USER_SIGNING)

enum class CSM_KeyUsage {
    MASTER = 0,          // Master key — signs other keys
    SELF_SIGNING = 1,    // Signs own devices
    USER_SIGNING = 2,    // Signs other users
};

const char* keyUsageToString(CSM_KeyUsage usage);
CSM_KeyUsage keyUsageFromString(const std::string& s);

// ---- Cross-Signing Key ----
// Original: CryptoCrossSigningKey.kt (userId, usages[], keys{}, signatures{}, trustLevel)

struct CSM_CrossSigningKey {
    std::string userId;
    std::vector<CSM_KeyUsage> usages;
    std::unordered_map<std::string, std::string> keys;      // "ed25519:base64key" → base64key
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> signatures;
    bool crossSigningVerified = false;
    bool locallyVerified = false;
    bool valid = false;

    // Original: isMasterKey / isSelfSigningKey / isUserKey
    bool isMasterKey() const;
    bool isSelfSigningKey() const;
    bool isUserKey() const;

    // Original: unpaddedBase64PublicKey — first key value
    std::string getPublicKey() const;

    // Original: addSignatureAndCopy
    void addSignature(const std::string& signerUserId, const std::string& signedKey,
                      const std::string& signature);

    // Original: isVerified
    bool isVerified() const { return crossSigningVerified || locallyVerified; }

    // Builder pattern
    struct Builder {
        std::string userId;
        CSM_KeyUsage usage;
        std::string publicKey;
        struct SignatureEntry {
            std::string userId;
            std::string keySigned;
            std::string signature;
        };
        std::vector<SignatureEntry> sigs;

        Builder& key(const std::string& pk) { publicKey = pk; return *this; }
        Builder& signature(const std::string& uid, const std::string& ks, const std::string& sgn) {
            sigs.push_back({uid, ks, sgn}); return *this;
        }
        CSM_CrossSigningKey build() const;
    };
};

// ---- Cross-Signing Info ----
// Original: MXCrossSigningInfo.kt (userId, crossSigningKeys[], wasTrustedOnce)

struct CSM_CrossSigningInfo {
    std::string userId;
    std::vector<CSM_CrossSigningKey> keys;   // MSK, USK, SSK
    bool wasTrustedOnce = false;
    bool valid = false;

    // Original: isTrusted() — masterKey + selfSigningKey both verified
    bool isTrusted() const;

    // Original: masterKey(), userKey(), selfSigningKey()
    const CSM_CrossSigningKey* masterKey() const;
    const CSM_CrossSigningKey* userKey() const;
    const CSM_CrossSigningKey* selfSigningKey() const;
};

// ---- User Trust Result ----
// Original: UserTrustResult

struct UserTrustResult {
    bool isTrusted = false;
    bool crossSigningVerified = false;
    bool locallyVerified = false;
    std::string error;
};

// ---- Device Trust Result ----
// Original: DeviceTrustResult

struct DeviceTrustResult {
    bool isTrusted = false;
    bool crossSigningVerified = false;
    bool locallyVerified = false;
    std::string deviceId;
    std::string userId;
};

// ---- Private Keys Info ----
// Original: PrivateKeysInfo

struct PrivateKeysInfo {
    std::string masterKeyPrivate;       // MSK private
    std::string selfSigningKeyPrivate;  // SSK private
    std::string userSigningKeyPrivate;  // USK private
    bool hasMaster = false;
    bool hasSelfSigning = false;
    bool hasUserSigning = false;

    // Original: allPrivateKeysKnown()
    bool allPrivateKeysKnown() const { return hasMaster && hasSelfSigning && hasUserSigning; }
};

// ---- Room Encryption Trust Level ----
// Original: CS_RoomEncryptionTrustLevel

enum class CS_RoomEncryptionTrustLevel {
    TRUSTED = 0,         // All members verified
    WARNING = 1,         // Some unverified
    UNTRUSTED = 2,       // Unknown/untrusted devices
    NORMAL = 3,          // Default
};

// ---- Cross-Signing Manager ----

class CrossSigningManager {
public:
    CrossSigningManager();

    // ====== Cross-Signing Status ======
    // Original: isCrossSigningInitialized / isCrossSigningVerified

    // Check if cross-signing has been set up (public keys uploaded).
    bool isInitialized() const;

    // Check if the current user's identity is fully verified.
    bool isVerified() const;

    // Check if we can cross-sign (have private keys).
    bool canCrossSign() const;

    // Check if all private keys are known.
    bool allPrivateKeysKnown() const;

    // ====== Key Management ======
    // Original: getMyCrossSigningKeys / getUserCrossSigningKeys

    // Set our own cross-signing keys.
    void setMyKeys(const CSM_CrossSigningInfo& info);

    // Set another user's cross-signing keys.
    void setUserKeys(const std::string& userId, const CSM_CrossSigningInfo& info);

    // Get our cross-signing keys.
    CSM_CrossSigningInfo getMyKeys() const;

    // Get a user's cross-signing keys.
    CSM_CrossSigningInfo getUserKeys(const std::string& userId) const;

    // ====== Private Key Import ======
    // Original: checkTrustFromPrivateKeys(masterKeyPrivate, uskPrivate, sskPrivate)

    // Import private keys and verify they match public keys.
    UserTrustResult importPrivateKeys(const std::string& masterKeyPrivate,
                                       const std::string& userSigningKeyPrivate,
                                       const std::string& selfSigningKeyPrivate);

    // Import a single private key by usage.
    bool importPrivateKey(CSM_KeyUsage usage, const std::string& privateKey);

    // Get our private keys info.
    PrivateKeysInfo getPrivateKeys() const;

    // ====== Trust Chain Verification ======
    // Original: checkSelfTrust / checkOtherMSKTrusted / checkDeviceTrust

    // Verify our own trust chain.
    UserTrustResult checkSelfTrust() const;

    // Check if another user is trusted (our MSK signed their MSK).
    UserTrustResult checkUserTrust(const std::string& otherUserId) const;

    // Check if a specific device is trusted.
    DeviceTrustResult checkDeviceTrust(const std::string& userId, const std::string& deviceId,
                                        bool locallyTrusted) const;

    // ====== Trust Operations ======
    // Original: trustUser / markMyMasterKeyAsTrusted / trustDevice

    // Mark our own master key as trusted.
    void markMyMasterKeyAsTrusted();

    // Mark another user as trusted (sign their MSK with our USK).
    void trustUser(const std::string& otherUserId);

    // Sign one of our own devices with our SSK.
    void trustDevice(const std::string& deviceId);

    // ====== Key Building ======
    // Original: CryptoCrossSigningKey.Builder

    // Build a master key.
    static CSM_CrossSigningKey buildMasterKey(const std::string& userId, const std::string& publicKey);

    // Build a self-signing key.
    static CSM_CrossSigningKey buildSelfSigningKey(const std::string& userId, const std::string& publicKey);

    // Build a user-signing key.
    static CSM_CrossSigningKey buildUserSigningKey(const std::string& userId, const std::string& publicKey);

    // Build cross-signing info from three keys.
    static CSM_CrossSigningInfo buildCrossSigningInfo(const std::string& userId,
                                                    const CSM_CrossSigningKey& msk,
                                                    const CSM_CrossSigningKey& usk,
                                                    const CSM_CrossSigningKey& ssk);

    // ====== Serialization ======

    // Export cross-signing info as JSON.
    std::string crossSigningInfoToJson(const CSM_CrossSigningInfo& info) const;

    // Export key as JSON.
    std::string keyToJson(const CSM_CrossSigningKey& key) const;

    // Export trust result as JSON.
    std::string trustResultToJson(const UserTrustResult& result) const;
    std::string deviceTrustToJson(const DeviceTrustResult& result) const;

private:
    CSM_CrossSigningInfo myKeys_;
    std::unordered_map<std::string, CSM_CrossSigningInfo> userKeys_; // userId → keys
    PrivateKeysInfo privateKeys_;

    // Check if a key's signatures are valid.
    bool verifyKeySignatures(const CSM_CrossSigningKey& key) const;

    // Verify chain: MSK → USK → user MSK (for other users)
    // Verify chain: MSK → SSK → device (for own devices)
};

} // namespace progressive
