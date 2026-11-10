#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <functional>

namespace pcp::jsonviewer {

using json = nlohmann::json;

struct JsonNode {
    std::string key;
    std::string value;
    std::string type; // "object", "array", "string", "number", "boolean", "null"
    int depth = 0;
    bool expanded = false;
    bool isLast = false;
    std::vector<JsonNode> children;
    int childCount = 0;
    size_t dataSize = 0;
};

class JsonViewer {
public:
    JsonViewer();
    ~JsonViewer();

    void setJson(const json& data);
    void setJson(const std::string& jsonStr);
    const json& root() const { return m_root; }

    std::vector<JsonNode> buildTree();
    JsonNode buildNode(const std::string& key, const json& value, int depth);

    std::string format(bool pretty = true, int indent = 2);
    json search(const std::string& path);
    json query(const std::string& jsonPath);

    void expandAll();
    void collapseAll();
    void expandToDepth(int maxDepth);

    using NodeCallback = std::function<void(const JsonNode&)>;
    void traverse(NodeCallback cb);

private:
    json m_root;
    static std::string typeName(const json& value);
    static std::string formatValue(const json& value);
    static std::string escapeString(const std::string& s);
    static size_t estimateSize(const json& value);
};

} // namespace pcp::jsonviewer
