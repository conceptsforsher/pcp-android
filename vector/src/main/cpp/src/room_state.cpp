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
    rules.rule = joinRuleFromString(rules.rawRule);
    rules.valid = rules.rule != JoinRule::Unknown;

    // Parse "allow" array for restricted rooms
    // {"join_rule": "restricted", "allow": [{"room_id": "!abc:server", "type": "m.room_membership"}]}
    if (rules.rule == JoinRule::Restricted) {
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

bool isPublicRoom(const RoomJoinRulesData& rules) { return rules.rule == JoinRule::Public; }
bool isInviteOnly(const RoomJoinRulesData& rules) { return rules.rule == JoinRule::Invite; }
bool isKnockable(const RoomJoinRulesData& rules) { return rules.rule == JoinRule::Knock; }

JoinRule joinRuleFromString(const std::string& rule) {
    if (rule == "public") return JoinRule::Public;
    if (rule == "invite") return JoinRule::Invite;
    if (rule == "knock") return JoinRule::Knock;
    if (rule == "private") return JoinRule::Private;
    if (rule == "restricted") return JoinRule::Restricted;
    return JoinRule::Unknown;
}

std::string joinRuleToString(JoinRule rule) {
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
    vis.visibility = historyVisibilityFromString(vis.rawValue);
    vis.valid = vis.visibility != HistoryVisibility::Unknown;
    return vis;
}

bool isHistoryPubliclyVisible(const RSH_RoomHistoryVisibility& vis) {
    return vis.visibility == HistoryVisibility::WorldReadable;
}

bool isHistoryVisibleToGuests(const RSH_RoomHistoryVisibility& vis) {
    return vis.visibility == HistoryVisibility::WorldReadable ||
           vis.visibility == HistoryVisibility::Shared;
}

HistoryVisibility historyVisibilityFromString(const std::string& vis) {
    if (vis == "world_readable") return HistoryVisibility::WorldReadable;
    if (vis == "shared") return HistoryVisibility::Shared;
    if (vis == "invited") return HistoryVisibility::Invited;
    if (vis == "joined") return HistoryVisibility::Joined;
    return HistoryVisibility::Unknown;
}

std::string historyVisibilityToString(HistoryVisibility vis) {
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
    json << R"("rule": ")" << esc(joinRuleToString(rules.rule)) << R"(",)";
    json << R"("isPublic": )" << (isPublicRoom(rules) ? "true" : "false") << ",";
    json << R"("isInviteOnly": )" << (isInviteOnly(rules) ? "true" : "false") << ",";
    json << R"("isKnockable": )" << (isKnockable(rules) ? "true" : "false") << ",";
    json << R"("allowCount": )" << static_cast<int>(rules.allow.size()) << "}";
    return json.str();
}

std::string historyVisibilityToJson(const RSH_RoomHistoryVisibility& vis) {
    std::ostringstream json;
    json << R"({"valid": )" << (vis.valid ? "true" : "false") << ",";
    json << R"("visibility": ")" << historyVisibilityToString(vis.visibility) << R"(",)";
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

// ==== Membership ====
// Original Kotlin (Membership.kt:26-50)

std::string membershipToString(Membership m) {
    switch (m) {
        case Membership::NONE:   return "none";
        case Membership::INVITE: return "invite";
        case Membership::JOIN:   return "join";
        case Membership::KNOCK:  return "knock";
        case Membership::LEAVE:  return "leave";
        case Membership::BAN:    return "ban";
    }
    return "none";
}

Membership membershipFromString(const std::string& s) {
    if (s == "invite") return Membership::INVITE;
    if (s == "join")   return Membership::JOIN;
    if (s == "knock")  return Membership::KNOCK;
    if (s == "leave")  return Membership::LEAVE;
    if (s == "ban")    return Membership::BAN;
    return Membership::NONE;
}

// ==== RoomMemberContent ====
// Original Kotlin (RoomMemberContent.kt:26-35)

RoomMemberContent parseRoomMemberContent(const std::string& contentJson) {
    RoomMemberContent c;
    c.membership = membershipFromString(extractStr(contentJson, "membership"));
    c.reason = extractStr(contentJson, "reason");
    c.displayName = extractStr(contentJson, "displayname");
    c.avatarUrl = extractStr(contentJson, "avatar_url");
    c.isDirect = extractBool(contentJson, "is_direct");

    auto inviteJson = contentJson.find("\"third_party_invite\"");
    if (inviteJson != std::string::npos) {
        auto objStart = contentJson.find('{', inviteJson);
        if (objStart != std::string::npos) {
            int depth = 1;
            size_t pos = objStart + 1;
            while (pos < contentJson.size() && depth > 0) {
                if (contentJson[pos] == '{') depth++;
                else if (contentJson[pos] == '}') depth--;
                pos++;
            }
            std::string inviteObj = contentJson.substr(objStart, pos - objStart);
            c.thirdPartyInvite.displayName = extractStr(inviteObj, "display_name");
            c.thirdPartyInvite.signedToken = extractStr(inviteObj, "signed");
            c.thirdPartyInvite.mxId = extractStr(inviteObj, "mxid");
        }
    }

    c.unsignedData = extractStr(contentJson, "unsigned");
    return c;
}

std::string buildRoomMemberContent(const RoomMemberContent& content) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"membership":")" << esc(membershipToString(content.membership)) << R"(")";
    if (!content.reason.empty())
        json << R"(,"reason":")" << esc(content.reason) << R"(")";
    if (!content.displayName.empty())
        json << R"(,"displayname":")" << esc(content.displayName) << R"(")";
    if (!content.avatarUrl.empty())
        json << R"(,"avatar_url":")" << esc(content.avatarUrl) << R"(")";
    json << R"(,"is_direct":)" << (content.isDirect ? "true" : "false");
    if (!content.thirdPartyInvite.displayName.empty() || !content.thirdPartyInvite.signedToken.empty()) {
        json << R"(,"third_party_invite":{)";
        json << R"("display_name":")" << esc(content.thirdPartyInvite.displayName) << R"(")";
        if (!content.thirdPartyInvite.signedToken.empty())
            json << R"(,"signed":")" << esc(content.thirdPartyInvite.signedToken) << R"(")";
        if (!content.thirdPartyInvite.mxId.empty())
            json << R"(,"mxid":")" << esc(content.thirdPartyInvite.mxId) << R"(")";
        json << "}";
    }
    json << "}";
    return json.str();
}

// ==== RoomAvatarContent ====
// Original Kotlin (RoomAvatarContent.kt:26-28)

std::string buildRoomAvatarContent(const RoomAvatarContent& content) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"url":")" << esc(content.url) << R"(")";
    if (!content.thumbnailUrl.empty())
        json << R"(,"thumbnail_url":")" << esc(content.thumbnailUrl) << R"(")";
    if (!content.thumbnailInfo.empty())
        json << R"(,"thumbnail_info":")" << esc(content.thumbnailInfo) << R"(")";
    json << "}";
    return json.str();
}

// ==== RoomNameContent ====
// Original Kotlin (RoomNameContent.kt:25-27)

std::string buildRoomNameContent(const RoomNameContent& content) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"name":")" << esc(content.name) << R"("})";
    return json.str();
}

// ==== RoomTopicContent ====
// Original Kotlin (RoomTopicContent.kt:22-24)

std::string buildRoomTopicContent(const RoomTopicContent& content) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"topic":")" << esc(content.topic) << R"("})";
    return json.str();
}

// ==== RoomAliasesContent ====
// Original Kotlin (RoomAliasesContent.kt:28-30)

std::string buildRoomAliasesContent(const RoomAliasesContent& content) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"aliases":[)";
    for (size_t i = 0; i < content.aliases.size(); i++) {
        if (i > 0) json << ",";
        json << R"(")" << esc(content.aliases[i]) << R"(")";
    }
    json << "]}";
    return json.str();
}

// ==== RoomServerAclContent ====
// Original Kotlin (RoomServerAclContent.kt:26-58)

RoomServerAclContent parseRoomServerAclContent(const std::string& contentJson) {
    RoomServerAclContent acl;
    acl.allowIpLiterals = extractBool(contentJson, "allow_ip_literals", true);

    auto allowPos = contentJson.find("\"allow\"");
    if (allowPos != std::string::npos) {
        allowPos = contentJson.find('[', allowPos);
        if (allowPos != std::string::npos) {
            allowPos++;
            while (allowPos < contentJson.size()) {
                while (allowPos < contentJson.size() && (contentJson[allowPos] == ' ' || contentJson[allowPos] == ',' || contentJson[allowPos] == '\n')) allowPos++;
                if (allowPos >= contentJson.size() || contentJson[allowPos] == ']') break;
                if (contentJson[allowPos] == '"') {
                    allowPos++;
                    size_t end = allowPos;
                    while (end < contentJson.size() && contentJson[end] != '"') end++;
                    acl.allowList.push_back(contentJson.substr(allowPos, end - allowPos));
                    allowPos = end + 1;
                }
            }
        }
    }

    auto denyPos = contentJson.find("\"deny\"");
    if (denyPos != std::string::npos) {
        denyPos = contentJson.find('[', denyPos);
        if (denyPos != std::string::npos) {
            denyPos++;
            while (denyPos < contentJson.size()) {
                while (denyPos < contentJson.size() && (contentJson[denyPos] == ' ' || contentJson[denyPos] == ',' || contentJson[denyPos] == '\n')) denyPos++;
                if (denyPos >= contentJson.size() || contentJson[denyPos] == ']') break;
                if (contentJson[denyPos] == '"') {
                    denyPos++;
                    size_t end = denyPos;
                    while (end < contentJson.size() && contentJson[end] != '"') end++;
                    acl.denyList.push_back(contentJson.substr(denyPos, end - denyPos));
                    denyPos = end + 1;
                }
            }
        }
    }

    return acl;
}

std::string buildRoomServerAclContent(const RoomServerAclContent& acl) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"allow_ip_literals":)" << (acl.allowIpLiterals ? "true" : "false");
    json << R"(,"allow":[)";
    for (size_t i = 0; i < acl.allowList.size(); i++) {
        if (i > 0) json << ",";
        json << R"(")" << esc(acl.allowList[i]) << R"(")";
    }
    json << R"(],"deny":[)";
    for (size_t i = 0; i < acl.denyList.size(); i++) {
        if (i > 0) json << ",";
        json << R"(")" << esc(acl.denyList[i]) << R"(")";
    }
    json << "]}";
    return json.str();
}

bool wildcardMatch(const std::string& pattern, const std::string& value) {
    size_t pi = 0, vi = 0;
    size_t pStar = std::string::npos, vStar = 0;

    while (vi < value.size()) {
        if (pi < pattern.size() && (pattern[pi] == '?' || pattern[pi] == value[vi])) {
            pi++; vi++;
        } else if (pi < pattern.size() && pattern[pi] == '*') {
            pStar = pi;
            vStar = vi;
            pi++;
        } else if (pStar != std::string::npos) {
            pi = pStar + 1;
            vStar++;
            vi = vStar;
        } else {
            return false;
        }
    }

    while (pi < pattern.size() && pattern[pi] == '*') pi++;
    return pi == pattern.size();
}

bool isServerAllowed(const std::string& serverName, const RoomServerAclContent& acl) {
    for (auto& deny : acl.denyList) {
        if (wildcardMatch(deny, serverName)) return false;
    }
    if (acl.allowList.empty()) return false;
    for (auto& allow : acl.allowList) {
        if (wildcardMatch(allow, serverName)) return true;
    }
    return false;
}

// ==== RoomThirdPartyInviteContent ====
// Original Kotlin (RoomThirdPartyInviteContent.kt:27-65)

RoomThirdPartyInviteContent parseRoomThirdPartyInvite(const std::string& contentJson) {
    RoomThirdPartyInviteContent c;
    c.displayName = extractStr(contentJson, "display_name");
    c.keyValidityUrl = extractStr(contentJson, "key_validity_url");
    c.publicKey = extractStr(contentJson, "public_key");

    auto pkPos = contentJson.find("\"public_keys\"");
    if (pkPos != std::string::npos) {
        pkPos = contentJson.find('[', pkPos);
        if (pkPos != std::string::npos) {
            pkPos++;
            while (pkPos < contentJson.size()) {
                while (pkPos < contentJson.size() && (contentJson[pkPos] == ' ' || contentJson[pkPos] == ',' || contentJson[pkPos] == '\n')) pkPos++;
                if (pkPos >= contentJson.size() || contentJson[pkPos] == ']') break;
                if (contentJson[pkPos] == '{') {
                    int d = 1;
                    size_t start = pkPos;
                    pkPos++;
                    while (pkPos < contentJson.size() && d > 0) {
                        if (contentJson[pkPos] == '{') d++;
                        else if (contentJson[pkPos] == '}') d--;
                        pkPos++;
                    }
                    std::string pkJson = contentJson.substr(start, pkPos - start);
                    PublicKeyInfo pk;
                    pk.keyValidityUrl = extractStr(pkJson, "key_validity_url");
                    pk.publicKey = extractStr(pkJson, "public_key");
                    c.publicKeys.push_back(pk);
                }
            }
        }
    }

    return c;
}

std::string buildRoomThirdPartyInviteContent(const RoomThirdPartyInviteContent& content) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"display_name":")" << esc(content.displayName) << R"(")";
    if (!content.keyValidityUrl.empty())
        json << R"(,"key_validity_url":")" << esc(content.keyValidityUrl) << R"(")";
    if (!content.publicKey.empty())
        json << R"(,"public_key":")" << esc(content.publicKey) << R"(")";
    if (!content.publicKeys.empty()) {
        json << R"(,"public_keys":[)";
        for (size_t i = 0; i < content.publicKeys.size(); i++) {
            if (i > 0) json << ",";
            json << R"({"key_validity_url":")" << esc(content.publicKeys[i].keyValidityUrl) << R"(")";
            json << R"(,"public_key":")" << esc(content.publicKeys[i].publicKey) << R"("})";
        }
        json << "]";
    }
    json << "}";
    return json.str();
}

// ==== RoomStrippedState ====
// Original Kotlin (RoomStrippedState.kt:24-111)

RoomStrippedState parseRoomStrippedState(const std::string& json) {
    RoomStrippedState s;
    s.roomId = extractStr(json, "room_id");
    s.name = extractStr(json, "name");
    s.topic = extractStr(json, "topic");
    s.canonicalAlias = extractStr(json, "canonical_alias");
    s.avatarUrl = extractStr(json, "avatar_url");
    s.roomType = extractStr(json, "room_type");
    s.membership = extractStr(json, "membership");

    auto numStr = extractStr(json, "num_joined_members");
    if (!numStr.empty()) s.numJoinedMembers = std::stoi(numStr);

    s.worldReadable = extractBool(json, "world_readable");
    s.guestCanJoin = extractBool(json, "guest_can_join");
    s.isFederated = extractBool(json, "m.federate", true);
    s.isEncrypted = extractBool(json, "is_encrypted");

    auto aliasPos = json.find("\"aliases\"");
    if (aliasPos != std::string::npos) {
        aliasPos = json.find('[', aliasPos);
        if (aliasPos != std::string::npos) {
            aliasPos++;
            while (aliasPos < json.size()) {
                while (aliasPos < json.size() && (json[aliasPos] == ' ' || json[aliasPos] == ',' || json[aliasPos] == '\n')) aliasPos++;
                if (aliasPos >= json.size() || json[aliasPos] == ']') break;
                if (json[aliasPos] == '"') {
                    aliasPos++;
                    size_t end = aliasPos;
                    while (end < json.size() && json[end] != '"') end++;
                    s.aliases.push_back(json.substr(aliasPos, end - aliasPos));
                    aliasPos = end + 1;
                }
            }
        }
    }

    return s;
}

std::string buildRoomStrippedState(const RoomStrippedState& state) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"room_id":")" << esc(state.roomId) << R"(")";
    if (!state.name.empty())
        json << R"(,"name":")" << esc(state.name) << R"(")";
    if (!state.topic.empty())
        json << R"(,"topic":")" << esc(state.topic) << R"(")";
    if (!state.canonicalAlias.empty())
        json << R"(,"canonical_alias":")" << esc(state.canonicalAlias) << R"(")";
    json << R"(,"num_joined_members":)" << state.numJoinedMembers;
    json << R"(,"world_readable":)" << (state.worldReadable ? "true" : "false");
    json << R"(,"guest_can_join":)" << (state.guestCanJoin ? "true" : "false");
    if (!state.avatarUrl.empty())
        json << R"(,"avatar_url":")" << esc(state.avatarUrl) << R"(")";
    json << R"(,"m.federate":)" << (state.isFederated ? "true" : "false");
    json << R"(,"is_encrypted":)" << (state.isEncrypted ? "true" : "false");
    if (!state.roomType.empty())
        json << R"(,"room_type":")" << esc(state.roomType) << R"(")";
    if (!state.membership.empty())
        json << R"(,"membership":")" << esc(state.membership) << R"(")";
    json << R"(,"aliases":[)";
    for (size_t i = 0; i < state.aliases.size(); i++) {
        if (i > 0) json << ",";
        json << R"(")" << esc(state.aliases[i]) << R"(")";
    }
    json << "]}";
    return json.str();
}

// ==== SpaceChildInfo ====
// Original Kotlin (SpaceChildInfo.kt:19-37):
//   data class SpaceChildInfo(childRoomId, isKnown, roomType, name, topic,
//       avatarUrl, order, activeMemberCount, viaServers, parentRoomId,
//       suggested, canonicalAlias, aliases, worldReadable)
// Event content: {"order": "...", "suggested": true/false, "via": [...]}

std::string buildSpaceChildContent(const SpaceChildInfo& info) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << "{";
    if (!info.order.empty())
        json << R"("order":")" << esc(info.order) << R"(")";
    else
        json << R"("order":null)";
    json << R"(,"suggested":)" << (info.suggested ? "true" : "false");
    json << R"(,"via":[)";
    for (size_t i = 0; i < info.viaServers.size(); i++) {
        if (i > 0) json << ",";
        json << R"(")" << esc(info.viaServers[i]) << R"(")";
    }
    json << "]}";
    return json.str();
}

// ==== SpaceParentInfo ====
// Original Kotlin (SpaceParentInfo.kt:19-24):
//   data class SpaceParentInfo(parentId, roomSummary, canonical, viaServers)
// Event content: {"canonical": true/false, "via": [...]}

std::string buildSpaceParentContent(const SpaceParentInfo& info) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"canonical":)" << (info.canonical ? "true" : "false");
    json << R"(,"via":[)";
    for (size_t i = 0; i < info.viaServers.size(); i++) {
        if (i > 0) json << ",";
        json << R"(")" << esc(info.viaServers[i]) << R"(")";
    }
    json << "]}";
    return json.str();
}

// ==== RoomEncryptionAlgorithm ====
// Original Kotlin (RoomEncryptionAlgorithm.kt:21-27):
//   sealed class RoomEncryptionAlgorithm
//   object Megolm : SupportedAlgorithm(MXCRYPTO_ALGORITHM_MEGOLM)
//   data class UnsupportedAlgorithm(val name: String?)
// Event content: {"algorithm": "m.megolm.v1.aes-sha2", "rotation_period_ms": ..., "rotation_period_msgs": ...}

std::string roomEncryptionAlgorithmToString(const RoomEncryptionAlgorithm& alg) {
    return alg.algorithm;
}

RoomEncryptionAlgorithm parseRoomEncryptionAlgorithm(const std::string& contentJson) {
    RoomEncryptionAlgorithm alg;
    alg.algorithm = extractStr(contentJson, "algorithm");

    auto rotMs = extractStr(contentJson, "rotation_period_ms");
    if (!rotMs.empty()) alg.rotationPeriodMs = std::stoll(rotMs);

    auto rotMsgs = extractStr(contentJson, "rotation_period_msgs");
    if (!rotMsgs.empty()) alg.rotationPeriodMsgs = std::stoll(rotMsgs);

    alg.isSupported = (alg.algorithm == MEGOLM_ALGORITHM);

    if (!alg.isSupported && alg.algorithm.empty()) {
        alg.algorithm = "";
        alg.isSupported = false;
    }

    return alg;
}

std::string buildRoomEncryptionContent(const RoomEncryptionAlgorithm& alg) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"algorithm":")" << esc(alg.algorithm) << R"(")";
    if (alg.rotationPeriodMs > 0)
        json << R"(,"rotation_period_ms":)" << alg.rotationPeriodMs;
    if (alg.rotationPeriodMsgs > 0)
        json << R"(,"rotation_period_msgs":)" << alg.rotationPeriodMsgs;
    for (auto& [key, val] : alg.additionalParams) {
        json << R"(,")" << esc(key) << R"(":")" << esc(val) << R"(")";
    }
    json << "}";
    return json.str();
}

// ==== RoomTag ====
// Original Kotlin (RoomTag.kt:19-28):
//   data class RoomTag(name: String, order: Double?)
// Event content: {"tags": {"m.favourite": {"order": 0.5}}}

std::string buildRoomTagContent(const RoomTagData& tag) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"tags":{")" << esc(tag.name) << R"(":{)";
    json << R"("order":)" << tag.order;
    json << "}}}";
    return json.str();
}

RoomTagData parseRoomTagContent(const std::string& contentJson) {
    RoomTagData tag;
    auto tagsPos = contentJson.find("\"tags\"");
    if (tagsPos != std::string::npos) {
        auto objStart = contentJson.find('{', tagsPos);
        if (objStart != std::string::npos) {
            // Find the first tag name inside tags object
            auto nameStart = contentJson.find('"', objStart + 1);
            if (nameStart != std::string::npos) {
                nameStart++;
                auto nameEnd = contentJson.find('"', nameStart);
                if (nameEnd != std::string::npos) {
                    tag.name = contentJson.substr(nameStart, nameEnd - nameStart);
                    // Parse order value inside tag data
                    auto orderStr = extractStr(contentJson.substr(nameEnd), "order");
                    if (!orderStr.empty()) {
                        tag.order = std::stod(orderStr);
                    }
                }
            }
        }
    }
    return tag;
}

// ==== RoomDirectoryVisibility ====
// Original Kotlin (RoomDirectoryVisibility.kt:22-26):
//   enum class RoomDirectoryVisibility { @Json(name="private") PRIVATE, @Json(name="public") PUBLIC }

std::string buildDirectoryVisibilityContent(RoomDirectoryVisibility visibility) {
    const char* val = directoryVisibilityToString(visibility);
    std::ostringstream json;
    json << R"({"visibility":")" << val << R"("})";
    return json.str();
}

} // namespace progressive
