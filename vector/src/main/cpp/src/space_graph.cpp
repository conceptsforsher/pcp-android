#include "progressive/space_graph.hpp"
#include <sstream>
#include <algorithm>
#include <queue>
#include <stack>
#include <functional>

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

// ====== Parsing ======

SpaceChildEntry parseSpaceChild(const std::string& stateKey, const std::string& contentJson) {
    SpaceChildEntry entry;
    entry.roomId = stateKey;
    entry.suggested = !extractBool(contentJson, "suggested");
    if (contentJson.find("\"suggested\"") == std::string::npos) entry.suggested = true; // default true
    if (contentJson.find("\"suggested\":false") != std::string::npos) entry.suggested = false;

    entry.order = extractStr(contentJson, "order");

    // Parse via array
    size_t pos = contentJson.find("\"via\"");
    if (pos != std::string::npos) {
        pos = contentJson.find('[', pos);
        if (pos != std::string::npos) {
            pos++;
            while (pos < contentJson.size()) {
                while (pos < contentJson.size() && (contentJson[pos] == ' ' || contentJson[pos] == ',')) pos++;
                if (pos >= contentJson.size() || contentJson[pos] == ']') break;
                if (contentJson[pos] == '"') {
                    pos++;
                    size_t e = pos;
                    while (e < contentJson.size() && contentJson[e] != '"') e++;
                    entry.via.push_back(contentJson.substr(pos, e - pos));
                    pos = e + 1;
                } else pos++;
            }
        }
    }

    entry.valid = !entry.roomId.empty();
    return entry;
}

SpaceParentEntry parseSpaceParent(const std::string& contentJson) {
    SpaceParentEntry entry;
    entry.spaceId = extractStr(contentJson, "room_id");
    if (entry.spaceId.empty()) entry.spaceId = extractStr(contentJson, "space_id");
    entry.canonical = extractBool(contentJson, "canonical");

    // Parse via
    size_t pos = contentJson.find("\"via\"");
    if (pos != std::string::npos) {
        pos = contentJson.find('[', pos);
        if (pos != std::string::npos) {
            pos++;
            while (pos < contentJson.size()) {
                while (pos < contentJson.size() && (contentJson[pos] == ' ' || contentJson[pos] == ',')) pos++;
                if (pos >= contentJson.size() || contentJson[pos] == ']') break;
                if (contentJson[pos] == '"') {
                    pos++;
                    size_t e = pos;
                    while (e < contentJson.size() && contentJson[e] != '"') e++;
                    entry.via.push_back(contentJson.substr(pos, e - pos));
                    pos = e + 1;
                } else pos++;
            }
        }
    }

    return entry;
}

// ====== SpaceGraph Implementation ======

SpaceGraph::SpaceGraph() {}

void SpaceGraph::setRoot(const std::string& spaceId, const std::string& name,
                          const std::string& topic, const std::string& avatarUrl) {
    rootId_ = spaceId;
    SpaceNode root;
    root.roomId = spaceId;
    root.name = name;
    root.topic = topic;
    root.avatarUrl = avatarUrl;
    root.type = SpaceNodeType::SPACE;
    root.depth = 0;
    root.isSuggested = true;
    root.valid = true;
    nodes_[spaceId] = root;
}

void SpaceGraph::addChild(const std::string& parentId, const SpaceChildEntry& child) {
    childMap_[parentId].push_back(child);
    parentMap_[child.roomId].push_back(parentId);

    // Create node if it doesn't exist
    if (nodes_.find(child.roomId) == nodes_.end()) {
        SpaceNode node;
        node.roomId = child.roomId;
        node.isSuggested = child.suggested;
        if (!child.via.empty()) node.viaServer = child.via[0];
        node.type = SpaceNodeType::ROOM; // May be updated later
        nodes_[child.roomId] = node;
    }
}

void SpaceGraph::setNodeMetadata(const std::string& roomId, const std::string& name,
                                  const std::string& topic, const std::string& avatarUrl,
                                  const std::string& joinRule, bool isJoined) {
    auto it = nodes_.find(roomId);
    if (it == nodes_.end()) {
        SpaceNode node;
        node.roomId = roomId;
        node.name = name;
        node.topic = topic;
        node.avatarUrl = avatarUrl;
        node.joinRule = joinRule;
        node.isJoined = isJoined;
        node.valid = true;
        nodes_[roomId] = node;
    } else {
        it->second.name = name;
        it->second.topic = topic;
        it->second.avatarUrl = avatarUrl;
        it->second.joinRule = joinRule;
        it->second.isJoined = isJoined;
    }
}

void SpaceGraph::addParent(const std::string& roomId, const SpaceParentEntry& parent) {
    parentMap_[roomId].push_back(parent.spaceId);
}

void SpaceGraph::setOrder(const std::string& parentId, const std::string& childId,
                           const std::string& order) {
    orderMap_[parentId][childId] = order;
}

// ====== Graph Traversal ======

void SpaceGraph::traverseBFS(const SpaceTraversalOptions& options, SpaceGraphResult& result) {
    std::queue<std::pair<std::string, int>> q; // (nodeId, depth)
    std::unordered_set<std::string> visited;

    q.push({rootId_, 0});
    visited.insert(rootId_);

    // Ensure root node exists
    if (nodes_.find(rootId_) != nodes_.end()) {
        result.root = nodes_[rootId_];
        result.totalSpaces++;
    }

    while (!q.empty() && result.totalNodes < options.maxResults) {
        auto [nodeId, depth] = q.front(); q.pop();

        if (depth > options.maxDepth) continue;

        // Get children
        auto childIt = childMap_.find(nodeId);
        if (childIt == childMap_.end()) continue;

        std::vector<SpaceChildEntry> sortedChildren = childIt->second;

        // Sort by order if we have order data
        auto orderIt = orderMap_.find(nodeId);
        if (orderIt != orderMap_.end()) {
            std::sort(sortedChildren.begin(), sortedChildren.end(),
                [&](const SpaceChildEntry& a, const SpaceChildEntry& b) {
                    auto oa = orderIt->second.find(a.roomId);
                    auto ob = orderIt->second.find(b.roomId);
                    std::string orderA = (oa != orderIt->second.end()) ? oa->second : "";
                    std::string orderB = (ob != orderIt->second.end()) ? ob->second : "";
                    return orderA < orderB;
                });
        }

        for (const auto& child : sortedChildren) {
            if (visited.find(child.roomId) != visited.end()) continue;
            if (options.includeSuggestedOnly && !child.suggested) continue;

            visited.insert(child.roomId);

            auto nodeIt = nodes_.find(child.roomId);
            if (nodeIt == nodes_.end() || !nodeIt->second.valid) continue;

            SpaceNode node = nodeIt->second;
            node.depth = depth + 1;
            node.valid = true;

            // Update root's child count
            if (nodeIt->second.type == SpaceNodeType::ROOM) {
                nodes_[nodeId].childCount++;
                nodes_[nodeId].totalDescendantCount++;
            }

            result.flatList.push_back(node);
            result.children[nodeId].push_back(node);
            result.totalNodes++;
            result.maxDepth = std::max(result.maxDepth, depth + 1);

            if (node.type == SpaceNodeType::ROOM) result.totalRooms++;
            else {
                result.totalSpaces++;
                if (options.includeSubspaces) {
                    q.push({child.roomId, depth + 1});
                }
            }
        }
    }
}

void SpaceGraph::traverseDFS(const std::string& nodeId, int depth,
                              const SpaceTraversalOptions& options,
                              SpaceGraphResult& result,
                              std::unordered_set<std::string>& visited) {
    if (depth > options.maxDepth || result.totalNodes >= options.maxResults) return;
    if (visited.find(nodeId) != visited.end()) return;

    visited.insert(nodeId);

    auto nodeIt = nodes_.find(nodeId);
    if (nodeIt != nodes_.end() && nodeIt->second.valid) {
        SpaceNode node = nodeIt->second;
        node.depth = depth;
        result.flatList.push_back(node);
        result.totalNodes++;
        result.maxDepth = std::max(result.maxDepth, depth);

        if (node.type == SpaceNodeType::ROOM) result.totalRooms++;
        else result.totalSpaces++;
    }

    auto childIt = childMap_.find(nodeId);
    if (childIt == childMap_.end()) return;

    for (const auto& child : childIt->second) {
        if (options.includeSuggestedOnly && !child.suggested) continue;

        auto cn = nodes_.find(child.roomId);
        if (cn == nodes_.end()) continue;

        if (cn->second.type == SpaceNodeType::ROOM || options.includeSubspaces) {
            traverseDFS(child.roomId, depth + 1, options, result, visited);
        }
    }
}

SpaceGraphResult SpaceGraph::traverse(const SpaceTraversalOptions& options) {
    SpaceGraphResult result;

    if (rootId_.empty()) return result;

    switch (options.mode) {
        case SpaceTraversal::BREADTH_FIRST:
            traverseBFS(options, result);
            break;
        case SpaceTraversal::DEPTH_FIRST:
        case SpaceTraversal::ORDERED: {
            std::unordered_set<std::string> visited;
            traverseDFS(rootId_, 0, options, result, visited);
            break;
        }
    }

    return result;
}

std::vector<SpaceNode> SpaceGraph::getChildren(const std::string& spaceId) const {
    std::vector<SpaceNode> result;
    auto it = childMap_.find(spaceId);
    if (it == childMap_.end()) return result;

    for (const auto& child : it->second) {
        auto ni = nodes_.find(child.roomId);
        if (ni != nodes_.end()) {
            result.push_back(ni->second);
        }
    }

    return sortByOrder(result, spaceId);
}

std::vector<std::string> SpaceGraph::getParents(const std::string& roomId) const {
    auto it = parentMap_.find(roomId);
    if (it != parentMap_.end()) return it->second;
    return {};
}

std::vector<std::string> SpaceGraph::getAncestors(const std::string& roomId) const {
    std::vector<std::string> ancestors;
    std::string current = roomId;
    std::unordered_set<std::string> visited;

    while (true) {
        visited.insert(current);
        auto it = parentMap_.find(current);
        if (it == parentMap_.end() || it->second.empty()) break;

        // Take the first canonical parent, or first parent
        std::string parent = it->second[0];
        if (visited.find(parent) != visited.end()) break; // Cycle guard

        ancestors.push_back(parent);
        current = parent;
        if (current == rootId_) break; // Reached root
    }

    return ancestors;
}

int SpaceGraph::getDepth(const std::string& roomId) const {
    if (roomId == rootId_) return 0;

    std::string current = roomId;
    int depth = 0;
    std::unordered_set<std::string> visited;

    while (depth < 50) { // Safety limit
        visited.insert(current);
        auto it = parentMap_.find(current);
        if (it == parentMap_.end() || it->second.empty()) break;

        current = it->second[0];
        if (visited.find(current) != visited.end()) break;
        depth++;
        if (current == rootId_) return depth;
    }

    return -1; // Not in this space
}

bool SpaceGraph::isInSpace(const std::string& spaceId, const std::string& roomId) const {
    if (roomId == spaceId) return true;

    std::queue<std::string> q;
    std::unordered_set<std::string> visited;
    q.push(spaceId);
    visited.insert(spaceId);

    int maxIter = 1000;
    while (!q.empty() && maxIter-- > 0) {
        auto current = q.front(); q.pop();
        auto it = childMap_.find(current);
        if (it == childMap_.end()) continue;

        for (const auto& child : it->second) {
            if (child.roomId == roomId) return true;
            if (visited.find(child.roomId) == visited.end()) {
                visited.insert(child.roomId);
                q.push(child.roomId);
            }
        }
    }

    return false;
}

// ====== Queries ======

std::vector<SpaceNode> SpaceGraph::getSpaceRooms(const std::string& spaceId) const {
    std::vector<SpaceNode> rooms;
    auto it = childMap_.find(spaceId);
    if (it == childMap_.end()) return rooms;

    for (const auto& child : it->second) {
        auto ni = nodes_.find(child.roomId);
        if (ni != nodes_.end() && ni->second.type == SpaceNodeType::ROOM) {
            rooms.push_back(ni->second);
        }
    }

    return sortByOrder(rooms, spaceId);
}

std::vector<SpaceNode> SpaceGraph::getSubspaces(const std::string& spaceId) const {
    std::vector<SpaceNode> subspaces;
    auto it = childMap_.find(spaceId);
    if (it == childMap_.end()) return subspaces;

    for (const auto& child : it->second) {
        auto ni = nodes_.find(child.roomId);
        if (ni != nodes_.end() &&
            (ni->second.type == SpaceNodeType::SPACE || ni->second.type == SpaceNodeType::SUBSPACE)) {
            subspaces.push_back(ni->second);
        }
    }

    return sortByOrder(subspaces, spaceId);
}

std::vector<SpaceNode> SpaceGraph::searchSpaceRooms(const std::string& spaceId,
                                                      const std::string& query) const {
    std::vector<SpaceNode> results;
    if (query.empty()) return results;

    std::string q;
    for (char c : query) q += static_cast<char>(std::tolower(c));

    SpaceTraversalOptions opts;
    opts.mode = SpaceTraversal::BREADTH_FIRST;
    opts.includeSubspaces = true;
    opts.includeSuggestedOnly = false;

    auto traversal = traverse(opts);

    for (const auto& node : traversal.flatList) {
        if (node.type != SpaceNodeType::ROOM) continue;

        std::string name;
        for (char c : node.name) name += static_cast<char>(std::tolower(c));
        std::string topic;
        for (char c : node.topic) topic += static_cast<char>(std::tolower(c));

        if (name.find(q) != std::string::npos || topic.find(q) != std::string::npos) {
            results.push_back(node);
        }
    }

    return results;
}

// ====== Statistics ======

int SpaceGraph::deepestDepth() const {
    int maxd = 0;
    for (const auto& [id, node] : nodes_) {
        if (node.valid && node.depth > maxd) maxd = node.depth;
    }
    return maxd;
}

// ====== Sorting ======

std::vector<SpaceNode> SpaceGraph::sortByOrder(std::vector<SpaceNode> nodes, const std::string& parentId) const {
    auto it = orderMap_.find(parentId);
    if (it == orderMap_.end()) {
        // Sort alphabetically by name
        std::sort(nodes.begin(), nodes.end(), [](const SpaceNode& a, const SpaceNode& b) {
            return a.name < b.name;
        });
        return nodes;
    }

    std::sort(nodes.begin(), nodes.end(), [&](const SpaceNode& a, const SpaceNode& b) {
        auto oa = it->second.find(a.roomId);
        auto ob = it->second.find(b.roomId);
        std::string orderA = (oa != it->second.end()) ? oa->second : "z";
        std::string orderB = (ob != it->second.end()) ? ob->second : "z";
        if (orderA != orderB) return orderA < orderB;
        return a.name < b.name;
    });

    return nodes;
}

// ====== Serialization ======

std::string SpaceGraph::nodeToJson(const std::string& nodeId, int depthLeft,
                                    std::unordered_set<std::string>& visited) const {
    if (depthLeft <= 0 || visited.find(nodeId) != visited.end()) return "{}";
    visited.insert(nodeId);

    auto it = nodes_.find(nodeId);
    if (it == nodes_.end()) return "{}";

    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    const auto& node = it->second;
    std::ostringstream os;
    os << R"({"id":")" << esc(node.roomId)
       << R"(","name":")" << esc(node.name)
       << R"(","type":")" << (node.type == SpaceNodeType::SPACE ? "space" :
                                node.type == SpaceNodeType::SUBSPACE ? "subspace" : "room")
       << R"(","depth":)" << node.depth
       << R"(,"is_joined":)" << (node.isJoined ? "true" : "false")
       << R"(,"can_join":)" << (node.canJoin ? "true" : "false");

    // Children
    auto childIt = childMap_.find(nodeId);
    if (childIt != childMap_.end() && !childIt->second.empty()) {
        os << R"(,"children":[)";
        bool first = true;
        for (const auto& child : childIt->second) {
            auto childNode = nodes_.find(child.roomId);
            if (childNode == nodes_.end()) continue;
            if (childNode->second.type == SpaceNodeType::ROOM) continue; // Rooms are leaves

            if (!first) os << ","; first = false;
            os << nodeToJson(child.roomId, depthLeft - 1, visited);
        }
        os << "]";
    }

    // Room children (leaf rooms)
    if (childIt != childMap_.end()) {
        os << R"(,"rooms":[)";
        bool first = true;
        for (const auto& child : childIt->second) {
            auto childNode = nodes_.find(child.roomId);
            if (childNode == nodes_.end()) continue;
            if (childNode->second.type != SpaceNodeType::ROOM) continue;

            if (!first) os << ","; first = false;
            os << R"({"id":")" << esc(childNode->second.roomId)
               << R"(","name":")" << esc(childNode->second.name)
               << R"(","is_joined":)" << (childNode->second.isJoined ? "true" : "false")
               << "}";
        }
        os << "]";
    }

    os << "}";
    return os.str();
}

std::string SpaceGraph::spaceToTreeJson(const std::string& spaceId, int maxDepth) const {
    std::unordered_set<std::string> visited;
    return nodeToJson(spaceId.empty() ? rootId_ : spaceId, maxDepth, visited);
}

std::string SpaceGraph::flatListToJson(const std::vector<SpaceNode>& nodes) const {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    std::ostringstream os;
    os << "[";
    for (size_t i = 0; i < nodes.size(); i++) {
        if (i > 0) os << ",";
        os << R"({"id":")" << esc(nodes[i].roomId)
           << R"(","name":")" << esc(nodes[i].name)
           << R"(","type":")" << (nodes[i].type == SpaceNodeType::SPACE ? "space" :
                                    nodes[i].type == SpaceNodeType::SUBSPACE ? "subspace" : "room")
           << R"(,"depth":)" << nodes[i].depth
           << R"(,"is_joined":)" << (nodes[i].isJoined ? "true" : "false")
           << "}";
    }
    os << "]";
    return os.str();
}

std::string SpaceGraph::graphResultToJson(const SpaceGraphResult& result) const {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    std::ostringstream os;
    os << R"({"root_id":")" << esc(result.root.roomId)
       << R"(","root_name":")" << esc(result.root.name)
       << R"(,"total_nodes":)" << result.totalNodes
       << R"(,"total_spaces":)" << result.totalSpaces
       << R"(,"total_rooms":)" << result.totalRooms
       << R"(,"max_depth":)" << result.maxDepth
       << R"(,"nodes":)" << flatListToJson(result.flatList)
       << "}";
    return os.str();
}

void SpaceGraph::clear() {
    rootId_.clear();
    nodes_.clear();
    childMap_.clear();
    parentMap_.clear();
    orderMap_.clear();
}

// ================================================================
// Hierarchy Building
// Original Kotlin: buildSpaceHierarchy from HierarchyLiveDataHelper.kt, SpaceHierarchyViewModel.kt
// ================================================================

SpaceHierarchy buildSpaceHierarchy(
    const std::string& rootSpaceId,
    const std::vector<RoomSummaryInfo>& rooms,
    const std::unordered_map<std::string, std::vector<SpaceChildWithOrder>>& children,
    const std::unordered_map<std::string, std::vector<std::string>>& parents,
    int maxDepth)
{
    SpaceHierarchy h;
    h.rootId = rootSpaceId;

    // Build node map from rooms
    for (const auto& room : rooms) {
        SpaceTreeNode node;
        node.roomId = room.roomId;
        node.name = room.displayName;
        // Original Kotlin: checking roomType == RoomType.SPACE
        node.type = (room.roomType == "m.space") ? "space" : "room";
        node.isJoined = (room.membership == "join");
        node.depth = -1; // Unvisited marker
        h.nodes[node.roomId] = node;
    }

    // Ensure root exists in node map (may not be joined)
    if (h.nodes.find(rootSpaceId) == h.nodes.end()) {
        SpaceTreeNode rootNode;
        rootNode.roomId = rootSpaceId;
        rootNode.type = "space";
        rootNode.isJoined = true;
        rootNode.depth = 0;
        h.nodes[rootSpaceId] = rootNode;
    }

    // Link children into parent → child edges
    for (const auto& [parentId, childList] : children) {
        auto& parentNode = h.nodes[parentId];
        for (const auto& child : childList) {
            parentNode.children.push_back(child.childRoomId);
        }
    }

    // Link parents into child → parent edges
    for (const auto& [childId, parentList] : parents) {
        auto it = h.nodes.find(childId);
        if (it != h.nodes.end()) {
            for (const auto& p : parentList) {
                if (std::find(it->second.parentIds.begin(), it->second.parentIds.end(), p)
                    == it->second.parentIds.end()) {
                    it->second.parentIds.push_back(p);
                }
            }
        }
    }

    // BFS from root to set depths and collect nodes
    std::queue<std::string> q;
    std::unordered_set<std::string> visited;
    q.push(rootSpaceId);
    visited.insert(rootSpaceId);

    auto rootIt = h.nodes.find(rootSpaceId);
    if (rootIt != h.nodes.end()) {
        rootIt->second.depth = 0;
    }

    while (!q.empty()) {
        auto currentId = q.front(); q.pop();
        auto& current = h.nodes[currentId];
        int nextDepth = current.depth + 1;
        if (nextDepth > maxDepth) continue;

        for (const auto& childId : current.children) {
            if (visited.find(childId) != visited.end()) continue;
            visited.insert(childId);

            auto childIt = h.nodes.find(childId);
            if (childIt != h.nodes.end()) {
                childIt->second.depth = nextDepth;
                h.allNodes.push_back(childIt->second);
                if (childIt->second.type == "space") {
                    h.totalSpaces++;
                } else {
                    h.totalRooms++;
                }
                h.totalNodes++;
                if (nextDepth > h.maxDepth) h.maxDepth = nextDepth;

                // Recurse into subspaces
                if (childIt->second.type == "space") {
                    q.push(childId);
                }
            }
        }
    }

    // Set root in allNodes too
    if (rootIt != h.nodes.end()) {
        h.root = rootIt->second;
        // Prepend root to allNodes if not there
        bool hasRoot = false;
        for (const auto& n : h.allNodes) {
            if (n.roomId == rootSpaceId) { hasRoot = true; break; }
        }
        if (!hasRoot) {
            h.allNodes.insert(h.allNodes.begin(), h.root);
            h.totalNodes++;
            h.totalSpaces++;
        }
    }

    return h;
}

// ================================================================
// Space Tree Queries
// Original Kotlin: SpaceGetter.kt, HierarchyLiveDataHelper.kt
// ================================================================

std::vector<std::string> getSpaceChildren(const SpaceHierarchy& hierarchy, const std::string& spaceId) {
    std::vector<std::string> result;
    auto it = hierarchy.nodes.find(spaceId);
    if (it == hierarchy.nodes.end()) return result;
    return it->second.children;
}

std::vector<std::string> getSpaceDescendants(const SpaceHierarchy& hierarchy, const std::string& spaceId) {
    std::vector<std::string> result;
    if (hierarchy.nodes.find(spaceId) == hierarchy.nodes.end()) return result;

    std::queue<std::string> q;
    std::unordered_set<std::string> visited;
    q.push(spaceId);
    visited.insert(spaceId);

    while (!q.empty()) {
        auto current = q.front(); q.pop();
        auto it = hierarchy.nodes.find(current);
        if (it == hierarchy.nodes.end()) continue;

        for (const auto& childId : it->second.children) {
            if (visited.find(childId) != visited.end()) continue;
            visited.insert(childId);
            result.push_back(childId);

            auto childIt = hierarchy.nodes.find(childId);
            if (childIt != hierarchy.nodes.end() && childIt->second.type == "space") {
                q.push(childId);
            }
        }
    }

    return result;
}

std::vector<std::string> getSpaceAncestors(const SpaceHierarchy& hierarchy, const std::string& roomId) {
    std::vector<std::string> ancestors;
    std::string current = roomId;
    std::unordered_set<std::string> visited;

    while (true) {
        visited.insert(current);
        auto it = hierarchy.nodes.find(current);
        if (it == hierarchy.nodes.end() || it->second.parentIds.empty()) break;

        std::string parent = it->second.parentIds[0]; // First parent
        if (visited.find(parent) != visited.end()) break; // Cycle guard
        ancestors.push_back(parent);
        current = parent;
    }

    return ancestors;
}

bool isRoomInSpace(const SpaceHierarchy& hierarchy, const std::string& spaceId, const std::string& roomId) {
    if (roomId == spaceId) return true;

    std::queue<std::string> q;
    std::unordered_set<std::string> visited;
    q.push(spaceId);
    visited.insert(spaceId);

    while (!q.empty()) {
        auto current = q.front(); q.pop();
        auto it = hierarchy.nodes.find(current);
        if (it == hierarchy.nodes.end()) continue;

        for (const auto& childId : it->second.children) {
            if (childId == roomId) return true;
            if (visited.find(childId) == visited.end()) {
                visited.insert(childId);
                q.push(childId);
            }
        }
    }

    return false;
}

std::vector<std::string> getSpacePath(const SpaceHierarchy& hierarchy, const std::string& roomId) {
    std::vector<std::string> path;
    std::string current = roomId;
    std::unordered_set<std::string> visited;

    path.push_back(current);

    while (true) {
        visited.insert(current);
        auto it = hierarchy.nodes.find(current);
        if (it == hierarchy.nodes.end() || it->second.parentIds.empty()) break;

        std::string parent = it->second.parentIds[0];
        if (visited.find(parent) != visited.end()) break;
        path.push_back(parent);
        current = parent;
    }

    // Reverse to get root→room order
    std::reverse(path.begin(), path.end());
    return path;
}

std::vector<SpaceTreeNode> flattenSpaceTree(const SpaceHierarchy& hierarchy, bool bfs) {
    std::vector<SpaceTreeNode> result;

    if (bfs) {
        // BFS order — level by level
        std::queue<std::string> q;
        std::unordered_set<std::string> visited;

        if (hierarchy.nodes.find(hierarchy.rootId) != hierarchy.nodes.end()) {
            q.push(hierarchy.rootId);
            visited.insert(hierarchy.rootId);
        }

        while (!q.empty()) {
            auto currentId = q.front(); q.pop();
            auto it = hierarchy.nodes.find(currentId);
            if (it == hierarchy.nodes.end()) continue;
            result.push_back(it->second);

            for (const auto& childId : it->second.children) {
                if (visited.find(childId) == visited.end()) {
                    visited.insert(childId);
                    q.push(childId);
                }
            }
        }
    } else {
        // DFS order — pre-order
        std::function<void(const std::string&, std::unordered_set<std::string>&)> dfs =
            [&](const std::string& nodeId, std::unordered_set<std::string>& visited) {
                if (visited.find(nodeId) != visited.end()) return;
                visited.insert(nodeId);
                auto it = hierarchy.nodes.find(nodeId);
                if (it != hierarchy.nodes.end()) {
                    result.push_back(it->second);
                    for (const auto& childId : it->second.children) {
                        dfs(childId, visited);
                    }
                }
            };
        std::unordered_set<std::string> visited;
        dfs(hierarchy.rootId, visited);
    }

    return result;
}

// ================================================================
// Space Relationship Validation
// Original Kotlin: RoomSummaryUpdater.validateSpaceRelationship (lines 239-467)
// ================================================================

static bool isActiveMembership(const std::string& m) {
    // Original Kotlin: Membership.activeMemberships() = [JOIN, INVITE]
    return m == "join" || m == "invite";
}

static bool isValidParentRelation(
    const std::vector<SpaceRelationEvent>& events,
    const std::string& parentRoomId,
    const std::string& childRoomId,
    const std::string& senderId,
    const std::unordered_map<std::string, std::unordered_map<std::string, int>>& powerLevels)
{
    // Original Kotlin: criteria (a) or (b)
    // (a) Check if there is a corresponding m.space.child event in the parent pointing to this child
    for (const auto& ev : events) {
        if (ev.eventType == "m.space.child"
            && ev.roomId == parentRoomId
            && ev.stateKey == childRoomId) {
            return true;
        }
    }

    // (b) Check if sender has sufficient power level in the parent to send m.space.child
    // Default: events_default = 100 for spaces, m.space.child requires state_default or events_default
    auto powerIt = powerLevels.find(parentRoomId);
    if (powerIt != powerLevels.end()) {
        auto userIt = powerIt->second.find(senderId);
        int userLevel = (userIt != powerIt->second.end()) ? userIt->second : 0; // users_default
        int requiredLevel = 50; // Default state_default for m.space.child
        // Check state_default if present
        auto stateDefIt = powerIt->second.find("state_default");
        if (stateDefIt != powerIt->second.end()) {
            requiredLevel = stateDefIt->second;
        }
        return userLevel >= requiredLevel;
    }

    // No power level info available: cannot validate, treat as valid (permissive)
    return true;
}

SpaceValidationResult validateSpaceRelationships(
    std::vector<RoomSummaryInfo>& rooms,
    const std::vector<SpaceRelationEvent>& events,
    const std::unordered_map<std::string, std::unordered_map<std::string, int>>& powerLevels,
    const std::string& userId)
{
    SpaceValidationResult result;

    // Build room lookup
    std::unordered_map<std::string, RoomSummaryInfo*> roomMap;
    for (auto& room : rooms) {
        room.flattenParentIds.clear();
        room.directParentNames.clear();
        room.spaceHighlightCount = 0;
        room.spaceNotificationCount = 0;
        roomMap[room.roomId] = &room;
    }

    // Build adjacency: parent → set of children that have been validated both ways
    // key: parent space roomId → set of child roomIds
    std::unordered_map<std::string, std::unordered_set<std::string>> parentToChildren;

    // Step 1: Process m.space.child events — space declares children
    // Original Kotlin: lines 253-284 (`First handle child relations`)
    for (const auto& ev : events) {
        if (ev.eventType != "m.space.child") continue;
        // ev.roomId is the space, ev.stateKey is the child roomId
        auto parentIt = roomMap.find(ev.roomId);
        if (parentIt == roomMap.end()) continue;
        if (!isActiveMembership(parentIt->second->membership)) continue;
        // Original Kotlin: space children are only from SPACE-type rooms
        if (parentIt->second->roomType != "m.space") continue;

        auto childIt = roomMap.find(ev.stateKey);
        if (childIt == roomMap.end()) continue;
        if (!isActiveMembership(childIt->second->membership)) continue;

        parentToChildren[ev.roomId].insert(ev.stateKey);
    }

    // Step 2: Process m.space.parent events — rooms claim parents
    // Original Kotlin: lines 286-358 (`Now let's check parent relations`)
    for (const auto& ev : events) {
        if (ev.eventType != "m.space.parent") continue;
        // ev.roomId is the child room, ev.stateKey is the parent space ID
        auto childIt = roomMap.find(ev.roomId);
        if (childIt == roomMap.end()) continue;
        if (!isActiveMembership(childIt->second->membership)) continue;

        auto parentIt = roomMap.find(ev.stateKey);
        if (parentIt == roomMap.end()) continue;

        // Validate this parent claim
        if (isValidParentRelation(events, ev.stateKey, ev.roomId, ev.sender, powerLevels)) {
            parentToChildren[ev.stateKey].insert(ev.roomId);
            result.parentRelationsValidated++;
        }
    }

    // Step 3: Build graph and detect / break cycles
    // Original Kotlin: lines 365-384
    Graph graph;
    // Only insert space→space edges (spaces that are joined)
    for (const auto& room : rooms) {
        if (room.roomType == "m.space" && room.membership == "join") {
            graph.getOrCreateNode(room.roomId);
        }
    }

    for (const auto& [parentId, childSet] : parentToChildren) {
        auto parentNodeIt = roomMap.find(parentId);
        if (parentNodeIt == roomMap.end()) continue;
        if (parentNodeIt->second->roomType != "m.space") continue;
        if (parentNodeIt->second->membership != "join") continue;

        for (const auto& childId : childSet) {
            auto childIt = roomMap.find(childId);
            if (childIt == roomMap.end()) continue;
            if (childIt->second->roomType != "m.space") continue;
            if (childIt->second->membership != "join") continue;

            // Edge: child → parent (Original Kotlin: graph.addEdge(it.roomId, sum.roomId))
            graph.addEdge(childId, parentId);
        }
    }

    std::vector<GraphEdge> backEdges = graph.findBackwardEdges();
    result.cyclesBroken = static_cast<int>(backEdges.size());

    // Remove cycle edges from adjacency
    for (const auto& edge : backEdges) {
        auto it = parentToChildren.find(edge.source.name);
        if (it != parentToChildren.end()) {
            it->second.erase(edge.destination.name);
        }
        auto it2 = parentToChildren.find(edge.destination.name);
        if (it2 != parentToChildren.end()) {
            it2->second.erase(edge.source.name);
        }
    }

    // Step 4: Compute transitive closure (flattened parent IDs)
    // Original Kotlin: lines 386-389
    Graph acyclicGraph = graph.withoutEdges(backEdges);
    auto flattenDest = acyclicGraph.flattenDestination();

    // Build map: spaceId → all transitively reachable parent space IDs
    std::unordered_map<std::string, std::vector<std::string>> flattenSpaceParentIds;
    for (const auto& [node, reachableSet] : flattenDest) {
        std::vector<std::string> ids;
        for (const auto& n : reachableSet) {
            ids.push_back(n.name);
        }
        // Original Kotlin: flattenSpaceParents[parent.roomId] + listOf(parent.roomId)
        // The flattenDestination gives all *parents* (spaces) that this space is transitively a child of
        // Add self
        if (std::find(ids.begin(), ids.end(), node.name) == ids.end()) {
            ids.push_back(node.name);
        }
        flattenSpaceParentIds[node.name] = ids;
    }

    // Step 5: Assign flattenParentIds to children
    // Original Kotlin: lines 391-413
    for (const auto& [parentId, childSet] : parentToChildren) {
        auto parentIt = roomMap.find(parentId);
        if (parentIt == roomMap.end()) continue;
        if (parentIt->second->roomType != "m.space") continue;
        if (parentIt->second->membership != "join") continue;

        auto flattenIt = flattenSpaceParentIds.find(parentId);
        if (flattenIt == flattenSpaceParentIds.end()) continue;

        const auto& flattenParents = flattenIt->second;

        for (const auto& childId : childSet) {
            auto childRoom = roomMap.find(childId);
            if (childRoom == roomMap.end()) continue;

            // Add parent name
            if (std::find(childRoom->second.directParentNames.begin(),
                          childRoom->second.directParentNames.end(),
                          parentIt->second->displayName)
                == childRoom->second.directParentNames.end()) {
                childRoom->second.directParentNames.push_back(parentIt->second->displayName);
            }

            // Add all transitive parent IDs
            for (const auto& pid : flattenParents) {
                if (std::find(childRoom->second.flattenParentIds.begin(),
                              childRoom->second.flattenParentIds.end(), pid)
                    == childRoom->second.flattenParentIds.end()) {
                    childRoom->second.flattenParentIds.push_back(pid);
                }
            }
        }
    }

    // Step 6: DM room → space membership inference
    // Original Kotlin: lines 415-440
    for (auto& room : rooms) {
        if (!room.isDirect) continue;
        if (!isActiveMembership(room.membership)) continue;

        // Find spaces whose joinedMemberIds intersect with this DM room's members
        std::unordered_set<std::string> relatedSpaceIds;
        for (const auto& otherRoom : rooms) {
            if (otherRoom.roomType != "m.space") continue;
            if (!isActiveMembership(otherRoom.membership)) continue;

            for (const auto& memberId : room.joinedMemberIds) {
                if (otherRoom.joinedMemberIds.find(memberId) != otherRoom.joinedMemberIds.end()) {
                    relatedSpaceIds.insert(otherRoom.roomId);
                    break;
                }
            }
        }

        if (!relatedSpaceIds.empty()) {
            // Add transitive parents of each related space
            std::unordered_set<std::string> flattenRelated(relatedSpaceIds.begin(), relatedSpaceIds.end());
            for (const auto& spaceId : relatedSpaceIds) {
                auto it = flattenSpaceParentIds.find(spaceId);
                if (it != flattenSpaceParentIds.end()) {
                    for (const auto& pid : it->second) {
                        flattenRelated.insert(pid);
                    }
                }
            }

            for (const auto& pid : flattenRelated) {
                if (std::find(room.flattenParentIds.begin(), room.flattenParentIds.end(), pid)
                    == room.flattenParentIds.end()) {
                    room.flattenParentIds.push_back(pid);
                }
            }
            result.dmRoomsAssigned++;
        }
    }

    // Step 7: Compute space notification counts
    // Original Kotlin: lines 444-462
    for (auto& room : rooms) {
        if (room.roomType != "m.space") continue;
        if (!isActiveMembership(room.membership)) continue;

        int highlightCount = 0;
        int notificationCount = 0;
        for (const auto& otherRoom : rooms) {
            if (otherRoom.roomType == "m.space") continue; // Only count non-space children
            if (otherRoom.membership != "join") continue;

            // Check if this space is in the child's flattenParentIds
            bool isChild = false;
            for (const auto& pid : otherRoom.flattenParentIds) {
                if (pid == room.roomId) { isChild = true; break; }
            }
            if (isChild) {
                highlightCount += otherRoom.highlightCount;
                notificationCount += otherRoom.notificationCount;
            }
        }
        room.spaceHighlightCount = highlightCount;
        room.spaceNotificationCount = notificationCount;
    }

    result.totalSpacesProcessed = 0;
    result.totalRoomsProcessed = 0;
    for (const auto& room : rooms) {
        if (room.roomType == "m.space") result.totalSpacesProcessed++;
        else result.totalRoomsProcessed++;
    }

    return result;
}

// ================================================================
// Compute Space Notification Counts (standalone)
// Original Kotlin: RoomSummaryUpdater.kt lines 444-462
// ================================================================

void computeSpaceNotificationCounts(std::vector<RoomSummaryInfo>& rooms) {
    for (auto& room : rooms) {
        if (room.roomType != "m.space") continue;
        if (room.membership != "join" && room.membership != "invite") continue;

        int highlightCount = 0;
        int notificationCount = 0;

        for (const auto& otherRoom : rooms) {
            if (otherRoom.roomType == "m.space") continue; // Only non-space children
            if (otherRoom.membership != "join") continue;

            bool isChild = false;
            for (const auto& pid : otherRoom.flattenParentIds) {
                if (pid == room.roomId) { isChild = true; break; }
            }
            if (isChild) {
                highlightCount += otherRoom.highlightCount;
                notificationCount += otherRoom.notificationCount;
            }
        }

        room.spaceHighlightCount = highlightCount;
        room.spaceNotificationCount = notificationCount;
    }
}

// ================================================================
// Break Space Cycles
// Original Kotlin: RoomSummaryUpdater.kt lines 365-384
// ================================================================

std::vector<GraphEdge> breakSpaceCycles(
    const std::vector<RoomSummaryInfo>& rooms,
    const std::unordered_map<std::string, std::vector<std::string>>& parentMap)
{
    Graph graph;

    // Only add space-type rooms that are joined
    for (const auto& room : rooms) {
        if (room.roomType == "m.space" && room.membership == "join") {
            graph.getOrCreateNode(room.roomId);
        }
    }

    // Build edges: for each child → parent relationship (space only)
    for (const auto& room : rooms) {
        if (room.roomType != "m.space" || room.membership != "join") continue;

        auto it = parentMap.find(room.roomId);
        if (it == parentMap.end()) continue;

        for (const auto& parentId : it->second) {
            // Parent must also be a joined space
            bool parentIsSpace = false;
            for (const auto& r : rooms) {
                if (r.roomId == parentId && r.roomType == "m.space" && r.membership == "join") {
                    parentIsSpace = true;
                    break;
                }
            }
            if (!parentIsSpace) continue;

            graph.getOrCreateNode(parentId);
            // Edge: child → parent (Original Kotlin: graph.addEdge(it.roomId, sum.roomId))
            graph.addEdge(room.roomId, parentId);
        }
    }

    return graph.findBackwardEdges();
}

// ================================================================
// Space Child Ordering
// Original Kotlin: SpaceChildContent.kt validOrder(), TopLevelSpaceComparator.kt
// ================================================================

std::string parseSpaceChildOrder(const std::string& rawOrder) {
    // Original Kotlin: validOrder() — length ≤ 50, only ASCII chars in range \x20-\x7E
    if (rawOrder.empty()) return "";
    if (rawOrder.size() > 50) return "";

    for (char c : rawOrder) {
        unsigned char uc = static_cast<unsigned char>(c);
        if (uc < 0x20 || uc > 0x7E) return "";
    }

    return rawOrder;
}

void sortSpaceChildren(std::vector<SpaceChildWithOrder>& children,
                       const std::unordered_map<std::string, std::string>& roomNames)
{
    // Original Kotlin: sortedBy { it.order } then compareValues
    // Items with non-empty order sort first, then items with no order (by name)
    std::sort(children.begin(), children.end(),
        [&](const SpaceChildWithOrder& a, const SpaceChildWithOrder& b) {
            bool aHasOrder = !a.order.empty();
            bool bHasOrder = !b.order.empty();

            if (aHasOrder && bHasOrder) {
                return a.order < b.order;
            }
            if (aHasOrder) return true;     // Ordered items first (original Kotlin: null orders last)
            if (bHasOrder) return false;

            // Both no order — sort by name
            auto nameA = roomNames.find(a.childRoomId);
            auto nameB = roomNames.find(b.childRoomId);
            std::string na = (nameA != roomNames.end()) ? nameA->second : a.childRoomId;
            std::string nb = (nameB != roomNames.end()) ? nameB->second : b.childRoomId;
            return na < nb;
        });
}

} // namespace progressive
