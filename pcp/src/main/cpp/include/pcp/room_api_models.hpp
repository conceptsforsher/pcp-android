#pragma once
// ============================================================================
// room_api_models.hpp — C++ translations of Matrix SDK API room model types
//
// Ported from:
//   matrix-sdk-android/src/main/java/org/matrix/android/sdk/api/session/room/model/
//
// These are @JsonClass data classes and enums that define the public
// Matrix room data model. Translating them to C++ provides a complete
// header-only type system for the native layer.
// ============================================================================

#include <string>
#include <vector>
#include <cstdint>

namespace pcp {
namespace room_models {

// ==== Membership (Membership.kt:20-38) ====
//
// Original Kotlin:
//   enum class Membership { NONE(""), INVITE("invite"), JOIN("join"),
//       LEAVE("leave"), BAN("ban"), KNOCK("knock"); }
enum class Membership {
    NONE, INVITE, JOIN, LEAVE, BAN, KNOCK
};
inline const char* membershipToString(Membership m) {
    switch (m) {
        case Membership::NONE: return "";
        case Membership::INVITE: return "invite";
        case Membership::JOIN: return "join";
        case Membership::LEAVE: return "leave";
        case Membership::BAN: return "ban";
        case Membership::KNOCK: return "knock";
    }
    return "";
}
inline Membership membershipFromString(const std::string& s) {
    if (s == "invite") return Membership::INVITE;
    if (s == "join") return Membership::JOIN;
    if (s == "leave") return Membership::LEAVE;
    if (s == "ban") return Membership::BAN;
    if (s == "knock") return Membership::KNOCK;
    return Membership::NONE;
}

// ==== RoomJoinRules (RoomJoinRules.kt:22-41) ====
//
// Original Kotlin:
//   enum class RoomJoinRules(val value: String) {
//       PUBLIC("public"), KNOCK("knock"), INVITE("invite"), PRIVATE("private"),
//       KNOCK_RESTRICTED("knock_restricted"); }
enum class RoomJoinRules { PUBLIC, KNOCK, INVITE, PRIVATE, KNOCK_RESTRICTED, UNKNOWN };
inline const char* joinRulesToString(RoomJoinRules r) {
    switch (r) {
        case RoomJoinRules::PUBLIC: return "public";
        case RoomJoinRules::KNOCK: return "knock";
        case RoomJoinRules::INVITE: return "invite";
        case RoomJoinRules::PRIVATE: return "private";
        case RoomJoinRules::KNOCK_RESTRICTED: return "knock_restricted";
        default: return "";
    }
}
inline RoomJoinRules joinRulesFromString(const std::string& s) {
    if (s == "public") return RoomJoinRules::PUBLIC;
    if (s == "knock") return RoomJoinRules::KNOCK;
    if (s == "invite") return RoomJoinRules::INVITE;
    if (s == "private") return RoomJoinRules::PRIVATE;
    if (s == "knock_restricted") return RoomJoinRules::KNOCK_RESTRICTED;
    return RoomJoinRules::UNKNOWN;
}

// ==== RoomJoinRulesContent (RoomJoinRulesContent.kt:22-29) ====
//
// Original Kotlin:
//   data class RoomJoinRulesContent(
//       @Json(name="join_rule") val joinRule: RoomJoinRules? = null)
struct RoomJoinRulesContent {
    RoomJoinRules joinRule = RoomJoinRules::UNKNOWN;
    std::string joinRuleRaw; // raw string from JSON
};

// ==== RoomJoinRulesAllowEntry (RoomJoinRulesAllowEntry.kt:21-29) ====
//
// Original Kotlin:
//   data class RoomJoinRulesAllowEntry(
//       @Json(name="type") val type: String?,
//       @Json(name="room_id") val roomId: String?)
struct RoomJoinRulesAllowEntry {
    std::string type;
    std::string roomId;
};

// ==== RoomHistoryVisibility (RoomHistoryVisibility.kt:21-39) ====
//
// Original Kotlin:
//   enum class RoomHistoryVisibility(val value: String) {
//       WORLD_READABLE, SHARED, INVITED, JOINED; }
enum class RoomHistoryVisibility { WORLD_READABLE, SHARED, INVITED, JOINED, UNKNOWN };
inline const char* historyVisibilityToString(RoomHistoryVisibility v) {
    switch (v) {
        case RoomHistoryVisibility::WORLD_READABLE: return "world_readable";
        case RoomHistoryVisibility::SHARED: return "shared";
        case RoomHistoryVisibility::INVITED: return "invited";
        case RoomHistoryVisibility::JOINED: return "joined";
        default: return "";
    }
}
inline RoomHistoryVisibility historyVisibilityFromString(const std::string& s) {
    if (s == "world_readable") return RoomHistoryVisibility::WORLD_READABLE;
    if (s == "shared") return RoomHistoryVisibility::SHARED;
    if (s == "invited") return RoomHistoryVisibility::INVITED;
    if (s == "joined") return RoomHistoryVisibility::JOINED;
    return RoomHistoryVisibility::UNKNOWN;
}

// ==== RoomHistoryVisibilityContent (RoomHistoryVisibilityContent.kt:21-26) ====
//
// Original Kotlin:
//   data class RoomHistoryVisibilityContent(
//       @Json(name="history_visibility") val historyVisibility: String = "shared")
struct RoomHistoryVisibilityContent {
    std::string historyVisibility = "shared";
    RoomHistoryVisibility getVisibility() const { return historyVisibilityFromString(historyVisibility); }
};

// ==== RoomDirectoryVisibility (RoomDirectoryVisibility.kt:21-27) ====
//
// Original Kotlin:
//   enum class RoomDirectoryVisibility(val value: String) {
//       PRIVATE("private"), PUBLIC("public"); }
enum class RoomDirectoryVisibility { PRIVATE, PUBLIC, UNKNOWN };
inline const char* directoryVisibilityToString(RoomDirectoryVisibility v) {
    switch (v) {
        case RoomDirectoryVisibility::PUBLIC: return "public";
        case RoomDirectoryVisibility::PRIVATE: return "private";
        default: return "private";
    }
}
inline RoomDirectoryVisibility directoryVisibilityFromString(const std::string& s) {
    if (s == "public") return RoomDirectoryVisibility::PUBLIC;
    if (s == "private") return RoomDirectoryVisibility::PRIVATE;
    return RoomDirectoryVisibility::UNKNOWN;
}

// ==== VersioningState (VersioningState.kt:22-34) ====
//
// Original Kotlin:
//   enum class VersioningState { NONE, UPGRADING_ROOM, UPGRADED_UPSTREAM } 
enum class VersioningState { NONE, UPGRADING_ROOM, UPGRADED_UPSTREAM };
inline const char* versioningStateToString(VersioningState v) {
    switch (v) {
        case VersioningState::UPGRADING_ROOM: return "upgrading_room";
        case VersioningState::UPGRADED_UPSTREAM: return "upgraded_upstream";
        default: return "none";
    }
}

// ==== RoomType (RoomType.kt:22-27) ====
//
// Original Kotlin:
//   object RoomType { const val SPACE = "m.space" }
namespace RoomType {
    constexpr const char* SPACE = "m.space";
    inline bool isSpace(const std::string& type) { return type == SPACE; }
}

// ==== RoomEncryptionAlgorithm (RoomEncryptionAlgorithm.kt:22-26) ====
//
// Original Kotlin:
//   @JsonClass(generateAdapter=false)
//   enum class RoomEncryptionAlgorithm(val value: String) {
//       MEGOLM_V1_AES_SHA2("m.megolm.v1.aes-sha2"); }
enum class RoomEncryptionAlgorithm { MEGOLM_V1_AES_SHA2, UNKNOWN };
inline const char* encryptionAlgorithmToString(RoomEncryptionAlgorithm a) {
    if (a == RoomEncryptionAlgorithm::MEGOLM_V1_AES_SHA2) return "m.megolm.v1.aes-sha2";
    return "";
}
inline RoomEncryptionAlgorithm encryptionAlgorithmFromString(const std::string& s) {
    if (s == "m.megolm.v1.aes-sha2") return RoomEncryptionAlgorithm::MEGOLM_V1_AES_SHA2;
    return RoomEncryptionAlgorithm::UNKNOWN;
}

// ==== RoomGuestAccessContent (RoomGuestAccessContent.kt:22-26) ====
//
// Original Kotlin:
//   data class RoomGuestAccessContent(
//       @Json(name="guest_access") val guestAccess: String? = "forbidden")
struct RoomGuestAccessContent {
    std::string guestAccess = "forbidden"; // "can_join" or "forbidden"
};

// ==== RoomNameContent (RoomNameContent.kt:21-24) ====
//
// Original Kotlin:
//   data class RoomNameContent(@Json(name="name") val name: String? = null)
struct RoomNameContent {
    std::string name;
    bool hasName() const { return !name.empty(); }
};

// ==== RoomTopicContent (RoomTopicContent.kt:21-24) ====
//
// Original Kotlin:
//   data class RoomTopicContent(@Json(name="topic") val topic: String? = null)
struct RoomTopicContent {
    std::string topic;
    bool hasTopic() const { return !topic.empty(); }
};

// ==== RoomAvatarContent (RoomAvatarContent.kt:21-26) ====
//
// Original Kotlin:
//   data class RoomAvatarContent(@Json(name="url") val url: String? = null,
//       @Json(name="info") val info: ThumbnailInfo? = null)
struct RoomAvatarContent {
    std::string url;
    // info: see ThumbnailInfo in message_content.hpp
    std::string infoJson; // raw JSON for thumbnail info
};

// ==== RoomCanonicalAliasContent (RoomCanonicalAliasContent.kt:21-26) ====
//
// Original Kotlin:
//   data class RoomCanonicalAliasContent(
//       @Json(name="alias") val alias: String? = null,
//       @Json(name="alt_aliases") val altAliases: List<String> = emptyList())
struct RoomCanonicalAliasContent {
    std::string alias;
    std::vector<std::string> altAliases;
};

// ==== RoomAliasesContent (RoomAliasesContent.kt:21-24) ====
//
// Original Kotlin:
//   data class RoomAliasesContent(
//       @Json(name="aliases") val aliases: List<String> = emptyList())
struct RoomAliasesContent {
    std::vector<std::string> aliases;
};

// ==== RoomTombstoneContent (tombstone/RoomTombstoneContent.kt:22-32) ====
//
// Original Kotlin:
//   data class RoomTombstoneContent(
//       @Json(name="body") val body: String? = null,
//       @Json(name="replacement_room") val replacementRoom: String? = null)
struct RoomTombstoneContent {
    std::string body;
    std::string replacementRoom;
    bool hasReplacementRoom() const { return !replacementRoom.empty(); }
};

// ==== RoomServerAclContent (RoomServerAclContent.kt:21-32) ====
//
// Original Kotlin:
//   data class RoomServerAclContent(
//       @Json(name="allow") val allow: List<String> = emptyList(),
//       @Json(name="deny") val deny: List<String> = emptyList(),
//       @Json(name="allow_ip_literals") val allowIpLiterals: Boolean = false)
struct RoomServerAclContent {
    std::vector<std::string> allow;
    std::vector<std::string> deny;
    bool allowIpLiterals = false;
};

// ==== RoomMemberContent (RoomMemberContent.kt:22-30) ====
//
// Original Kotlin:
//   data class RoomMemberContent(
//       @Json(name="membership") val membership: String? = null,
//       @Json(name="displayname") val displayName: String? = null,
//       @Json(name="avatar_url") val avatarUrl: String? = null,
//       @Json(name="reason") val reason: String? = null,
//       @Json(name="is_direct") val isDirect: Boolean? = null,
//       @Json(name="third_party_invite") val thirdPartyInvite: ThirdPartyInviteContent? = null)
struct RoomMemberContent {
    std::string membership;
    std::string displayName;
    std::string avatarUrl;
    std::string reason;
    bool isDirect = false;
    bool hasIsDirectField = false; // field presence
    std::string thirdPartyInviteJson; // raw JSON for ThirdPartyInviteContent
};

// ==== RoomThirdPartyInviteContent (RoomThirdPartyInviteContent.kt:22-30) ====
struct RoomThirdPartyInviteContent {
    std::string displayName;
    std::string keyValidityUrl;
    std::string publicKey;
    std::string publicKeysJson; // raw JSON array of public keys
};

// ==== RoomPowerLevelsContent → see power_levels.hpp for existing C++ version ====

// ==== RoomMemberSummary (RoomMemberSummary.kt:22-38) ====
//
// Original Kotlin:
//   data class RoomMemberSummary(
//       val userId: String, val displayName: String? = null,
//       val avatarUrl: String? = null, val membership: Membership? = null,
//       val isDirect: Boolean = false)
struct RoomMemberSummary {
    std::string userId;
    std::string displayName;
    std::string avatarUrl;
    Membership membership = Membership::NONE;
    bool isDirect = false;
};

// ==== ReadReceipt (ReadReceipt.kt:23-30) ====
//
// Original Kotlin:
//   data class ReadReceipt(val userId: String, val timestamp: Long,
//       val eventId: String, val originServerTs: Long? = null)
struct ReadReceipt {
    std::string userId;
    int64_t timestamp = 0;       // epoch ms
    std::string eventId;
    int64_t originServerTs = 0;
};

// ==== EventAnnotationsSummary (EventAnnotationsSummary.kt:24-30) ====
//
// Original Kotlin:
//   data class EventAnnotationsSummary(
//       val editSummary: EditAggregatedSummary?,
//       val reactionSummary: ReactionAggregatedSummary?,
//       val referencesSummary: ReferencesAggregatedSummary?
//   )
struct EditAggregatedSummary {
    std::string latestEditEventId;
    std::string latestEditContentJson;
    std::vector<std::string> sourceEvents;
    int64_t aggregationOriginTs = 0;
    int editCount = 0;
};

struct ReactionAggregatedSummary {
    int totalCount = 0;
    struct ReactionItem {
        std::string key;       // emoji unicode
        int count = 0;
        bool addedByMe = false;
        int64_t firstTs = 0;
    };
    std::vector<ReactionItem> reactions;
};

struct ReferencesAggregatedSummary {
    int totalCount = 0;
    std::string latestEventId;
};

struct EventAnnotationsSummary {
    EditAggregatedSummary editSummary;
    ReactionAggregatedSummary reactionSummary;
    ReferencesAggregatedSummary referencesSummary;
    bool hasEdits() const { return editSummary.editCount > 0; }
    bool hasReactions() const { return reactionSummary.totalCount > 0; }
};

// ==== PollResponseAggregatedSummary (PollResponseAggregatedSummary.kt:25-31) ====
//
// Original Kotlin:
//   data class PollResponseAggregatedSummary(
//       @Json(name="myAnswer") val myAnswer: List<String> = emptyList(),
//       @Json(name="totalAnswers") val totalAnswers: Int? = null,
//       @Json(name="answerToMembersMap") val answerToMembersMap:
//           Map<String, List<String>>? = null)
struct PollResponseAggregatedSummary {
    std::vector<std::string> myAnswer;
    int totalAnswers = 0;
    struct AnswerEntry {
        std::string answerId;
        std::vector<std::string> memberIds;
    };
    std::vector<AnswerEntry> answerToMembers;
};

// ==== PollSummaryContent (PollSummaryContent.kt:24-30) ====
struct PollSummaryContent {
    std::string question;
    struct AnswerOption {
        std::string id;
        std::string text;
    };
    std::vector<AnswerOption> answers;
    int totalVotes = 0;
    bool isClosed = false;
    std::string winnerId;
    int64_t closeTimestamp = 0;
};

// ==== SpaceChildInfo (SpaceChildInfo.kt:26-50) ====
//
// Original Kotlin:
//   data class SpaceChildInfo(
//       val childRoomId: String, val order: String? = null,
//       val suggested: Boolean = false, val viaServers: List<String> = emptyList(),
//       val name: String? = null, val topic: String? = null,
//       val avatarUrl: String? = null, val activeMemberCount: Int? = null,
//       val roomType: String? = null)
struct SpaceChildInfo {
    std::string childRoomId;
    std::string order;
    bool suggested = false;
    std::vector<std::string> viaServers;
    std::string name;
    std::string topic;
    std::string avatarUrl;
    int activeMemberCount = 0;
    std::string roomType;
};

// ==== SpaceParentInfo (SpaceParentInfo.kt:23-33) ====
//
// Original Kotlin:
//   data class SpaceParentInfo(
//       val parentRoomId: String, val canonical: Boolean = false,
//       val viaServers: List<String> = emptyList())
struct SpaceParentInfo {
    std::string parentRoomId;
    bool canonical = false;
    std::vector<std::string> viaServers;
};

// ==== Invite (Invite.kt:22-35) ====
//
// Original Kotlin:
//   data class Invite(val roomId: String, val inviterId: String? = null,
//       val inviterName: String? = null, val inviterAvatarUrl: String? = null,
//       val roomName: String? = null, val roomAvatarUrl: String? = null,
//       val isDirect: Boolean = false)
struct Invite {
    std::string roomId;
    std::string inviterId;
    std::string inviterName;
    std::string inviterAvatarUrl;
    std::string roomName;
    std::string roomAvatarUrl;
    bool isDirect = false;
};

// ==== RoomLocalEcho (localecho/RoomLocalEcho.kt:22-28) ====
//
// Original Kotlin:
//   data class RoomLocalEcho(val sendState: SendState? = null)
enum class SendState { UNSENT, SENDING, SENT, FAILED_UNKNOWN_DEVICES, UNDELIVERABLE };
inline const char* sendStateToString(SendState s) {
    switch (s) {
        case SendState::UNSENT: return "unsent";
        case SendState::SENDING: return "sending";
        case SendState::SENT: return "sent";
        case SendState::FAILED_UNKNOWN_DEVICES: return "failed_unknown_devices";
        case SendState::UNDELIVERABLE: return "undeliverable";
    }
    return "unsent";
}

struct RoomLocalEcho {
    SendState sendState = SendState::UNSENT;
};

// ==== RoomStrippedState (RoomStrippedState.kt:24-34) ====
//
// Original Kotlin:
//   data class RoomStrippedState(
//       val roomId: String, val name: String? = null,
//       val canonicalAlias: String? = null, val avatarUrl: String? = null,
//       val numJoinedMembers: Int? = null, val topic: String? = null,
//       val inviterId: String? = null, val roomType: String? = null)
struct RoomStrippedState {
    std::string roomId;
    std::string name;
    std::string canonicalAlias;
    std::string avatarUrl;
    int numJoinedMembers = 0;
    std::string topic;
    std::string inviterId;
    std::string roomType;
};

// ==== Predecessor (create/Predecessor.kt:21-27) ====
//
// Original Kotlin:
//   data class Predecessor(val roomId: String, val eventId: String)
struct Predecessor {
    std::string roomId;
    std::string eventId;
};

// ==== RoomCreateContent (create/RoomCreateContent.kt:22-28) ====
//
// Original Kotlin:
//   data class RoomCreateContent(val creator: String? = null,
//       val federate: Boolean = true, val roomVersion: String? = "1",
//       val predecessor: Predecessor? = null, val roomType: String? = null)
struct RoomCreateContent {
    std::string creator;
    bool federate = true;
    std::string roomVersion = "1";
    Predecessor predecessor;
    bool hasPredecessor = false;
    std::string roomType;
};

// ==== CreateRoomPreset (create/CreateRoomPreset.kt:22-33) ====
//
// Original Kotlin:
//   enum class CreateRoomPreset {
//       PRIVATE_CHAT, PUBLIC_CHAT, TRUSTED_PRIVATE_CHAT }
enum class CreateRoomPreset { PRIVATE_CHAT, PUBLIC_CHAT, TRUSTED_PRIVATE_CHAT };
inline const char* createRoomPresetToString(CreateRoomPreset p) {
    switch (p) {
        case CreateRoomPreset::PRIVATE_CHAT: return "private_chat";
        case CreateRoomPreset::PUBLIC_CHAT: return "public_chat";
        case CreateRoomPreset::TRUSTED_PRIVATE_CHAT: return "trusted_private_chat";
    }
    return "private_chat";
}

// ==== RoomFeaturePreset (create/RoomFeaturePreset.kt:22-32) ====
enum class RoomFeaturePreset { FULL, LIMITED, CUSTOM };
inline const char* roomFeaturePresetToString(RoomFeaturePreset p) {
    switch (p) {
        case RoomFeaturePreset::FULL: return "full";
        case RoomFeaturePreset::LIMITED: return "limited";
        case RoomFeaturePreset::CUSTOM: return "custom";
    }
    return "full";
}

// ==== CreateRoomStateEvent (create/CreateRoomStateEvent.kt:21-27) ====
struct CreateRoomStateEvent {
    std::string type;
    std::string stateKey;
    std::string contentJson; // raw JSON for state content
};

// ==== CreateRoomParams (create/CreateRoomParams.kt:22-80) ====
//
// Complete set of parameters for the createRoom API.
struct CreateRoomParams {
    std::string name;
    std::string topic;
    std::string roomAliasName;
    bool isDirect = false;
    bool isFederated = true;
    CreateRoomPreset preset = CreateRoomPreset::PRIVATE_CHAT;
    RoomFeaturePreset featurePreset = RoomFeaturePreset::FULL;
    std::string roomVersion;
    std::string visibility;     // "public" or "private"
    std::vector<std::string> inviteList;
    std::vector<std::string> invite3pidList;
    std::vector<CreateRoomStateEvent> initialStateEvents;
    std::string powerLevelContentOverrideJson;
    std::string parentSpaceId;
    std::string predecessorRoomId;
    bool enableEncryption = true; // Element default: encrypt all rooms
};

// ==== RoomTag (tag/RoomTag.kt:22-35) ====
//
// Original Kotlin:
//   data class RoomTag(val name: String, val order: Double? = null)
struct RoomTag {
    std::string name;
    double order = 0.0;
    bool hasOrder = false;

    static constexpr const char* ROOM_TAG_FAVOURITE = "m.favourite";
    static constexpr const char* ROOM_TAG_LOW_PRIORITY = "m.lowpriority";
    static constexpr const char* ROOM_TAG_SERVER_NOTICE = "m.server_notice";
};

// ==== RoomTagContent (tag/RoomTagContent.kt:21-25) ====
struct RoomTagContent {
    double order = 0.0;
    std::string orderRaw; // raw JSON for order (can be double or string)
};

// ==== PublicRoom / PublicRoomsResponse / PublicRoomsParams ====
//
// Original Kotlin (roomdirectory/*.kt)
struct PublicRoom {
    std::string roomId;
    std::string name;
    std::string topic;
    std::string avatarUrl;
    std::string canonicalAlias;
    int numJoinedMembers = 0;
    bool worldReadable = false;
    bool guestCanJoin = false;
    std::string joinRule;
    std::string roomType;
    std::vector<std::string> aliases;
};

struct PublicRoomsFilter {
    std::string genericSearchTerm;
    std::string roomTypesJson;
};

struct PublicRoomsParams {
    std::string server;
    int limit = 20;
    std::string since;
    PublicRoomsFilter filter;
    bool includeAllNetworks = false;
    std::string thirdPartyInstanceId;
};

struct PublicRoomsResponse {
    std::vector<PublicRoom> chunk;
    std::string nextBatch;
    std::string prevBatch;
    int totalRoomCountEstimate = 0;
};

// ==== FieldType (forward decl needed by ThirdPartyProtocol) ====
//
// Original Kotlin (thirdparty/FieldType.kt)
struct FieldType {
    std::string regexp;
    std::string placeholder;
    std::string label; // maps to field_type pattern in 3PID spec
};

// ==== ThirdPartyProtocol / ThirdPartyProtocolInstance ====
//
// Original Kotlin (thirdparty/*.kt)
struct ThirdPartyProtocolInstance {
    std::string networkId;
    std::string desc;
    std::string botUserId;
    std::string icon;
    struct Field {
        std::string regexp;
        std::string placeholder;
        std::string label; // field_type pattern
    };
    std::vector<Field> fields;
};

struct ThirdPartyProtocol {
    std::string userProtocol;
    std::string locationProtocol;
    std::string icon;
    std::vector<FieldType> fieldTypes;
    std::vector<ThirdPartyProtocolInstance> instances;
};

} // namespace room_models
} // namespace pcp
