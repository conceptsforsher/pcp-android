#include "pcp/room_state_manager.hpp"
#include "pcp/room_content.hpp"
#include <sstream>
#include <algorithm>

namespace pcp {

// ====== Enums ======





// ====== Helpers ======

static std::string extractStr(const std::string& json, const std::string& key) {
    auto pp = json.find("\"" + key + "\"");
    if (pp == std::string::npos) return "";
    pp = json.find('"', pp + key.size() + 2);
    if (pp == std::string::npos) return "";
    pp++;
    size_t e = pp;
    while (e < json.size() && json[e] != '"') e++;
    return json.substr(pp, e - pp);
}

// ====== History Visibility Functions ======
// Original: shouldShareHistory() = WORLD_READABLE || SHARED

bool shouldShareHistory(RSM_RoomHistoryVisibility visibility) {
    return visibility == RSM_RoomHistoryVisibility::WORLD_READABLE ||
           visibility == RSM_RoomHistoryVisibility::SHARED;
}

bool canSeeEvent(RSM_RoomHistoryVisibility visibility, MembershipState memberStateAtEventTime,
                  MembershipState memberCurrentState) {
    switch (visibility) {
        case RSM_RoomHistoryVisibility::WORLD_READABLE:
            // Anyone can see all events, even non-members
            return true;

        case RSM_RoomHistoryVisibility::SHARED:
            // Joined members see all events; non-members see nothing
            return memberCurrentState == MembershipState::JOIN;

        case RSM_RoomHistoryVisibility::INVITED:
            // Members see events from when they were invited onwards
            return memberCurrentState == MembershipState::JOIN ||
                   memberCurrentState == MembershipState::INVITE ||
                   (memberStateAtEventTime == MembershipState::INVITE);

        case RSM_RoomHistoryVisibility::JOINED:
            // Members see events from when they joined onwards
            return memberCurrentState == MembershipState::JOIN &&
                   memberStateAtEventTime == MembershipState::JOIN;
    }
    return false;
}

bool canNonMemberSeeEvents(RSM_RoomHistoryVisibility visibility) {
    return visibility == RSM_RoomHistoryVisibility::WORLD_READABLE;
}

std::string getVisibilityLabel(RSM_RoomHistoryVisibility visibility) {
    switch (visibility) {
        case RSM_RoomHistoryVisibility::WORLD_READABLE: return "Anyone";
        case RSM_RoomHistoryVisibility::SHARED: return "Members (since beginning)";
        case RSM_RoomHistoryVisibility::INVITED: return "Members (since invite)";
        case RSM_RoomHistoryVisibility::JOINED: return "Members (since join)";
    }
    return "Members";
}

std::string getVisibilityDescription(RSM_RoomHistoryVisibility visibility) {
    switch (visibility) {
        case RSM_RoomHistoryVisibility::WORLD_READABLE:
            return "Anyone can read the room history, even without joining.";
        case RSM_RoomHistoryVisibility::SHARED:
            return "All members can see the entire room history.";
        case RSM_RoomHistoryVisibility::INVITED:
            return "Members can see history from the point they were invited.";
        case RSM_RoomHistoryVisibility::JOINED:
            return "Members can only see history from the point they joined.";
    }
    return "";
}

// ====== Content Builders ======

static std::string roomHistoryVisibilityToString(RSM_RoomHistoryVisibility vis) { switch(vis) { case RSM_RoomHistoryVisibility::WORLD_READABLE: return "world_readable"; case RSM_RoomHistoryVisibility::SHARED: return "shared"; case RSM_RoomHistoryVisibility::INVITED: return "invited"; case RSM_RoomHistoryVisibility::JOINED: return "joined"; } return "joined"; }

std::string buildHistoryVisibilityContent(RSM_RoomHistoryVisibility visibility) {
    return R"({"history_visibility":")" + std::string(roomHistoryVisibilityToString(visibility)) + R"("})";
}

static std::string roomJoinRulesToString(RoomJoinRule rule) { switch(rule) { case RoomJoinRule::PUBLIC: return "public"; case RoomJoinRule::KNOCK: return "knock"; case RoomJoinRule::INVITE: return "invite"; case RoomJoinRule::PRIVATE: return "private"; } return "invite"; }

std::string buildJoinRulesContent(RoomJoinRule rule) {
    return R"({"join_rule":")" + std::string(roomJoinRulesToString(rule)) + R"("})";
}



// ====== pcp::RoomStateManager ======

RoomStateManager::RoomStateManager() {}

void pcp::RoomStateManager::setHistoryVisibility(const std::string& roomId, RSM_RoomHistoryVisibility visibility) {
    auto it = rooms_.find(roomId);
    auto& state = it->second;
    state.historyVisibility = visibility;
    state.isWorldReadable = (visibility == RSM_RoomHistoryVisibility::WORLD_READABLE);
    state.canShareHistory = shouldShareHistory(visibility);
}

void pcp::RoomStateManager::setJoinRule(const std::string& roomId, RoomJoinRule rule) {
    auto it = rooms_.find(roomId);
    if (it == rooms_.end()) {
        RoomStateSummary s;
        s.roomId = roomId;
        rooms_[roomId] = s;
        it = rooms_.find(roomId);
    }
    auto& state = it->second;
    state.joinRule = rule;
    state.isPublicRoom = (rule == RoomJoinRule::PUBLIC);
}

void pcp::RoomStateManager::setRoomName(const std::string& roomId, const std::string& name) {
    rooms_[roomId].roomName = name;
}

void pcp::RoomStateManager::setEncrypted(const std::string& roomId, bool encrypted) {
    rooms_[roomId].isEncrypted = encrypted;
}

void pcp::RoomStateManager::setMemberCount(const std::string& roomId, int count) {
    rooms_[roomId].memberCount = count;
}

RoomStateSummary pcp::RoomStateManager::getRoomState(const std::string& roomId) const {
    auto it = rooms_.find(roomId);
    if (it != rooms_.end()) return it->second;
    RoomStateSummary s;
    s.roomId = roomId;
    return s;
}

bool pcp::RoomStateManager::canShareRoomHistory(const std::string& roomId) const {
    auto state = getRoomState(roomId);
    return state.canShareHistory;
}

bool pcp::RoomStateManager::isPublicRoom(const std::string& roomId) const {
    return getRoomState(roomId).isPublicRoom;
}

bool pcp::RoomStateManager::isWorldReadable(const std::string& roomId) const {
    return getRoomState(roomId).isWorldReadable;
}

bool pcp::RoomStateManager::isInviteOnly(const std::string& roomId) const {
    return getRoomState(roomId).joinRule == RoomJoinRule::INVITE;
}

bool pcp::RoomStateManager::areGuestsAllowed(const std::string& roomId) const {
    auto state = getRoomState(roomId);
    return state.isPublicRoom && state.isWorldReadable;
}

void pcp::RoomStateManager::clear() { rooms_.clear(); }

// ====== Serialization ======

std::string pcp::RoomStateManager::roomStateToJson(const RoomStateSummary& state) const {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    std::ostringstream os;
    os << R"({"room_id":")" << esc(state.roomId)
       << R"(","name":")" << esc(state.roomName)
       << R"(","history_visibility":")" << roomHistoryVisibilityToString(state.historyVisibility)
       << R"(","visibility_label":")" << getVisibilityLabel(state.historyVisibility)
       << R"(","join_rule":")" << roomJoinRulesToString(state.joinRule)
       << R"(,"is_public":)" << (state.isPublicRoom ? "true" : "false")
       << R"(,"is_world_readable":)" << (state.isWorldReadable ? "true" : "false")
       << R"(,"can_share_history":)" << (state.canShareHistory ? "true" : "false")
       << R"(,"is_invite_only":)" << (roomJoinRulesToString(state.joinRule) == std::string("invite") ? "true" : "false")
       << R"(,"is_encrypted":)" << (state.isEncrypted ? "true" : "false")
       << R"(,"members":)" << state.memberCount
       << "}";
    return os.str();
}

} // namespace pcp
