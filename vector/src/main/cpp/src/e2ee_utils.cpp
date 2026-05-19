#include "progressive/e2ee_utils.hpp"
#include <regex>

namespace progressive {

E2eeDecoration computeE2eeDecoration(
    bool isEncrypted, bool isVerifiedDevice, bool isCrossSigned,
    bool hasDecryptError, const std::string& errorReason, bool isBlacklisted
) {
    E2eeDecoration d;
    d.isEncrypted = isEncrypted;
    d.hasError = hasDecryptError;
    d.errorReason = errorReason;
    d.keyRequested = hasDecryptError;

    if (!isEncrypted) {
        d.trustLevel = TrustLevel::Unknown;
    } else if (isBlacklisted) {
        d.trustLevel = TrustLevel::Blacklisted;
    } else if (hasDecryptError) {
        d.trustLevel = TrustLevel::Warning;
    } else if (isVerifiedDevice || isCrossSigned) {
        d.trustLevel = TrustLevel::Verified;
    } else {
        d.trustLevel = TrustLevel::Warning;
    }

    return d;
}

std::string getTrustLabel(TrustLevel level) {
    switch (level) {
        case TrustLevel::Verified:    return "Verified";
        case TrustLevel::Warning:     return "Warning";
        case TrustLevel::Blacklisted: return "Blacklisted";
        default:                      return "Unknown";
    }
}

std::string getTrustBadge(TrustLevel level) {
    switch (level) {
        case TrustLevel::Verified:    return "\u2713";     // ✓
        case TrustLevel::Warning:     return "\u26A0";     // ⚠
        case TrustLevel::Blacklisted: return "\u2717";     // ✗
        default:                      return "?";
    }
}

bool requiresCrossSigning(const std::string& algorithm) {
    return algorithm == "m.megolm.v1.aes-sha2" ||
           algorithm == "m.olm.v1.curve25519-aes-sha2";
}

bool isValidDeviceKey(const std::string& key, const std::string& keyType) {
    if (key.empty()) return false;

    if (keyType == "ed25519") {
        // Ed25519 key: "curve25519:" + 43 base64 chars
        std::regex edRe(R"(^curve25519:[A-Za-z0-9+/]{43}$)");
        return std::regex_match(key, edRe);
    }

    if (keyType == "curve25519") {
        // Same format as ed25519 for Matrix
        std::regex cvRe(R"(^curve25519:[A-Za-z0-9+/]{43}$)");
        return std::regex_match(key, cvRe);
    }

    return key.size() >= 8;
}

bool deviceIdMatches(const std::string& localDeviceId, const std::string& remoteDeviceId) {
    return localDeviceId == remoteDeviceId;
}

TrustLevel parseVerificationState(const std::string& stateJson) {
    // Parse simple state strings from Matrix verification events
    if (stateJson.find("verified") != std::string::npos) return TrustLevel::Verified;
    if (stateJson.find("blacklist") != std::string::npos) return TrustLevel::Blacklisted;
    if (stateJson.find("warning") != std::string::npos) return TrustLevel::Warning;
    return TrustLevel::Unknown;
}

} // namespace progressive
