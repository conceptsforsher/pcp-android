#pragma once

#include <string>
#include <vector>
#include <map>

namespace progressive {

// ---- Matrix Cross-Signing Utils ----
// Original Kotlin:
//   CryptoCrossSigningKey.kt (110L)
//   MXCrossSigningInfo.kt (36L)
//   DeviceTrustLevel.kt (25L)
//   RoomEncryptionTrustLevel.kt (42L)
//   CrossSigningService.kt (125L)

// ---- Key Usage / Key Type Enums ----

enum class CrossSigningKeyType { Master, SelfSigning, UserSigning, Unknown };

enum class KeyUsage { Master, SelfSigning, UserSigning };

inline std::string keyUsageToString(KeyUsage u) {
    switch (u) {
        case KeyUsage::Master:       return "master";
        case KeyUsage::SelfSigning:  return "self_signing";
        case KeyUsage::UserSigning:  return "user_signing";
    }
    return "";
}

inline KeyUsage keyUsageFromString(const std::string& s) {
    if (s == "master")        return KeyUsage::Master;
    if (s == "self_signing")  return KeyUsage::SelfSigning;
    if (s == "user_signing")  return KeyUsage::UserSigning;
    return KeyUsage::Master;
}

// ---- Cross-Signing State ----
// Original Kotlin: conceptually derived from CrossSigningService state checks

enum class CrossSigningState {
    NOT_BOOTSTRAPPED,      // No keys uploaded at all
    CROSS_SIGNING_EXISTS,  // Public keys exist but private not available
    CAN_CROSS_SIGN,        // Private keys available, can sign
    TRUSTED                // Master key verified
};

inline std::string crossSigningStateToString(CrossSigningState s) {
    switch (s) {
        case CrossSigningState::NOT_BOOTSTRAPPED:     return "NOT_BOOTSTRAPPED";
        case CrossSigningState::CROSS_SIGNING_EXISTS: return "CROSS_SIGNING_EXISTS";
        case CrossSigningState::CAN_CROSS_SIGN:       return "CAN_CROSS_SIGN";
        case CrossSigningState::TRUSTED:              return "TRUSTED";
    }
    return "UNKNOWN";
}

// ---- Device Trust Level ----
// Original Kotlin: DeviceTrustLevel.kt (25L)

struct DeviceTrustLevel {
    bool crossSigningVerified = false;
    bool locallyVerified = false;

    bool isVerified() const { return crossSigningVerified || locallyVerified; }
    bool isCrossSigningVerified() const { return crossSigningVerified; }
    bool isLocallyVerified() const { return locallyVerified; }
};

// ---- Room Encryption Trust Level ----
// Original Kotlin: RoomEncryptionTrustLevel.kt (42L)

enum class RoomEncryptionTrustLevel {
    Default,
    Warning,
    Trusted,
    E2EWithUnsupportedAlgorithm
};

inline std::string roomEncryptionTrustLevelToString(RoomEncryptionTrustLevel t) {
    switch (t) {
        case RoomEncryptionTrustLevel::Default:                    return "Default";
        case RoomEncryptionTrustLevel::Warning:                    return "Warning";
        case RoomEncryptionTrustLevel::Trusted:                    return "Trusted";
        case RoomEncryptionTrustLevel::E2EWithUnsupportedAlgorithm: return "E2EWithUnsupportedAlgorithm";
    }
    return "Default";
}

// ---- Cross-Signing Key Struct ----
// Original Kotlin: CryptoCrossSigningKey.kt (110L)

struct CrossSigningKey {
    std::string userId;
    std::vector<std::string> usages;
    std::map<std::string, std::string> keys;
    std::map<std::string, std::map<std::string, std::string>> signatures;
    bool isTrusted = false;          // Original Kotlin: current trust state
    bool wasTrustedOnce = false;     // Original Kotlin: at some point trusted

    bool isMasterKey() const {
        for (const auto& u : usages) {
            if (u == "master") return true;
        }
        return false;
    }

    bool isSelfSigningKey() const {
        for (const auto& u : usages) {
            if (u == "self_signing") return true;
        }
        return false;
    }

    bool isUserSigningKey() const {
        for (const auto& u : usages) {
            if (u == "user_signing") return true;
        }
        return false;
    }

    std::string unpaddedBase64PublicKey() const {
        if (keys.empty()) return "";
        return keys.begin()->second;
    }
};

// ---- Cross-Signing Info ----
// Original Kotlin: MXCrossSigningInfo.kt (36L)

struct CrossSigningInfo {
    std::string userId;
    CrossSigningKey masterKey;
    CrossSigningKey selfSigningKey;
    CrossSigningKey userSigningKey;
    bool hasMaster = false;
    bool hasSelfSigning = false;
    bool hasUserSigning = false;
    bool wasTrustedOnce = false;

    bool isTrusted() const {
        return hasMaster && hasSelfSigning && hasUserSigning && wasTrustedOnce;
    }

    bool isSetup() const {
        return hasMaster && hasSelfSigning && hasUserSigning;
    }
};

// ---- Cross-Signing Bootstrap Info ----

struct CrossSigningBootstrapInfo {
    bool needsBootstrap = true;
    bool isBootstrapped = false;
    CrossSigningState state = CrossSigningState::NOT_BOOTSTRAPPED;
};

// ---- Cross-Signing Status (legacy) ----

struct CrossSigningStatus {
    bool masterKeyExists = false;
    bool selfSigningKeyExists = false;
    bool userSigningKeyExists = false;
    bool isSetup = false;
    bool isVerified = false;
    bool needsBootstrap = false;
    std::string masterKeyId;
};

// ---- Cross-Signing Reset ----

struct CrossSigningReset {
    bool canReset = false;
    bool needsAuth = false;
    std::string warningMessage;
};

// ---- Device Verification Info ----
// Original Kotlin: CryptoDeviceInfo.kt (78L)

struct DeviceVerificationInfo {
    std::string deviceId;
    std::string userId;
    bool isVerified = false;
    bool isCrossSigningVerified = false;
    DeviceTrustLevel trustLevel;

    bool isUnknown() const {
        return !isVerified && !isCrossSigningVerified;
    }
};

// ---- Device Trust Summary ----
// Aggregated trust across all devices for a given user

struct DeviceTrustSummary {
    int totalDevices = 0;
    int verifiedDevices = 0;
    int crossSigningVerifiedDevices = 0;
    int unknownDevices = 0;
    RoomEncryptionTrustLevel overallTrustLevel = RoomEncryptionTrustLevel::Default;
    bool allDevicesVerified = false;
    bool anyDeviceUnverified = false;
};

// ---- JSON Parsers ----

CrossSigningKey parseCrossSigningKey(const std::string& json);
CrossSigningInfo parseCrossSigningInfo(const std::string& json);

// ---- JSON Builders ----

std::string buildCrossSigningKey(const CrossSigningKey& key);
std::string buildCrossSigningInfo(const CrossSigningInfo& info);

// ---- Cross-Signing Computation ----

CrossSigningState computeCrossSigningState(const CrossSigningInfo& info, bool hasPrivateKeys);
CrossSigningBootstrapInfo computeBootstrapInfo(const CrossSigningInfo& info, bool hasPrivateKeys);
bool isUserTrusted(const CrossSigningInfo& info);
bool wasUserOnceTrusted(const CrossSigningInfo& info);

DeviceTrustSummary computeDeviceTrustSummary(
    const std::vector<DeviceVerificationInfo>& devices);

RoomEncryptionTrustLevel computeShieldForGroup(
    const std::vector<DeviceVerificationInfo>& devices);

// Original Kotlin: aggregate a user-friendly trust summary string for a device list.
// Returns a formatted string like "3 of 5 devices verified".
std::string deviceVerificationSummary(
    const std::vector<DeviceVerificationInfo>& devices);

// ---- Legacy Functions (from cross_signing.hpp) ----

CrossSigningStatus parseCrossSigningStatus(const std::string& accountDataJson, const std::string& userId);
bool needsCrossSigningSetup(const CrossSigningStatus& status);
CrossSigningReset checkResetEligibility(const CrossSigningStatus& status, bool hasPasswordAuth);
std::string formatCrossSigningStatus(const CrossSigningStatus& status);
std::string getCrossSigningStorageKey(CrossSigningKeyType keyType, const std::string& userId);
std::string parseCrossSigningKeyId(const std::string& keyContentJson);
bool isKeySignedByMaster(const std::string& keyJson, const std::string& masterKeyId);
std::string buildBootstrapBody(const std::string& masterKey, const std::string& selfSigningKey,
    const std::string& userSigningKey, const std::string& masterKeySignature,
    const std::string& selfSigningSignature, const std::string& userSigningSignature);

} // namespace progressive
