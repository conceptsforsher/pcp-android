#include "progressive/e2ee_decoration.hpp"
#include <sstream>

namespace progressive {

E2eeMessageDecoration computeE2eeDecoration(
    bool isEncrypted,
    bool isFromVerifiedDevice,
    bool isFromCrossSignedDevice,
    bool hasDecryptError,
    bool isFromBlacklistedDevice,
    bool sentBeforeWeJoined,
    const std::string& errorReason
) {
    E2eeMessageDecoration dec;

    if (!isEncrypted) {
        dec.state = E2eeState::None;
        dec.showShield = false;
    } else if (isFromBlacklistedDevice) {
        dec.state = E2eeState::Blacklisted;
        dec.isWarning = true;
        dec.showShield = true;
    } else if (hasDecryptError) {
        dec.state = E2eeState::Error;
        dec.isError = true;
        dec.showShield = true;
    } else if (sentBeforeWeJoined) {
        dec.state = E2eeState::AuthenticityUnknown;
        dec.isWarning = true;
        dec.showShield = true;
    } else if (isFromVerifiedDevice || isFromCrossSignedDevice) {
        dec.state = E2eeState::Verified;
        dec.showShield = true;
    } else {
        dec.state = E2eeState::Unverified;
        dec.isWarning = true;
        dec.showShield = true;
    }

    dec.iconRes = getE2eeIconName(dec.state);
    dec.tintColor = getE2eeColor(dec.state);
    dec.accessibility = getE2eeAccessibility(dec.state, errorReason);

    return dec;
}

std::string formatE2eeState(E2eeState state) {
    switch (state) {
        case E2eeState::None:                return "Not encrypted";
        case E2eeState::Verified:            return "Encrypted by verified device";
        case E2eeState::Unverified:          return "Encrypted by unverified device";
        case E2eeState::Warning:             return "Encrypted — warning";
        case E2eeState::Error:               return "Unable to decrypt";
        case E2eeState::Blacklisted:         return "From blacklisted device";
        case E2eeState::AuthenticityUnknown: return "Authenticity unknown";
        default:                             return "Unknown";
    }
}

std::string getE2eeIconName(E2eeState state) {
    switch (state) {
        case E2eeState::Verified:            return "ic_shield_trusted";
        case E2eeState::Unverified:
        case E2eeState::Warning:             return "ic_shield_warning";
        case E2eeState::Error:               return "ic_shield_error";
        case E2eeState::Blacklisted:         return "ic_shield_black";
        case E2eeState::AuthenticityUnknown: return "ic_shield_unknown";
        default:                             return "";
    }
}

std::string getE2eeColor(E2eeState state) {
    switch (state) {
        case E2eeState::Verified:            return "#4CAF50"; // green
        case E2eeState::Unverified:
        case E2eeState::Warning:             return "#FF9800"; // orange
        case E2eeState::Error:               return "#F44336"; // red
        case E2eeState::Blacklisted:         return "#000000"; // black
        case E2eeState::AuthenticityUnknown: return "#9E9E9E"; // grey
        default:                             return "#757575";
    }
}

std::string getE2eeAccessibility(E2eeState state, const std::string& errorReason) {
    auto base = formatE2eeState(state);
    if (state == E2eeState::Error && !errorReason.empty()) {
        return base + ": " + errorReason;
    }
    return base;
}

} // namespace progressive
