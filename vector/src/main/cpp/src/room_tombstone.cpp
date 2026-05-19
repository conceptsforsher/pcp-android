#include "progressive/room_tombstone.hpp"

namespace progressive {

// ==== Versioning State ====
//
// Original Kotlin (VersioningState.kt):
//   enum class VersioningState { NONE, UPGRADED_ROOM_NOT_JOINED,
//       UPGRADED_ROOM_JOINED, PREDECESSOR_ROOM }

const char* versioningStateToString(VersioningState state) {
    switch (state) {
        case VersioningState::NONE: return "NONE";
        case VersioningState::UPGRADED_ROOM_NOT_JOINED: return "UPGRADED_ROOM_NOT_JOINED";
        case VersioningState::UPGRADED_ROOM_JOINED: return "UPGRADED_ROOM_JOINED";
        case VersioningState::PREDECESSOR_ROOM: return "PREDECESSOR_ROOM";
    }
    return "NONE";
}

// ==== Tombstone Event Detection ====
//
// Original Kotlin (RoomTombstoneEventProcessor.kt:47-49):
//   override fun shouldProcess(eventId, eventType, insertType): Boolean {
//       return eventType == EventType.STATE_ROOM_TOMBSTONE
//   }

bool shouldProcessTombstoneEvent(const std::string& eventType) {
    // Original Kotlin: EventType.STATE_ROOM_TOMBSTONE == "m.room.tombstone"
    return eventType == "m.room.tombstone";
}

// ==== JSON Parsing ====
//
// Original Kotlin (RoomTombstoneEventProcessor.kt:30-34):
//   val createRoomContent = event.getClearContent()
//       .toModel<RoomTombstoneContent>()
//   if (createRoomContent?.replacementRoomId == null) return

static std::string extractJsonField(const std::string& json, const std::string& key) {
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

RoomTombstoneContent parseRoomTombstoneContent(const std::string& stateEventJson) {
    RoomTombstoneContent result;

    // Original Kotlin: extract "content" from the state event
    auto contentPos = stateEventJson.find("\"content\"");
    if (contentPos == std::string::npos) return result;
    contentPos = stateEventJson.find('{', contentPos);
    if (contentPos == std::string::npos) return result;

    // Brace-count to extract content object
    int depth = 1;
    size_t start = contentPos;
    contentPos++;
    while (contentPos < stateEventJson.size() && depth > 0) {
        if (stateEventJson[contentPos] == '{') depth++;
        else if (stateEventJson[contentPos] == '}') depth--;
        contentPos++;
    }
    std::string contentJson = stateEventJson.substr(start, contentPos - start);

    // Original Kotlin: extract body and replacement_room from content
    // @Json(name = "body") val body: String?
    result.body = extractJsonField(contentJson, "body");

    // Original Kotlin: @Json(name = "replacement_room") val replacementRoomId: String?
    result.replacementRoomId = extractJsonField(contentJson, "replacement_room");

    return result;
}

std::string tombstoneContentToJson(const RoomTombstoneContent& content) {
    // Original Kotlin: Moshi serialization
    std::string json = "{";
    if (!content.body.empty()) {
        json += "\"body\":\"" + content.body + "\",";
    }
    json += "\"replacement_room\":\"" + content.replacementRoomId + "\"";
    json += "}";
    return json;
}

// ==== Room Upgrade Handler ====

UpgradeInfo processRoomUpgrade(const std::string& tombstoneEventJson) {
    UpgradeInfo info;
    auto content = parseRoomTombstoneContent(tombstoneEventJson);
    if (!content.isUpgrade()) return info;

    info.predecessorRoomId = "";  // roomId from context
    info.successorRoomId = content.replacementRoomId;
    info.isUpgrade = true;
    return info;
}

std::string formatUpgradeNotice(const UpgradeInfo& info) {
    if (!info.isUpgrade) return "";
    if (info.successorRoomId.empty()) return "This room has been replaced";
    return "This room has been replaced. Continue in the new room?";
}

} // namespace progressive
