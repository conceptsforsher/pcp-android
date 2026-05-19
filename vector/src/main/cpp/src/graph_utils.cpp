#include "progressive/graph_utils.hpp"
#include <stack>
#include <sstream>
#include <chrono>

namespace progressive {

int64_t SimpleClock::epochMillis() const {
    auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()).count();
}

// ==== Graph Implementation ====
//
// Original Kotlin (GraphUtils.kt:38-166)

GraphNode Graph::getOrCreateNode(const std::string& name) {
    // Original Kotlin: adjacencyList.entries.firstOrNull { it.key.name == name }
    for (const auto& [node, _] : adjacencyList_) {
        if (node.name == name) return node;
    }
    GraphNode newNode{name};
    adjacencyList_[newNode] = {};  // create entry
    return newNode;
}

void Graph::addEdge(const std::string& sourceName, const std::string& destinationName) {
    auto src = getOrCreateNode(sourceName);
    auto dst = getOrCreateNode(destinationName);
    adjacencyList_[src].push_back({src, dst});
}

void Graph::addEdge(const GraphNode& source, const GraphNode& destination) {
    // Ensure both nodes exist
    getOrCreateNode(source.name);
    getOrCreateNode(destination.name);
    adjacencyList_[source].push_back({source, destination});
}

std::vector<GraphEdge> Graph::edgesOf(const GraphNode& node) const {
    auto it = adjacencyList_.find(node);
    if (it != adjacencyList_.end()) return it->second;
    return {};
}

Graph Graph::withoutEdges(const std::vector<GraphEdge>& edgesToPrune) const {
    // Original Kotlin: copy graph without specified edges
    Graph output;
    // Use set for O(1) lookup
    std::unordered_set<GraphEdge, GraphEdgeHash> pruneSet(
        edgesToPrune.begin(), edgesToPrune.end());

    for (const auto& [vertex, edges] : adjacencyList_) {
        output.getOrCreateNode(vertex.name);
        for (const auto& edge : edges) {
            if (pruneSet.find(edge) == pruneSet.end()) {
                output.addEdge(edge.source, edge.destination);
            }
        }
    }
    return output;
}

std::vector<GraphEdge> Graph::findBackwardEdges(const GraphNode* startFrom) {
    // Original Kotlin: iterative DFS with 3-color marking
    // Colors: -1 = NOT_VISITED, 0 = IN_PATH, 1 = COMPLETED
    constexpr int NOT_VISITED = -1;
    constexpr int IN_PATH = 0;
    constexpr int COMPLETED = 1;

    std::vector<GraphEdge> backwardEdges;
    std::unordered_map<GraphNode, int, GraphNodeHash> visited;
    for (const auto& [node, _] : adjacencyList_) visited[node] = NOT_VISITED;

    std::vector<GraphNode> stack;

    // Find starting node
    if (startFrom && visited.count(*startFrom) && visited[*startFrom] == NOT_VISITED) {
        stack.push_back(*startFrom);
        visited[*startFrom] = IN_PATH;
    } else {
        for (const auto& [node, _] : adjacencyList_) {
            if (visited[node] == NOT_VISITED) {
                stack.push_back(node);
                visited[node] = IN_PATH;
                break;
            }
        }
    }

    while (!stack.empty()) {
        const GraphNode& vertex = stack.back();

        // Find next unvisited outgoing edge
        GraphNode* destination = nullptr;
        for (const auto& edge : adjacencyList_[vertex]) {
            switch (visited[edge.destination]) {
                case NOT_VISITED:
                    destination = const_cast<GraphNode*>(&edge.destination);
                    break;
                case IN_PATH:
                    // Original Kotlin: Cycle!!
                    backwardEdges.push_back(edge);
                    break;
                case COMPLETED:
                    // dead end
                    break;
            }
            if (destination) break; // take the first candidate
        }

        if (!destination) {
            // dead end, pop
            visited[vertex] = COMPLETED;
            stack.pop_back();

            if (stack.empty()) {
                // Try next component of forest
                for (const auto& [node, _] : adjacencyList_) {
                    if (visited[node] == NOT_VISITED) {
                        stack.push_back(node);
                        visited[node] = IN_PATH;
                        break;
                    }
                }
            }
        } else {
            stack.push_back(*destination);
            visited[*destination] = IN_PATH;
        }
    }

    return backwardEdges;
}

std::unordered_map<GraphNode, std::unordered_set<GraphNode, GraphNodeHash>, GraphNodeHash>
Graph::flattenDestination() const {
    // Original Kotlin: transitive closure via recursive descent
    // Only call on acyclic graph!

    // Helper: recursively collects all reachable nodes
    std::function<std::unordered_set<GraphNode, GraphNodeHash>(const GraphNode&)> flattenOf;
    std::unordered_map<GraphNode, std::unordered_set<GraphNode, GraphNodeHash>, GraphNodeHash> memo;

    flattenOf = [&](const GraphNode& node) -> std::unordered_set<GraphNode, GraphNodeHash> {
        auto it = memo.find(node);
        if (it != memo.end()) return it->second;

        std::unordered_set<GraphNode, GraphNodeHash> result;
        auto edges = edgesOf(node);
        for (const auto& edge : edges) {
            result.insert(edge.destination);
            auto sub = flattenOf(edge.destination);
            result.insert(sub.begin(), sub.end());
        }
        memo[node] = result;
        return result;
    };

    std::unordered_map<GraphNode, std::unordered_set<GraphNode, GraphNodeHash>, GraphNodeHash> result;
    for (const auto& [node, _] : adjacencyList_) {
        result[node] = flattenOf(node);
    }
    return result;
}

std::string Graph::toString() const {
    std::ostringstream os;
    for (const auto& [node, edges] : adjacencyList_) {
        os << node.name << " : [";
        bool first = true;
        for (const auto& e : edges) {
            if (!first) os << " ";
            first = false;
            os << e.destination.name;
        }
        os << "]\n";
    }
    return os.str();
}

} // namespace progressive
