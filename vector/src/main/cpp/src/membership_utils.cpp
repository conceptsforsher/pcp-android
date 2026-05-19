#include "progressive/membership_utils.hpp"
#include "progressive/json_parser.hpp"
#include <sstream>
#include <algorithm>

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

} // namespace progressive
