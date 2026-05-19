#pragma once

#include <string>
#include <vector>
#include <unordered_map>

namespace progressive {

// ==== Create Room Models ====
//
// Original Kotlin: create/*.kt in room/model/create/

// ================================================================
// Room String Constants
// Ported from:
//   org.matrix.android.sdk.api.session.room.model.RoomDirectoryVisibility.kt
//   org.matrix.android.sdk.api.session.room.model.RoomJoinRules.kt
//   org.matrix.android.sdk.api.session.room.model.RoomHistoryVisibility.kt
//   org.matrix.android.sdk.api.session.room.model.RoomType.kt
//   org.matrix.android.sdk.api.session.room.model.GuestAccess.kt
// ================================================================

// Original Kotlin (RoomDirectoryVisibility.kt):
//   enum class RoomDirectoryVisibility { PUBLIC("public"), PRIVATE("private") }
constexpr const char* kRoomVisibilityPublic  = "public";
constexpr const char* kRoomVisibilityPrivate = "private";

// Original Kotlin (RoomJoinRules.kt:27-33):
//   enum class RoomJoinRules(val value: String) {
//       PUBLIC("public"), INVITE("invite"), KNOCK("knock"),
//       PRIVATE("private"), RESTRICTED("restricted") }
constexpr const char* kJoinRulePublic     = "public";
constexpr const char* kJoinRuleInvite     = "invite";
constexpr const char* kJoinRuleKnock      = "knock";
constexpr const char* kJoinRulePrivate    = "private";
constexpr const char* kJoinRuleRestricted = "restricted";

// Original Kotlin (RoomHistoryVisibility.kt:27-49):
//   enum class RoomHistoryVisibility(val value: String) {
//       WORLD_READABLE("world_readable"), SHARED("shared"),
//       INVITED("invited"), JOINED("joined") }
constexpr const char* kHistoryVisibilityWorldReadable = "world_readable";
constexpr const char* kHistoryVisibilityShared        = "shared";
constexpr const char* kHistoryVisibilityInvited       = "invited";
constexpr const char* kHistoryVisibilityJoined        = "joined";

// Original Kotlin (RoomType.kt:21):
//   object RoomType { const val SPACE = "m.space" }
constexpr const char* kRoomTypeSpace = "m.space";

// Original Kotlin (GuestAccess.kt):
//   enum class GuestAccess(val value: String) { CanJoin("can_join"), Forbidden("forbidden") }
constexpr const char* kGuestAccessCanJoin   = "can_join";
constexpr const char* kGuestAccessForbidden = "forbidden";

// Original Kotlin (ThreePid.Medium):
//   enum class Medium { EMAIL, MSISDN }
constexpr const char* kThreePidMediumEmail  = "email";
constexpr const char* kThreePidMediumMsisdn = "msisdn";

// Original Kotlin (CreateRoomParams.kt:170-171):
//   companion object {
//       internal const val CREATION_CONTENT_KEY_M_FEDERATE = "m.federate"
//       internal const val CREATION_CONTENT_KEY_ROOM_TYPE = "type" }
constexpr const char* kCreationContentKeyMFederate = "m.federate";
constexpr const char* kCreationContentKeyRoomType  = "type";

// ================================================================
// Room Directory Visibility
// ================================================================

// Original Kotlin (RoomDirectoryVisibility.kt):
//   enum class RoomDirectoryVisibility(@Json(name = "visibility") val value: String) {
//       PUBLIC("public"), PRIVATE("private") }
//
// A public visibility indicates the room will be shown in the published room list.
// A private visibility will hide the room from the published room list.
// NB: This should not be confused with join_rules.
enum class RoomDirectoryVisibility {
    PUBLIC,   // "public"  — shown in published room list
    PRIVATE   // "private" — hidden from published room list
};

const char* roomDirectoryVisibilityToString(RoomDirectoryVisibility v);
RoomDirectoryVisibility roomDirectoryVisibilityFromString(const std::string& s);

// ================================================================
// CreateRoomPreset
// ================================================================

// Original Kotlin (CreateRoomPreset.kt:24-32):
//   enum class CreateRoomPreset {
//       @Json(name="private_chat") PRESET_PRIVATE_CHAT,
//       @Json(name="public_chat") PRESET_PUBLIC_CHAT,
//       @Json(name="trusted_private_chat") PRESET_TRUSTED_PRIVATE_CHAT
//   }
//
// private_chat => join_rules=invite, history_visibility=shared
// trusted_private_chat => join_rules=invite, history_visibility=shared, all invitees same power level
// public_chat => join_rules=public, history_visibility=shared
enum class CreateRoomPreset {
    PRIVATE_CHAT = 0,          // "private_chat"
    PUBLIC_CHAT = 1,           // "public_chat"
    TRUSTED_PRIVATE_CHAT = 2   // "trusted_private_chat"
};

const char* createRoomPresetToString(CreateRoomPreset p);
CreateRoomPreset createRoomPresetFromString(const std::string& s);

// Get the join rule string for a given preset.
// Original Kotlin: CreateLocalRoomStateEventsTask.kt:202-220
//   when (preset) {
//       PRIVATE_CHAT, TRUSTED_PRIVATE_CHAT → joinRules = RoomJoinRules.INVITE
//       PUBLIC_CHAT → joinRules = RoomJoinRules.PUBLIC }
const char* getJoinRuleForPreset(CreateRoomPreset preset);

// Get the history visibility string for a given preset.
// Original Kotlin: CreateLocalRoomStateEventsTask.kt:202-220
//   Both private_chat and public_chat use SHARED history visibility
const char* getHistoryVisibilityForPreset(CreateRoomPreset preset);

// Get the guest access string for a given preset.
// Original Kotlin: CreateLocalRoomStateEventsTask.kt:202-220
//   PRIVATE_CHAT/TRUSTED_PRIVATE_CHAT → CanJoin; PUBLIC_CHAT → Forbidden
const char* getGuestAccessForPreset(CreateRoomPreset preset);

// ================================================================
// Predecessor
// ================================================================

// Original Kotlin (Predecessor.kt:25-28):
//   data class Predecessor(roomId, eventId)
struct Predecessor {
    std::string roomId;    // "room_id" key
    std::string eventId;   // "event_id" key
};

// ================================================================
// RoomCreateContent (m.room.create event content)
// ================================================================

// Original Kotlin (RoomCreateContent.kt:27-34):
//   data class RoomCreateContent(
//       @Json(name="creator") val creator: String?,
//       @Json(name="room_version") val roomVersion: String?,
//       @Json(name="predecessor") val predecessor: Predecessor?,
//       @Json(name="type") val type: String?,
//       @Json(name="additional_creators") val additionalCreators: List<String>?)
struct RoomCreateContent {
    std::string creator;
    std::string roomVersion;
    Predecessor predecessor;
    std::string type;
    std::vector<std::string> additionalCreators;

    // Original Kotlin: explicitlyPrivilegeRoomCreators()
    bool explicitlyPrivilegeRoomCreators() const {
        return roomVersion == "org.matrix.hydra.11" || roomVersion == "12";
    }
};

// ================================================================
// CreateRoomStateEvent — initial state event for room creation
// ================================================================

// Original Kotlin (CreateRoomStateEvent.kt:25-38):
//   data class CreateRoomStateEvent(type, content, stateKey)
struct CreateRoomStateEvent {
    std::string type;        // "type" key — event type (e.g. "m.room.join_rules")
    std::string contentJson; // "content" key — raw JSON content
    std::string stateKey;    // "state_key" key — defaults to ""
};

// ================================================================
// ThreePidInviteBody — third party invite in room creation
// ================================================================

// Original Kotlin (ThreePidInviteBody.kt):
//   data class ThreePidInviteBody(
//       @Json(name="id_server") val idServer: String,
//       @Json(name="id_access_token") val idAccessToken: String,
//       @Json(name="medium") val medium: String,
//       @Json(name="address") val address: String)
struct ThreePidInviteBody {
    std::string idServer;       // "id_server" — identity server hostname
    std::string idAccessToken;  // "id_access_token" — identity server token
    std::string medium;         // "medium" — e.g. "email", "msisdn"
    std::string address;        // "address" — the third party identifier
};

// ================================================================
// CreateRoomParams — user-facing room creation parameters
// ================================================================

// Original Kotlin (CreateRoomParams.kt:33-177):
//   open class CreateRoomParams { visibility, roomAliasName, name, topic,
//       invitedUserIds, guestAccess, preset, isDirect, powerLevelContentOverride, ... }
struct CreateRoomParams {
    std::string roomAliasName;
    std::string name;
    std::string topic;
    std::string avatarUrl;                     // avatarUri (mapped to URL)
    std::vector<std::string> invitedUserIds;
    std::string guestAccess;                   // "can_join" or "forbidden"
    std::string roomDirectoryVisibility;        // "public" or "private"
    CreateRoomPreset preset = CreateRoomPreset::PRIVATE_CHAT;
    bool isDirect = false;
    bool enableEncryption = false;
    bool disableFederation = false;
    std::string algorithm;                     // "m.megolm.v1.aes-sha2"
    std::string historyVisibility;
    std::string roomVersion;
    std::string roomType;                      // e.g. "m.space" for spaces
    std::vector<CreateRoomStateEvent> initialStates;
    // powerLevelContentOverride handled separately

    // Original Kotlin: setDirectMessage()
    void setDirectMessage() {
        preset = CreateRoomPreset::TRUSTED_PRIVATE_CHAT;
        isDirect = true;
    }

    // Original Kotlin: enableEncryption()
    void enableRoomEncryption() {
        algorithm = "m.megolm.v1.aes-sha2";
        enableEncryption = true;
    }
};

// ================================================================
// CreateRoomBody — wire body for POST /_matrix/client/v3/createRoom
// ================================================================

// Original Kotlin (CreateRoomBody.kt:32-129):
//   internal data class CreateRoomBody(
//       @Json(name="visibility") val visibility: RoomDirectoryVisibility?,
//       @Json(name="room_alias_name") val roomAliasName: String?,
//       @Json(name="name") val name: String?,
//       @Json(name="topic") val topic: String?,
//       @Json(name="invite") val invitedUserIds: List<String>?,
//       @Json(name="invite_3pid") val invite3pids: List<ThreePidInviteBody>?,
//       @Json(name="creation_content") val creationContent: Any?,
//       @Json(name="initial_state") val initialStates: List<Event>?,
//       @Json(name="preset") val preset: CreateRoomPreset?,
//       @Json(name="is_direct") val isDirect: Boolean?,
//       @Json(name="power_level_content_override") val powerLevelContentOverride: PowerLevelsContent?,
//       @Json(name="room_version") val roomVersion: String?)
//
// This is the JSON body sent to POST /_matrix/client/v3/createRoom.
struct CreateRoomBody {
    std::string visibility;                       // "public" or "private"
    std::string roomAliasName;                    // local part for room alias
    std::string name;                             // room name
    std::string topic;                            // room topic
    std::vector<std::string> invitedUserIds;      // user IDs to invite
    std::vector<ThreePidInviteBody> invite3pids;  // third party IDs to invite
    std::string creationContentJson;              // extra m.room.create keys (raw JSON)
    std::vector<CreateRoomStateEvent> initialStates; // initial state events
    std::string preset;                           // "private_chat", "public_chat", "trusted_private_chat"
    bool isDirect = false;                        // m.room.member is_direct flag
    std::string powerLevelContentOverrideJson;    // power level content (raw JSON)
    std::string roomVersion;                      // room version (e.g. "9" or "org.matrix.hydra.11")

    // Original Kotlin (CreateRoomBody.kt:136-138):
    //   private fun CreateRoomBody.isDirect(): Boolean {
    //       return preset == CreateRoomPreset.PRESET_TRUSTED_PRIVATE_CHAT && isDirect == true }
    bool isDirectChat() const {
        return preset == "trusted_private_chat" && isDirect;
    }

    // Original Kotlin (CreateRoomBody.kt:140-146):
    //   internal fun CreateRoomBody.getDirectUserId(): String? {
    //       return if (isDirect()) invitedUserIds?.firstOrNull()
    //               ?: invite3pids?.firstOrNull()?.address
    //               ?: throw ... } else null }
    std::string getDirectUserId() const {
        if (!isDirectChat()) return "";
        if (!invitedUserIds.empty()) return invitedUserIds[0];
        if (!invite3pids.empty()) return invite3pids[0].address;
        return "";
    }

    // Check if any fields are set.
    bool isEmpty() const {
        return visibility.empty() && roomAliasName.empty() && name.empty()
            && topic.empty() && invitedUserIds.empty() && invite3pids.empty()
            && creationContentJson.empty() && initialStates.empty()
            && preset.empty() && !isDirect
            && powerLevelContentOverrideJson.empty() && roomVersion.empty();
    }
};

// ================================================================
// CreateRoomResponse — parsed response from createRoom
// ================================================================

// Original Kotlin (CreateRoomResponse.kt:22-28):
//   internal data class CreateRoomResponse(
//       @Json(name = "room_id") val roomId: String)
//
// Response from POST /_matrix/client/v3/createRoom:
//   {"room_id": "!abc123:matrix.org"}
struct CreateRoomResponse {
    std::string roomId;          // the created room ID
    bool valid = false;          // true if parsing succeeded
};

// ================================================================
// Relation Models
// ================================================================
//
// Original Kotlin: relation/*.kt

// Original Kotlin (ReplyToContent.kt:24-26):
//   data class ReplyToContent(@Json(name="event_id") eventId: String?)
#ifndef PROGRESSIVE_REPLY_TO_DEFINED
#define PROGRESSIVE_REPLY_TO_DEFINED
struct ReplyToContent {
    std::string eventId;     // "event_id" key
};
#endif

// Original Kotlin (RelationContent.kt:26-37):
//   interface RelationContent { type, eventId, inReplyTo, option, isFallingBack }
#ifndef PROGRESSIVE_RELATION_CONTENT_DEFINED
#define PROGRESSIVE_RELATION_CONTENT_DEFINED
struct RelationContent {
    std::string type;        // "rel_type" key — e.g. "m.annotation", "m.replace"
    std::string eventId;     // "event_id" key
    ReplyToContent inReplyTo; // "m.in_reply_to" key
    int option = -1;         // "option" key (poll answer index)
    bool isFallingBack = false; // "is_falling_back" key

    // Original Kotlin: isReply() = inReplyTo?.eventId != null
    bool isReply() const { return !inReplyTo.eventId.empty(); }

    // Original Kotlin: shouldRenderInThread() = isFallingBack == false
    bool shouldRenderInThread() const { return !isFallingBack; }
};
#endif

// Original Kotlin (ReactionContent.kt:25-27):
//   data class ReactionContent(@Json(name="m.relates_to") relatesTo: ReactionInfo?)
#ifndef PROGRESSIVE_REACTION_INFO_DEFINED
#define PROGRESSIVE_REACTION_INFO_DEFINED
struct ReactionInfo {
    std::string eventId;     // "event_id" key
    std::string key;         // "key" key — the emoji/key reaction
    std::string relType;     // Should be "m.annotation"
};
#endif

struct ReactionContent {
    ReactionInfo relatesTo;  // "m.relates_to" key
};

// ================================================================
// JSON Parsing & Building
// ================================================================

// --- Parse functions ---

CreateRoomParams parseCreateRoomParams(const std::string& json);
RoomCreateContent parseRoomCreateContent(const std::string& json);
RelationContent parseRelationContent(const std::string& json);
ReactionContent parseReactionContent(const std::string& json);
CreateRoomResponse parseCreateRoomResponse(const std::string& json);
CreateRoomBody parseCreateRoomBody(const std::string& json);

// --- Serialize functions ---

std::string createRoomParamsToJson(const CreateRoomParams& params);
std::string relationContentToJson(const RelationContent& rel);

// Build the JSON body for POST /_matrix/client/v3/createRoom.
// Original Kotlin: CreateRoomBodyBuilder.kt:56-105 (build())
// All optional fields are omitted from JSON when empty/null.
// Uses manual JSON building (no third-party JSON lib).
std::string buildCreateRoomBody(const CreateRoomBody& body);

// Build a single initial state event JSON object.
// Format: {"type":"m.room.join_rules","state_key":"","content":{"join_rule":"invite"}}
std::string buildInitialStateEvent(const CreateRoomStateEvent& ev);

// Build an initial state event from raw type, content JSON, and optional state key.
std::string buildInitialStateEvent(const std::string& type,
                                   const std::string& contentJson,
                                   const std::string& stateKey = "");

// Convenience: build join rules initial state event.
// {"type":"m.room.join_rules","state_key":"","content":{"join_rule":"invite"}}
std::string buildJoinRulesEvent(const std::string& joinRule);

// Convenience: build history visibility initial state event.
// {"type":"m.room.history_visibility","state_key":"","content":{"history_visibility":"shared"}}
std::string buildHistoryVisibilityEvent(const std::string& visibility);

// Convenience: build guest access initial state event.
// {"type":"m.room.guest_access","state_key":"","content":{"guest_access":"forbidden"}}
std::string buildGuestAccessEvent(const std::string& guestAccess);

// Convenience: build room encryption initial state event.
// {"type":"m.room.encryption","state_key":"","content":{"algorithm":"m.megolm.v1.aes-sha2"}}
std::string buildEncryptionEvent(const std::string& algorithm = "m.megolm.v1.aes-sha2");

// Build all preset-driven initial state events as a JSON array string.
// Original Kotlin: CreateLocalRoomStateEventsTask.kt:202-225 (createRoomPresetEvents)
std::string buildPresetInitialStates(CreateRoomPreset preset);

// Build creation content JSON from a map of key-value pairs.
// e.g. {"m.federate":true,"type":"m.space"}
std::string buildCreationContent(const std::unordered_map<std::string, std::string>& fields);

// Build creation content with federation flag and optional room type.
std::string buildCreationContent(bool federated, const std::string& roomType = "");

// ================================================================
// CreateRoomCapabilitiesIntegration
// Ported from: server capabilities for MSC3814 room creation
// ================================================================

// Original Kotlin: CreateRoomCapabilitiesIntegration
struct CreateRoomCapabilitiesIntegration {
    bool needsMSC3814 = false;
};

// Original Kotlin: RoomPresetCapabilities
struct RoomPresetCapabilities {
    CreateRoomPreset preset = CreateRoomPreset::PRIVATE_CHAT;
    std::string joinRule;
    std::string historyVisibility;
    std::string guestAccess;
    bool encryptionEnabled = false;
};

// ================================================================
// CreateRoomValidationError
// Ported from: CreateRoomParams validation
// ================================================================

// Original Kotlin: CreateRoomValidationError enum
enum class CreateRoomValidationError {
    NONE,
    MISSING_NAME,
    INVALID_ALIAS,
    INVALID_VERSION,
    NO_PERMISSION,
    EMPTY_INVITE_LIST,
    UNSUPPORTED_VERSION
};

// ================================================================
// CreateRoomDefaults
// Ported from: default room creation config
// ================================================================

// Original Kotlin: CreateRoomDefaults
struct CreateRoomDefaults {
    CreateRoomPreset preset = CreateRoomPreset::PRIVATE_CHAT;
    std::string visibility;
    std::string powerLevelContentOverride;
};

// ================================================================
// Extended Create Room Functions
// ================================================================

// Build create room body JSON with capability awareness (MSC3814).
// Original Kotlin: buildCreateRoomWithCapabilities()
std::string buildCreateRoomWithCapabilities(const CreateRoomBody& body,
                                            const CreateRoomCapabilitiesIntegration& caps);

// Get available room versions from server capabilities.
// Original Kotlin: getAvailableCreateRoomVersions()
std::vector<std::string> getAvailableCreateRoomVersions();

// Validate a create room request body.
// Original Kotlin: validateCreateRoomRequest()
CreateRoomValidationError validateCreateRoomRequest(const CreateRoomBody& body);

// Get human-readable error message for a validation error.
// Original Kotlin: getCreateRoomError()
const char* getCreateRoomError(CreateRoomValidationError err);

// Get default room creation configuration.
// Original Kotlin: getDefaultCreateRoomConfig()
CreateRoomDefaults getDefaultCreateRoomConfig();

// Check if room creation is allowed for the current user.
// Original Kotlin: isCreateRoomAllowed()
bool isCreateRoomAllowed();

// ================================================================
// Room Versioning State
// Ported from: org.matrix.android.sdk.api.session.room.model.VersioningState.kt (39L)
// ================================================================

// Original Kotlin (VersioningState.kt:22-39):
//   enum class VersioningState {
//       NONE, UPGRADED_ROOM_NOT_JOINED, UPGRADED_ROOM_JOINED;
//       fun isUpgraded() = this != NONE }
enum class VersioningState {
    NONE,                        // room is not versioned
    UPGRADED_ROOM_NOT_JOINED,    // upgraded but new room not joined
    UPGRADED_ROOM_JOINED         // upgraded and new room joined
};

const char* versioningStateToString(VersioningState state);
VersioningState versioningStateFromString(const std::string& s);
inline bool isVersioningStateUpgraded(VersioningState state) { return state != VersioningState::NONE; }

} // namespace progressive
