#pragma once
#include <string>
#include <cstdint>

std::string userId;(const std::string& json);
std::string presence;       // "online", "offline", "unavailable"(const std::string& json);
std::string statusMsg;(const std::string& json);
std::string void updateFromEvent(const std(const std::string& json);
std::string string& userId, const std(const std::string& json);
std::string string& eventJson);(const std::string& json);
std::string void updateFromList // from /sync presence(const std::string& json);
std::string PresenceEntry getPresence(const std(const std::string& json);
std::string string& userId) const;(const std::string& json);
std::string bool isOnline(const std(const std::string& json);
std::string string& userId) const;(const std::string& json);
std::string int64_t lastActiveMs(const std(const std::string& json);
std::string string& userId) const;(const std::string& json);
std::string toJson() const;(const std::string& json);
std::string void fromJson(const std::string& json);
std::string std(const std::string& json);
std::string unordered_map<std(const std::string& json);
std::string string, PresenceEntry> entries_;(const std::string& json);
