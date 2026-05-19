#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>

namespace progressive {

// ================================================================
// Call Manager — full Matrix VoIP call lifecycle management
//
// Ported from Element Android:
//   org.matrix.android.sdk.api.session.call.*
//   VectorCallActivity.kt, CallViewModel.kt
//
// Implements MSC2746 (VoIP with Matrix) signaling:
//   m.call.invite → m.call.candidates → m.call.answer → m.call.hangup
//   m.call.reject (decline), m.call.negotiate (renegotiation)
//
// State machine: idle → inviting/ringing → connecting → connected → ended
// ================================================================

// ---- Call State ----

enum class CallManagerCallState {
    IDLE = 0,              // No call active
    INVITING = 1,          // Outgoing: sent invite, waiting for answer
    RINGING = 2,           // Incoming: received invite, waiting to answer
    CONNECTING = 3,        // Call answered, ICE negotiation
    CONNECTED = 4,         // Call active (media flowing)
    ON_HOLD = 5,           // Call on hold (local or remote)
    ENDED = 6,             // Call finished (hangup/reject/timeout)
    REJECTED = 7,          // Call declined
    TIMED_OUT = 8,         // Call timed out (no answer)
    BUSY = 9,              // Remote busy (reject with busy)
};

const char* callManagerStateToString(CallManagerCallState state);
CallManagerCallState callManagerStateFromString(const std::string& s);

// ---- Call Type ----

enum class CallType {
    VOICE = 0,             // Audio only
    VIDEO = 1,             // Audio + video
};

// ---- End Call Reason ----

enum class CallManagerEndReason {
    UNKNOWN = 0,
    USER_HUNG_UP = 1,       // Local user hung up
    REMOTE_HUNG_UP = 2,     // Remote user hung up
    REJECTED = 3,           // Remote user rejected
    TIMEOUT = 4,            // No answer
    BUSY = 5,               // Remote is busy
    INVITE_EXPIRED = 6,     // Invite lifetime expired
    ICE_FAILED = 7,         // ICE negotiation failed
    NETWORK_ERROR = 8,      // Network dropped
};

const char* callManagerEndReasonToString(CallManagerEndReason reason);
CallManagerEndReason callManagerEndReasonFromString(const std::string& s);

// ---- SDP (Session Description Protocol) ----

struct SdpSession {
    std::string type;            // "offer" or "answer"
    std::string sdp;             // Full SDP text
    std::string sessionId;       // o= line session ID
    std::string fingerprint;     // a=fingerprint line (DTLS)
    std::string hash;            // fingerprint hash algorithm (sha-256)
    std::string setup;           // a=setup: (active/passive/actpass)
    std::vector<std::string> iceUfrag;   // a=ice-ufrag lines
    std::vector<std::string> icePwd;     // a=ice-pwd lines
    std::vector<std::string> candidates; // a=candidate lines
    bool hasAudio = false;
    bool hasVideo = false;
    bool valid = false;
};

// Parse SDP from Matrix VoIP event content.
SdpSession parseSdp(const std::string& sdpText, const std::string& type);

// Format SDP as JSON for Matrix event.
std::string sdpToJson(const SdpSession& sdp);

// ---- ICE Candidate ----

struct ParsedIceCandidate {
    std::string sdpMid;          // Media stream ID
    int sdpMLineIndex = 0;       // Media line index
    std::string candidate;       // Full candidate string
    std::string foundation;      // 1st token: foundation
    int component = 1;           // RTP (1) or RTCP (2)
    std::string transport;       // "UDP" or "TCP"
    uint32_t priority = 0;       // Candidate priority
    std::string ip;
    int port = 0;
    std::string type;            // "host", "srflx", "prflx", "relay"
    std::string relayProtocol;   // "udp" or "tcp" (for relay)
};

// Parse ICE candidate string (RFC 5245).
// Format: foundation component transport priority ip port typ type [raddr rport generation]
ParsedIceCandidate parseParsedIceCandidateLine(const std::string& candidateLine, const std::string& sdpMid, int sdpMLineIndex);

// ---- Call Info (extended) ----

struct CallSession {
    std::string callId;
    std::string roomId;
    std::string callerId;
    std::string callerName;
    std::string calleeId;        // Who we're calling (for outgoing)
    std::string calleeName;
    std::vector<std::string> participants;
    CallType type = CallType::VOICE;
    CallManagerCallState state = CallManagerCallState::IDLE;
    bool isIncoming = false;     // true = we received the invite
    bool isMuted = false;
    bool isSpeakerOn = true;
    bool isVideoOn = false;      // Local video enabled
    int64_t createdAtMs = 0;     // When the call was created
    int64_t startedAtMs = 0;     // When the call started ringing
    int64_t answeredAtMs = 0;    // When answered
    int64_t endedAtMs = 0;       // When ended
    int durationSeconds = 0;     // Call duration (connected time)
    CallManagerEndReason endReason = CallManagerEndReason::UNKNOWN;
    std::string endReasonText;   // Human-readable reason
    SdpSession localSdp;         // Our SDP
    SdpSession remoteSdp;        // Their SDP
    std::vector<ParsedIceCandidate> localCandidates;
    std::vector<ParsedIceCandidate> remoteCandidates;
    int inviteLifetimeSec = 120; // How long to wait before timeout
    std::string roomName;        // For display
    bool hasRemoteVideo = false; // Peer is sending video
    bool isConference = false;   // Multi-party call
};

// ---- Call Event Types ----

enum class CallEventType {
    INVITE = 0,          // m.call.invite
    ANSWER = 1,          // m.call.answer
    HANGUP = 2,          // m.call.hangup
    REJECT = 3,          // m.call.reject
    CANDIDATES = 4,      // m.call.candidates
    NEGOTIATE = 5,       // m.call.negotiate
    SELECT_ANSWER = 6,   // m.call.select_answer
};

// Parse call event type from event type string.
CallEventType parseCallEventType(const std::string& eventType);

// ---- Call Event (for timeline) ----

struct CallEvent {
    CallEventType type = CallEventType::INVITE;
    std::string callId;
    std::string senderId;
    std::string senderName;
    std::string contentJson;     // Raw event content
    int64_t timestampMs = 0;
    int version = 0;             // VoIP version (0 or 1)
    bool valid = false;
};

// ---- Call Notification Formatting ----

struct CallNotification {
    std::string title;           // "Alice is calling..."
    std::string body;            // "Voice call" or "Video call"
    bool isVideo = false;
    CallManagerCallState state = CallManagerCallState::RINGING;
    int64_t timestampMs = 0;
};

// Format a call notification for system notification.
CallNotification formatCallNotification(const CallSession& call);

// ---- Call Manager ----

class CallManager {
public:
    CallManager();

    // ====== Call Lifecycle ======

    // Create a new outgoing call.
    // Returns the invite event content JSON.
    std::string startOutgoingCall(const std::string& roomId, const std::string& calleeId,
                                   const std::string& calleeName, CallType type,
                                   const std::string& sdpOffer, std::string& error);

    // Handle an incoming call invite.
    // Returns the call ID.
    std::string handleIncomingCall(const std::string& callId, const std::string& roomId,
                                    const std::string& callerId, const std::string& callerName,
                                    CallType type, const std::string& sdpOffer,
                                    int inviteLifetimeSec = 120);

    // Answer an incoming call.
    // Returns the answer event content JSON.
    std::string answerCall(const std::string& callId, const std::string& sdpAnswer, std::string& error);

    // Reject/decline an incoming call.
    // Returns the reject event content JSON.
    std::string rejectCall(const std::string& callId, const std::string& reason = "rejected");

    // Hang up an active call.
    // Returns the hangup event content JSON.
    std::string hangupCall(const std::string& callId, CallManagerEndReason reason = CallManagerEndReason::USER_HUNG_UP,
                            const std::string& reasonText = "");

    // Timeout an unanswered call.
    std::string timeoutCall(const std::string& callId);

    // ====== ICE Candidates ======

    // Add a local ICE candidate to a call.
    void addLocalParsedIceCandidate(const std::string& callId, const ParsedIceCandidate& candidate);

    // Add a remote ICE candidate (received from peer).
    void addRemoteParsedIceCandidate(const std::string& callId, const ParsedIceCandidate& candidate);

    // Build candidates event content for sending.
    std::string buildCandidatesEvent(const std::string& callId,
                                      const std::vector<ParsedIceCandidate>& candidates);

    // Get all collected remote candidates for a call.
    std::vector<ParsedIceCandidate> getRemoteCandidates(const std::string& callId) const;

    // ====== Call State Management ======

    // Set the call to "connected" state (media flowing).
    void setCallConnected(const std::string& callId);

    // Set the call to "connecting" state (ICE in progress).
    void setCallConnecting(const std::string& callId);

    // Mute/unmute local audio.
    void setMuted(const std::string& callId, bool muted);

    // Enable/disable local video.
    void setVideoEnabled(const std::string& callId, bool enabled);

    // Set remote video state.
    void setRemoteVideo(const std::string& callId, bool hasVideo);

    // ====== Call Queries ======

    // Get call by ID.
    bool getCall(const std::string& callId, CallSession& out) const;

    // Get the active (connected) call if any.
    bool getActiveCall(CallSession& out) const;

    // Get an incoming ringing call if any.
    bool getIncomingCall(CallSession& out) const;

    // Get all calls in a room.
    std::vector<CallSession> getRoomCalls(const std::string& roomId) const;

    // Check if a room has an active call.
    bool isRoomInCall(const std::string& roomId) const;

    // Get total call count.
    int totalCalls() const { return static_cast<int>(calls_.size()); }

    // Get active call count.
    int activeCallCount() const;

    // ====== Call Duration ======

    // Get the current duration of a connected call (seconds).
    int getCallDuration(const std::string& callId) const;

    // Format call duration as "MM:SS" or "HH:MM:SS".
    std::string formatCallDuration(int seconds) const;

    // ====== Serialization ======

    // Format call info as JSON for UI.
    std::string callToJson(const CallSession& call) const;

    // Format all calls as JSON array.
    std::string allCallsToJson() const;

    // ====== Call Event History ======

    // Parse a call event from the timeline.
    CallEvent parseCallEvent(const std::string& eventType, const std::string& contentJson,
                              const std::string& senderId, int64_t timestampMs);

    // Format a call event for timeline display.
    std::string formatCallEvent(const CallEvent& event, const std::string& senderDisplayName);

private:
    std::vector<CallSession> calls_;

    CallSession* findCall(const std::string& callId);
    const CallSession* findCall(const std::string& callId) const;

    std::string generateCallId() const;
    bool isValidStateTransition(CallManagerCallState from, CallManagerCallState to) const;
    int64_t nowMs() const;
};

// JNI compat aliases (1055949e JNI uses old type names)

} // namespace progressive
