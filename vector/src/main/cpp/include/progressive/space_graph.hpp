#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cstdint>
#include "progressive/graph_utils.hpp"

namespace progressive {

// ================================================================
// Space Graph — hierarchical Matrix space management
//
// Ported from Element Android/Web:
//   SpaceListViewModel.kt, SpaceHierarchyViewModel.kt
//   SpaceGraph.kt, SpaceNode.kt
//
// Matrix Space spec (MSC1772):
//   m.space.child state events define space → room/sub-space relations
//   m.space.parent state events define reverse links
//   m.space.order state events define ordering
//
// Space structure:
//   !space:org
//     ├── !room1:org (m.space.child)
//     ├── !room2:org (m.space.child)
//     └── !subspace:org (m.space.child) — recursive
//           ├── !room3:org
//           └── !room4:org
// ================================================================

// ---- Space Node Type ----

enum class SpaceNodeType {
    SPACE = 0,            // m.space (a space)
    ROOM = 1,             // A regular room
    SUBSPACE = 2,         // A room that is also a space
};

// ---- Space Node ----

struct SpaceNode {
    std::string roomId;              // !room:example.org
    std::string name;                // Room name
    std::string topic;               // Room topic
    std::string avatarUrl;           // mxc:// avatar
    SpaceNodeType type = SpaceNodeType::ROOM;
    int depth = 0;                   // Distance from root (0 = root itself)
    int childCount = 0;              // Direct children count
    int totalDescendantCount = 0;    // All descendants (recursive)
    bool isSuggested = true;         // Visible in spaces (not suggested = hidden)
    bool canJoin = true;             // Can join (vs invite only)
    bool isJoined = false;           // User is a member
    std::string joinRule;            // "public", "invite", "knock"
    std::string viaServer;           // Recommended server for joining
    int order = 0;                   // m.space.order value
    bool valid = false;
};

// ---- Space Child Entry ----
// From m.space.child state event content

struct SpaceChildEntry {
    std::string roomId;              // Child room/space ID
    bool suggested = true;           // Showing in hierarchy
    std::string order;               // String order (for sorting)
    std::vector<std::string> via;    // Recommended servers for joining
    bool valid = false;
};

// Parse m.space.child state event content.
SpaceChildEntry parseSpaceChild(const std::string& stateKey, const std::string& contentJson);

// ---- Space Parent Entry ----
// From m.space.parent state event content

struct SpaceParentEntry {
    std::string spaceId;             // Parent space ID
    bool canonical = false;          // Is this the canonical parent?
    std::vector<std::string> via;    // Recommended servers
};

// Parse m.space.parent state event content.
SpaceParentEntry parseSpaceParent(const std::string& contentJson);

// ---- Space Order Entry ----

struct SpaceOrderEntry {
    std::string roomId;
    std::string order;               // String for lexicographic sort
};

// ---- Space Traversal Options ----

enum class SpaceTraversal {
    BREADTH_FIRST = 0,      // Level by level (spaces → rooms)
    DEPTH_FIRST = 1,        // Deep paths first
    ORDERED = 2,            // By m.space.order
};

struct SpaceTraversalOptions {
    SpaceTraversal mode = SpaceTraversal::BREADTH_FIRST;
    int maxDepth = 10;              // Maximum recursion depth
    int maxResults = 500;           // Maximum nodes to return
    bool includeSubspaces = true;    // Include subspaces in results
    bool includeSuggestedOnly = true; // Only show suggested children
    bool filterByType = false;       // Filter by node type
    SpaceNodeType typeFilter = SpaceNodeType::ROOM;
};

// ---- Space Graph Result ----

struct SpaceGraphResult {
    SpaceNode root;                           // The root space
    std::vector<SpaceNode> flatList;          // All nodes (flattened)
    std::unordered_map<std::string, std::vector<SpaceNode>> children; // parent → children
    int totalNodes = 0;
    int totalSpaces = 0;
    int totalRooms = 0;
    int maxDepth = 0;
};

// ---- Space Graph ----

class SpaceGraph {
public:
    SpaceGraph();

    // ====== Space Setup ======

    // Set the root space.
    void setRoot(const std::string& spaceId, const std::string& name = "",
                 const std::string& topic = "", const std::string& avatarUrl = "");

    // Add a child to a parent space/room from m.space.child state event.
    void addChild(const std::string& parentId, const SpaceChildEntry& child);

    // Add room metadata (name, topic, avatar, join rule) for a node.
    void setNodeMetadata(const std::string& roomId, const std::string& name,
                          const std::string& topic, const std::string& avatarUrl,
                          const std::string& joinRule, bool isJoined);

    // Add m.space.parent event data for reverse linking.
    void addParent(const std::string& roomId, const SpaceParentEntry& parent);

    // Add m.space.order event data.
    void setOrder(const std::string& parentId, const std::string& childId,
                  const std::string& order);

    // ====== Graph Traversal ======

    // Traverse the space hierarchy and return all nodes.
    SpaceGraphResult traverse(const SpaceTraversalOptions& options = {}) const;

    // Get direct children of a space.
    std::vector<SpaceNode> getChildren(const std::string& spaceId) const;

    // Get the parent space(s) of a room/space.
    std::vector<std::string> getParents(const std::string& roomId) const;

    // Get all ancestors (parent chain) to root.
    std::vector<std::string> getAncestors(const std::string& roomId) const;

    // Get depth of a node (distance from root).
    int getDepth(const std::string& roomId) const;

    // Check if a room is within a space hierarchy.
    bool isInSpace(const std::string& spaceId, const std::string& roomId) const;

    // ====== Space Queries ======

    // Get all rooms in a space (flat list, no subspaces).
    std::vector<SpaceNode> getSpaceRooms(const std::string& spaceId) const;

    // Get all subspaces in a space.
    std::vector<SpaceNode> getSubspaces(const std::string& spaceId) const;

    // Search for rooms within a space by name/topic.
    std::vector<SpaceNode> searchSpaceRooms(const std::string& spaceId,
                                              const std::string& query) const;

    // ====== Statistics ======

    int nodeCount() const { return static_cast<int>(nodes_.size()); }
    int edgeCount() const { return static_cast<int>(childMap_.size()); }
    int deepestDepth() const;

    // ====== Serialization ======

    // Export space tree as JSON (nested structure).
    std::string spaceToTreeJson(const std::string& spaceId, int maxDepth = 5) const;

    // Export flat list as JSON.
    std::string flatListToJson(const std::vector<SpaceNode>& nodes) const;

    // Export space result as JSON.
    std::string graphResultToJson(const SpaceGraphResult& result) const;

    // ====== Clear ======

    void clear();

private:
    std::string rootId_;
    std::unordered_map<std::string, SpaceNode> nodes_;              // roomId → SpaceNode
    std::unordered_map<std::string, std::vector<SpaceChildEntry>> childMap_; // parentId → children
    std::unordered_map<std::string, std::vector<std::string>> parentMap_;    // roomId → parent IDs
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> orderMap_; // parentId → (childId → order)

    // BFS traversal helper.
    void traverseBFS(const SpaceTraversalOptions& options, SpaceGraphResult& result);

    // DFS traversal helper.
    void traverseDFS(const std::string& nodeId, int depth,
                     const SpaceTraversalOptions& options,
                     SpaceGraphResult& result,
                     std::unordered_set<std::string>& visited);

    // Sort children by order string.
    std::vector<SpaceNode> sortByOrder(std::vector<SpaceNode> nodes, const std::string& parentId) const;

    // Compute tree JSON recursively.
    std::string nodeToJson(const std::string& nodeId, int depthLeft,
                            std::unordered_set<std::string>& visited) const;
};

// ================================================================
// Space Tree Node — hierarchical node in the space tree
// Original Kotlin: SpaceNode (conceptually), SpaceHierarchyViewModel.kt
// ================================================================

struct SpaceTreeNode {
    std::string roomId;
    std::string name;
    std::string type;                       // "space", "room", or ""
    std::vector<std::string> children;      // Child room/subspace IDs
    std::vector<std::string> parentIds;     // Parent space IDs
    int depth = 0;
    bool isJoined = false;
    bool isSuggested = true;
    std::string order;                      // m.space.order
};

// ================================================================
// Space Hierarchy — full tree with root + all nodes + flattened lookup
// Original Kotlin: SpaceHierarchyData.kt, HierarchyLiveDataHelper.kt
// ================================================================

struct SpaceHierarchy {
    std::string rootId;
    SpaceTreeNode root;                                     // Root space node
    std::unordered_map<std::string, SpaceTreeNode> nodes;   // roomId → node
    std::vector<SpaceTreeNode> allNodes;                    // Flattened (BFS order)
    int totalNodes = 0;
    int totalSpaces = 0;
    int totalRooms = 0;
    int maxDepth = 0;

    bool empty() const { return totalNodes == 0; }
    const SpaceTreeNode* findNode(const std::string& roomId) const {
        auto it = nodes.find(roomId);
        return (it != nodes.end()) ? &it->second : nullptr;
    }
};

// ================================================================
// Space Child With Order — for sorting space children
// Original Kotlin: SpaceChildInfo.kt (order field), SpaceChildContent.kt
// ================================================================

struct SpaceChildWithOrder {
    std::string childRoomId;
    std::string order;
    std::vector<std::string> viaServers;
    bool suggested = false;
    bool autoJoin = false;
};

// ================================================================
// Room Summary Info — minimal room data for space validation
// Original Kotlin: RoomSummaryEntity fields, RoomSummary.kt
// ================================================================

struct RoomSummaryInfo {
    std::string roomId;
    std::string roomType;                   // "m.space" or ""
    std::string membership;                 // "join", "invite", "leave", "ban"
    std::string displayName;
    std::unordered_set<std::string> joinedMemberIds;
    bool isDirect = false;
    int highlightCount = 0;
    int notificationCount = 0;
    // Output fields — set by validateSpaceRelationships
    std::vector<std::string> flattenParentIds;
    std::vector<std::string> directParentNames;
    int spaceHighlightCount = 0;
    int spaceNotificationCount = 0;
};

// ================================================================
// Space Relation Event — raw state event for space relationships
// Original Kotlin: SpaceChildSummaryEvent.kt, RoomChildRelationInfo.kt
// ================================================================

struct SpaceRelationEvent {
    std::string eventType;      // "m.space.child" or "m.space.parent"
    std::string roomId;         // The room this event is in
    std::string stateKey;       // The other room/spaces ID
    std::string sender;
    std::string contentJson;    // Raw content JSON string
    int64_t originServerTs = 0;
};

// ================================================================
// Space Validation Result
// Original Kotlin: RoomSummaryUpdater.validateSpaceRelationship return
// ================================================================

struct SpaceValidationResult {
    int totalRoomsProcessed = 0;
    int totalSpacesProcessed = 0;
    int cyclesBroken = 0;
    int parentRelationsValidated = 0;
    int dmRoomsAssigned = 0;
    bool ok() const { return cyclesBroken >= 0; }
};

// ================================================================
// Hierarchy building — from flat room summaries + child/parent relations
// ================================================================

// Original Kotlin: buildSpaceHierarchy (conceptual, from SpaceListViewModel/HierarchyLiveDataHelper)
// Build a SpaceHierarchy tree from a list of rooms and child/parent relations.
// `rooms`: all room summaries (JOIN members only typically)
// `children`: child entries keyed by parent roomId
// `parents`: parent entries keyed by child roomId
SpaceHierarchy buildSpaceHierarchy(
    const std::string& rootSpaceId,
    const std::vector<RoomSummaryInfo>& rooms,
    const std::unordered_map<std::string, std::vector<SpaceChildWithOrder>>& children,
    const std::unordered_map<std::string, std::vector<std::string>>& parents,
    int maxDepth = 10);

// ================================================================
// Space tree queries
// Original Kotlin: SpaceGetter.kt, HierarchyLiveDataHelper.kt
// ================================================================

// Direct children of a space (only immediate, not recursive).
std::vector<std::string> getSpaceChildren(const SpaceHierarchy& hierarchy, const std::string& spaceId);

// All descendants — transitive (children + children of subspaces, etc.).
std::vector<std::string> getSpaceDescendants(const SpaceHierarchy& hierarchy, const std::string& spaceId);

// All ancestors up to root from a room.
std::vector<std::string> getSpaceAncestors(const SpaceHierarchy& hierarchy, const std::string& roomId);

// Check if a room is in a space's hierarchy (any depth).
bool isRoomInSpace(const SpaceHierarchy& hierarchy, const std::string& spaceId, const std::string& roomId);

// Path from root space to a room (inclusive).
std::vector<std::string> getSpacePath(const SpaceHierarchy& hierarchy, const std::string& roomId);

// Flatten the space tree into a list in BFS or DFS order.
std::vector<SpaceTreeNode> flattenSpaceTree(const SpaceHierarchy& hierarchy, bool bfs = true);

// ================================================================
// Space relationship validation — ported from RoomSummaryUpdater.kt
// Original Kotlin: RoomSummaryUpdater.validateSpaceRelationship (lines 239-467)
// ================================================================

// Full validation:
//   1. Build parent/child graph from state events
//   2. Validate parent relations (cross-check power levels / child events)
//   3. Detect and break cycles (uses Graph::findBackwardEdges)
//   4. Compute flattened parent IDs (transitive closure)
//   5. Handle DM room → space membership inference
//   6. Compute space notification counts from children
// `rooms`: IN/OUT — flattened parent IDs and notification counts are set
// `events`: all m.space.child and m.space.parent state events across rooms
// `powerLevels`: map from roomId → (userId → powerLevel), fallback to default
// `userId`: current user (for power level checks)
SpaceValidationResult validateSpaceRelationships(
    std::vector<RoomSummaryInfo>& rooms,
    const std::vector<SpaceRelationEvent>& events,
    const std::unordered_map<std::string, std::unordered_map<std::string, int>>& powerLevels,
    const std::string& userId);

// ================================================================
// Compute space notification counts
// Original Kotlin: RoomSummaryUpdater.kt lines 444-462
// ================================================================

// Aggregates highlightCount and notificationCount from child rooms into parent spaces.
// `rooms`: IN/OUT — spaceHighlightCount and spaceNotificationCount are set on space-type rooms.
void computeSpaceNotificationCounts(std::vector<RoomSummaryInfo>& rooms);

// ================================================================
// Break space cycles
// Original Kotlin: RoomSummaryUpdater.kt lines 365-384
// ================================================================

// Builds a Graph from space→space parent relationships (spaces only, joined),
// finds backward edges (cycles), and returns the edges that need to be removed.
// Caller should remove these edges from the child/parent lookup maps.
std::vector<GraphEdge> breakSpaceCycles(
    const std::vector<RoomSummaryInfo>& rooms,
    const std::unordered_map<std::string, std::vector<std::string>>& parentMap);

// ================================================================
// Space child ordering
// Original Kotlin: SpaceOrderUtils.kt, TopLevelSpaceComparator.kt, SpaceChildContent.kt
// ================================================================

// Sort vector of SpaceChildWithOrder by order string, then by name (via roomNames lookup).
// Items with empty order sort after ordered items.
void sortSpaceChildren(std::vector<SpaceChildWithOrder>& children,
                       const std::unordered_map<std::string, std::string>& roomNames);

// Parse and validate an order string per spec:
// Only ASCII chars in range \x20-\x7E, max 50 chars.
// Returns empty string if invalid.
std::string parseSpaceChildOrder(const std::string& rawOrder);

} // namespace progressive
