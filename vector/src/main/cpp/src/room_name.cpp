#include "progressive/room_name.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

namespace progressive {

// ---- Room Name Calculation ----
// Original algorithm from matrix-js-sdk:
//   1. Room state name → use if set
//   2. Canonical alias → use if set
//   3. Compute from members (heroes or active members)

RoomName computeRoomDisplayName(
    const std::string& roomNameState,
    const std::string& canonicalAlias,
    const std::vector<RoomMember>& heroes,
    const std::vector<RoomMember>& activeMembers,
    const std::vector<RoomMember>& leftMembers,
    const std::string& myUserId,
    const std::string& directUserId,
    const std::string& inviterName,
    int invitedCount,
    int joinedCount,
    bool isInvite,
    const std::vector<std::string>& excludedUserIds
) {
    // Original: 1. Check m.room.name state event
    //   ContentMapper.map(content).toModel<RoomNameContent>()?.name
    std::string parsedRoomName = parseRoomNameContent(roomNameState);
    if (!parsedRoomName.empty()) {
        return {parsedRoomName, normalizeRoomName(parsedRoomName)};
    }

    // Original: 2. Check canonical alias
    //   ContentMapper.map(content).toModel<RoomCanonicalAliasContent>()?.canonicalAlias
    std::string parsedAlias = parseCanonicalAliasContent(canonicalAlias);
    if (!parsedAlias.empty()) {
        return {parsedAlias, normalizeRoomName(parsedAlias)};
    }

    // Original: 3. Invite rooms — show inviter
    if (isInvite) {
        std::string name = inviterName.empty() ? "Room Invitation" : inviterName;
        return {name, normalizeRoomName(name)};
    }

    // Original: 4. Compute from members
    // Build list of "other" members (heroes or active, excluding self + excluded)
    auto isExcluded = [&](const std::string& uid) -> bool {
        if (uid == myUserId) return true;
        for (const auto& ex : excludedUserIds) {
            if (ex == uid) return true;
        }
        return false;
    };

    std::vector<RoomMember> others;

    // Original: if (heroes?.isNotEmpty() == true) — use heroes list
    if (!heroes.empty()) {
        for (const auto& hero : heroes) {
            if ((hero.membership == RoomMembership::Join || hero.membership == RoomMembership::Invite) &&
                !isExcluded(hero.userId)) {
                others.push_back(hero);
            }
        }
    } else {
        // Original: active members, not self, not excluded, limit 5
        for (const auto& member : activeMembers) {
            if (isExcluded(member.userId)) continue;
            others.push_back(member);
            if (others.size() >= 5) break;
        }
    }

    int otherCount = static_cast<int>(others.size());
    std::string name;

    // Original: when(otherMembersCount) { 0→..., 1→..., 2→..., ... }
    if (otherCount == 0) {
        // Get left member names
        std::vector<std::string> leftNames;
        for (const auto& lm : leftMembers) {
            if (!isExcluded(lm.userId)) {
                leftNames.push_back(lm.displayName.empty() ? lm.userId : lm.displayName);
            }
        }

        if (!directUserId.empty() && leftNames.empty()) {
            name = directUserId;
        } else {
            name = getEmptyRoomName(!directUserId.empty(), leftNames);
        }
    } else if (otherCount == 1) {
        // Original: getNameFor1member(resolveRoomMemberName(member))
        name = resolveMemberName(others[0], activeMembers);
    } else if (otherCount == 2) {
        // Original: getNameFor2members(m1, m2)
        std::string n1 = resolveMemberName(others[0], activeMembers);
        std::string n2 = resolveMemberName(others[1], activeMembers);
        name = n1 + " and " + n2;
    } else if (otherCount == 3) {
        // Original: getNameFor3members(m1, m2, m3)
        std::string n1 = resolveMemberName(others[0], activeMembers);
        std::string n2 = resolveMemberName(others[1], activeMembers);
        std::string n3 = resolveMemberName(others[2], activeMembers);
        name = n1 + ", " + n2 + " and " + n3;
    } else if (otherCount == 4) {
        // Original: getNameFor4members(m1, m2, m3, m4)
        std::string n1 = resolveMemberName(others[0], activeMembers);
        std::string n2 = resolveMemberName(others[1], activeMembers);
        std::string n3 = resolveMemberName(others[2], activeMembers);
        std::string n4 = resolveMemberName(others[3], activeMembers);
        name = n1 + ", " + n2 + ", " + n3 + " and " + n4;
    } else {
        // Original: getNameFor4membersAndMore(m1, m2, m3, remainingCount)
        std::string n1 = resolveMemberName(others[0], activeMembers);
        std::string n2 = resolveMemberName(others[1], activeMembers);
        std::string n3 = resolveMemberName(others[2], activeMembers);
        int remaining = invitedCount + joinedCount - otherCount + 1;
        name = n1 + ", " + n2 + ", " + n3 + " and " + std::to_string(remaining) + " others";
    }

    return {name, normalizeRoomName(name)};
}

std::string normalizeRoomName(const std::string& name) {
    // Original: Normalizer.normalize(this) — lowercase + strip non-alphanumeric
    std::string result;
    for (char c : name) {
        unsigned char uc = static_cast<unsigned char>(c);
        if (std::isalnum(uc) || std::isspace(uc)) {
            result += static_cast<char>(std::tolower(uc));
        }
    }
    // Trim
    while (!result.empty() && std::isspace(static_cast<unsigned char>(result.front()))) result.erase(0, 1);
    while (!result.empty() && std::isspace(static_cast<unsigned char>(result.back()))) result.pop_back();
    return result;
}

bool isUniqueDisplayName(const std::vector<RoomMember>& members, const std::string& displayName) {
    int count = 0;
    for (const auto& m : members) {
        if (m.displayName == displayName) count++;
        if (count > 1) return false;
    }
    return count <= 1;
}

std::string resolveMemberName(
    const RoomMember& member,
    const std::vector<RoomMember>& allMembers)
{
    // Original: if (isUnique) displayName else "displayName (userId)"
    std::string displayName = member.displayName.empty() ? member.userId : member.displayName;
    if (isUniqueDisplayName(allMembers, displayName)) {
        return displayName;
    }
    return displayName + " (" + member.userId + ")";
}

std::string getEmptyRoomName(bool isDirect, const std::vector<std::string>& leftMemberNames) {
    if (isDirect) {
        return "Direct Message";
    }
    if (leftMemberNames.empty()) {
        return "Empty Room";
    }
    // Room with only left members
    if (leftMemberNames.size() == 1) {
        return leftMemberNames[0] + " (left)";
    }
    return std::to_string(leftMemberNames.size()) + " members (left)";
}

std::string roomNameToJson(const RoomName& name) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"name": ")" << esc(name.name) << R"(",)";
    json << R"("normalizedName": ")" << esc(name.normalizedName) << R"(")";
    json << "}";
    return json.str();
}

std::string parseRoomNameContent(const std::string& contentJson) {
    // {"name": "My Room Name"}
    auto search = "\"name\":\"";
    auto pos = contentJson.find(search);
    if (pos == std::string::npos) {
        search = "\"name\": \"";
        pos = contentJson.find(search);
    }
    if (pos == std::string::npos) return "";
    pos += search.size();
    auto end = contentJson.find('"', pos);
    if (end == std::string::npos) return "";
    return contentJson.substr(pos, end - pos);
}

std::string parseCanonicalAliasContent(const std::string& contentJson) {
    // {"alias": "#myroom:server.org"} or {"canonical_alias": "..."}
    for (const auto* key : {"\"alias\":\"", "\"canonical_alias\":\"", "\"alias\": \"", "\"canonical_alias\": \""}) {
        auto pos = contentJson.find(key);
        if (pos != std::string::npos) {
            pos += strlen(key);
            auto end = contentJson.find('"', pos);
            if (end != std::string::npos) return contentJson.substr(pos, end - pos);
        }
    }
    return "";
}

} // namespace progressive
