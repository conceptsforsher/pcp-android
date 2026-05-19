#include "progressive/cross_signing.hpp"
#include "progressive/json_parser.hpp"
#include <sstream>

namespace progressive {

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

} // namespace progressive
