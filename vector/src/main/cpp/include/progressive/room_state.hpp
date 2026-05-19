#ifndef PROGRESSIVE_ROOM_STATE_HPP
#define PROGRESSIVE_ROOM_STATE_HPP

#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>

namespace progressive {

// ---- Room State Event Parsers ----
// Ported from: org.matrix.android.sdk.api.session.room.model.RoomJoinRulesData.kt
//              org.matrix.android.sdk.api.session.room.model.RSH_RoomHistoryVisibility.kt
//              org.matrix.android.sdk.api.session.room.model.RoomGuestAccess.kt
//              org.matrix.android.sdk.api.session.room.model.RoomCreate.kt

// ---- Room Join Rules ----
// Original Kotlin (RoomJoinRulesData.kt):
//   enum class JoinRules { PUBLIC, INVITE, KNOCK, PRIVATE, RESTRICTED }
//   data class RoomJoinRulesContent(@Json(name = "join_rule") val joinRule: String)

enum class JoinRule {
    Public,      // anyone can join
    Invite,      // only invited users can join
    Knock,       // users can request to join (MSC2403)
    Private,     // same as invite (legacy)
    Restricted,  // join if invited OR member of allowed room (MSC3083)
    Unknown
};

struct RoomJoinRulesData {
    JoinRule rule = JoinRule::Unknown;
    std::string rawRule;         // original string from event
    std::vector<std::string> allow;  // allowed room IDs for restricted rules
    bool valid = false;
};

// Parse m.room.join_rules event content.
// {"join_rule": "public"} → JoinRule::Public
// {"join_rule": "restricted", "allow": [{"room_id": "!abc:server", "type": "m.room_membership"}]}
RoomJoinRulesData parseJoinRules(const std::string& contentJson);

bool isPublicRoom(const RoomJoinRulesData& rules);
bool isInviteOnly(const RoomJoinRulesData& rules);
bool isKnockable(const RoomJoinRulesData& rules);

std::string joinRuleToString(JoinRule rule);
JoinRule joinRuleFromString(const std::string& rule);

// ---- Room History Visibility ----
// Original Kotlin (RSH_RoomHistoryVisibility.kt):
//   enum class HistoryVisibility { SHARED, INVITED, JOINED, WORLD_READABLE }
//   Who can see events in the room's history

enum class HistoryVisibility {
    WorldReadable,  // anyone can read
    Shared,         // members + invited + anyone who already joined
    Invited,        // members + invited users
    Joined,         // members only (joined at time of event)
    Unknown
};

struct RSH_RoomHistoryVisibility {
    HistoryVisibility visibility = HistoryVisibility::Unknown;
    std::string rawValue;
    bool valid = false;
};

// Parse m.room.history_visibility event content.
// {"history_visibility": "shared"} → HistoryVisibility::Shared
RSH_RoomHistoryVisibility parseHistoryVisibility(const std::string& contentJson);

bool isHistoryPubliclyVisible(const RSH_RoomHistoryVisibility& vis);
bool isHistoryVisibleToGuests(const RSH_RoomHistoryVisibility& vis);

std::string historyVisibilityToString(HistoryVisibility vis);
HistoryVisibility historyVisibilityFromString(const std::string& vis);

// ---- Room Guest Access ----
// Original Kotlin (RoomGuestAccess.kt):
//   data class RoomGuestAccess(@Json(name = "guest_access") val guestAccess: String)
//   "can_join" or "forbidden"

enum class GuestAccessType {
    CanJoin,     // guests can join
    Forbidden,   // guests not allowed
    Unknown
};

struct RoomGuestAccess {
    GuestAccessType access = GuestAccessType::Unknown;
    std::string rawValue;
    bool valid = false;
};

// Parse m.room.guest_access event content.
// {"guest_access": "can_join"} → GuestAccessType::CanJoin
RoomGuestAccess parseGuestAccess(const std::string& contentJson);

bool areGuestsAllowed(const RoomGuestAccess& access);

std::string guestAccessToString(GuestAccessType access);

// ---- Room Create ----
struct RoomCreate {
    std::string creator;         // user ID who created the room
    std::string roomVersion;     // room version (e.g. "9")
    bool isFederated = true;     // m.federate: true/false
    std::string predecessorRoomId; // for room upgrades
    std::string predecessorEventId; // last event in predecessor
    bool valid = false;
};

// Parse m.room.create event content.
RoomCreate parseRoomCreate(const std::string& contentJson);

// Check if this is an upgraded room (has predecessor).
bool isUpgradedRoom(const RoomCreate& create);

// ---- Room Tombstone ----
// Ported from: org.matrix.android.sdk...tombstone.RoomTombstoneContent.kt (35L)
//              org.matrix.android.sdk...tombstone.RoomTombstoneEventProcessor.kt (49L)
//
// Tombstone events signal that a room has been replaced by an upgraded version.

struct RoomTombstone {
    std::string body;
    std::string replacementRoomId;
    bool valid = false;
};

RoomTombstone parseTombstone(const std::string& contentJson);
bool isRoomUpgraded(const RoomTombstone& tombstone);
std::string tombstoneToJson(const RoomTombstone& tombstone);

// ---- Room Versioning State ----
// Ported from: org.matrix.android.sdk.api.session.room.model.RoomVersionState.kt (39L)
//
// Tracks whether a room has been upgraded and whether the user joined the new room.

enum class RoomVersionState {
    None,                       // not versioned
    UpgradedRoomNotJoined,      // upgraded but new room not joined
    UpgradedRoomJoined          // upgraded and new room joined
};

inline bool isVersioned(RoomVersionState state) { return state != RoomVersionState::None; }
inline std::string roomVersioningStateToString(RoomVersionState state) {
    switch (state) {
        case RoomVersionState::None: return "none";
        case RoomVersionState::UpgradedRoomNotJoined: return "upgraded_not_joined";
        case RoomVersionState::UpgradedRoomJoined: return "upgraded_joined";
    }
    return "unknown";
}

// ---- JSON Serialization ----

std::string joinRulesToJson(const RoomJoinRulesData& rules);
std::string historyVisibilityToJson(const RSH_RoomHistoryVisibility& vis);
std::string guestAccessToJson(const RoomGuestAccess& access);
std::string roomCreateToJson(const RoomCreate& create);

// ==== Membership ====
// Original Kotlin (Membership.kt:26-50):
//   enum class Membership { NONE, @Json(name="invite") INVITE, @Json(name="join") JOIN,
//       @Json(name="knock") KNOCK, @Json(name="leave") LEAVE, @Json(name="ban") BAN }

enum class Membership {
    NONE,
    INVITE,
    JOIN,
    KNOCK,
    LEAVE,
    BAN
};

inline bool isActiveMembership(Membership m) {
    return m == Membership::INVITE || m == Membership::JOIN;
}

inline bool isLeftMembership(Membership m) {
    return m == Membership::KNOCK || m == Membership::LEAVE || m == Membership::BAN;
}

std::string membershipToString(Membership m);
Membership membershipFromString(const std::string& s);

// ==== Invite (Third-Party) ====
// Original Kotlin (Invite.kt:26-29):
//   data class Invite(@Json(name="display_name") displayName: String, @Json(name="signed") signed: Signed)

struct ThirdPartyInviteInfo {
    std::string displayName;
    std::string signedToken;
    std::string mxId;
};

// ==== RoomMemberContent ====
// Original Kotlin (RoomMemberContent.kt:26-35):
//   data class RoomMemberContent(membership: Membership, reason: String?, displayName: String?,
//       avatarUrl: String?, isDirect: Boolean, thirdPartyInvite: Invite?, unsignedData: UnsignedData?)

struct RoomMemberContent {
    Membership membership = Membership::NONE;
    std::string reason;
    std::string displayName;
    std::string avatarUrl;
    bool isDirect = false;
    ThirdPartyInviteInfo thirdPartyInvite;
    std::string unsignedData;

    std::string safeReason() const {
        if (reason.empty()) return "";
        for (char c : reason) if (c != ' ') return reason;
        return "";
    }
};

RoomMemberContent parseRoomMemberContent(const std::string& contentJson);
std::string buildRoomMemberContent(const RoomMemberContent& content);

// ==== RoomMemberSummary ====
// Original Kotlin (RoomMemberSummary.kt:24-30):
//   data class RoomMemberSummary(membership: Membership, userId: String, userPresence: UserPresence?,
//       displayName: String?, avatarUrl: String?)

struct RoomMemberSummary {
    Membership membership = Membership::NONE;
    std::string userId;
    std::string userPresence;
    std::string displayName;
    std::string avatarUrl;
};

// ==== RoomAvatarContent ====
// Original Kotlin (RoomAvatarContent.kt:26-28):
//   data class RoomAvatarContent(@Json(name="url") avatarUrl: String?)

struct RoomAvatarContent {
    std::string url;
    std::string thumbnailUrl;
    std::string thumbnailInfo;
};

std::string buildRoomAvatarContent(const RoomAvatarContent& content);

// ==== RoomNameContent ====
// Original Kotlin (RoomNameContent.kt:25-27):
//   data class RoomNameContent(@Json(name="name") name: String?)

struct RoomNameContent {
    std::string name;
};

std::string buildRoomNameContent(const RoomNameContent& content);

// ==== RoomTopicContent ====
// Original Kotlin (RoomTopicContent.kt:22-24):
//   data class RoomTopicContent(@Json(name="topic") topic: String?)

struct RoomTopicContent {
    std::string topic;
};

std::string buildRoomTopicContent(const RoomTopicContent& content);

// ==== RoomAliasesContent ====
// Original Kotlin (RoomAliasesContent.kt:28-30):
//   data class RoomAliasesContent(@Json(name="aliases") aliases: List<String>)

struct RoomAliasesContent {
    std::vector<std::string> aliases;
};

std::string buildRoomAliasesContent(const RoomAliasesContent& content);

// ==== RoomServerAclContent ====
// Original Kotlin (RoomServerAclContent.kt:26-58):
//   data class RoomServerAclContent(allowIpLiterals: Boolean, allowList: List<String>, denyList: List<String>)
//   companion object { const val ALL = "*" }

constexpr const char* ACL_ALL = "*";

struct RoomServerAclContent {
    bool allowIpLiterals = true;
    std::vector<std::string> allowList;
    std::vector<std::string> denyList;
};

RoomServerAclContent parseRoomServerAclContent(const std::string& contentJson);
std::string buildRoomServerAclContent(const RoomServerAclContent& acl);
bool wildcardMatch(const std::string& pattern, const std::string& value);
bool isServerAllowed(const std::string& serverName, const RoomServerAclContent& acl);

// ==== RoomThirdPartyInviteContent ====
// Original Kotlin (RoomThirdPartyInviteContent.kt:27-65):
//   data class RoomThirdPartyInviteContent(displayName: String?, keyValidityUrl: String?,
//       publicKey: String?, publicKeys: List<PublicKeys>?)

struct PublicKeyInfo {
    std::string keyValidityUrl;
    std::string publicKey;
};

struct RoomThirdPartyInviteContent {
    std::string displayName;
    std::string keyValidityUrl;
    std::string publicKey;
    std::vector<PublicKeyInfo> publicKeys;
};

RoomThirdPartyInviteContent parseRoomThirdPartyInvite(const std::string& contentJson);
std::string buildRoomThirdPartyInviteContent(const RoomThirdPartyInviteContent& content);

// ==== RoomStrippedState ====
// Original Kotlin (RoomStrippedState.kt:24-111):
//   data class RoomStrippedState(aliases, canonicalAlias, name, numJoinedMembers, roomId,
//       topic, worldReadable, guestCanJoin, avatarUrl, isFederated, isEncrypted, roomType, membership)

struct RoomStrippedState {
    std::vector<std::string> aliases;
    std::string canonicalAlias;
    std::string name;
    int numJoinedMembers = 0;
    std::string roomId;
    std::string topic;
    bool worldReadable = false;
    bool guestCanJoin = false;
    std::string avatarUrl;
    bool isFederated = true;
    bool isEncrypted = false;
    std::string roomType;
    std::string membership;

    std::string getPrimaryAlias() const {
        if (!canonicalAlias.empty()) return canonicalAlias;
        if (!aliases.empty()) return aliases[0];
        return "";
    }
};

RoomStrippedState parseRoomStrippedState(const std::string& json);
std::string buildRoomStrippedState(const RoomStrippedState& state);

// ==== SpaceChildInfo ====
// Original Kotlin (SpaceChildInfo.kt:19-37):
//   data class SpaceChildInfo(childRoomId: String, isKnown: Boolean, roomType: String?,
//       name: String?, topic: String?, avatarUrl: String?, order: String?,
//       activeMemberCount: Int?, viaServers: List<String>, parentRoomId: String?,
//       suggested: Boolean?, canonicalAlias: String?, aliases: List<String>?, worldReadable: Boolean)

struct SpaceChildInfo {
    std::string childRoomId;
    bool isKnown = false;
    std::string roomType;
    std::string name;
    std::string topic;
    std::string avatarUrl;
    std::string order;
    int activeMemberCount = 0;
    std::vector<std::string> viaServers;
    std::string parentRoomId;
    bool suggested = false;
    std::string canonicalAlias;
    std::vector<std::string> aliases;
    bool worldReadable = false;
};

std::string buildSpaceChildContent(const SpaceChildInfo& info);

// ==== SpaceParentInfo ====
// Original Kotlin (SpaceParentInfo.kt:19-24):
//   data class SpaceParentInfo(parentId: String?, roomSummary: RoomSummary?,
//       canonical: Boolean?, viaServers: List<String>)

struct SpaceParentInfo {
    std::string parentId;
    std::string roomSummaryJson;
    bool canonical = false;
    std::vector<std::string> viaServers;
};

std::string buildSpaceParentContent(const SpaceParentInfo& info);

// ==== RoomEncryptionAlgorithm ====
// Original Kotlin (RoomEncryptionAlgorithm.kt:21-27):
//   sealed class RoomEncryptionAlgorithm {
//       abstract class SupportedAlgorithm(val alg: String)
//       object Megolm : SupportedAlgorithm(MXCRYPTO_ALGORITHM_MEGOLM)
//       data class UnsupportedAlgorithm(val name: String?)
//   }

constexpr const char* MEGOLM_ALGORITHM = "m.megolm.v1.aes-sha2";

struct RoomEncryptionAlgorithm {
    std::string algorithm;
    int64_t rotationPeriodMs = 0;
    int64_t rotationPeriodMsgs = 0;
    std::unordered_map<std::string, std::string> additionalParams;
    bool isSupported = false;

    static RoomEncryptionAlgorithm megolm() {
        return {MEGOLM_ALGORITHM, 0, 0, {}, true};
    }

    static RoomEncryptionAlgorithm unsupported(const std::string& name) {
        return {name, 0, 0, {}, false};
    }
};

std::string roomEncryptionAlgorithmToString(const RoomEncryptionAlgorithm& alg);
RoomEncryptionAlgorithm parseRoomEncryptionAlgorithm(const std::string& contentJson);
std::string buildRoomEncryptionContent(const RoomEncryptionAlgorithm& alg);

// ==== RoomTag ====
// Original Kotlin (RoomTag.kt:19-28):
//   data class RoomTag(name: String, order: Double?)
//   companion object { ROOM_TAG_FAVOURITE, ROOM_TAG_LOW_PRIORITY, ROOM_TAG_SERVER_NOTICE }

namespace RoomTag {
    constexpr const char* FAVOURITE = "m.favourite";
    constexpr const char* LOW_PRIORITY = "m.lowpriority";
    constexpr const char* SERVER_NOTICE = "m.server_notice";
}

struct RoomTagData {
    std::string name;
    double order = 0.0;
};

std::string buildRoomTagContent(const RoomTagData& tag);
RoomTagData parseRoomTagContent(const std::string& contentJson);

// ==== RoomDirectoryVisibility ====
// Original Kotlin (RoomDirectoryVisibility.kt:22-26):
//   enum class RoomDirectoryVisibility { @Json(name="private") PRIVATE, @Json(name="public") PUBLIC }

enum class RoomDirectoryVisibility {
    PRIVATE,
    PUBLIC
};

constexpr const char* directoryVisibilityToString(RoomDirectoryVisibility v) {
    return v == RoomDirectoryVisibility::PUBLIC ? "public" : "private";
}

inline RoomDirectoryVisibility directoryVisibilityFromString(const std::string& s) {
    if (s == "public") return RoomDirectoryVisibility::PUBLIC;
    return RoomDirectoryVisibility::PRIVATE;
}

std::string buildDirectoryVisibilityContent(RoomDirectoryVisibility visibility);

// ==== Room Members Load Status ====
// Original Kotlin (RoomMembersLoadStatusType.kt:19-22):
//   internal enum class RoomMembersLoadStatusType { NONE, LOADING, LOADED }

enum class RoomMembersLoadStatus {
    NONE,
    LOADING,
    LOADED
};

inline const char* roomMembersLoadStatusToString(RoomMembersLoadStatus s) {
    switch (s) {
        case RoomMembersLoadStatus::NONE:    return "NONE";
        case RoomMembersLoadStatus::LOADING: return "LOADING";
        case RoomMembersLoadStatus::LOADED:  return "LOADED";
    }
    return "UNKNOWN";
}

// ==== Room Member State (from RoomDataSource.kt + RoomMemberSummaryEntity) ====
// Original Kotlin: RoomMemberSummaryEntity fields
//   data class RoomMemberSummaryEntity(primaryKey, userId, roomId, displayName,
//       avatarUrl, membership, userPresenceEntity, isRoomCreator, joinedTs, invitedBy)

struct RoomMemberState {
    std::string userId;
    std::string displayName;
    std::string avatarUrl;
    Membership membership = Membership::NONE;
    int powerLevel = 0;
    bool isRoomCreator = false;
    int64_t joinedTs = 0;
    std::string invitedBy;          // userId of inviter (for invited members)
    std::string userPresence;       // "online", "offline", etc.
};

// Snapshot of all room members, ported from the aggregate member state.
// Original Kotlin: the merged state from RoomMemberHelper queries
struct RoomMembersState {
    std::string roomId;
    std::vector<RoomMemberState> members;
    int totalCount = 0;
    int joinedCount = 0;
    int invitedCount = 0;
    int leftCount = 0;
    int bannedCount = 0;
    RoomMembersLoadStatus loadStatus = RoomMembersLoadStatus::NONE;
    bool isTruncated = false;     // server-side truncation
};

// Query room members from a raw JSON API response.
// Ported from RoomDataSource + LoadRoomMembersTask response parsing.
// @param roomId — the room ID
// @param apiResponseJson — the raw /members response JSON
// @param isTruncated — whether the server truncated the result
// @param loadStatus — current load status of the member list
RoomMembersState queryRoomMembers(
    const std::string& roomId,
    const std::string& apiResponseJson,
    bool isTruncated,
    RoomMembersLoadStatus loadStatus = RoomMembersLoadStatus::LOADED);

// Get active members (joined + invited) filtered from RoomMembersState.
// Ported from RoomMemberHelper.queryActiveRoomMembersEvent()
std::vector<RoomMemberState> getActiveMembers(const RoomMembersState& state);

// Get all members except the current user.
std::vector<RoomMemberState> getOtherMembers(const RoomMembersState& state, const std::string& currentUserId);

// Check if a user is a member (joined or invited) in the room.
// Ported from RoomMemberHelper.getActiveRoomMemberIds().contains()
bool isMemberInRoom(const RoomMembersState& state, const std::string& userId);

// Get the count of joined members.
inline int getMemberCount(const RoomMembersState& state) {
    return state.joinedCount;
}

// Get the count of invited members.
inline int getInvitedMemberCount(const RoomMembersState& state) {
    return state.invitedCount;
}

// Get all joined member userIds.
std::vector<std::string> getJoinedMemberIds(const RoomMembersState& state);

// Get all active (joined+invited) member userIds.
std::vector<std::string> getActiveMemberIds(const RoomMembersState& state);

// Serialize RoomMembersState to JSON.
std::string roomMembersStateToJson(const RoomMembersState& state);

// ================================================================
// Power Levels Extended (EXPAND)
//
// Ported from PowerLevelsContent.kt, RoomMemberHelper.kt power level
// lookup, and room state change diff logic.
// ================================================================

// Original Kotlin (PowerLevelsContent.kt notifications sub-object):
//   "notifications": { "room": 50 }
// Extended from the basic RoomPowerLevelsContent in room_content.hpp
// with proper notification sub-object support.
struct PowerLevelsNotifications {
    int room = 50;             // "notifications.room" — default 50
    int message = 0;           // "notifications" — maps to room notification level
};

// Original Kotlin (PowerLevelsContent.kt:27-68) — extended struct with notifications sub-object.
struct RoomPowerLevelsContent {
    std::unordered_map<std::string, int> users;    // "users" key — per-user power levels
    int usersDefault = 0;                          // "users_default" — default PL for users
    std::unordered_map<std::string, int> events;   // "events" key — per-event-type PL required
    int eventsDefault = 0;                         // "events_default" — default PL for sending events
    int stateDefault = 50;                         // "state_default" — default PL for state events
    int ban = 50;                                  // "ban" key — default 50
    int kick = 50;                                 // "kick" key — default 50
    int redact = 50;                               // "redact" key — default 50
    int invite = 0;                                // "invite" key — default 0
    PowerLevelsNotifications notifications;        // "notifications" sub-object {room, message}

    // Original Kotlin: setUserPowerLevel(userId, powerLevel)
    void setUserPowerLevel(const std::string& userId, int powerLevel) {
        if (powerLevel == usersDefault) {
            users.erase(userId);
        } else {
            users[userId] = powerLevel;
        }
    }

    // Original Kotlin: notificationLevel(key)
    int notificationLevel(const std::string& key) const {
        if (key == "room") return notifications.room;
        return 0;
    }
};

// Original Kotlin: ContentMapper.map(...).toModel<PowerLevelsContent>()
// Parse m.room.power_levels event content JSON with full notifications sub-object.
RoomPowerLevelsContent parsePowerLevelsContent(const std::string& contentJson);

// Original Kotlin: build power_levels event content for PUT /state/m.room.power_levels
std::string buildPowerLevelsContent(const RoomPowerLevelsContent& content);

// Original Kotlin: RoomMemberHelper power level lookup — what PL is needed for an action?
// Returns the minimum power level required for the given action string.
// E.g., "m.room.name" → checks events map, then eventsDefault, then stateDefault.
int getMinimumPowerLevelForAction(const RoomPowerLevelsContent& pl, const std::string& action);

// Original Kotlin: RoomMemberHelper canUserPerformAction check
// Returns true if userLevel >= minimum required level for the action.
bool canUserPerformAction(const RoomPowerLevelsContent& pl, int userLevel, const std::string& action);

// Original Kotlin: PowerLevelsChange tracking from state event changes.
// Captures what changed between two power level states.
struct PowerLevelsChange {
    std::string userId;              // user who had PL changed
    int oldLevel = 0;                // previous power level
    int newLevel = 0;                // new power level
    std::string changedBy;           // user ID of who made the change
};

// Original Kotlin: compute power level diff between two state snapshots.
// Compares prevContent (old PL state) against newContent (current PL state).
// Returns list of changes per user, including newly added and removed users.
std::vector<PowerLevelsChange> computePowerLevelsChange(
    const RoomPowerLevelsContent& prevContent,
    const RoomPowerLevelsContent& newContent,
    const std::string& changedBy
);

} // namespace progressive

#endif // PROGRESSIVE_ROOM_STATE_HPP
