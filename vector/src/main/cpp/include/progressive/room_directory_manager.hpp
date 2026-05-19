#include "progressive/room_content.hpp"
#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>

namespace progressive {

// ================================================================
// Room Discovery / Directory Search
//
// Faithful port from Element Android original sources:
//   PublicRoom.kt — room data model (9 fields + getPrimaryAlias)
//   PublicRoomsFilter.kt — generic_search_term
//   PublicRoomsParams.kt — limit, since, filter, include_all_networks
//   PublicRoomsResponse.kt — chunk, next_batch, prev_batch, total_estimate
//   RoomDirectoryService.kt — getPublicRooms, visibility, alias check
//   RoomDirectoryVisibility.kt — PRIVATE / PUBLIC
//
// Matrix API:
//   POST /publicRooms — search public rooms
//   GET /directory/list/room/{roomId} — visibility
//   PUT /directory/list/room/{roomId} — set visibility
// ================================================================

// ---- Room Directory Visibility ----
// Original: RoomDirectoryVisibility.kt (PRIVATE, PUBLIC)


const char* visibilityToString(RoomDirectoryVisibility v);
RoomDirectoryVisibility visibilityFromString(const std::string& s);

// ---- Public Room ----
// Original: PublicRoom.kt (9 fields, getPrimaryAlias())


// ---- Public Rooms Filter ----
// Original: PublicRoomsFilter.kt (generic_search_term)


// ---- Public Rooms Params ----
// Original: PublicRoomsParams.kt (limit, since, filter, include_all_networks, third_party_instance_id)


// ---- Public Rooms Response ----
// Original: PublicRoomsResponse.kt (chunk, next_batch, prev_batch, total_room_count_estimate)


// ---- Alias Availability ----
// Original: AliasAvailabilityResult (checkAliasAvailability)


// ---- Alias Availability ----
// Original: AliasAvailabilityResult (checkAliasAvailability)

struct AliasAvailabilityResult {
    bool available = false;
    std::string alias;
    std::string error;
};

// ---- Room Directory Manager ----

class RoomDirectoryManager {
public:
    RoomDirectoryManager();

    // ====== Public Rooms Search ======

    // Build the POST /publicRooms request body.
    // Original: getPublicRooms(server, PublicRoomsParams) → POST /publicRooms
    std::string buildPublicRoomsRequest(const PublicRoomsParams& params) const;

    // Parse POST /publicRooms response.
    // Original: PublicRoomsResponse.chunk
    PublicRoomsResponse parsePublicRoomsResponse(const std::string& json) const;

    // Accumulate paginated results (merge multiple pages).
    void accumulateResults(PublicRoomsResponse& existing, const PublicRoomsResponse& nextPage) const;

    // ====== Room Directory Visibility ======

    // Build visibility request body.
    // Original: setRoomDirectoryVisibility(roomId, visibility)
    std::string buildVisibilityRequest(RoomDirectoryVisibility visibility) const;

    // Parse visibility response.
    // Original: getRoomDirectoryVisibility(roomId) → GET /directory/list/room/{roomId}
    RoomDirectoryVisibility parseVisibilityResponse(const std::string& json) const;

    // ====== Alias Check ======

    // Build alias availability check request.
    // Original: checkAliasAvailability(aliasLocalPart)
    std::string buildAliasCheckRequest(const std::string& aliasLocalPart) const;

    // Parse alias availability response.
    AliasAvailabilityResult parseAliasAvailability(const std::string& json, const std::string& aliasLocalPart) const;

    // ====== Room Preview ======

    // Format a public room for display.
    std::string formatRoomPreview(const PublicRoom& room) const;

    // Build room join URL.
    std::string buildRoomJoinUrl(const std::string& roomId, const std::string& viaServer) const;

    // Build room preview map URL (for avatar).
    std::string buildRoomAvatarUrl(const std::string& avatarUrl, const std::string& homeServer) const;

    // ====== Filtering & Sorting ======

    // Filter rooms by search term (client-side filter on top of server filter).
    std::vector<PublicRoom> filterRooms(const std::vector<PublicRoom>& rooms, const std::string& query) const;

    // Sort rooms by member count (descending), then by name (ascending).
    void sortRoomsByPopularity(std::vector<PublicRoom>& rooms) const;

    // Sort rooms by name (alphabetical).
    void sortRoomsByName(std::vector<PublicRoom>& rooms) const;

    // ====== Serialization ======

    // Export a single room as JSON.
    std::string roomToJson(const PublicRoom& room) const;

    // Export room list as JSON array.
    std::string roomsToJson(const std::vector<PublicRoom>& rooms) const;

    // Export response as JSON.
    std::string responseToJson(const PublicRoomsResponse& resp) const;

    // Export alias result as JSON.
    std::string aliasResultToJson(const AliasAvailabilityResult& result) const;

private:
    static std::string extractStr(const std::string& json, const std::string& key);
    static int64_t extractInt(const std::string& json, const std::string& key);
    static bool extractBool(const std::string& json, const std::string& key);
};

} // namespace progressive
