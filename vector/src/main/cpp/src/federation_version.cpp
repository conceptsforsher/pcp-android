#include "progressive/federation_version.hpp"
#include <cstdlib>

namespace progressive {

// Manual JSON helpers for federation version (no external deps).
// The response format is simple: {"server": {"name": "Synapse", "version": "1.62.0"}}

static std::string extractInnerObject(const std::string& json, const std::string& key) {
    // Find "key":
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return "";
    pos = json.find('{', pos);
    if (pos == std::string::npos) return "";
    // Brace-count to extract the object
    int depth = 1;
    size_t start = pos;
    pos++;
    while (pos < json.size() && depth > 0) {
        if (json[pos] == '{') depth++;
        else if (json[pos] == '}') depth--;
        pos++;
    }
    return json.substr(start, pos - start);
}

static std::string extractString(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return "";
    pos = json.find(':', pos);
    if (pos == std::string::npos) return "";
    pos++;
    // Skip whitespace
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n')) pos++;
    if (pos >= json.size()) return "";
    if (json[pos] == '"') {
        pos++;
        size_t end = pos;
        while (end < json.size() && json[end] != '"') {
            if (json[end] == '\\') end++; // skip escaped char
            end++;
        }
        return json.substr(pos, end - pos);
    }
    // Could be a number or null — for federation version, name/version are strings
    return "";
}

// Original Kotlin (GetFederationVersionTask.kt:30-40):
//   override suspend fun execute(params: Unit): FederationVersion {
//       val result = executeRequest(null) { federationAPI.getVersion() }
//       return FederationVersion(
//           name = result.server?.name,
//           version = result.server?.version
//       )
//   }

FederationVersion parseFederationVersion(const std::string& json) {
    FederationVersion result;

    auto serverObj = extractInnerObject(json, "server");
    if (serverObj.empty()) return result;

    // Original Kotlin: result.server?.name
    result.name = extractString(serverObj, "name");

    // Original Kotlin: result.server?.version
    result.version = extractString(serverObj, "version");

    return result;
}

std::string federationVersionToJson(const FederationVersion& version) {
    std::string srv = "{\"name\":\"" + version.name + "\",\"version\":\"" + version.version + "\"}";
    return "{\"server\":" + srv + "}";
}

} // namespace progressive
