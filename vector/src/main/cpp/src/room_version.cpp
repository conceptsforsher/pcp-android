#include "progressive/room_version.hpp"
#include <sstream>

namespace progressive {

std::vector<RoomVersion> getKnownRoomVersions() {
    std::vector<RoomVersion> versions = {
        {"1",  "v1  - Original", false, false},
        {"2",  "v2  - State resolution v2", false, false},
        {"3",  "v3  - Event IDs as hashes", false, false},
        {"4",  "v4  - State resolution v2 + event IDs as hashes", false, false},
        {"5",  "v5  - Knock + restrictions", false, false},
        {"6",  "v6  - MSC2176 implicit PL", false, true},
        {"7",  "v7  - MSC2175 ignore PL invite", false, false},
        {"8",  "v8  - MSC2403 add reason to knock", false, false},
        {"9",  "v9  - MSC2174 implicit profiles", false, true},
        {"10", "v10 - MSC2174 + MSC2176 + MSC2175", false, true},
        {"11", "v11 - MSC3820 room version 11", false, true},
    };
    return versions;
}

bool isValidRoomVersion(const std::string& version) {
    for (const auto& v : getKnownRoomVersions()) {
        if (v.id == version) return true;
    }
    return false;
}

std::string getDefaultRoomVersion() {
    return "10"; // Current Element default
}

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

} // namespace progressive
