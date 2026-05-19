#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include "progressive/message_content.hpp"

namespace progressive {

// ==== Call Signaling Models — Matrix VoIP (MSC2746) ====
//
// Ref: https://github.com/matrix-org/matrix-doc/pull/2746
//
// Original Kotlin: call/*.kt in room/model/call/

// Original Kotlin (CallSignalingContent.kt:21-37):
//   interface CallSignalingContent {
//       val callId: String?
//       val partyId: String?
//       val version: String?
//   }
struct CallSignalingContent {
    std::string callId;      // "call_id" key
    std::string partyId;     // "party_id" key — identifies remote echo
    std::string version;     // "version" key — VoIP spec version ("0")
};

// Original Kotlin (SdpType.kt:23-30):
//   enum class SdpType { OFFER, ANSWER }
enum class SdpType {
    OFFER = 0,
    ANSWER = 1
};
const char* sdpTypeToString(SdpType t);
SdpType sdpTypeFromString(const std::string& s);

// Original Kotlin (EndCallReason.kt:24-46):
//   enum class EndCallReason { ICE_FAILED, ICE_TIMEOUT, USER_HANGUP, ... }
enum class EndCallReason {
    ICE_FAILED = 0,       // "ice_failed"
    ICE_TIMEOUT = 1,      // "ice_timeout"
    USER_HANGUP = 2,      // "user_hangup"
    REPLACED = 3,         // "replaced"
    USER_MEDIA_FAILED = 4,// "user_media_failed"
    INVITE_TIMEOUT = 5,   // "invite_timeout"
    UNKNOWN_ERROR = 6,    // "unknown_error"
    USER_BUSY = 7,        // "user_busy"
    ANSWERED_ELSEWHERE = 8// "answered_elsewhere"
};
const char* endCallReasonToString(EndCallReason r);
EndCallReason endCallReasonFromString(const std::string& s);

// ==== CallEndReason (high-level UI reason) ====
//
// Original Kotlin: CallEndReason (deduced from EndCallReason + call state)
enum class CallEndReason {
    UNKNOWN = 0,             // "unknown"
    HUNG_UP = 1,             // "hung_up"
    REJECTED = 2,            // "rejected"
    BUSY = 3,                // "busy"
    TIMEOUT = 4,             // "timeout"
    ANSWERED_ELSEWHERE = 5,  // "answered_elsewhere"
    ICE_FAILED = 6,          // "ice_failed"
    INVITE_EXPIRED = 7       // "invite_expired"
};

const char* callEndReasonToString(CallEndReason r);
CallEndReason callEndReasonFromString(const std::string& s);

// ==== CallRingInfo — Incoming Call UI ====
//
// Original Kotlin: CallRingInfo / incoming call notification data
struct CallRingInfo {
    std::string callId;            // call ID
    std::string callerName;        // display name of caller
    std::string callerAvatar;      // avatar URL of caller
    std::string roomName;          // room display name
    bool isVideoCall = false;      // whether it's a video call
    bool isGroupCall = false;      // whether it's a group call
    int inviteLifetimeMs = 120000; // how long the invite is valid
};

// Original Kotlin (CallCapabilities.kt:25-32):
//   data class CallCapabilities(@Json(name="m.call.transferee") transferee: Boolean?)
struct CallCapabilities {
    bool transferee = false;        // "m.call.transferee" — supports call transfer
    bool supportsDtmf = false;      // "m.call.dtmf" — supports DTMF tones
    bool useStereo = false;         // "m.call.use_stereo" — stereo audio support
    bool supportsVideo = true;      // implicit — almost all clients support video
    bool supportsScreenSharing = false; // "m.call.screen_sharing" — screen sharing

    bool supportsCallTransfer() const { return transferee; }
    bool hasVideo() const { return supportsVideo; }
    bool hasScreenSharing() const { return supportsScreenSharing; }
};

// Original Kotlin (CallCandidate.kt:25-36):
//   data class CallCandidate(sdpMid, sdpMLineIndex, candidate)
struct CallCandidate {
    std::string sdpMid;        // "sdpMid" key
    int sdpMLineIndex = 0;     // "sdpMLineIndex" key
    std::string candidate;     // "candidate" key — SDP 'a' line
};

// ==== Call Invite ====
// Original Kotlin (CallInviteContent.kt:26-59):
//   data class CallInviteContent(callId, partyId, offer, version, lifetime, invitee, capabilities)
//   data class Offer(type: SdpType, sdp: String)

struct CallInviteContent : CallSignalingContent {
    struct Offer {
        SdpType type = SdpType::OFFER;  // "type" key — must be "offer"
        std::string sdp;                 // "sdp" key — session description
    };
    Offer offer;                     // "offer" key
    int lifetime = 0;                // "lifetime" key — validity in ms
    std::string invitee;             // "invitee" key — target user ID
    CallCapabilities capabilities;   // "capabilities" key

    // Original Kotlin: fun isVideo() = offer?.sdp?.contains("m=video") == true
    bool isVideo() const {
        return offer.sdp.find("m=video") != std::string::npos;
    }
};

// Original Kotlin (CallAnswerContent.kt:26-49):
//   data class CallAnswerContent(callId, partyId, answer, version, capabilities)
//   data class Answer(type: SdpType, sdp: String)
struct CallAnswerContent : CallSignalingContent {
    struct Answer {
        SdpType type = SdpType::ANSWER;  // "type" key — must be "answer"
        std::string sdp;                  // "sdp" key
    };
    Answer answer;                   // "answer" key
    CallCapabilities capabilities;   // "capabilities" key
};

// Original Kotlin (CallCandidatesContent.kt:26-44):
//   data class CallCandidatesContent(callId, partyId, candidates, version)
struct CallCandidatesContent : CallSignalingContent {
    std::vector<CallCandidate> candidates;  // "candidates" key
};

// Original Kotlin (CallHangupContent.kt:27-46):
//   data class CallHangupContent(callId, partyId, version, reason)
struct CallHangupContent : CallSignalingContent {
    EndCallReason reason = EndCallReason::USER_HANGUP;  // "reason" key
};

// Original Kotlin (CallRejectContent.kt:27-44):
//   data class CallRejectContent(callId, partyId, version, reason)
struct CallRejectContent : CallSignalingContent {
    EndCallReason reason = EndCallReason::USER_BUSY;    // "reason" key
};

// Original Kotlin (CallNegotiateContent.kt:26-52):
//   data class CallNegotiateContent(callId, partyId, lifetime, description, version)
//   data class Description(type: SdpType?, sdp: String?)
struct CallNegotiateContent : CallSignalingContent {
    struct Description {
        SdpType type = SdpType::OFFER;
        std::string sdp;
    };
    int lifetime = 0;                // "lifetime" key
    Description description;         // "description" key
};

// Original Kotlin (CallSelectAnswerContent.kt:26-43):
//   data class CallSelectAnswerContent(callId, partyId, selectedPartyId, version)
struct CallSelectAnswerContent : CallSignalingContent {
    std::string selectedPartyId;     // "selected_party_id" key
};

// Original Kotlin (CallReplacesContent.kt:28-73):
//   data class CallReplacesContent(callId, partyId, replacementId, targetRoomId,
//       targetUser, createCall, awaitCall, version)
struct CallReplacesContent : CallSignalingContent {
    struct TargetUser {
        std::string id;              // "id" key — Matrix user ID
        std::string displayName;     // "display_name" key
        std::string avatarUrl;       // "avatar_url" key
    };
    std::string replacementId;       // "replacement_id" key
    std::string targetRoomId;        // "target_room" key
    TargetUser targetUser;           // "target_user" key
    std::string createCall;          // "create_call" key
    std::string awaitCall;           // "await_call" key
};

// Original Kotlin (CallAssertedIdentityContent.kt:27-56):
//   data class CallAssertedIdentityContent(callId, partyId, version, assertedIdentity)
//   data class AssertedIdentity(id, displayName, avatarUrl)
struct CallAssertedIdentityContent : CallSignalingContent {
    struct AssertedIdentity {
        std::string id;              // "id" key
        std::string displayName;     // "display_name" key
        std::string avatarUrl;       // "avatar_url" key
    };
    AssertedIdentity assertedIdentity;  // "asserted_identity" key
};

// ==== JSON Parsing ====

CallInviteContent parseCallInvite(const std::string& contentJson);
CallAnswerContent parseCallAnswer(const std::string& contentJson);
CallCandidatesContent parseCallCandidates(const std::string& contentJson);
CallHangupContent parseCallHangup(const std::string& contentJson);
CallRejectContent parseCallReject(const std::string& contentJson);
CallNegotiateContent parseCallNegotiate(const std::string& contentJson);
CallReplacesContent parseCallReplaces(const std::string& contentJson);
CallAssertedIdentityContent parseCallAssertedIdentity(const std::string& contentJson);

// ==== Call State (Lifecycle) ====
//
// Moved to webrtc_utils.hpp — CallState enum
//

// ==== TURN Server Response ====
//
// Original Kotlin (TurnServerResponse.kt:26-47):
//   data class TurnServerResponse(username, password, uris, ttl)
//
// Response from GET /_matrix/client/r0/voip/turnServer

struct TurnServerResponse {
    std::string username;            // "username" key
    std::string password;            // "password" key
    std::vector<std::string> uris;   // "uris" key — TURN URI list
    int ttl = 0;                     // "ttl" key — time-to-live in seconds

    bool isValid() const { return !username.empty() && !uris.empty(); }
};

TurnServerResponse parseTurnServerResponse(const std::string& json);

} // namespace progressive
