#pragma once
#include <string>
#include <vector>
#include <cstdint>

namespace pcp {

struct RoomPreview {
    std::string roomId;
    std::string name;
    std::string topic;
    std::string avatarUrl;
    std::string alias;
    int memberCount = 0;
    int roomVersion = 0;
    bool isEncrypted = false;
    bool isPublic = false;
    bool isSpace = false;
    bool isDirect = false;
    bool canJoin = false;
    std::string joinRule;        // "public", "invite", "knock"
    std::string guestAccess;     // "can_join", "forbidden"
    std::vector<std::string> viaServers;  // recommended servers for joining
};

// Parse room preview from /publicRooms or /room/{id} API
RoomPreview parseRoomPreview(const std::string& json, const std::string& roomId = "");

// Parse room summary for preview (from sync response)
RoomPreview parseRoomSummary(const std::string& roomId, const std::string& summaryJson);

// Format room preview as display card text
std::string formatRoomPreviewCard(const RoomPreview& preview);

// Build preview JSON for UI rendering
std::string roomPreviewToJson(const RoomPreview& preview);

} // namespace pcp
