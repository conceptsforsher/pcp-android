#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// ================================================================
// Room Directory Utilities — filter, search, sort room entries
//
// Ported from Element Android:
//   RoomDirectoryService.kt, DirectoryAPI.kt
// ================================================================

// ---- Room Directory Entry (legacy) ----

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

// ================================================================
// Public Rooms — query the Matrix public room directory
//
// Ported from Element Android:
//   PublicRoom.kt, PublicRoomsResponse.kt, PublicRoomsFilter.kt,
//   PublicRoomsParams.kt, RoomAPI.kt
//
// Matrix API: POST /_matrix/client/r0/publicRooms
//   Body: {"limit": 10, "since": "token", "filter": {...},
//          "include_all_networks": false, "third_party_instance_id": null}
//   Response: {"chunk": [...], "next_batch": "...", "prev_batch": "...",
//              "total_room_count_estimate": 42}
// ================================================================

// ---- Public Room ----

// Original Kotlin: PublicRoom.kt data class
struct PublicRoom {
    std::string roomId;                       // Required: room ID
    std::string name;                         // Room name
    std::string topic;                        // Room topic
    int numJoinedMembers = 0;                 // Required: join count
    bool worldReadable = false;               // Required: visible to guests
    bool guestCanJoin = false;                // Required: guests can join
    std::string avatarUrl;                    // Room avatar URL
    std::vector<std::string> aliases;         // Room aliases
    std::string canonicalAlias;               // Canonical alias
    bool isFederated = true;                  // m.federate

    // Return the canonical alias or first alias.
    // Original Kotlin: PublicRoom.getPrimaryAlias()
    std::string getPrimaryAlias() const {
        if (!canonicalAlias.empty()) return canonicalAlias;
        if (!aliases.empty()) return aliases.front();
        return "";
    }
};

// ---- Public Rooms Response ----

// Original Kotlin: PublicRoomsResponse.kt data class
struct PublicRoomsResponse {
    std::vector<PublicRoom> chunk;            // Paginated chunk of rooms
    int totalRoomCount = 0;                   // Estimated total rooms
    std::string nextBatch;                    // Pagination token for next
    std::string prevBatch;                    // Pagination token for previous
    std::string error;
};

// ---- Room Directory Visibility ----

// Original Kotlin: RoomDirectoryVisibility.kt enum
enum class RoomDirectoryVisibilityState {
    PRIVATE,
    PUBLIC
};

// ================================================================
// Public Rooms API functions
// ================================================================

// Build JSON for the filter part of a /publicRooms request.
// Original Kotlin: PublicRoomsFilter.kt
//   {"generic_search_term": "search term"}
std::string buildPublicRoomsFilter(const std::string& searchTerm);

// Build the full JSON body for POST /publicRooms.
// Original Kotlin: PublicRoomsParams.kt
//   limit: max results to return (use 0 for none)
//   since: pagination token from previous response
//   searchTerm: filter by search term (empty = no filter)
//   includeAllNetworks: include all third-party networks
//   thirdPartyInstanceId: specific third-party network
std::string buildPublicRoomsRequest(
    int limit = 50,
    const std::string& since = "",
    const std::string& searchTerm = "",
    bool includeAllNetworks = false,
    const std::string& thirdPartyInstanceId = ""
);

// Parse a /publicRooms response JSON string.
// Original Kotlin: PublicRoomsResponse.kt
PublicRoomsResponse parsePublicRoomsResponse(const std::string& json);

// ================================================================
// Room Directory Visibility API functions
// ================================================================

// Build JSON body for PUT /directory/list/room/{roomId}.
// Original Kotlin: RoomDirectoryVisibilityJson.kt
//   Body: {"visibility": "public"}
std::string buildRoomVisibilityRequest(RoomDirectoryVisibilityState visibility);

// Parse response from GET /directory/list/room/{roomId}.
// Original Kotlin: DirectoryAPI.getRoomDirectoryVisibility()
//   Response: {"visibility": "public"}
RoomDirectoryVisibilityState parseRoomVisibilityResponse(const std::string& json);

// ================================================================
// Expanded Room Directory — servers, pagination, networks
//
// Original Kotlin: RoomDirectoryServer.kt, RoomDirectoryPagination.kt,
//   RoomNetworkInfo.kt
// ================================================================

// ---- Room Directory Server ----
// Original Kotlin: RoomDirectoryServer model

struct RoomDirectoryServer {
    std::string serverName;
    bool isAvailable = false;
};

// ---- Public Room Filter (structured) ----
// Original Kotlin: PublicRoomsFilter structured request

struct PublicRoomFilter {
    std::string searchTerm;
    std::string server;              // specific homeserver to query
    int limit = 50;
    std::string since;               // pagination token
    bool includeAllNetworks = false;
    std::string thirdPartyInstanceId;
};

// ---- Room Directory Result ----
// Original Kotlin: RoomDirectoryResult wrapper

struct RoomDirectoryResult {
    std::vector<PublicRoom> rooms;
    int totalCount = 0;
    std::string nextBatch;
    std::string prevBatch;
    std::string server;              // server queried
};

// ---- Room Directory Pagination ----
// Original Kotlin: RoomDirectoryPagination model

struct RoomDirectoryPagination {
    bool hasMore = false;
    std::string nextBatch;
    std::string prevBatch;
    int totalAvailable = 0;
};

// ---- Room Network Info ----
// Original Kotlin: RoomNetworkInfo — third-party network

struct RoomNetworkInfo {
    std::string networkId;
    std::string networkName;
    int roomCount = 0;
    bool isFederated = true;
};

// ================================================================
// Expanded Room Directory Functions
// ================================================================

// Build the full JSON body for POST /publicRooms using a structured filter.
// Original Kotlin: PublicRoomsParams with full PublicRoomFilter fields
std::string buildPublicRoomsRequest(const PublicRoomFilter& filter);

// Parse a /publicRooms response into a structured result.
// Original Kotlin: parse full RoomDirectoryResult from response
RoomDirectoryResult parseRoomDirectoryResult(const std::string& json);

// Get pagination info from a response.
// Original Kotlin: extract pagination fields from response
RoomDirectoryPagination parseRoomDirectoryPagination(const std::string& json);

// Compute the filter for the next page of results.
// Original Kotlin: get next page request filter
std::string getNextPage(const std::string& nextBatch, int limit);

// Compute the filter for the previous page of results.
// Original Kotlin: get previous page request filter
std::string getPreviousPage(const std::string& prevBatch, int limit);

// Get available third-party room networks.
// Original Kotlin: parse third-party network list response
std::vector<RoomNetworkInfo> getRoomNetworks(const std::string& json);

// Build JSON body for a third-party protocol room request.
// Original Kotlin: POST /_matrix/client/r0/thirdparty/protocol/{protocol}
std::string buildThirdPartyRoomRequest(const std::string& networkId, const std::string& searchTerm, int limit = 50);

} // namespace progressive
