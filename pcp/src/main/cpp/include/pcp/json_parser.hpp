#ifndef PCP_JSON_PARSER_HPP
#define PCP_JSON_PARSER_HPP

#include <string>
#include <cstdint>

namespace pcp {

// Minimal JSON parser — extracts a string value for a given key
// Handles objects like {"key": "value"} or {"key": 123}
std::string parseJsonStringValue(const std::string& json, const std::string& key);

bool parseJsonBoolValue(const std::string& json, const std::string& key, bool defaultValue = false);

int64_t parseJsonInt64Value(const std::string& json, const std::string& key, int64_t defaultValue = 0);

} // namespace pcp

#endif // PCP_JSON_PARSER_HPP
