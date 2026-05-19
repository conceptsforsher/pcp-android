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

// ==== Member Event Handling (from RoomMemberEventHandler.kt:37-153) ====
// Original Kotlin:
//   internal class RoomMemberEventHandler {
//       fun handle(realm, roomId, event, isInitialSync, aggregator): Boolean
//   }

// Types of member changes in a room.
// Original: derived from membership transitions + profile diff
enum class MemberChangeAction {
    UNKNOWN,
    JOINED,              // user joined the room
    LEFT,                // user left voluntarily
    INVITED,             // user was invited
    BANNED,              // user was banned
    KICKED,              // user was kicked (left, but sender != user)
    KNOCKED,             // user knocked to join
    PROFILE_CHANGED,     // display name or avatar changed (membership unchanged)
    DISPLAY_NAME_CHANGED, // only display name changed
    AVATAR_CHANGED       // only avatar changed
};

// Aggregated info about a member event.
struct MemberChangeInfo {
    std::string roomId;
    std::string userId;
    std::string membership;       // "join", "invite", etc.
    std::string prevMembership;   // previous membership string
    std::string displayName;
    std::string avatarUrl;
    std::string reason;
    int64_t timestamp = 0;
    std::string senderId;
    MemberChangeAction action = MemberChangeAction::UNKNOWN;
    bool isProfileChange = false;
    bool isValid = false;
};

// Process a single m.room.member event and return the change info.
// Ported from RoomMemberEventHandler.handle()
// @param roomId — the room the event is for
// @param eventJson — the full event JSON (or content-only JSON)
// @param prevContentJson — previous content for diffing (incremental sync)
// @param isInitialSync — true if this is part of initial sync
// @param myUserId — the current user's userId (for self-detection)
MemberChangeInfo processMemberEvent(
    const std::string& roomId,
    const std::string& eventJson,
    const std::string& prevContentJson,
    bool isInitialSync);

// Determine the MemberChangeAction from old→new membership.
// Original: derived from computeMemberStateDiff + membership transition
MemberChangeAction computeMemberChangeAction(
    const std::string& oldMembership,
    const std::string& newMembership,
    bool isSelfAction);

// Check whether the membership string actually changed.
inline bool isMembershipChange(const std::string& oldM, const std::string& newM) {
    return oldM != newM;
}

// ==== Room Display Name Resolution (from RoomDisplayNameResolver.kt:59-166) ====

// Configuration for room display name computation.
// Original: matrixConfiguration.roomDisplayNameFallbackProvider
struct RoomDisplayNameConfig {
    int maxMembersToShow = 3;
    bool showHeroesFirst = true;
    std::string emptyRoomName = "Empty Room";
};

// Source of the room name — tracks how the name was computed.
enum class RoomNameSource {
    UNKNOWN,
    STATE_EVENT,   // from m.room.name
    ALIAS,         // from canonical alias
    HEROES,        // from heroes list
    MEMBERS,       // from member display names
    GENERATED,     // fallback: "Empty Room", roomId, etc.
    DIRECT_USER    // from the DM partner's userId
};

// Full room name resolution result.
struct RoomNameResult {
    std::string displayName;
    std::string normalizedName;   // lowercased for sorting
    RoomNameSource source = RoomNameSource::UNKNOWN;
    bool isFromState = false;     // true if name comes from m.room.name
};

// Compute the room display name using the standard Matrix algorithm.
// Ported from RoomDisplayNameResolver.resolve() + js-sdk calculateRoomName()
// @param currentMembership — the current user's membership in this room (for invite handling)
// @param isDirectRoom — whether this is a DM
// @param directUserId — the DM partner's userId (if known)
// @param heroes — list of hero userIds (from room summary)
// @param activeMembers — joined+invited members with their display names
// @param leftMembers — left members (for fallback in empty rooms)
RoomNameResult resolveRoomDisplayName(
    const std::string& roomName,
    const std::string& canonicalAlias,
    const std::string& currentMembership,
    bool isDirectRoom,
    const std::string& directUserId,
    const std::vector<std::string>& heroes,
    const std::vector<MemberInfo>& activeMembers,
    const std::vector<MemberInfo>& leftMembers,
    const std::string& currentUserId,
    const std::vector<std::string>& excludedUserIds,
    const RoomDisplayNameConfig& config = {});

// Compute the room avatar URL.
// Ported from RoomAvatarResolver.kt:48-84
// @param roomAvatarUrl — the m.room.avatar event URL (may be empty)
// @param isDirectRoom — whether this is a direct chat
// @param activeMembers — joined+invited members
// @param leftMembers — members who have left
// @param currentUserId — the current user's userId
std::string resolveRoomAvatar(
    const std::string& roomAvatarUrl,
    bool isDirectRoom,
    const std::vector<MemberInfo>& activeMembers,
    const std::vector<MemberInfo>& leftMembers,
    const std::string& currentUserId,
    const std::vector<std::string>& excludedUserIds = {});

// Format a list of member display names into a human-readable string.
// Handles separators (comma + "and") and truncation.
// @param names — list of display names
// @param maxNames — maximum number of names before adding "+N more"
std::string formatMemberNameList(const std::vector<std::string>& names, int maxNames = 3);

// Check if a room name comes from an m.room.name state event.
inline bool isRoomNameFromState(const RoomNameResult& result) {
    return result.isFromState;
}

// Determine the source of the room name.
inline RoomNameSource getNameSource(const RoomNameResult& result) {
    return result.source;
}

// Format hero display names as a single string for room list display.
// Uses formatMemberNameList with the heroes list.
std::string formatRoomHeroesList(
    const std::vector<std::string>& heroIds,
    const std::vector<MemberInfo>& members,
    int maxToShow = 3);

} // namespace progressive

#endif // PROGRESSIVE_MEMBERSHIP_UTILS_HPP
