#include "progressive/cross_signing_manager.hpp"
#include <sstream>
#include <algorithm>

namespace progressive {

// ====== Enum ======

const char* keyUsageToString(CSM_KeyUsage usage) {
    switch (usage) {
        case CSM_KeyUsage::MASTER: return "master";
        case CSM_KeyUsage::SELF_SIGNING: return "self_signing";
        case CSM_KeyUsage::USER_SIGNING: return "user_signing";
    }
    return "master";
}

CSM_KeyUsage keyUsageFromString(const std::string& s) {
    if (s == "master") return CSM_KeyUsage::MASTER;
    if (s == "self_signing") return CSM_KeyUsage::SELF_SIGNING;
    if (s == "user_signing") return CSM_KeyUsage::USER_SIGNING;
    return CSM_KeyUsage::MASTER;
}

// ====== CSM_CrossSigningKey ======

bool CSM_CrossSigningKey::isMasterKey() const {
    return std::find(usages.begin(), usages.end(), CSM_KeyUsage::MASTER) != usages.end();
}
bool CSM_CrossSigningKey::isSelfSigningKey() const {
    return std::find(usages.begin(), usages.end(), CSM_KeyUsage::SELF_SIGNING) != usages.end();
}
bool CSM_CrossSigningKey::isUserKey() const {
    return std::find(usages.begin(), usages.end(), CSM_KeyUsage::USER_SIGNING) != usages.end();
}

std::string CSM_CrossSigningKey::getPublicKey() const {
    if (keys.empty()) return "";
    return keys.begin()->second;
}

void CSM_CrossSigningKey::addSignature(const std::string& signerUserId, const std::string& signedKey,
                                    const std::string& signature) {
    signatures[signerUserId]["ed25519:" + signedKey] = signature;
}

CSM_CrossSigningKey CSM_CrossSigningKey::Builder::build() const {
    CSM_CrossSigningKey key;
    key.userId = userId;
    key.usages = {usage};
    key.keys["ed25519:" + publicKey] = publicKey;
    for (const auto& s : sigs) {
        key.signatures[s.userId]["ed25519:" + s.keySigned] = s.signature;
    }
    key.valid = !publicKey.empty();
    return key;
}

// ====== CSM_CrossSigningInfo ======

bool CSM_CrossSigningInfo::isTrusted() const {
    auto* msk = masterKey();
    auto* ssk = selfSigningKey();
    return msk && ssk && msk->isVerified() && ssk->isVerified();
}

const CSM_CrossSigningKey* CSM_CrossSigningInfo::masterKey() const {
    for (const auto& k : keys) if (k.isMasterKey()) return &k;
    return nullptr;
}
const CSM_CrossSigningKey* CSM_CrossSigningInfo::userKey() const {
    for (const auto& k : keys) if (k.isUserKey()) return &k;
    return nullptr;
}
const CSM_CrossSigningKey* CSM_CrossSigningInfo::selfSigningKey() const {
    for (const auto& k : keys) if (k.isSelfSigningKey()) return &k;
    return nullptr;
}

// ====== CrossSigningManager ======

CrossSigningManager::CrossSigningManager() {}

bool CrossSigningManager::isInitialized() const { return myKeys_.valid && myKeys_.keys.size() >= 3; }
bool CrossSigningManager::isVerified() const { return isInitialized() && myKeys_.isTrusted(); }
bool CrossSigningManager::canCrossSign() const { return privateKeys_.allPrivateKeysKnown(); }
bool CrossSigningManager::allPrivateKeysKnown() const { return privateKeys_.allPrivateKeysKnown(); }

void CrossSigningManager::setMyKeys(const CSM_CrossSigningInfo& info) { myKeys_ = info; }
void CrossSigningManager::setUserKeys(const std::string& userId, const CSM_CrossSigningInfo& info) { userKeys_[userId] = info; }
CSM_CrossSigningInfo CrossSigningManager::getMyKeys() const { return myKeys_; }
CSM_CrossSigningInfo CrossSigningManager::getUserKeys(const std::string& userId) const {
    auto it = userKeys_.find(userId);
    return (it != userKeys_.end()) ? it->second : CSM_CrossSigningInfo{};
}

// ====== Private Key Import ======

UserTrustResult CrossSigningManager::importPrivateKeys(const std::string& masterKeyPrivate,
                                                        const std::string& userSigningKeyPrivate,
                                                        const std::string& selfSigningKeyPrivate) {
    UserTrustResult result;

    if (!masterKeyPrivate.empty()) {
        privateKeys_.masterKeyPrivate = masterKeyPrivate;
        privateKeys_.hasMaster = true;
    }
    if (!userSigningKeyPrivate.empty()) {
        privateKeys_.userSigningKeyPrivate = userSigningKeyPrivate;
        privateKeys_.hasUserSigning = true;
    }
    if (!selfSigningKeyPrivate.empty()) {
        privateKeys_.selfSigningKeyPrivate = selfSigningKeyPrivate;
        privateKeys_.hasSelfSigning = true;
    }

    result.crossSigningVerified = canCrossSign();
    result.locallyVerified = canCrossSign();
    result.isTrusted = result.crossSigningVerified || result.locallyVerified;

    return result;
}

bool CrossSigningManager::importPrivateKey(CSM_KeyUsage usage, const std::string& privateKey) {
    switch (usage) {
        case CSM_KeyUsage::MASTER: privateKeys_.masterKeyPrivate = privateKey; privateKeys_.hasMaster = true; break;
        case CSM_KeyUsage::SELF_SIGNING: privateKeys_.selfSigningKeyPrivate = privateKey; privateKeys_.hasSelfSigning = true; break;
        case CSM_KeyUsage::USER_SIGNING: privateKeys_.userSigningKeyPrivate = privateKey; privateKeys_.hasUserSigning = true; break;
    }
    return true;
}

PrivateKeysInfo CrossSigningManager::getPrivateKeys() const { return privateKeys_; }

// ====== Trust Chain ======

bool CrossSigningManager::verifyKeySignatures(const CSM_CrossSigningKey& key) const {
    // Check that the MSK has signed this key
    auto* msk = myKeys_.masterKey();
    if (!msk) return false;

    auto it = key.signatures.find(myKeys_.userId);
    if (it == key.signatures.end()) return false;

    return !it->second.empty(); // Has at least one signature from us
}

UserTrustResult CrossSigningManager::checkSelfTrust() const {
    UserTrustResult result;
    if (!isInitialized()) {
        result.error = "Cross-signing not initialized";
        return result;
    }

    result.crossSigningVerified = myKeys_.isTrusted();
    result.locallyVerified = myKeys_.wasTrustedOnce;
    result.isTrusted = result.crossSigningVerified || result.locallyVerified;

    return result;
}

UserTrustResult CrossSigningManager::checkUserTrust(const std::string& otherUserId) const {
    UserTrustResult result;
    if (!isInitialized()) {
        result.error = "Self not initialized";
        return result;
    }

    auto it = userKeys_.find(otherUserId);
    if (it == userKeys_.end()) {
        result.error = "No cross-signing keys for user: " + otherUserId;
        return result;
    }

    const auto& otherInfo = it->second;
    auto* otherMsk = otherInfo.masterKey();
    if (!otherMsk) {
        result.error = "No master key for user";
        return result;
    }

    // Check that our USK has signed their MSK
    if (verifyKeySignatures(*otherMsk)) {
        result.crossSigningVerified = true;
        result.isTrusted = true;
    }

    return result;
}

DeviceTrustResult CrossSigningManager::checkDeviceTrust(const std::string& userId, const std::string& deviceId,
                                                         bool locallyTrusted) const {
    DeviceTrustResult result;
    result.deviceId = deviceId;
    result.userId = userId;
    result.locallyVerified = locallyTrusted;

    // Check cross-signing trust chain
    if (userId == myKeys_.userId) {
        // Own device — check if signed by our SSK
        auto* ssk = myKeys_.selfSigningKey();
        if (ssk && ssk->isVerified()) {
            result.crossSigningVerified = true;
            result.isTrusted = true;
        }
    } else {
        // Other user's device — check user trust
        auto userTrust = checkUserTrust(userId);
        result.crossSigningVerified = userTrust.crossSigningVerified;
        result.isTrusted = userTrust.isTrusted || locallyTrusted;
    }

    return result;
}

// ====== Trust Operations ======

void CrossSigningManager::markMyMasterKeyAsTrusted() {
    for (auto& key : myKeys_.keys) {
        if (key.isMasterKey()) {
            key.crossSigningVerified = true;
            key.locallyVerified = true;
        }
    }
    myKeys_.wasTrustedOnce = true;
}

void CrossSigningManager::trustUser(const std::string& otherUserId) {
    auto it = userKeys_.find(otherUserId);
    if (it == userKeys_.end()) return;

    for (auto& key : it->second.keys) {
        if (key.isMasterKey()) {
            key.crossSigningVerified = true;
        }
    }
}

void CrossSigningManager::trustDevice(const std::string& deviceId) {
    (void)deviceId;
    // Sign the device key with our SSK (simplified)
    if (!canCrossSign()) return;

    // In a real implementation: add signature to device
}

// ====== Key Building ======

CSM_CrossSigningKey CrossSigningManager::buildMasterKey(const std::string& userId, const std::string& publicKey) {
    return CSM_CrossSigningKey::Builder()
        .key(publicKey)
        .userId(userId)
        .usage(CSM_KeyUsage::MASTER)
        .build();
}

CSM_CrossSigningKey CrossSigningManager::buildSelfSigningKey(const std::string& userId, const std::string& publicKey) {
    return CSM_CrossSigningKey::Builder()
        .key(publicKey)
        .userId(userId)
        .usage(CSM_KeyUsage::SELF_SIGNING)
        .build();
}

CSM_CrossSigningKey CrossSigningManager::buildUserSigningKey(const std::string& userId, const std::string& publicKey) {
    return CSM_CrossSigningKey::Builder()
        .key(publicKey)
        .userId(userId)
        .usage(CSM_KeyUsage::USER_SIGNING)
        .build();
}

CSM_CrossSigningInfo CrossSigningManager::buildCrossSigningInfo(const std::string& userId,
                                                              const CSM_CrossSigningKey& msk,
                                                              const CSM_CrossSigningKey& usk,
                                                              const CSM_CrossSigningKey& ssk) {
    CSM_CrossSigningInfo info;
    info.userId = userId;
    info.keys = {msk, usk, ssk};
    info.valid = msk.valid && usk.valid && ssk.valid;
    return info;
}

// ====== Serialization ======

std::string CrossSigningManager::crossSigningInfoToJson(const CSM_CrossSigningInfo& info) const {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    std::ostringstream os;
    os << R"({"user_id":")" << esc(info.userId)
       << R"(","is_trusted":)" << (info.isTrusted() ? "true" : "false")
       << R"(,"was_trusted_once":)" << (info.wasTrustedOnce ? "true" : "false")
       << R"(,"key_count":)" << static_cast<int>(info.keys.size())
       << R"(,"has_msk":)" << (info.masterKey() != nullptr ? "true" : "false")
       << R"(,"has_usk":)" << (info.userKey() != nullptr ? "true" : "false")
       << R"(,"has_ssk":)" << (info.selfSigningKey() != nullptr ? "true" : "false")
       << "}";
    return os.str();
}

std::string CrossSigningManager::keyToJson(const CSM_CrossSigningKey& key) const {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    std::ostringstream os;
    os << R"({"user_id":")" << esc(key.userId)
       << R"(","public_key":")" << esc(key.getPublicKey())
       << R"(","is_msk":)" << (key.isMasterKey() ? "true" : "false")
       << R"(,"is_ssk":)" << (key.isSelfSigningKey() ? "true" : "false")
       << R"(,"is_usk":)" << (key.isUserKey() ? "true" : "false")
       << R"(,"verified":)" << (key.isVerified() ? "true" : "false")
       << R"(,"signature_count":)" << static_cast<int>(key.signatures.size())
       << "}";
    return os.str();
}

std::string CrossSigningManager::trustResultToJson(const UserTrustResult& result) const {
    std::ostringstream os;
    os << R"({"is_trusted":)" << (result.isTrusted ? "true" : "false")
       << R"(,"cross_signing_verified":)" << (result.crossSigningVerified ? "true" : "false")
       << R"(,"locally_verified":)" << (result.locallyVerified ? "true" : "false");
    if (!result.error.empty()) os << R"(,"error":")" << result.error << R"(")";
    os << "}";
    return os.str();
}

std::string CrossSigningManager::deviceTrustToJson(const DeviceTrustResult& result) const {
    std::ostringstream os;
    os << R"({"device_id":")" << result.deviceId
       << R"(","user_id":")" << result.userId
       << R"(,"is_trusted":)" << (result.isTrusted ? "true" : "false")
       << R"(,"cross_signing_verified":)" << (result.crossSigningVerified ? "true" : "false")
       << R"(,"locally_verified":)" << (result.locallyVerified ? "true" : "false")
       << "}";
    return os.str();
}

} // namespace progressive
