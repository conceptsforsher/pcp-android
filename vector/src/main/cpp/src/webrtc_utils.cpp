#include "progressive/webrtc_utils.hpp"
#include "progressive/event_classifier.hpp"
#include "progressive/json_parser.hpp"
#include <sstream>
#include <chrono>

namespace progressive {

// ====================================================================
// JSON Helpers (local)
// ====================================================================

static std::string jEscape(const std::string& s) {
    std::string out;
    for (char c : s) {
        if (c == '"') out += "\\\"";
        else if (c == '\\') out += "\\\\";
        else out += c;
    }
    return out;
}

static int extractJsonInt(const std::string& json, const std::string& key) {
    auto val = parseJsonStringValue(json, key);
    if (val.empty()) return 0;
    int result = 0;
    for (char c : val) {
        if (c >= '0' && c <= '9') result = result * 10 + (c - '0');
        else break;
    }
    return result;
}

static bool extractJsonBool(const std::string& json, const std::string& key) {
    auto val = parseJsonStringValue(json, key);
    return val == "true";
}

static std::string extractJsonObject(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return "";
    pos = json.find(':', pos);
    if (pos == std::string::npos) return "";
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n' || json[pos] == '\r')) pos++;
    if (pos >= json.size() || json[pos] != '{') return "";
    int depth = 1;
    size_t start = pos;
    pos++;
    while (pos < json.size() && depth > 0) {
        if (json[pos] == '{') depth++;
        else if (json[pos] == '}') depth--;
        pos++;
    }
    return json.substr(start, pos - start);
}

// ====================================================================
// CallState
// ====================================================================

// Original Kotlin: CallState.kt
const char* CallStateToString(CallState state) {
    switch (state) {
        case CallState::IDLE:          return "idle";
        case CallState::INVITE_SENT:   return "invite_sent";
        case CallState::RINGING:       return "ringing";
        case CallState::CREATED:       return "created";
        case CallState::CONNECTING:    return "connecting";
        case CallState::CONNECTED:     return "connected";
        case CallState::DISCONNECTED:  return "disconnected";
        case CallState::TERMINATED:    return "terminated";
    }
    return "idle";
}

CallState CallStateFromString(const std::string& s) {
    if (s == "idle")          return CallState::IDLE;
    if (s == "invite_sent")   return CallState::INVITE_SENT;
    if (s == "ringing")       return CallState::RINGING;
    if (s == "created")       return CallState::CREATED;
    if (s == "connecting")    return CallState::CONNECTING;
    if (s == "connected")     return CallState::CONNECTED;
    if (s == "disconnected")  return CallState::DISCONNECTED;
    if (s == "terminated")    return CallState::TERMINATED;
    return CallState::IDLE;
}

// ====================================================================
// CallCandidate Build/Parse
// ====================================================================

// Original Kotlin: CallCandidate.kt
std::string buildCallCandidate(const CallCandidate& cand) {
    std::ostringstream json;
    json << "{\"sdpMid\": \"" << jEscape(cand.sdpMid) << "\"";
    json << ",\"sdpMLineIndex\": " << cand.sdpMLineIndex;
    json << ",\"candidate\": \"" << jEscape(cand.candidate) << "\"}";
    return json.str();
}

// Original Kotlin: CallCandidate.kt
CallCandidate parseCallCandidate(const std::string& json) {
    CallCandidate cand;
    cand.sdpMid = parseJsonStringValue(json, "sdpMid");
    cand.sdpMLineIndex = extractJsonInt(json, "sdpMLineIndex");
    cand.candidate = parseJsonStringValue(json, "candidate");
    return cand;
}

// ====================================================================
// CallCapabilities Build/Parse
// ====================================================================

// Original Kotlin: CallCapabilities.kt
std::string callCapabilitiesToJson(const CallCapabilities& caps) {
    std::ostringstream json;
    json << "{";
    bool first = true;
    if (caps.transferee) {
        json << "\"m.call.transferee\": true";
        first = false;
    }
    if (caps.supportsDtmf) {
        if (!first) json << ",";
        json << "\"m.call.dtmf\": true";
        first = false;
    }
    if (caps.useStereo) {
        if (!first) json << ",";
        json << "\"m.call.use_stereo\": true";
        first = false;
    }
    if (caps.supportsScreenSharing) {
        if (!first) json << ",";
        json << "\"m.call.screen_sharing\": true";
        first = false;
    }
    // supportsVideo and hasVideo are implicitly true — not serialized unless false
    if (!caps.supportsVideo) {
        if (!first) json << ",";
        json << "\"m.call.video\": false";
        first = false;
    }
    json << "}";
    return json.str();
}

// Original Kotlin: CallCapabilities.kt
CallCapabilities parseCallCapabilities(const std::string& json) {
    CallCapabilities caps;
    caps.transferee = extractJsonBool(json, "m.call.transferee");
    caps.supportsDtmf = extractJsonBool(json, "m.call.dtmf");
    caps.useStereo = extractJsonBool(json, "m.call.use_stereo");
    caps.supportsScreenSharing = extractJsonBool(json, "m.call.screen_sharing");

    // Video is implicitly supported unless explicitly disabled
    auto v = extractJsonBool(json, "m.call.video");
    caps.supportsVideo = true; // default true — most WebRTC clients support video
    if (json.find("\"m.call.video\": false") != std::string::npos ||
        json.find("\"m.call.video\":false") != std::string::npos)
        caps.supportsVideo = false;

    return caps;
}

// ====================================================================
// Builders — CallInviteContent (m.call.invite)
// ====================================================================

// Original Kotlin: CallInviteContent.kt / CallSignalingService.kt
std::string buildCallInviteContent(const std::string& callId, bool isVideo,
    const std::string& sdpOffer, int lifetimeSeconds,
    const std::string& invitee, const CallCapabilities& capabilities) {
    std::ostringstream json;
    json << "{\"call_id\": \"" << jEscape(callId) << "\"";
    json << ",\"offer\": {\"type\": \"offer\", \"sdp\": \"" << jEscape(sdpOffer) << "\"}";
    json << ",\"version\": \"0\"";
    json << ",\"lifetime\": " << lifetimeSeconds;
    if (!invitee.empty())
        json << ",\"invitee\": \"" << jEscape(invitee) << "\"";
    auto capJson = callCapabilitiesToJson(capabilities);
    if (capJson != "{}")
        json << ",\"capabilities\": " << capJson;
    json << "}";
    return json.str();
}

std::string buildCallInviteContent(const CallInviteContent& content) {
    std::ostringstream json;
    json << "{\"call_id\": \"" << jEscape(content.callId) << "\"";
    if (!content.partyId.empty())
        json << ",\"party_id\": \"" << jEscape(content.partyId) << "\"";
    json << ",\"offer\": {\"type\": \"" << sdpTypeToString(content.offer.type)
         << "\", \"sdp\": \"" << jEscape(content.offer.sdp) << "\"}";
    json << ",\"version\": \"" << jEscape(content.version.empty() ? "0" : content.version) << "\"";
    if (content.lifetime > 0)
        json << ",\"lifetime\": " << content.lifetime;
    if (!content.invitee.empty())
        json << ",\"invitee\": \"" << jEscape(content.invitee) << "\"";
    auto capJson = callCapabilitiesToJson(content.capabilities);
    if (capJson != "{}")
        json << ",\"capabilities\": " << capJson;
    json << "}";
    return json.str();
}

// ====================================================================
// Builders — CallAnswerContent (m.call.answer)
// ====================================================================

// Original Kotlin: CallAnswerContent.kt / CallSignalingService.kt
std::string buildCallAnswerContent(const std::string& callId, const std::string& sdpAnswer,
    const CallCapabilities& capabilities) {
    std::ostringstream json;
    json << "{\"call_id\": \"" << jEscape(callId) << "\"";
    json << ",\"answer\": {\"type\": \"answer\", \"sdp\": \"" << jEscape(sdpAnswer) << "\"}";
    json << ",\"version\": \"0\"";
    auto capJson = callCapabilitiesToJson(capabilities);
    if (capJson != "{}")
        json << ",\"capabilities\": " << capJson;
    json << "}";
    return json.str();
}

std::string buildCallAnswerContent(const CallAnswerContent& content) {
    std::ostringstream json;
    json << "{\"call_id\": \"" << jEscape(content.callId) << "\"";
    if (!content.partyId.empty())
        json << ",\"party_id\": \"" << jEscape(content.partyId) << "\"";
    json << ",\"answer\": {\"type\": \"" << sdpTypeToString(content.answer.type)
         << "\", \"sdp\": \"" << jEscape(content.answer.sdp) << "\"}";
    json << ",\"version\": \"" << jEscape(content.version.empty() ? "0" : content.version) << "\"";
    auto capJson = callCapabilitiesToJson(content.capabilities);
    if (capJson != "{}")
        json << ",\"capabilities\": " << capJson;
    json << "}";
    return json.str();
}

// ====================================================================
// Builders — CallHangupContent (m.call.hangup)
// ====================================================================

// Original Kotlin: CallHangupContent.kt / CallSignalingService.kt
std::string buildCallHangupContent(const std::string& callId,
    EndCallReason reason, const std::string& partyId) {
    std::ostringstream json;
    json << "{\"call_id\": \"" << jEscape(callId) << "\"";
    if (!partyId.empty())
        json << ",\"party_id\": \"" << jEscape(partyId) << "\"";
    json << ",\"version\": \"0\"";
    if (reason != EndCallReason::USER_HANGUP)
        json << ",\"reason\": \"" << endCallReasonToString(reason) << "\"";
    json << "}";
    return json.str();
}

std::string buildCallHangupContent(const std::string& callId, const std::string& reason) {
    std::ostringstream json;
    json << "{\"call_id\": \"" << jEscape(callId) << "\"";
    json << ",\"version\": \"0\"";
    if (!reason.empty())
        json << ",\"reason\": \"" << jEscape(reason) << "\"";
    json << "}";
    return json.str();
}

std::string buildCallHangupContent(const CallHangupContent& content) {
    std::ostringstream json;
    json << "{\"call_id\": \"" << jEscape(content.callId) << "\"";
    if (!content.partyId.empty())
        json << ",\"party_id\": \"" << jEscape(content.partyId) << "\"";
    json << ",\"version\": \"" << jEscape(content.version.empty() ? "0" : content.version) << "\"";
    if (content.reason != EndCallReason::USER_HANGUP)
        json << ",\"reason\": \"" << endCallReasonToString(content.reason) << "\"";
    json << "}";
    return json.str();
}

// ====================================================================
// Builders — CallRejectContent (m.call.reject)
// ====================================================================

// Original Kotlin: CallRejectContent.kt / CallSignalingService.kt
std::string buildCallRejectContent(const std::string& callId,
    EndCallReason reason) {
    std::ostringstream json;
    json << "{\"call_id\": \"" << jEscape(callId) << "\"";
    json << ",\"version\": \"0\"";
    if (reason != EndCallReason::USER_HANGUP)
        json << ",\"reason\": \"" << endCallReasonToString(reason) << "\"";
    json << "}";
    return json.str();
}

std::string buildCallRejectContent(const CallRejectContent& content) {
    std::ostringstream json;
    json << "{\"call_id\": \"" << jEscape(content.callId) << "\"";
    if (!content.partyId.empty())
        json << ",\"party_id\": \"" << jEscape(content.partyId) << "\"";
    json << ",\"version\": \"" << jEscape(content.version.empty() ? "0" : content.version) << "\"";
    json << ",\"reason\": \"" << endCallReasonToString(content.reason) << "\"";
    json << "}";
    return json.str();
}

// ====================================================================
// Builders — CallCandidatesContent (m.call.candidates)
// ====================================================================

// Original Kotlin: CallCandidatesContent.kt / CallSignalingService.kt
std::string buildCallCandidatesContent(const std::string& callId,
    const std::vector<CallCandidate>& candidates, const std::string& partyId) {
    std::ostringstream json;
    json << "{\"call_id\": \"" << jEscape(callId) << "\"";
    if (!partyId.empty())
        json << ",\"party_id\": \"" << jEscape(partyId) << "\"";
    json << ",\"candidates\": [";
    for (size_t i = 0; i < candidates.size(); ++i) {
        if (i > 0) json << ",";
        json << buildCallCandidate(candidates[i]);
    }
    json << "]";
    json << ",\"version\": \"0\"";
    json << "}";
    return json.str();
}

std::string buildCallCandidatesContent(const CallCandidatesContent& content) {
    std::ostringstream json;
    json << "{\"call_id\": \"" << jEscape(content.callId) << "\"";
    if (!content.partyId.empty())
        json << ",\"party_id\": \"" << jEscape(content.partyId) << "\"";
    json << ",\"candidates\": [";
    for (size_t i = 0; i < content.candidates.size(); ++i) {
        if (i > 0) json << ",";
        json << buildCallCandidate(content.candidates[i]);
    }
    json << "]";
    json << ",\"version\": \"" << jEscape(content.version.empty() ? "0" : content.version) << "\"";
    json << "}";
    return json.str();
}

// ====================================================================
// Builders — CallNegotiateContent (m.call.negotiate)
// ====================================================================

// Original Kotlin: CallNegotiateContent.kt / CallSignalingService.kt
std::string buildCallNegotiateContent(const std::string& callId,
    const std::string& sdpDescription, SdpType descType,
    int lifetimeSeconds) {
    std::ostringstream json;
    json << "{\"call_id\": \"" << jEscape(callId) << "\"";
    json << ",\"description\": {\"type\": \"" << sdpTypeToString(descType)
         << "\", \"sdp\": \"" << jEscape(sdpDescription) << "\"}";
    json << ",\"version\": \"0\"";
    json << ",\"lifetime\": " << lifetimeSeconds;
    json << "}";
    return json.str();
}

std::string buildCallNegotiateContent(const CallNegotiateContent& content) {
    std::ostringstream json;
    json << "{\"call_id\": \"" << jEscape(content.callId) << "\"";
    if (!content.partyId.empty())
        json << ",\"party_id\": \"" << jEscape(content.partyId) << "\"";
    json << ",\"description\": {\"type\": \"" << sdpTypeToString(content.description.type)
         << "\", \"sdp\": \"" << jEscape(content.description.sdp) << "\"}";
    json << ",\"version\": \"" << jEscape(content.version.empty() ? "0" : content.version) << "\"";
    json << ",\"lifetime\": " << content.lifetime;
    json << "}";
    return json.str();
}

// ====================================================================
// Builders — CallSelectAnswerContent (m.call.select_answer)
// ====================================================================

// Original Kotlin: CallSelectAnswerContent.kt / CallSignalingService.kt
std::string buildCallSelectAnswerContent(const std::string& callId,
    const std::string& selectedPartyId) {
    std::ostringstream json;
    json << "{\"call_id\": \"" << jEscape(callId) << "\"";
    json << ",\"selected_party_id\": \"" << jEscape(selectedPartyId) << "\"";
    json << ",\"version\": \"0\"";
    json << "}";
    return json.str();
}

std::string buildCallSelectAnswerContent(const CallSelectAnswerContent& content) {
    std::ostringstream json;
    json << "{\"call_id\": \"" << jEscape(content.callId) << "\"";
    if (!content.partyId.empty())
        json << ",\"party_id\": \"" << jEscape(content.partyId) << "\"";
    json << ",\"selected_party_id\": \"" << jEscape(content.selectedPartyId) << "\"";
    json << ",\"version\": \"" << jEscape(content.version.empty() ? "0" : content.version) << "\"";
    json << "}";
    return json.str();
}

// ====================================================================
// Builders — CallReplacesContent (m.call.replaces)
// ====================================================================

// Original Kotlin: CallReplacesContent.kt / CallSignalingService.kt
std::string buildCallReplacesContent(const std::string& callId,
    const std::string& replacementId) {
    std::ostringstream json;
    json << "{\"call_id\": \"" << jEscape(callId) << "\"";
    if (!replacementId.empty())
        json << ",\"replacement_id\": \"" << jEscape(replacementId) << "\"";
    json << ",\"version\": \"0\"";
    json << "}";
    return json.str();
}

std::string buildCallReplacesContent(const CallReplacesContent& content) {
    std::ostringstream json;
    json << "{\"call_id\": \"" << jEscape(content.callId) << "\"";
    if (!content.partyId.empty())
        json << ",\"party_id\": \"" << jEscape(content.partyId) << "\"";
    if (!content.replacementId.empty())
        json << ",\"replacement_id\": \"" << jEscape(content.replacementId) << "\"";
    if (!content.targetRoomId.empty())
        json << ",\"target_room\": \"" << jEscape(content.targetRoomId) << "\"";
    if (!content.targetUser.id.empty()) {
        json << ",\"target_user\": {";
        json << "\"id\": \"" << jEscape(content.targetUser.id) << "\"";
        if (!content.targetUser.displayName.empty())
            json << ",\"display_name\": \"" << jEscape(content.targetUser.displayName) << "\"";
        if (!content.targetUser.avatarUrl.empty())
            json << ",\"avatar_url\": \"" << jEscape(content.targetUser.avatarUrl) << "\"";
        json << "}";
    }
    if (!content.createCall.empty())
        json << ",\"create_call\": \"" << jEscape(content.createCall) << "\"";
    if (!content.awaitCall.empty())
        json << ",\"await_call\": \"" << jEscape(content.awaitCall) << "\"";
    json << ",\"version\": \"" << jEscape(content.version.empty() ? "0" : content.version) << "\"";
    json << "}";
    return json.str();
}

// ====================================================================
// Builders — CallAssertedIdentityContent (m.call.asserted_identity)
// ====================================================================

// Original Kotlin: CallAssertedIdentityContent.kt / CallSignalingService.kt
std::string buildCallAssertedIdentityContent(const std::string& callId,
    const std::string& assertedId, const std::string& displayName,
    const std::string& avatarUrl) {
    std::ostringstream json;
    json << "{\"call_id\": \"" << jEscape(callId) << "\"";
    json << ",\"asserted_identity\": {";
    bool first = true;
    if (!assertedId.empty()) {
        json << "\"id\": \"" << jEscape(assertedId) << "\"";
        first = false;
    }
    if (!displayName.empty()) {
        if (!first) json << ",";
        json << "\"display_name\": \"" << jEscape(displayName) << "\"";
        first = false;
    }
    if (!avatarUrl.empty()) {
        if (!first) json << ",";
        json << "\"avatar_url\": \"" << jEscape(avatarUrl) << "\"";
    }
    json << "}";
    json << ",\"version\": \"0\"";
    json << "}";
    return json.str();
}

std::string buildCallAssertedIdentityContent(const CallAssertedIdentityContent& content) {
    std::ostringstream json;
    json << "{\"call_id\": \"" << jEscape(content.callId) << "\"";
    if (!content.partyId.empty())
        json << ",\"party_id\": \"" << jEscape(content.partyId) << "\"";
    json << ",\"asserted_identity\": {";
    bool first = true;
    if (!content.assertedIdentity.id.empty()) {
        json << "\"id\": \"" << jEscape(content.assertedIdentity.id) << "\"";
        first = false;
    }
    if (!content.assertedIdentity.displayName.empty()) {
        if (!first) json << ",";
        json << "\"display_name\": \"" << jEscape(content.assertedIdentity.displayName) << "\"";
        first = false;
    }
    if (!content.assertedIdentity.avatarUrl.empty()) {
        if (!first) json << ",";
        json << "\"avatar_url\": \"" << jEscape(content.assertedIdentity.avatarUrl) << "\"";
    }
    json << "}";
    json << ",\"version\": \"" << jEscape(content.version.empty() ? "0" : content.version) << "\"";
    json << "}";
    return json.str();
}

// ====================================================================
// Legacy functions (kept for backward compatibility)
// ====================================================================

CallInfo parseCallInviteLegacy(const std::string& eventContentJson, const std::string& eventId,
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
        auto descObj = extractJsonObject(eventContentJson, "description");
        if (!descObj.empty()) {
            offer.type = parseJsonStringValue(descObj, "type");
            offer.sdp  = parseJsonStringValue(descObj, "sdp");
        }
    }
    // Also try offer / answer sub-objects
    if (offer.sdp.empty()) {
        auto offerObj = extractJsonObject(eventContentJson, "offer");
        if (!offerObj.empty()) {
            offer.type = parseJsonStringValue(offerObj, "type");
            offer.sdp  = parseJsonStringValue(offerObj, "sdp");
        }
    }
    if (offer.sdp.empty()) {
        auto answerObj = extractJsonObject(eventContentJson, "answer");
        if (!answerObj.empty()) {
            offer.type = parseJsonStringValue(answerObj, "type");
            offer.sdp  = parseJsonStringValue(answerObj, "sdp");
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

std::string buildCallInviteContentLegacy(const std::string& callId, bool isVideo,
    const std::string& sdpOffer, int lifetimeSeconds) {
    std::ostringstream json;
    json << R"({"call_id": ")" << jEscape(callId) << R"(")";
    json << R"(,"offer": {"type": "offer", "sdp": ")" << jEscape(sdpOffer) << R"("})";
    if (isVideo) json << R"(,"m.video": true)";
    json << R"(,"version": "0")";
    if (lifetimeSeconds > 0)
        json << R"(,"lifetime": )" << lifetimeSeconds;
    json << "}";
    return json.str();
}

std::string buildCallAnswerContentLegacy(const std::string& callId, const std::string& sdpAnswer) {
    std::ostringstream json;
    json << R"({"call_id": ")" << jEscape(callId) << R"(")";
    json << R"(,"answer": {"type": "answer", "sdp": ")" << jEscape(sdpAnswer) << R"("})";
    json << R"(,"version": "0")";
    json << "}";
    return json.str();
}

std::string buildCallCandidatesContentLegacy(const std::string& callId,
    const std::vector<IceCandidate>& candidates) {
    std::ostringstream json;
    json << R"({"call_id": ")" << jEscape(callId) << R"(")";
    json << R"(,"candidates": [)";
    for (size_t i = 0; i < candidates.size(); ++i) {
        if (i > 0) json << ",";
        json << R"({"sdpMid": ")" << jEscape(candidates[i].sdpMid) << R"(")";
        json << R"(,"sdpMLineIndex": )" << candidates[i].sdpMLineIndex;
        json << R"(,"candidate": ")" << jEscape(candidates[i].candidate) << R"(")" << "}";
    }
    json << R"(],"version": "0")";
    json << "}";
    return json.str();
}

std::string buildCallHangupContentLegacy(const std::string& callId, const std::string& reason) {
    std::ostringstream json;
    json << R"({"call_id": ")" << jEscape(callId) << R"(")";
    if (!reason.empty()) json << R"(,"reason": ")" << jEscape(reason) << R"(")";
    json << R"(,"version": "0")";
    json << "}";
    return json.str();
}

// ====================================================================
// Utility Functions
// ====================================================================

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
    if (eventContentJson.find("\"m.call.negotiate\"") != std::string::npos) return "negotiate";
    if (eventContentJson.find("\"m.call.select_answer\"") != std::string::npos) return "select_answer";
    if (eventContentJson.find("\"m.call.replaces\"") != std::string::npos) return "replaces";
    if (eventContentJson.find("\"m.call.asserted_identity\"") != std::string::npos) return "asserted_identity";
    return "unknown";
}

} // namespace progressive
