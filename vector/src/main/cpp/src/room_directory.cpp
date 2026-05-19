#include "progressive/room_directory.hpp"
#include <sstream>
#include <algorithm>
#include <unordered_set>

namespace progressive {

std::vector<RoomDirectoryEntry> filterDirectory(
    const std::vector<RoomDirectoryEntry>& rooms,
    const DirectoryFilter& filter,
    int maxResults
) {
    std::vector<RoomDirectoryEntry> result;

    for (const auto& room : rooms) {
        // Server filter
        if (!filter.serverFilter.empty()) {
            auto server = room.roomId;
            auto colon = server.find(':');
            if (colon != std::string::npos) server = server.substr(colon + 1);
            if (server != filter.serverFilter) continue;
        }

        // Name filter
        if (!filter.nameFilter.empty()) {
            auto lower = room.name;
            auto lowerFilter = filter.nameFilter;
            std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
            std::transform(lowerFilter.begin(), lowerFilter.end(), lowerFilter.begin(), ::tolower);
            if (lower.find(lowerFilter) == std::string::npos) continue;
        }

        // Member count filter
        if (room.memberCount < filter.minMembers) continue;
        if (room.memberCount > filter.maxMembers) continue;

        // Public/private
        if (filter.showOnlyPublic && !room.isPublic) continue;

        // Already joined
        if (filter.showOnlyUnjoined && room.isJoined) continue;

        result.push_back(room);
    }

    if (static_cast<int>(result.size()) > maxResults) {
        result.resize(maxResults);
    }

    return result;
}

DirectoryStats computeDirectoryStats(const std::vector<RoomDirectoryEntry>& rooms) {
    DirectoryStats stats;
    stats.totalRooms = static_cast<int>(rooms.size());
    stats.filteredRooms = stats.totalRooms;

    for (const auto& room : rooms) {
        stats.totalMembers += room.memberCount;
        if (room.memberCount > stats.biggestRoomMembers) {
            stats.biggestRoomMembers = room.memberCount;
            stats.biggestRoom = room.name;
        }
    }

    stats.availableServers = extractServers(rooms);
    return stats;
}

std::vector<RoomDirectoryEntry> searchRooms(
    const std::vector<RoomDirectoryEntry>& rooms,
    const std::string& query, int maxResults
) {
    if (query.empty()) return {};

    std::vector<std::pair<RoomDirectoryEntry, double>> scored;

    auto lowerQuery = query;
    std::transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);

    for (auto room : rooms) {
        auto lowerName = room.name;
        std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);

        double score = 0.0;
        if (lowerName == lowerQuery) score = 1.0;
        else if (lowerName.rfind(lowerQuery, 0) == 0) score = 0.8;
        else if (lowerName.find(lowerQuery) != std::string::npos) score = 0.5;
        else if (room.topic.find(query) != std::string::npos) score = 0.3;

        if (score > 0.0) {
            room.relevance = score;
            scored.push_back({room, score});
        }
    }

    std::sort(scored.begin(), scored.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    std::vector<RoomDirectoryEntry> result;
    int limit = std::min(maxResults, static_cast<int>(scored.size()));
    for (int i = 0; i < limit; ++i) {
        result.push_back(scored[i].first);
    }

    return result;
}

void sortRooms(std::vector<RoomDirectoryEntry>& rooms, const std::string& sortBy) {
    if (sortBy == "members") {
        std::sort(rooms.begin(), rooms.end(), [](const auto& a, const auto& b) {
            return a.memberCount > b.memberCount;
        });
    } else if (sortBy == "relevance") {
        std::sort(rooms.begin(), rooms.end(), [](const auto& a, const auto& b) {
            return a.relevance > b.relevance;
        });
    } else { // name (default)
        std::sort(rooms.begin(), rooms.end(), [](const auto& a, const auto& b) {
            return a.name < b.name;
        });
    }
}

std::string formatDirectoryEntry(const RoomDirectoryEntry& entry) {
    std::ostringstream out;
    out << entry.name << " (" << entry.memberCount << " members)\n";
    if (!entry.topic.empty()) out << "  " << entry.topic << "\n";
    return out.str();
}

std::vector<std::string> extractServers(const std::vector<RoomDirectoryEntry>& rooms) {
    std::unordered_set<std::string> servers;
    for (const auto& room : rooms) {
        auto server = room.roomId;
        auto colon = server.find(':');
        if (colon != std::string::npos) {
            servers.insert(server.substr(colon + 1));
        }
    }
    return std::vector<std::string>(servers.begin(), servers.end());
}

std::string directoryStatsToJson(const DirectoryStats& stats) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << "{";
    json << R"("totalRooms": )" << stats.totalRooms << ",";
    json << R"("filteredRooms": )" << stats.filteredRooms << ",";
    json << R"("totalMembers": )" << stats.totalMembers << ",";
    json << R"("biggestRoom": ")" << esc(stats.biggestRoom) << R"(",)";
    json << R"("servers": [)";
    for (size_t i = 0; i < stats.availableServers.size(); ++i) {
        if (i > 0) json << ",";
        json << R"(")" << esc(stats.availableServers[i]) << R"(")";
    }
    json << "]}";
    return json.str();
}

} // namespace progressive
