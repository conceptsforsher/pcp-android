#include "json_viewer.hpp"
#include <sstream>
#include <iomanip>
#include <algorithm>

namespace pcp::jsonviewer {

JsonViewer::JsonViewer() = default;
JsonViewer::~JsonViewer() = default;

void JsonViewer::setJson(const json& data) { m_root = data; }

void JsonViewer::setJson(const std::string& jsonStr) {
    try { m_root = json::parse(jsonStr); }
    catch (...) { m_root = json::object(); }
}

std::vector<JsonNode> JsonViewer::buildTree() {
    std::vector<JsonNode> tree;
    if (m_root.is_object()) {
        for (auto it = m_root.begin(); it != m_root.end(); ++it) {
            bool last = std::next(it) == m_root.end();
            auto node = buildNode(it.key(), it.value(), 0);
            node.isLast = last;
            tree.push_back(node);
        }
    } else if (m_root.is_array()) {
        int idx = 0;
        for (auto it = m_root.begin(); it != m_root.end(); ++it, ++idx) {
            bool last = std::next(it) == m_root.end();
            auto node = buildNode("[" + std::to_string(idx) + "]", *it, 0);
            node.isLast = last;
            tree.push_back(node);
        }
    } else {
        tree.push_back(buildNode("root", m_root, 0));
    }
    return tree;
}

JsonNode JsonViewer::buildNode(const std::string& key, const json& value, int depth) {
    JsonNode node;
    node.key = key;
    node.type = typeName(value);
    node.depth = depth;
    node.dataSize = estimateSize(value);
    node.expanded = depth < 2;
    if (value.is_object()) {
        node.childCount = static_cast<int>(value.size());
        node.value = "{...} (" + std::to_string(value.size()) + ")";
        if (node.expanded) {
            for (auto it = value.begin(); it != value.end(); ++it) {
                node.children.push_back(buildNode(it.key(), it.value(), depth + 1));
            }
        }
    } else if (value.is_array()) {
        node.childCount = static_cast<int>(value.size());
        node.value = "[...] (" + std::to_string(value.size()) + ")";
        if (node.expanded) {
            int idx = 0;
            for (auto it = value.begin(); it != value.end(); ++it, ++idx) {
                node.children.push_back(
                    buildNode("[" + std::to_string(idx) + "]", *it, depth + 1));
            }
        }
    } else {
        node.value = formatValue(value);
    }
    return node;
}

std::string JsonViewer::format(bool pretty, int indent) {
    return pretty ? m_root.dump(indent) : m_root.dump();
}

json JsonViewer::search(const std::string& path) {
    json current = m_root;
    std::istringstream ss(path);
    std::string segment;
    while (std::getline(ss, segment, '.')) {
        size_t bracket = segment.find('[');
        if (bracket != std::string::npos) {
            std::string key = segment.substr(0, bracket);
            size_t endBracket = segment.find(']', bracket);
            int idx = std::stoi(segment.substr(bracket + 1, endBracket - bracket - 1));
            if (!key.empty()) current = current[key];
            if (current.is_array() && idx < static_cast<int>(current.size())) {
                current = current[idx];
            }
        } else {
            if (current.is_object() && current.contains(segment)) {
                current = current[segment];
            } else { return json(); }
        }
    }
    return current;
}

json JsonViewer::query(const std::string& jsonPath) { return search(jsonPath); }
void JsonViewer::expandAll() {}
void JsonViewer::collapseAll() {}
void JsonViewer::expandToDepth(int /*maxDepth*/) {}

void JsonViewer::traverse(NodeCallback cb) {
    auto tree = buildTree();
    for (auto& node : tree) cb(node);
}

std::string JsonViewer::typeName(const json& value) {
    if (value.is_object()) return "object";
    if (value.is_array()) return "array";
    if (value.is_string()) return "string";
    if (value.is_number_float()) return "float";
    if (value.is_number_integer()) return "integer";
    if (value.is_boolean()) return "boolean";
    if (value.is_null()) return "null";
    return "unknown";
}

std::string JsonViewer::formatValue(const json& value) {
    if (value.is_string()) return value.get<std::string>();
    if (value.is_boolean()) return value.get<bool>() ? "true" : "false";
    if (value.is_null()) return "null";
    if (value.is_number()) {
        std::ostringstream os;
        os << std::setprecision(10) << value.get<double>();
        return os.str();
    }
    return value.dump();
}

std::string JsonViewer::escapeString(const std::string& s) {
    std::ostringstream os;
    for (char c : s) {
        if (c == '"') os << "\\\"";
        else if (c == '\\') os << "\\\\";
        else if (c == '\n') os << "\\n";
        else if (c == '\r') os << "\\r";
        else if (c == '\t') os << "\\t";
        else os << c;
    }
    return os.str();
}

size_t JsonViewer::estimateSize(const json& value) {
    return value.dump().size();
}

} // namespace pcp::jsonviewer
