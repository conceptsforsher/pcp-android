#include "progressive/megolm_decryptor.hpp"
#include <olm/inbound_group_session.h>
#include <olm/olm.h>
#include <cstring>
#include <algorithm>
#include <android/log.h>

#define LOG_TAG "MegolmDecryptor"
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)

namespace progressive {

static const char B64_C[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static std::vector<uint8_t> b64Decode(const std::string& in) {
    std::vector<uint8_t> r;
    int val = 0, vb = -8;
    for (char c : in) {
        if (c == '=') break;
        const char* p = strchr(B64_C, c); if (!p) continue;
        val = (val<<6)+(int)(p-B64_C); vb += 6;
        if (vb >= 0) { r.push_back((uint8_t)((val>>vb)&0xFF)); vb -= 8; }
    }
    return r;
}

// ==== Megolm Session ====

MegolmSession createInboundMegolmSession(const std::vector<uint8_t>& sessionKey) {
    MegolmSession result;

    size_t size = olm_inbound_group_session_size();
    if (size == 0) return result;

    void* session = malloc(size);
    if (!session) return result;

    auto* olmSession = olm_inbound_group_session(session);
    size_t ret = olm_import_inbound_group_session(
        olmSession, sessionKey.data(), sessionKey.size());
    if (ret == olm_error()) {
        const char* err = olm_inbound_group_session_last_error(olmSession);
        LOGW("olm_import_inbound_group_session failed: %s", err ? err : "unknown");
        olm_clear_inbound_group_session(olmSession);
        free(session);
        return result;
    }

    // Get session ID (base64-encoded)
    size_t idLen = olm_inbound_group_session_id_length(olmSession);
    std::vector<uint8_t> idBuf(idLen);
    ret = olm_inbound_group_session_id(olmSession, idBuf.data(), idLen);
    if (ret == olm_error()) { free(session); return result; }
    idBuf.resize(ret);
    std::string sessionId(idBuf.begin(), idBuf.end());

    result.session = session;
    result.sessionId = sessionId;
    result.firstKnownIndex = (uint32_t)olm_inbound_group_session_first_known_index(olmSession);
    result.valid = true;

    return result;
}

void destroyMegolmSession(MegolmSession& session) {
    if (session.session) {
        olm_clear_inbound_group_session(olm_inbound_group_session(session.session));
        free(session.session);
        session.session = nullptr;
    }
    session.valid = false;
}

std::string megolmDecrypt(MegolmSession& session, const std::string& ciphertext) {
    if (!session.valid || !session.session) return "";

    auto* olmSession = olm_inbound_group_session(session.session);

    // libolm overwrites message buffer with base64-decoded data — use mutable copy
    std::vector<uint8_t> msg(ciphertext.begin(), ciphertext.end());

    size_t maxLen = olm_group_decrypt_max_plaintext_length(olmSession, msg.data(), msg.size());
    if (maxLen == olm_error()) return "";

    std::vector<uint8_t> plaintext(maxLen);
    uint32_t messageIndex = 0;
    size_t ret = olm_group_decrypt(olmSession, msg.data(), msg.size(),
        plaintext.data(), maxLen, &messageIndex);
    if (ret == olm_error()) {
        const char* err = olm_inbound_group_session_last_error(olmSession);
        LOGW("olm_group_decrypt failed: %s", err ? err : "unknown");
        return "";
    }

    return std::string(plaintext.begin(), plaintext.begin() + ret);
}

std::string getMegolmSessionId(const MegolmSession& session) {
    return session.sessionId;
}

std::string exportMegolmSession(const MegolmSession& session) {
    if (!session.valid || !session.session) return "";

    auto* olmSession = olm_inbound_group_session(session.session);
    size_t len = olm_export_inbound_group_session_length(olmSession);
    if (len == olm_error()) return "";

    std::vector<uint8_t> key(len);
    uint32_t msgIndex = session.firstKnownIndex;
    size_t ret = olm_export_inbound_group_session(olmSession, key.data(), len, msgIndex);
    if (ret == olm_error()) return "";
    return std::string(key.begin(), key.begin() + ret);
}

// ==== Session Manager ====

bool MegolmSessionManager::addSession(const std::string& roomId, const std::string& senderKey,
                                       const std::string& sessionId, const std::string& sessionKeyBase64) {
    auto keyBytes = b64Decode(sessionKeyBase64);
    if (keyBytes.empty()) return false;

    auto session = createInboundMegolmSession(keyBytes);
    if (!session.valid) return false;

    SessionKey key{roomId, senderKey, sessionId};
    sessions_[key] = std::move(session);
    return true;
}

MegolmSession* MegolmSessionManager::findSession(const std::string& roomId, const std::string& senderKey,
                                                   const std::string& sessionId) {
    SessionKey key{roomId, senderKey, sessionId};
    auto it = sessions_.find(key);
    return it != sessions_.end() ? &it->second : nullptr;
}

void MegolmSessionManager::clearRoom(const std::string& roomId) {
    auto it = sessions_.begin();
    while (it != sessions_.end()) {
        if (it->first.roomId == roomId) {
            destroyMegolmSession(it->second);
            it = sessions_.erase(it);
        } else { ++it; }
    }
}

void MegolmSessionManager::clearAll() {
    for (auto& pair : sessions_) destroyMegolmSession(pair.second);
    sessions_.clear();
}

} // namespace progressive
