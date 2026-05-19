#ifndef PROGRESSIVE_CROSS_SIGNING_HPP
#define PROGRESSIVE_CROSS_SIGNING_HPP

#include <string>
#include <vector>
#include <map>

namespace progressive {

// ---- Matrix Cross-Signing Utilities ----
// Ported from: org.matrix.android.sdk.api.session.crypto.crosssigning.CryptoCrossSigningKey.kt (110L)
//              org.matrix.android.sdk.internal.crypto.CrossSigningService.kt

enum class CrossSigningKey { Master, SelfSigning, UserSigning, Unknown };

// Key usage enum from CryptoCrossSigningKey.kt:KeyUsage
enum class KeyUsage { Master, SelfSigning, UserSigning };

inline std::string keyUsageToString(KeyUsage u) {
    switch (u) { case KeyUsage::Master: return "master"; case KeyUsage::SelfSigning: return "self_signing"; case KeyUsage::UserSigning: return "user_signing"; }
    return "";
}

struct CrossSigningStatus {
    bool masterKeyExists = false;
    bool selfSigningKeyExists = false;
    bool userSigningKeyExists = false;
    bool isSetup = false;           // all three keys present
    bool isVerified = false;        // master key verified by user
    bool needsBootstrap = false;    // keys missing, need to create
    std::string masterKeyId;        // base64-encoded public key
};

struct CrossSigningReset {
    bool canReset = false;
    bool needsAuth = false;         // UIA required
    std::string warningMessage;     // "This will invalidate all verified devices"
};

// Parse cross-signing status from account data.
CrossSigningStatus parseCrossSigningStatus(const std::string& accountDataJson, const std::string& userId);

// Check if cross-signing needs to be set up.
bool needsCrossSigningSetup(const CrossSigningStatus& status);

// Check if cross-signing reset is allowed.
CrossSigningReset checkResetEligibility(const CrossSigningStatus& status, bool hasPasswordAuth);

// Format cross-signing status for display.
std::string formatCrossSigningStatus(const CrossSigningStatus& status);

// Get the local storage key for a cross-signing key type.
std::string getCrossSigningStorageKey(CrossSigningKey keyType, const std::string& userId);

// Parse key ID from cross-signing key content.
std::string parseCrossSigningKeyId(const std::string& keyContentJson);

// Check if a key is signed by the master key.
bool isKeySignedByMaster(const std::string& keyJson, const std::string& masterKeyId);

// Build cross-signing bootstrap request body.
std::string buildBootstrapBody(const std::string& masterKey, const std::string& selfSigningKey,
    const std::string& userSigningKey, const std::string& masterKeySignature,
    const std::string& selfSigningSignature, const std::string& userSigningSignature);

} // namespace progressive

#endif // PROGRESSIVE_CROSS_SIGNING_HPP
