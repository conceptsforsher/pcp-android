#ifndef PROGRESSIVE_ROOM_NAME_HPP
#define PROGRESSIVE_ROOM_NAME_HPP

#include <string>
#include <vector>

namespace progressive {

// ---- Room Display Name Resolver ----
// Faithful port from original Kotlin:
//   org.matrix.android.sdk.internal.session.room.membership.RoomDisplayNameResolver.kt (184 lines)
//
// This implements the room name calculation algorithm from matrix-js-sdk:
//   https://github.com/matrix-org/matrix-js-sdk/blob/develop/lib/models/room.js#L617
//
// Algorithm (from original Kotlin:59-166):
//   1. If room has m.room.name state event → use that name
//   2. If room has m.room.canonical_alias → use the alias
//   3. For INVITE rooms → show inviter's name
//   4. For JOIN rooms → compute from members:
//      a. Use "heroes" list if available (up to 5)
//      b. Otherwise take other members (not self, not excluded)
//      c. Format: 0→empty, 1→"Alice", 2→"Alice and Bob",
//         3→"Alice, Bob and Carol", 4→"Alice, Bob, Carol and Dave",
//         5+→"Alice, Bob, Carol and N others"

enum class RoomMembership {
    Unknown,
    Join,
    Invite,
    Leave,
    Ban,
    Knock
};

struct RoomMember {
    std::string userId;
    std::string displayName;
    std::string avatarUrl;
    RoomMembership membership = RoomMembership::Unknown;
    bool isUnique = true;  // displayName is unique among members
};

struct RoomName {
    std::string name;          // display name
    std::string normalizedName; // normalized for sorting (lowercase, stripped)
};

// Compute the room display name using the matrix-js-sdk algorithm.
// Original Kotlin (RoomDisplayNameResolver.kt:resolve):
//   fun resolve(realm: Realm, roomId: String): RoomName
RoomName computeRoomDisplayName(
    const std::string& roomNameState,      // m.room.name content (empty if not set)
    const std::string& canonicalAlias,     // m.room.canonical_alias content
    const std::vector<RoomMember>& heroes, // hero users (empty if not set)
    const std::vector<RoomMember>& activeMembers, // active (joined + invited) members
    const std::vector<RoomMember>& leftMembers,   // left members
    const std::string& myUserId,           // current user ID
    const std::string& directUserId,       // DM partner (empty if not DM)
    const std::string& inviterName,        // inviter's display name (for invite rooms)
    int invitedCount,                      // number of invited members
    int joinedCount,                       // number of joined members
    bool isInvite,                         // this user is invited
    const std::vector<std::string>& excludedUserIds = {} // users to exclude
);

// Normalize a display name for sorting.
// Original Kotlin: Normalizer.normalize(this)
std::string normalizeRoomName(const std::string& name);

// Check if a display name is unique among members.
bool isUniqueDisplayName(const std::vector<RoomMember>& members, const std::string& displayName);

// Resolve member name with disambiguation.
// Original Kotlin: resolveRoomMemberName(member, helper)
//   val isUnique = helper.isUniqueDisplayName(member.displayName)
//   if (isUnique) displayName else "displayName (userId)"
std::string resolveMemberName(
    const RoomMember& member,
    const std::vector<RoomMember>& allMembers);

// Get a fallback name for an empty room.
// "Empty Room" for group rooms, user ID for DMs
std::string getEmptyRoomName(bool isDirect, const std::vector<std::string>& leftMemberNames);

// Format room name as JSON.
std::string roomNameToJson(const RoomName& name);

// Parse a room name state event content JSON.
// {"name": "My Room Name"}
std::string parseRoomNameContent(const std::string& contentJson);

// Parse a canonical alias state event content JSON.  
// {"alias": "#myroom:server.org"}
std::string parseCanonicalAliasContent(const std::string& contentJson);

} // namespace progressive

#endif // PROGRESSIVE_ROOM_NAME_HPP
