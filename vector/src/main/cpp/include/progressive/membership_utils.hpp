#ifndef PROGRESSIVE_MEMBERSHIP_UTILS_HPP
#define PROGRESSIVE_MEMBERSHIP_UTILS_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// ---- Room MemberState ----

enum class MemberState { None, Join, Invite, Leave, Ban, Knock, Unknown };

struct MemberInfo {
    std::string userId;
    std::string displayName;
    std::string avatarUrl;
    MemberState membership = MemberState::Unknown;
    int powerLevel = 0;
    std::string reason;        // ban reason or invite reason
    int64_t timestampMs = 0;
    bool isFromCache = false;
};

struct MemberStateChange {
    std::string userId;
    std::string displayName;
    MemberState oldMemberState = MemberState::Unknown;
    MemberState newMemberState = MemberState::Unknown;
    std::string senderId;       // who made the change
    int64_t timestampMs = 0;
};

// Parse membership from a state event.
MemberInfo parseMemberInfo(const std::string& stateContentJson, const std::string& userId);

// Parse membership string to enum.
MemberState parseMemberState(const std::string& membershipStr);

// Format membership as human-readable text.
std::string formatMemberState(MemberState membership);

// Check if membership is a positive state (join/invite/knock).
bool isActiveMember(MemberState membership);

// Check if membership allows reading room messages.
bool canReadMessages(MemberState membership);

// Detect membership changes between two state events.
MemberStateChange detectMemberStateChange(const MemberInfo& oldInfo, const MemberInfo& newInfo);

// Format a membership change as human-readable text.
std::string formatMemberStateChange(const MemberStateChange& change);

// ---- Member List ----

struct MemberListInfo {
    std::string roomId;
    std::vector<MemberInfo> members;
    int totalMembers = 0;
    int joinedMembers = 0;
    int invitedMembers = 0;
    int bannedMembers = 0;
    bool isTruncated = false;    // server truncated the list
};

// Parse member list from Matrix API response.
MemberListInfo parseMemberList(const std::string& roomId, const std::string& apiResponseJson, bool isTruncated);

// Filter members by membership type.
std::vector<MemberInfo> filterByMemberState(const std::vector<MemberInfo>& members, MemberState type);

// Filter members by display name (search).
std::vector<MemberInfo> searchMembers(const std::vector<MemberInfo>& members, const std::string& query);

// Sort members (name, power level, join date).
void sortMembers(std::vector<MemberInfo>& members, const std::string& sortBy);

// Format member list stats as JSON.
std::string memberListToJson(const MemberListInfo& list);

// ---- Member Sorting (from RoomMemberListComparator.kt 53L) ----
// Sort by: power level (high→low) → display name (case-insensitive) → userId

void sortMembersByPowerAndName(std::vector<MemberInfo>& members);
bool memberCompare(const MemberInfo& a, const MemberInfo& b);

// ---- MemberState Diff (from TimelineEventVisibilityHelper.kt:261-279) ----
// Detects what changed between two membership events.
// Original: computeMemberStateDiff() → MemberStateDiff

struct MemberStateDiff {
    bool isJoin = false;
    bool isPart = false;
    bool isDisplaynameChange = false;
    bool isAvatarChange = false;
    bool hasChanged = false;  // any change at all
};

// Compute the difference between two membership states.
// @param oldMemberState, newMemberState — previous and current membership
// @param oldName, newName — previous and current display name
// @param oldAvatar, newAvatar — previous and current avatar URL
// @param isSelf — true if sender == stateKey (for part detection)
MemberStateDiff computeMemberStateDiff(
    MemberState oldMemberState, MemberState newMemberState,
    const std::string& oldName, const std::string& newName,
    const std::string& oldAvatar, const std::string& newAvatar,
    bool isSelf);

} // namespace progressive

#endif // PROGRESSIVE_MEMBERSHIP_UTILS_HPP
