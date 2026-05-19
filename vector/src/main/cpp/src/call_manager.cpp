#include "progressive/call_manager.hpp"
#include <sstream>
#include <algorithm>
#include <ctime>

namespace progressive {

// ====== Call State/Reason String Conversions ======

const char* callManagerStateToString(CallManagerCallState state) {
    switch (state) {
        case CallManagerCallState::IDLE: return "idle";
        case CallManagerCallState::INVITING: return "inviting";
        case CallManagerCallState::RINGING: return "ringing";
        case CallManagerCallState::CONNECTING: return "connecting";
        case CallManagerCallState::CONNECTED: return "connected";
        case CallManagerCallState::ON_HOLD: return "on_hold";
        case CallManagerCallState::ENDED: return "ended";
        case CallManagerCallState::REJECTED: return "rejected";
        case CallManagerCallState::TIMED_OUT: return "timed_out";
        case CallManagerCallState::BUSY: return "busy";
    }
    return "unknown";
}

CallManagerCallState callManagerStateFromString(const std::string& s) {
    if (s == "inviting") return CallManagerCallState::INVITING;
    if (s == "ringing") return CallManagerCallState::RINGING;
    if (s == "connecting") return CallManagerCallState::CONNECTING;
    if (s == "connected") return CallManagerCallState::CONNECTED;
    if (s == "on_hold") return CallManagerCallState::ON_HOLD;
    if (s == "ended") return CallManagerCallState::ENDED;
    if (s == "rejected") return CallManagerCallState::REJECTED;
    if (s == "timed_out") return CallManagerCallState::TIMED_OUT;
    if (s == "busy") return CallManagerCallState::BUSY;
    return CallManagerCallState::IDLE;
}

const char* callManagerEndReasonToString(CallManagerEndReason reason) {
    switch (reason) {
        case CallManagerEndReason::USER_HUNG_UP: return "user_hung_up";
        case CallManagerEndReason::REMOTE_HUNG_UP: return "remote_hung_up";
        case CallManagerEndReason::REJECTED: return "rejected";
        case CallManagerEndReason::TIMEOUT: return "timeout";
        case CallManagerEndReason::BUSY: return "busy";
        case CallManagerEndReason::INVITE_EXPIRED: return "invite_expired";
        case CallManagerEndReason::ICE_FAILED: return "ice_failed";
        case CallManagerEndReason::NETWORK_ERROR: return "network_error";
        default: return "unknown";
    }
}

CallManagerEndReason callManagerEndReasonFromString(const std::string& s) {
    if (s == "user_hung_up") return CallManagerEndReason::USER_HUNG_UP;
    if (s == "remote_hung_up") return CallManagerEndReason::REMOTE_HUNG_UP;
    if (s == "rejected") return CallManagerEndReason::REJECTED;
    if (s == "timeout") return CallManagerEndReason::TIMEOUT;
    if (s == "busy") return CallManagerEndReason::BUSY;
    if (s == "invite_expired") return CallManagerEndReason::INVITE_EXPIRED;
    if (s == "ice_failed") return CallManagerEndReason::ICE_FAILED;
    if (s == "network_error") return CallManagerEndReason::NETWORK_ERROR;
    return CallManagerEndReason::UNKNOWN;
}

// ====== JSON helpers (local) ======

static std::string extractStr(const std::string& json, const std::string& key) {
    auto pp = json.find("\"" + key + "\"");
    if (pp == std::string::npos) return "";
    pp = json.find('"', pp + key.size() + 2);
    if (pp == std::string::npos) return "";
    pp++;
    size_t e = pp;
    while (e < json.size() && json[e] != '"') e++;
    return json.substr(pp, e - pp);
}

static int64_t extractInt(const std::string& json, const std::string& key) {
    auto pp = json.find("\"" + key + "\"");
    if (pp == std::string::npos) return 0;
    pp = json.find(':', pp);
    if (pp == std::string::npos) return 0;
    pp++;
    while (pp < json.size() && (json[pp] == ' ' || json[pp] == '\t')) pp++;
    int64_t v = 0;
    while (pp < json.size() && json[pp] >= '0' && json[pp] <= '9') { v=v*10+(json[pp]-'0'); pp++; }
    return v;
}

// ====== Call Event Type Parsing ======

CallEventType parseCallEventType(const std::string& eventType) {
    if (eventType == "m.call.invite") return CallEventType::INVITE;
    if (eventType == "m.call.answer") return CallEventType::ANSWER;
    if (eventType == "m.call.hangup") return CallEventType::HANGUP;
    if (eventType == "m.call.reject") return CallEventType::REJECT;
    if (eventType == "m.call.candidates") return CallEventType::CANDIDATES;
    if (eventType == "m.call.negotiate") return CallEventType::NEGOTIATE;
    if (eventType == "m.call.select_answer") return CallEventType::SELECT_ANSWER;
    return CallEventType::INVITE; // fallback
}

// ====== SDP Parsing ======

SdpSession parseSdp(const std::string& sdpText, const std::string& type) {
    SdpSession s;
    s.sdp = sdpText;
    s.type = type;
    s.valid = !sdpText.empty();

    // Parse lines
    std::istringstream iss(sdpText);
    std::string line;
    while (std::getline(iss, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();

        if (line.rfind("o=", 0) == 0) {
            // o=- <session_id> <version> IN IP4 <ip>
            size_t pos = 2; // skip "o="
            while (pos < line.size() && line[pos] == ' ') pos++;
            // Skip username
            auto sp = line.find(' ', pos);
            if (sp != std::string::npos) {
                s.sessionId = line.substr(sp + 1, line.find(' ', sp + 1) - sp - 1);
            }
        } else if (line.rfind("a=fingerprint:", 0) == 0) {
            // a=fingerprint:sha-256 <hash>
            auto colon = line.find(':', 2);
            if (colon != std::string::npos) {
                s.hash = line.substr(2, colon - 2);
                s.fingerprint = line.substr(colon + 1);
                // Trim
                s.fingerprint.erase(0, s.fingerprint.find_first_not_of(" \t"));
            }
        } else if (line.rfind("a=setup:", 0) == 0) {
            s.setup = line.substr(8);
        } else if (line.rfind("a=ice-ufrag:", 0) == 0) {
            s.iceUfrag.push_back(line.substr(12));
        } else if (line.rfind("a=ice-pwd:", 0) == 0) {
            s.icePwd.push_back(line.substr(10));
        } else if (line.rfind("a=candidate:", 0) == 0) {
            s.candidates.push_back(line.substr(12));
        } else if (line.rfind("m=audio", 0) == 0) {
            s.hasAudio = true;
        } else if (line.rfind("m=video", 0) == 0) {
            s.hasVideo = true;
        }
    }

    return s;
}

std::string sdpToJson(const SdpSession& sdp) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) {
            if (c == '"') out += "\\\"";
            else if (c == '\n') out += "\\n";
            else if (c == '\r') out += "\\r";
            else out += c;
        }
        return out;
    };

    std::ostringstream os;
    os << R"({"type":")" << sdp.type << R"(")";
    os << R"(,"sdp":")" << esc(sdp.sdp) << R"(")";
    if (!sdp.sessionId.empty()) os << R"(,"session_id":")" << sdp.sessionId << R"(")";
    if (!sdp.fingerprint.empty()) os << R"(,"fingerprint":")" << sdp.fingerprint << R"(")";
    os << R"(,"has_audio":)" << (sdp.hasAudio ? "true" : "false");
    os << R"(,"has_video":)" << (sdp.hasVideo ? "true" : "false");
    os << R"(,"valid":)" << (sdp.valid ? "true" : "false");
    os << "}";
    return os.str();
}

// ====== ICE Candidate Parsing ======

ParsedIceCandidate parseParsedIceCandidateLine(const std::string& candidateLine, const std::string& sdpMid, int sdpMLineIndex) {
    ParsedIceCandidate ice;
    ice.sdpMid = sdpMid;
    ice.sdpMLineIndex = sdpMLineIndex;
    ice.candidate = candidateLine;

    // Parse tokens: foundation component-id transport priority ip port typ type [raddr rport [tcptype]]
    std::istringstream iss(candidateLine);
    std::string token;
    int idx = 0;

    while (iss >> token) {
        switch (idx) {
            case 0: ice.foundation = token; break;
            case 1: ice.component = std::stoi(token); break;
            case 2: ice.transport = token; break;
            case 3: ice.priority = static_cast<uint32_t>(std::stoul(token)); break;
            case 4: ice.ip = token; break;
            case 5: ice.port = std::stoi(token); break;
            case 6: /* "typ" keyword */ break;
            case 7: ice.type = token; break;
            case 8: /* raddr */ break;
            case 9: /* rport */ break;
            case 10: ice.relayProtocol = token; break;
        }
        idx++;
    }

    return ice;
}

// ====== Call Notification ======

CallNotification formatCallNotification(const CallSession& call) {
    CallNotification notif;
    notif.isVideo = call.type == CallType::VIDEO;
    notif.state = call.state;
    notif.timestampMs = call.createdAtMs;

    std::string caller = call.callerName.empty() ? call.callerId : call.callerName;

    if (call.isIncoming && call.state == CallManagerCallState::RINGING) {
        notif.title = caller + " is calling...";
        notif.body = call.type == CallType::VIDEO ? "Video call" : "Voice call";
    } else if (call.state == CallManagerCallState::CONNECTED) {
        notif.title = "Call with " + caller;
        notif.body = call.type == CallType::VIDEO ? "Video call in progress" : "Voice call in progress";
    } else if (call.state == CallManagerCallState::ENDED) {
        notif.title = "Call ended";
        notif.body = "Call with " + caller + " ended";
    } else if (call.state == CallManagerCallState::REJECTED) {
        notif.title = "Call declined";
        notif.body = caller + " declined the call";
    } else if (call.state == CallManagerCallState::TIMED_OUT) {
        notif.title = "Missed call";
        notif.body = "Missed " + std::string(call.type == CallType::VIDEO ? "video" : "voice") + " call from " + caller;
    }

    return notif;
}

// ====== CallManager Implementation ======

CallManager::CallManager() {}

std::string CallManager::generateCallId() const {
    auto now = static_cast<int64_t>(std::time(nullptr));
    std::ostringstream id;
    id << "call_" << now << "_" << (calls_.size() + 1);
    return id.str();
}

int64_t CallManager::nowMs() const {
    return static_cast<int64_t>(std::time(nullptr)) * 1000;
}

bool CallManager::isValidStateTransition(CallManagerCallState from, CallManagerCallState to) const {
    // Allow only valid transitions
    switch (from) {
        case CallManagerCallState::IDLE:
            return to == CallManagerCallState::INVITING || to == CallManagerCallState::RINGING;
        case CallManagerCallState::INVITING:
            return to == CallManagerCallState::CONNECTING || to == CallManagerCallState::ENDED ||
                   to == CallManagerCallState::REJECTED || to == CallManagerCallState::TIMED_OUT;
        case CallManagerCallState::RINGING:
            return to == CallManagerCallState::CONNECTING || to == CallManagerCallState::REJECTED ||
                   to == CallManagerCallState::TIMED_OUT;
        case CallManagerCallState::CONNECTING:
            return to == CallManagerCallState::CONNECTED || to == CallManagerCallState::ENDED;
        case CallManagerCallState::CONNECTED:
            return to == CallManagerCallState::ENDED || to == CallManagerCallState::ON_HOLD;
        case CallManagerCallState::ON_HOLD:
            return to == CallManagerCallState::CONNECTED || to == CallManagerCallState::ENDED;
        case CallManagerCallState::ENDED:
        case CallManagerCallState::REJECTED:
        case CallManagerCallState::TIMED_OUT:
        case CallManagerCallState::BUSY:
            return to == CallManagerCallState::IDLE; // Allow cleanup
    }
    return false;
}

CallSession* CallManager::findCall(const std::string& callId) {
    for (auto& c : calls_) {
        if (c.callId == callId) return &c;
    }
    return nullptr;
}

const CallSession* CallManager::findCall(const std::string& callId) const {
    for (const auto& c : calls_) {
        if (c.callId == callId) return &c;
    }
    return nullptr;
}

// ====== Call Lifecycle ======

std::string CallManager::startOutgoingCall(const std::string& roomId, const std::string& calleeId,
                                            const std::string& calleeName, CallType type,
                                            const std::string& sdpOffer, std::string& error) {
    // Check if room already has an active call
    if (isRoomInCall(roomId)) {
        error = "Room already has an active call";
        return "";
    }

    CallSession call;
    call.callId = generateCallId();
    call.roomId = roomId;
    call.calleeId = calleeId;
    call.calleeName = calleeName;
    call.type = type;
    call.state = CallManagerCallState::INVITING;
    call.isIncoming = false;
    call.createdAtMs = nowMs();
    call.startedAtMs = call.createdAtMs;
    call.localSdp = parseSdp(sdpOffer, "offer");

    calls_.push_back(call);

    // Build invite event content
    std::ostringstream os;
    os << R"({"call_id":")" << call.callId << R"(")";
    os << R"(,"offer":{)" << R"("type":"offer","sdp":")" << sdpOffer << R"("})";
    os << R"(,"version":1)";
    os << R"(,"lifetime":)" << call.inviteLifetimeSec * 1000;
    os << "}";
    return os.str();
}

std::string CallManager::handleIncomingCall(const std::string& callId, const std::string& roomId,
                                             const std::string& callerId, const std::string& callerName,
                                             CallType type, const std::string& sdpOffer,
                                             int inviteLifetimeSec) {
    // Check if already exists
    auto* existing = findCall(callId);
    if (existing) return callId;

    // Check if room has active call
    if (isRoomInCall(roomId)) {
        // Busy — can't accept another call in same room
    }

    CallSession call;
    call.callId = callId;
    call.roomId = roomId;
    call.callerId = callerId;
    call.callerName = callerName;
    call.type = type;
    call.state = CallManagerCallState::RINGING;
    call.isIncoming = true;
    call.createdAtMs = nowMs();
    call.startedAtMs = call.createdAtMs;
    call.remoteSdp = parseSdp(sdpOffer, "offer");
    call.inviteLifetimeSec = inviteLifetimeSec;

    calls_.push_back(call);
    return callId;
}

std::string CallManager::answerCall(const std::string& callId, const std::string& sdpAnswer, std::string& error) {
    auto* call = findCall(callId);
    if (!call) {
        error = "Call not found: " + callId;
        return "";
    }
    if (call->state != CallManagerCallState::RINGING) {
        error = "Call is not ringing";
        return "";
    }

    call->state = CallManagerCallState::CONNECTING;
    call->answeredAtMs = nowMs();
    call->localSdp = parseSdp(sdpAnswer, "answer");

    std::ostringstream os;
    os << R"({"call_id":")" << callId << R"(")";
    os << R"(,"answer":{)" << R"("type":"answer","sdp":")" << sdpAnswer << R"("})";
    os << R"(,"version":1)";
    os << "}";
    return os.str();
}

std::string CallManager::rejectCall(const std::string& callId, const std::string& reason) {
    auto* call = findCall(callId);
    if (!call) return "";

    call->state = CallManagerCallState::REJECTED;
    call->endedAtMs = nowMs();

    std::ostringstream os;
    os << R"({"call_id":")" << callId << R"(")";
    os << R"(,"reason":")" << reason << R"(")";
    os << R"(,"version":1)";
    os << "}";
    return os.str();
}

std::string CallManager::hangupCall(const std::string& callId, CallManagerEndReason reason,
                                     const std::string& reasonText) {
    auto* call = findCall(callId);
    if (!call) return "";

    call->state = CallManagerCallState::ENDED;
    call->endedAtMs = nowMs();
    call->endReason = reason;
    call->endReasonText = reasonText;

    if (call->answeredAtMs > 0) {
        call->durationSeconds = static_cast<int>((call->endedAtMs - call->answeredAtMs) / 1000);
    }

    std::ostringstream os;
    os << R"({"call_id":")" << callId << R"(")";
    os << R"(,"reason":")" << callManagerEndReasonToString(reason) << R"(")";
    os << R"(,"version":1)";
    os << "}";
    return os.str();
}

std::string CallManager::timeoutCall(const std::string& callId) {
    auto* call = findCall(callId);
    if (!call) return "";

    call->state = CallManagerCallState::TIMED_OUT;
    call->endedAtMs = nowMs();
    call->endReason = CallManagerEndReason::TIMEOUT;

    std::ostringstream os;
    os << R"({"call_id":")" << callId << R"(")";
    os << R"(,"reason":"timeout","version":1)";
    os << "}";
    return os.str();
}

// ====== ICE Candidates ======

void CallManager::addLocalParsedIceCandidate(const std::string& callId, const ParsedIceCandidate& candidate) {
    auto* call = findCall(callId);
    if (call) call->localCandidates.push_back(candidate);
}

void CallManager::addRemoteParsedIceCandidate(const std::string& callId, const ParsedIceCandidate& candidate) {
    auto* call = findCall(callId);
    if (call) call->remoteCandidates.push_back(candidate);
}

std::string CallManager::buildCandidatesEvent(const std::string& callId,
                                               const std::vector<ParsedIceCandidate>& candidates) {
    std::ostringstream os;
    os << R"({"call_id":")" << callId << R"(")";
    os << R"(,"candidates":[)";
    for (size_t i = 0; i < candidates.size(); i++) {
        if (i > 0) os << ",";
        os << R"({"sdpMid":")" << candidates[i].sdpMid
           << R"(","sdpMLineIndex":)" << candidates[i].sdpMLineIndex
           << R"(,"candidate":")" << candidates[i].candidate << R"("})";
    }
    os << R"(],"version":1)";
    os << "}";
    return os.str();
}

std::vector<ParsedIceCandidate> CallManager::getRemoteCandidates(const std::string& callId) const {
    auto* call = findCall(callId);
    if (!call) return {};
    return call->remoteCandidates;
}

// ====== Call State Management ======

void CallManager::setCallConnected(const std::string& callId) {
    auto* call = findCall(callId);
    if (call && call->state == CallManagerCallState::CONNECTING) {
        call->state = CallManagerCallState::CONNECTED;
    }
}

void CallManager::setCallConnecting(const std::string& callId) {
    auto* call = findCall(callId);
    if (call) call->state = CallManagerCallState::CONNECTING;
}

void CallManager::setMuted(const std::string& callId, bool muted) {
    auto* call = findCall(callId);
    if (call) call->isMuted = muted;
}

void CallManager::setVideoEnabled(const std::string& callId, bool enabled) {
    auto* call = findCall(callId);
    if (call) call->isVideoOn = enabled;
}

void CallManager::setRemoteVideo(const std::string& callId, bool hasVideo) {
    auto* call = findCall(callId);
    if (call) call->hasRemoteVideo = hasVideo;
}

// ====== Call Queries ======

bool CallManager::getCall(const std::string& callId, CallSession& out) const {
    auto* call = findCall(callId);
    if (!call) return false;
    out = *call;
    return true;
}

bool CallManager::getActiveCall(CallSession& out) const {
    for (const auto& c : calls_) {
        if (c.state == CallManagerCallState::CONNECTED) {
            out = c;
            return true;
        }
    }
    return false;
}

bool CallManager::getIncomingCall(CallSession& out) const {
    for (const auto& c : calls_) {
        if (c.state == CallManagerCallState::RINGING && c.isIncoming) {
            out = c;
            return true;
        }
    }
    return false;
}

std::vector<CallSession> CallManager::getRoomCalls(const std::string& roomId) const {
    std::vector<CallSession> result;
    for (const auto& c : calls_) {
        if (c.roomId == roomId) result.push_back(c);
    }
    return result;
}

bool CallManager::isRoomInCall(const std::string& roomId) const {
    for (const auto& c : calls_) {
        if (c.roomId == roomId &&
            (c.state == CallManagerCallState::INVITING || c.state == CallManagerCallState::RINGING ||
             c.state == CallManagerCallState::CONNECTING || c.state == CallManagerCallState::CONNECTED)) {
            return true;
        }
    }
    return false;
}

int CallManager::activeCallCount() const {
    int count = 0;
    for (const auto& c : calls_) {
        if (c.state == CallManagerCallState::CONNECTED || c.state == CallManagerCallState::CONNECTING) count++;
    }
    return count;
}

// ====== Call Duration ======

int CallManager::getCallDuration(const std::string& callId) const {
    auto* call = findCall(callId);
    if (!call || call->answeredAtMs <= 0) return 0;
    return static_cast<int>((nowMs() - call->answeredAtMs) / 1000);
}

std::string CallManager::formatCallDuration(int seconds) const {
    if (seconds < 0) seconds = 0;
    int hours = seconds / 3600;
    int mins = (seconds % 3600) / 60;
    int secs = seconds % 60;

    std::ostringstream os;
    if (hours > 0) {
        os << hours << ":" << (mins < 10 ? "0" : "") << mins << ":" << (secs < 10 ? "0" : "") << secs;
    } else {
        os << (mins < 10 ? "0" : "") << mins << ":" << (secs < 10 ? "0" : "") << secs;
    }
    return os.str();
}

// ====== Serialization ======

std::string CallManager::callToJson(const CallSession& call) const {
    std::ostringstream os;
    os << R"({"call_id":")" << call.callId
       << R"(","room_id":")" << call.roomId
       << R"(","caller_id":")" << call.callerId
       << R"(","caller_name":")" << call.callerName
       << R"(","callee_name":")" << call.calleeName
       << R"(","type":")" << (call.type == CallType::VIDEO ? "video" : "voice")
       << R"(","state":")" << callManagerStateToString(call.state)
       << R"(","is_incoming":)" << (call.isIncoming ? "true" : "false")
       << R"(,"is_muted":)" << (call.isMuted ? "true" : "false")
       << R"(,"is_video_on":)" << (call.isVideoOn ? "true" : "false")
       << R"(,"duration":)" << call.durationSeconds
       << R"(,"duration_formatted":")" << formatCallDuration(call.durationSeconds) << R"(")";
    if (call.createdAtMs > 0) os << R"(,"created_at":)" << call.createdAtMs;
    if (call.answeredAtMs > 0) os << R"(,"answered_at":)" << call.answeredAtMs;
    if (!call.endReasonText.empty()) os << R"(,"end_reason":")" << call.endReasonText << R"(")";

    // SDP info
    os << R"(,"has_local_sdp":)" << (call.localSdp.valid ? "true" : "false");
    os << R"(,"has_remote_sdp":)" << (call.remoteSdp.valid ? "true" : "false");
    os << R"(,"ice_candidates_local":)" << static_cast<int>(call.localCandidates.size());
    os << R"(,"ice_candidates_remote":)" << static_cast<int>(call.remoteCandidates.size());

    // Remote video
    os << R"(,"has_remote_video":)" << (call.hasRemoteVideo ? "true" : "false");
    os << "}";
    return os.str();
}

std::string CallManager::allCallsToJson() const {
    std::ostringstream os;
    os << "[";
    for (size_t i = 0; i < calls_.size(); i++) {
        if (i > 0) os << ",";
        os << callToJson(calls_[i]);
    }
    os << "]";
    return os.str();
}

// ====== Call Event History ======

CallEvent CallManager::parseCallEvent(const std::string& eventType, const std::string& contentJson,
                                       const std::string& senderId, int64_t timestampMs) {
    CallEvent event;
    event.type = parseCallEventType(eventType);
    event.callId = extractStr(contentJson, "call_id");
    event.senderId = senderId;
    event.contentJson = contentJson;
    event.timestampMs = timestampMs;
    event.version = static_cast<int>(extractInt(contentJson, "version"));
    event.valid = !event.callId.empty();
    return event;
}

std::string CallManager::formatCallEvent(const CallEvent& event, const std::string& senderDisplayName) {
    std::ostringstream os;
    std::string sender = senderDisplayName.empty() ? event.senderId : senderDisplayName;

    switch (event.type) {
        case CallEventType::INVITE:
            os << sender << " started a call";
            break;
        case CallEventType::ANSWER:
            os << sender << " answered the call";
            break;
        case CallEventType::HANGUP:
            os << "Call ended";
            break;
        case CallEventType::REJECT:
            os << sender << " declined the call";
            break;
        case CallEventType::CANDIDATES:
            os << "Call connection updated";
            break;
        default:
            os << "Call event";
    }

    return os.str();
}

// ====== CallSignalingState ======

static std::string managerExtractStr(const std::string& json, const std::string& key) {
    auto pp = json.find("\"" + key + "\"");
    if (pp == std::string::npos) return "";
    pp = json.find('"', pp + key.size() + 2);
    if (pp == std::string::npos) return "";
    pp++;
    size_t e = pp;
    while (e < json.size() && json[e] != '"') e++;
    return json.substr(pp, e - pp);
}

static int managerExtractInt(const std::string& json, const std::string& key) {
    auto pp = json.find("\"" + key + "\"");
    if (pp == std::string::npos) return 0;
    pp = json.find(':', pp);
    if (pp == std::string::npos) return 0;
    pp++;
    while (pp < json.size() && (json[pp] == ' ' || json[pp] == '\t')) pp++;
    int v = 0;
    while (pp < json.size() && json[pp] >= '0' && json[pp] <= '9') { v = v * 10 + (json[pp] - '0'); pp++; }
    return v;
}

// Original Kotlin: have-local-offer / have-remote-offer signaling states
const char* callSignalingStateToString(CallSignalingState s) {
    switch (s) {
        case CallSignalingState::STABLE:            return "stable";
        case CallSignalingState::HAVE_LOCAL_OFFER:  return "have-local-offer";
        case CallSignalingState::HAVE_REMOTE_OFFER: return "have-remote-offer";
        case CallSignalingState::CLOSED:             return "closed";
    }
    return "stable";
}

CallSignalingState callSignalingStateFromString(const std::string& s) {
    if (s == "stable")              return CallSignalingState::STABLE;
    if (s == "have-local-offer")    return CallSignalingState::HAVE_LOCAL_OFFER;
    if (s == "have-remote-offer")   return CallSignalingState::HAVE_REMOTE_OFFER;
    if (s == "closed")              return CallSignalingState::CLOSED;
    return CallSignalingState::STABLE;
}

// ====== Group Call / Participant state enums ======

const char* groupCallStateToString(GroupCallState s) {
    switch (s) {
        case GroupCallState::CREATING: return "creating";
        case GroupCallState::ACTIVE:   return "active";
        case GroupCallState::ENDED:    return "ended";
    }
    return "creating";
}

GroupCallState groupCallStateFromString(const std::string& s) {
    if (s == "creating") return GroupCallState::CREATING;
    if (s == "active")   return GroupCallState::ACTIVE;
    if (s == "ended")    return GroupCallState::ENDED;
    return GroupCallState::CREATING;
}

const char* callParticipantStateToString(CallParticipantState s) {
    switch (s) {
        case CallParticipantState::CONNECTING:    return "connecting";
        case CallParticipantState::CONNECTED:     return "connected";
        case CallParticipantState::DISCONNECTED:  return "disconnected";
        case CallParticipantState::LEFT:          return "left";
    }
    return "connecting";
}

CallParticipantState callParticipantStateFromString(const std::string& s) {
    if (s == "connecting")    return CallParticipantState::CONNECTING;
    if (s == "connected")     return CallParticipantState::CONNECTED;
    if (s == "disconnected")  return CallParticipantState::DISCONNECTED;
    if (s == "left")          return CallParticipantState::LEFT;
    return CallParticipantState::CONNECTING;
}

// ====== Call Event Processing ======
//
// Original Kotlin: CallSignalingHandler.kt / CallEventProcessor.kt

// Original Kotlin: CallSignalingHandler.handleCallInviteEvent()
std::string CallManager::processCallInvite(const std::string& roomId, const std::string& senderId,
                                             const std::string& contentJson, int64_t timestampMs) {
    // Parse call ID and check if it's a new call
    std::string callId = managerExtractStr(contentJson, "call_id");
    if (callId.empty()) return "";

    // Check if already known
    if (findCall(callId)) return callId;

    // Check if room already has active call
    if (isRoomInCall(roomId)) return callId; // Could send busy

    // Determine call type from SDP
    bool isVideo = contentJson.find("m=video") != std::string::npos;

    // Parse lifetime
    int lifetime = managerExtractInt(contentJson, "lifetime");
    if (lifetime <= 0) lifetime = 120000;
    int lifetimeSec = lifetime / 1000;

    // Extract the SDP offer string
    std::string sdpOffer;
    auto offerPos = contentJson.find("\"offer\"");
    if (offerPos != std::string::npos) {
        auto sdpPos = contentJson.find("\"sdp\"", offerPos);
        if (sdpPos != std::string::npos) {
            sdpOffer = managerExtractStr(contentJson.substr(sdpPos > 10 ? sdpPos - 10 : 0, 1000), "sdp");
        }
    }

    return handleIncomingCall(callId, roomId, senderId, "", isVideo ? CallType::VIDEO : CallType::VOICE,
                               sdpOffer, lifetimeSec);
}

// Original Kotlin: CallSignalingHandler.handleCallAnswerEvent()
bool CallManager::processCallAnswer(const std::string& contentJson) {
    std::string callId = managerExtractStr(contentJson, "call_id");
    if (callId.empty()) return false;

    auto* call = findCall(callId);
    if (!call) return false;
    if (call->state != CallManagerCallState::INVITING) return false;

    // Extract answer SDP
    std::string sdpAnswer;
    auto ansPos = contentJson.find("\"answer\"");
    if (ansPos != std::string::npos) {
        auto sdpPos = contentJson.find("\"sdp\"", ansPos);
        if (sdpPos != std::string::npos) {
            sdpAnswer = managerExtractStr(contentJson.substr(sdpPos > 10 ? sdpPos - 10 : 0, 1000), "sdp");
        }
    }

    call->state = CallManagerCallState::CONNECTING;
    call->answeredAtMs = nowMs();
    call->remoteSdp = parseSdp(sdpAnswer, "answer");
    return true;
}

// Original Kotlin: CallSignalingHandler.handleCallHangupEvent()
bool CallManager::processCallHangup(const std::string& contentJson) {
    std::string callId = managerExtractStr(contentJson, "call_id");
    if (callId.empty()) return false;

    auto* call = findCall(callId);
    if (!call) return false;

    std::string reasonStr = managerExtractStr(contentJson, "reason");
    auto reason = callManagerEndReasonFromString(reasonStr);
    if (reason == CallManagerEndReason::UNKNOWN) reason = CallManagerEndReason::REMOTE_HUNG_UP;

    call->state = CallManagerCallState::ENDED;
    call->endedAtMs = nowMs();
    call->endReason = reason;

    if (call->answeredAtMs > 0) {
        call->durationSeconds = static_cast<int>((call->endedAtMs - call->answeredAtMs) / 1000);
    }
    return true;
}

// Original Kotlin: CallSignalingHandler.handleCallCandidatesEvent()
int CallManager::processCallCandidates(const std::string& contentJson) {
    std::string callId = managerExtractStr(contentJson, "call_id");
    if (callId.empty()) return 0;

    auto* call = findCall(callId);
    if (!call) return 0;

    int added = 0;
    auto candArr = contentJson.find("\"candidates\"");
    if (candArr != std::string::npos) {
        candArr = contentJson.find('[', candArr);
        if (candArr != std::string::npos) {
            size_t pos = candArr + 1;
            while (pos < contentJson.size()) {
                while (pos < contentJson.size() && (contentJson[pos] == ' ' || contentJson[pos] == ',' || contentJson[pos] == '\n')) pos++;
                if (pos >= contentJson.size() || contentJson[pos] == ']') break;
                if (contentJson[pos] == '{') {
                    int depth = 1;
                    size_t start = pos;
                    pos++;
                    while (pos < contentJson.size() && depth > 0) {
                        if (contentJson[pos] == '{') depth++;
                        else if (contentJson[pos] == '}') depth--;
                        pos++;
                    }
                    std::string candJson = contentJson.substr(start, pos - start);
                    ParsedIceCandidate ice;
                    ice.sdpMid = managerExtractStr(candJson, "sdpMid");
                    ice.sdpMLineIndex = managerExtractInt(candJson, "sdpMLineIndex");
                    ice.candidate = managerExtractStr(candJson, "candidate");
                    call->remoteCandidates.push_back(ice);
                    added++;
                }
            }
        }
    }

    return added;
}

// Original Kotlin: CallSignalingHandler.handleCallNegotiateEvent()
bool CallManager::processCallNegotiate(const std::string& contentJson) {
    std::string callId = managerExtractStr(contentJson, "call_id");
    if (callId.empty()) return false;

    auto* call = findCall(callId);
    if (!call) return false;

    // Extract description SDP
    std::string descSdp;
    auto descPos = contentJson.find("\"description\"");
    if (descPos != std::string::npos) {
        auto sdpPos = contentJson.find("\"sdp\"", descPos);
        if (sdpPos != std::string::npos) {
            descSdp = managerExtractStr(contentJson.substr(sdpPos > 10 ? sdpPos - 10 : 0, 1000), "sdp");
        }
    }
    if (!descSdp.empty()) {
        std::string descType = managerExtractStr(contentJson, "type");
        if (descType.empty()) {
            auto typePos = contentJson.rfind("\"type\"", contentJson.find("\"description\""));
            if (typePos != std::string::npos) descType = managerExtractStr(contentJson.substr(typePos, 50), "type");
        }
        call->remoteSdp = parseSdp(descSdp, descType.empty() ? "offer" : descType);
    }
    return true;
}

// Original Kotlin: CallEventProcessor.shouldProcess()
bool CallManager::isCallEvent(const std::string& eventType) const {
    return eventType == "m.call.invite" ||
           eventType == "m.call.answer" ||
           eventType == "m.call.hangup" ||
           eventType == "m.call.reject" ||
           eventType == "m.call.candidates" ||
           eventType == "m.call.negotiate" ||
           eventType == "m.call.select_answer" ||
           eventType == "m.call.replaces" ||
           eventType == "m.call.asserted_identity";
}

// ====== Call Display / Hold / Resume ======

// Original Kotlin: get call display name from room or opponent
std::string CallManager::getCallDisplayName(const CallSession& call) const {
    if (!call.roomName.empty()) return call.roomName;
    if (!call.callerName.empty()) return call.callerName;
    if (!call.calleeName.empty()) return call.calleeName;
    if (!call.opponentDisplayName.empty()) return call.opponentDisplayName;
    if (call.isIncoming) return call.callerId;
    return call.calleeId;
}

// Original Kotlin: setCallOnHold()
void CallManager::setCallOnHold(const std::string& callId) {
    auto* call = findCall(callId);
    if (call && call->state == CallManagerCallState::CONNECTED) {
        call->state = CallManagerCallState::ON_HOLD;
        call->isHeld = true;
    }
}

// Original Kotlin: resumeCall()
void CallManager::resumeCall(const std::string& callId) {
    auto* call = findCall(callId);
    if (call && call->state == CallManagerCallState::ON_HOLD) {
        call->state = CallManagerCallState::CONNECTED;
        call->isHeld = false;
    }
}

// ================================================================
// CallSessionManager Implementation
//
// Original Kotlin: MxCallFactory.kt / ActiveCallHandler.kt
// ================================================================

CallSessionManager::CallSessionManager() {}

// Original Kotlin: MxCallFactory.createOutgoingCall()
std::string CallSessionManager::createCall(const std::string& roomId, const std::string& opponentUserId,
                                             const std::string& opponentName, bool isVideoCall) {
    std::string error;
    return manager_.startOutgoingCall(roomId, opponentUserId, opponentName,
                                       isVideoCall ? CallType::VIDEO : CallType::VOICE, "", error);
}

// Original Kotlin: MxCall.accept()
std::string CallSessionManager::answerCall(const std::string& callId, const std::string& sdpAnswer) {
    std::string error;
    return manager_.answerCall(callId, sdpAnswer, error);
}

// Original Kotlin: MxCall.hangUp()
std::string CallSessionManager::hangupCall(const std::string& callId, const std::string& reason) {
    return manager_.hangupCall(callId, CallManagerEndReason::USER_HUNG_UP, reason);
}

// Original Kotlin: MxCall.reject()
std::string CallSessionManager::rejectCall(const std::string& callId) {
    return manager_.rejectCall(callId, "rejected");
}

// Original Kotlin: CallListener.onCallInviteReceived()
void CallSessionManager::onCallInviteReceived(const std::string& roomId, const std::string& senderId,
                                               const std::string& senderName, const std::string& contentJson,
                                               int64_t timestampMs) {
    manager_.processCallInvite(roomId, senderId, contentJson, timestampMs);
    // Update caller name if available
    auto sessions = manager_.getRoomCalls(roomId);
    for (auto& s : sessions) {
        if (!s.callerName.empty() && s.callerName == senderName) continue;
        // Find the call this invite created — must do via manager
    }
}

// Original Kotlin: CallListener.onCallAnswerReceived()
void CallSessionManager::onCallAnswerReceived(const std::string& contentJson) {
    manager_.processCallAnswer(contentJson);
}

// Original Kotlin: CallListener.onCallHangupReceived()
void CallSessionManager::onCallHangupReceived(const std::string& contentJson) {
    manager_.processCallHangup(contentJson);
}

// Original Kotlin: ActiveCallHandler.getActiveCallsLiveData()
bool CallSessionManager::getActiveCall(CallSession& out) const {
    return manager_.getActiveCall(out);
}

// Original Kotlin: ActiveCallHandler — get calls for room
std::vector<CallSession> CallSessionManager::getCallsForRoom(const std::string& roomId) const {
    return manager_.getRoomCalls(roomId);
}

// Original Kotlin: MxCall — hold/resume
void CallSessionManager::setCallOnHold(const std::string& callId) {
    manager_.setCallOnHold(callId);
}

void CallSessionManager::resumeCall(const std::string& callId) {
    manager_.resumeCall(callId);
}

// Original Kotlin: CallEventProcessor.shouldProcess()
bool CallSessionManager::isCallEvent(const std::string& eventType) const {
    return manager_.isCallEvent(eventType);
}

} // namespace progressive
