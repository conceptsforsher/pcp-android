#ifndef PROGRESSIVE_ROOM_DIRECTORY_HPP
#define PROGRESSIVE_ROOM_DIRECTORY_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// ---- Room Directory Utilities ----

struct RoomDirectoryEntry {
    std::string roomId;
    std::string name;
    std::string topic;
    std::string alias;
    std::string avatarUrl;
    int memberCount = 0;
    bool isPublic = true;
    bool isJoined = false;
    double relevance = 0.0;     // search relevance 0-1
};

struct DirectoryFilter {
    std::string serverFilter;    // filter by server (empty = all)
    std::string nameFilter;      // search by name (empty = all)
    int minMembers = 0;          // minimum member count
    int maxMembers = INT32_MAX;  // maximum member count
    bool showOnlyPublic = true;  // hide private rooms
    bool showOnlyUnjoined = false; // hide already joined
};

struct DirectoryStats {
    int totalRooms = 0;
    int filteredRooms = 0;
    int64_t totalMembers = 0;
    std::string biggestRoom;
    int biggestRoomMembers = 0;
    std::vector<std::string> availableServers;
};

// Filter and rank room directory entries.
std::vector<RoomDirectoryEntry> filterDirectory(
    const std::vector<RoomDirectoryEntry>& rooms,
    const DirectoryFilter& filter,
    int maxResults = 50
);

// Compute directory statistics.
DirectoryStats computeDirectoryStats(const std::vector<RoomDirectoryEntry>& rooms);

// Search rooms by name (fuzzy match).
std::vector<RoomDirectoryEntry> searchRooms(
    const std::vector<RoomDirectoryEntry>& rooms,
    const std::string& query, int maxResults = 20
);

// Sort rooms: "name", "members", "relevance".
void sortRooms(std::vector<RoomDirectoryEntry>& rooms, const std::string& sortBy);

// Format directory entry for display.
std::string formatDirectoryEntry(const RoomDirectoryEntry& entry);

// Format directory stats as JSON.
std::string directoryStatsToJson(const DirectoryStats& stats);

// Extract unique servers from room list.
std::vector<std::string> extractServers(const std::vector<RoomDirectoryEntry>& rooms);

} // namespace progressive

#endif // PROGRESSIVE_ROOM_DIRECTORY_HPP
