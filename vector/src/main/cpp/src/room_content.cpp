#include "progressive/room_content.hpp"

namespace progressive {

// ==== Membership ====

const char* membershipToString(Membership m) {
    switch (m) {
        case Membership::NONE: return "none";
        case Membership::INVITE: return "invite";
        case Membership::JOIN: return "join";
        case Membership::KNOCK: return "knock";
        case Membership::LEAVE: return "leave";
        case Membership::BAN: return "ban";
    }
    return "none";
}

Membership membershipFromString(const std::string& s) {
    if (s == "invite") return Membership::INVITE;
    if (s == "join") return Membership::JOIN;
    if (s == "knock") return Membership::KNOCK;
    if (s == "leave") return Membership::LEAVE;
    if (s == "ban") return Membership::BAN;
    return Membership::NONE;
}

// ==== Room Join Rules ====

const char* roomJoinRulesToString(RoomJoinRules r) {
    switch (r) {
        case RoomJoinRules::PUBLIC: return "public";
        case RoomJoinRules::INVITE: return "invite";
        case RoomJoinRules::KNOCK: return "knock";
        case RoomJoinRules::PRIVATE: return "private";
        case RoomJoinRules::RESTRICTED: return "restricted";
    }
    return "invite";
}

RoomJoinRules roomJoinRulesFromString(const std::string& s) {
    if (s == "public") return RoomJoinRules::PUBLIC;
    if (s == "invite") return RoomJoinRules::INVITE;
    if (s == "knock") return RoomJoinRules::KNOCK;
    if (s == "private") return RoomJoinRules::PRIVATE;
    if (s == "restricted") return RoomJoinRules::RESTRICTED;
    return RoomJoinRules::INVITE;
}

// ==== Room History Visibility ====

const char* roomHistoryVisibilityToString(RoomHistoryVisibility v) {
    switch (v) {
        case RoomHistoryVisibility::WORLD_READABLE: return "world_readable";
        case RoomHistoryVisibility::SHARED: return "shared";
        case RoomHistoryVisibility::INVITED: return "invited";
        case RoomHistoryVisibility::JOINED: return "joined";
    }
    return "shared";
}

RoomHistoryVisibility roomHistoryVisibilityFromString(const std::string& s) {
    if (s == "world_readable") return RoomHistoryVisibility::WORLD_READABLE;
    if (s == "shared") return RoomHistoryVisibility::SHARED;
    if (s == "invited") return RoomHistoryVisibility::INVITED;
    if (s == "joined") return RoomHistoryVisibility::JOINED;
    return RoomHistoryVisibility::SHARED;
}

// ==== Guest Access ====

const char* guestAccessToString(GuestAccess g) {
    switch (g) {
        case GuestAccess::CAN_JOIN: return "can_join";
        case GuestAccess::FORBIDDEN: return "forbidden";
    }
    return "forbidden";
}

GuestAccess guestAccessFromString(const std::string& s) {
    if (s == "can_join") return GuestAccess::CAN_JOIN;
    return GuestAccess::FORBIDDEN;
}

// ==== Versioning State ====

const char* roomVersioningStateToString(RoomVersioningState s) {
    switch (s) {
        case RoomVersioningState::NONE: return "NONE";
        case RoomVersioningState::UPGRADED_ROOM_NOT_JOINED: return "UPGRADED_ROOM_NOT_JOINED";
        case RoomVersioningState::UPGRADED_ROOM_JOINED: return "UPGRADED_ROOM_JOINED";
    }
    return "NONE";
}

RoomVersioningState roomVersioningStateFromString(const std::string& s) {
    if (s == "UPGRADED_ROOM_NOT_JOINED") return RoomVersioningState::UPGRADED_ROOM_NOT_JOINED;
    if (s == "UPGRADED_ROOM_JOINED") return RoomVersioningState::UPGRADED_ROOM_JOINED;
    return RoomVersioningState::NONE;
}

// ==== JSON Helpers ====

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

static int extractJsonInt(const std::string& json, const std::string& key, int defaultVal = 0) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return defaultVal;
    pos = json.find(':', pos);
    if (pos == std::string::npos) return defaultVal;
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    if (pos >= json.size()) return defaultVal;
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

// ==== Parse RoomMemberContent ====
//
// Original Kotlin (RoomMemberContent.kt:25-41)

RoomMemberContent parseRoomMemberContent(const std::string& json) {
    RoomMemberContent c;
    c.membership = membershipFromString(extractJsonString(json, "membership"));
    c.reason = extractJsonString(json, "reason");
    c.displayName = extractJsonString(json, "displayname");
    c.avatarUrl = extractJsonString(json, "avatar_url");
    c.isDirect = extractJsonBool(json, "is_direct");

    auto inviteJson = extractJsonObject(json, "third_party_invite");
    if (!inviteJson.empty()) {
        c.thirdPartyInvite.displayName = extractJsonString(inviteJson, "display_name");
        c.thirdPartyInvite.signedToken = extractJsonString(inviteJson, "signed");
    }

    c.unsignedData = extractJsonObject(json, "unsigned");
    return c;
}

// ==== Parse PowerLevelsContent ====
//
// Original Kotlin (PowerLevelsContent.kt:27-68)

RoomPowerLevelsContent parsePowerLevelsContent(const std::string& json) {
    RoomPowerLevelsContent c;
    c.ban = extractJsonInt(json, "ban", 50);
    c.kick = extractJsonInt(json, "kick", 50);
    c.invite = extractJsonInt(json, "invite", 0);
    c.redact = extractJsonInt(json, "redact", 50);
    c.eventsDefault = extractJsonInt(json, "events_default", 0);
    c.usersDefault = extractJsonInt(json, "users_default", 0);
    c.stateDefault = extractJsonInt(json, "state_default", 50);

    // Parse "events" map
    auto eventsJson = extractJsonObject(json, "events");
    if (!eventsJson.empty()) {
        size_t pos = 1;
        while (pos < eventsJson.size()) {
            while (pos < eventsJson.size() && (eventsJson[pos] == ' ' || eventsJson[pos] == ',')) pos++;
            if (pos >= eventsJson.size() || eventsJson[pos] == '}') break;
            if (eventsJson[pos] == '"') {
                pos++;
                size_t keyEnd = pos;
                while (keyEnd < eventsJson.size() && eventsJson[keyEnd] != '"') keyEnd++;
                std::string key = eventsJson.substr(pos, keyEnd - pos);
                pos = keyEnd + 1;
                while (pos < eventsJson.size() && eventsJson[pos] != ':') pos++;
                pos++;
                while (pos < eventsJson.size() && (eventsJson[pos] == ' ' || eventsJson[pos] == '\t')) pos++;
                int val = 0;
                while (pos < eventsJson.size() && eventsJson[pos] >= '0' && eventsJson[pos] <= '9') {
                    val = val * 10 + (eventsJson[pos] - '0');
                    pos++;
                }
                c.events[key] = val;
            }
        }
    }

    // Parse "users" map
    auto usersJson = extractJsonObject(json, "users");
    if (!usersJson.empty()) {
        size_t pos = 1;
        while (pos < usersJson.size()) {
            while (pos < usersJson.size() && (usersJson[pos] == ' ' || usersJson[pos] == ',')) pos++;
            if (pos >= usersJson.size() || usersJson[pos] == '}') break;
            if (usersJson[pos] == '"') {
                pos++;
                size_t keyEnd = pos;
                while (keyEnd < usersJson.size() && usersJson[keyEnd] != '"') keyEnd++;
                std::string key = usersJson.substr(pos, keyEnd - pos);
                pos = keyEnd + 1;
                while (pos < usersJson.size() && usersJson[pos] != ':') pos++;
                pos++;
                while (pos < usersJson.size() && (usersJson[pos] == ' ' || usersJson[pos] == '\t')) pos++;
                int val = 0;
                while (pos < usersJson.size() && usersJson[pos] >= '0' && usersJson[pos] <= '9') {
                    val = val * 10 + (usersJson[pos] - '0');
                    pos++;
                }
                c.users[key] = val;
            }
        }
    }

    // Parse notifications.room level
    auto notifJson = extractJsonObject(json, "notifications");
    if (!notifJson.empty()) {
        c.notificationRoomLevel = extractJsonInt(notifJson, "room", 50);
    }

    return c;
}

// ==== Simple Parse Functions ====

RoomNameContent parseRoomNameContent(const std::string& json) {
    // Original Kotlin (RoomNameContent.kt:24-26): @Json(name="name") name
    return {extractJsonString(json, "name")};
}

RoomTopicContent parseRoomTopicContent(const std::string& json) {
    // Original Kotlin (RoomTopicContent.kt:22-24): @Json(name="topic") topic
    return {extractJsonString(json, "topic")};
}

RoomAvatarContent parseRoomAvatarContent(const std::string& json) {
    // Original Kotlin (RoomAvatarContent.kt:22-24): @Json(name="url") avatarUrl
    return {extractJsonString(json, "url")};
}

RoomCanonicalAliasContent parseRoomCanonicalAliasContent(const std::string& json) {
    // Original Kotlin (RoomCanonicalAliasContent.kt:25-37)
    RoomCanonicalAliasContent c;
    c.canonicalAlias = extractJsonString(json, "alias");
    // Parse alt_aliases array
    auto pos = json.find("\"alt_aliases\"");
    if (pos != std::string::npos) {
        pos = json.find('[', pos);
        if (pos != std::string::npos) {
            pos++;
            while (pos < json.size()) {
                while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == ',' || json[pos] == '\n')) pos++;
                if (pos >= json.size() || json[pos] == ']') break;
                if (json[pos] == '"') {
                    pos++;
                    size_t end = pos;
                    while (end < json.size() && json[end] != '"') end++;
                    c.alternativeAliases.push_back(json.substr(pos, end - pos));
                    pos = end + 1;
                }
            }
        }
    }
    return c;
}

RoomJoinRulesContent parseRoomJoinRulesContent(const std::string& json) {
    // Original Kotlin (RoomJoinRulesContent.kt:26-43)
    RoomJoinRulesContent c;
    c.joinRules = roomJoinRulesFromString(extractJsonString(json, "join_rule"));
    return c;
}

RoomHistoryVisibilityContent parseRoomHistoryVisibilityContent(const std::string& json) {
    // Original Kotlin (RoomHistoryVisibilityContent.kt:24-33)
    RoomHistoryVisibilityContent c;
    c.historyVisibility = roomHistoryVisibilityFromString(extractJsonString(json, "history_visibility"));
    return c;
}

RoomGuestAccessContent parseRoomGuestAccessContent(const std::string& json) {
    // Original Kotlin (RoomGuestAccessContent.kt:30-37)
    RoomGuestAccessContent c;
    c.guestAccess = guestAccessFromString(extractJsonString(json, "guest_access"));
    return c;
}

// ==== Room Directory Visibility ====

const char* roomDirectoryVisibilityToString(RoomDirectoryVisibility v) {
    return v == RoomDirectoryVisibility::PUBLIC ? "public" : "private";
}
RoomDirectoryVisibility roomDirectoryVisibilityFromString(const std::string& s) {
    if (s == "public") return RoomDirectoryVisibility::PUBLIC;
    return RoomDirectoryVisibility::PRIVATE;
}

// ==== Room Server ACL ====
//
// Original Kotlin (RoomServerAclContent.kt:26-49)

RoomServerAclContent parseRoomServerAclContent(const std::string& json) {
    RoomServerAclContent acl;
    acl.allowIpLiterals = extractJsonBool(json, "allow_ip_literals");

    auto allowPos = json.find("\"allow\"");
    if (allowPos != std::string::npos) {
        allowPos = json.find('[', allowPos);
        if (allowPos != std::string::npos) {
            allowPos++;
            while (allowPos < json.size()) {
                while (allowPos < json.size() && (json[allowPos] == ' ' || json[allowPos] == ',' || json[allowPos] == '\n')) allowPos++;
                if (allowPos >= json.size() || json[allowPos] == ']') break;
                if (json[allowPos] == '"') {
                    allowPos++;
                    size_t end = allowPos;
                    while (end < json.size() && json[end] != '"') end++;
                    acl.allowList.push_back(json.substr(allowPos, end - allowPos));
                    allowPos = end + 1;
                }
            }
        }
    }

    auto denyPos = json.find("\"deny\"");
    if (denyPos != std::string::npos) {
        denyPos = json.find('[', denyPos);
        if (denyPos != std::string::npos) {
            denyPos++;
            while (denyPos < json.size()) {
                while (denyPos < json.size() && (json[denyPos] == ' ' || json[denyPos] == ',' || json[denyPos] == '\n')) denyPos++;
                if (denyPos >= json.size() || json[denyPos] == ']') break;
                if (json[denyPos] == '"') {
                    denyPos++;
                    size_t end = denyPos;
                    while (end < json.size() && json[end] != '"') end++;
                    acl.denyList.push_back(json.substr(denyPos, end - denyPos));
                    denyPos = end + 1;
                }
            }
        }
    }

    return acl;
}

// ==== Room Third Party Invite ====
//
// Original Kotlin (RoomThirdPartyInviteContent.kt:28-43)

RoomThirdPartyInviteContent parseRoomThirdPartyInvite(const std::string& json) {
    RoomThirdPartyInviteContent c;
    c.displayName = extractJsonString(json, "display_name");
    c.keyValidityUrl = extractJsonString(json, "key_validity_url");
    c.publicKey = extractJsonString(json, "public_key");

    // Parse public_keys array
    auto pkPos = json.find("\"public_keys\"");
    if (pkPos != std::string::npos) {
        pkPos = json.find('[', pkPos);
        if (pkPos != std::string::npos) {
            pkPos++;
            while (pkPos < json.size()) {
                while (pkPos < json.size() && (json[pkPos] == ' ' || json[pkPos] == ',' || json[pkPos] == '\n')) pkPos++;
                if (pkPos >= json.size() || json[pkPos] == ']') break;
                if (json[pkPos] == '{') {
                    int d = 1;
                    size_t start = pkPos;
                    pkPos++;
                    while (pkPos < json.size() && d > 0) {
                        if (json[pkPos] == '{') d++;
                        else if (json[pkPos] == '}') d--;
                        pkPos++;
                    }
                    std::string pkJson = json.substr(start, pkPos - start);
                    PublicKeyInfo pk;
                    pk.keyValidityUrl = extractJsonString(pkJson, "key_validity_url");
                    pk.publicKey = extractJsonString(pkJson, "public_key");
                    c.publicKeys.push_back(pk);
                }
            }
        }
    }

    return c;
}

// ==== Room Stripped State ====
//
// Original Kotlin (RoomStrippedState.kt:26-103)

RoomStrippedState parseRoomStrippedState(const std::string& json) {
    RoomStrippedState s;
    s.roomId = extractJsonString(json, "room_id");
    s.name = extractJsonString(json, "name");
    s.topic = extractJsonString(json, "topic");
    s.canonicalAlias = extractJsonString(json, "canonical_alias");
    s.avatarUrl = extractJsonString(json, "avatar_url");
    s.roomType = extractJsonString(json, "room_type");
    s.membership = extractJsonString(json, "membership");
    s.numJoinedMembers = extractJsonInt(json, "num_joined_members");
    s.worldReadable = extractJsonBool(json, "world_readable");
    s.guestCanJoin = extractJsonBool(json, "guest_can_join");
    s.isFederated = extractJsonBool(json, "m.federate");
    s.isEncrypted = extractJsonBool(json, "is_encrypted");

    // Parse aliases array
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

// ==== Public Room ====
//
// Original Kotlin (PublicRoom.kt:26-79)

PublicRoom parsePublicRoom(const std::string& json) {
    PublicRoom r;
    r.roomId = extractJsonString(json, "room_id");
    r.name = extractJsonString(json, "name");
    r.topic = extractJsonString(json, "topic");
    r.canonicalAlias = extractJsonString(json, "canonical_alias");
    r.avatarUrl = extractJsonString(json, "avatar_url");
    r.numJoinedMembers = extractJsonInt(json, "num_joined_members");
    r.worldReadable = extractJsonBool(json, "world_readable");
    r.guestCanJoin = extractJsonBool(json, "guest_can_join");
    r.isFederated = extractJsonBool(json, "m.federate");
    return r;
}

PublicRoomsResponse parsePublicRoomsResponse(const std::string& json) {
    PublicRoomsResponse r;
    r.nextBatch = extractJsonString(json, "next_batch");
    r.prevBatch = extractJsonString(json, "prev_batch");
    r.totalRoomCountEstimate = extractJsonInt(json, "total_room_count_estimate");

    auto chunkPos = json.find("\"chunk\"");
    if (chunkPos != std::string::npos) {
        chunkPos = json.find('[', chunkPos);
        if (chunkPos != std::string::npos) {
            chunkPos++;
            while (chunkPos < json.size()) {
                while (chunkPos < json.size() && (json[chunkPos] == ' ' || json[chunkPos] == ',' || json[chunkPos] == '\n')) chunkPos++;
                if (chunkPos >= json.size() || json[chunkPos] == ']') break;
                if (json[chunkPos] == '{') {
                    int d = 1;
                    size_t start = chunkPos;
                    chunkPos++;
                    while (chunkPos < json.size() && d > 0) {
                        if (json[chunkPos] == '{') d++;
                        else if (json[chunkPos] == '}') d--;
                        chunkPos++;
                    }
                    r.chunk.push_back(parsePublicRoom(json.substr(start, chunkPos - start)));
                }
            }
        }
    }

    return r;
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
    // Check deny list first (more specific)
    for (auto& deny : acl.denyList) {
        if (wildcardMatch(deny, serverName)) return false;
    }
    // Check allow list
    if (acl.allowList.empty()) return false; // empty allow = deny all
    for (auto& allow : acl.allowList) {
        if (wildcardMatch(allow, serverName)) return true;
    }
    return false;
}

bool canJoinRoom(const RoomJoinRulesContent& rules, bool userIsInvited,
                 bool userIsMember, bool isMemberOfAllowedRoom) {
    // Already in the room
    if (userIsMember) return true;
    // Invited users can always join
    if (userIsInvited) return true;

    using JR = RoomJoinRules;
    switch (rules.joinRules) {
        case JR::PUBLIC:
            return true;
        case JR::INVITE:
        case JR::PRIVATE:
            return false;
        case JR::KNOCK:
            return true; // can knock (request to join)
        case JR::RESTRICTED:
            // Can join if member of an allowed room/space
            return isMemberOfAllowedRoom;
    }
    return false;
}

} // namespace progressive
