#include "progressive/invite_utils.hpp"
#include "progressive/json_parser.hpp"
#include <sstream>
#include <algorithm>
#include <chrono>

namespace progressive {

InviteInfo parseInviteInfo(const std::string& roomId, const std::string& stateEventsJson) {
    InviteInfo info;
    info.roomId = roomId;

    info.roomName = parseJsonStringValue(stateEventsJson, "name");
    info.inviterName = parseJsonStringValue(stateEventsJson, "sender_name");
    info.inviterId   = parseJsonStringValue(stateEventsJson, "sender");

    auto ts = parseJsonStringValue(stateEventsJson, "origin_server_ts");
    if (!ts.empty()) info.invitedAtMs = std::stoll(ts);

    auto isDirect = parseJsonStringValue(stateEventsJson, "is_direct");
    info.isDirect = (isDirect == "true");

    auto memberCountStr = parseJsonStringValue(stateEventsJson, "num_joined_members");
    if (!memberCountStr.empty()) info.memberCount = std::stoi(memberCountStr);

    return info;
}

InviteValidation validateInvite(
    const std::string& inviteeId,
    const std::string& myUserId,
    const std::vector<std::string>& currentMembers,
    const std::vector<std::string>& bannedUsers,
    const std::string& reason,
    int maxReasonLength
) {
    InviteValidation result;

    if (inviteeId.empty()) {
        result.errorMessage = "User ID is required.";
        return result;
    }

    if (inviteeId == myUserId) {
        result.isSelf = true;
        result.errorMessage = "You cannot invite yourself.";
        return result;
    }

    // Already member
    for (const auto& member : currentMembers) {
        if (member == inviteeId) {
            result.alreadyMember = true;
            result.errorMessage = "User is already a member of this room.";
            return result;
        }
    }

    // Banned
    for (const auto& banned : bannedUsers) {
        if (banned == inviteeId) {
            result.banned = true;
            result.errorMessage = "User is banned from this room.";
            return result;
        }
    }

    // Reason too long
    if (static_cast<int>(reason.size()) > maxReasonLength) {
        result.errorMessage = "Invite reason is too long (max " +
                              std::to_string(maxReasonLength) + " characters).";
        return result;
    }

    result.valid = true;
    return result;
}

std::string buildInviteBody(const std::string& userId, const std::string& reason) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"user_id": ")" << esc(userId) << R"(")";
    if (!reason.empty())
        json << R"(,"reason": ")" << esc(reason) << R"(")";
    json << "}";
    return json.str();
}

std::string buildThreePidInviteBody(const std::string& idServer,
    const std::string& medium, const std::string& address) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"id_server": ")" << esc(idServer) << R"(")";
    json << R"(,"medium": ")" << esc(medium) << R"(")";
    json << R"(,"address": ")" << esc(address) << R"(")";
    json << "}";
    return json.str();
}

std::string formatInvitePreview(const InviteInfo& info) {
    std::ostringstream out;
    if (info.isEncrypted) out << "🔒 ";
    out << info.roomName;
    if (!info.inviterName.empty()) {
        out << "\nInvited by " << info.inviterName;
    }
    if (info.memberCount > 0) {
        out << " · " << info.memberCount << " members";
    }
    return out.str();
}

bool isInviteExpired(int64_t invitedAtMs, int maxAgeDays) {
    if (invitedAtMs <= 0) return false;
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    return (now - invitedAtMs) > maxAgeDays * 24LL * 3600 * 1000;
}

void sortInvites(std::vector<InviteInfo>& invites) {
    std::sort(invites.begin(), invites.end(), [](const InviteInfo& a, const InviteInfo& b) {
        return a.invitedAtMs > b.invitedAtMs;
    });
}

std::string buildKnockBody(const std::string& reason) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    if (reason.empty()) return "{}";
    return R"({"reason": ")" + esc(reason) + R"("})";
}

std::string formatKnockReason(const std::string& reason) {
    if (reason.empty()) return "No reason provided";
    return reason;
}

} // namespace progressive
