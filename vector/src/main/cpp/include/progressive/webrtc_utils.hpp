#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include "progressive/call_models.hpp"

namespace progressive {

// ---- WebRTC / VoIP Utilities ----

// Original Kotlin: CallInfo from MxCall.kt
struct CallInfo {
    std::string callId;
    std::string roomId;
    std::string callerId;
    std::string callerName;
    std::vector<std::string> participants;
    bool isVideo = false;
    bool isActive = false;
    bool isIncoming = false;
    int64_t startedAtMs = 0;
    int64_t answeredAtMs = 0;
    int durationSeconds = 0;
};

// Original Kotlin: SdpType.kt — offer/answer
struct SdpOffer {
    std::string type;          // "offer" or "answer"
    std::string sdp;           // SDP content
    bool valid = false;
};

// Original Kotlin: CallCandidate.kt
struct IceCandidate {
    std::string sdpMid;
    int sdpMLineIndex = 0;
    std::string candidate;     // candidate string
};

// ---- Call State ----
//
// Original Kotlin: CallState.kt (sealed class)
//   Idle, CreateOffer, Dialing, LocalRinging, Answering, Connected, Ended

enum class CallState {
    IDLE = 0,         // No call activity
    INVITE_SENT = 1,  // Outgoing invite has been sent
    RINGING = 2,      // Incoming call, local ringing
    CREATED = 3,      // Call offer created, awaiting local SDP
    CONNECTING = 4,   // ICE/DTLS negotiation in progress
    CONNECTED = 5,    // Media flowing
    DISCONNECTED = 6, // Connection lost (recoverable)
    TERMINATED = 7    // Call ended, resources released
};

const char* CallStateToString(CallState state);
CallState CallStateFromString(const std::string& s);

// ---- CallCandidate Build/Parse ----
//
// Original Kotlin: CallCandidate.kt (data class)

std::string buildCallCandidate(const CallCandidate& cand);
CallCandidate parseCallCandidate(const std::string& json);

// ---- CallCapabilities Build/Parse ----
//
// Original Kotlin: CallCapabilities.kt (data class)

std::string callCapabilitiesToJson(const CallCapabilities& caps);
CallCapabilities parseCallCapabilities(const std::string& json);

// ---- Builders — JSON for Matrix call events ----
//
// Original Kotlin: CallSignalingService.kt, CallInviteContent.kt, etc.

// Original Kotlin: CallInviteContent.kt (m.call.invite)
std::string buildCallInviteContent(const std::string& callId, bool isVideo,
    const std::string& sdpOffer, int lifetimeSeconds = 60000,
    const std::string& invitee = "", const CallCapabilities& capabilities = {});
std::string buildCallInviteContent(const CallInviteContent& content);

// Original Kotlin: CallAnswerContent.kt (m.call.answer)
std::string buildCallAnswerContent(const std::string& callId, const std::string& sdpAnswer,
    const CallCapabilities& capabilities = {});
std::string buildCallAnswerContent(const CallAnswerContent& content);

// Original Kotlin: CallHangupContent.kt (m.call.hangup)
std::string buildCallHangupContent(const std::string& callId,
    EndCallReason reason = EndCallReason::USER_HANGUP, const std::string& partyId = "");
std::string buildCallHangupContent(const std::string& callId, const std::string& reason);
std::string buildCallHangupContent(const CallHangupContent& content);

// Original Kotlin: CallRejectContent.kt (m.call.reject)
std::string buildCallRejectContent(const std::string& callId,
    EndCallReason reason = EndCallReason::USER_BUSY);
std::string buildCallRejectContent(const CallRejectContent& content);

// Original Kotlin: CallCandidatesContent.kt (m.call.candidates)
std::string buildCallCandidatesContent(const std::string& callId,
    const std::vector<CallCandidate>& candidates, const std::string& partyId = "");
std::string buildCallCandidatesContent(const CallCandidatesContent& content);

// Original Kotlin: CallNegotiateContent.kt (m.call.negotiate)
std::string buildCallNegotiateContent(const std::string& callId,
    const std::string& sdpDescription, SdpType descType = SdpType::OFFER,
    int lifetimeSeconds = 60000);
std::string buildCallNegotiateContent(const CallNegotiateContent& content);

// Original Kotlin: CallSelectAnswerContent.kt (m.call.select_answer)
std::string buildCallSelectAnswerContent(const std::string& callId,
    const std::string& selectedPartyId);
std::string buildCallSelectAnswerContent(const CallSelectAnswerContent& content);

// Original Kotlin: CallReplacesContent.kt (m.call.replaces)
std::string buildCallReplacesContent(const std::string& callId,
    const std::string& replacementId);
std::string buildCallReplacesContent(const CallReplacesContent& content);

// Original Kotlin: CallAssertedIdentityContent.kt (m.call.asserted_identity)
std::string buildCallAssertedIdentityContent(const std::string& callId,
    const std::string& assertedId, const std::string& displayName = "",
    const std::string& avatarUrl = "");
std::string buildCallAssertedIdentityContent(const CallAssertedIdentityContent& content);

// ---- Parsers — Parse JSON into call event models ----
//
// (Re-exported from call_models.hpp; convenience overloads below)
//   CallInviteContent    parseCallInviteContent()
//   CallAnswerContent    parseCallAnswerContent()
//   CallHangupContent    parseCallHangupContent()
//   CallRejectContent    parseCallRejectContent()
//   CallCandidatesContent parseCallCandidatesContent()
//   CallNegotiateContent parseCallNegotiateContent()

// Convenience aliases matching the call_models.hpp parse functions
inline CallInviteContent parseCallInviteContent(const std::string& json) {
    return parseCallInvite(json);
}
inline CallAnswerContent parseCallAnswerContent(const std::string& json) {
    return parseCallAnswer(json);
}
inline CallHangupContent parseCallHangupContent(const std::string& json) {
    return parseCallHangup(json);
}
inline CallRejectContent parseCallRejectContent(const std::string& json) {
    return parseCallReject(json);
}
inline CallCandidatesContent parseCallCandidatesContent(const std::string& json) {
    return parseCallCandidates(json);
}
inline CallNegotiateContent parseCallNegotiateContent(const std::string& json) {
    return parseCallNegotiate(json);
}

// ---- Legacy / Utility Functions ----

// Parse call invite event content (legacy — returns CallInfo).
CallInfo parseCallInviteLegacy(const std::string& eventContentJson, const std::string& eventId,
    const std::string& roomId, const std::string& senderId);

// Parse SDP offer/answer from Matrix VoIP event.
SdpOffer parseSdpOffer(const std::string& eventContentJson);

// Parse ICE candidate from Matrix VoIP event.
IceCandidate parseIceCandidate(const std::string& eventContentJson);

// Build VoIP invite event content (legacy simple builder).
std::string buildCallInviteContentLegacy(const std::string& callId, bool isVideo,
    const std::string& sdpOffer, int lifetimeSeconds = 60000);

// Build VoIP answer event content (legacy simple builder).
std::string buildCallAnswerContentLegacy(const std::string& callId, const std::string& sdpAnswer);

// Build ICE candidate event content (legacy — uses IceCandidate struct).
std::string buildCallCandidatesContentLegacy(const std::string& callId,
    const std::vector<IceCandidate>& candidates);

// Build call hangup event content (legacy simple builder).
std::string buildCallHangupContentLegacy(const std::string& callId, const std::string& reason = "");

// Format call info for notification.
std::string formatCallNotification(const CallInfo& call);

// Format call duration as "MM:SS" or "HH:MM:SS".
std::string formatCallDuration(int seconds);

// Check if a call event is expired.
bool isCallExpired(int64_t createdAtMs, int timeoutSeconds = 120);

// Get call state from event content.
std::string getCallState(const std::string& eventContentJson);

} // namespace progressive
