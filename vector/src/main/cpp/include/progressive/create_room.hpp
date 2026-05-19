#pragma once

#include <string>
#include <vector>
#include <unordered_map>

namespace progressive {

// ==== Create Room Models ====
//
// Original Kotlin: create/*.kt in room/model/create/

// Original Kotlin (CreateRoomPreset.kt:24-32):
//   enum class CreateRoomPreset {
//       @Json(name="private_chat") PRESET_PRIVATE_CHAT,
//       @Json(name="public_chat") PRESET_PUBLIC_CHAT,
//       @Json(name="trusted_private_chat") PRESET_TRUSTED_PRIVATE_CHAT
//   }
enum class CreateRoomPreset {
    PRIVATE_CHAT = 0,          // "private_chat"
    PUBLIC_CHAT = 1,           // "public_chat"
    TRUSTED_PRIVATE_CHAT = 2   // "trusted_private_chat"
};
const char* createRoomPresetToString(CreateRoomPreset p);

// Original Kotlin (Predecessor.kt:25-28):
//   data class Predecessor(roomId, eventId)
struct Predecessor {
    std::string roomId;    // "room_id" key
    std::string eventId;   // "event_id" key
};

// Original Kotlin (RoomCreateContent.kt:27-34):
//   data class RoomCreateContent(creator, roomVersion, predecessor, type, additionalCreators)
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

// Original Kotlin (CreateRoomStateEvent.kt:25-38):
//   data class CreateRoomStateEvent(type, content, stateKey)
struct CreateRoomStateEvent {
    std::string type;        // "type" key — event type
    std::string contentJson; // "content" key — raw JSON
    std::string stateKey;    // "state_key" key — defaults to ""
};

// Original Kotlin (CreateRoomParams.kt:33-147):
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

// ==== Relation Models ====
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

// ==== JSON Parsing ====

CreateRoomParams parseCreateRoomParams(const std::string& json);
RoomCreateContent parseRoomCreateContent(const std::string& json);
RelationContent parseRelationContent(const std::string& json);
ReactionContent parseReactionContent(const std::string& json);

std::string createRoomParamsToJson(const CreateRoomParams& params);
std::string relationContentToJson(const RelationContent& rel);

} // namespace progressive
