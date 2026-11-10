#pragma once
// ============================================================================
// call_api_models.hpp — C++ translations of Matrix VoIP call content types
//
// Ported from:
//   matrix-sdk-android/src/main/java/org/matrix/android/sdk/api/session/room/model/call/
//
// These define the signaling payloads for Matrix VoIP (MSC3401/WebRTC).
// ============================================================================

#include <string>
#include <vector>
#include <cstdint>

namespace pcp {
namespace call_models {

// ==== EndCallReason (EndCallReason.kt:21-29) ====
//
// Original Kotlin:
//   enum class EndCallReason { ICE_FAILED, INVITE_TIMEOUT,
//       USER_HANGUP, USER_MEDIA_FAILED, USER_BUSY,
//       UNKNOWN }
enum class EndCallReason {
    ICE_FAILED, INVITE_TIMEOUT, USER_HANGUP, USER_MEDIA_FAILED,
    USER_BUSY, UNKNOWN
};
inline const char* endCallReasonToString(EndCallReason r) {
    switch (r) {
        case EndCallReason::ICE_FAILED: return "ice_failed";
        case EndCallReason::INVITE_TIMEOUT: return "invite_timeout";
        case EndCallReason::USER_HANGUP: return "user_hangup";
        case EndCallReason::USER_MEDIA_FAILED: return "user_media_failed";
        case EndCallReason::USER_BUSY: return "user_busy";
        default: return "unknown";
    }
}

// ==== SdpType (SdpType.kt:21-27) ====
//
// Original Kotlin:
//   enum class SdpType(val value: String) {
//       OFFER("offer"), ANSWER("answer"); }
enum class SdpType { OFFER, ANSWER, UNKNOWN };
inline const char* sdpTypeToString(SdpType t) {
    switch (t) {
        case SdpType::OFFER: return "offer";
        case SdpType::ANSWER: return "answer";
        default: return "";
    }
}

// ==== CallCapabilities (CallCapabilities.kt:21-25) ====
//
// Original Kotlin:
//   data class CallCapabilities(val supportsVideo: Boolean = false)
struct CallCapabilities {
    bool supportsVideo = false;
};

// ==== CallCandidate (CallCandidate.kt:22-30) ====
//
// Original Kotlin:
//   data class CallCandidate(
//       val sdpMid: String? = null, val sdpMLineIndex: Int? = null,
//       val candidate: String? = null)
struct CallCandidate {
    std::string sdpMid;
    int sdpMLineIndex = 0;
    std::string candidate;
};

// ==== CallInviteContent (CallInviteContent.kt:22-42) ====
//
// Original Kotlin:
//   data class CallInviteContent(
//       val callId: String, val offer: Any? = null, // SDP object
//       val version: Int? = 0, val lifetime: Int? = 60000,
//       val capabilities: CallCapabilities? = null,
//       val partyId: String? = null,
//       val invitee: String? = null, // for third party invites
//       val is DTMF: Boolean? = null)
struct CallInviteContent {
    std::string callId;
    std::string offerSdp;       // raw SDP string
    std::string offerType;      // "offer" or "answer"
    int version = 0;
    int lifetime = 60000;       // milliseconds
    CallCapabilities capabilities;
    bool hasCapabilities = false;
    std::string partyId;
    std::string invitee;
    bool isDtmf = false;
};

// ==== CallAnswerContent (CallAnswerContent.kt:21-30) ====
//
// Original Kotlin:
//   data class CallAnswerContent(
//       val callId: String, val answer: Any? = null,
//       val version: Int? = 0, val capabilities: CallCapabilities? = null,
//       val partyId: String? = null)
struct CallAnswerContent {
    std::string callId;
    std::string answerSdp;      // raw SDP string
    int version = 0;
    CallCapabilities capabilities;
    bool hasCapabilities = false;
    std::string partyId;
};

// ==== CallHangupContent (CallHangupContent.kt:21-30) ====
//
// Original Kotlin:
//   data class CallHangupContent(
//       val callId: String, val version: Int? = 0,
//       val reason: EndCallReason? = null, val partyId: String? = null)
struct CallHangupContent {
    std::string callId;
    int version = 0;
    EndCallReason reason = EndCallReason::UNKNOWN;
    std::string partyId;
};

// ==== CallRejectContent (CallRejectContent.kt:21-27) ====
//
// Original Kotlin:
//   data class CallRejectContent(
//       val callId: String, val version: Int? = 0,
//       val partyId: String? = null)
struct CallRejectContent {
    std::string callId;
    int version = 0;
    std::string partyId;
};

// ==== CallCandidatesContent (CallCandidatesContent.kt:21-28) ====
//
// Original Kotlin:
//   data class CallCandidatesContent(
//       val callId: String, val candidates: List<CallCandidate>,
//       val version: Int? = 0, val partyId: String? = null)
struct CallCandidatesContent {
    std::string callId;
    std::vector<CallCandidate> candidates;
    int version = 0;
    std::string partyId;
};

// ==== CallSelectAnswerContent (CallSelectAnswerContent.kt:21-27) ====
//
// Original Kotlin:
//   data class CallSelectAnswerContent(
//       val callId: String, val selectedPartyId: String,
//       val version: Int? = 0)
struct CallSelectAnswerContent {
    std::string callId;
    std::string selectedPartyId;
    int version = 0;
};

// ==== CallNegotiateContent (CallNegotiateContent.kt:22-30) ====
//
// Original Kotlin:
//   data class CallNegotiateContent(
//       val callId: String, val description: Any? = null,
//       val lifetime: Int? = 60000, val version: Int? = 0,
//       val partyId: String? = null)
struct CallNegotiateContent {
    std::string callId;
    std::string descriptionSdp; // raw SDP string
    int lifetime = 60000;
    int version = 0;
    std::string partyId;
};

// ==== CallReplacesContent (CallReplacesContent.kt:21-28) ====
//
// Original Kotlin:
//   data class CallReplacesContent(
//       val callId: String, val replacementId: String,
//       val version: Int? = 0)
struct CallReplacesContent {
    std::string callId;
    std::string replacementId;
    int version = 0;
};

// ==== CallAssertedIdentityContent (CallAssertedIdentityContent.kt:22-27) ====
//
// Original Kotlin:
//   data class CallAssertedIdentityContent(
//       val callId: String, val assertedIdentity: AssertedIdentity? = null,
//       val version: Int? = 0)
struct AssertedIdentity {
    std::string id;             // mxid
    std::string displayName;
};

struct CallAssertedIdentityContent {
    std::string callId;
    AssertedIdentity assertedIdentity;
    bool hasAssertedIdentity = false;
    int version = 0;
};

// ==== CallSignalingContent — discriminator ====
//
// Maps CallEventType to content struct for routing incoming call events.

enum class CallEventType {
    INVITE, ANSWER, HANGUP, REJECT, CANDIDATES,
    SELECT_ANSWER, NEGOTIATE, REPLACES, ASSERTED_IDENTITY, UNKNOWN
};

inline CallEventType callEventTypeFromString(const std::string& type) {
    if (type == "m.call.invite") return CallEventType::INVITE;
    if (type == "m.call.answer") return CallEventType::ANSWER;
    if (type == "m.call.hangup") return CallEventType::HANGUP;
    if (type == "m.call.reject") return CallEventType::REJECT;
    if (type == "m.call.candidates") return CallEventType::CANDIDATES;
    if (type == "m.call.select_answer") return CallEventType::SELECT_ANSWER;
    if (type == "m.call.negotiate") return CallEventType::NEGOTIATE;
    if (type == "m.call.replaces") return CallEventType::REPLACES;
    if (type == "m.call.asserted_identity" ||
        type == "org.matrix.call.asserted_identity")
        return CallEventType::ASSERTED_IDENTITY;
    return CallEventType::UNKNOWN;
}

inline const char* callEventTypeToString(CallEventType t) {
    switch (t) {
        case CallEventType::INVITE: return "m.call.invite";
        case CallEventType::ANSWER: return "m.call.answer";
        case CallEventType::HANGUP: return "m.call.hangup";
        case CallEventType::REJECT: return "m.call.reject";
        case CallEventType::CANDIDATES: return "m.call.candidates";
        case CallEventType::SELECT_ANSWER: return "m.call.select_answer";
        case CallEventType::NEGOTIATE: return "m.call.negotiate";
        case CallEventType::REPLACES: return "m.call.replaces";
        case CallEventType::ASSERTED_IDENTITY: return "m.call.asserted_identity";
        default: return "";
    }
}

} // namespace call_models
} // namespace pcp
