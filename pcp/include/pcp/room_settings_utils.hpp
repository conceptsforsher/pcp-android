#include <nlohmann/json.hpp>
#pragma once
#include <string>
#include <cstdint>

namespace pcp {

struct RoomSettings {
    std::string roomId;
    std::string name;
    std::string topic;
    std::string avatarUrl;
    std::string joinRule;           // "public", "invite", "knock", "private", "restricted"
    std::string historyVisibility;  // "world_readable", "shared", "invited", "joined"
    std::string guestAccess;        // "can_join", "forbidden"
    bool isEncrypted = false;
    bool isFederated = true;
    int roomVersion = 10;
};

// Parse room settings from /state response
RoomSettings parseRoomSettings(const std::string& stateJson);

// Build room settings update body
std::string buildRoomSettingsUpdate(const std::string& field, const std::string& value);

// Format room settings for display
std::string formatRoomSettings(const RoomSettings& settings);

// Get room setting labels
std::string getJoinRuleLabel(const std::string& rule);
std::string getHistoryVisibilityLabel(const std::string& vis);
std::string getGuestAccessLabel(const std::string& access);

// Validate settings values
bool isValidJoinRule(const std::string& rule);
bool isValidHistoryVisibility(const std::string& vis);

} // namespace pcp
