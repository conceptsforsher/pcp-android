#ifndef PROGRESSIVE_E2EE_DECORATION_HPP
#define PROGRESSIVE_E2EE_DECORATION_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// ---- E2EE Message Decoration ----

enum class E2eeState {
    None,              // not encrypted
    Verified,          // trusted device
    Unverified,        // unknown device
    Warning,           // unverified + unencrypted
    Error,             // UISI — unable to decrypt
    Blacklisted,       // blocked device
    AuthenticityUnknown // sent before we joined
};

struct E2eeMessageDecoration {
    E2eeState state = E2eeState::None;

    // Visual
    std::string iconRes;       // drawable resource name for Kotlin
    std::string tintColor;     // "#FF0000" format
    std::string accessibility; // screen reader text

    // Computed
    bool showShield = false;   // should display shield icon
    bool isWarning = false;    // red/warning color
    bool isError = false;      // UISI error
};

// Compute E2EE decoration for a message based on device trust and encryption state.
E2eeMessageDecoration computeE2eeDecoration(
    bool isEncrypted,
    bool isFromVerifiedDevice,
    bool isFromCrossSignedDevice,
    bool hasDecryptError,
    bool isFromBlacklistedDevice,
    bool sentBeforeWeJoined,
    const std::string& errorReason
);

// Format E2EE state as human-readable text.
std::string formatE2eeState(E2eeState state);

// Get an icon name for the E2EE state.
std::string getE2eeIconName(E2eeState state);

// Get a color for the E2EE state (#RRGGBB format).
std::string getE2eeColor(E2eeState state);

// Get accessibility description for the E2EE state.
std::string getE2eeAccessibility(E2eeState state, const std::string& errorReason);

// ================================================================
// Room-Level Trust Computation (ported from ShieldSummaryUpdater +
// ComputeShieldForGroupUseCase)
// ================================================================

// Room encryption trust level.
// Ported from RoomEncryptionTrustLevel.kt
enum class RoomEncryptionTrustLevel {
    Default,                    // Black shield — no one verified
    Warning,                    // Red shield — some devices unverified
    Trusted,                    // Green shield — all devices verified
    E2EWithUnsupportedAlgorithm
};

// Per-device trust information.
struct DeviceTrustInfo {
    std::string deviceId;
    std::string userId;
    bool isVerified = false;            // legacy verification
    bool isCrossSigningVerified = false;  // cross-signing verified
    bool isBlacklisted = false;
};

// Per-user identity information.
struct UserIdentityInfo {
    std::string userId;
    bool isTrusted = false;        // current trust via cross-signing
    bool wasTrustedOnce = false;   // previously verified (now expired/revoked)
};

// Compute shield for a group of users (the room-level trust).
//
// Ported from ComputeShieldForGroupUseCase.kt
//
// Logic:
//   1. Filter trusted users (isTrusted() or wasTrustedOnce)
//   2. If none trusted → Default (black shield)
//   3. Check all devices of trusted users:
//      a. If ANY device is unverified → Warning (red shield)
//      b. If all devices verified AND all users trusted → Trusted (green)
//      c. Otherwise → Default (black)
//
// Parameters:
//   myUserId:       our own user ID (may be empty if no identity)
//   userIdentities: map of userId → UserIdentityInfo for each room member
//   userDevices:    map of userId → list of DeviceTrustInfo for each room member
//
RoomEncryptionTrustLevel computeRoomShield(
    const std::string& myUserId,
    const std::vector<UserIdentityInfo>& userIdentities,
    const std::vector<DeviceTrustInfo>& userDevices
);

// Convert RoomEncryptionTrustLevel to a human-readable label.
const char* roomShieldToString(RoomEncryptionTrustLevel level);

// Get the shield icon name for a room trust level.
const char* roomShieldIconName(RoomEncryptionTrustLevel level);

// Check if a room shield update is needed (for ShieldSummaryUpdater).
// Returns true if the shield level changed and the room needs a refresh.
bool shieldLevelChanged(
    RoomEncryptionTrustLevel oldLevel,
    RoomEncryptionTrustLevel newLevel
);

} // namespace progressive

#endif // PROGRESSIVE_E2EE_DECORATION_HPP
