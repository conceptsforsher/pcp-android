#include <nlohmann/json.hpp>
#ifndef PCP_ROOM_VERSION_HPP
#define PCP_ROOM_VERSION_HPP

#include <string>
#include <vector>

namespace pcp {

struct RoomVersion {
    std::string id;          // e.g. "1", "9", "10"
    std::string description; // e.g. "v1 - Stable", "v10 - MSC... "
    bool isDefault = false;
    bool isStable = false;
};

// Get list of known Matrix room versions.
std::vector<RoomVersion> getKnownRoomVersions();

// Check if a room version string is valid.
bool isValidRoomVersion(const std::string& version);

// Get the default recommended room version.
std::string getDefaultRoomVersion();

// Format room versions as JSON for settings UI.
std::string roomVersionsToJson();

} // namespace pcp

#endif // PCP_ROOM_VERSION_HPP
