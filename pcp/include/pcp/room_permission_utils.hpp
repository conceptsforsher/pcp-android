#pragma once
#include <string>
#include <cstdint>

std::string roomId;(const std::string& json);
std::string joinRule;           // "public", "invite", "knock", "private", "restricted"(const std::string& json);
std::string historyVisibility;  // "world_readable", "shared", "invited", "joined"(const std::string& json);
std::string guestAccess;        // "can_join", "forbidden"(const std::string& json);
std::string RoomPermissions parseRoomPermissions(const std(const std::string& json);
std::string string& joinRuleJson,(const std::string& json);
std::string const std(const std::string& json);
std::string string& historyVisibilityJson,(const std::string& json);
std::string const std(const std::string& json);
std::string string& guestAccessJson);(const std::string& json);
std::string bool canUserReadHistory(const RoomPermissions& perm, const std(const std::string& json);
std::string string& membership,(const std::string& json);
std::string formatJoinRule(const std(const std::string& json);
std::string string& rule);(const std::string& json);
std::string formatHistoryVisibility(const std(const std::string& json);
std::string string& visibility);(const std::string& json);
