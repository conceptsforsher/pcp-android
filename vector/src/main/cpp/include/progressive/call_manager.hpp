#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include "progressive/call_models.hpp"

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
    std::string opponentUserId;  // The other party's MXID
    std::string opponentDisplayName; // Display name of opponent
    std::vector<std::string> participants;
    CallType type = CallType::VOICE;
    CallManagerCallState state = CallManagerCallState::IDLE;
    bool isIncoming = false;     // true = we received the invite
    bool isMuted = false;
    bool isSpeakerOn = true;
    bool isVideoOn = false;      // Local video enabled
    bool isHeld = false;         // Call is on hold
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

// ---- Call Participant ----
//
// Original Kotlin: CallParticipant from group call / Jitsi integration
struct CallParticipant {
    std::string userId;          // Matrix user ID
    std::string displayName;     // Display name
    std::string avatarUrl;       // Avatar URL
    bool isMuted = false;        // Audio muted
    bool isVideoOn = false;      // Video enabled
    bool isSpeaking = false;     // Currently speaking (dominant speaker)
    std::string deviceId;        // Device ID (for VoIP v1 party_id)
};

// ---- Call Signaling State ----
//
// Original Kotlin: WebRTC signaling state machine
enum class CallSignalingState {
    STABLE = 0,              // "stable" — no offer/answer exchange in progress
    HAVE_LOCAL_OFFER = 1,    // "have-local-offer" — local SDP offer sent
    HAVE_REMOTE_OFFER = 2,   // "have-remote-offer" — remote SDP offer received
    CLOSED = 3               // "closed" — connection closed
};

const char* callSignalingStateToString(CallSignalingState s);
CallSignalingState callSignalingStateFromString(const std::string& s);

// ---- Call Ice Candidate (high level) ----
//
// Original Kotlin: WebRTC RTCIceCandidate wrapper
struct CallIceCandidate {
    std::string sdpMid;          // Media stream identification tag
    int sdpMLineIndex = 0;       // Media line index (0-based)
    std::string candidate;       // Candidate string (a=candidate:...)
    std::string serverUrl;       // Optional: STUN/TURN server URL

    bool isValid() const { return !sdpMid.empty() && !candidate.empty(); }
};

// ---- Call Session Description ----
//
// Original Kotlin: WebRTC RTCSessionDescription wrapper
struct CallSessionDescription {
    std::string type;            // "offer", "answer", "pranswer", "rollback"
    std::string sdp;             // Full SDP text

    bool isValid() const { return !type.empty() && !sdp.empty(); }
    bool isOffer() const { return type == "offer"; }
    bool isAnswer() const { return type == "answer"; }
};

// ---- Group Call Info ----
//
// Original Kotlin: GroupCall / conference call models
struct GroupCallInfo {
    std::string groupId;         // Group/conference identifier
    std::string roomId;          // Room where the group call is active
    std::vector<CallParticipant> participants; // Current participants
    std::string state;           // "creating", "active", "ended"
    bool isActive = false;       // Whether the group call is active

    int participantCount() const { return static_cast<int>(participants.size()); }
};

// ---- Group Call State ----
//
// Original Kotlin: GroupCallState enum
enum class GroupCallState {
    CREATING = 0,  // "creating" — group call is being set up
    ACTIVE = 1,    // "active" — group call is live
    ENDED = 2      // "ended" — group call has finished
};

const char* groupCallStateToString(GroupCallState s);
GroupCallState groupCallStateFromString(const std::string& s);

// ---- Call Participant State ----
//
// Original Kotlin: individual peer state within group call
enum class CallParticipantState {
    CONNECTING = 0,    // "connecting" — establishing connection
    CONNECTED = 1,     // "connected" — media flowing
    DISCONNECTED = 2,  // "disconnected" — connection lost
    LEFT = 3           // "left" — participant left
};

const char* callParticipantStateToString(CallParticipantState s);
CallParticipantState callParticipantStateFromString(const std::string& s);

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

    // ====== Call Event Processing ======
    //
    // Original Kotlin: CallEventProcessor.kt / CallSignalingHandler.kt

    // Process an incoming m.call.invite event.
    // Returns the call ID, or empty if ignored.
    std::string processCallInvite(const std::string& roomId, const std::string& senderId,
                                   const std::string& contentJson, int64_t timestampMs);

    // Process an incoming m.call.answer event.
    // Returns true if the call state was updated.
    bool processCallAnswer(const std::string& contentJson);

    // Process an incoming m.call.hangup event.
    // Returns true if the call state was updated.
    bool processCallHangup(const std::string& contentJson);

    // Process an incoming m.call.candidates event.
    // Returns the number of candidates added.
    int processCallCandidates(const std::string& contentJson);

    // Process an incoming m.call.negotiate event.
    // Returns true if the call state was updated.
    bool processCallNegotiate(const std::string& contentJson);

    // Check if an event type string is a call event type.
    bool isCallEvent(const std::string& eventType) const;

    // ==== Call Display ====

    // Get a human-readable display name for a call.
    // Uses room name if available, otherwise opponent name/ID.
    std::string getCallDisplayName(const CallSession& call) const;

    // ==== Hold / Resume ====

    // Set a call on hold.
    void setCallOnHold(const std::string& callId);

    // Resume a call that is on hold.
    void resumeCall(const std::string& callId);

    // ====== Call Event History ======

    // Parse a call event from the timeline.
    CallEvent parseCallEvent(const std::string& eventType, const std::string& contentJson,
                              const std::string& senderId, int64_t timestampMs);

    // Format a call event for timeline display.
    std::string formatCallEvent(const CallEvent& event, const std::string& senderDisplayName);

private:
    std::vector<CallSession> calls_;
    std::vector<GroupCallInfo> groupCalls_;

    CallSession* findCall(const std::string& callId);
    const CallSession* findCall(const std::string& callId) const;

    std::string generateCallId() const;
    bool isValidStateTransition(CallManagerCallState from, CallManagerCallState to) const;
    int64_t nowMs() const;
};

// ================================================================
// CallSessionManager — convenience wrapper with listener-oriented API
//
// Original Kotlin: MxCallFactory.kt / ActiveCallHandler.kt
// ================================================================

class CallSessionManager {
public:
    CallSessionManager();

    // Create a new outgoing call.
    std::string createCall(const std::string& roomId, const std::string& opponentUserId,
                            const std::string& opponentName, bool isVideoCall);

    // Answer an incoming call by call ID.
    std::string answerCall(const std::string& callId, const std::string& sdpAnswer);

    // Hang up an active call.
    std::string hangupCall(const std::string& callId, const std::string& reason = "");

    // Reject an incoming call.
    std::string rejectCall(const std::string& callId);

    // Handle events from the timeline (via CallListener pattern).
    void onCallInviteReceived(const std::string& roomId, const std::string& senderId,
                               const std::string& senderName, const std::string& contentJson,
                               int64_t timestampMs);
    void onCallAnswerReceived(const std::string& contentJson);
    void onCallHangupReceived(const std::string& contentJson);

    // Get the currently active call if any.
    bool getActiveCall(CallSession& out) const;

    // Get all calls for a specific room.
    std::vector<CallSession> getCallsForRoom(const std::string& roomId) const;

    // Hold / Resume helpers.
    void setCallOnHold(const std::string& callId);
    void resumeCall(const std::string& callId);

    // Check events from the timeline.
    bool isCallEvent(const std::string& eventType) const;

    // Get underlying manager for advanced access.
    CallManager& manager() { return manager_; }
    const CallManager& manager() const { return manager_; }

private:
    CallManager manager_;
};

// JNI compat aliases (1055949e JNI uses old type names)

} // namespace progressive
