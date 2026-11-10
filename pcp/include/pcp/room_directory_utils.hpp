#pragma once
#include <string>
#include <cstdint>

std::string roomId;(const std::string& json);
std::string name;(const std::string& json);
std::string topic;(const std::string& json);
std::string alias;(const std::string& json);
std::string avatarUrl;(const std::string& json);
std::string RoomDirectoryEntry parseDirectoryEntry(const std::string& json);
std::string buildDirectoryFilter(const std(const std::string& json);
std::string string& searchTerm, int limit = 20,(const std::string& json);
std::string const std(const std::string& json);
std::string string& server = "");(const std::string& json);
std::string std(const std::string& json);
std::string vector<RoomDirectoryEntry> parseDirectoryList(const std::string& json);
std::string formatDirectoryEntry(const RoomDirectoryEntry& entry);(const std::string& json);
