#ifndef PROGRESSIVE_WEBRTC_UTILS_HPP
#define PROGRESSIVE_WEBRTC_UTILS_HPP

#include <string>
#include <vector>

namespace progressive {

// ---- WebRTC / VoIP Utilities ----

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

struct SdpOffer {
    std::string type;          // "offer" or "answer"
    std::string sdp;           // SDP content
    bool valid = false;
};

struct IceCandidate {
    std::string sdpMid;
    int sdpMLineIndex = 0;
    std::string candidate;     // candidate string
};

// Parse call invite event content.
CallInfo parseCallInvite(const std::string& eventContentJson, const std::string& eventId,
    const std::string& roomId, const std::string& senderId);

// Parse SDP offer/answer from Matrix VoIP event.
SdpOffer parseSdpOffer(const std::string& eventContentJson);

// Parse ICE candidate from Matrix VoIP event.
IceCandidate parseIceCandidate(const std::string& eventContentJson);

// Build VoIP invite event content.
std::string buildCallInviteContent(const std::string& callId, bool isVideo,
    const std::string& sdpOffer, int lifetimeSeconds = 60000);

// Build VoIP answer event content.
std::string buildCallAnswerContent(const std::string& callId, const std::string& sdpAnswer);

// Build ICE candidate event content.
std::string buildCallCandidatesContent(const std::string& callId,
    const std::vector<IceCandidate>& candidates);

// Build call hangup event content.
std::string buildCallHangupContent(const std::string& callId, const std::string& reason = "");

// Format call info for notification.
std::string formatCallNotification(const CallInfo& call);

// Format call duration as "MM:SS" or "HH:MM:SS".
std::string formatCallDuration(int seconds);

// Check if a call event is expired.
bool isCallExpired(int64_t createdAtMs, int timeoutSeconds = 120);

// Get call state from event content.
std::string getCallState(const std::string& eventContentJson);

} // namespace progressive

#endif // PROGRESSIVE_WEBRTC_UTILS_HPP
