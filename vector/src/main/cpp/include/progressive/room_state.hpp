#ifndef PROGRESSIVE_ROOM_STATE_HPP
#define PROGRESSIVE_ROOM_STATE_HPP

#include <string>
#include <vector>
#include <cstdint>

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

} // namespace progressive

#endif // PROGRESSIVE_ROOM_STATE_HPP
