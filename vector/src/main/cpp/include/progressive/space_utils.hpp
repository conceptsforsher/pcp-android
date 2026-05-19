#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include <cstdint>
#include "progressive/space_graph.hpp"

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

// ================================================================
// Space Summary Info — public-facing space metadata
// Original Kotlin: SpacePeekResult.kt (SpacePeekSummary), RoomSummary.kt
// ================================================================

struct SpaceSummaryInfo {
    std::string roomId;
    std::string name;
    std::string topic;
    std::string avatarUrl;
    std::string alias;
    int numJoinedMembers = 0;
    bool isPublic = false;
    bool isWorldReadable = false;
    std::string joinRule;           // "public", "invite", "knock", etc.
    std::string roomType;           // "m.space" or ""
};

// ================================================================
// Space Type
// Original Kotlin: CreateSpaceParams.kt preset logic
// ================================================================

enum class SpaceType {
    PUBLIC = 0,         // Published, world-readable, guests can join
    PRIVATE = 1,        // Private chat preset
    COMMUNITY = 2       // Not a Matrix space type — groups/communities
};

// ================================================================
// Space utility functions
// ================================================================

// Original Kotlin: roomType == RoomType.SPACE (CreateRoomParams.kt:27)
inline bool isSpaceRoom(const std::string& roomType) {
    return roomType == "m.space";
}

// Original Kotlin: DefaultSpaceService.createSpace preset logic
// Determines space type from join rules + visibility
inline SpaceType getSpaceType(const std::string& joinRule, bool isPublic, bool worldReadable) {
    if (joinRule == "public" || isPublic || worldReadable)
        return SpaceType::PUBLIC;
    return SpaceType::PRIVATE;
}

// ================================================================
// Space Room Filter — for filtering room lists by space membership
// Original Kotlin: SpaceFilter.kt, SpaceListViewModel.kt
// ================================================================

struct SpaceRoomFilter {
    enum FilterType {
        NONE = 0,           // No space filter — show all rooms
        ACTIVE_SPACE = 1,   // Only rooms in this specific space (via flattenParentIds)
        EXCLUDE_SPACE = 2,  // Exclude rooms in this space
        ORPHAN = 3          // Only rooms NOT in any space
    };
    FilterType type = NONE;
    std::string spaceId;    // Used for ACTIVE_SPACE and EXCLUDE_SPACE
};

// ================================================================
// Room filter helpers
// Original Kotlin: SpaceFilter.kt, SpaceListViewModel filtering
// ================================================================

// Filter a list of room IDs to only those belonging to a specific space.
// Uses flattenParentIds (transitive closure) on each room.
// Returns the filtered subset of roomIds.
std::vector<std::string> filterRoomsBySpace(
    const std::vector<std::string>& roomIds,
    const SpaceRoomFilter& filter,
    const std::unordered_map<std::string, std::vector<std::string>>& flattenParentIds);

// Original Kotlin: suggested=true child events (SpaceChildContent.suggested)
// Rooms with suggested=true child events in any space.
std::vector<std::string> getSuggestedRooms(
    const std::unordered_map<std::string, std::vector<SpaceChildWithOrder>>& spaceChildren);

// Original Kotlin: orphan rooms — rooms not referenced by any space's flattenParentIds
// Finds rooms that have no parent space (i.e., not in flattenParentIds of any space).
std::vector<std::string> getOrphanedRooms(
    const std::unordered_map<std::string, std::vector<std::string>>& allFlattenParentIds,
    const std::unordered_set<std::string>& spaceRoomIds);

// ================================================================
// Space child / parent state event builders — full events (not just content)
// Original Kotlin: Space.kt addChildren/removeChildren, DefaultSpaceService.kt setSpaceParent
// ================================================================

// Build m.space.child state event JSON including type, state_key, and content.
// stateKey = child room ID.
// Used when sending state events like PUT /_matrix/client/r0/rooms/{roomId}/state/m.space.child/{stateKey}
std::string buildSpaceChildEvent(const std::string& stateKey,
                                 bool suggested = true,
                                 const std::string& order = "",
                                 const std::vector<std::string>& via = {});

// Build m.space.parent state event JSON including type, state_key, and content.
// stateKey = parent space ID.
std::string buildSpaceParentEvent(const std::string& parentSpaceId,
                                  bool canonical = false,
                                  const std::vector<std::string>& via = {});

// ================================================================
// Space child / parent event parsers — full event JSON → structured data
// Original Kotlin: SpaceChildSummaryEvent.kt, SpaceChildContent.kt, SpaceParentContent.kt
// ================================================================

// Parse a full m.space.child state event JSON.
// Extracts type, state_key, sender, origin_server_ts, and content (order, via, suggested).
SpaceChildWithOrder parseSpaceChildEvent(const std::string& eventJson);

// Parse a full m.space.parent state event JSON.
// Extracts type, state_key, sender, content (via, canonical). Returns (spaceId, canonical, via, sender).
std::pair<std::string, SpaceParentEntry> parseSpaceParentEvent(const std::string& eventJson);

} // namespace progressive
