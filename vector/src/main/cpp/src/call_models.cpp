#include "progressive/call_models.hpp"

namespace progressive {

// ==== SdpType ====
const char* sdpTypeToString(SdpType t) {
    return t == SdpType::OFFER ? "offer" : "answer";
}
SdpType sdpTypeFromString(const std::string& s) {
    if (s == "offer") return SdpType::OFFER;
    return SdpType::ANSWER;
}

// ==== EndCallReason ====
const char* endCallReasonToString(EndCallReason r) {
    switch (r) {
        case EndCallReason::ICE_FAILED: return "ice_failed";
        case EndCallReason::ICE_TIMEOUT: return "ice_timeout";
        case EndCallReason::USER_HANGUP: return "user_hangup";
        case EndCallReason::REPLACED: return "replaced";
        case EndCallReason::USER_MEDIA_FAILED: return "user_media_failed";
        case EndCallReason::INVITE_TIMEOUT: return "invite_timeout";
        case EndCallReason::UNKNOWN_ERROR: return "unknown_error";
        case EndCallReason::USER_BUSY: return "user_busy";
        case EndCallReason::ANSWERED_ELSEWHERE: return "answered_elsewhere";
    }
    return "user_hangup";
}
EndCallReason endCallReasonFromString(const std::string& s) {
    if (s == "ice_failed") return EndCallReason::ICE_FAILED;
    if (s == "ice_timeout") return EndCallReason::ICE_TIMEOUT;
    if (s == "user_hangup") return EndCallReason::USER_HANGUP;
    if (s == "replaced") return EndCallReason::REPLACED;
    if (s == "user_media_failed") return EndCallReason::USER_MEDIA_FAILED;
    if (s == "invite_timeout") return EndCallReason::INVITE_TIMEOUT;
    if (s == "unknown_error") return EndCallReason::UNKNOWN_ERROR;
    if (s == "user_busy") return EndCallReason::USER_BUSY;
    if (s == "answered_elsewhere") return EndCallReason::ANSWERED_ELSEWHERE;
    return EndCallReason::USER_HANGUP;
}

// ==== JSON Helpers ====
static std::string extractJsonString(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return "";
    pos = json.find(':', pos);
    if (pos == std::string::npos) return "";
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    if (pos >= json.size() || json[pos] != '"') return "";
    pos++;
    size_t end = pos;
    while (end < json.size() && json[end] != '"') {
        if (json[end] == '\\') end++;
        end++;
    }
    return json.substr(pos, end - pos);
}
static int extractJsonInt(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return 0;
    pos = json.find(':', pos);
    if (pos == std::string::npos) return 0;
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    if (pos >= json.size()) return 0;
    int val = 0;
    while (pos < json.size() && json[pos] >= '0' && json[pos] <= '9') {
        val = val * 10 + (json[pos] - '0');
        pos++;
    }
    return val;
}
static bool extractJsonBool(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return false;
    pos = json.find(':', pos);
    if (pos == std::string::npos) return false;
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    return json.compare(pos, 4, "true") == 0;
}
static std::string extractJsonObject(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return "";
    pos = json.find(':', pos);
    if (pos == std::string::npos) return "";
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
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

static void fillCallBase(CallSignalingContent& c, const std::string& json) {
    c.callId = extractJsonString(json, "call_id");
    c.partyId = extractJsonString(json, "party_id");
    c.version = extractJsonString(json, "version");
}

// Original Kotlin (CallInviteContent.kt:26-59)
CallInviteContent parseCallInvite(const std::string& json) {
    CallInviteContent c;
    fillCallBase(c, json);
    c.lifetime = extractJsonInt(json, "lifetime");
    c.invitee = extractJsonString(json, "invitee");

    auto offerJson = extractJsonObject(json, "offer");
    if (!offerJson.empty()) {
        c.offer.type = sdpTypeFromString(extractJsonString(offerJson, "type"));
        c.offer.sdp = extractJsonString(offerJson, "sdp");
    }

    auto capJson = extractJsonObject(json, "capabilities");
    if (!capJson.empty()) {
        c.capabilities.transferee = extractJsonBool(capJson, "m.call.transferee");
    }

    return c;
}

// Original Kotlin (CallAnswerContent.kt:26-49)
CallAnswerContent parseCallAnswer(const std::string& json) {
    CallAnswerContent c;
    fillCallBase(c, json);

    auto answerJson = extractJsonObject(json, "answer");
    if (!answerJson.empty()) {
        c.answer.type = sdpTypeFromString(extractJsonString(answerJson, "type"));
        c.answer.sdp = extractJsonString(answerJson, "sdp");
    }

    return c;
}

// Original Kotlin (CallCandidatesContent.kt:26-44)
CallCandidatesContent parseCallCandidates(const std::string& json) {
    CallCandidatesContent c;
    fillCallBase(c, json);

    auto candArr = json.find("\"candidates\"");
    if (candArr != std::string::npos) {
        candArr = json.find('[', candArr);
        if (candArr != std::string::npos) {
            size_t pos = candArr + 1;
            while (pos < json.size()) {
                while (pos < json.size() && (json[pos] == ' ' || json[pos] == ',' || json[pos] == '\n')) pos++;
                if (pos >= json.size() || json[pos] == ']') break;
                if (json[pos] == '{') {
                    int d = 1;
                    size_t start = pos;
                    pos++;
                    while (pos < json.size() && d > 0) {
                        if (json[pos] == '{') d++;
                        else if (json[pos] == '}') d--;
                        pos++;
                    }
                    std::string candJson = json.substr(start, pos - start);
                    CallCandidate cand;
                    cand.sdpMid = extractJsonString(candJson, "sdpMid");
                    cand.sdpMLineIndex = extractJsonInt(candJson, "sdpMLineIndex");
                    cand.candidate = extractJsonString(candJson, "candidate");
                    c.candidates.push_back(cand);
                }
            }
        }
    }

    return c;
}

// Original Kotlin (CallHangupContent.kt:27-46)
CallHangupContent parseCallHangup(const std::string& json) {
    CallHangupContent c;
    fillCallBase(c, json);
    c.reason = endCallReasonFromString(extractJsonString(json, "reason"));
    return c;
}

// Original Kotlin (CallRejectContent.kt:27-44)
CallRejectContent parseCallReject(const std::string& json) {
    CallRejectContent c;
    fillCallBase(c, json);
    c.reason = endCallReasonFromString(extractJsonString(json, "reason"));
    return c;
}

// Original Kotlin (CallNegotiateContent.kt:26-52)
CallNegotiateContent parseCallNegotiate(const std::string& json) {
    CallNegotiateContent c;
    fillCallBase(c, json);
    c.lifetime = extractJsonInt(json, "lifetime");

    auto descJson = extractJsonObject(json, "description");
    if (!descJson.empty()) {
        c.description.type = sdpTypeFromString(extractJsonString(descJson, "type"));
        c.description.sdp = extractJsonString(descJson, "sdp");
    }

    return c;
}

// Original Kotlin (CallReplacesContent.kt:28-73)
CallReplacesContent parseCallReplaces(const std::string& json) {
    CallReplacesContent c;
    fillCallBase(c, json);
    c.replacementId = extractJsonString(json, "replacement_id");
    c.targetRoomId = extractJsonString(json, "target_room");
    c.createCall = extractJsonString(json, "create_call");
    c.awaitCall = extractJsonString(json, "await_call");

    auto targetJson = extractJsonObject(json, "target_user");
    if (!targetJson.empty()) {
        c.targetUser.id = extractJsonString(targetJson, "id");
        c.targetUser.displayName = extractJsonString(targetJson, "display_name");
        c.targetUser.avatarUrl = extractJsonString(targetJson, "avatar_url");
    }

    return c;
}

// Original Kotlin (CallAssertedIdentityContent.kt:27-56)
CallAssertedIdentityContent parseCallAssertedIdentity(const std::string& json) {
    CallAssertedIdentityContent c;
    fillCallBase(c, json);

    auto aiJson = extractJsonObject(json, "asserted_identity");
    if (!aiJson.empty()) {
        c.assertedIdentity.id = extractJsonString(aiJson, "id");
        c.assertedIdentity.displayName = extractJsonString(aiJson, "display_name");
        c.assertedIdentity.avatarUrl = extractJsonString(aiJson, "avatar_url");
    }

    return c;
}

// ==== Parse TurnServerResponse ====
//
// Original Kotlin (TurnServerResponse.kt:26-47)

TurnServerResponse parseTurnServerResponse(const std::string& json) {
    TurnServerResponse ts;
    ts.username = extractJsonString(json, "username");
    ts.password = extractJsonString(json, "password");
    ts.ttl = extractJsonInt(json, "ttl");

    auto urisPos = json.find("\"uris\"");
    if (urisPos != std::string::npos) {
        urisPos = json.find('[', urisPos);
        if (urisPos != std::string::npos) {
            urisPos++;
            while (urisPos < json.size()) {
                while (urisPos < json.size() && (json[urisPos] == ' ' || json[urisPos] == ',' || json[urisPos] == '\n')) urisPos++;
                if (urisPos >= json.size() || json[urisPos] == ']') break;
                if (json[urisPos] == '"') {
                    urisPos++;
                    size_t end = urisPos;
                    while (end < json.size() && json[end] != '"') end++;
                    ts.uris.push_back(json.substr(urisPos, end - urisPos));
                    urisPos = end + 1;
                }
            }
        }
    }

    return ts;
}

} // namespace progressive
