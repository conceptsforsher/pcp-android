#include "progressive/room_version.hpp"
#include <sstream>
#include <algorithm>
#include <unordered_set>

namespace progressive {

// ====================================================================
// RoomVersionStatus
// ====================================================================

// Original Kotlin: RoomVersionStatus.kt
const char* roomVersionStatusToString(RoomVersionStatus status) {
    switch (status) {
        case RoomVersionStatus::STABLE:       return "stable";
        case RoomVersionStatus::UNSTABLE:     return "unstable";
        case RoomVersionStatus::DEPRECATED:   return "deprecated";
        case RoomVersionStatus::DISCONTINUED: return "discontinued";
    }
    return "stable";
}

RoomVersionStatus roomVersionStatusFromString(const std::string& s) {
    if (s == "stable")       return RoomVersionStatus::STABLE;
    if (s == "unstable")     return RoomVersionStatus::UNSTABLE;
    if (s == "deprecated")   return RoomVersionStatus::DEPRECATED;
    if (s == "discontinued") return RoomVersionStatus::DISCONTINUED;
    return RoomVersionStatus::STABLE;
}

// ====================================================================
// VersionComparison
// ====================================================================

// Original Kotlin: VersionComparison.kt
const char* versionComparisonToString(VersionComparison cmp) {
    switch (cmp) {
        case VersionComparison::SAME:          return "same";
        case VersionComparison::NEWER:         return "newer";
        case VersionComparison::OLDER:         return "older";
        case VersionComparison::INCOMPARABLE:  return "incomparable";
    }
    return "incomparable";
}

// ====================================================================
// Master room version definitions
// ====================================================================

namespace {

struct MasterRoomVersion {
    std::string version;
    std::string description;
    RoomVersionStatus status;
    const char* releaseDate;
    std::vector<const char*> features;
};

const std::vector<MasterRoomVersion> MASTER_VERSIONS = {
    {
        RoomVersions::V1,
        "v1  - Original",
        RoomVersionStatus::DISCONTINUED,
        "2014-09-15",
        {}
    },
    {
        RoomVersions::V2,
        "v2  - State resolution v2",
        RoomVersionStatus::DISCONTINUED,
        "2018-07-15",
        {RoomVersionFeatures::STATE_RESOLUTION_V2}
    },
    {
        RoomVersions::V3,
        "v3  - Event IDs as hashes",
        RoomVersionStatus::DISCONTINUED,
        "2018-10-12",
        {RoomVersionFeatures::STATE_RESOLUTION_V2, RoomVersionFeatures::EVENT_ID_HASHES}
    },
    {
        RoomVersions::V4,
        "v4  - State resolution v2 + event IDs as hashes",
        RoomVersionStatus::DISCONTINUED,
        "2019-01-11",
        {RoomVersionFeatures::STATE_RESOLUTION_V2, RoomVersionFeatures::EVENT_ID_HASHES}
    },
    {
        RoomVersions::V5,
        "v5  - Knock + restrictions",
        RoomVersionStatus::DISCONTINUED,
        "2019-03-28",
        {RoomVersionFeatures::STATE_RESOLUTION_V2, RoomVersionFeatures::EVENT_ID_HASHES,
         RoomVersionFeatures::KNOCK, RoomVersionFeatures::RESTRICTED}
    },
    {
        RoomVersions::V6,
        "v6  - MSC2176 implicit PL",
        RoomVersionStatus::STABLE,
        "2019-11-18",
        {RoomVersionFeatures::STATE_RESOLUTION_V2, RoomVersionFeatures::EVENT_ID_HASHES,
         RoomVersionFeatures::KNOCK, RoomVersionFeatures::RESTRICTED,
         RoomVersionFeatures::IMPLICIT_PL, RoomVersionFeatures::REDACTION_RULES}
    },
    {
        RoomVersions::V7,
        "v7  - MSC2175 ignore PL invite",
        RoomVersionStatus::STABLE,
        "2020-10-01",
        {RoomVersionFeatures::STATE_RESOLUTION_V2, RoomVersionFeatures::EVENT_ID_HASHES,
         RoomVersionFeatures::KNOCK, RoomVersionFeatures::RESTRICTED,
         RoomVersionFeatures::IMPLICIT_PL, RoomVersionFeatures::REDACTION_RULES,
         RoomVersionFeatures::IGNORE_PL_INVITE}
    },
    {
        RoomVersions::V8,
        "v8  - MSC2403 add reason to knock",
        RoomVersionStatus::STABLE,
        "2021-02-17",
        {RoomVersionFeatures::STATE_RESOLUTION_V2, RoomVersionFeatures::EVENT_ID_HASHES,
         RoomVersionFeatures::KNOCK, RoomVersionFeatures::RESTRICTED,
         RoomVersionFeatures::IMPLICIT_PL, RoomVersionFeatures::REDACTION_RULES,
         RoomVersionFeatures::IGNORE_PL_INVITE, RoomVersionFeatures::KNOCK_REASON}
    },
    {
        RoomVersions::V9,
        "v9  - MSC2174 implicit profiles",
        RoomVersionStatus::STABLE,
        "2022-01-21",
        {RoomVersionFeatures::STATE_RESOLUTION_V2, RoomVersionFeatures::EVENT_ID_HASHES,
         RoomVersionFeatures::KNOCK, RoomVersionFeatures::RESTRICTED,
         RoomVersionFeatures::IMPLICIT_PL, RoomVersionFeatures::REDACTION_RULES,
         RoomVersionFeatures::IGNORE_PL_INVITE, RoomVersionFeatures::KNOCK_REASON,
         RoomVersionFeatures::IMPLICIT_PROFILES}
    },
    {
        RoomVersions::V10,
        "v10 - MSC2174 + MSC2176 + MSC2175",
        RoomVersionStatus::STABLE,
        "2023-03-15",
        {RoomVersionFeatures::STATE_RESOLUTION_V2, RoomVersionFeatures::EVENT_ID_HASHES,
         RoomVersionFeatures::KNOCK, RoomVersionFeatures::RESTRICTED,
         RoomVersionFeatures::IMPLICIT_PL, RoomVersionFeatures::REDACTION_RULES,
         RoomVersionFeatures::IGNORE_PL_INVITE, RoomVersionFeatures::KNOCK_REASON,
         RoomVersionFeatures::IMPLICIT_PROFILES}
    },
    {
        RoomVersions::V11,
        "v11 - MSC3820 room version 11",
        RoomVersionStatus::STABLE,
        "2024-02-12",
        {RoomVersionFeatures::STATE_RESOLUTION_V2, RoomVersionFeatures::EVENT_ID_HASHES,
         RoomVersionFeatures::KNOCK, RoomVersionFeatures::RESTRICTED,
         RoomVersionFeatures::IMPLICIT_PL, RoomVersionFeatures::REDACTION_RULES,
         RoomVersionFeatures::IGNORE_PL_INVITE, RoomVersionFeatures::KNOCK_REASON,
         RoomVersionFeatures::IMPLICIT_PROFILES, RoomVersionFeatures::ROOM_VERSION_11}
    },
};

} // anonymous namespace

// ====================================================================
// Version info queries
// ====================================================================

// Original Kotlin: getRoomVersionInfo()
RoomVersionInfo getRoomVersionInfo(const std::string& version) {
    for (const auto& mv : MASTER_VERSIONS) {
        if (mv.version == version) {
            RoomVersionInfo info;
            info.version = mv.version;
            info.status = mv.status;
            info.releaseDate = mv.releaseDate;
            for (const auto* f : mv.features) {
                info.features.push_back(f);
            }
            return info;
        }
    }
    // Unknown version — return with minimal info
    RoomVersionInfo unknown;
    unknown.version = version;
    unknown.status = RoomVersionStatus::UNSTABLE;
    return unknown;
}

// Original Kotlin: getAllRoomVersions()
std::vector<RoomVersionInfo> getAllRoomVersions() {
    std::vector<RoomVersionInfo> result;
    for (const auto& mv : MASTER_VERSIONS) {
        RoomVersionInfo info;
        info.version = mv.version;
        info.status = mv.status;
        info.releaseDate = mv.releaseDate;
        for (const auto* f : mv.features) {
            info.features.push_back(f);
        }
        result.push_back(std::move(info));
    }
    return result;
}

// Original Kotlin: getRoomVersionFeatures()
std::vector<std::string> getRoomVersionFeatures(const std::string& version) {
    for (const auto& mv : MASTER_VERSIONS) {
        if (mv.version == version) {
            std::vector<std::string> features;
            for (const auto* f : mv.features) {
                features.push_back(f);
            }
            return features;
        }
    }
    return {};
}

// Original Kotlin: isFeatureSupported()
bool isFeatureSupported(const std::string& version, const std::string& feature) {
    auto features = getRoomVersionFeatures(version);
    for (const auto& f : features) {
        if (f == feature) return true;
    }
    return false;
}

// Original Kotlin: getLatestRoomVersion()
std::string getLatestRoomVersion() {
    std::string latest = "1";
    int latestNum = 1;
    for (const auto& mv : MASTER_VERSIONS) {
        if (mv.status == RoomVersionStatus::STABLE) {
            int num = std::stoi(mv.version);
            if (num > latestNum) {
                latestNum = num;
                latest = mv.version;
            }
        }
    }
    return latest;
}

// Original Kotlin: getDefaultRoomVersion()
std::string getDefaultRoomVersion() {
    return "10"; // Current Element default
}

// ====================================================================
// Legacy functions
// ====================================================================

// Original Kotlin: getKnownRoomVersions() — legacy
std::vector<RoomVersion> getKnownRoomVersions() {
    std::vector<RoomVersion> versions;
    for (const auto& mv : MASTER_VERSIONS) {
        RoomVersion v;
        v.id = mv.version;
        v.description = mv.description;
        v.isDefault = (mv.version == getDefaultRoomVersion());
        v.isStable = (mv.status == RoomVersionStatus::STABLE);
        versions.push_back(v);
    }
    return versions;
}

// Original Kotlin: isValidRoomVersion()
bool isValidRoomVersion(const std::string& version) {
    for (const auto& v : getKnownRoomVersions()) {
        if (v.id == version) return true;
    }
    return false;
}

// Original Kotlin: roomVersionsToJson()
std::string roomVersionsToJson() {
    auto versions = getKnownRoomVersions();
    std::ostringstream json;
    json << "[";
    for (size_t i = 0; i < versions.size(); ++i) {
        if (i > 0) json << ",";
        const auto& v = versions[i];
        json << R"({"id": ")" << v.id
             << R"(", "description": ")" << v.description
             << R"(", "stable": )" << (v.isStable ? "true" : "false") << "}";
    }
    json << "]";
    return json.str();
}

// ====================================================================
// Stability checks
// ====================================================================

// Original Kotlin: isRoomVersionStable()
bool isRoomVersionStable(const std::string& version) {
    for (const auto& mv : MASTER_VERSIONS) {
        if (mv.version == version && mv.status == RoomVersionStatus::STABLE) {
            return true;
        }
    }
    return false;
}

// Original Kotlin: isRoomVersionDeprecated()
bool isRoomVersionDeprecated(const std::string& version) {
    for (const auto& mv : MASTER_VERSIONS) {
        if (mv.version == version &&
            (mv.status == RoomVersionStatus::DEPRECATED ||
             mv.status == RoomVersionStatus::DISCONTINUED)) {
            return true;
        }
    }
    return false;
}

// ====================================================================
// Version comparison
// ====================================================================

namespace {

int parseVersionNumber(const std::string& version) {
    try {
        return std::stoi(version);
    } catch (...) {
        return -1;
    }
}

} // anonymous namespace

// Original Kotlin: compareVersions()
VersionComparison compareVersions(const std::string& v1, const std::string& v2) {
    int n1 = parseVersionNumber(v1);
    int n2 = parseVersionNumber(v2);

    if (n1 < 0 || n2 < 0) return VersionComparison::INCOMPARABLE;
    if (n1 == n2) return VersionComparison::SAME;
    if (n1 > n2) return VersionComparison::NEWER;
    return VersionComparison::OLDER;
}

// ====================================================================
// Capabilities parsing
// ====================================================================

namespace {

std::string jExtractStr(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return "";
    pos = json.find(':', pos);
    if (pos == std::string::npos) return "";
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n' || json[pos] == '\r')) pos++;
    if (pos >= json.size() || json[pos] != '"') return "";
    pos++;
    size_t end = pos;
    while (end < json.size() && json[end] != '"') {
        if (json[end] == '\\') end++;
        end++;
    }
    return json.substr(pos, end - pos);
}

std::string extractObj(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return "";
    pos = json.find(':', pos);
    if (pos == std::string::npos) return "";
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n' || json[pos] == '\r')) pos++;
    if (pos >= json.size() || json[pos] != '{') return "";
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

} // anonymous namespace

// Original Kotlin: parseRoomVersionsCapability()
// Parses the m.room_versions block from a /capabilities response.
// Expected JSON format:
// {
//   "capabilities": {
//     "m.room_versions": {
//       "default": "10",
//       "available": { "1": "stable", "10": "stable", ... }
//     }
//   }
// }
RoomVersionCapability parseRoomVersionsCapability(const std::string& capabilitiesJson) {
    RoomVersionCapability cap;

    // Extract the m.room_versions object
    std::string roomVersionsObj = extractObj(capabilitiesJson, "m.room_versions");
    if (roomVersionsObj.empty()) {
        // Try nested under "capabilities"
        auto capsObj = extractObj(capabilitiesJson, "capabilities");
        if (!capsObj.empty()) {
            roomVersionsObj = extractObj(capsObj, "m.room_versions");
        }
    }

    if (roomVersionsObj.empty()) return cap;

    // Extract default version
    cap.defaultVersion = jExtractStr(roomVersionsObj, "default");

    // Extract available versions map
    auto availableObj = extractObj(roomVersionsObj, "available");
    if (!availableObj.empty()) {
        // Parse key-value pairs: {"1": "stable", "10": "stable", ...}
        size_t pos = 1; // skip opening {
        while (pos < availableObj.size()) {
            // Skip whitespace and commas
            while (pos < availableObj.size() &&
                   (availableObj[pos] == ' ' || availableObj[pos] == '\t' ||
                    availableObj[pos] == '\n' || availableObj[pos] == '\r' ||
                    availableObj[pos] == ',')) {
                pos++;
            }
            if (pos >= availableObj.size() || availableObj[pos] == '}') break;

            if (availableObj[pos] == '"') {
                pos++; // skip opening quote
                size_t keyEnd = pos;
                while (keyEnd < availableObj.size() && availableObj[keyEnd] != '"') {
                    if (availableObj[keyEnd] == '\\') keyEnd++;
                    keyEnd++;
                }
                std::string versionKey = availableObj.substr(pos, keyEnd - pos);
                pos = keyEnd + 1;

                // Find colon
                while (pos < availableObj.size() && availableObj[pos] != ':') pos++;
                pos++;
                while (pos < availableObj.size() &&
                       (availableObj[pos] == ' ' || availableObj[pos] == '\t')) pos++;

                std::string statusVal;
                if (pos < availableObj.size() && availableObj[pos] == '"') {
                    pos++;
                    while (pos < availableObj.size() && availableObj[pos] != '"') {
                        if (availableObj[pos] == '\\') pos++;
                        statusVal += availableObj[pos];
                        pos++;
                    }
                    pos++; // skip closing quote
                }
                cap.available[versionKey] = statusVal;
            } else {
                pos++;
            }
        }
    }

    // Check if default is stable
    if (!cap.defaultVersion.empty()) {
        auto defIt = cap.available.find(cap.defaultVersion);
        if (defIt != cap.available.end() && defIt->second == "stable") {
            cap.isDefaultStable = true;
        }
    }

    return cap;
}

} // namespace progressive
