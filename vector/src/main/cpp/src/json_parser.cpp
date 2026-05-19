#include "progressive/json_parser.hpp"
#include <cstddef>

namespace progressive {

std::string parseJsonStringValue(const std::string& json, const std::string& key) {
    // Search for "key"
    std::string search = '"' + key + '"';
    auto pos = json.find(search);
    if (pos == std::string::npos) return {};

    pos += search.size();

    // Skip whitespace and colon
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n' || json[pos] == '\r'))
        ++pos;
    if (pos >= json.size() || json[pos] != ':') return {};
    ++pos;

    // Skip whitespace after colon
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n' || json[pos] == '\r'))
        ++pos;
    if (pos >= json.size()) return {};

    // Handle string value: "xxx"
    if (json[pos] == '"') {
        ++pos;
        auto end = json.find('"', pos);
        if (end == std::string::npos) return {};
        return json.substr(pos, end - pos);
    }

    // Handle numeric/literal value: 123, true, false, null
    auto end = pos;
    while (end < json.size() && json[end] != ',' && json[end] != '}' && json[end] != ' ' &&
           json[end] != '\t' && json[end] != '\n' && json[end] != '\r') {
        ++end;
    }
    return json.substr(pos, end - pos);
}

} // namespace progressive
