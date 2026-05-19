#pragma once

#include <string>
#include <cstdint>

namespace progressive {

// Minimal JSON parser — extracts a string value for a given key
// Handles objects like {"key": "value"} or {"key": 123}
std::string parseJsonStringValue(const std::string& json, const std::string& key);

// Extract a boolean value for a given key.
// Returns the value, or defaultValue if the key is not found or not a boolean.
bool parseJsonBoolValue(const std::string& json, const std::string& key, bool defaultValue = false);

// Extract an int64_t value for a given key.
// Returns the value, or defaultValue if the key is not found or not a number.
int64_t parseJsonInt64Value(const std::string& json, const std::string& key, int64_t defaultValue = 0);

} // namespace progressive
