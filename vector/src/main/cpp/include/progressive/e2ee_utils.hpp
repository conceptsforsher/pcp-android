#ifndef PROGRESSIVE_E2EE_UTILS_HPP
#define PROGRESSIVE_E2EE_UTILS_HPP

#include <string>

namespace progressive {

// E2EE trust level based on cross-signing and device verification.
enum class TrustLevel { Unknown, Warning, Verified, Blacklisted };

// Decoration info for a message in the timeline.
struct E2eeDecoration {
    TrustLevel trustLevel = TrustLevel::Unknown;
    bool isEncrypted = false;
    bool hasError = false;          // UISI — unable to decrypt
    bool keyRequested = false;      // re-requested keys
    std::string errorReason;        // human-readable reason
};

// Compute E2EE decoration from event properties.
E2eeDecoration computeE2eeDecoration(
    bool isEncrypted,
    bool isVerifiedDevice,
    bool isCrossSigned,
    bool hasDecryptError,
    const std::string& errorReason,
    bool isBlacklisted
);

// Get a human-readable trust label: "Verified", "Warning", "Encrypted", etc.
std::string getTrustLabel(TrustLevel level);

// Get a short trust label for badges: "V", "W", "E", "!".
std::string getTrustBadge(TrustLevel level);

// Check if an encryption algorithm requires cross-signing.
bool requiresCrossSigning(const std::string& algorithm);

// Validate a Matrix device key (Curve25519 or Ed25519).
bool isValidDeviceKey(const std::string& key, const std::string& keyType = "ed25519");

// Check if two device IDs match (case-sensitive prefix match).
bool deviceIdMatches(const std::string& localDeviceId, const std::string& remoteDeviceId);

// Parse a Matrix device verification state from JSON.
TrustLevel parseVerificationState(const std::string& stateJson);

} // namespace progressive

#endif // PROGRESSIVE_E2EE_UTILS_HPP
