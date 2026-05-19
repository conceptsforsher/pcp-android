#pragma once

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

// ---- Key Share Request (to_device event) ----
// Original Kotlin (IncomingRoomKeyRequest.kt + m.room_key_request):
//   { "action":"request", "requesting_device_id":"...", "request_id":"...",
//     "body":{ "room_id":"...", "session_id":"...", "sender_key":"...", "algorithm":"..." } }

struct KeyShareRequest {
    std::string action;              // "request" or "request_cancellation"
    std::string requestingDeviceId;  // device that is requesting the key
    std::string requestId;           // unique identifier for this request
    std::string roomId;              // room the session belongs to
    std::string sessionId;           // the megolm session id
    std::string senderKey;           // the curve25519 sender key
    std::string algorithm;           // "m.megolm.v1.aes-sha2"
};

// ---- Key Share Response (forwarded room key) ----
// Original Kotlin (ForwardedRoomKeyContent.kt + m.forwarded_room_key):
//   { "room_id":"...", "session_id":"...", "session_key":"...",
//     "sender_key":"...", "algorithm":"...", "forwarded_count":..., "message_index":... }

struct KeyShareResponse {
    std::string action;              // "send" (implicit)
    std::string requestId;           // original request id for matching
    std::string roomId;              // room the session belongs to
    std::string sessionId;           // the megolm session id
    std::string sessionKey;          // the base64-encoded megolm session key
    std::string senderKey;           // the curve25519 key of the original sender
    std::string senderClaimedEd25519Key; // ed25519 key claimed by original sender
    std::vector<uint8_t> senderChainIndex; // chain index at forwarding point
    int64_t messageIndex = 0;        // message index at forwarding point
    int64_t forwardedCount = 0;      // number of times this key has been forwarded
    std::string algorithm;           // "m.megolm.v1.aes-sha2"
    bool sharedHistory = false;      // MSC3061: share with invited members
};

// ---- Key Share Decision ----
// Original Kotlin (KeysBackup.kt + cryptography decision logic):
//   Decide whether to share a requested key, ignore the request, or defer.

enum class KeyShareDecision {
    SHARE = 0,      // Share the key with the requesting device
    IGNORE = 1,     // Ignore the request (don't have key, or not allowed)
    DEFER = 2       // Defer the decision (e.g., need user confirmation)
};

struct KeyShareResult {
    bool success = false;
    std::string sessionKey;      // exported session key
    int messageIndex = 0;
    std::string errorMessage;
};

// ---- Key Share JSON Builders / Parsers ----
// Original Kotlin (IncomingRoomKeyRequest.kt + m.room_key_request):
//   Build request: { "action":"request", "requesting_device_id":"...", "request_id":"...",
//     "body":{ "room_id":"...", "session_id":"...", "sender_key":"...", "algorithm":"..." } }

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

// ---- NEW: Key Share Request/Response (structured) ----

// Build to_device JSON for a key share request.
// Output: { "action":"request", "requesting_device_id":"...", ... }
// Original Kotlin (m.room_key_request full to_device content):
std::string buildKeyShareRequest(const KeyShareRequest& request);

// Build to_device JSON for a key share response (forwarded key).
// Output: { "room_id":"...", "session_id":"...", "session_key":"...", ... }
// Original Kotlin (m.forwarded_room_key to_device content):
std::string buildKeyShareResponse(const KeyShareResponse& response);

// Parse a key share request from to_device event JSON content.
// Input: full content of m.room_key_request event.
// Original Kotlin (IncomingRoomKeyRequest.kt + parseRequest):
KeyShareRequest parseKeyShareRequest(const std::string& eventContentJson);

// Parse a key share response from to_device event JSON content.
// Input: full content of m.forwarded_room_key event.
// Original Kotlin (ForwardedRoomKeyContent.kt + parseResponse):
KeyShareResponse parseKeyShareResponse(const std::string& eventContentJson);

// ---- Key Share Decision Logic ----
// Original Kotlin (KeysBackup.kt + RoomKeyRequestHandler):
//   Decide whether to share a requested key.
//   Checks: room membership, device verification, session availability.

// Determine whether to share, ignore, or defer a key share request.
// Parameters:
//   isRoomMember:     true if the requesting user is in the room
//   isDeviceVerified: true if the requesting device is verified
//   hasSession:       true if we have the requested megolm session locally
//   sessionIsVerified:true if the session was verified when received
KeyShareDecision decideKeyShare(bool isRoomMember, bool isDeviceVerified,
                                bool hasSession, bool sessionIsVerified);

} // namespace progressive
