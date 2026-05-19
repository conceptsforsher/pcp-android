#include "progressive/room_state_manager.hpp"
#include "progressive/room_content.hpp"
#include <sstream>
#include <algorithm>

namespace progressive {

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

std::string buildHistoryVisibilityContent(RSM_RoomHistoryVisibility visibility) {
    return R"({"history_visibility":")" + std::string(roomHistoryVisibilityToString(visibility)) + R"("})";
}

std::string buildJoinRulesContent(RoomJoinRule rule) {
    return R"({"join_rule":")" + std::string(roomJoinRulesToString(rule)) + R"("})";
}

    auto vis = extractStr(contentJson, "history_visibility");
    return historyVisibilityFromString(vis);
}

    auto rule = extractStr(contentJson, "join_rule");
    return joinRuleFromString(rule);
}

// ====== progressive::RoomStateManager ======

progressive::RoomStateManager::progressive::RoomStateManager() {}

progressive::progressive::RSM_RoomStateSummary& progressive::RoomStateManager::getOrCreateState(const std::string& roomId) {
    auto it = rooms_.find(roomId);
    if (it == rooms_.end()) {
        progressive::progressive::RSM_RoomStateSummary s;
        s.roomId = roomId;
        rooms_[roomId] = s;
    }
    return rooms_[roomId];
}

void progressive::RoomStateManager::setHistoryVisibility(const std::string& roomId, RSM_RoomHistoryVisibility visibility) {
    auto& state = getOrCreateState(roomId);
    state.historyVisibility = visibility;
    state.isWorldReadable = (visibility == RSM_RoomHistoryVisibility::WORLD_READABLE);
    state.canShareHistory = shouldShareHistory(visibility);
}

void progressive::RoomStateManager::setJoinRule(const std::string& roomId, RoomJoinRule rule) {
    auto& state = getOrCreateState(roomId);
    state.joinRules = rule;
    state.isPublicRoom = (rule == RoomJoinRule::PUBLIC);
}

void progressive::RoomStateManager::setRoomName(const std::string& roomId, const std::string& name) {
    getOrCreateState(roomId).roomName = name;
}

void progressive::RoomStateManager::setEncrypted(const std::string& roomId, bool encrypted) {
    getOrCreateState(roomId).isEncrypted = encrypted;
}

void progressive::RoomStateManager::setMemberCount(const std::string& roomId, int count) {
    getOrCreateState(roomId).memberCount = count;
}

progressive::progressive::RSM_RoomStateSummary progressive::RoomStateManager::getRoomState(const std::string& roomId) const {
    auto it = rooms_.find(roomId);
    if (it != rooms_.end()) return it->second;
    progressive::progressive::RSM_RoomStateSummary s;
    s.roomId = roomId;
    return s;
}

bool progressive::RoomStateManager::canShareRoomHistory(const std::string& roomId) const {
    auto state = getRoomState(roomId);
    return state.canShareHistory;
}

bool progressive::RoomStateManager::isPublicRoom(const std::string& roomId) const {
    return getRoomState(roomId).isPublicRoom;
}

bool progressive::RoomStateManager::isWorldReadable(const std::string& roomId) const {
    return getRoomState(roomId).isWorldReadable;
}

bool progressive::RoomStateManager::isInviteOnly(const std::string& roomId) const {
    return getRoomState(roomId).joinRules == RoomJoinRule::INVITE;
}

bool progressive::RoomStateManager::areGuestsAllowed(const std::string& roomId) const {
    auto state = getRoomState(roomId);
    return state.isPublicRoom && state.isWorldReadable;
}

void progressive::RoomStateManager::clear() { rooms_.clear(); }

// ====== Serialization ======

std::string progressive::RoomStateManager::roomStateToJson(const progressive::progressive::RSM_RoomStateSummary& state) const {
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
       << R"(","join_rule":")" << roomJoinRulesToString(state.joinRules)
       << R"(,"is_public":)" << (state.isPublicRoom ? "true" : "false")
       << R"(,"is_world_readable":)" << (state.isWorldReadable ? "true" : "false")
       << R"(,"can_share_history":)" << (state.canShareHistory ? "true" : "false")
       << R"(,"is_invite_only":)" << (roomJoinRulesToString(state.joinRules) == std::string("invite") ? "true" : "false")
       << R"(,"is_encrypted":)" << (state.isEncrypted ? "true" : "false")
       << R"(,"members":)" << state.memberCount
       << "}";
    return os.str();
}

} // namespace progressive
