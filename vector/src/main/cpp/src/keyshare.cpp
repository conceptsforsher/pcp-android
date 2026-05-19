#include "progressive/keyshare.hpp"
#include "progressive/json_parser.hpp"
#include <sstream>
#include <chrono>

namespace progressive {

// ---- Existing functions ----

KeyRequestInfo parseKeyRequest(const std::string& eventContentJson, const std::string& eventId,
    const std::string& senderId) {
    KeyRequestInfo info;
    info.requestId = eventId;

    auto body = parseJsonStringValue(eventContentJson, "body");
    if (body.empty()) return info;

    std::string wrapped = "{" + body + "}";
    info.algorithm           = parseJsonStringValue(wrapped, "algorithm");
    info.roomId              = parseJsonStringValue(wrapped, "room_id");
    info.sessionId           = parseJsonStringValue(wrapped, "session_id");
    info.senderKey           = parseJsonStringValue(wrapped, "sender_key");
    info.requestingDeviceId  = parseJsonStringValue(wrapped, "requesting_device_id");
    info.requestingUserId    = senderId;

    auto requestId = parseJsonStringValue(wrapped, "request_id");
    if (!requestId.empty()) info.requestId = requestId;

    info.requestedAtMs = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();

    return info;
}

bool shouldShareKey(const std::string& algorithm, bool hasSession,
    bool sessionIsVerified, bool userIsTrusted) {
    if (!hasSession) return false;
    if (algorithm != "m.megolm.v1.aes-sha2") return false;
    return sessionIsVerified || userIsTrusted;
}

std::string buildForwardedKeyContent(const std::string& roomId, const std::string& sessionId,
    const std::string& sessionKey, int messageIndex, const std::string& algorithm,
    bool sharedHistory) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << "{";
    json << R"("room_id": ")" << esc(roomId) << R"(",)";
    json << R"("session_id": ")" << esc(sessionId) << R"(",)";
    json << R"("session_key": ")" << esc(sessionKey) << R"(",)";
    json << R"("algorithm": ")" << esc(algorithm) << R"(",)";
    json << R"("message_index": )" << messageIndex;
    if (sharedHistory) {
        json << R"(,"org.matrix.msc3061.shared_history": true)";
    }
    json << "}";
    return json.str();
}

std::string buildKeyRequestBody(const std::string& roomId, const std::string& sessionId,
    const std::string& senderKey, const std::string& algorithm,
    const std::string& requestId, const std::string& requestingDeviceId) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << "{";
    json << R"("action": "request",)";
    json << R"("body": {)";
    json << R"("algorithm": ")" << esc(algorithm) << R"(",)";
    json << R"("room_id": ")" << esc(roomId) << R"(",)";
    json << R"("session_id": ")" << esc(sessionId) << R"(",)";
    json << R"("sender_key": ")" << esc(senderKey) << R"(",)";
    json << R"("request_id": ")" << esc(requestId) << R"(",)";
    json << R"("requesting_device_id": ")" << esc(requestingDeviceId) << R"(")";
    json << "}}";
    return json.str();
}

std::string buildKeyRequestCancelBody(const std::string& requestId) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    return R"({"action": "request_cancellation", "body": {"request_id": ")" + esc(requestId) + R"("}})";
}

std::string formatKeyRequestNotification(const KeyRequestInfo& info) {
    std::ostringstream out;
    out << "Key request from " << info.requestingUserId;
    if (!info.roomId.empty()) {
        out << " for room " << info.roomId;
    }
    return out.str();
}

bool isKeyRequestExpired(const KeyRequestInfo& info, int timeoutMinutes) {
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    return (now - info.requestedAtMs) > timeoutMinutes * 60 * 1000LL;
}

// ---- NEW: Key Share Request/Response (structured) ----
// Original Kotlin (IncomingRoomKeyRequest.kt + m.room_key_request):
//   to_device content: { "action":"request", "requesting_device_id":"...", "request_id":"...",
//     "body":{ "room_id":"...", "session_id":"...", "sender_key":"...", "algorithm":"..." } }

std::string buildKeyShareRequest(const KeyShareRequest& request) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"action":")" << esc(request.action) << R"(")";
    json << R"(,"requesting_device_id":")" << esc(request.requestingDeviceId) << R"(")";
    json << R"(,"request_id":")" << esc(request.requestId) << R"(")";
    json << R"(,"body":{)";
    json << R"("algorithm":")" << esc(request.algorithm) << R"(")";
    json << R"(,"room_id":")" << esc(request.roomId) << R"(")";
    json << R"(,"session_id":")" << esc(request.sessionId) << R"(")";
    json << R"(,"sender_key":")" << esc(request.senderKey) << R"(")";
    json << "}}";
    return json.str();
}

// Original Kotlin (ForwardedRoomKeyContent.kt + m.forwarded_room_key):
//   to_device content: { "room_id":"...", "session_id":"...", "session_key":"...",
//     "sender_key":"...", "algorithm":"...", "message_index":..., "forwarded_count":...,
//     "sender_claimed_ed25519_key":"..." }

std::string buildKeyShareResponse(const KeyShareResponse& response) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"room_id":")" << esc(response.roomId) << R"(")";
    json << R"(,"session_id":")" << esc(response.sessionId) << R"(")";
    json << R"(,"session_key":")" << esc(response.sessionKey) << R"(")";
    json << R"(,"sender_key":")" << esc(response.senderKey) << R"(")";
    json << R"(,"algorithm":")" << esc(response.algorithm) << R"(")";
    json << R"(,"message_index":)" << response.messageIndex;
    json << R"(,"forwarded_count":)" << response.forwardedCount;
    if (!response.senderClaimedEd25519Key.empty()) {
        json << R"(,"sender_claimed_ed25519_key":")" << esc(response.senderClaimedEd25519Key) << R"(")";
    }
    if (response.sharedHistory) {
        json << R"(,"org.matrix.msc3061.shared_history":true)";
    }
    json << "}";
    return json.str();
}

// Original Kotlin (IncomingRoomKeyRequest.kt + parseRequest):
//   Parse m.room_key_request event content into KeyShareRequest.

KeyShareRequest parseKeyShareRequest(const std::string& eventContentJson) {
    KeyShareRequest req;
    req.action = parseJsonStringValue(eventContentJson, "action");
    req.requestingDeviceId = parseJsonStringValue(eventContentJson, "requesting_device_id");
    req.requestId = parseJsonStringValue(eventContentJson, "request_id");

    // Parse body sub-object
    auto bodyStart = eventContentJson.find("\"body\"");
    if (bodyStart != std::string::npos) {
        bodyStart = eventContentJson.find('{', bodyStart);
        if (bodyStart != std::string::npos) {
            int depth = 1;
            size_t pos = bodyStart + 1;
            while (pos < eventContentJson.size() && depth > 0) {
                if (eventContentJson[pos] == '{') depth++;
                else if (eventContentJson[pos] == '}') depth--;
                pos++;
            }
            std::string body = eventContentJson.substr(bodyStart, pos - bodyStart);

            req.algorithm = parseJsonStringValue(body, "algorithm");
            req.roomId = parseJsonStringValue(body, "room_id");
            req.sessionId = parseJsonStringValue(body, "session_id");
            req.senderKey = parseJsonStringValue(body, "sender_key");

            // request_id might also be in body
            auto bodyReqId = parseJsonStringValue(body, "request_id");
            if (!bodyReqId.empty()) req.requestId = bodyReqId;
        }
    }

    return req;
}

// Original Kotlin (ForwardedRoomKeyContent.kt + parseResponse):
//   Parse m.forwarded_room_key event content into KeyShareResponse.

KeyShareResponse parseKeyShareResponse(const std::string& eventContentJson) {
    KeyShareResponse resp;
    resp.roomId = parseJsonStringValue(eventContentJson, "room_id");
    resp.sessionId = parseJsonStringValue(eventContentJson, "session_id");
    resp.sessionKey = parseJsonStringValue(eventContentJson, "session_key");
    resp.senderKey = parseJsonStringValue(eventContentJson, "sender_key");
    resp.algorithm = parseJsonStringValue(eventContentJson, "algorithm");
    resp.senderClaimedEd25519Key = parseJsonStringValue(eventContentJson, "sender_claimed_ed25519_key");

    auto msgIdx = parseJsonStringValue(eventContentJson, "message_index");
    if (!msgIdx.empty()) {
        try { resp.messageIndex = std::stoll(msgIdx); } catch (...) { resp.messageIndex = 0; }
    }

    auto fwdCount = parseJsonStringValue(eventContentJson, "forwarded_count");
    if (!fwdCount.empty()) {
        try { resp.forwardedCount = std::stoll(fwdCount); } catch (...) { resp.forwardedCount = 0; }
    }

    resp.sharedHistory = (eventContentJson.find("\"org.matrix.msc3061.shared_history\":true") != std::string::npos ||
                          eventContentJson.find("\"org.matrix.msc3061.shared_history\": true") != std::string::npos);

    return resp;
}

// ---- Key Share Decision Logic ----
// Original Kotlin (KeysBackup.kt + RoomKeyRequestHandler + cryptography best practices):
//   Decide whether to share, ignore, or defer a key share request.
//   Checks: room membership, device verification, session availability, session verification.

KeyShareDecision decideKeyShare(bool isRoomMember, bool isDeviceVerified,
                                bool hasSession, bool sessionIsVerified) {
    // Must have the session to share it
    if (!hasSession) return KeyShareDecision::IGNORE;

    // Must be a room member (prevents leaking keys outside rooms)
    if (!isRoomMember) return KeyShareDecision::IGNORE;

    // If the requesting device is verified, share automatically
    if (isDeviceVerified) return KeyShareDecision::SHARE;

    // If the session itself was verified when received, share
    if (sessionIsVerified) return KeyShareDecision::SHARE;

    // Neither device nor session is verified — defer for user input
    return KeyShareDecision::DEFER;
}

} // namespace progressive
