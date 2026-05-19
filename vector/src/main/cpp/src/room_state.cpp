#include "progressive/room_state.hpp"
#include <sstream>

namespace progressive {

// Helper: extract string field from JSON
static std::string extractStr(const std::string& json, const std::string& field) {
    auto search = "\"" + field + "\":\"";
    auto pos = json.find(search);
    if (pos == std::string::npos) {
        search = "\"" + field + "\": \"";
        pos = json.find(search);
    }
    if (pos == std::string::npos) return "";
    pos += search.size();
    auto end = json.find('"', pos);
    if (end == std::string::npos) return "";
    return json.substr(pos, end - pos);
}

// Helper: extract bool field
static bool extractBool(const std::string& json, const std::string& field, bool defaultVal = false) {
    auto search = "\"" + field + "\":";
    auto pos = json.find(search);
    if (pos == std::string::npos) return defaultVal;
    pos += search.size();
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    if (json.find("true", pos) == pos) return true;
    if (json.find("false", pos) == pos) return false;
    return defaultVal;
}

// ==== Room Join Rules ====
// Original Kotlin (RoomJoinRulesData.kt):
//   data class RoomJoinRulesContent(@Json(name = "join_rule") val joinRule: String?) {
//       fun isPublic() = joinRule == JoinRules.PUBLIC
//       fun isInvite() = joinRule == JoinRules.INVITE
//   }

RoomJoinRulesData parseJoinRules(const std::string& contentJson) {
    RoomJoinRulesData rules;

    // Original Kotlin: content.get("join_rule")?.asString()
    rules.rawRule = extractStr(contentJson, "join_rule");
    rules.joinRules = joinRuleFromString(rules.rawRule);
    rules.valid = rules.joinRules != JoinRule::Unknown;

    // Parse "allow" array for restricted rooms
    // {"join_rule": "restricted", "allow": [{"room_id": "!abc:server", "type": "m.room_membership"}]}
    if (rules.joinRules == JoinRule::Restricted) {
        auto allowPos = contentJson.find("\"allow\"");
        if (allowPos != std::string::npos) {
            size_t pos = contentJson.find("\"room_id\"", allowPos);
            while (pos != std::string::npos && pos < contentJson.find(']', allowPos)) {
                auto roomId = extractStr(contentJson.substr(pos), "room_id");
                if (!roomId.empty()) rules.allow.push_back(roomId);
                pos = contentJson.find("\"room_id\"", pos + 1);
            }
        }
    }

    return rules;
}

bool isPublicRoom(const RoomJoinRulesData& rules) { return rules.joinRules == JoinRule::Public; }
bool isInviteOnly(const RoomJoinRulesData& rules) { return rules.joinRules == JoinRule::Invite; }
bool isKnockable(const RoomJoinRulesData& rules) { return rules.joinRules == JoinRule::Knock; }

JoinRule joinRuleFromString(const std::string& rule) {
    if (rule == "public") return JoinRule::Public;
    if (rule == "invite") return JoinRule::Invite;
    if (rule == "knock") return JoinRule::Knock;
    if (rule == "private") return JoinRule::Private;
    if (rule == "restricted") return JoinRule::Restricted;
    return JoinRule::Unknown;
}

std::string roomJoinRulesToString(JoinRule rule) {
    switch (rule) {
        case JoinRule::Public: return "public";
        case JoinRule::Invite: return "invite";
        case JoinRule::Knock: return "knock";
        case JoinRule::Private: return "private";
        case JoinRule::Restricted: return "restricted";
        default: return "unknown";
    }
}

// ==== Room History Visibility ====
// Original Kotlin (RSH_RoomHistoryVisibility.kt):
//   data class RoomHistoryVisibilityContent(@Json(name = "history_visibility") val historyVisibility: String)

RSH_RoomHistoryVisibility parseHistoryVisibility(const std::string& contentJson) {
    RSH_RoomHistoryVisibility vis;
    vis.rawValue = extractStr(contentJson, "history_visibility");
    vis.historyVisibility = historyVisibilityFromString(vis.rawValue);
    vis.valid = vis.historyVisibility != HistoryVisibility::Unknown;
    return vis;
}

bool isHistoryPubliclyVisible(const RSH_RoomHistoryVisibility& vis) {
    return vis.historyVisibility == HistoryVisibility::WorldReadable;
}

bool isHistoryVisibleToGuests(const RSH_RoomHistoryVisibility& vis) {
    return vis.historyVisibility == HistoryVisibility::WorldReadable ||
           vis.historyVisibility == HistoryVisibility::Shared;
}

HistoryVisibility historyVisibilityFromString(const std::string& vis) {
    if (vis == "world_readable") return HistoryVisibility::WorldReadable;
    if (vis == "shared") return HistoryVisibility::Shared;
    if (vis == "invited") return HistoryVisibility::Invited;
    if (vis == "joined") return HistoryVisibility::Joined;
    return HistoryVisibility::Unknown;
}

std::string roomHistoryVisibilityToString(HistoryVisibility vis) {
    switch (vis) {
        case HistoryVisibility::WorldReadable: return "world_readable";
        case HistoryVisibility::Shared: return "shared";
        case HistoryVisibility::Invited: return "invited";
        case HistoryVisibility::Joined: return "joined";
        default: return "unknown";
    }
}

// ==== Room Guest Access ====
// Original Kotlin (RoomGuestAccess.kt):
//   data class RoomGuestAccessContent(@Json(name = "guest_access") val guestAccess: String)

RoomGuestAccess parseGuestAccess(const std::string& contentJson) {
    RoomGuestAccess access;
    access.rawValue = extractStr(contentJson, "guest_access");
    access.access = (access.rawValue == "can_join") ? GuestAccessType::CanJoin :
                    (access.rawValue == "forbidden") ? GuestAccessType::Forbidden : GuestAccessType::Unknown;
    access.valid = access.access != GuestAccessType::Unknown;
    return access;
}

bool areGuestsAllowed(const RoomGuestAccess& access) {
    return access.access == GuestAccessType::CanJoin;
}

std::string guestAccessToString(GuestAccessType access) {
    switch (access) {
        case GuestAccessType::CanJoin: return "can_join";
        case GuestAccessType::Forbidden: return "forbidden";
        default: return "unknown";
    }
}

// ==== Room Create ====
// Original Kotlin (RoomCreate.kt):
//   data class RoomCreateContent(
//       @Json(name = "creator") val creator: String?,
//       @Json(name = "room_version") val roomVersion: String?,
//       @Json(name = "m.federate") val federate: Boolean = true,
//       @Json(name = "predecessor") val predecessor: RoomPredecessor?
//   )

RoomCreate parseRoomCreate(const std::string& contentJson) {
    RoomCreate create;
    create.creator = extractStr(contentJson, "creator");
    create.roomVersion = extractStr(contentJson, "room_version");
    create.isFederated = extractBool(contentJson, "m.federate", true);

    // Parse predecessor for room upgrades
    auto predPos = contentJson.find("\"predecessor\"");
    if (predPos != std::string::npos) {
        // predecessor is a nested object: {"room_id": "!old:server", "event_id": "$event"}
        auto roomId = extractStr(contentJson.substr(predPos), "room_id");
        auto eventId = extractStr(contentJson.substr(predPos), "event_id");
        if (!roomId.empty()) {
            create.predecessorRoomId = roomId;
            create.predecessorEventId = eventId;
        }
    }

    create.valid = !create.creator.empty();
    return create;
}

bool isUpgradedRoom(const RoomCreate& create) {
    return !create.predecessorRoomId.empty();
}

// ==== JSON Serialization ====

std::string joinRulesToJson(const RoomJoinRulesData& rules) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"valid": )" << (rules.valid ? "true" : "false") << ",";
    json << R"("rule": ")" << esc(roomJoinRulesToString(rules.joinRules)) << R"(",)";
    json << R"("isPublic": )" << (isPublicRoom(rules) ? "true" : "false") << ",";
    json << R"("isInviteOnly": )" << (isInviteOnly(rules) ? "true" : "false") << ",";
    json << R"("isKnockable": )" << (isKnockable(rules) ? "true" : "false") << ",";
    json << R"("allowCount": )" << static_cast<int>(rules.allow.size()) << "}";
    return json.str();
}

std::string historyVisibilityToJson(const RSH_RoomHistoryVisibility& vis) {
    std::ostringstream json;
    json << R"({"valid": )" << (vis.valid ? "true" : "false") << ",";
    json << R"("visibility": ")" << roomHistoryVisibilityToString(vis.historyVisibility) << R"(",)";
    json << R"("isPublic": )" << (isHistoryPubliclyVisible(vis) ? "true" : "false") << ",";
    json << R"("isVisibleToGuests": )" << (isHistoryVisibleToGuests(vis) ? "true" : "false") << "}";
    return json.str();
}

std::string guestAccessToJson(const RoomGuestAccess& access) {
    std::ostringstream json;
    json << R"({"valid": )" << (access.valid ? "true" : "false") << ",";
    json << R"("access": ")" << guestAccessToString(access.access) << R"(",)";
    json << R"("guestsAllowed": )" << (areGuestsAllowed(access) ? "true" : "false") << "}";
    return json.str();
}

std::string roomCreateToJson(const RoomCreate& create) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"valid": )" << (create.valid ? "true" : "false") << ",";
    json << R"("creator": ")" << esc(create.creator) << R"(",)";
    json << R"("roomVersion": ")" << esc(create.roomVersion) << R"(",)";
    json << R"("isFederated": )" << (create.isFederated ? "true" : "false") << ",";
    json << R"("isUpgraded": )" << (isUpgradedRoom(create) ? "true" : "false") << ",";
    json << R"("predecessorRoomId": ")" << esc(create.predecessorRoomId) << R"(")";
    json << "}";
    return json.str();
}

// ==== Room Tombstone (from RoomTombstoneContent.kt + RoomTombstoneEventProcessor.kt) ====
// Original Kotlin:
//   data class RoomTombstoneContent(
//       @Json(name = "body") val body: String? = null,
//       @Json(name = "replacement_room") val replacementRoomId: String?
//   )

RoomTombstone parseTombstone(const std::string& contentJson) {
    RoomTombstone tombstone;
    tombstone.body = extractStr(contentJson, "body");
    tombstone.replacementRoomId = extractStr(contentJson, "replacement_room");
    tombstone.valid = !tombstone.replacementRoomId.empty();
    return tombstone;
}

bool isRoomUpgraded(const RoomTombstone& tombstone) {
    return tombstone.valid && !tombstone.replacementRoomId.empty();
}

std::string tombstoneToJson(const RoomTombstone& tombstone) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"valid": )" << (tombstone.valid ? "true" : "false") << ",";
    json << R"("body": ")" << esc(tombstone.body) << R"(",)";
    json << R"("replacementRoomId": ")" << esc(tombstone.replacementRoomId) << R"(")";
    json << "}";
    return json.str();
}

} // namespace progressive
