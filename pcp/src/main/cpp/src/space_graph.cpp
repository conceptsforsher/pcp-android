#include "pcp/space_graph.hpp"
#include <sstream>
#include <algorithm>
#include <queue>
#include <stack>

namespace pcp {

// ================================================================
// SpaceChildEntry parsing
// ================================================================

SpaceChildEntry parseSpaceChild(const std::string& stateKey, const std::string& contentJson) {
    SpaceChildEntry entry;
    entry.roomId = stateKey;
    entry.valid = true;

    auto pos = contentJson.find("\"suggested\"");
    if (pos != std::string::npos) {
        entry.suggested = contentJson.find("false", pos) == std::string::npos;
    }
    pos = contentJson.find("\"order\"");
    if (pos != std::string::npos) {
        auto start = contentJson.find('"', pos + 7);
        auto end = contentJson.find('"', start + 1);
        if (start != std::string::npos && end != std::string::npos) {
            entry.order = contentJson.substr(start + 1, end - start - 1);
        }
    }
    pos = contentJson.find("\"via\"");
    if (pos != std::string::npos) {
        auto arrStart = contentJson.find('[', pos);
        auto arrEnd = contentJson.find(']', arrStart);
        if (arrStart != std::string::npos && arrEnd != std::string::npos) {
            std::string arr = contentJson.substr(arrStart + 1, arrEnd - arrStart - 1);
            size_t s = 0;
            while (s < arr.size()) {
                auto q1 = arr.find('"', s);
                if (q1 == std::string::npos) break;
                auto q2 = arr.find('"', q1 + 1);
                if (q2 == std::string::npos) break;
                entry.via.push_back(arr.substr(q1 + 1, q2 - q1 - 1));
                s = q2 + 1;
            }
        }
    }
    return entry;
}

// ================================================================
// SpaceParentEntry parsing
// ================================================================

SpaceParentEntry parseSpaceParent(const std::string& contentJson) {
    SpaceParentEntry entry;
    auto pos = contentJson.find("\"room_id\"");
    if (pos == std::string::npos) pos = contentJson.find("\"space_id\"");
    if (pos != std::string::npos) {
        auto start = contentJson.find('"', pos + 8);
        auto end = contentJson.find('"', start + 1);
        if (start != std::string::npos && end != std::string::npos) {
            entry.spaceId = contentJson.substr(start + 1, end - start - 1);
        }
    }
    pos = contentJson.find("\"canonical\"");
    if (pos != std::string::npos) {
        entry.canonical = contentJson.find("true", pos) != std::string::npos;
    }
    pos = contentJson.find("\"via\"");
    if (pos != std::string::npos) {
        auto arrStart = contentJson.find('[', pos);
        auto arrEnd = contentJson.find(']', arrStart);
        if (arrStart != std::string::npos && arrEnd != std::string::npos) {
            std::string arr = contentJson.substr(arrStart + 1, arrEnd - arrStart - 1);
            size_t s = 0;
            while (s < arr.size()) {
                auto q1 = arr.find('"', s);
                if (q1 == std::string::npos) break;
                auto q2 = arr.find('"', q1 + 1);
                if (q2 == std::string::npos) break;
                entry.via.push_back(arr.substr(q1 + 1, q2 - q1 - 1));
                s = q2 + 1;
            }
        }
    }
    return entry;
}

// ================================================================
// SpaceGraph implementation
// ================================================================

SpaceGraph::SpaceGraph() {}

void SpaceGraph::clear() {
    rootId_.clear();
    nodes_.clear();
    childMap_.clear();
    parentMap_.clear();
    orderMap_.clear();
}

// ---- Setup ----

void SpaceGraph::setRoot(const std::string& spaceId, const std::string& name,
                          const std::string& topic, const std::string& avatarUrl) {
    rootId_ = spaceId;
    SpaceNode& node = nodes_[spaceId];
    node.roomId = spaceId;
    node.name = name;
    node.topic = topic;
    node.avatarUrl = avatarUrl;
    node.type = SpaceNodeType::SPACE;
    node.depth = 0;
    node.valid = true;
}

void SpaceGraph::addChild(const std::string& parentId, const SpaceChildEntry& child) {
    if (!child.valid) return;
    childMap_[parentId].push_back(child);

    SpaceNode& cn = nodes_[child.roomId];
    cn.roomId = child.roomId;
    cn.type = SpaceNodeType::ROOM;
    cn.isSuggested = child.suggested;
    cn.viaServer = child.via.empty() ? "" : child.via[0];
    cn.valid = true;
}

void SpaceGraph::setNodeMetadata(const std::string& roomId, const std::string& name,
                                  const std::string& topic, const std::string& avatarUrl,
                                  const std::string& joinRule, bool isJoined) {
    SpaceNode& node = nodes_[roomId];
    node.roomId = roomId;
    if (!name.empty()) node.name = name;
    if (!topic.empty()) node.topic = topic;
    if (!avatarUrl.empty()) node.avatarUrl = avatarUrl;
    if (!joinRule.empty()) node.joinRule = joinRule;
    node.isJoined = isJoined;
    node.canJoin = (joinRule == "public" || joinRule == "knock");
    node.valid = true;
}

void SpaceGraph::addParent(const std::string& roomId, const SpaceParentEntry& parent) {
    if (parent.spaceId.empty()) return;
    parentMap_[roomId].push_back(parent.spaceId);
}

void SpaceGraph::setOrder(const std::string& parentId, const std::string& childId,
                           const std::string& order) {
    orderMap_[parentId][childId] = order;
}

// ---- Traversal ----

SpaceGraphResult SpaceGraph::traverse(const SpaceTraversalOptions& options) const {
    SpaceGraphResult result;
    if (options.mode == SpaceTraversal::DEPTH_FIRST) {
        std::unordered_set<std::string> visited;
        traverseDFS(rootId_, 0, options, result, visited);
    } else {
        traverseBFS(options, result);
    }
    return result;
}

void SpaceGraph::traverseBFS(const SpaceTraversalOptions& options, SpaceGraphResult& result) const {
    std::unordered_set<std::string> visited;
    // Queue of (nodeId, depth)
    using QueueEntry = std::pair<std::string, int>;
    std::deque<QueueEntry> q;

    auto rootIt = nodes_.find(rootId_);
    if (rootIt == nodes_.end()) return;
    result.root = rootIt->second;
    q.push_back({rootId_, 0});
    visited.insert(rootId_);

    while (!q.empty() && (int)result.flatList.size() < options.maxResults) {
        auto [nodeId, depth] = q.front();
        q.pop_front();

        if (depth > options.maxDepth) continue;

        auto nodeIt = nodes_.find(nodeId);
        if (nodeIt == nodes_.end()) continue;

        SpaceNode node = nodeIt->second;
        node.depth = depth;

        // Apply filters
        if (options.includeSuggestedOnly && !node.isSuggested) {
            if (!options.filterByType || node.type != options.typeFilter) continue;
        }
        if (options.filterByType && node.type != options.typeFilter) {
            // Skip this node but still traverse children
            bool skip = true;
            auto childIt = childMap_.find(nodeId);
            if (childIt != childMap_.end()) {
                for (const auto& child : childIt->second) {
                    if (!visited.count(child.roomId)) {
                        visited.insert(child.roomId);
                        auto childNodeIt = nodes_.find(child.roomId);
                        if (childNodeIt != nodes_.end()) {
                            SpaceNode cn = childNodeIt->second;
                            cn.depth = depth + 1;
                            if (options.includeSuggestedOnly && !cn.isSuggested) continue;
                            if (!options.filterByType || cn.type == options.typeFilter) {
                                result.children[nodeId].push_back(cn);
                            }
                            if (options.includeSubspaces || cn.type != SpaceNodeType::SPACE) {
                                q.push_back({child.roomId, depth + 1});
                            }
                        }
                    }
                }
            }
            if (skip) continue;
        }

        // Count types
        if (node.type == SpaceNodeType::SPACE || node.type == SpaceNodeType::SUBSPACE) {
            result.totalSpaces++;
        } else {
            result.totalRooms++;
        }
        result.totalNodes++;
        if (depth > result.maxDepth) result.maxDepth = depth;
        result.flatList.push_back(node);

        // Push children
        auto childIt = childMap_.find(nodeId);
        if (childIt != childMap_.end()) {
            // Sort children by order
            auto orderIt = orderMap_.find(nodeId);
            std::vector<SpaceChildEntry> orderedChildren = childIt->second;

            if (orderIt != orderMap_.end()) {
                std::sort(orderedChildren.begin(), orderedChildren.end(),
                    [&](const SpaceChildEntry& a, const SpaceChildEntry& b) {
                        auto oa = orderIt->second.find(a.roomId);
                        auto ob = orderIt->second.find(b.roomId);
                        std::string sa = (oa != orderIt->second.end()) ? oa->second : "";
                        std::string sb = (ob != orderIt->second.end()) ? ob->second : "";
                        return sa < sb;
                    });
            }

            for (const auto& child : orderedChildren) {
                if (!options.includeSuggestedOnly || child.suggested) {
                    if (!visited.count(child.roomId)) {
                        visited.insert(child.roomId);
                        auto childNodeIt = nodes_.find(child.roomId);
                        SpaceNode cn;
                        if (childNodeIt != nodes_.end()) {
                            cn = childNodeIt->second;
                        } else {
                            cn.roomId = child.roomId;
                        }
                        cn.depth = depth + 1;
                        result.children[nodeId].push_back(cn);

                        if (options.includeSubspaces || cn.type == SpaceNodeType::ROOM) {
                            q.push_back({child.roomId, depth + 1});
                        }
                    }
                }
            }
        }

        // Also add children to q for subspaces
        if (options.includeSubspaces) {
            auto subIt = childMap_.find(nodeId);
            if (subIt != childMap_.end()) {
                for (const auto& child : subIt->second) {
                    if (!visited.count(child.roomId)) {
                        visited.insert(child.roomId);
                        if (options.includeSubspaces) {
                            q.push_back({child.roomId, depth + 1});
                        }
                    }
                }
            }
        }
    }
}

void SpaceGraph::traverseDFS(const std::string& nodeId, int depth,
                               const SpaceTraversalOptions& options,
                               SpaceGraphResult& result,
                               std::unordered_set<std::string>& visited) const {
    if (depth > options.maxDepth) return;
    if ((int)result.flatList.size() >= options.maxResults) return;

    auto nodeIt = nodes_.find(nodeId);
    if (nodeIt == nodes_.end()) return;

    SpaceNode node = nodeIt->second;
    node.depth = depth;

    if (!options.filterByType || node.type == options.typeFilter) {
        if (!options.includeSuggestedOnly || node.isSuggested) {
            result.flatList.push_back(node);
        }
    }

    auto childIt = childMap_.find(nodeId);
    if (childIt != childMap_.end()) {
        // Sort children by order
        std::vector<SpaceChildEntry> ordered = childIt->second;
        auto orderIt = orderMap_.find(nodeId);
        if (orderIt != orderMap_.end()) {
            std::sort(ordered.begin(), ordered.end(),
                [&](const SpaceChildEntry& a, const SpaceChildEntry& b) {
                    auto oa = orderIt->second.find(a.roomId);
                    auto ob = orderIt->second.find(b.roomId);
                    std::string sa = (oa != orderIt->second.end()) ? oa->second : "";
                    std::string sb = (ob != orderIt->second.end()) ? ob->second : "";
                    return sa < sb;
                });
        }

        for (const auto& child : ordered) {
            if (!visited.count(child.roomId)) {
                visited.insert(child.roomId);
                traverseDFS(child.roomId, depth + 1, options, result, visited);
            }
        }
    }
}

// ---- Children / Parents ----

std::vector<SpaceNode> SpaceGraph::getChildren(const std::string& spaceId) const {
    std::vector<SpaceNode> result;
    auto it = childMap_.find(spaceId);
    if (it == childMap_.end()) return result;

    auto orderIt = orderMap_.find(spaceId);
    std::vector<SpaceChildEntry> ordered = it->second;

    if (orderIt != orderMap_.end()) {
        std::sort(ordered.begin(), ordered.end(),
            [&](const SpaceChildEntry& a, const SpaceChildEntry& b) {
                auto oa = orderIt->second.find(a.roomId);
                auto ob = orderIt->second.find(b.roomId);
                std::string sa = (oa != orderIt->second.end()) ? oa->second : "";
                std::string sb = (ob != orderIt->second.end()) ? ob->second : "";
                return sa < sb;
            });
    }

    for (const auto& child : ordered) {
        auto nodeIt = nodes_.find(child.roomId);
        if (nodeIt != nodes_.end()) {
            result.push_back(nodeIt->second);
        } else {
            SpaceNode n; n.roomId = child.roomId; result.push_back(n);
        }
    }
    return result;
}

std::vector<std::string> SpaceGraph::getParents(const std::string& roomId) const {
    std::vector<std::string> result;
    auto it = parentMap_.find(roomId);
    if (it != parentMap_.end()) return it->second;
    return result;
}

std::vector<std::string> SpaceGraph::getAncestors(const std::string& roomId) const {
    std::vector<std::string> result;
    std::string current = roomId;
    std::unordered_set<std::string> seen;
    int maxLoop = 100;

    while (current != rootId_ && maxLoop-- > 0) {
        seen.insert(current);
        auto it = parentMap_.find(current);
        if (it == parentMap_.end() || it->second.empty()) break;
        std::string next = it->second[0];
        if (seen.count(next)) break;
        result.push_back(next);
        current = next;
    }
    return result;
}

int SpaceGraph::getDepth(const std::string& roomId) const {
    if (roomId == rootId_) return 0;
    auto ancestors = getAncestors(roomId);
    return (int)ancestors.size();
}

bool SpaceGraph::isInSpace(const std::string& spaceId, const std::string& roomId) const {
    if (roomId == spaceId) return true;
    auto ancs = getAncestors(roomId);
    return std::find(ancs.begin(), ancs.end(), spaceId) != ancs.end();
}

std::vector<SpaceNode> SpaceGraph::getSpaceRooms(const std::string& spaceId) const {
    std::vector<SpaceNode> result;
    auto it = childMap_.find(spaceId);
    if (it == childMap_.end()) return result;

    for (const auto& child : it->second) {
        auto nodeIt = nodes_.find(child.roomId);
        if (nodeIt != nodes_.end()) {
            SpaceNode node = nodeIt->second;
            if (node.type == SpaceNodeType::ROOM) {
                result.push_back(node);
            }
        } else {
            SpaceNode n; n.roomId = child.roomId; n.type = SpaceNodeType::ROOM;
            result.push_back(n);
        }
    }
    return result;
}

std::vector<SpaceNode> SpaceGraph::getSubspaces(const std::string& spaceId) const {
    std::vector<SpaceNode> result;
    auto it = childMap_.find(spaceId);
    if (it == childMap_.end()) return result;

    for (const auto& child : it->second) {
        auto nodeIt = nodes_.find(child.roomId);
        if (nodeIt != nodes_.end()) {
            SpaceNode node = nodeIt->second;
            if (node.type == SpaceNodeType::SPACE || node.type == SpaceNodeType::SUBSPACE) {
                result.push_back(node);
            }
        }
    }
    return result;
}

std::vector<SpaceNode> SpaceGraph::searchSpaceRooms(const std::string& spaceId,
                                                       const std::string& query) const {
    std::vector<SpaceNode> result;
    if (query.empty()) return getChildren(spaceId);

    SpaceTraversalOptions opts;
    opts.mode = SpaceTraversal::BREADTH_FIRST;
    opts.includeSubspaces = true;
    opts.includeSuggestedOnly = false;
    opts.maxResults = 200;

    auto traversal = traverse(opts);
    std::string lowerQuery = query;
    std::transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);

    for (const auto& node : traversal.flatList) {
        if (node.roomId == spaceId) continue;

        std::string lowerName = node.name;
        std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
        std::string lowerTopic = node.topic;
        std::transform(lowerTopic.begin(), lowerTopic.end(), lowerTopic.begin(), ::tolower);

        if (lowerName.find(lowerQuery) != std::string::npos ||
            lowerTopic.find(lowerQuery) != std::string::npos ||
            node.roomId.find(lowerQuery) != std::string::npos) {
            result.push_back(node);
        }
    }
    return result;
}

// ---- Depth ----

int SpaceGraph::deepestDepth() const {
    int maxD = 0;
    for (const auto& [id, node] : nodes_) {
        int d = getDepth(id);
        if (d > maxD) maxD = d;
    }
    return maxD;
}

// ---- Serialization ----

std::string SpaceGraph::nodeToJson(const std::string& nodeId, int depthLeft,
                                     std::unordered_set<std::string>& visited) const {
    std::ostringstream os;
    if (depthLeft <= 0) return "null";
    if (visited.count(nodeId)) return "null";
    visited.insert(nodeId);

    auto nodeIt = nodes_.find(nodeId);
    if (nodeIt == nodes_.end()) return "null";
    const SpaceNode& node = nodeIt->second;

    os << "{";
    os << R"("room_id":")" << node.roomId << R"(")";
    if (!node.name.empty()) os << R"(,"name":")" << node.name << R"(")";
    os << R"(,"type":")" << (node.type == SpaceNodeType::SPACE ? "space" : "room") << R"(")";
    if (node.isJoined) os << R"(,"joined":true)";

    auto childIt = childMap_.find(nodeId);
    if (childIt != childMap_.end() && !childIt->second.empty()) {
        os << R"(,"children":[)";
        bool first = true;
        for (const auto& child : childIt->second) {
            if (!first) os << ",";
            first = false;
            std::string childJson = nodeToJson(child.roomId, depthLeft - 1, visited);
            if (childJson != "null") os << childJson;
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
    std::ostringstream os;
    os << "[";
    for (size_t i = 0; i < nodes.size(); i++) {
        if (i > 0) os << ",";
        const auto& n = nodes[i];
        os << "{";
        os << R"("room_id":")" << n.roomId << R"(")";
        if (!n.name.empty()) os << R"(,"name":")" << n.name << R"(")";
        os << R"(,"type":)" << static_cast<int>(n.type);
        os << R"(,"depth":)" << n.depth;
        os << "}";
    }
    os << "]";
    return os.str();
}

std::string SpaceGraph::graphResultToJson(const SpaceGraphResult& result) const {
    std::ostringstream os;
    os << "{";
    os << R"("total_nodes":)" << result.totalNodes;
    os << R"(,"total_spaces":)" << result.totalSpaces;
    os << R"(,"total_rooms":)" << result.totalRooms;
    os << R"(,"max_depth":)" << result.maxDepth;
    os << R"(,"root":{)";
    os << R"("room_id":")" << result.root.roomId << R"(")";
    if (!result.root.name.empty()) os << R"(,"name":")" << result.root.name << R"(")";
    os << "}";
    os << R"(,"flat_list":)" << flatListToJson(result.flatList);
    os << "}";
    return os.str();
}

// ---- Sort by Order ----

std::vector<SpaceNode> SpaceGraph::sortByOrder(std::vector<SpaceNode> nodes, const std::string& parentId) const {
    auto orderIt = orderMap_.find(parentId);
    if (orderIt != orderMap_.end()) {
        std::sort(nodes.begin(), nodes.end(),
            [&](const SpaceNode& a, const SpaceNode& b) {
                auto oa = orderIt->second.find(a.roomId);
                auto ob = orderIt->second.find(b.roomId);
                std::string sa = (oa != orderIt->second.end()) ? oa->second : "";
                std::string sb = (ob != orderIt->second.end()) ? ob->second : "";
                return sa < sb;
            });
    }
    return nodes;
}

} // namespace pcp
