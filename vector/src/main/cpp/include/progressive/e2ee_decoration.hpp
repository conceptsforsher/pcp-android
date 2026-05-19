#ifndef PROGRESSIVE_E2EE_DECORATION_HPP
#define PROGRESSIVE_E2EE_DECORATION_HPP

#include <string>

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

} // namespace progressive

#endif // PROGRESSIVE_E2EE_DECORATION_HPP
