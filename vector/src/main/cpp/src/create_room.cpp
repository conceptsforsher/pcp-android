#include "progressive/create_room.hpp"

namespace progressive {

const char* createRoomPresetToString(CreateRoomPreset p) {
    switch (p) {
        case CreateRoomPreset::PRIVATE_CHAT: return "private_chat";
        case CreateRoomPreset::PUBLIC_CHAT: return "public_chat";
        case CreateRoomPreset::TRUSTED_PRIVATE_CHAT: return "trusted_private_chat";
    }
    return "private_chat";
}

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

// ==== Parse RoomCreateContent ====
//
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

// ==== Parse CreateRoomParams ====
//
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

// ==== Parse RelationContent ====
//
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

// ==== Parse ReactionContent ====
//
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

// ==== Serialize ====

std::string createRoomParamsToJson(const CreateRoomParams& params) {
    std::string json = "{";
    if (!params.name.empty()) json += "\"name\":\"" + params.name + "\",";
    if (!params.topic.empty()) json += "\"topic\":\"" + params.topic + "\",";
    if (!params.roomAliasName.empty()) json += "\"room_alias_name\":\"" + params.roomAliasName + "\",";
    json += "\"preset\":\"" + std::string(createRoomPresetToString(params.preset)) + "\",";
    json += "\"is_direct\":" + std::string(params.isDirect ? "true" : "false");
    if (!params.algorithm.empty()) json += ",\"algorithm\":\"" + params.algorithm + "\"";
    if (!params.roomVersion.empty()) json += ",\"room_version\":\"" + params.roomVersion + "\"";
    json += "}";
    return json;
}

std::string relationContentToJson(const RelationContent& rel) {
    std::string json = "{";
    if (!rel.type.empty()) json += "\"rel_type\":\"" + rel.type + "\",";
    if (!rel.eventId.empty()) json += "\"event_id\":\"" + rel.eventId + "\",";
    if (!rel.inReplyTo.eventId.empty()) {
        json += "\"m.in_reply_to\":{\"event_id\":\"" + rel.inReplyTo.eventId + "\"},";
    }
    if (rel.option >= 0) json += "\"option\":" + std::to_string(rel.option) + ",";
    json += "\"is_falling_back\":" + std::string(rel.isFallingBack ? "true" : "false");
    json += "}";
    return json;
}

} // namespace progressive
