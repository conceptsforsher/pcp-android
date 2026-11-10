#include <nlohmann/json.hpp>
#pragma once
#include <string>

namespace pcp {

// Build m.room.name state event
std::string buildRoomNameEvent(const std::string& name);

// Build m.room.topic event
std::string buildRoomTopicEvent(const std::string& topic);

// Build m.room.guest_access event
std::string buildGuestAccessEvent(const std::string& access);

// Build m.room.history_visibility event
std::string buildHistoryVisibilityEvent(const std::string& visibility);

// Build m.room.join_rules event
std::string buildJoinRulesEvent(const std::string& rule);

} // namespace pcp
