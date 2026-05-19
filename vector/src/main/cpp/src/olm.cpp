#include "progressive/olm.hpp"
#include <olm/olm.h>
#include <sstream>
#include <cstring>
#include <cstdlib>

namespace progressive {

// ==== Utility functions ====

std::string generateRandomBytes(int count) {
    std::string result(count, 0);
    for (int i = 0; i < count; ++i) result[i] = static_cast<char>(rand() % 256);
    return result;
}

std::string olmErrorToString(OlmError error) {
    switch (error) {
        case OlmError::None: return "No error";
        case OlmError::NotEnoughRandom: return "Not enough random data";
        case OlmError::OutputBufferTooSmall: return "Output buffer too small";
        case OlmError::BadMessageVersion: return "Bad message version";
        case OlmError::BadMessageFormat: return "Bad message format";
        case OlmError::BadMessageMac: return "Bad message MAC";
        case OlmError::BadMessageKeyId: return "Bad message key ID";
        case OlmError::InvalidBase64: return "Invalid base64";
        case OlmError::BadAccountKey: return "Bad account key";
        case OlmError::UnknownPickleVersion: return "Unknown pickle version";
        case OlmError::Corruption: return "Data corruption";
        case OlmError::SessionNotFound: return "Session not found";
        default: return "Unknown error";
    }
}

std::string formatPickle(const std::string& type, const std::string& pickle) {
    std::ostringstream out;
    out << type << ":" << pickle;
    return out.str();
}

// ==== OlmAccount ====

OlmAccount::OlmAccount() {
    size_t sz = olm_account_size();
    account_ = new uint8_t[sz];
    memset(account_, 0, sz);
    olm_account(account_);
}

OlmAccount::~OlmAccount() {
    olm_clear_account(static_cast<::OlmAccount*>(account_));
    delete[] static_cast<uint8_t*>(account_);
}

OlmAccountResult OlmAccount::create() {
    OlmAccountResult result;
    auto* acc = static_cast<::OlmAccount*>(account_);
    size_t randLen = olm_create_account_random_length(acc);
    auto random = generateRandomBytes(randLen);
    int rc = olm_create_account(acc, random.data(), random.size());
    if (rc == static_cast<size_t>(-1)) {
        result.error = OlmError::NotEnoughRandom;
        return result;
    }
    result.success = true;
    return result;
}

OlmAccountResult OlmAccount::identityKeys() {
    OlmAccountResult result;
    auto* acc = static_cast<::OlmAccount*>(account_);
    size_t len = olm_account_identity_keys_length(acc);
    std::string out(len, 0);
    size_t written = olm_account_identity_keys(acc, &out[0], len);
    if (written == static_cast<size_t>(-1)) {
        result.error = OlmError::OutputBufferTooSmall;
        return result;
    }
    out.resize(written);
    result.success = true;
    result.data = out;
    return result;
}

OlmAccountResult OlmAccount::generateOneTimeKeys(int count) {
    OlmAccountResult result;
    auto* acc = static_cast<::OlmAccount*>(account_);
    size_t randLen = olm_account_generate_one_time_keys_random_length(acc, count);
    auto random = generateRandomBytes(randLen);
    int rc = olm_account_generate_one_time_keys(acc, count, random.data(), random.size());
    if (rc == static_cast<size_t>(-1)) {
        result.error = OlmError::NotEnoughRandom;
        return result;
    }
    result.success = true;
    return result;
}

OlmAccountResult OlmAccount::sign(const std::string& message) {
    OlmAccountResult result;
    auto* acc = static_cast<::OlmAccount*>(account_);
    size_t sigLen = olm_account_signature_length(acc);
    std::string sig(sigLen, 0);
    size_t written = olm_account_sign(acc, message.data(), message.size(), &sig[0], sigLen);
    if (written == static_cast<size_t>(-1)) {
        result.error = OlmError::OutputBufferTooSmall;
        return result;
    }
    sig.resize(written);
    result.success = true;
    result.data = sig;
    return result;
}

OlmAccountResult OlmAccount::pickle(const std::string& key) {
    OlmAccountResult result;
    auto* acc = static_cast<::OlmAccount*>(account_);
    size_t len = olm_pickle_account_length(acc);
    std::string out(len, 0);
    size_t written = olm_pickle_account(acc, key.data(), key.size(), &out[0], len);
    if (written == static_cast<size_t>(-1)) {
        result.error = OlmError::UnknownPickleVersion;
        return result;
    }
    out.resize(written);
    result.success = true;
    result.data = out;
    return result;
}

OlmAccountResult OlmAccount::unpickle(const std::string& key, const std::string& pickle) {
    OlmAccountResult result;
    auto* acc = static_cast<::OlmAccount*>(account_);
    int rc = olm_unpickle_account(acc, key.data(), key.size(),
        (void*)pickle.data(), pickle.size());
    if (rc == static_cast<size_t>(-1)) {
        result.error = OlmError::BadAccountKey;
        return result;
    }
    result.success = true;
    return result;
}

OlmAccountResult OlmAccount::ed25519Key() {
    auto keys = identityKeys();
    if (!keys.success) return keys;
    auto pos = keys.data.find("\"ed25519\":\"");
    if (pos == std::string::npos) {
        keys.success = false;
        return keys;
    }
    pos += 12;
    auto end = keys.data.find('"', pos);
    keys.data = keys.data.substr(pos, end - pos);
    return keys;
}

OlmAccountResult OlmAccount::curve25519Key() {
    auto keys = identityKeys();
    if (!keys.success) return keys;
    auto pos = keys.data.find("\"curve25519\":\"");
    if (pos == std::string::npos) {
        keys.success = false;
        return keys;
    }
    pos += 15;
    auto end = keys.data.find('"', pos);
    keys.data = keys.data.substr(pos, end - pos);
    return keys;
}

int OlmAccount::maxOneTimeKeys() {
    auto* acc = static_cast<::OlmAccount*>(account_);
    return static_cast<int>(olm_account_max_number_of_one_time_keys(acc));
}

// ==== OlmSession ====

OlmSession::OlmSession() {
    size_t sz = olm_session_size();
    session_ = new uint8_t[sz];
    memset(session_, 0, sz);
    olm_session(session_);
}

OlmSession::~OlmSession() {
    olm_clear_session(static_cast<::OlmSession*>(session_));
    delete[] static_cast<uint8_t*>(session_);
}

OlmSessionResult OlmSession::createOutbound(OlmAccount& account,
    const std::string& theirIdentityKey, const std::string& theirOneTimeKey) {
    OlmSessionResult result;
    auto* sess = static_cast<::OlmSession*>(session_);
    auto* acc = static_cast<::OlmAccount*>(account.account_);
    size_t randLen = olm_create_outbound_session_random_length(sess);
    auto random = generateRandomBytes(randLen);
    size_t rc = olm_create_outbound_session(sess, acc,
        theirIdentityKey.data(), theirIdentityKey.size(),
        theirOneTimeKey.data(), theirOneTimeKey.size(),
        random.data(), random.size());
    if (rc == static_cast<size_t>(-1)) {
        result.error = OlmError::BadMessageFormat;
        return result;
    }
    result.success = true;
    return result;
}

OlmSessionResult OlmSession::createInbound(OlmAccount& account, const std::string& preKeyMessage) {
    OlmSessionResult result;
    auto* sess = static_cast<::OlmSession*>(session_);
    auto* acc = static_cast<::OlmAccount*>(account.account_);
    // Real API: olm_create_inbound_session(session, account, msg, msg_len) — NO random bytes
    size_t rc = olm_create_inbound_session(sess, acc,
        (void*)preKeyMessage.data(), preKeyMessage.size());
    if (rc == static_cast<size_t>(-1)) {
        result.error = OlmError::BadMessageFormat;
        return result;
    }
    result.success = true;
    return result;
}

OlmSessionResult OlmSession::createInboundFrom(OlmAccount& account,
    const std::string& theirIdentityKey, const std::string& encryptedMessage) {
    OlmSessionResult result;
    auto* sess = static_cast<::OlmSession*>(session_);
    auto* acc = static_cast<::OlmAccount*>(account.account_);
    size_t rc = olm_create_inbound_session_from(sess, acc,
        theirIdentityKey.data(), theirIdentityKey.size(),
        (void*)encryptedMessage.data(), encryptedMessage.size());
    if (rc == static_cast<size_t>(-1)) {
        result.error = OlmError::BadMessageFormat;
        return result;
    }
    result.success = true;
    return result;
}

OlmSessionResult OlmSession::encrypt(const std::string& plaintext) {
    OlmSessionResult result;
    auto* sess = static_cast<::OlmSession*>(session_);

    // Real API: olm_encrypt(session, plaintext, pt_len, random, random_len, message, msg_len)
    size_t randLen = olm_encrypt_random_length(sess);
    auto random = generateRandomBytes(randLen);

    size_t msgLen = olm_encrypt_message_length(sess, plaintext.size());
    std::string msg(msgLen, 0);

    size_t written = olm_encrypt(sess, plaintext.data(), plaintext.size(),
        random.data(), random.size(), &msg[0], msgLen);
    if (written == static_cast<size_t>(-1)) {
        result.error = OlmError::OutputBufferTooSmall;
        return result;
    }
    msg.resize(written);
    result.success = true;
    result.data = msg;
    result.messageType = olm_encrypt_message_type(sess);
    return result;
}

OlmSessionResult OlmSession::decrypt(const std::string& encryptedMessage, int messageType) {
    OlmSessionResult result;
    auto* sess = static_cast<::OlmSession*>(session_);
    size_t ptLen = olm_decrypt_max_plaintext_length(sess, messageType,
        (void*)encryptedMessage.data(), encryptedMessage.size());
    std::string pt(ptLen, 0);
    size_t written = olm_decrypt(sess, messageType,
        (void*)encryptedMessage.data(), encryptedMessage.size(), &pt[0], ptLen);
    if (written == static_cast<size_t>(-1)) {
        result.error = OlmError::BadMessageFormat;
        return result;
    }
    pt.resize(written);
    result.success = true;
    result.data = pt;
    return result;
}

OlmSessionResult OlmSession::pickle(const std::string& key) {
    OlmSessionResult result;
    auto* sess = static_cast<::OlmSession*>(session_);
    size_t len = olm_pickle_session_length(sess);
    std::string out(len, 0);
    size_t written = olm_pickle_session(sess, key.data(), key.size(), &out[0], len);
    if (written == static_cast<size_t>(-1)) {
        result.error = OlmError::UnknownPickleVersion;
        return result;
    }
    out.resize(written);
    result.success = true;
    result.data = out;
    return result;
}

OlmSessionResult OlmSession::unpickle(const std::string& key, const std::string& pickle) {
    OlmSessionResult result;
    auto* sess = static_cast<::OlmSession*>(session_);
    int rc = olm_unpickle_session(sess, key.data(), key.size(),
        (void*)pickle.data(), pickle.size());
    if (rc == static_cast<size_t>(-1)) {
        result.error = OlmError::BadAccountKey;
        return result;
    }
    result.success = true;
    return result;
}

bool OlmSession::matchesInbound(const std::string& preKeyMessage) {
    auto* sess = static_cast<::OlmSession*>(session_);
    size_t rc = olm_matches_inbound_session(sess, (void*)preKeyMessage.data(), preKeyMessage.size());
    return rc == 1;
}

// ============================================================================
// OlmDecryptionResult Model — JSON Parser
// ============================================================================

// Original Kotlin (OlmDecryptionResult.kt:27-59):
//   data class OlmDecryptionResult(payload, keysClaimed, senderKey,
//       forwardingCurve25519KeyChain, isSafe, verificationState)
OlmDecryptionResultModel parseOlmDecryptionResult(const std::string& json) {
    OlmDecryptionResultModel result;

    auto extractStr = [&](const std::string& key) -> std::string {
        auto search = "\"" + key + "\":\"";
        auto pos = json.find(search);
        if (pos == std::string::npos) {
            search = "\"" + key + "\": \"";
            pos = json.find(search);
        }
        if (pos == std::string::npos) return "";
        pos += search.size();
        auto end = json.find('"', pos);
        if (end == std::string::npos) return "";
        return json.substr(pos, end - pos);
    };

    result.senderKey       = extractStr("sender_key");
    result.curve25519Key   = extractStr("curve25519_key");
    result.ed25519Key      = extractStr("ed25519_key");
    result.payloadJson     = extractStr("payload");
    result.isSafe          = json.find("\"key_safety\": true") != std::string::npos
                           || json.find("\"key_safety\":true") != std::string::npos;

    auto vs = extractStr("verification_state");
    if (!vs.empty()) result.verificationState = std::atoi(vs.c_str());

    // Parse keys_claimed map
    auto kcPos = json.find("\"keys_claimed\"");
    if (kcPos != std::string::npos) {
        auto brace = json.find('{', kcPos);
        if (brace != std::string::npos) {
            size_t pos = brace + 1;
            while (pos < json.size() && json[pos] != '}') {
                if (json[pos] == '"') {
                    size_t keyEnd = json.find('"', pos + 1);
                    if (keyEnd == std::string::npos) break;
                    std::string mk = json.substr(pos + 1, keyEnd - pos - 1);
                    auto colon = json.find(':', keyEnd);
                    if (colon == std::string::npos) break;
                    auto vq = json.find('"', colon);
                    if (vq == std::string::npos) break;
                    auto ve = json.find('"', vq + 1);
                    if (ve == std::string::npos) break;
                    result.keysClaimed[mk] = json.substr(vq + 1, ve - vq - 1);
                    pos = ve + 1;
                    continue;
                }
                pos++;
            }
        }
    }

    // Parse keys_proved map
    auto kpPos = json.find("\"keys_proved\"");
    if (kpPos != std::string::npos) {
        auto brace = json.find('{', kpPos);
        if (brace != std::string::npos) {
            size_t pos = brace + 1;
            while (pos < json.size() && json[pos] != '}') {
                if (json[pos] == '"') {
                    size_t keyEnd = json.find('"', pos + 1);
                    if (keyEnd == std::string::npos) break;
                    std::string mk = json.substr(pos + 1, keyEnd - pos - 1);
                    auto colon = json.find(':', keyEnd);
                    if (colon == std::string::npos) break;
                    auto vq = json.find('"', colon);
                    if (vq == std::string::npos) break;
                    auto ve = json.find('"', vq + 1);
                    if (ve == std::string::npos) break;
                    result.keysProved[mk] = json.substr(vq + 1, ve - vq - 1);
                    pos = ve + 1;
                    continue;
                }
                pos++;
            }
        }
    }

    // Parse forwarding_curve25519_key_chain array
    auto fcPos = json.find("\"forwarding_curve25519_key_chain\"");
    if (fcPos != std::string::npos) {
        auto bracket = json.find('[', fcPos);
        if (bracket != std::string::npos) {
            size_t pos = bracket + 1;
            while (pos < json.size() && json[pos] != ']') {
                if (json[pos] == '"') {
                    size_t end = json.find('"', pos + 1);
                    if (end != std::string::npos) {
                        result.forwardingCurve25519KeyChain.push_back(
                            json.substr(pos + 1, end - pos - 1));
                        pos = end + 1;
                        continue;
                    }
                }
                pos++;
            }
        }
    }

    return result;
}

// Original Kotlin: serialize OlmDecryptionResultModel to JSON
std::string buildOlmDecryptionResult(const OlmDecryptionResultModel& result) {
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
    json << R"("sender_key": ")" << esc(result.senderKey) << R"(")";
    if (!result.curve25519Key.empty()) {
        json << R"(,"curve25519_key": ")" << esc(result.curve25519Key) << R"(")";
    }
    if (!result.ed25519Key.empty()) {
        json << R"(,"ed25519_key": ")" << esc(result.ed25519Key) << R"(")";
    }

    // keys_claimed
    if (!result.keysClaimed.empty()) {
        json << R"(,"keys_claimed": {)";
        bool first = true;
        for (const auto& [k, v] : result.keysClaimed) {
            if (!first) json << ",";
            first = false;
            json << R"(")" << esc(k) << R"(": ")" << esc(v) << R"(")";
        }
        json << "}";
    }

    // keys_proved
    if (!result.keysProved.empty()) {
        json << R"(,"keys_proved": {)";
        bool first = true;
        for (const auto& [k, v] : result.keysProved) {
            if (!first) json << ",";
            first = false;
            json << R"(")" << esc(k) << R"(": ")" << esc(v) << R"(")";
        }
        json << "}";
    }

    // forwarding_curve25519_key_chain
    if (!result.forwardingCurve25519KeyChain.empty()) {
        json << R"(,"forwarding_curve25519_key_chain": [)";
        for (size_t i = 0; i < result.forwardingCurve25519KeyChain.size(); ++i) {
            if (i > 0) json << ",";
            json << R"(")" << esc(result.forwardingCurve25519KeyChain[i]) << R"(")";
        }
        json << "]";
    }

    if (!result.payloadJson.empty()) {
        json << R"(,"payload": )" << result.payloadJson;
    }

    json << R"(,"key_safety": )" << (result.isSafe ? "true" : "false");
    json << R"(,"verification_state": )" << result.verificationState;
    json << "}";
    return json.str();
}

// ============================================================================
// OlmAccountState — parse/build
// ============================================================================

// Original Kotlin: parse from Matrix /sync device_one_time_keys_count JSON.
OlmAccountState parseOlmAccountState(const std::string& json) {
    OlmAccountState state;

    auto extractInt = [&](const std::string& key) -> int {
        auto pp = json.find("\"" + key + "\"");
        if (pp == std::string::npos) return 0;
        pp = json.find(':', pp);
        if (pp == std::string::npos) return 0;
        pp++;
        while (pp < json.size() && (json[pp] == ' ' || json[pp] == '\t')) pp++;
        int v = 0;
        while (pp < json.size() && json[pp] >= '0' && json[pp] <= '9') { v = v*10+(json[pp]-'0'); pp++; }
        return v;
    };

    state.uploadedSignedKeyCount = extractInt("uploaded_signed_key_count");

    // one_time_key_counts: {"curve25519": N, "signed_curve25519": M}
    auto otPos = json.find("\"one_time_key_counts\"");
    if (otPos != std::string::npos) {
        auto brace = json.find('{', otPos);
        if (brace != std::string::npos) {
            size_t pos = brace + 1;
            while (pos < json.size() && json[pos] != '}') {
                if (json[pos] == '"') {
                    size_t keyEnd = json.find('"', pos + 1);
                    if (keyEnd == std::string::npos) break;
                    std::string k = json.substr(pos + 1, keyEnd - pos - 1);
                    auto colon = json.find(':', keyEnd);
                    if (colon == std::string::npos) break;
                    colon++;
                    while (colon < json.size() && (json[colon] == ' ' || json[colon] == '\t')) colon++;
                    int v = 0;
                    while (colon < json.size() && json[colon] >= '0' && json[colon] <= '9') {
                        v = v*10+(json[colon]-'0'); colon++;
                    }
                    state.oneTimeKeyCounts[k] = v;
                    pos = colon;
                    continue;
                }
                pos++;
            }
        }
    }

    return state;
}

std::string buildOlmAccountState(const OlmAccountState& state) {
    std::ostringstream json;
    json << "{";
    json << R"("uploaded_signed_key_count": )" << state.uploadedSignedKeyCount;
    if (!state.oneTimeKeyCounts.empty()) {
        json << R"(,"one_time_key_counts": {)";
        bool first = true;
        for (const auto& [k, v] : state.oneTimeKeyCounts) {
            if (!first) json << ",";
            first = false;
            json << R"(")" << k << R"(": )" << v;
        }
        json << "}";
    }
    json << "}";
    return json.str();
}

// ============================================================================
// Room Algorithm Detection
// ============================================================================

// Original Kotlin (PrepareToEncryptUseCase.kt:89-91):
//   fun getEncryptionAlgorithm(roomId): String?
std::string getOlmAlgorithmForRoom(const std::string& algorithmJson) {
    if (algorithmJson.empty()) return "";

    auto extractStr = [&](const std::string& key) -> std::string {
        auto search = "\"" + key + "\":\"";
        auto pos = algorithmJson.find(search);
        if (pos == std::string::npos) {
            search = "\"" + key + "\": \"";
            pos = algorithmJson.find(search);
        }
        if (pos == std::string::npos) return "";
        pos += search.size();
        auto end = algorithmJson.find('"', pos);
        if (end == std::string::npos) return "";
        return algorithmJson.substr(pos, end - pos);
    };

    // Check for "algorithm" field
    auto alg = extractStr("algorithm");
    if (!alg.empty()) return alg;

    // Default: megolm
    return CryptoConstants::ALGORITHM_MEGOLM;
}

// ============================================================================
// Megolm Session Data — parse/build
// ============================================================================

// Original Kotlin (MegolmSessionData.kt:26-81):
//   data class MegolmSessionData(algorithm, sessionId, senderKey, roomId, sessionKey, ...)
MegolmSessionData parseMegolmSessionData(const std::string& json) {
    MegolmSessionData data;

    auto extractStr = [&](const std::string& key) -> std::string {
        auto search = "\"" + key + "\":\"";
        auto pos = json.find(search);
        if (pos == std::string::npos) {
            search = "\"" + key + "\": \"";
            pos = json.find(search);
        }
        if (pos == std::string::npos) return "";
        pos += search.size();
        auto end = json.find('"', pos);
        if (end == std::string::npos) return "";
        return json.substr(pos, end - pos);
    };

    data.algorithm             = extractStr("algorithm");
    data.sessionId             = extractStr("session_id");
    data.senderKey             = extractStr("sender_key");
    data.roomId                = extractStr("room_id");
    data.sessionKey            = extractStr("session_key");
    data.senderClaimedEd25519Key = extractStr("sender_claimed_ed25519_key");

    data.sharedHistory = json.find("\"org.matrix.msc3061.shared_history\": true") != std::string::npos
                      || json.find("\"org.matrix.msc3061.shared_history\":true") != std::string::npos;

    // Parse sender_claimed_keys map
    auto scPos = json.find("\"sender_claimed_keys\"");
    if (scPos != std::string::npos) {
        auto brace = json.find('{', scPos);
        if (brace != std::string::npos) {
            size_t pos = brace + 1;
            while (pos < json.size() && json[pos] != '}') {
                if (json[pos] == '"') {
                    size_t keyEnd = json.find('"', pos + 1);
                    if (keyEnd == std::string::npos) break;
                    std::string mk = json.substr(pos + 1, keyEnd - pos - 1);
                    auto colon = json.find(':', keyEnd);
                    if (colon == std::string::npos) break;
                    auto vq = json.find('"', colon);
                    if (vq == std::string::npos) break;
                    auto ve = json.find('"', vq + 1);
                    if (ve == std::string::npos) break;
                    data.senderClaimedKeys[mk] = json.substr(vq + 1, ve - vq - 1);
                    pos = ve + 1;
                    continue;
                }
                pos++;
            }
        }
    }

    // Parse forwarding_curve25519_key_chain array
    auto fcPos = json.find("\"forwarding_curve25519_key_chain\"");
    if (fcPos != std::string::npos) {
        auto bracket = json.find('[', fcPos);
        if (bracket != std::string::npos) {
            size_t pos = bracket + 1;
            while (pos < json.size() && json[pos] != ']') {
                if (json[pos] == '"') {
                    size_t end = json.find('"', pos + 1);
                    if (end != std::string::npos) {
                        data.forwardingCurve25519KeyChain.push_back(
                            json.substr(pos + 1, end - pos - 1));
                        pos = end + 1;
                        continue;
                    }
                }
                pos++;
            }
        }
    }

    return data;
}

// Original Kotlin: serialize MegolmSessionData to JSON
std::string buildMegolmSessionData(const MegolmSessionData& data) {
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
    json << R"("algorithm": ")" << esc(data.algorithm) << R"(")";
    json << R"(,"session_id": ")" << esc(data.sessionId) << R"(")";
    json << R"(,"sender_key": ")" << esc(data.senderKey) << R"(")";
    json << R"(,"room_id": ")" << esc(data.roomId) << R"(")";
    json << R"(,"session_key": ")" << esc(data.sessionKey) << R"(")";
    if (!data.senderClaimedEd25519Key.empty()) {
        json << R"(,"sender_claimed_ed25519_key": ")" << esc(data.senderClaimedEd25519Key) << R"(")";
    }
    if (!data.senderClaimedKeys.empty()) {
        json << R"(,"sender_claimed_keys": {)";
        bool first = true;
        for (const auto& [k, v] : data.senderClaimedKeys) {
            if (!first) json << ",";
            first = false;
            json << R"(")" << esc(k) << R"(": ")" << esc(v) << R"(")";
        }
        json << "}";
    }
    if (!data.forwardingCurve25519KeyChain.empty()) {
        json << R"(,"forwarding_curve25519_key_chain": [)";
        for (size_t i = 0; i < data.forwardingCurve25519KeyChain.size(); ++i) {
            if (i > 0) json << ",";
            json << R"(")" << esc(data.forwardingCurve25519KeyChain[i]) << R"(")";
        }
        json << "]";
    }
    json << R"(,"org.matrix.msc3061.shared_history": )" << (data.sharedHistory ? "true" : "false");
    json << "}";
    return json.str();
}

// ============================================================================
// MegolmInboundSessionInfo — parse/build
// ============================================================================

// Original Kotlin: parse session info from crypto store
MegolmInboundSessionInfo parseMegolmInboundSessionInfo(const std::string& json) {
    MegolmInboundSessionInfo info;

    auto extractStr = [&](const std::string& key) -> std::string {
        auto search = "\"" + key + "\":\"";
        auto pos = json.find(search);
        if (pos == std::string::npos) {
            search = "\"" + key + "\": \"";
            pos = json.find(search);
        }
        if (pos == std::string::npos) return "";
        pos += search.size();
        auto end = json.find('"', pos);
        if (end == std::string::npos) return "";
        return json.substr(pos, end - pos);
    };

    auto extractInt64 = [&](const std::string& key) -> int64_t {
        auto pp = json.find("\"" + key + "\"");
        if (pp == std::string::npos) return 0;
        pp = json.find(':', pp);
        if (pp == std::string::npos) return 0;
        pp++;
        while (pp < json.size() && (json[pp] == ' ' || json[pp] == '\t')) pp++;
        int64_t v = 0;
        while (pp < json.size() && json[pp] >= '0' && json[pp] <= '9') {
            v = v*10+(json[pp]-'0'); pp++;
        }
        return v;
    };

    info.sessionId    = extractStr("session_id");
    info.senderKey    = extractStr("sender_key");
    info.ed25519Key   = extractStr("ed25519_key");
    info.roomId       = extractStr("room_id");
    info.algorithm    = extractStr("algorithm");
    info.firstKnownIndex = extractInt64("first_known_index");
    info.lastReceivedTs  = extractInt64("last_received_ts");
    info.messageCount    = extractInt64("message_count");

    info.isInbound       = json.find("\"is_inbound\": true") != std::string::npos
                         || json.find("\"is_inbound\":true") != std::string::npos;
    info.hasBeenBackedUp = json.find("\"has_been_backed_up\": true") != std::string::npos
                         || json.find("\"has_been_backed_up\":true") != std::string::npos;
    info.sharedHistory   = json.find("\"shared_history\": true") != std::string::npos
                         || json.find("\"shared_history\":true") != std::string::npos;

    if (info.algorithm.empty()) info.algorithm = CryptoConstants::ALGORITHM_MEGOLM;
    return info;
}

// Original Kotlin: serialize MegolmInboundSessionInfo to JSON
std::string buildMegolmInboundSessionInfo(const MegolmInboundSessionInfo& info) {
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
    json << R"("session_id": ")" << esc(info.sessionId) << R"(")";
    json << R"(,"sender_key": ")" << esc(info.senderKey) << R"(")";
    json << R"(,"ed25519_key": ")" << esc(info.ed25519Key) << R"(")";
    json << R"(,"room_id": ")" << esc(info.roomId) << R"(")";
    json << R"(,"algorithm": ")" << esc(info.algorithm) << R"(")";
    json << R"(,"first_known_index": )" << info.firstKnownIndex;
    json << R"(,"is_inbound": )" << (info.isInbound ? "true" : "false");
    json << R"(,"has_been_backed_up": )" << (info.hasBeenBackedUp ? "true" : "false");
    json << R"(,"shared_history": )" << (info.sharedHistory ? "true" : "false");
    json << R"(,"last_received_ts": )" << info.lastReceivedTs;
    json << R"(,"message_count": )" << info.messageCount;
    json << "}";
    return json.str();
}

// ============================================================================
// Room History Visibility
// ============================================================================

// Original Kotlin (RoomHistoryVisibility.kt):
//   fun RoomHistoryVisibility.shouldShareHistory()
RoomHistoryVisibility parseRoomHistoryVisibility(const std::string& value) {
    if (value == "world_readable") return RoomHistoryVisibility::WORLD_READABLE;
    if (value == "shared")        return RoomHistoryVisibility::SHARED;
    if (value == "invited")       return RoomHistoryVisibility::INVITED;
    if (value == "joined")        return RoomHistoryVisibility::JOINED;
    return RoomHistoryVisibility::JOINED;
}

std::string roomHistoryVisibilityToString(RoomHistoryVisibility visibility) {
    switch (visibility) {
        case RoomHistoryVisibility::WORLD_READABLE: return "world_readable";
        case RoomHistoryVisibility::SHARED:         return "shared";
        case RoomHistoryVisibility::INVITED:        return "invited";
        case RoomHistoryVisibility::JOINED:         return "joined";
    }
    return "joined";
}

// ============================================================================
// Session ID Formatting
// ============================================================================

// Original Kotlin (MegolmSessionData.kt): sessionId.take(8) + "..." + sessionId.takeLast(8)
std::string formatMegolmSessionId(const std::string& sessionId, int prefixLen) {
    if (sessionId.empty()) return "";
    if (static_cast<int>(sessionId.size()) <= prefixLen * 2 + 3) return sessionId;
    return sessionId.substr(0, prefixLen) + "..." + sessionId.substr(sessionId.size() - prefixLen);
}

} // namespace progressive
