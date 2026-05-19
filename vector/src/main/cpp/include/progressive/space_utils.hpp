#ifndef PROGRESSIVE_SPACE_UTILS_HPP
#define PROGRESSIVE_SPACE_UTILS_HPP

#include <string>
#include <vector>

namespace progressive {

// ---- Matrix Spaces Utilities ----

// Space filter for room queries — from SpaceFilter.kt (53L)
enum class SpaceFilterKind { NoFilter, OrphanRooms, ActiveSpace, ExcludeSpace };

struct SpaceFilter {
    SpaceFilterKind kind = SpaceFilterKind::NoFilter;
    std::string spaceId;  // for ActiveSpace/ExcludeSpace
};

inline SpaceFilter spaceFilterNoFilter() { return {SpaceFilterKind::NoFilter}; }
inline SpaceFilter spaceFilterOrphanRooms() { return {SpaceFilterKind::OrphanRooms}; }
inline SpaceFilter spaceFilterActiveSpace(const std::string& id) { return {SpaceFilterKind::ActiveSpace, id}; }
inline SpaceFilter spaceFilterExcludeSpace(const std::string& id) { return {SpaceFilterKind::ExcludeSpace, id}; }

struct SpaceInfo {
    std::string spaceId;
    std::string name;
    std::string topic;
    std::string avatarUrl;
    bool isPublic = false;
    bool isJoined = true;
    bool isSuggested = false;
    int childRoomCount = 0;
    int childSpaceCount = 0;
    int totalMembers = 0;
};

struct SpaceChild {
    std::string childId;          // room or space ID
    std::string name;
    std::string topic;
    std::string avatarUrl;
    bool isRoom = true;           // false = sub-space
    bool isSuggested = false;
    bool isJoined = true;
    int memberCount = 0;
    bool isEncrypted = false;
    std::string order;            // sort order string
};

struct SpaceTree {
    std::string rootSpaceId;
    std::string rootSpaceName;
    std::vector<SpaceInfo> spaces;
    std::vector<SpaceChild> orphanRooms; // rooms not in any space
    int totalRooms = 0;
    int totalSpaces = 0;
};

// Parse space info from state events.
SpaceInfo parseSpaceInfo(const std::string& roomId, const std::string& stateEventsJson);

// Parse space children from m.space.child state events.
std::vector<SpaceChild> parseSpaceChildren(const std::string& stateEventsJson);

// Sort space children by order string or name.
void sortSpaceChildren(std::vector<SpaceChild>& children);

// Filter space children: rooms only, spaces only, suggested only.
std::vector<SpaceChild> filterSpaceChildren(const std::vector<SpaceChild>& children,
    bool roomsOnly = false, bool spacesOnly = false, bool suggestedOnly = false);

// Search space children by name.
std::vector<SpaceChild> searchSpaceChildren(const std::vector<SpaceChild>& children,
    const std::string& query);

// Format space tree for display.
std::string formatSpaceTree(const SpaceTree& tree);

// Format space info for display.
std::string formatSpaceInfo(const SpaceInfo& info);

// Build m.space.child state event content JSON.
std::string buildSpaceChildContent(bool suggested = false, const std::string& order = "",
    bool autoJoin = false, bool canonical = false);

// Build m.space.parent state event content JSON.
std::string buildSpaceParentContent(const std::string& parentSpaceId, bool canonical = false);

} // namespace progressive

#endif // PROGRESSIVE_SPACE_UTILS_HPP
