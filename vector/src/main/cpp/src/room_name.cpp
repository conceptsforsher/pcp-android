#include "progressive/room_name.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

namespace progressive {

// ==== Room Name Error Functions ====

std::string getRoomNameError(RoomNameError error) {
    // Original Kotlin: human-readable error descriptions for UI
    switch (error) {
        case RoomNameError::NONE:
            return "";
        case RoomNameError::TOO_LONG:
            return "Room name is too long (maximum 255 characters)";
        case RoomNameError::TOO_SHORT:
            return "Room name is too short";
        case RoomNameError::INVALID_CHARS:
            return "Room name contains invalid characters";
        case RoomNameError::EMPTY:
            return "Room name cannot be empty";
        case RoomNameError::DUPLICATE:
            return "Room name is the same as the current name";
    }
    return "Unknown error";
}

// ==== Room Name Config ====

RoomNameConfig getDefaultRoomNameConfig() {
    // Original Kotlin: standard Matrix room name limits
    RoomNameConfig config;
    config.maxLength = 255;
    config.minLength = 1;
    // Matrix spec: common disallowed chars for room names
    config.disallowedRegex = "[<>]";
    return config;
}

// ==== Room Name Event Validator ====

RoomNameError RoomNameEventValidator::validateRoomName(
    const std::string& name,
    const RoomNameConfig& config,
    const std::string& currentName)
{
    // Original Kotlin: validate room name changes against Matrix limits

    // Check emptiness
    bool allSpace = true;
    for (char c : name) {
        if (!std::isspace(static_cast<unsigned char>(c))) {
            allSpace = false;
            break;
        }
    }
    if (name.empty() || allSpace) {
        return RoomNameError::EMPTY;
    }

    // Check min length
    if ((int)name.size() < config.minLength) {
        return RoomNameError::TOO_SHORT;
    }

    // Check max length
    if ((int)name.size() > config.maxLength) {
        return RoomNameError::TOO_LONG;
    }

    // Check disallowed characters
    if (!config.disallowedRegex.empty()) {
        // Simple disallowed char check: < and >
        for (char c : name) {
            if (c == '<' || c == '>') {
                return RoomNameError::INVALID_CHARS;
            }
        }
    }

    // Check if name starts or ends with whitespace
    if (!name.empty()) {
        if (std::isspace(static_cast<unsigned char>(name.front())) ||
            std::isspace(static_cast<unsigned char>(name.back()))) {
            return RoomNameError::INVALID_CHARS;
        }
    }

    // Check duplicate (same as current)
    if (!currentName.empty() && name == currentName) {
        return RoomNameError::DUPLICATE;
    }

    return RoomNameError::NONE;
}

std::string RoomNameEventValidator::sanitizeRoomName(
    const std::string& name,
    const RoomNameConfig& config)
{
    // Original Kotlin: strip invalid characters and trim whitespace

    // Trim leading whitespace
    size_t start = 0;
    while (start < name.size() && std::isspace(static_cast<unsigned char>(name[start]))) {
        start++;
    }

    // Trim trailing whitespace
    size_t end = name.size();
    while (end > start && std::isspace(static_cast<unsigned char>(name[end - 1]))) {
        end--;
    }

    std::string result;
    result.reserve(end - start);

    // Strip disallowed chars
    for (size_t i = start; i < end; i++) {
        char c = name[i];
        if (c == '<' || c == '>') continue;  // Always strip < >
        result += c;
    }

    // Truncate to max length
    if ((int)result.size() > config.maxLength) {
        result.resize(config.maxLength);
    }

    // Trim again after stripping
    while (!result.empty() && std::isspace(static_cast<unsigned char>(result.front()))) {
        result.erase(0, 1);
    }
    while (!result.empty() && std::isspace(static_cast<unsigned char>(result.back()))) {
        result.pop_back();
    }

    return result;
}

// ==== Room Name History ====

std::string RoomNameHistory::currentName() const {
    // Original Kotlin: most recent name in history
    if (entries.empty()) return "";
    return entries.back().name;
}

std::string RoomNameHistory::previousName() const {
    // Original Kotlin: name before the current one
    if (entries.size() < 2) return "";
    return entries[entries.size() - 2].name;
}

bool RoomNameHistory::wasRenamed() const {
    // Original Kotlin: was the room ever renamed?
    return entries.size() > 1;
}

void trackRoomNameChange(
    RoomNameHistory& history,
    const std::string& roomId,
    const std::string& newName,
    int64_t timestampMs)
{
    // Original Kotlin: record a name change in history
    history.roomId = roomId;
    history.entries.push_back({newName, timestampMs});
}

std::string formatRoomNameHistory(const RoomNameHistory& history) {
    // Original Kotlin: human-readable name change history
    if (history.entries.empty()) return "No name changes";
    if (history.entries.size() == 1) {
        return "Room created as '" + history.entries[0].name + "'";
    }

    int count = static_cast<int>(history.entries.size());
    std::string newest = history.entries[count - 1].name;
    std::string oldest = history.entries[0].name;

    if (count == 2) {
        return "Renamed from '" + oldest + "' to '" + newest + "'";
    }

    return "Renamed " + std::to_string(count - 1) + " times from '" +
           oldest + "' to '" + newest + "'";
}

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

// ==== Format Member Name List ====
//
// Original Kotlin (RoomDisplayNameResolver.kt formatMemberNames):
//   Formats a list of display names using English/Oxford comma style:
//   0 → "Empty Room"
//   1 → "Alice"
//   2 → "Alice and Bob"
//   3 → "Alice, Bob, and Carol"
//   4 → "Alice, Bob, Carol, and Dave"
//   5+ → "Alice, Bob, Carol, and 2 others"

std::string formatMemberNameList(const std::vector<std::string>& memberNames) {
    int count = static_cast<int>(memberNames.size());

    if (count == 0) {
        return "Empty Room";
    }
    if (count == 1) {
        return memberNames[0];
    }
    if (count == 2) {
        return memberNames[0] + " and " + memberNames[1];
    }
    if (count == 3) {
        return memberNames[0] + ", " + memberNames[1] + ", and " + memberNames[2];
    }
    if (count == 4) {
        return memberNames[0] + ", " + memberNames[1] + ", " +
               memberNames[2] + ", and " + memberNames[3];
    }
    // 5+ members: show first 3 and count remaining
    int remaining = count - 3;
    return memberNames[0] + ", " + memberNames[1] + ", " +
           memberNames[2] + ", and " + std::to_string(remaining) + " others";
}

// ==== Resolve Room Name (Full) ====
//
// Original Kotlin (RoomDisplayNameResolver.kt resolve with source tracking):
//   Wraps computeRoomDisplayName and adds source attribution metadata.
//   Determines where the final name came from: state event, alias, members, heroes, or inviter.

RoomNameInfo resolveRoomName(
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
    const std::vector<std::string>& excludedUserIds)
{
    RoomNameInfo info;

    // Original Kotlin: Try m.room.name state event first
    std::string parsedRoomName = parseRoomNameContent(roomNameState);
    if (!parsedRoomName.empty()) {
        info.name = parsedRoomName;
        info.computedDisplayName = parsedRoomName;
        info.isNameFromState = true;
        info.totalMemberCount = joinedCount + invitedCount;
        return info;
    }

    // Original Kotlin: Try canonical alias
    std::string parsedAlias = parseCanonicalAliasContent(canonicalAlias);
    if (!parsedAlias.empty()) {
        info.name = parsedAlias;
        info.computedDisplayName = parsedAlias;
        info.isNameFromAlias = true;
        info.totalMemberCount = joinedCount + invitedCount;
        return info;
    }

    // Original Kotlin: Invite rooms — show inviter
    if (isInvite) {
        info.name = inviterName.empty() ? "Room Invitation" : inviterName;
        info.computedDisplayName = info.name;
        info.isNameFromInviter = true;
        info.totalMemberCount = joinedCount + invitedCount;
        return info;
    }

    // Original Kotlin: Build member list for name computation
    auto isExcluded = [&](const std::string& uid) -> bool {
        if (uid == myUserId) return true;
        for (const auto& ex : excludedUserIds) {
            if (ex == uid) return true;
        }
        return false;
    };

    std::vector<RoomMember> others;

    // Use heroes if available
    if (!heroes.empty()) {
        for (const auto& hero : heroes) {
            if ((hero.membership == RoomMembership::Join || hero.membership == RoomMembership::Invite) &&
                !isExcluded(hero.userId)) {
                others.push_back(hero);
            }
        }
        info.isNameFromHeroes = true;
    } else {
        // Use active members
        for (const auto& member : activeMembers) {
            if (isExcluded(member.userId)) continue;
            others.push_back(member);
            if (others.size() >= 5) break;
        }
        info.isNameFromMembers = true;
    }

    info.memberCount = static_cast<int>(others.size());
    info.totalMemberCount = joinedCount + invitedCount;

    // Collect source member names
    for (const auto& m : others) {
        info.sourceMemberNames.push_back(m.displayName.empty() ? m.userId : m.displayName);
    }

    // Compute display name from member list
    RoomName computed = computeRoomDisplayName(
        roomNameState, canonicalAlias, heroes, activeMembers, leftMembers,
        myUserId, directUserId, inviterName, invitedCount, joinedCount,
        isInvite, excludedUserIds
    );

    info.name = computed.name;
    info.computedDisplayName = computed.name;

    return info;
}

// ==== Check if Room Name is Generated ====
//
// Original Kotlin: Determines if the room name was auto-generated
//   from members vs. explicitly set via state event or alias

bool isRoomNameGenerated(const RoomNameInfo& info) {
    return info.isAutoGenerated();
}

// ==== Room Name Info to JSON ====
//
// Original Kotlin: Serialize RoomNameInfo for JNI bridge to Kotlin UI layer

std::string roomNameInfoToJson(const RoomNameInfo& info) {
    std::ostringstream json;
    json << "{";
    json << R"("name":")" << info.name << R"(",)";
    json << R"("computed_display_name":")" << info.computedDisplayName << R"(",)";
    json << R"("is_name_from_state":)" << (info.isNameFromState ? "true" : "false") << ",";
    json << R"("is_name_from_alias":)" << (info.isNameFromAlias ? "true" : "false") << ",";
    json << R"("is_name_from_members":)" << (info.isNameFromMembers ? "true" : "false") << ",";
    json << R"("is_name_from_heroes":)" << (info.isNameFromHeroes ? "true" : "false") << ",";
    json << R"("is_name_from_inviter":)" << (info.isNameFromInviter ? "true" : "false") << ",";
    json << R"("member_count":)" << info.memberCount << ",";
    json << R"("total_member_count":)" << info.totalMemberCount << ",";
    json << R"("is_auto_generated":)" << (info.isAutoGenerated() ? "true" : "false") << ",";
    json << R"("is_empty_room":)" << (info.isEmptyRoom() ? "true" : "false") << ",";

    // Source member names array
    json << R"("source_member_names":[)";
    bool first = true;
    for (const auto& name : info.sourceMemberNames) {
        if (!first) json << ",";
        first = false;
        json << R"(")" << name << R"(")";
    }
    json << "]";

    json << "}";
    return json.str();
}

} // namespace progressive
