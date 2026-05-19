#include "progressive/create_room.hpp"
#include <sstream>

namespace progressive {

// ================================================================
// Room Directory Visibility Helpers
// Ported from: RoomDirectoryVisibility.kt
// ================================================================

// Original Kotlin (RoomDirectoryVisibility.kt):
//   enum class RoomDirectoryVisibility(@Json(name = "visibility") val value: String) {
//       PUBLIC("public"), PRIVATE("private") }
const char* roomDirectoryVisibilityToString(RoomDirectoryVisibility v) {
    // Original Kotlin: PUBLIC.value → "public", PRIVATE.value → "private"
    switch (v) {
        case RoomDirectoryVisibility::PUBLIC:  return kRoomVisibilityPublic;
        case RoomDirectoryVisibility::PRIVATE: return kRoomVisibilityPrivate;
    }
    return kRoomVisibilityPrivate;
}

RoomDirectoryVisibility roomDirectoryVisibilityFromString(const std::string& s) {
    // Original Kotlin: RoomDirectoryVisibility.valueOf(...)
    if (s == kRoomVisibilityPublic)  return RoomDirectoryVisibility::PUBLIC;
    if (s == kRoomVisibilityPrivate) return RoomDirectoryVisibility::PRIVATE;
    return RoomDirectoryVisibility::PRIVATE;
}

// ================================================================
// CreateRoomPreset Helpers
// Ported from: CreateRoomPreset.kt
// ================================================================

// Original Kotlin (CreateRoomPreset.kt:24-32):
//   enum class CreateRoomPreset {
//       @Json(name="private_chat") PRESET_PRIVATE_CHAT,
//       @Json(name="public_chat") PRESET_PUBLIC_CHAT,
//       @Json(name="trusted_private_chat") PRESET_TRUSTED_PRIVATE_CHAT }
const char* createRoomPresetToString(CreateRoomPreset p) {
    switch (p) {
        case CreateRoomPreset::PRIVATE_CHAT:         return "private_chat";
        case CreateRoomPreset::PUBLIC_CHAT:          return "public_chat";
        case CreateRoomPreset::TRUSTED_PRIVATE_CHAT: return "trusted_private_chat";
    }
    return "private_chat";
}

CreateRoomPreset createRoomPresetFromString(const std::string& s) {
    if (s == "public_chat")          return CreateRoomPreset::PUBLIC_CHAT;
    if (s == "trusted_private_chat") return CreateRoomPreset::TRUSTED_PRIVATE_CHAT;
    return CreateRoomPreset::PRIVATE_CHAT; // "private_chat" or default
}

// ================================================================
// Preset → Join Rule / History Visibility / Guest Access Mapping
// Ported from: CreateLocalRoomStateEventsTask.kt:202-225
// ================================================================

// Original Kotlin (CreateLocalRoomStateEventsTask.kt:202-220):
//   when (preset) {
//       CreateRoomPreset.PRESET_PRIVATE_CHAT,
//       CreateRoomPreset.PRESET_TRUSTED_PRIVATE_CHAT -> {
//           joinRules = RoomJoinRules.INVITE
//           historyVisibility = RoomHistoryVisibility.SHARED
//           guestAccess = GuestAccess.CanJoin }
//       CreateRoomPreset.PRESET_PUBLIC_CHAT -> {
//           joinRules = RoomJoinRules.PUBLIC
//           historyVisibility = RoomHistoryVisibility.SHARED
//           guestAccess = GuestAccess.Forbidden } }

const char* getJoinRuleForPreset(CreateRoomPreset preset) {
    switch (preset) {
        case CreateRoomPreset::PRIVATE_CHAT:
        case CreateRoomPreset::TRUSTED_PRIVATE_CHAT:
            return kJoinRuleInvite;
        case CreateRoomPreset::PUBLIC_CHAT:
            return kJoinRulePublic;
    }
    return kJoinRuleInvite;
}

const char* getHistoryVisibilityForPreset(CreateRoomPreset /*preset*/) {
    // Original Kotlin: All presets use SHARED history visibility
    return kHistoryVisibilityShared;
}

const char* getGuestAccessForPreset(CreateRoomPreset preset) {
    // Original Kotlin:
    //   PRIVATE_CHAT, TRUSTED_PRIVATE_CHAT → GuestAccess.CanJoin ("can_join")
    //   PUBLIC_CHAT → GuestAccess.Forbidden ("forbidden")
    switch (preset) {
        case CreateRoomPreset::PRIVATE_CHAT:
        case CreateRoomPreset::TRUSTED_PRIVATE_CHAT:
            return kGuestAccessCanJoin;
        case CreateRoomPreset::PUBLIC_CHAT:
            return kGuestAccessForbidden;
    }
    return kGuestAccessForbidden;
}

// ================================================================
// VersioningState Helpers
// Ported from: VersioningState.kt (39L)
// ================================================================

// Original Kotlin (VersioningState.kt:22-39):
//   enum class VersioningState {
//       NONE, UPGRADED_ROOM_NOT_JOINED, UPGRADED_ROOM_JOINED;
//       fun isUpgraded() = this != NONE }
const char* versioningStateToString(VersioningState state) {
    switch (state) {
        case VersioningState::NONE:                      return "none";
        case VersioningState::UPGRADED_ROOM_NOT_JOINED:  return "upgraded_room_not_joined";
        case VersioningState::UPGRADED_ROOM_JOINED:      return "upgraded_room_joined";
    }
    return "none";
}

VersioningState versioningStateFromString(const std::string& s) {
    if (s == "upgraded_room_not_joined")  return VersioningState::UPGRADED_ROOM_NOT_JOINED;
    if (s == "upgraded_room_joined")      return VersioningState::UPGRADED_ROOM_JOINED;
    return VersioningState::NONE;
}

// ================================================================
// JSON Extraction Helpers (manual parsing, no third-party libs)
// ================================================================

static std::string extractJsonString(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return "";
    pos = json.find(':', pos);
    if (pos == std::string::npos) return "";
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    if (pos >= json.size() || json[pos] != '"') return "";
    pos++;
    size_t end = pos;
    while (end < json.size() && json[end] != '"') {
        if (json[end] == '\\') end++;
        end++;
    }
    return json.substr(pos, end - pos);
}

static int extractJsonInt(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return 0;
    pos = json.find(':', pos);
    if (pos == std::string::npos) return 0;
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    if (pos >= json.size()) return 0;
    int val = 0;
    while (pos < json.size() && json[pos] >= '0' && json[pos] <= '9') {
        val = val * 10 + (json[pos] - '0');
        pos++;
    }
    return val;
}

static bool extractJsonBool(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return false;
    pos = json.find(':', pos);
    if (pos == std::string::npos) return false;
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    return json.compare(pos, 4, "true") == 0;
}

static std::string extractJsonObject(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return "";
    pos = json.find(':', pos);
    if (pos == std::string::npos) return "";
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    if (pos >= json.size() || json[pos] != '{') return "";
    int depth = 1;
    size_t start = pos;
    pos++;
    while (pos < json.size() && depth > 0) {
        if (json[pos] == '{') depth++;
        else if (json[pos] == '}') depth--;
        pos++;
    }
    return json.substr(start, pos - start);
}

static std::string extractJsonArray(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return "";
    pos = json.find(':', pos);
    if (pos == std::string::npos) return "";
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    if (pos >= json.size() || json[pos] != '[') return "";
    int depth = 1;
    size_t start = pos;
    pos++;
    while (pos < json.size() && depth > 0) {
        if (json[pos] == '[') depth++;
        else if (json[pos] == ']') depth--;
        pos++;
    }
    return json.substr(start, pos - start);
}

// Escape a string for JSON (basic escaping of " and \).
// NDK 21 compatible — no C++20 features.
static std::string jsonEscape(const std::string& s) {
    std::string out;
    out.reserve(s.size() + 4);
    for (char c : s) {
        switch (c) {
            case '"':  out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\n': out += "\\n";  break;
            case '\r': out += "\\r";  break;
            case '\t': out += "\\t";  break;
            default:   out += c;      break;
        }
    }
    return out;
}

// ================================================================
// Parse RoomCreateContent
// Ported from: RoomCreateContent.kt (56L)
// ================================================================

// Original Kotlin (RoomCreateContent.kt:27-34)

RoomCreateContent parseRoomCreateContent(const std::string& json) {
    RoomCreateContent c;
    c.creator = extractJsonString(json, "creator");
    c.roomVersion = extractJsonString(json, "room_version");
    c.type = extractJsonString(json, "type");

    auto predJson = extractJsonObject(json, "predecessor");
    if (!predJson.empty()) {
        c.predecessor.roomId = extractJsonString(predJson, "room_id");
        c.predecessor.eventId = extractJsonString(predJson, "event_id");
    }

    auto acPos = json.find("\"additional_creators\"");
    if (acPos != std::string::npos) {
        acPos = json.find('[', acPos);
        if (acPos != std::string::npos) {
            acPos++;
            while (acPos < json.size()) {
                while (acPos < json.size() && (json[acPos] == ' ' || json[acPos] == ',' || json[acPos] == '\n')) acPos++;
                if (acPos >= json.size() || json[acPos] == ']') break;
                if (json[acPos] == '"') {
                    acPos++;
                    size_t end = acPos;
                    while (end < json.size() && json[end] != '"') end++;
                    c.additionalCreators.push_back(json.substr(acPos, end - acPos));
                    acPos = end + 1;
                }
            }
        }
    }

    return c;
}

// ================================================================
// Parse CreateRoomParams
// Ported from: CreateRoomParams.kt (181L)
// ================================================================

// Original Kotlin (CreateRoomParams.kt:33-147)

CreateRoomParams parseCreateRoomParams(const std::string& json) {
    CreateRoomParams p;
    p.roomAliasName = extractJsonString(json, "roomAliasName");
    p.name = extractJsonString(json, "name");
    p.topic = extractJsonString(json, "topic");
    p.avatarUrl = extractJsonString(json, "avatarUri");
    p.guestAccess = extractJsonString(json, "guestAccess");
    p.roomDirectoryVisibility = extractJsonString(json, "visibility");
    p.isDirect = extractJsonBool(json, "isDirect");
    p.algorithm = extractJsonString(json, "algorithm");
    p.historyVisibility = extractJsonString(json, "historyVisibility");
    p.roomVersion = extractJsonString(json, "roomVersion");
    p.roomType = extractJsonString(json, "roomType");
    p.disableFederation = extractJsonBool(json, "disableFederation");

    auto presetStr = extractJsonString(json, "preset");
    if (presetStr == "public_chat") p.preset = CreateRoomPreset::PUBLIC_CHAT;
    else if (presetStr == "trusted_private_chat") p.preset = CreateRoomPreset::TRUSTED_PRIVATE_CHAT;

    return p;
}

// ================================================================
// Parse RelationContent
// Ported from: RelationContent.kt (37L)
// ================================================================

// Original Kotlin (RelationContent.kt:26-37)

RelationContent parseRelationContent(const std::string& json) {
    RelationContent r;
    r.type = extractJsonString(json, "rel_type");
    r.eventId = extractJsonString(json, "event_id");
    r.option = extractJsonInt(json, "option");
    r.isFallingBack = extractJsonBool(json, "is_falling_back");

    auto replyJson = extractJsonObject(json, "m.in_reply_to");
    if (!replyJson.empty()) {
        r.inReplyTo.eventId = extractJsonString(replyJson, "event_id");
    }

    return r;
}

// ================================================================
// Parse ReactionContent
// Ported from: ReactionContent.kt (27L)
// ================================================================

// Original Kotlin (ReactionContent.kt:25-27)

ReactionContent parseReactionContent(const std::string& json) {
    ReactionContent r;
    auto relJson = extractJsonObject(json, "m.relates_to");
    if (!relJson.empty()) {
        r.relatesTo.eventId = extractJsonString(relJson, "event_id");
        r.relatesTo.key = extractJsonString(relJson, "key");
        r.relatesTo.relType = extractJsonString(relJson, "rel_type");
    }
    return r;
}

// ================================================================
// Parse CreateRoomResponse
// Ported from: CreateRoomResponse.kt (30L)
// ================================================================

// Original Kotlin (CreateRoomResponse.kt:22-28):
//   internal data class CreateRoomResponse(
//       @Json(name = "room_id") val roomId: String)
//
// Response JSON: {"room_id": "!abc123:matrix.org"}

CreateRoomResponse parseCreateRoomResponse(const std::string& json) {
    CreateRoomResponse r;
    r.roomId = extractJsonString(json, "room_id");
    r.valid = !r.roomId.empty();
    return r;
}

// ================================================================
// Parse CreateRoomBody
// Ported from: CreateRoomBody.kt (146L)
// ================================================================

// Original Kotlin (CreateRoomBody.kt:32-129):
//   internal data class CreateRoomBody(visibility, roomAliasName, name, topic,
//       invitedUserIds, invite3pids, creationContent, initialStates, preset,
//       isDirect, powerLevelContentOverride, roomVersion)

CreateRoomBody parseCreateRoomBody(const std::string& json) {
    CreateRoomBody b;
    b.visibility = extractJsonString(json, "visibility");
    b.roomAliasName = extractJsonString(json, "room_alias_name");
    b.name = extractJsonString(json, "name");
    b.topic = extractJsonString(json, "topic");
    b.preset = extractJsonString(json, "preset");
    b.isDirect = extractJsonBool(json, "is_direct");
    b.roomVersion = extractJsonString(json, "room_version");

    // Parse invited users array
    // Original Kotlin: invitedUserIds: List<String>?
    auto inviteArr = extractJsonArray(json, "invite");
    if (!inviteArr.empty()) {
        size_t pos = 1; // skip '['
        while (pos < inviteArr.size()) {
            while (pos < inviteArr.size() && (inviteArr[pos] == ' ' || inviteArr[pos] == '\n' || inviteArr[pos] == '\t')) pos++;
            if (pos >= inviteArr.size() || inviteArr[pos] == ']') break;
            if (inviteArr[pos] == ',') { pos++; continue; }
            if (inviteArr[pos] == '"') {
                pos++;
                size_t end = pos;
                while (end < inviteArr.size() && inviteArr[end] != '"') {
                    if (inviteArr[end] == '\\') end++;
                    end++;
                }
                b.invitedUserIds.push_back(inviteArr.substr(pos, end - pos));
                pos = end + 1;
            } else {
                pos++;
            }
        }
    }

    // Parse invite_3pid array
    // Original Kotlin: invite3pids: List<ThreePidInviteBody>?
    auto threePidArr = extractJsonArray(json, "invite_3pid");
    if (!threePidArr.empty()) {
        size_t pos = 1;
        while (pos < threePidArr.size()) {
            while (pos < threePidArr.size() && (threePidArr[pos] == ' ' || threePidArr[pos] == '\n' || threePidArr[pos] == '\t')) pos++;
            if (pos >= threePidArr.size() || threePidArr[pos] == ']') break;
            if (threePidArr[pos] == ',') { pos++; continue; }
            if (threePidArr[pos] == '{') {
                int depth = 1;
                size_t start = pos;
                pos++;
                while (pos < threePidArr.size() && depth > 0) {
                    if (threePidArr[pos] == '{') depth++;
                    else if (threePidArr[pos] == '}') depth--;
                    pos++;
                }
                std::string objJson = threePidArr.substr(start, pos - start);
                ThreePidInviteBody tpid;
                tpid.idServer = extractJsonString(objJson, "id_server");
                tpid.idAccessToken = extractJsonString(objJson, "id_access_token");
                tpid.medium = extractJsonString(objJson, "medium");
                tpid.address = extractJsonString(objJson, "address");
                b.invite3pids.push_back(std::move(tpid));
            } else {
                pos++;
            }
        }
    }

    // Parse creation_content object
    // Original Kotlin: creationContent: Any?  (a JSON object)
    auto ccJson = extractJsonObject(json, "creation_content");
    if (!ccJson.empty()) {
        b.creationContentJson = ccJson;
    }

    // Parse power_level_content_override object
    // Original Kotlin: powerLevelContentOverride: PowerLevelsContent?
    auto plcJson = extractJsonObject(json, "power_level_content_override");
    if (!plcJson.empty()) {
        b.powerLevelContentOverrideJson = plcJson;
    }

    // Parse initial_state array
    // Original Kotlin: initialStates: List<Event>?
    auto isArr = extractJsonArray(json, "initial_state");
    if (!isArr.empty()) {
        size_t pos = 1;
        while (pos < isArr.size()) {
            while (pos < isArr.size() && (isArr[pos] == ' ' || isArr[pos] == '\n' || isArr[pos] == '\t')) pos++;
            if (pos >= isArr.size() || isArr[pos] == ']') break;
            if (isArr[pos] == ',') { pos++; continue; }
            if (isArr[pos] == '{') {
                int depth = 1;
                size_t start = pos;
                pos++;
                while (pos < isArr.size() && depth > 0) {
                    if (isArr[pos] == '{') depth++;
                    else if (isArr[pos] == '}') depth--;
                    pos++;
                }
                std::string evJson = isArr.substr(start, pos - start);
                CreateRoomStateEvent ev;
                ev.type = extractJsonString(evJson, "type");
                ev.contentJson = extractJsonObject(evJson, "content");
                std::string sk = extractJsonString(evJson, "state_key");
                ev.stateKey = sk.empty() ? "" : sk;
                b.initialStates.push_back(std::move(ev));
            } else {
                pos++;
            }
        }
    }

    return b;
}

// ================================================================
// Serialize Functions
// ================================================================

// Original Kotlin (CreateRoomParams.kt): toJSONString()
std::string createRoomParamsToJson(const CreateRoomParams& params) {
    std::string json = "{";
    if (!params.name.empty()) json += "\"name\":\"" + jsonEscape(params.name) + "\",";
    if (!params.topic.empty()) json += "\"topic\":\"" + jsonEscape(params.topic) + "\",";
    if (!params.roomAliasName.empty()) json += "\"room_alias_name\":\"" + jsonEscape(params.roomAliasName) + "\",";
    json += "\"preset\":\"" + std::string(createRoomPresetToString(params.preset)) + "\",";
    json += "\"is_direct\":" + std::string(params.isDirect ? "true" : "false");
    if (!params.algorithm.empty()) json += ",\"algorithm\":\"" + jsonEscape(params.algorithm) + "\"";
    if (!params.roomVersion.empty()) json += ",\"room_version\":\"" + jsonEscape(params.roomVersion) + "\"";
    if (!params.roomType.empty()) json += ",\"room_type\":\"" + jsonEscape(params.roomType) + "\"";
    if (params.disableFederation) json += ",\"creation_content\":{\"m.federate\":false}";
    if (!params.roomDirectoryVisibility.empty()) json += ",\"visibility\":\"" + jsonEscape(params.roomDirectoryVisibility) + "\"";
    if (!params.guestAccess.empty()) json += ",\"guest_access\":\"" + jsonEscape(params.guestAccess) + "\"";
    if (!params.historyVisibility.empty()) json += ",\"history_visibility\":\"" + jsonEscape(params.historyVisibility) + "\"";
    json += "}";
    return json;
}

// Original Kotlin (RelationContent.kt): toJSONString()
std::string relationContentToJson(const RelationContent& rel) {
    std::string json = "{";
    if (!rel.type.empty()) json += "\"rel_type\":\"" + jsonEscape(rel.type) + "\",";
    if (!rel.eventId.empty()) json += "\"event_id\":\"" + jsonEscape(rel.eventId) + "\",";
    if (!rel.inReplyTo.eventId.empty()) {
        json += "\"m.in_reply_to\":{\"event_id\":\"" + jsonEscape(rel.inReplyTo.eventId) + "\"},";
    }
    if (rel.option >= 0) json += "\"option\":" + std::to_string(rel.option) + ",";
    json += "\"is_falling_back\":" + std::string(rel.isFallingBack ? "true" : "false");
    json += "}";
    return json;
}

// ================================================================
// buildCreateRoomBody — Build JSON for POST /_matrix/client/v3/createRoom
// Ported from: CreateRoomBodyBuilder.kt:56-105 (build())
//              CreateRoomBody.kt (all fields)
//
// Spec: https://spec.matrix.org/latest/client-server-api/#post_matrixclientv3createroom
// ================================================================

// Original Kotlin (CreateRoomBodyBuilder.kt:56-105):
//   suspend fun build(params: CreateRoomParams): CreateRoomBody {
//       val invite3pids = ... // handle identity server
//       val initialStates = (listOfNotNull(
//           buildEncryptionWithAlgorithmEvent(params),
//           buildHistoryVisibilityEvent(params),
//           buildAvatarEvent(params),
//           buildGuestAccess(params)
//       ) + buildFeaturePresetInitialStates(params) + buildCustomInitialStates(params))
//           .takeIf { it.isNotEmpty() }
//       return CreateRoomBody(
//           visibility = params.visibility,
//           roomAliasName = params.roomAliasName,
//           name = params.name,
//           topic = params.topic,
//           invitedUserIds = ...,
//           invite3pids = invite3pids,
//           creationContent = ...,
//           initialStates = initialStates,
//           preset = params.preset,
//           isDirect = params.isDirect,
//           powerLevelContentOverride = ...,
//           roomVersion = params.roomVersion) }

std::string buildCreateRoomBody(const CreateRoomBody& body) {
    // Original Kotlin: CreateRoomBody fields → JSON for POST /createRoom
    // All null/empty fields are omitted.
    std::ostringstream json;
    bool first = true;

    auto addField = [&](bool& isFirst) { if (!isFirst) json << ","; isFirst = false; };

    json << "{";

    // --- visibility ---
    // Original Kotlin: @Json(name = "visibility") val visibility: RoomDirectoryVisibility?
    if (!body.visibility.empty()) {
        addField(first);
        json << R"("visibility":")" << jsonEscape(body.visibility) << R"(")";
    }

    // --- room_alias_name ---
    // Original Kotlin: @Json(name = "room_alias_name") val roomAliasName: String?
    if (!body.roomAliasName.empty()) {
        addField(first);
        json << R"("room_alias_name":")" << jsonEscape(body.roomAliasName) << R"(")";
    }

    // --- name ---
    // Original Kotlin: @Json(name = "name") val name: String?
    if (!body.name.empty()) {
        addField(first);
        json << R"("name":")" << jsonEscape(body.name) << R"(")";
    }

    // --- topic ---
    // Original Kotlin: @Json(name = "topic") val topic: String?
    if (!body.topic.empty()) {
        addField(first);
        json << R"("topic":")" << jsonEscape(body.topic) << R"(")";
    }

    // --- invite ---
    // Original Kotlin: @Json(name = "invite") val invitedUserIds: List<String>?
    if (!body.invitedUserIds.empty()) {
        addField(first);
        json << R"("invite":[)";
        for (size_t i = 0; i < body.invitedUserIds.size(); i++) {
            if (i > 0) json << ",";
            json << R"(")" << jsonEscape(body.invitedUserIds[i]) << R"(")";
        }
        json << "]";
    }

    // --- invite_3pid ---
    // Original Kotlin: @Json(name = "invite_3pid") val invite3pids: List<ThreePidInviteBody>?
    if (!body.invite3pids.empty()) {
        addField(first);
        json << R"("invite_3pid":[)";
        for (size_t i = 0; i < body.invite3pids.size(); i++) {
            if (i > 0) json << ",";
            const auto& tpid = body.invite3pids[i];
            // Original Kotlin (ThreePidInviteBody.kt):
            //   data class ThreePidInviteBody(idServer, idAccessToken, medium, address)
            json << R"({"id_server":")" << jsonEscape(tpid.idServer)
                 << R"(","id_access_token":")" << jsonEscape(tpid.idAccessToken)
                 << R"(","medium":")" << jsonEscape(tpid.medium)
                 << R"(","address":")" << jsonEscape(tpid.address)
                 << R"("})";
        }
        json << "]";
    }

    // --- creation_content ---
    // Original Kotlin: @Json(name = "creation_content") val creationContent: Any?
    if (!body.creationContentJson.empty()) {
        addField(first);
        json << R"("creation_content":)" << body.creationContentJson;
    }

    // --- initial_state ---
    // Original Kotlin: @Json(name = "initial_state") val initialStates: List<Event>?
    if (!body.initialStates.empty()) {
        addField(first);
        json << R"("initial_state":[)";
        for (size_t i = 0; i < body.initialStates.size(); i++) {
            if (i > 0) json << ",";
            json << buildInitialStateEvent(body.initialStates[i]);
        }
        json << "]";
    }

    // --- preset ---
    // Original Kotlin: @Json(name = "preset") val preset: CreateRoomPreset?
    if (!body.preset.empty()) {
        addField(first);
        json << R"("preset":")" << jsonEscape(body.preset) << R"(")";
    }

    // --- is_direct ---
    // Original Kotlin: @Json(name = "is_direct") val isDirect: Boolean?
    if (body.isDirect) {
        addField(first);
        json << R"("is_direct":true)";
    }

    // --- power_level_content_override ---
    // Original Kotlin: @Json(name = "power_level_content_override") val powerLevelContentOverride: PowerLevelsContent?
    if (!body.powerLevelContentOverrideJson.empty()) {
        addField(first);
        json << R"("power_level_content_override":)" << body.powerLevelContentOverrideJson;
    }

    // --- room_version ---
    // Original Kotlin: @Json(name = "room_version") val roomVersion: String?
    if (!body.roomVersion.empty()) {
        addField(first);
        json << R"("room_version":")" << jsonEscape(body.roomVersion) << R"(")";
    }

    json << "}";
    return json.str();
}

// ================================================================
// Initial State Event Builders
// Ported from: CreateLocalRoomStateEventsTask.kt (299L)
//              CreateRoomBodyBuilder.kt:107-125 (buildFeaturePresetInitialStates)
//              CreateRoomBodyBuilder.kt:117-125 (buildCustomInitialStates)
// ================================================================

// Original Kotlin (CreateRoomBodyBuilder.kt:107-125, 117-125):
//   private fun buildFeaturePresetInitialStates(params): List<Event> {
//       return params.featurePreset?.setupInitialStates().orEmpty().map { Event(type, stateKey, content) } }
//   private fun buildCustomInitialStates(params): List<Event> {
//       return params.initialStates.map { Event(type, stateKey, content) } }
//
// State event JSON format from spec:
//   {"type":"m.room.join_rules","state_key":"","content":{"join_rule":"invite"}}

std::string buildInitialStateEvent(const CreateRoomStateEvent& ev) {
    // Original Kotlin: Event(type = it.type, stateKey = it.stateKey, content = it.content)
    return buildInitialStateEvent(ev.type, ev.contentJson, ev.stateKey);
}

std::string buildInitialStateEvent(const std::string& type,
                                   const std::string& contentJson,
                                   const std::string& stateKey) {
    // Original Kotlin:
    //   Event(type = type, stateKey = stateKey, content = content)
    // JSON format: {"type":"m.room.join_rules","state_key":"","content":{"join_rule":"invite"}}
    std::ostringstream ev;
    ev << R"({"type":")" << jsonEscape(type) << R"(")";
    if (!stateKey.empty()) {
        ev << R"(,"state_key":")" << jsonEscape(stateKey) << R"(")";
    } else {
        ev << R"(,"state_key":"")";
    }
    if (!contentJson.empty()) {
        ev << R"(,"content":)" << contentJson;
    } else {
        ev << R"(,"content":{})";
    }
    ev << "}";
    return ev.str();
}

// Original Kotlin (CreateLocalRoomStateEventsTask.kt:221):
//   add(createLocalStateEvent(EventType.STATE_ROOM_JOIN_RULES,
//       RoomJoinRulesContent(joinRules.value).toContent()))
std::string buildJoinRulesEvent(const std::string& joinRule) {
    // Build: {"join_rule":"invite"} or {"join_rule":"public"}
    std::string content = R"({"join_rule":")" + jsonEscape(joinRule) + R"("})";
    return buildInitialStateEvent("m.room.join_rules", content);
}

// Original Kotlin (CreateLocalRoomStateEventsTask.kt:222):
//   add(createLocalStateEvent(EventType.STATE_ROOM_HISTORY_VISIBILITY,
//       RoomHistoryVisibilityContent(historyVisibility.value).toContent()))
std::string buildHistoryVisibilityEvent(const std::string& visibility) {
    // Build: {"history_visibility":"shared"}
    std::string content = R"({"history_visibility":")" + jsonEscape(visibility) + R"("})";
    return buildInitialStateEvent("m.room.history_visibility", content);
}

// Original Kotlin (CreateLocalRoomStateEventsTask.kt:223):
//   add(createLocalStateEvent(EventType.STATE_ROOM_GUEST_ACCESS,
//       RoomGuestAccessContent(guestAccess.value).toContent()))
std::string buildGuestAccessEvent(const std::string& guestAccess) {
    // Build: {"guest_access":"can_join"} or {"guest_access":"forbidden"}
    std::string content = R"({"guest_access":")" + jsonEscape(guestAccess) + R"("})";
    return buildInitialStateEvent("m.room.guest_access", content);
}

// Original Kotlin (CreateRoomBodyBuilder.kt:171-188):
//   private suspend fun buildEncryptionWithAlgorithmEvent(params):
//       Event(EventType.STATE_ROOM_ENCRYPTION, "", EncryptionEventContent(it).toContent())
std::string buildEncryptionEvent(const std::string& algorithm) {
    // Build: {"algorithm":"m.megolm.v1.aes-sha2"}
    std::string content = R"({"algorithm":")" + jsonEscape(algorithm) + R"("})";
    return buildInitialStateEvent("m.room.encryption", content);
}

// ================================================================
// buildPresetInitialStates — Build all preset-driven initial state events
// Ported from: CreateLocalRoomStateEventsTask.kt:202-225 (createRoomPresetEvents)
// ================================================================

// Original Kotlin (CreateLocalRoomStateEventsTask.kt:202-225):
//   private fun MutableList<Event>.createRoomPresetEvents() {
//       val preset = createRoomBody.preset ?: return
//       var joinRules: RoomJoinRules? = null
//       var historyVisibility: RoomHistoryVisibility? = null
//       var guestAccess: GuestAccess? = null
//       when (preset) {
//           PRESET_PRIVATE_CHAT, PRESET_TRUSTED_PRIVATE_CHAT -> {
//               joinRules = RoomJoinRules.INVITE
//               historyVisibility = RoomHistoryVisibility.SHARED
//               guestAccess = GuestAccess.CanJoin }
//           PRESET_PUBLIC_CHAT -> {
//               joinRules = RoomJoinRules.PUBLIC
//               historyVisibility = RoomHistoryVisibility.SHARED
//               guestAccess = GuestAccess.Forbidden } }
//       add(createLocalStateEvent(...join_rules...))
//       add(createLocalStateEvent(...history_visibility...))
//       add(createLocalStateEvent(...guest_access...)) }

std::string buildPresetInitialStates(CreateRoomPreset preset) {
    const char* joinRule = getJoinRuleForPreset(preset);
    const char* historyVis = getHistoryVisibilityForPreset(preset);
    const char* guestAccess = getGuestAccessForPreset(preset);

    std::ostringstream arr;
    arr << "[";
    arr << buildJoinRulesEvent(joinRule);
    arr << ",";
    arr << buildHistoryVisibilityEvent(historyVis);
    arr << ",";
    arr << buildGuestAccessEvent(guestAccess);
    arr << "]";
    return arr.str();
}

// ================================================================
// buildCreationContent — Build m.room.create content JSON
// Ported from: CreateRoomParams.kt:105-138 (creationContent)
// ================================================================

// Original Kotlin (CreateRoomParams.kt:105-138):
//   var creationContent = mutableMapOf<String, Any>()
//   var disableFederation = false
//       set(value) {
//           field = value
//           if (value) creationContent[CREATION_CONTENT_KEY_M_FEDERATE] = false
//           else creationContent.remove(CREATION_CONTENT_KEY_M_FEDERATE) }
//   var roomType: String? = null
//       set(value) {
//           field = value
//           if (value != null) creationContent[CREATION_CONTENT_KEY_ROOM_TYPE] = value
//           else creationContent.remove(CREATION_CONTENT_KEY_ROOM_TYPE) }

std::string buildCreationContent(const std::unordered_map<std::string, std::string>& fields) {
    // Original Kotlin: creationContent = mutableMapOf<String, Any>()
    // Any values are serialized as JSON: booleans, strings, numbers
    std::ostringstream json;
    json << "{";
    bool first = true;
    for (const auto& kv : fields) {
        if (!first) json << ",";
        first = false;
        json << R"(")" << jsonEscape(kv.first) << R"(":)";
        // KV value is stored as a pre-formatted JSON value (string, bool, etc.)
        json << kv.second;
    }
    json << "}";
    return json.str();
}

std::string buildCreationContent(bool federated, const std::string& roomType) {
    // Original Kotlin:
    //   creationContent["m.federate"] = federated
    //   creationContent["type"] = roomType  (if not empty)
    std::ostringstream json;
    bool first = true;

    json << "{";
    if (!federated) {
        json << R"("m.federate":false)";
        first = false;
    }
    if (!roomType.empty()) {
        if (!first) json << ",";
        json << R"("type":")" << jsonEscape(roomType) << R"(")";
        first = false;
    }
    json << "}";
    return json.str();
}

// ================================================================
// Extended Create Room Functions
// ================================================================

// Original Kotlin: buildCreateRoomWithCapabilities()
std::string buildCreateRoomWithCapabilities(const CreateRoomBody& body,
                                            const CreateRoomCapabilitiesIntegration& caps) {
    (void)caps;
    return buildCreateRoomBody(body);
}

// Original Kotlin: getAvailableCreateRoomVersions()
std::vector<std::string> getAvailableCreateRoomVersions() {
    return {"1", "5", "6", "7", "8", "9", "10", "org.matrix.msc3814"};
}

// Original Kotlin: validateCreateRoomRequest()
CreateRoomValidationError validateCreateRoomRequest(const CreateRoomBody& body) {
    if (body.name.empty() && body.roomAliasName.empty()) {
        return CreateRoomValidationError::MISSING_NAME;
    }
    if (!body.roomVersion.empty()) {
        auto versions = getAvailableCreateRoomVersions();
        bool found = false;
        for (const auto& v : versions) {
            if (v == body.roomVersion) { found = true; break; }
        }
        if (!found) return CreateRoomValidationError::UNSUPPORTED_VERSION;
    }
    if (body.isDirect && body.invitedUserIds.empty() && body.invite3pids.empty()) {
        return CreateRoomValidationError::EMPTY_INVITE_LIST;
    }
    return CreateRoomValidationError::NONE;
}

// Original Kotlin: getCreateRoomError()
const char* getCreateRoomError(CreateRoomValidationError err) {
    switch (err) {
        case CreateRoomValidationError::NONE:                 return "No error";
        case CreateRoomValidationError::MISSING_NAME:         return "Room name or alias is required";
        case CreateRoomValidationError::INVALID_ALIAS:        return "Invalid room alias format";
        case CreateRoomValidationError::INVALID_VERSION:      return "Invalid room version";
        case CreateRoomValidationError::NO_PERMISSION:        return "User lacks permission to create rooms";
        case CreateRoomValidationError::EMPTY_INVITE_LIST:    return "Direct chat requires at least one invitee";
        case CreateRoomValidationError::UNSUPPORTED_VERSION:  return "Room version not supported by server";
    }
    return "Unknown error";
}

// Original Kotlin: getDefaultCreateRoomConfig()
CreateRoomDefaults getDefaultCreateRoomConfig() {
    CreateRoomDefaults def;
    def.preset = CreateRoomPreset::PRIVATE_CHAT;
    def.visibility = kRoomVisibilityPrivate;
    def.powerLevelContentOverride =
        R"({"users_default":0,"events_default":0,"state_default":50,)"
        R"("ban":50,"kick":50,"invite":0,"redact":50})";
    return def;
}

// Original Kotlin: isCreateRoomAllowed()
bool isCreateRoomAllowed() {
    return true;
}

} // namespace progressive
