#ifndef PROGRESSIVE_KEYSHARE_HPP
#define PROGRESSIVE_KEYSHARE_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// ---- Key Share Request/Response ----

struct KeyRequestInfo {
    std::string requestId;
    std::string roomId;
    std::string sessionId;
    std::string senderKey;
    std::string algorithm;       // "m.megolm.v1.aes-sha2"
    std::string requestingDeviceId;
    std::string requestingUserId;
    int64_t requestedAtMs = 0;
    bool isHandled = false;
    bool isApproved = false;
};

struct KeyShareResult {
    bool success = false;
    std::string sessionKey;      // exported session key
    int messageIndex = 0;
    std::string errorMessage;
};

// Parse key request from Matrix to_device event.
KeyRequestInfo parseKeyRequest(const std::string& eventContentJson, const std::string& eventId,
    const std::string& senderId);

// Check if we should share this key (have the session, it's verified).
bool shouldShareKey(const std::string& algorithm, bool hasSession,
    bool sessionIsVerified, bool userIsTrusted);

// Build key share forward event content.
std::string buildForwardedKeyContent(const std::string& roomId, const std::string& sessionId,
    const std::string& sessionKey, int messageIndex, const std::string& algorithm,
    bool sharedHistory = false);

// Build key request event content.
std::string buildKeyRequestBody(const std::string& roomId, const std::string& sessionId,
    const std::string& senderKey, const std::string& algorithm,
    const std::string& requestId, const std::string& requestingDeviceId);

// Build key request cancellation content.
std::string buildKeyRequestCancelBody(const std::string& requestId);

// Format key request for notification.
std::string formatKeyRequestNotification(const KeyRequestInfo& info);

// Check if a key request has expired (default 10 minutes).
bool isKeyRequestExpired(const KeyRequestInfo& info, int timeoutMinutes = 10);

} // namespace progressive

#endif // PROGRESSIVE_KEYSHARE_HPP
