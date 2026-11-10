#include "pcp/cross_signing.hpp"
#include "pcp/json_parser.hpp"
#include <sstream>

namespace pcp {

CrossSigningStatus parseCrossSigningStatus(const std::string& accountDataJson, const std::string& userId) {
    // Original Kotlin (CrossSigningService.kt:89-112):
    //   val crossSigningInfo = cryptoStore.getCrossSigningInfo(myUserId)
    //   return CrossSigningStatus(
    //     masterKey = crossSigningInfo?.masterKey?.publicKey != null,
    //     selfSigningKey = crossSigningInfo?.selfSigningKey?.publicKey != null,
    //     userSigningKey = crossSigningInfo?.userSigningKey?.publicKey != null
    //   )
    CrossSigningStatus status;

    // Check for master key in account data
    auto masterKey = parseJsonStringValue(accountDataJson, "master_key");
    if (!masterKey.empty()) {
        status.masterKeyExists = true;
        status.masterKeyId = parseJsonStringValue("{" + masterKey + "}", "public_key");
    }

    auto selfKey = parseJsonStringValue(accountDataJson, "self_signing_key");
    if (!selfKey.empty()) status.selfSigningKeyExists = true;

    auto userKey = parseJsonStringValue(accountDataJson, "user_signing_key");
    if (!userKey.empty()) status.userSigningKeyExists = true;

    // Original Kotlin: crossSigningInfo.isTrusted()
    status.isVerified = status.masterKeyExists &&
        accountDataJson.find("\"trusted\": true") != std::string::npos;

    status.isSetup = status.masterKeyExists && status.selfSigningKeyExists && status.userSigningKeyExists;
    status.needsBootstrap = !status.isSetup;

    return status;
}

bool needsCrossSigningSetup(const CrossSigningStatus& status) {
    // Original Kotlin (SharedSecureStorageViewModel.kt:156):
    //   return !crossSigningService.isCrossSigningSetup()
    return status.needsBootstrap;
}

CrossSigningReset checkResetEligibility(const CrossSigningStatus& status, bool hasPasswordAuth) {
    // Original Kotlin (CrossSigningService.kt:234-250):
    //   canReset = isCrossSigningSetup() && (hasPassword || hasSecurityKey)
    CrossSigningReset reset;
    reset.canReset = status.isSetup && hasPasswordAuth;
    reset.needsAuth = !hasPasswordAuth;

    if (reset.canReset) {
        reset.warningMessage = "Resetting cross-signing will invalidate all device verifications. "
                               "You will need to re-verify every device you trust.";
    }

    return reset;
}

std::string formatCrossSigningStatus(const CrossSigningStatus& status) {
    std::ostringstream out;
    out << "Cross-Signing Status\n";
    out << "====================\n";
    out << "Master key: " << (status.masterKeyExists ? "Present" : "Missing") << "\n";
    out << "Self-signing key: " << (status.selfSigningKeyExists ? "Present" : "Missing") << "\n";
    out << "User-signing key: " << (status.userSigningKeyExists ? "Present" : "Missing") << "\n";
    out << "Setup complete: " << (status.isSetup ? "Yes" : "No") << "\n";
    out << "Verified: " << (status.isVerified ? "Yes" : "No") << "\n";
    if (status.masterKeyId.size() > 20) {
        out << "Master key: " << status.masterKeyId.substr(0, 20) << "...";
    }
    return out.str();
}

std::string getCrossSigningStorageKey(CrossSigningKey keyType, const std::string& userId) {
    // Original Kotlin (CrossSigningService.kt:45):
    //   private fun storageKey(type: CrossSigningKey): String = "mx_secret_${type.name.lowercase()}_$userId"
    std::string typeStr;
    switch (keyType) {
        case CrossSigningKey::Master:      typeStr = "master"; break;
        case CrossSigningKey::SelfSigning: typeStr = "self_signing"; break;
        case CrossSigningKey::UserSigning: typeStr = "user_signing"; break;
        default:                           typeStr = "unknown";
    }
    return "mx_secret_" + typeStr + "_" + userId;
}

std::string parseCrossSigningKeyId(const std::string& keyContentJson) {
    return parseJsonStringValue(keyContentJson, "public_key");
}

bool isKeySignedByMaster(const std::string& keyJson, const std::string& masterKeyId) {
    // Check if signatures object contains master key's signature
    auto signatures = parseJsonStringValue(keyJson, "signatures");
    if (signatures.empty()) return false;
    return signatures.find(masterKeyId) != std::string::npos;
}

std::string buildBootstrapBody(const std::string& masterKey, const std::string& selfSigningKey,
    const std::string& userSigningKey, const std::string& masterKeySignature,
    const std::string& selfSigningSignature, const std::string& userSigningSignature) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << "{";
    json << R"("master_key": )" << masterKey << ",";
    json << R"("self_signing_key": )" << selfSigningKey << ",";
    json << R"("user_signing_key": )" << userSigningKey;
    json << "}";
    return json.str();
}

// ============================================================================
// CrossSigningKey — parse/build
// ============================================================================

// Original Kotlin (CryptoCrossSigningKey.kt: JSON deserialization)
CrossSigningKey parseCrossSigningKey(const std::string& json) {
    CrossSigningKey key;

    auto extractStr = [&](const std::string& field) -> std::string {
        auto search = "\"" + field + "\":\"";
        auto pos = json.find(search);
        if (pos == std::string::npos) {
            search = "\"" + field + "\": \"";
            pos = json.find(search);
        }
        if (pos == std::string::npos) return "";
        pos += search.size();
        auto end = json.find('"', pos);
        if (end == std::string::npos) return "";
        return json.substr(pos, end - pos);
    };

    key.userId = extractStr("user_id");

    // Parse usages array
    auto uPos = json.find("\"usages\"");
    if (uPos != std::string::npos) {
        auto bracket = json.find('[', uPos);
        if (bracket != std::string::npos) {
            size_t pos = bracket + 1;
            while (pos < json.size() && json[pos] != ']') {
                if (json[pos] == '"') {
                    size_t end = json.find('"', pos + 1);
                    if (end != std::string::npos) {
                        key.usages.push_back(json.substr(pos + 1, end - pos - 1));
                        pos = end + 1;
                        continue;
                    }
                }
                pos++;
            }
        }
    }

    // Parse keys map
    auto kPos = json.find("\"keys\"");
    if (kPos != std::string::npos) {
        auto brace = json.find('{', kPos);
        if (brace != std::string::npos) {
            size_t pos = brace + 1;
            while (pos < json.size() && json[pos] != '}') {
                if (json[pos] == '"') {
                    size_t keyEnd = json.find('"', pos + 1);
                    if (keyEnd == std::string::npos) break;
                    std::string k = json.substr(pos + 1, keyEnd - pos - 1);
                    auto colon = json.find(':', keyEnd);
                    if (colon == std::string::npos) break;
                    auto vq = json.find('"', colon);
                    if (vq == std::string::npos) break;
                    auto ve = json.find('"', vq + 1);
                    if (ve == std::string::npos) break;
                    key.keys[k] = json.substr(vq + 1, ve - vq - 1);
                    pos = ve + 1;
                    continue;
                }
                pos++;
            }
        }
    }

    // Parse signatures map
    auto sPos = json.find("\"signatures\"");
    if (sPos != std::string::npos) {
        auto brace = json.find('{', sPos);
        if (brace != std::string::npos) {
            int depth = 0;
            size_t pos = brace + 1;
            std::string currentUserId;
            while (pos < json.size()) {
                if (json[pos] == '"') {
                    size_t keyEnd = json.find('"', pos + 1);
                    if (keyEnd == std::string::npos) break;
                    std::string name = json.substr(pos + 1, keyEnd - pos - 1);
                    pos = keyEnd + 1;

                    // Check if next token is : followed by {
                    auto colon = json.find(':', pos);
                    if (colon == std::string::npos) break;
                    auto openBrace = json.find('{', colon);
                    auto closeBrace = json.find('}', openBrace);

                    if (openBrace != std::string::npos && closeBrace != std::string::npos
                        && openBrace >= colon && openBrace < colon + 20) {
                        // Nested user → signature map
                        currentUserId = name;
                        pos = openBrace + 1;
                    } else {
                        // Outer level: user_id → { key_id: signature }
                        currentUserId = name;
                        pos = colon + 1;
                        if (json[pos] == '{') pos++;
                    }
                    continue;
                }

                if (json[pos] == '}' || json[pos] == ',') {
                    pos++;
                    continue;
                }

                // Parse key: value inside a signatures sub-map
                if (!currentUserId.empty() && json[pos] == ':' ) {
                    pos++;
                    // Find the value
                    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
                    // The value was parsed by the outer loop
                }

                pos++;
                if (pos >= json.size() || (depth == 0 && json[pos] == '}')) break;
            }
        }
    }

    key.isTrusted      = json.find("\"trusted\": true") != std::string::npos
                      || json.find("\"trusted\":true") != std::string::npos;
    key.wasTrustedOnce = json.find("\"was_trusted_once\": true") != std::string::npos
                      || json.find("\"was_trusted_once\":true") != std::string::npos
                      || key.isTrusted;

    return key;
}

// Original Kotlin (CryptoCrossSigningKey.kt: JSON serialization)
std::string buildCrossSigningKey(const CrossSigningKey& key) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) {
            if (c == '"') out += "\\\"";
            else if (c == '\\') out += "\\\\";
            else out += c;
        }
        return out;
    };

    std::ostringstream json;
    json << "{";
    json << R"("user_id": ")" << esc(key.userId) << R"(")";

    if (!key.usages.empty()) {
        json << R"(,"usages": [)";
        for (size_t i = 0; i < key.usages.size(); ++i) {
            if (i > 0) json << ",";
            json << R"(")" << esc(key.usages[i]) << R"(")";
        }
        json << "]";
    }

    if (!key.keys.empty()) {
        json << R"(,"keys": {)";
        bool first = true;
        for (const auto& [k, v] : key.keys) {
            if (!first) json << ",";
            first = false;
            json << R"(")" << esc(k) << R"(": ")" << esc(v) << R"(")";
        }
        json << "}";
    }

    if (!key.signatures.empty()) {
        json << R"(,"signatures": {)";
        bool firstUser = true;
        for (const auto& [userId, sigs] : key.signatures) {
            if (!firstUser) json << ",";
            firstUser = false;
            json << R"(")" << esc(userId) << R"(": {)";
            bool firstKey = true;
            for (const auto& [kid, sig] : sigs) {
                if (!firstKey) json << ",";
                firstKey = false;
                json << R"(")" << esc(kid) << R"(": ")" << esc(sig) << R"(")";
            }
            json << "}";
        }
        json << "}";
    }

    json << R"(,"trusted": )" << (key.isTrusted ? "true" : "false");
    json << R"(,"was_trusted_once": )" << (key.wasTrustedOnce ? "true" : "false");
    json << "}";
    return json.str();
}

// ============================================================================
// CrossSigningInfo — parse/build
// ============================================================================

// Original Kotlin (MXCrossSigningInfo.kt: JSON deserialization)
CrossSigningInfo parseCrossSigningInfo(const std::string& json) {
    CrossSigningInfo info;

    auto extractStr = [&](const std::string& field) -> std::string {
        auto search = "\"" + field + "\":\"";
        auto pos = json.find(search);
        if (pos == std::string::npos) {
            search = "\"" + field + "\": \"";
            pos = json.find(search);
        }
        if (pos == std::string::npos) return "";
        pos += search.size();
        auto end = json.find('"', pos);
        if (end == std::string::npos) return "";
        return json.substr(pos, end - pos);
    };

    info.userId = extractStr("user_id");

    // Parse nested master key
    auto mkPos = json.find("\"master_key\"");
    if (mkPos != std::string::npos) {
        auto brace = json.find('{', mkPos);
        if (brace != std::string::npos) {
            int depth = 1;
            size_t end = brace + 1;
            while (end < json.size() && depth > 0) {
                if (json[end] == '{') depth++;
                else if (json[end] == '}') depth--;
                end++;
            }
            info.masterKey = parseCrossSigningKey(json.substr(brace, end - brace));
            info.hasMaster = true;
        }
    }

    // Parse nested self_signing key
    auto ssPos = json.find("\"self_signing_key\"");
    if (ssPos != std::string::npos) {
        auto brace = json.find('{', ssPos);
        if (brace != std::string::npos) {
            int depth = 1;
            size_t end = brace + 1;
            while (end < json.size() && depth > 0) {
                if (json[end] == '{') depth++;
                else if (json[end] == '}') depth--;
                end++;
            }
            info.selfSigningKey = parseCrossSigningKey(json.substr(brace, end - brace));
            info.hasSelfSigning = true;
        }
    }

    // Parse nested user_signing key
    auto usPos = json.find("\"user_signing_key\"");
    if (usPos != std::string::npos) {
        auto brace = json.find('{', usPos);
        if (brace != std::string::npos) {
            int depth = 1;
            size_t end = brace + 1;
            while (end < json.size() && depth > 0) {
                if (json[end] == '{') depth++;
                else if (json[end] == '}') depth--;
                end++;
            }
            info.userSigningKey = parseCrossSigningKey(json.substr(brace, end - brace));
            info.hasUserSigning = true;
        }
    }

    info.wasTrustedOnce = json.find("\"was_trusted_once\": true") != std::string::npos
                       || json.find("\"was_trusted_once\":true") != std::string::npos
                       || (info.hasMaster && info.masterKey.wasTrustedOnce);

    return info;
}

// Original Kotlin: serialize CrossSigningInfo to JSON
std::string buildCrossSigningInfo(const CrossSigningInfo& info) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) {
            if (c == '"') out += "\\\"";
            else if (c == '\\') out += "\\\\";
            else out += c;
        }
        return out;
    };

    std::ostringstream json;
    json << "{";
    json << R"("user_id": ")" << esc(info.userId) << R"(")";
    if (info.hasMaster) {
        json << R"(,"master_key": )" << buildCrossSigningKey(info.masterKey);
    }
    if (info.hasSelfSigning) {
        json << R"(,"self_signing_key": )" << buildCrossSigningKey(info.selfSigningKey);
    }
    if (info.hasUserSigning) {
        json << R"(,"user_signing_key": )" << buildCrossSigningKey(info.userSigningKey);
    }
    json << R"(,"was_trusted_once": )" << (info.wasTrustedOnce ? "true" : "false");
    json << "}";
    return json.str();
}

// ============================================================================
// State computation
// ============================================================================

// Original Kotlin (CrossSigningService.kt): determine state based on key presence + private keys
CrossSigningState computeCrossSigningState(const CrossSigningInfo& info, bool hasPrivateKeys) {
    if (!info.isSetup()) return CrossSigningState::NOT_BOOTSTRAPPED;
    if (!hasPrivateKeys) return CrossSigningState::CROSS_SIGNING_EXISTS;
    if (!info.isTrusted()) return CrossSigningState::CAN_CROSS_SIGN;
    return CrossSigningState::TRUSTED;
}

CrossSigningBootstrapInfo computeBootstrapInfo(const CrossSigningInfo& info, bool hasPrivateKeys) {
    CrossSigningBootstrapInfo result;
    result.state = computeCrossSigningState(info, hasPrivateKeys);
    result.isBootstrapped = info.isSetup() && hasPrivateKeys && info.isTrusted();
    result.needsBootstrap = !result.isBootstrapped;
    return result;
}

bool isUserTrusted(const CrossSigningInfo& info) {
    return info.isTrusted();
}

bool wasUserOnceTrusted(const CrossSigningInfo& info) {
    return info.wasTrustedOnce || info.isTrusted();
}

// ============================================================================
// Device trust computation
// ============================================================================

// Original Kotlin (DeviceTrustLevel.kt + RoomEncryptionTrustLevel.kt):
//   Compute trust summary across all devices for a user/room.
DeviceTrustSummary computeDeviceTrustSummary(
    const std::vector<DeviceVerificationInfo>& devices) {
    DeviceTrustSummary summary;
    summary.totalDevices = static_cast<int>(devices.size());

    for (const auto& dev : devices) {
        if (dev.isCrossSigningVerified) {
            summary.crossSigningVerifiedDevices++;
        }
        if (dev.isVerified) {
            summary.verifiedDevices++;
        }
        if (dev.isUnknown()) {
            summary.unknownDevices++;
        }
    }

    summary.allDevicesVerified = (summary.verifiedDevices == summary.totalDevices
                               && summary.totalDevices > 0);
    summary.anyDeviceUnverified = (summary.unknownDevices > 0);

    // Determine overall trust level
    if (summary.allDevicesVerified) {
        summary.overallTrustLevel = RoomEncryptionTrustLevel::Trusted;
    } else if (summary.unknownDevices > 0) {
        summary.overallTrustLevel = RoomEncryptionTrustLevel::Warning;
    } else {
        summary.overallTrustLevel = RoomEncryptionTrustLevel::Default;
    }

    return summary;
}

RoomEncryptionTrustLevel computeShieldForGroup(
    const std::vector<DeviceVerificationInfo>& devices) {
    if (devices.empty()) return RoomEncryptionTrustLevel::Default;

    bool allCrossSigning = true;
    bool hasUnknown = false;

    for (const auto& dev : devices) {
        if (!dev.isCrossSigningVerified) allCrossSigning = false;
        if (dev.isUnknown()) hasUnknown = true;
    }

    if (allCrossSigning && !devices.empty()) return RoomEncryptionTrustLevel::Trusted;
    if (hasUnknown) return RoomEncryptionTrustLevel::Warning;
    return RoomEncryptionTrustLevel::Default;
}

// Original Kotlin: summarize trust across devices for user-facing display
std::string deviceVerificationSummary(
    const std::vector<DeviceVerificationInfo>& devices) {
    if (devices.empty()) return "No devices";

    auto summary = computeDeviceTrustSummary(devices);

    if (summary.allDevicesVerified) {
        return std::to_string(summary.totalDevices) + " of " +
               std::to_string(summary.totalDevices) + " devices verified";
    }

    return std::to_string(summary.crossSigningVerifiedDevices) + " of " +
           std::to_string(summary.totalDevices) + " devices verified";
}

} // namespace pcp
