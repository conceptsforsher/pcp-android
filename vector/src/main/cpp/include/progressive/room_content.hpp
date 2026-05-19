#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>

namespace progressive {

// ==== Membership ====
//
// Original Kotlin (Membership.kt:26-50):
//   enum class Membership {
//       NONE, @Json(name="invite") INVITE, @Json(name="join") JOIN,
//       @Json(name="knock") KNOCK, @Json(name="leave") LEAVE,
//       @Json(name="ban") BAN
//   }

enum class Membership {
    NONE = 0,
    INVITE = 1,
    JOIN = 2,
    KNOCK = 3,
    LEAVE = 4,
    BAN = 5
};

const char* membershipToString(Membership m);
Membership membershipFromString(const std::string& s);

inline bool isActiveMembership(Membership m) {
    return m == Membership::INVITE || m == Membership::JOIN;
}

inline bool isLeftMembership(Membership m) {
    return m == Membership::KNOCK || m == Membership::LEAVE || m == Membership::BAN;
}

// ==== RoomMemberContent ====
//
// Original Kotlin (RoomMemberContent.kt:25-41):
//   data class RoomMemberContent(
//       @Json(name="membership") membership: Membership,
//       @Json(name="reason") reason, @Json(name="displayname") displayName,
//       @Json(name="avatar_url") avatarUrl, @Json(name="is_direct") isDirect,
//       @Json(name="third_party_invite") thirdPartyInvite,
//       @Json(name="unsigned") unsignedData
//   )

struct Invite {
    std::string displayName;
    std::string signedToken;
    std::string mxId;
};

struct RoomMemberContent {
    Membership membership = Membership::NONE;
    std::string reason;              // "reason" key — optional kick/ban reason
    std::string displayName;         // "displayname" key
    std::string avatarUrl;           // "avatar_url" key
    bool isDirect = false;           // "is_direct" key
    Invite thirdPartyInvite;         // "third_party_invite" key
    std::string unsignedData;        // "unsigned" key — raw JSON

    // Original Kotlin: val safeReason = reason?.takeIf { it.isNotBlank() }
    std::string safeReason() const {
        if (reason.empty()) return "";
        for (char c : reason) if (c != ' ') return reason;
        return "";
    }
};

// ==== RoomMemberSummary ====
//
// Original Kotlin (RoomMemberSummary.kt:25-33):
//   data class RoomMemberSummary(
//       membership, userId, userPresence, displayName, avatarUrl
//   )

struct RoomMemberSummary {
    Membership membership = Membership::NONE;
    std::string userId;
    std::string userPresence;        // "online", "unavailable", "offline"
    std::string displayName;
    std::string avatarUrl;
};

// ==== PowerLevelsContent ====
//
// Original Kotlin (PowerLevelsContent.kt:27-68):
//   data class PowerLevelsContent(
//       ban, kick, invite, redact, events_default, events,
//       users_default, users, state_default, notifications
//   )

struct RoomPowerLevelsContent {
    int ban = 50;                    // "ban" key — default 50
    int kick = 50;                   // "kick" key — default 50
    int invite = 0;                  // "invite" key — default 0
    int redact = 50;                 // "redact" key — default 50
    int eventsDefault = 0;           // "events_default" key — default 0
    int usersDefault = 0;            // "users_default" key — default 0
    int stateDefault = 50;           // "state_default" key — default 50
    std::unordered_map<std::string, int> events;     // "events" key
    std::unordered_map<std::string, int> users;      // "users" key
    int notificationRoomLevel = 50;  // "notifications.room" key — default 50

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
        if (key == "room") return notificationRoomLevel;
        return 0;
    }
};

// ==== Room State Content Types ====
//
// Original Kotlin (RoomNameContent.kt:24-26):
//   data class RoomNameContent(@Json(name="name") name: String?)

struct RoomNameContent {
    std::string name;                // "name" key
};

// Original Kotlin (RoomTopicContent.kt:22-24):
//   data class RoomTopicContent(@Json(name="topic") topic: String?)

struct RoomTopicContent {
    std::string topic;               // "topic" key
};

// Original Kotlin (RoomAvatarContent.kt:22-24):
//   data class RoomAvatarContent(@Json(name="url") avatarUrl: String?)

struct RoomAvatarContent {
    std::string avatarUrl;           // "url" key — MXC URI
};

// Original Kotlin (RoomCanonicalAliasContent.kt:25-37):
//   data class RoomCanonicalAliasContent(
//       @Json(name="alias") canonicalAlias: String?,
//       @Json(name="alt_aliases") alternativeAliases: List<String>?
//   )

struct RoomCanonicalAliasContent {
    std::string canonicalAlias;              // "alias" key
    std::vector<std::string> alternativeAliases; // "alt_aliases" key
};

// Original Kotlin (RoomAliasesContent.kt:28-30):
//   data class RoomAliasesContent(@Json(name="aliases") aliases: List<String>)

struct RoomAliasesContent {
    std::vector<std::string> aliases;  // "aliases" key
};

// ==== Room Join Rules ====
//
// Original Kotlin (RoomJoinRules.kt:25-32):
//   enum class RoomJoinRules(val value: String) {
//       PUBLIC("public"), INVITE("invite"), KNOCK("knock"),
//       PRIVATE("private"), RESTRICTED("restricted")
//   }

enum class RoomJoinRules {
    PUBLIC = 0,
    INVITE = 1,
    KNOCK = 2,
    PRIVATE = 3,
    RESTRICTED = 4
};

const char* roomJoinRulesToString(RoomJoinRules r);
RoomJoinRules roomJoinRulesFromString(const std::string& s);

// Original Kotlin (RoomJoinRulesContent.kt:26-43):
//   data class RoomJoinRulesContent(
//       @Json(name="join_rule") joinRulesStr,
//       @Json(name="allow") allowList
//   )
struct RoomJoinRulesAllowEntry {
    std::string type;        // "m.room_membership"
    std::string roomId;      // room ID for allowed members
};

struct RoomJoinRulesContent {
    RoomJoinRules joinRules = RoomJoinRules::INVITE;
    std::vector<RoomJoinRulesAllowEntry> allowList; // "allow" key
};

// ==== Room History Visibility ====
//
// Original Kotlin (RoomHistoryVisibility.kt:26-54):
//   enum class RoomHistoryVisibility(val value: String) {
//       WORLD_READABLE("world_readable"), SHARED("shared"),
//       INVITED("invited"), JOINED("joined")
//   }

enum class RoomHistoryVisibility {
    WORLD_READABLE = 0,
    SHARED = 1,
    INVITED = 2,
    JOINED = 3
};

const char* roomHistoryVisibilityToString(RoomHistoryVisibility v);
RoomHistoryVisibility roomHistoryVisibilityFromString(const std::string& s);

inline bool shouldShareHistory(RoomHistoryVisibility v) {
    return v == RoomHistoryVisibility::WORLD_READABLE
        || v == RoomHistoryVisibility::SHARED;
}

// Original Kotlin (RoomHistoryVisibilityContent.kt:24-33):
//   data class RoomHistoryVisibilityContent(
//       @Json(name="history_visibility") historyVisibilityStr
//   )

struct RoomHistoryVisibilityContent {
    RoomHistoryVisibility historyVisibility = RoomHistoryVisibility::SHARED;
};

// ==== Room Guest Access ====
//
// Original Kotlin (GuestAccess enum from RoomGuestAccessContent.kt:41-44):
//   enum class GuestAccess(val value: String) {
//       @Json(name="can_join") CanJoin("can_join"),
//       @Json(name="forbidden") Forbidden("forbidden")
//   }

enum class GuestAccess {
    CAN_JOIN = 0,
    FORBIDDEN = 1
};

const char* guestAccessToString(GuestAccess g);
GuestAccess guestAccessFromString(const std::string& s);

// Original Kotlin (RoomGuestAccessContent.kt:30-37):
//   data class RoomGuestAccessContent(
//       @Json(name="guest_access") guestAccessStr
//   )

struct RoomGuestAccessContent {
    GuestAccess guestAccess = GuestAccess::FORBIDDEN;
};

// ==== Room Encryption Algorithm ====
//
// Original Kotlin (RoomEncryptionAlgorithm.kt:24-31):
//   sealed class RoomEncryptionAlgorithm {
//       object Megolm : SupportedAlgorithm(MXCRYPTO_ALGORITHM_MEGOLM)
//       data class UnsupportedAlgorithm(val name: String?)
//   }

constexpr const char* MEGOLM_ALGORITHM = "m.megolm.v1.aes-sha2";

struct RoomEncryptionAlgorithm {
    std::string algorithm;           // e.g. "m.megolm.v1.aes-sha2"
    bool isSupported = false;

    static RoomEncryptionAlgorithm megolm() {
        return {MEGOLM_ALGORITHM, true};
    }

    static RoomEncryptionAlgorithm unsupported(const std::string& name) {
        return {name, false};
    }
};

// ==== Read Receipt ====
//
// Original Kotlin (ReadReceipt.kt:19-23):
//   data class ReadReceipt(roomMember, originServerTs, threadId)

struct RoomReadReceipt {
    RoomMemberSummary roomMember;
    int64_t originServerTs = 0;
    std::string threadId;            // null for main timeline
};

// ==== Signed (Invite Signature) ====
//
// Original Kotlin (Signed.kt:25-30):
//   data class Signed(token, signatures, mxid)

struct Signed {
    std::string token;
    std::string signatures;          // Raw JSON object (Any in Kotlin)
    std::string mxId;
};

// ==== Room Type ====
//
// Original Kotlin (RoomType.kt:19-21):
//   object RoomType { const val SPACE = "m.space" }

namespace RoomType {
    constexpr const char* SPACE = "m.space";
}

// ==== Versioning State ====
//
// Original Kotlin (VersioningState.kt:21-33):
//   enum class VersioningState {
//       NONE, UPGRADED_ROOM_NOT_JOINED, UPGRADED_ROOM_JOINED
//   }

enum class RoomVersioningState {
    NONE = 0,
    UPGRADED_ROOM_NOT_JOINED = 1,
    UPGRADED_ROOM_JOINED = 2
};

inline bool isRoomUpgraded(RoomVersioningState s) {
    return s != RoomVersioningState::NONE;
}

const char* roomVersioningStateToString(RoomVersioningState s);
RoomVersioningState roomVersioningStateFromString(const std::string& s);

// ==== JSON Parsing ====

RoomMemberContent parseRoomMemberContent(const std::string& contentJson);
RoomPowerLevelsContent parsePowerLevelsContent(const std::string& contentJson);
RoomNameContent parseRoomNameContent(const std::string& contentJson);
RoomTopicContent parseRoomTopicContent(const std::string& contentJson);
RoomAvatarContent parseRoomAvatarContent(const std::string& contentJson);
// ==== Room Server ACL ====
//
// Original Kotlin (RoomServerAclContent.kt:26-49):
//   data class RoomServerAclContent(allowIpLiterals, allowList, denyList)
//   companion object { const val ALL = "*" }

constexpr const char* ACL_ALL = "*";

struct RoomServerAclContent {
    bool allowIpLiterals = true;         // "allow_ip_literals" key
    std::vector<std::string> allowList;  // "allow" key — wildcards supported
    std::vector<std::string> denyList;   // "deny" key — wildcards supported
};

// ==== Room Third-Party Invite ====
//
// Original Kotlin (RoomThirdPartyInviteContent.kt:28-43):
//   data class RoomThirdPartyInviteContent(displayName, keyValidityUrl, publicKey, publicKeys)

struct PublicKeyInfo {
    std::string keyValidityUrl;          // "key_validity_url" key
    std::string publicKey;               // "public_key" key — base64 ed25519
};

struct RoomThirdPartyInviteContent {
    std::string displayName;             // "display_name" key
    std::string keyValidityUrl;          // "key_validity_url" key
    std::string publicKey;               // "public_key" key (legacy)
    std::vector<PublicKeyInfo> publicKeys; // "public_keys" key
};

// ==== Room Stripped State ====
//
// Original Kotlin (RoomStrippedState.kt:26-103):
//   data class RoomStrippedState(aliases, canonicalAlias, name, numJoinedMembers,
//       roomId, topic, worldReadable, guestCanJoin, avatarUrl, isFederated,
//       isEncrypted, roomType, membership)

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
    std::string roomType;                // e.g. "m.space"
    std::string membership;              // e.g. "leave"

    // Original Kotlin: getPrimaryAlias()
    std::string getPrimaryAlias() const {
        if (!canonicalAlias.empty()) return canonicalAlias;
        if (!aliases.empty()) return aliases[0];
        return "";
    }
};

// ==== Room Directory Visibility ====
//
// Original Kotlin (RoomDirectoryVisibility.kt:23-26):
//   enum class RoomDirectoryVisibility { PRIVATE, PUBLIC }

enum class RoomDirectoryVisibility {
    PRIVATE = 0,    // "private"
    PUBLIC = 1      // "public"
};
const char* roomDirectoryVisibilityToString(RoomDirectoryVisibility v);
RoomDirectoryVisibility roomDirectoryVisibilityFromString(const std::string& s);

// ==== Room Tag ====
//
// Original Kotlin (RoomTag.kt:21-31):
//   data class RoomTag(name, order)

namespace RoomTag {
    constexpr const char* FAVOURITE = "m.favourite";
    constexpr const char* LOW_PRIORITY = "m.lowpriority";
    constexpr const char* SERVER_NOTICE = "m.server_notice";
}

struct RoomTagData {
    std::string name;
    double order = 0.0;
};

// Original Kotlin (RoomTagContent.kt:25-27):
//   data class RoomTagContent(tags: Map<String, Map<String, Any>>)
struct RoomTagContent {
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> tags;
};

// ==== Public Room (Directory) ====
//
// Original Kotlin (PublicRoom.kt:26-79):
//   data class PublicRoom(aliases, canonicalAlias, name, numJoinedMembers,
//       roomId, topic, worldReadable, guestCanJoin, avatarUrl, isFederated)

struct PublicRoom {
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

    // Original Kotlin: getPrimaryAlias()
    std::string getPrimaryAlias() const {
        if (!canonicalAlias.empty()) return canonicalAlias;
        if (!aliases.empty()) return aliases[0];
        return "";
    }
};

// Original Kotlin (PublicRoomsResponse.kt:26-47):
//   data class PublicRoomsResponse(nextBatch, prevBatch, chunk, totalRoomCountEstimate)
struct PublicRoomsResponse {
    std::string nextBatch;               // "next_batch" key
    std::string prevBatch;               // "prev_batch" key
    std::vector<PublicRoom> chunk;       // "chunk" key
    int totalRoomCountEstimate = 0;      // "total_room_count_estimate" key
};

// Original Kotlin (PublicRoomsParams.kt:25-51):
//   data class PublicRoomsParams(limit, since, filter, includeAllNetworks, thirdPartyInstanceId)
struct PublicRoomsParams {
    int limit = 0;
    std::string since;                   // pagination token
    std::string searchTerm;              // "filter.generic_search_term"
    std::string filter;                  // JNI compat alias for searchTerm
    bool includeAllNetworks = false;
    std::string thirdPartyInstanceId;
};

// ==== Third Party Protocol ====
//
// Original Kotlin (ThirdPartyProtocol.kt:26-67):
//   data class ThirdPartyProtocol(userFields, locationFields, icon, fieldTypes, instances)

struct FieldType {
    std::string regexp;                  // "regexp" key — validation pattern
    std::string placeholder;             // "placeholder" key — example value
};

struct ThirdPartyProtocolInstance {
    std::string desc;                    // "desc" key — human-readable name
    std::string icon;                    // "icon" key
    std::string networkId;               // "network_id" key
    std::string instanceId;              // "instance_id" key
    std::string botUserId;               // "bot_user_id" key
};

struct ThirdPartyProtocol {
    std::vector<std::string> userFields;     // "user_fields" key
    std::vector<std::string> locationFields; // "location_fields" key
    std::string icon;                        // "icon" key
    std::unordered_map<std::string, FieldType> fieldTypes; // "field_types" key
    std::vector<ThirdPartyProtocolInstance> instances;    // "instances" key
};

// ==== Space Info ====
//
// Original Kotlin (SpaceChildInfo.kt:23-36):
//   data class SpaceChildInfo(childRoomId, isKnown, roomType, name, topic,
//       avatarUrl, order, activeMemberCount, viaServers, parentRoomId,
//       suggested, canonicalAlias, aliases, worldReadable)

struct SpaceChildInfo {
    std::string childRoomId;
    bool isKnown = false;
    std::string roomType;
    std::string name;
    std::string topic;
    std::string avatarUrl;
    std::string order;                        // fractional index for sorting
    int activeMemberCount = 0;
    std::vector<std::string> viaServers;
    std::string parentRoomId;
    bool suggested = false;
    std::string canonicalAlias;
    std::vector<std::string> aliases;
    bool worldReadable = false;
};

// Original Kotlin (SpaceParentInfo.kt:21-26):
//   data class SpaceParentInfo(parentId, roomSummary, canonical, viaServers)

struct SpaceParentInfo {
    std::string parentId;
    std::string roomSummaryJson;              // RoomSummary serialized
    bool canonical = false;
    std::vector<std::string> viaServers;
};

// ==== Local Room State ====
//
// Original Kotlin (LocalRoomCreationState.kt:23-28):
//   enum class LocalRoomCreationState { NOT_CREATED, CREATING, FAILURE, CREATED }

enum class LocalRoomCreationState {
    NOT_CREATED = 0,
    CREATING = 1,
    FAILURE = 2,
    CREATED = 3
};

// Original Kotlin (LocalRoomSummary.kt:28-43):
//   data class LocalRoomSummary(roomId, roomSummary, createRoomParams,
//       replacementRoomId, creationState)

struct LocalRoomSummary {
    std::string roomId;
    std::string roomSummaryJson;              // RoomSummary serialized
    std::string createRoomParamsJson;         // CreateRoomParams serialized
    std::string replacementRoomId;
    LocalRoomCreationState creationState = LocalRoomCreationState::NOT_CREATED;
};

// ==== JSON Parsing (extended) ====

// Room server ACL
RoomServerAclContent parseRoomServerAclContent(const std::string& contentJson);

// Wildcard pattern match (* matches zero or more chars, ? matches exactly one).
// Used for server ACL hostname evaluation.
bool wildcardMatch(const std::string& pattern, const std::string& value);

// Check if a server is allowed to participate in a room based on ACL content.
// Returns true if the server hostname matches an allow entry and no deny entry.
// If allowList is empty, all servers are denied (except for the special case
// where the ACL has never been set — caller should handle this separately).
bool isServerAllowed(const std::string& serverName, const RoomServerAclContent& acl);

// Room third-party invite
RoomThirdPartyInviteContent parseRoomThirdPartyInvite(const std::string& contentJson);
// Room stripped state
RoomStrippedState parseRoomStrippedState(const std::string& json);
// Public rooms
PublicRoom parsePublicRoom(const std::string& json);
PublicRoomsResponse parsePublicRoomsResponse(const std::string& json);

RoomCanonicalAliasContent parseRoomCanonicalAliasContent(const std::string& contentJson);
RoomJoinRulesContent parseRoomJoinRulesContent(const std::string& contentJson);
RoomHistoryVisibilityContent parseRoomHistoryVisibilityContent(const std::string& contentJson);
RoomGuestAccessContent parseRoomGuestAccessContent(const std::string& contentJson);

// Evaluate whether a user can join a room based on join rules.
// userIsInvited: true if the user has a pending invite
// userIsMember: true if the user is already in the room
// isMemberOfAllowedRoom: true if user belongs to a room in the allowed list (for restricted joins)
bool canJoinRoom(const RoomJoinRulesContent& rules, bool userIsInvited,
                  bool userIsMember, bool isMemberOfAllowedRoom);

// Join rule convenience checks
inline bool isPublicRoom(const RoomJoinRulesContent& rules) {
    return rules.joinRules == RoomJoinRules::PUBLIC;
}
inline bool isInviteOnly(const RoomJoinRulesContent& rules) {
    return rules.joinRules == RoomJoinRules::INVITE || rules.joinRules == RoomJoinRules::PRIVATE;
}

// History visibility convenience checks
inline bool isHistoryPubliclyVisible(const RoomHistoryVisibilityContent& vis) {
    return vis.historyVisibility == RoomHistoryVisibility::WORLD_READABLE;
}

// Guest access convenience check
inline bool areGuestsAllowed(const RoomGuestAccessContent& access) {
    return access.guestAccess == GuestAccess::CAN_JOIN;
}

// ==== Room Event Content Wrappers ====
//
// These types represent the complete event content JSON for room state events.
// They extend the simple state content structs with full Matrix event content fields.

// ==== RoomTopicEventContent ====
// Original Kotlin (RoomTopicContent.kt:26-27):
//   data class RoomTopicContent(@Json(name="topic") topic: String?)
// Full event content for m.room.topic

struct RoomTopicEventContent {
    std::string topic;
};

RoomTopicEventContent parseRoomTopicEventContent(const std::string& contentJson);
std::string buildRoomTopicEventContent(const RoomTopicEventContent& content);

// ==== RoomAvatarEventContent ====
// Original Kotlin (RoomAvatarContent.kt:26-27):
//   data class RoomAvatarContent(@Json(name="url") avatarUrl: String?)
// Full event content for m.room.avatar, with thumbnail info sub-object

struct RoomAvatarInfo {
    int w = 0;
    int h = 0;
    int size = 0;
    std::string mimetype;
};

struct RoomAvatarEventContent {
    std::string url;
    std::string thumbnailUrl;
    RoomAvatarInfo thumbnailInfo;
};

RoomAvatarEventContent parseRoomAvatarEventContent(const std::string& contentJson);
std::string buildRoomAvatarEventContent(const RoomAvatarEventContent& content);

// ==== RoomNameEventContent ====
// Original Kotlin (RoomNameContent.kt:26-27):
//   data class RoomNameContent(@Json(name="name") name: String?)
// Full event content for m.room.name

struct RoomNameEventContent {
    std::string name;
};

RoomNameEventContent parseRoomNameEventContent(const std::string& contentJson);
std::string buildRoomNameEventContent(const RoomNameEventContent& content);

// ==== Room Aliases Event Content Builders ====
// Original Kotlin (RoomAliasesContent.kt:28-30):
//   data class RoomAliasesContent(@Json(name="aliases") aliases: List<String>)
// Builder for m.room.aliases event content (deprecated but still used)

std::string buildRoomAliasesContent(const std::string& roomId, const std::vector<std::string>& aliases);

// ==== Room Canonical Alias Event Content Builder ====
// Original Kotlin (RoomCanonicalAliasContent.kt:25-37):
//   data class RoomCanonicalAliasContent(alias: String?, alt_aliases: List<String>?)
// Builder for m.room.canonical_alias event content

std::string buildCanonicalAliasContent(const std::string& canonicalAlias, const std::vector<std::string>& altAliases);

// ==== Room Alias Resolution (EXPAND) ====
//
// Ported from RoomAliasDescription.kt, GetRoomIdByAliasTask.kt,
// DirectoryAPI.kt (resolveAlias, createAlias, deleteAlias endpoints).

// Original Kotlin (RoomAliasDescription.kt:26-34):
//   data class RoomAliasDescription(roomId: String, servers: List<String>)
struct RoomAliasMapping {
    std::string roomId;                          // resolved room ID
    std::vector<std::string> servers;            // servers that know about this alias
    std::string canonicalAlias;                  // canonical alias for the room
    std::vector<std::string> aliases;            // all known aliases for the room
};

// Original Kotlin: GetRoomIdByAliasTask — GET /_matrix/client/r0/directory/room/{alias}
// Resolve a room alias to a room ID. Returns empty RoomAliasMapping.roomId on failure.
RoomAliasMapping resolveRoomAlias(const std::string& roomAlias, const std::string& apiResponseJson);

// Original Kotlin: Retrofit @GET("directory/room/{alias}")
// Build the request URL path for alias resolution.
// Returns the endpoint path: "/_matrix/client/r0/directory/room/{alias}"
std::string buildAliasResolveRequest(const std::string& roomAlias);

// Original Kotlin: Parse API response {"room_id": "...", "servers": [...]}
// Manual JSON parser for the alias resolution response.
RoomAliasMapping parseAliasResolveResponse(const std::string& responseJson);

// Original Kotlin: DirectoryAPI.setRoomAlias — PUT /directory/room/{alias}
// Build the JSON body: {"room_id": "!abc:server"}
std::string buildCreateAliasRequest(const std::string& roomAlias, const std::string& roomId);

// Original Kotlin: DirectoryAPI.deleteRoomAlias — DELETE /directory/room/{alias}
// Build the request path for alias deletion.
// Returns the endpoint path: "/_matrix/client/r0/directory/room/{alias}"
std::string buildDeleteAliasRequest(const std::string& roomAlias);

// ==== Alias Parsing Utilities (EXPAND) ====
//
// Ported from RoomAliasLocalpart.kt alias validation/formatting helpers.

// Original Kotlin: alias parsing — split "#localpart:domain" into parts.
struct RoomAliasLocalpart {
    std::string localpart;                       // e.g. "myroom" from #myroom:example.org
    std::string domain;                          // e.g. "example.org"
};

// Original Kotlin: parse an alias string into localpart and domain components.
// Input: "#myroom:example.org" → localpart="myroom", domain="example.org"
RoomAliasLocalpart parseAliasLocalpart(const std::string& roomAlias);

// Original Kotlin: format "#localpart:domain" from components.
std::string formatRoomAlias(const std::string& localpart, const std::string& domain);

// Original Kotlin: validate alias format — must start with '#', contain ':',
// have non-empty localpart and domain.
bool isValidAlias(const std::string& roomAlias);

// Original Kotlin: check if alias is available based on API response.
// Returns true if the response indicates the alias is NOT currently in use.
// (An empty room_id in the response means the alias is available.)
bool isAliasAvailable(const std::string& responseJson);

} // namespace progressive
