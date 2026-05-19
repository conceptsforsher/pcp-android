#include "progressive/membership_utils.hpp"
#include "progressive/json_parser.hpp"
#include <sstream>
#include <algorithm>
#include <unordered_set>
#include <cctype>

namespace progressive {

MemberState parseMemberState(const std::string& membershipStr) {
    if (membershipStr == "join")     return MemberState::Join;
    if (membershipStr == "invite")   return MemberState::Invite;
    if (membershipStr == "leave")    return MemberState::Leave;
    if (membershipStr == "ban")      return MemberState::Ban;
    if (membershipStr == "knock")    return MemberState::Knock;
    return MemberState::Unknown;
}

MemberInfo parseMemberInfo(const std::string& stateContentJson, const std::string& userId) {
    MemberInfo info;
    info.userId = userId;

    auto membershipStr = parseJsonStringValue(stateContentJson, "membership");
    info.membership = parseMemberState(membershipStr);

    info.displayName = parseJsonStringValue(stateContentJson, "displayname");
    info.avatarUrl   = parseJsonStringValue(stateContentJson, "avatar_url");
    info.reason      = parseJsonStringValue(stateContentJson, "reason");

    auto ts = parseJsonStringValue(stateContentJson, "origin_server_ts");
    if (!ts.empty()) info.timestampMs = std::stoll(ts);

    return info;
}

std::string formatMemberState(MemberState membership) {
    switch (membership) {
        case MemberState::Join:  return "Joined";
        case MemberState::Invite: return "Invited";
        case MemberState::Leave: return "Left";
        case MemberState::Ban:   return "Banned";
        case MemberState::Knock: return "Knocked";
        default:                return "Unknown";
    }
}

bool isActiveMember(MemberState membership) {
    return membership == MemberState::Join ||
           membership == MemberState::Invite ||
           membership == MemberState::Knock;
}

bool canReadMessages(MemberState membership) {
    return membership == MemberState::Join;
}

MemberStateChange detectMemberStateChange(const MemberInfo& oldInfo, const MemberInfo& newInfo) {
    MemberStateChange change;
    change.userId = oldInfo.userId;
    change.displayName = newInfo.displayName.empty() ? oldInfo.displayName : newInfo.displayName;
    change.oldMemberState = oldInfo.membership;
    change.newMemberState = newInfo.membership;
    change.timestampMs = newInfo.timestampMs;
    return change;
}

std::string formatMemberStateChange(const MemberStateChange& change) {
    std::ostringstream out;
    out << change.displayName;
    if (change.oldMemberState == MemberState::Unknown) {
        out << " " << formatMemberState(change.newMemberState);
    } else {
        out << " changed from " << formatMemberState(change.oldMemberState)
            << " to " << formatMemberState(change.newMemberState);
    }
    return out.str();
}

MemberListInfo parseMemberList(const std::string& roomId, const std::string& apiResponseJson, bool isTruncated) {
    MemberListInfo list;
    list.roomId = roomId;
    list.isTruncated = isTruncated;

    // Parse each chunk/event
    size_t pos = 0;
    while (true) {
        pos = apiResponseJson.find("\"user_id\"", pos);
        if (pos == std::string::npos) {
            pos = apiResponseJson.find("\"sender\"", pos);
            if (pos == std::string::npos) break;
        }

        auto objStart = apiResponseJson.rfind('{', pos);
        if (objStart == std::string::npos) break;

        int depth = 0;
        auto objEnd = objStart;
        while (objEnd < apiResponseJson.size()) {
            if (apiResponseJson[objEnd] == '{') ++depth;
            else if (apiResponseJson[objEnd] == '}') --depth;
            if (depth == 0) break;
            ++objEnd;
        }
        if (objEnd >= apiResponseJson.size()) break;

        std::string obj = apiResponseJson.substr(objStart, objEnd - objStart + 1);

        MemberInfo info;
        info.userId      = parseJsonStringValue(obj, "user_id");
        if (info.userId.empty()) info.userId = parseJsonStringValue(obj, "sender");
        info.displayName = parseJsonStringValue(obj, "display_name");
        if (info.displayName.empty()) {
            auto content = parseJsonStringValue(obj, "content");
            if (!content.empty()) {
                info.displayName = parseJsonStringValue("{" + content + "}", "displayname");
                info.avatarUrl   = parseJsonStringValue("{" + content + "}", "avatar_url");
                auto ms = parseJsonStringValue("{" + content + "}", "membership");
                info.membership = parseMemberState(ms);
            }
        }
        info.avatarUrl = parseJsonStringValue(obj, "avatar_url");

        if (!info.userId.empty()) list.members.push_back(info);
        pos = objEnd + 1;
    }

    list.totalMembers = static_cast<int>(list.members.size());
    for (const auto& m : list.members) {
        switch (m.membership) {
            case MemberState::Join:   list.joinedMembers++; break;
            case MemberState::Invite: list.invitedMembers++; break;
            case MemberState::Ban:    list.bannedMembers++; break;
            default: break;
        }
    }

    return list;
}

std::vector<MemberInfo> filterByMemberState(const std::vector<MemberInfo>& members, MemberState type) {
    std::vector<MemberInfo> result;
    for (const auto& m : members) {
        if (m.membership == type) result.push_back(m);
    }
    return result;
}

std::vector<MemberInfo> searchMembers(const std::vector<MemberInfo>& members, const std::string& query) {
    if (query.empty()) return members;
    auto lowerQuery = query;
    std::transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);

    std::vector<MemberInfo> result;
    for (const auto& m : members) {
        auto lowerName = m.displayName;
        auto lowerId = m.userId;
        std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
        std::transform(lowerId.begin(), lowerId.end(), lowerId.begin(), ::tolower);
        if (lowerName.find(lowerQuery) != std::string::npos ||
            lowerId.find(lowerQuery) != std::string::npos) {
            result.push_back(m);
        }
    }
    return result;
}

void sortMembers(std::vector<MemberInfo>& members, const std::string& sortBy) {
    if (sortBy == "power") {
        std::sort(members.begin(), members.end(), [](const auto& a, const auto& b) {
            return a.powerLevel > b.powerLevel;
        });
    } else if (sortBy == "date") {
        std::sort(members.begin(), members.end(), [](const auto& a, const auto& b) {
            return a.timestampMs > b.timestampMs;
        });
    } else { // name
        std::sort(members.begin(), members.end(), [](const auto& a, const auto& b) {
            auto na = a.displayName;
            auto nb = b.displayName;
            std::transform(na.begin(), na.end(), na.begin(), ::tolower);
            std::transform(nb.begin(), nb.end(), nb.begin(), ::tolower);
            return na < nb;
        });
    }
}

std::string memberListToJson(const MemberListInfo& list) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"roomId": ")" << esc(list.roomId) << R"(")";
    json << R"(,"totalMembers": )" << list.totalMembers << ",";
    json << R"(,"joined": )" << list.joinedMembers << ",";
    json << R"(,"invited": )" << list.invitedMembers;
    json << "}";
    return json.str();
}

// ==== Member Sorting (from RoomMemberListComparator.kt:14-52) ====
// Original: compare by powerLevel desc, then displayName asc CI, then userId asc

bool memberCompare(const MemberInfo& a, const MemberInfo& b) {
    // Sort by power level (higher = first)
    if (a.powerLevel != b.powerLevel) return a.powerLevel > b.powerLevel;

    const auto& aName = a.displayName;
    const auto& bName = b.displayName;

    // If both have names, compare case-insensitive
    if (!aName.empty() && !bName.empty()) {
        // Case-insensitive compare
        auto al = aName, bl = bName;
        for (char& c : al) c = std::tolower(static_cast<unsigned char>(c));
        for (char& c : bl) c = std::tolower(static_cast<unsigned char>(c));
        if (al != bl) return al < bl;
        // Same name → compare userId
        return a.userId < b.userId;
    }

    // One has no display name — named members first
    if (aName.empty() && !bName.empty()) return false;
    if (!aName.empty() && bName.empty()) return true;

    // Both unnamed → compare userId
    return a.userId < b.userId;
}

void sortMembersByPowerAndName(std::vector<MemberInfo>& members) {
    std::sort(members.begin(), members.end(), memberCompare);
}

// ==== MemberState Diff (from TimelineEventVisibilityHelper.kt:261-279) ====

MemberStateDiff computeMemberStateDiff(
    MemberState oldMemberState, MemberState newMemberState,
    const std::string& oldName, const std::string& newName,
    const std::string& oldAvatar, const std::string& newAvatar,
    bool isSelf)
{
    MemberStateDiff diff;

    // Original: val isMemberStateChanged = content?.membership != prevContent?.membership
    bool membershipChanged = (oldMemberState != newMemberState);

    // Original: val isJoin = isMemberStateChanged && content?.membership == MemberState.JOIN
    diff.isJoin = membershipChanged && newMemberState == MemberState::Join;

    // Original: val isPart = isMemberStateChanged && content?.membership == LEAVE && root.stateKey == root.senderId
    diff.isPart = membershipChanged && newMemberState == MemberState::Leave && isSelf;

    // Original: val isProfileChanged = !isMemberStateChanged && content?.membership == MemberState.JOIN
    bool profileChanged = !membershipChanged && newMemberState == MemberState::Join;

    // Original: val isDisplaynameChange = isProfileChanged && content?.displayName != prevContent?.displayName
    diff.isDisplaynameChange = profileChanged && oldName != newName;

    // Original: val isAvatarChange = isProfileChanged && content?.avatarUrl !== prevContent?.avatarUrl
    diff.isAvatarChange = profileChanged && oldAvatar != newAvatar;

    diff.hasChanged = diff.isJoin || diff.isPart || diff.isDisplaynameChange || diff.isAvatarChange;
    return diff;
}

// ==== Member Event Handling (from RoomMemberEventHandler.kt:37-153) ====

MemberChangeAction computeMemberChangeAction(
    const std::string& oldMembership,
    const std::string& newMembership,
    bool isSelfAction)
{
    // Original Kotlin: membership transition logic in handleIncrementalSync
    // + Membership.toMembershipChangeState() from RoomChangeMembershipStateDataSource
    bool membershipChanged = (oldMembership != newMembership);

    if (membershipChanged) {
        if (newMembership == "join")   return MemberChangeAction::JOINED;
        if (newMembership == "invite") return MemberChangeAction::INVITED;
        if (newMembership == "leave")  return isSelfAction ? MemberChangeAction::LEFT : MemberChangeAction::KICKED;
        if (newMembership == "ban")    return MemberChangeAction::BANNED;
        if (newMembership == "knock")  return MemberChangeAction::KNOCKED;
    } else if (newMembership == "join") {
        return MemberChangeAction::PROFILE_CHANGED;
    }
    return MemberChangeAction::UNKNOWN;
}

MemberChangeInfo processMemberEvent(
    const std::string& roomId,
    const std::string& eventJson,
    const std::string& prevContentJson,
    bool isInitialSync)
{
    // Original Kotlin: RoomMemberEventHandler.handle()
    MemberChangeInfo info;
    info.roomId = roomId;

    // Extract userId from stateKey
    info.userId = parseJsonStringValue(eventJson, "state_key");
    if (info.userId.empty()) return info;

    // Extract sender
    info.senderId = parseJsonStringValue(eventJson, "sender");

    // Extract timestamp
    auto ts = parseJsonStringValue(eventJson, "origin_server_ts");
    if (!ts.empty()) info.timestamp = std::stoll(ts);

    // Parse membership content
    auto content = parseJsonStringValue(eventJson, "content");
    std::string contentJson = "{" + content + "}";

    info.membership    = parseJsonStringValue(contentJson, "membership");
    info.displayName   = parseJsonStringValue(contentJson, "displayname");
    info.avatarUrl     = parseJsonStringValue(contentJson, "avatar_url");
    info.reason        = parseJsonStringValue(contentJson, "reason");
    info.prevMembership = info.membership;

    // For incremental sync, diff against prevContent
    if (!isInitialSync && !prevContentJson.empty()) {
        info.prevMembership = parseJsonStringValue(prevContentJson, "membership");
        std::string prevName  = parseJsonStringValue(prevContentJson, "displayname");
        std::string prevAvatar = parseJsonStringValue(prevContentJson, "avatar_url");

        bool membershipChanged = (info.prevMembership != info.membership);
        if (membershipChanged) {
            // Original: computeMemberStateDiff → detect join/part/leave/ban
            bool isSelf = (info.senderId == info.userId);
            info.action = computeMemberChangeAction(info.prevMembership, info.membership, isSelf);
        } else {
            // No membership change, check for profile changes
            // Original: userIdsToFetch for profile diffs
            if ((!prevName.empty() && prevName != info.displayName) ||
                (!prevAvatar.empty() && prevAvatar != info.avatarUrl)) {
                info.isProfileChange = true;
                if (prevName != info.displayName && prevAvatar != info.avatarUrl) {
                    info.action = MemberChangeAction::PROFILE_CHANGED;
                } else if (prevName != info.displayName) {
                    info.action = MemberChangeAction::DISPLAY_NAME_CHANGED;
                } else {
                    info.action = MemberChangeAction::AVATAR_CHANGED;
                }
            }
        }
    } else {
        // Initial sync: no diff, just record the join/invite
        if (info.membership == "join")  info.action = MemberChangeAction::JOINED;
        if (info.membership == "invite") info.action = MemberChangeAction::INVITED;
        if (info.membership == "leave")  info.action = MemberChangeAction::LEFT;
        if (info.membership == "ban")    info.action = MemberChangeAction::BANNED;
        if (info.membership == "knock")  info.action = MemberChangeAction::KNOCKED;
    }

    info.isValid = true;
    return info;
}

// ==== Room Display Name Resolution (from RoomDisplayNameResolver.kt:59-166) ====

static std::string normalizeForCompare(const std::string& s) {
    std::string out = s;
    for (char& c : out) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return out;
}

std::string formatMemberNameList(const std::vector<std::string>& names, int maxNames) {
    // Original Kotlin: roomDisplayNameFallbackProvider.getNameForXmembers()
    // Formats "A", "A and B", "A, B and C", "A, B, C and +N more"
    if (names.empty()) return "";

    std::ostringstream out;
    size_t count = names.size();

    if (count == 1) {
        out << names[0];
    } else if (count == 2) {
        out << names[0] << " and " << names[1];
    } else if (count == 3) {
        out << names[0] << ", " << names[1] << " and " << names[2];
    } else {
        // More than 3: show first maxNames, then "+N more"
        if (maxNames < 1) maxNames = 1;
        size_t shown = std::min(static_cast<size_t>(maxNames), count);
        for (size_t i = 0; i < shown; i++) {
            if (i > 0) out << ", ";
            out << names[i];
        }
        out << " and " << (count - shown) << " others";
    }
    return out.str();
}

std::string formatRoomHeroesList(
    const std::vector<std::string>& heroIds,
    const std::vector<MemberInfo>& members,
    int maxToShow)
{
    // Original: heroes list from room summary → resolve names → format
    std::vector<std::string> heroNames;
    for (const auto& heroId : heroIds) {
        bool found = false;
        for (const auto& m : members) {
            if (m.userId == heroId) {
                heroNames.push_back(m.displayName.empty() ? m.userId : m.displayName);
                found = true;
                break;
            }
        }
        if (!found) heroNames.push_back(heroId);
    }
    return formatMemberNameList(heroNames, maxToShow);
}

RoomNameResult resolveRoomDisplayName(
    const std::string& roomName,
    const std::string& canonicalAlias,
    const std::string& currentMembership,
    bool isDirectRoom,
    const std::string& directUserId,
    const std::vector<std::string>& heroes,
    const std::vector<MemberInfo>& activeMembers,
    const std::vector<MemberInfo>& leftMembers,
    const std::string& currentUserId,
    const std::vector<std::string>& excludedUserIds,
    const RoomDisplayNameConfig& config)
{
    // Original Kotlin: RoomDisplayNameResolver.resolve()
    // Matrix algorithm per js-sdk calculateRoomName()
    RoomNameResult result;

    // 1) m.room.name
    if (!roomName.empty()) {
        result.displayName = roomName;
        result.normalizedName = normalizeForCompare(roomName);
        result.source = RoomNameSource::STATE_EVENT;
        result.isFromState = true;
        return result;
    }

    // 2) Canonical alias
    if (!canonicalAlias.empty()) {
        result.displayName = canonicalAlias;
        result.normalizedName = normalizeForCompare(canonicalAlias);
        result.source = RoomNameSource::ALIAS;
        return result;
    }

    // Build set of excluded userIds for quick lookup
    std::unordered_set<std::string> excludedSet(excludedUserIds.begin(), excludedUserIds.end());

    // Helper: check if a user is excluded
    auto isExcluded = [&](const std::string& uid) -> bool {
        return excludedSet.find(uid) != excludedSet.end();
    };

    // 3) Invite case: show inviter name
    if (currentMembership == "invite") {
        // For invited rooms, the name is typically the inviter
        // Look for active members, prefer one who is NOT current user
        for (const auto& m : activeMembers) {
            if (m.userId != currentUserId && !isExcluded(m.userId)) {
                std::string name = m.displayName.empty() ? m.userId : m.displayName;
                result.displayName = name;
                result.normalizedName = normalizeForCompare(name);
                result.source = RoomNameSource::MEMBERS;
                return result;
            }
        }
        // Fallback: use roomId
        result.displayName = config.emptyRoomName;
        result.normalizedName = normalizeForCompare(result.displayName);
        result.source = RoomNameSource::GENERATED;
        return result;
    }

    // 4) Joined room: compute name from members
    // Build a filtered list of "other members" (not current user, not excluded)
    std::vector<const MemberInfo*> otherMembers;
    for (const auto& m : activeMembers) {
        if (m.userId != currentUserId && !isExcluded(m.userId)) {
            otherMembers.push_back(&m);
        }
    }

    // If there are heroes, prefer them first
    std::vector<const MemberInfo*> displayMembers;
    if (config.showHeroesFirst && !heroes.empty()) {
        for (const auto& heroId : heroes) {
            for (const auto& m : activeMembers) {
                if (m.userId == heroId && !isExcluded(m.userId) && m.userId != currentUserId) {
                    displayMembers.push_back(&m);
                    break;
                }
            }
        }
    }

    // If no heroes matched or heroes disabled, use all other members (limit 5)
    if (displayMembers.empty()) {
        for (auto* m : otherMembers) {
            if (displayMembers.size() < 5) {
                displayMembers.push_back(m);
            }
        }
    }

    size_t otherCount = displayMembers.size();

    // Collect display names from displayMembers
    std::vector<std::string> names;
    for (auto* m : displayMembers) {
        names.push_back(m->displayName.empty() ? m->userId : m->displayName);
    }

    // Also collect left member names for fallback
    std::vector<std::string> leftNames;
    for (const auto& m : leftMembers) {
        if (!isExcluded(m.userId)) {
            leftNames.push_back(m.displayName.empty() ? m.userId : m.displayName);
        }
    }

    if (otherCount == 0) {
        // Empty room: try left members or DM userId
        if (isDirectRoom && !directUserId.empty()) {
            result.displayName = directUserId;
            result.normalizedName = normalizeForCompare(directUserId);
            result.source = RoomNameSource::DIRECT_USER;
        } else if (!leftNames.empty()) {
            result.displayName = formatMemberNameList(leftNames, config.maxMembersToShow);
            result.normalizedName = normalizeForCompare(result.displayName);
            result.source = RoomNameSource::MEMBERS;
        } else {
            result.displayName = config.emptyRoomName;
            result.normalizedName = normalizeForCompare(config.emptyRoomName);
            result.source = RoomNameSource::GENERATED;
        }
    } else {
        result.displayName = formatMemberNameList(names, config.maxMembersToShow);
        result.normalizedName = normalizeForCompare(result.displayName);
        result.source = (config.showHeroesFirst && !heroes.empty())
            ? RoomNameSource::HEROES : RoomNameSource::MEMBERS;
    }

    return result;
}

// ==== Room Avatar Resolution (from RoomAvatarResolver.kt:48-84) ====

std::string resolveRoomAvatar(
    const std::string& roomAvatarUrl,
    bool isDirectRoom,
    const std::vector<MemberInfo>& activeMembers,
    const std::vector<MemberInfo>& leftMembers,
    const std::string& currentUserId,
    const std::vector<std::string>& excludedUserIds)
{
    // Original Kotlin: RoomAvatarResolver.resolve()
    // 1) Use m.room.avatar if set
    if (!roomAvatarUrl.empty()) return roomAvatarUrl;

    // 2) For DM rooms, use the other member's avatar
    if (isDirectRoom) {
        std::unordered_set<std::string> excludedSet(excludedUserIds.begin(), excludedUserIds.end());
        auto isExcluded = [&](const std::string& uid) {
            return excludedSet.find(uid) != excludedSet.end();
        };

        // Count active members excluding excluded and current user
        std::vector<const MemberInfo*> others;
        for (const auto& m : activeMembers) {
            if (m.userId != currentUserId && !isExcluded(m.userId)) {
                others.push_back(&m);
            }
        }

        if (others.size() == 1) {
            // Single other member: use their avatar, or fallback to a left member
            if (!others[0]->avatarUrl.empty()) return others[0]->avatarUrl;
            for (const auto& m : leftMembers) {
                if (!m.avatarUrl.empty() && !isExcluded(m.userId)) {
                    return m.avatarUrl;
                }
            }
            return others[0]->avatarUrl; // might be empty
        } else if (others.size() == 2) {
            for (const auto& m : others) {
                if (!m->avatarUrl.empty()) return m->avatarUrl;
            }
        }
    }

    return "";
}

} // namespace progressive
