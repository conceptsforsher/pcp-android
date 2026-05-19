#include "progressive/room_directory.hpp"
#include <sstream>
#include <algorithm>
#include <unordered_set>
#include <cctype>

namespace progressive {

// ====== JSON helpers ======

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

static bool extractBool(const std::string& json, const std::string& key) {
    return json.find("\"" + key + "\":true") != std::string::npos;
}

static int extractIntVal(const std::string& json, const std::string& key, int def = 0) {
    auto pp = json.find("\"" + key + "\"");
    if (pp == std::string::npos) return def;
    pp = json.find(':', pp + key.size() + 2);
    if (pp == std::string::npos) return def;
    pp++;
    while (pp < json.size() && (json[pp] == ' ' || json[pp] == '\n')) pp++;
    std::string num;
    while (pp < json.size() && (std::isdigit(json[pp]) || json[pp] == '-')) {
        num += json[pp];
        pp++;
    }
    if (num.empty()) return def;
    return std::stoi(num);
}

static std::string escapeJson(const std::string& s) {
    std::string out;
    for (char c : s) {
        if (c == '"' || c == '\\') out += '\\';
        out += c;
    }
    return out;
}

// ================================================================
// Legacy Room Directory Functions
// ================================================================

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
    std::ostringstream json;
    json << "{";
    json << R"("totalRooms": )" << stats.totalRooms << ",";
    json << R"("filteredRooms": )" << stats.filteredRooms << ",";
    json << R"("totalMembers": )" << stats.totalMembers << ",";
    json << R"("biggestRoom": ")" << escapeJson(stats.biggestRoom) << R"(",)";
    json << R"("servers": [)";
    for (size_t i = 0; i < stats.availableServers.size(); ++i) {
        if (i > 0) json << ",";
        json << R"(")" << escapeJson(stats.availableServers[i]) << R"(")";
    }
    json << "]}";
    return json.str();
}

// ================================================================
// Public Rooms API Functions
// ================================================================

// Original Kotlin: PublicRoomsFilter.kt
//   {"generic_search_term": "search term"}
std::string buildPublicRoomsFilter(const std::string& searchTerm) {
    std::ostringstream os;
    os << R"({"generic_search_term":")" << escapeJson(searchTerm) << R"("})";
    return os.str();
}

// Original Kotlin: PublicRoomsParams.kt (simple overload)
//   {"limit": 10, "since": "token", "filter": {...}, "include_all_networks": false}
std::string buildPublicRoomsRequest(
    int limit,
    const std::string& since,
    const std::string& searchTerm,
    bool includeAllNetworks,
    const std::string& thirdPartyInstanceId
) {
    std::ostringstream os;
    os << "{";

    if (limit > 0) {
        os << R"("limit":)" << limit << ",";
    }

    if (!since.empty()) {
        os << R"("since":")" << escapeJson(since) << R"(",)";
    }

    if (!searchTerm.empty()) {
        os << R"("filter":)" << buildPublicRoomsFilter(searchTerm) << ",";
    }

    os << R"("include_all_networks":)" << (includeAllNetworks ? "true" : "false");

    if (!thirdPartyInstanceId.empty()) {
        os << R"(,"third_party_instance_id":")" << escapeJson(thirdPartyInstanceId) << R"(")";
    }

    os << "}";
    return os.str();
}

// Original Kotlin: PublicRoomsResponse.kt
PublicRoomsResponse parsePublicRoomsResponse(const std::string& json) {
    PublicRoomsResponse resp;

    // Check for error
    auto err = extractStr(json, "errcode");
    if (!err.empty()) {
        resp.error = err + ": " + extractStr(json, "error");
        return resp;
    }

    resp.nextBatch = extractStr(json, "next_batch");
    resp.prevBatch = extractStr(json, "prev_batch");
    resp.totalRoomCount = extractIntVal(json, "total_room_count_estimate");

    // Parse chunk array
    size_t pos = json.find("\"chunk\"");
    if (pos == std::string::npos) return resp;

    pos = json.find('[', pos);
    if (pos == std::string::npos) return resp;

    pos++;
    while (pos < json.size()) {
        while (pos < json.size() && (json[pos] == ' ' || json[pos] == ',' || json[pos] == '\n')) pos++;
        if (pos >= json.size() || json[pos] == ']') break;

        size_t objStart = pos;
        int depth = 0;
        while (pos < json.size()) {
            if (json[pos] == '{') depth++;
            else if (json[pos] == '}') depth--;
            if (depth == 0 && json[pos] == '}') { pos++; break; }
            pos++;
        }
        std::string roomJson = json.substr(objStart, pos - objStart);

        // Original Kotlin: PublicRoom.kt
        PublicRoom room;
        room.roomId = extractStr(roomJson, "room_id");
        room.name = extractStr(roomJson, "name");
        room.topic = extractStr(roomJson, "topic");
        room.numJoinedMembers = extractIntVal(roomJson, "num_joined_members");
        room.worldReadable = extractBool(roomJson, "world_readable");
        room.guestCanJoin = extractBool(roomJson, "guest_can_join");
        room.avatarUrl = extractStr(roomJson, "avatar_url");
        room.canonicalAlias = extractStr(roomJson, "canonical_alias");
        room.isFederated = !extractBool(roomJson, "m.federate") ? true :
            (roomJson.find("\"m.federate\":false") == std::string::npos);

        // Parse aliases array
        size_t aliasesPos = roomJson.find("\"aliases\"");
        if (aliasesPos != std::string::npos) {
            aliasesPos = roomJson.find('[', aliasesPos);
            if (aliasesPos != std::string::npos) {
                aliasesPos++;
                while (aliasesPos < roomJson.size()) {
                    while (aliasesPos < roomJson.size() && (roomJson[aliasesPos] == ' ' || roomJson[aliasesPos] == ',' || roomJson[aliasesPos] == '\n')) aliasesPos++;
                    if (aliasesPos >= roomJson.size() || roomJson[aliasesPos] == ']') break;
                    if (roomJson[aliasesPos] == '"') {
                        aliasesPos++;
                        size_t aliasEnd = aliasesPos;
                        while (aliasEnd < roomJson.size() && roomJson[aliasEnd] != '"') aliasEnd++;
                        room.aliases.push_back(roomJson.substr(aliasesPos, aliasEnd - aliasesPos));
                        aliasesPos = aliasEnd + 1;
                    } else {
                        aliasesPos++;
                    }
                }
            }
        }

        if (!room.roomId.empty()) resp.chunk.push_back(room);
    }

    return resp;
}

// ================================================================
// Room Directory Visibility API Functions
// ================================================================

// Original Kotlin: RoomDirectoryVisibilityJson.kt
//   Body: {"visibility": "public"} or {"visibility": "private"}
std::string buildRoomVisibilityRequest(RoomDirectoryVisibilityState visibility) {
    std::ostringstream os;
    os << R"({"visibility":")";
    // Original Kotlin: RoomDirectoryVisibility enum @Json names
    if (visibility == RoomDirectoryVisibilityState::PUBLIC) {
        os << "public";
    } else {
        os << "private";
    }
    os << R"("})";
    return os.str();
}

// Original Kotlin: DirectoryAPI.getRoomDirectoryVisibility() response
//   Response: {"visibility": "public"} or {"visibility": "private"}
RoomDirectoryVisibilityState parseRoomVisibilityResponse(const std::string& json) {
    auto visibility = extractStr(json, "visibility");
    // Original Kotlin: RoomDirectoryVisibility enum values
    if (visibility == "public") {
        return RoomDirectoryVisibilityState::PUBLIC;
    }
    return RoomDirectoryVisibilityState::PRIVATE;
}

// ================================================================
// Expanded Room Directory — structured filter, result, pagination, networks
// ================================================================

// Original Kotlin: PublicRoomsParams with full PublicRoomFilter fields
std::string buildPublicRoomsRequest(const PublicRoomFilter& filter) {
    std::ostringstream os;
    os << "{";

    if (filter.limit > 0) {
        os << R"("limit":)" << filter.limit << ",";
    }

    if (!filter.since.empty()) {
        os << R"("since":")" << escapeJson(filter.since) << R"(",)";
    }

    if (!filter.searchTerm.empty()) {
        os << R"("filter":)" << buildPublicRoomsFilter(filter.searchTerm) << ",";
    }

    if (!filter.server.empty()) {
        os << R"("server":")" << escapeJson(filter.server) << R"(",)";
    }

    if (!filter.thirdPartyInstanceId.empty()) {
        os << R"("third_party_instance_id":")" << escapeJson(filter.thirdPartyInstanceId) << R"(",)";
    }

    os << R"("include_all_networks":)" << (filter.includeAllNetworks ? "true" : "false");

    os << "}";
    return os.str();
}

// Original Kotlin: parse full RoomDirectoryResult from /publicRooms response
RoomDirectoryResult parseRoomDirectoryResult(const std::string& json) {
    RoomDirectoryResult result;

    // Check for error
    auto err = extractStr(json, "errcode");
    if (!err.empty()) {
        return result; // caller checks for empty rooms + error
    }

    result.nextBatch = extractStr(json, "next_batch");
    result.prevBatch = extractStr(json, "prev_batch");
    result.totalCount = extractIntVal(json, "total_room_count_estimate");
    result.server = extractStr(json, "server");

    // Parse chunk array
    size_t pos = json.find("\"chunk\"");
    if (pos == std::string::npos) return result;

    pos = json.find('[', pos);
    if (pos == std::string::npos) return result;

    pos++;
    while (pos < json.size()) {
        while (pos < json.size() && (json[pos] == ' ' || json[pos] == ',' || json[pos] == '\n')) pos++;
        if (pos >= json.size() || json[pos] == ']') break;

        size_t objStart = pos;
        int depth = 0;
        while (pos < json.size()) {
            if (json[pos] == '{') depth++;
            else if (json[pos] == '}') depth--;
            if (depth == 0 && json[pos] == '}') { pos++; break; }
            pos++;
        }
        std::string roomJson = json.substr(objStart, pos - objStart);

        PublicRoom room;
        room.roomId = extractStr(roomJson, "room_id");
        room.name = extractStr(roomJson, "name");
        room.topic = extractStr(roomJson, "topic");
        room.numJoinedMembers = extractIntVal(roomJson, "num_joined_members");
        room.worldReadable = extractBool(roomJson, "world_readable");
        room.guestCanJoin = extractBool(roomJson, "guest_can_join");
        room.avatarUrl = extractStr(roomJson, "avatar_url");
        room.canonicalAlias = extractStr(roomJson, "canonical_alias");
        room.isFederated = !extractBool(roomJson, "m.federate") ? true :
            (roomJson.find("\"m.federate\":false") == std::string::npos);

        // Parse aliases array
        size_t aliasesPos = roomJson.find("\"aliases\"");
        if (aliasesPos != std::string::npos) {
            aliasesPos = roomJson.find('[', aliasesPos);
            if (aliasesPos != std::string::npos) {
                aliasesPos++;
                while (aliasesPos < roomJson.size()) {
                    while (aliasesPos < roomJson.size() && (roomJson[aliasesPos] == ' ' || roomJson[aliasesPos] == ',' || roomJson[aliasesPos] == '\n')) aliasesPos++;
                    if (aliasesPos >= roomJson.size() || roomJson[aliasesPos] == ']') break;
                    if (roomJson[aliasesPos] == '"') {
                        aliasesPos++;
                        size_t aliasEnd = aliasesPos;
                        while (aliasEnd < roomJson.size() && roomJson[aliasEnd] != '"') aliasEnd++;
                        room.aliases.push_back(roomJson.substr(aliasesPos, aliasEnd - aliasesPos));
                        aliasesPos = aliasEnd + 1;
                    } else {
                        aliasesPos++;
                    }
                }
            }
        }

        if (!room.roomId.empty()) result.rooms.push_back(room);
    }

    return result;
}

// Original Kotlin: extract pagination fields from response
RoomDirectoryPagination parseRoomDirectoryPagination(const std::string& json) {
    RoomDirectoryPagination pagination;
    pagination.nextBatch = extractStr(json, "next_batch");
    pagination.prevBatch = extractStr(json, "prev_batch");
    pagination.hasMore = !pagination.nextBatch.empty();
    pagination.totalAvailable = extractIntVal(json, "total_room_count_estimate");
    return pagination;
}

// Original Kotlin: get next page request filter JSON
std::string getNextPage(const std::string& nextBatch, int limit) {
    if (nextBatch.empty()) return "";
    std::ostringstream os;
    os << R"({"since":")" << escapeJson(nextBatch) << R"(",)";
    if (limit > 0) os << R"("limit":)" << limit << ",";
    os << R"("include_all_networks":false})";
    return os.str();
}

// Original Kotlin: get previous page request filter JSON
std::string getPreviousPage(const std::string& prevBatch, int limit) {
    if (prevBatch.empty()) return "";
    std::ostringstream os;
    os << R"({"since":")" << escapeJson(prevBatch) << R"(",)";
    if (limit > 0) os << R"("limit":)" << limit << ",";
    os << R"("include_all_networks":false})";
    return os.str();
}

// Original Kotlin: parse third-party network list response
// Response: [{"id": "irc", "display_name": "IRC", "room_count": 42, ...}]
std::vector<RoomNetworkInfo> getRoomNetworks(const std::string& json) {
    std::vector<RoomNetworkInfo> networks;

    size_t pos = 0;
    // Find the first array
    pos = json.find('[');
    if (pos == std::string::npos) return networks;

    pos++;
    while (pos < json.size()) {
        while (pos < json.size() && (json[pos] == ' ' || json[pos] == ',' || json[pos] == '\n')) pos++;
        if (pos >= json.size() || json[pos] == ']') break;

        size_t objStart = pos;
        int depth = 0;
        while (pos < json.size()) {
            if (json[pos] == '{') depth++;
            else if (json[pos] == '}') depth--;
            if (depth == 0 && json[pos] == '}') { pos++; break; }
            pos++;
        }
        std::string networkJson = json.substr(objStart, pos - objStart);

        RoomNetworkInfo network;
        network.networkId = extractStr(networkJson, "id");
        network.networkName = extractStr(networkJson, "display_name");
        network.roomCount = extractIntVal(networkJson, "room_count");
        network.isFederated = !extractBool(networkJson, "m.federate") ? true :
            (networkJson.find("\"m.federate\":false") == std::string::npos);

        if (!network.networkId.empty()) networks.push_back(network);
    }

    return networks;
}

// Original Kotlin: POST /thirdparty/protocol/{protocol} request body
// {"search_term": "...", "limit": 50, "fields": [...]}
std::string buildThirdPartyRoomRequest(const std::string& networkId, const std::string& searchTerm, int limit) {
    std::ostringstream os;
    os << "{";
    os << R"("protocol":")" << escapeJson(networkId) << R"(")";
    if (!searchTerm.empty()) {
        os << R"(,"search_term":")" << escapeJson(searchTerm) << R"(")";
    }
    if (limit > 0) {
        os << R"(,"limit":)" << limit;
    }
    os << "}";
    return os.str();
}

} // namespace progressive
