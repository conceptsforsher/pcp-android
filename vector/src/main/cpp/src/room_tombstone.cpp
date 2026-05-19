#include "progressive/room_tombstone.hpp"

namespace progressive {

// ==== JSON Helper ====
// Manual JSON field extractor — extracts a string value for the given key.
// Handles escaped characters (\", \\, \/, etc.).
// Does NOT parse nested objects/arrays; for those see the brace-count extraction below.

static std::string extractJsonField(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return "";
    pos = json.find(':', pos);
    if (pos == std::string::npos) return "";
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    if (pos >= json.size() || json[pos] != '"') return "";
    pos++;
    std::string val;
    while (pos < json.size() && json[pos] != '"') {
        if (json[pos] == '\\') {
            pos++;
            if (pos >= json.size()) break;
            // Simple escape handling
            if (json[pos] == 'n') val += '\n';
            else if (json[pos] == 't') val += '\t';
            else if (json[pos] == 'r') val += '\r';
            else if (json[pos] == '\\') val += '\\';
            else if (json[pos] == '"') val += '"';
            else if (json[pos] == '/') val += '/';
            else { val += '\\'; val += json[pos]; }
            pos++;
        } else {
            val += json[pos++];
        }
    }
    return val;
}

// Extract a nested JSON object for a given key using brace counting.
static std::string extractJsonObject(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return "";
    pos = json.find(':', pos);
    if (pos == std::string::npos) return "";
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    if (pos >= json.size() || json[pos] != '{') return "";
    size_t start = pos;
    int depth = 1;
    pos++;
    while (pos < json.size() && depth > 0) {
        if (json[pos] == '{') depth++;
        else if (json[pos] == '}') depth--;
        pos++;
    }
    return json.substr(start, pos - start);
}

// Extract a boolean field for a given key.
static bool extractJsonBool(const std::string& json, const std::string& key, bool defaultVal = false) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return defaultVal;
    pos = json.find(':', pos);
    if (pos == std::string::npos) return defaultVal;
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    if (pos >= json.size()) return defaultVal;
    // Check for "true" or "false" as strings or bare tokens
    if (json.compare(pos, 4, "true") == 0) return true;
    if (json.compare(pos, 5, "false") == 0) return false;
    if (json[pos] == '"') {
        pos++;
        if (json.compare(pos, 4, "true") == 0) return true;
        if (json.compare(pos, 5, "false") == 0) return false;
    }
    return defaultVal;
}

// Extract a JSON array (list of strings) for a given key.
static std::vector<std::string> extractJsonStringArray(const std::string& json, const std::string& key) {
    std::vector<std::string> result;
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return result;
    pos = json.find(':', pos);
    if (pos == std::string::npos) return result;
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    if (pos >= json.size() || json[pos] != '[') return result;
    pos++; // skip '['
    while (pos < json.size() && json[pos] != ']') {
        while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == ',')) pos++;
        if (json[pos] == '"') {
            pos++;
            std::string val;
            while (pos < json.size() && json[pos] != '"') {
                if (json[pos] == '\\') { pos++; if (pos >= json.size()) break; }
                val += json[pos++];
            }
            if (pos < json.size()) pos++; // skip closing quote
            if (!val.empty()) result.push_back(val);
        }
    }
    return result;
}

// Escape a string for JSON output.
static std::string jsonEscape(const std::string& s) {
    std::string out;
    out.reserve(s.size() + 2);
    for (char ch : s) {
        switch (ch) {
            case '"':  out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\n': out += "\\n";  break;
            case '\r': out += "\\r";  break;
            case '\t': out += "\\t";  break;
            case '\b': out += "\\b";  break;
            case '\f': out += "\\f";  break;
            default:   out += ch;     break;
        }
    }
    return out;
}

// Find the "content" object within a full matrix event JSON.
static std::string extractContentJson(const std::string& eventJson) {
    auto contentPos = eventJson.find("\"content\"");
    if (contentPos == std::string::npos) return eventJson; // assume it IS content already

    contentPos = eventJson.find('{', contentPos);
    if (contentPos == std::string::npos) return eventJson;

    int depth = 1;
    size_t start = contentPos;
    contentPos++;
    while (contentPos < eventJson.size() && depth > 0) {
        if (eventJson[contentPos] == '{') depth++;
        else if (eventJson[contentPos] == '}') depth--;
        contentPos++;
    }
    return eventJson.substr(start, contentPos - start);
}

// ============================================================
//  Versioning State
// ============================================================

// Original Kotlin (VersioningState.kt:22-39):
//   enum class VersioningState {
//       NONE, UPGRADED_ROOM_NOT_JOINED, UPGRADED_ROOM_JOINED;
//       fun isUpgraded() = this != NONE
//   }

const char* versioningStateToString(VersioningState state) {
    switch (state) {
        case VersioningState::NONE:                      return "NONE";
        case VersioningState::UPGRADING:                 return "UPGRADING";
        case VersioningState::UPGRADED_ROOM_JOINED:      return "UPGRADED_ROOM_JOINED";
        case VersioningState::UPGRADED_ROOM_NOT_JOINED:  return "UPGRADED_ROOM_NOT_JOINED";
        case VersioningState::PRE_UPGRADE_ROOM:          return "PRE_UPGRADE_ROOM";
    }
    return "NONE";
}

VersioningState versioningStateFromString(const std::string& s) {
    if (s == "UPGRADING")                return VersioningState::UPGRADING;
    if (s == "UPGRADED_ROOM_JOINED")     return VersioningState::UPGRADED_ROOM_JOINED;
    if (s == "UPGRADED_ROOM_NOT_JOINED") return VersioningState::UPGRADED_ROOM_NOT_JOINED;
    if (s == "PRE_UPGRADE_ROOM")         return VersioningState::PRE_UPGRADE_ROOM;
    // Lower-case aliases (used in some storage formats):
    if (s == "upgrading")                return VersioningState::UPGRADING;
    if (s == "upgraded_room_joined")     return VersioningState::UPGRADED_ROOM_JOINED;
    if (s == "upgraded_room_not_joined") return VersioningState::UPGRADED_ROOM_NOT_JOINED;
    if (s == "pre_upgrade_room")         return VersioningState::PRE_UPGRADE_ROOM;
    return VersioningState::NONE;
}

// ============================================================
//  Compute Versioning State
// ============================================================

// Original Kotlin (RoomTombstoneEventProcessor.kt:33-44):
//   if (event.roomId == null) return
//   val createRoomContent = event.getClearContent().toModel<RoomTombstoneContent>()
//   if (createRoomContent?.replacementRoomId == null) return
//   ...
//   if (predecessorRoomSummary.versioningState == VersioningState.NONE) {
//       predecessorRoomSummary.versioningState = VersioningState.UPGRADED_ROOM_NOT_JOINED
//   }

VersioningState computeVersioningState(
    const std::string& tombstoneJson,
    const std::string& currentMembership,
    VersioningState currentVersioningState)
{
    // Parse tombstone content to check if replacement_room is set
    auto content = parseRoomTombstoneContent(tombstoneJson);
    if (!content.isUpgrade()) {
        // No replacement room — not an upgrade
        return currentVersioningState;
    }

    // We have a valid tombstone with replacement_room.
    // Determine new state based on membership.
    bool isJoined =
        (currentMembership == "join") ||
        (currentMembership == "JOIN");

    if (currentVersioningState == VersioningState::NONE) {
        // This room just received a tombstone event.
        if (isJoined) {
            return VersioningState::UPGRADED_ROOM_JOINED;
        }
        return VersioningState::UPGRADED_ROOM_NOT_JOINED;
    }

    if (currentVersioningState == VersioningState::UPGRADED_ROOM_NOT_JOINED && isJoined) {
        // User has now joined the replacement room
        return VersioningState::UPGRADED_ROOM_JOINED;
    }

    return currentVersioningState;
}

// ============================================================
//  Room Predecessor
// ============================================================

// Original Kotlin (Predecessor.kt:24-28):
//   @JsonClass(generateAdapter = true)
//   data class Predecessor(
//       @Json(name = "room_id") val roomId: String? = null,
//       @Json(name = "event_id") val eventId: String? = null
//   )

// JSON: {"room_id": "!old:example.org", "event_id": "$ev123:example.org"}

std::string buildPredecessor(const RoomPredecessor& p) {
    std::string json = "{\"room_id\":\"";
    json += jsonEscape(p.roomId);
    json += "\",\"event_id\":\"";
    json += jsonEscape(p.eventId);
    json += "\"}";
    return json;
}

RoomPredecessor parsePredecessor(const std::string& json) {
    RoomPredecessor result;
    // Original Kotlin: @Json(name = "room_id")
    result.roomId  = extractJsonField(json, "room_id");
    // Original Kotlin: @Json(name = "event_id")
    result.eventId = extractJsonField(json, "event_id");
    return result;
}

// ============================================================
//  Room Create Content
// ============================================================

// Original Kotlin (RoomCreateContent.kt:28-37):
//   @JsonClass(generateAdapter = true)
//   data class RoomCreateContent(
//       @Json(name = "creator") val creator: String? = null,
//       @Json(name = "room_version") val roomVersion: String? = null,
//       @Json(name = "predecessor") val predecessor: Predecessor? = null,
//       @Json(name = "type") val type: String? = null,
//       @Json(name = "additional_creators") val additionalCreators: List<String>? = null,
//   )

RoomCreateContent parseRoomCreateContent(const std::string& json) {
    RoomCreateContent result;

    std::string contentJson = extractContentJson(json);

    // Original Kotlin: @Json(name = "creator")
    result.creator = extractJsonField(contentJson, "creator");

    // Original Kotlin: @Json(name = "room_version")
    result.roomVersion = extractJsonField(contentJson, "room_version");

    // Original Kotlin: @Json(name = "predecessor")
    std::string predJson = extractJsonObject(contentJson, "predecessor");
    if (!predJson.empty()) {
        result.predecessor = parsePredecessor(predJson);
    }

    // Original Kotlin: @Json(name = "type")
    result.type = extractJsonField(contentJson, "type");

    // Matrix spec: "m.federate" (legacy) and "federate"
    result.mFederate = extractJsonBool(contentJson, "m.federate", true);
    result.federate  = extractJsonBool(contentJson, "federate", result.mFederate);

    // Original Kotlin: @Json(name = "additional_creators")
    result.additionalCreators = extractJsonStringArray(contentJson, "additional_creators");

    return result;
}

std::string buildRoomCreateContent(const RoomCreateContent& c) {
    std::string json = "{";

    // Original Kotlin: @Json(name = "creator")
    json += "\"creator\":\"";
    json += jsonEscape(c.creator);
    json += "\"";

    // Original Kotlin: @Json(name = "room_version")
    if (!c.roomVersion.empty()) {
        json += ",\"room_version\":\"";
        json += jsonEscape(c.roomVersion);
        json += "\"";
    }

    // Deduplicate federation flags: if both are the same, only emit "federate"
    if (!c.federate) {
        json += ",\"federate\":false";
    }

    // Original Kotlin: @Json(name = "predecessor")
    if (c.hasPredecessor()) {
        json += ",\"predecessor\":";
        json += buildPredecessor(c.predecessor);
    }

    // Original Kotlin: @Json(name = "type")
    if (!c.type.empty()) {
        json += ",\"type\":\"";
        json += jsonEscape(c.type);
        json += "\"";
    }

    // Original Kotlin: @Json(name = "additional_creators")
    if (!c.additionalCreators.empty()) {
        json += ",\"additional_creators\":[";
        for (size_t i = 0; i < c.additionalCreators.size(); ++i) {
            if (i > 0) json += ",";
            json += "\"";
            json += jsonEscape(c.additionalCreators[i]);
            json += "\"";
        }
        json += "]";
    }

    json += "}";
    return json;
}

// ============================================================
//  Tombstone Content — Parse / Build
// ============================================================

// Original Kotlin (RoomTombstoneContent.kt:24-35):
//   @Json(name = "body") val body: String? = null,
//   @Json(name = "replacement_room") val replacementRoomId: String?

RoomTombstoneContent parseRoomTombstoneContent(const std::string& stateEventJson) {
    RoomTombstoneContent result;

    std::string contentJson = extractContentJson(stateEventJson);

    // Original Kotlin: @Json(name = "body") val body: String?
    result.body = extractJsonField(contentJson, "body");

    // Original Kotlin: @Json(name = "replacement_room") val replacementRoomId: String?
    result.replacementRoomId = extractJsonField(contentJson, "replacement_room");

    return result;
}

std::string buildTombstoneContent(const RoomTombstoneContent& content) {
    // Original Kotlin: Moshi serialization
    // JSON: {"body":"...","replacement_room":"..."}
    std::string json = "{\"body\":\"";
    json += jsonEscape(content.body);
    json += "\",\"replacement_room\":\"";
    json += jsonEscape(content.replacementRoomId);
    json += "\"}";
    return json;
}

// Backward-compatible alias.
std::string tombstoneContentToJson(const RoomTombstoneContent& content) {
    return buildTombstoneContent(content);
}

// ============================================================
//  Room Upgrade Body
// ============================================================

// Original Kotlin (RoomUpgradeBody.kt:22-26):
//   @JsonClass(generateAdapter = true)
//   internal data class RoomUpgradeBody(
//       @Json(name = "new_version") val newVersion: String
//   )

// Original Kotlin (RoomVersionUpgradeTask.kt:48-54):
//   executeRequest(globalErrorReceiver) {
//       roomAPI.upgradeRoom(roomId = params.roomId, body = RoomUpgradeBody(params.newVersion))
//   }.replacementRoomId

std::string buildRoomUpgradeBody(const RoomUpgradeBody& body) {
    // JSON: {"new_version":"9"}
    std::string json = "{\"new_version\":\"";
    json += jsonEscape(body.newVersion);
    json += "\"";

    // Optional fields
    if (!body.newRoomName.empty()) {
        json += ",\"new_room_name\":\"";
        json += jsonEscape(body.newRoomName);
        json += "\"";
    }

    if (!body.newRoomTopic.empty()) {
        json += ",\"new_room_topic\":\"";
        json += jsonEscape(body.newRoomTopic);
        json += "\"";
    }

    if (!body.inviteUsers.empty()) {
        json += ",\"invite_users\":[";
        for (size_t i = 0; i < body.inviteUsers.size(); ++i) {
            if (i > 0) json += ",";
            json += "\"";
            json += jsonEscape(body.inviteUsers[i]);
            json += "\"";
        }
        json += "]";
    }

    json += "}";
    return json;
}

// ============================================================
//  Tombstone Analytics
// ============================================================

// Original Kotlin (RoomTombstoneEventProcessor.kt:33-36):
//   val createRoomContent = event.getClearContent().toModel<RoomTombstoneContent>()
//   if (createRoomContent?.replacementRoomId == null) return

bool isRoomTombstoned(const std::string& tombstoneEventJson) {
    auto content = parseRoomTombstoneContent(tombstoneEventJson);
    return content.isUpgrade();
}

std::string getReplacementRoomId(const std::string& tombstoneEventJson) {
    auto content = parseRoomTombstoneContent(tombstoneEventJson);
    return content.replacementRoomId;
}

// ============================================================
//  Tombstone Event Processing
// ============================================================

// Original Kotlin (RoomTombstoneEventProcessor.kt:33-44):
//   if (event.roomId == null) return
//   val createRoomContent = event.getClearContent().toModel<RoomTombstoneContent>()
//   if (createRoomContent?.replacementRoomId == null) return
//   ...
//   if (predecessorRoomSummary.versioningState == VersioningState.NONE) {
//       predecessorRoomSummary.versioningState = VersioningState.UPGRADED_ROOM_NOT_JOINED
//   }

VersioningState processTombstoneEvent(
    const std::string& tombstoneEventJson,
    const std::string& currentMembership,
    VersioningState currentState)
{
    return computeVersioningState(tombstoneEventJson, currentMembership, currentState);
}

// ============================================================
//  Tombstone Event Detection
// ============================================================

// Original Kotlin (RoomTombstoneEventProcessor.kt:46-48):
//   override fun shouldProcess(eventId, eventType, insertType): Boolean {
//       return eventType == EventType.STATE_ROOM_TOMBSTONE
//   }

bool shouldProcessTombstoneEvent(const std::string& eventType) {
    // Original Kotlin: EventType.STATE_ROOM_TOMBSTONE == "m.room.tombstone"
    return eventType == "m.room.tombstone";
}

// ============================================================
//  Room Upgrade Handler (legacy, backward compat)
// ============================================================

UpgradeInfo processRoomUpgrade(const std::string& tombstoneEventJson) {
    UpgradeInfo info;
    auto content = parseRoomTombstoneContent(tombstoneEventJson);
    if (!content.isUpgrade()) return info;

    // predecessorRoomId — roomId from calling context (set by caller or empty)
    info.predecessorRoomId.clear();
    info.successorRoomId = content.replacementRoomId;
    info.isUpgrade = true;
    return info;
}

std::string formatUpgradeNotice(const UpgradeInfo& info) {
    // Original Kotlin: format a user-visible notice
    if (!info.isUpgrade) return "";
    if (!info.noticeText.empty()) return info.noticeText;
    if (info.successorRoomId.empty()) return "This room has been replaced";
    return "This room has been replaced. Continue in the new room?";
}

} // namespace progressive
