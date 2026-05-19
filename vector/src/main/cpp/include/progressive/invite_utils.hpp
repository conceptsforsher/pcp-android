#ifndef PROGRESSIVE_INVITE_UTILS_HPP
#define PROGRESSIVE_INVITE_UTILS_HPP

#include <string>
#include <vector>

namespace progressive {

// ---- Invite Management ----

struct InviteInfo {
    std::string roomId;
    std::string roomName;
    std::string roomAvatarUrl;
    std::string inviterName;
    std::string inviterId;
    int64_t invitedAtMs = 0;
    bool isDirect = false;
    bool isEncrypted = false;
    int memberCount = 0;
};

struct InviteValidation {
    bool valid = false;
    std::string errorMessage;
    bool userNotFound = false;
    bool isSelf = false;           // inviting yourself
    bool alreadyMember = false;
    bool banned = false;
    bool serverNotFound = false;
};

// Parse invite info from room summary and state events.
InviteInfo parseInviteInfo(const std::string& roomId, const std::string& stateEventsJson);

// Validate an invite before sending.
InviteValidation validateInvite(
    const std::string& inviteeId,
    const std::string& myUserId,
    const std::vector<std::string>& currentMembers,
    const std::vector<std::string>& bannedUsers,
    const std::string& reason,
    int maxReasonLength = 500
);

// Build invite request body JSON.
std::string buildInviteBody(const std::string& userId, const std::string& reason = "");

// Build 3PID invite request body.
std::string buildThreePidInviteBody(const std::string& idServer,
    const std::string& medium, const std::string& address);

// Format invite for room list preview.
std::string formatInvitePreview(const InviteInfo& info);

// Check if an invite is expired (older than N days).
bool isInviteExpired(int64_t invitedAtMs, int maxAgeDays = 30);

// Sort invites by recency.
void sortInvites(std::vector<InviteInfo>& invites);

// ---- Knock (Request to Join) ----

struct KnockInfo {
    std::string roomId;
    std::string reason;
    int64_t knockedAtMs = 0;
};

// Build knock request body JSON.
std::string buildKnockBody(const std::string& reason = "");

// Format knock reason for display.
std::string formatKnockReason(const std::string& reason);

} // namespace progressive

#endif // PROGRESSIVE_INVITE_UTILS_HPP
