#include "progressive/e2ee_decoration.hpp"
#include <sstream>
#include <unordered_map>

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

// ================================================================
// Room-Level Shield Computation
//
// Ported from ComputeShieldForGroupUseCase.kt + ShieldSummaryUpdater.kt
// ================================================================

RoomEncryptionTrustLevel computeRoomShield(
    const std::string& myUserId,
    const std::vector<UserIdentityInfo>& userIdentities,
    const std::vector<DeviceTrustInfo>& userDevices)
{
    // Original Kotlin (ComputeShieldForGroupUseCase.kt:28-70):
    //   val myIdentity = olmMachine.getIdentity(myUserId)
    //   val allTrustedUserIds = userIds
    //       .filter { userId ->
    //           val identity = olmMachine.getIdentity(userId)?.toMxCrossSigningInfo()
    //           identity?.isTrusted() == true ||
    //               identity?.wasTrustedOnce == true
    //       }

    // Step 1: Build identity map for quick lookup
    std::unordered_map<std::string, const UserIdentityInfo*> identityMap;
    for (const auto& id : userIdentities) {
        identityMap[id.userId] = &id;
    }

    // Step 2: Filter trusted users
    std::vector<std::string> trustedUserIds;
    for (const auto& id : userIdentities) {
        // Always include users that were previously verified
        if (id.isTrusted || id.wasTrustedOnce) {
            trustedUserIds.push_back(id.userId);
        }
    }

    // Step 3: If no trusted users → Default (black shield)
    // Original Kotlin:
    //   return if (allTrustedUserIds.isEmpty()) RoomEncryptionTrustLevel.Default
    if (trustedUserIds.empty()) {
        return RoomEncryptionTrustLevel::Default;
    }

    // Step 4: Check all devices of trusted users for unverified devices
    // Original Kotlin:
    //   allTrustedUserIds.map { userId -> olmMachine.getUserDevices(userId) }
    //       .flatten()
    //       .let { allDevices ->
    //           if (myIdentity != null) {
    //               allDevices.any { !it.toCryptoDeviceInfo().trustLevel?.crossSigningVerified.orFalse() }
    //           } else {
    //               allDevices.any { !it.toCryptoDeviceInfo().isVerified }
    //           }
    //       }

    bool hasUnverifiedDevice = false;
    bool hasMyIdentity = !myUserId.empty() && identityMap.find(myUserId) != identityMap.end();

    for (const auto& device : userDevices) {
        // Only check devices of trusted users
        bool deviceBelongsToTrustedUser = false;
        for (const auto& uid : trustedUserIds) {
            if (device.userId == uid) { deviceBelongsToTrustedUser = true; break; }
        }
        if (!deviceBelongsToTrustedUser) continue;

        if (hasMyIdentity) {
            // Cross-signing path: check if any device is NOT cross-signing verified
            // Original: allDevices.any { !it.toCryptoDeviceInfo().trustLevel?.crossSigningVerified.orFalse() }
            if (!device.isCrossSigningVerified) {
                hasUnverifiedDevice = true;
                break;
            }
        } else {
            // Legacy path: check if any device is NOT verified
            // Original: allDevices.any { !it.toCryptoDeviceInfo().isVerified }
            if (!device.isVerified) {
                hasUnverifiedDevice = true;
                break;
            }
        }
    }

    // Step 5: Determine final trust level
    // Original Kotlin:
    //   .let { hasWarning ->
    //       if (hasWarning) RoomEncryptionTrustLevel.Warning
    //       else {
    //           if (userIds.size == allTrustedUserIds.size) RoomEncryptionTrustLevel.Trusted
    //           else RoomEncryptionTrustLevel.Default
    //       }
    //   }
    if (hasUnverifiedDevice) {
        return RoomEncryptionTrustLevel::Warning;
    }

    // All users are trusted and all devices are verified → green shield
    if (userIdentities.size() == trustedUserIds.size()) {
        return RoomEncryptionTrustLevel::Trusted;
    }

    return RoomEncryptionTrustLevel::Default;
}

const char* roomShieldToString(RoomEncryptionTrustLevel level) {
    switch (level) {
        case RoomEncryptionTrustLevel::Default:    return "Default (black shield)";
        case RoomEncryptionTrustLevel::Warning:    return "Warning (red shield)";
        case RoomEncryptionTrustLevel::Trusted:    return "Trusted (green shield)";
        case RoomEncryptionTrustLevel::E2EWithUnsupportedAlgorithm:
            return "E2EE with unsupported algorithm";
        default: return "Unknown";
    }
}

const char* roomShieldIconName(RoomEncryptionTrustLevel level) {
    switch (level) {
        case RoomEncryptionTrustLevel::Default:    return "ic_shield_black";
        case RoomEncryptionTrustLevel::Warning:    return "ic_shield_warning";
        case RoomEncryptionTrustLevel::Trusted:    return "ic_shield_trusted";
        case RoomEncryptionTrustLevel::E2EWithUnsupportedAlgorithm:
            return "ic_shield_unknown";
        default: return "";
    }
}

bool shieldLevelChanged(
    RoomEncryptionTrustLevel oldLevel,
    RoomEncryptionTrustLevel newLevel)
{
    return oldLevel != newLevel;
}

} // namespace progressive
