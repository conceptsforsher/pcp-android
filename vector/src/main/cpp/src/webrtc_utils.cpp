#include "progressive/webrtc_utils.hpp"
#include "progressive/event_classifier.hpp"
#include "progressive/json_parser.hpp"
#include <sstream>
#include <chrono>

namespace progressive {

CallInfo parseCallInvite(const std::string& eventContentJson, const std::string& eventId,
    const std::string& roomId, const std::string& senderId) {
    CallInfo call;
    call.callId      = parseJsonStringValue(eventContentJson, "call_id");
    if (call.callId.empty()) call.callId = eventId;
    call.roomId      = roomId;
    call.callerId    = senderId;
    call.isVideo     = (eventContentJson.find("\"m.video\"") != std::string::npos);
    call.isActive    = true;
    call.isIncoming  = true;
    call.startedAtMs = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    return call;
}

SdpOffer parseSdpOffer(const std::string& eventContentJson) {
    SdpOffer offer;
    offer.type = parseJsonStringValue(eventContentJson, "type");
    offer.sdp  = parseJsonStringValue(eventContentJson, "sdp");
    // Alternative: description field
    if (offer.sdp.empty()) {
        auto desc = parseJsonStringValue(eventContentJson, "description");
        if (!desc.empty()) {
            auto offerType = parseJsonStringValue("{" + desc + "}", "type");
            auto offerSdp  = parseJsonStringValue("{" + desc + "}", "sdp");
            if (!offerSdp.empty()) {
                offer.type = offerType;
                offer.sdp = offerSdp;
            }
        }
    }
    offer.valid = !offer.sdp.empty();
    return offer;
}

IceCandidate parseIceCandidate(const std::string& eventContentJson) {
    IceCandidate ice;
    ice.sdpMid = parseJsonStringValue(eventContentJson, "sdpMid");
    ice.candidate = parseJsonStringValue(eventContentJson, "candidate");

    auto mlIdx = parseJsonStringValue(eventContentJson, "sdpMLineIndex");
    if (!mlIdx.empty()) ice.sdpMLineIndex = std::stoi(mlIdx);

    // Also try in `candidates` array
    if (ice.candidate.empty()) {
        auto cands = parseJsonStringValue(eventContentJson, "candidates");
        if (!cands.empty()) {
            auto firstCand = parseJsonStringValue("[" + cands + "]", "candidate");
            if (!firstCand.empty()) ice.candidate = firstCand;
        }
    }
    return ice;
}

std::string buildCallInviteContent(const std::string& callId, bool isVideo,
    const std::string& sdpOffer, int lifetimeSeconds) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"call_id": ")" << esc(callId) << R"(")";
    json << R"(,"offer": {"type": "offer", "sdp": ")" << esc(sdpOffer) << R"("})";
    if (isVideo) json << R"(,"m.video": true)";
    json << R"(,"version": "1")";
    if (lifetimeSeconds > 0)
        json << R"(,"lifetime": )" << lifetimeSeconds;
    json << "}";
    return json.str();
}

std::string buildCallAnswerContent(const std::string& callId, const std::string& sdpAnswer) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"call_id": ")" << esc(callId) << R"(")";
    json << R"(,"answer": {"type": "answer", "sdp": ")" << esc(sdpAnswer) << R"("})";
    json << R"(,"version": "1")";
    json << "}";
    return json.str();
}

std::string buildCallCandidatesContent(const std::string& callId,
    const std::vector<IceCandidate>& candidates) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"call_id": ")" << esc(callId) << R"(")";
    json << R"(,"candidates": [)";
    for (size_t i = 0; i < candidates.size(); ++i) {
        if (i > 0) json << ",";
        json << R"({"sdpMid": ")" << esc(candidates[i].sdpMid) << R"(")";
        json << R"(,"sdpMLineIndex": )" << candidates[i].sdpMLineIndex;
        json << R"(,"candidate": ")" << esc(candidates[i].candidate) << R"(")" << "}";
    }
    json << R"(],"version": "1")";
    json << "}";
    return json.str();
}

std::string buildCallHangupContent(const std::string& callId, const std::string& reason) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"call_id": ")" << esc(callId) << R"(")";
    if (!reason.empty()) json << R"(,"reason": ")" << esc(reason) << R"(")";
    json << R"(,"version": "1")";
    json << "}";
    return json.str();
}

std::string formatCallNotification(const CallInfo& call) {
    std::ostringstream out;
    out << (call.isVideo ? "Video" : "Voice") << " call";
    if (call.isIncoming) out << " from " << call.callerName;
    return out.str();
}

std::string formatCallDuration(int seconds) {
    int h = seconds / 3600;
    int m = (seconds % 3600) / 60;
    int s = seconds % 60;

    std::ostringstream out;
    if (h > 0) {
        out << h << ":";
        if (m < 10) out << "0";
    }
    out << m << ":";
    if (s < 10) out << "0";
    out << s;
    return out.str();
}

bool isCallExpired(int64_t createdAtMs, int timeoutSeconds) {
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    return (now - createdAtMs) > timeoutSeconds * 1000LL;
}

std::string getCallState(const std::string& eventContentJson) {
    if (eventContentJson.find("\"m.call.invite\"") != std::string::npos) return "invite";
    if (eventContentJson.find("\"m.call.answer\"") != std::string::npos) return "answer";
    if (eventContentJson.find("\"m.call.candidates\"") != std::string::npos) return "candidates";
    if (eventContentJson.find("\"m.call.hangup\"") != std::string::npos) return "hangup";
    if (eventContentJson.find("\"m.call.reject\"") != std::string::npos) return "reject";
    return "unknown";
}

} // namespace progressive
