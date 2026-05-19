#include "progressive/space_graph.hpp"
#include <sstream>
#include <algorithm>
#include <queue>
#include <stack>

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
        node.joinRules = joinRule;
        node.isJoined = isJoined;
        node.valid = true;
        nodes_[roomId] = node;
    } else {
        it->second.name = name;
        it->second.topic = topic;
        it->second.avatarUrl = avatarUrl;
        it->second.joinRules = joinRule;
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

} // namespace progressive
