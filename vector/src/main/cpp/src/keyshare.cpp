#include "progressive/keyshare.hpp"
#include "progressive/json_parser.hpp"
#include <sstream>
#include <chrono>

namespace progressive {

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

} // namespace progressive
