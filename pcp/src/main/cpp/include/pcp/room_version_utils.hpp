#pragma once
#include <string>
#include <vector>

namespace pcp {

struct RoomVersionInfo {
    std::string version;        // "1", "2", ..., "10", "11"
    bool isStable = true;
    bool supportsKnock = false;
    bool supportsRestricted = false;
    int maxPowerLevel = 100;
};

static const std::vector<std::string> STABLE_VERSIONS = {"1","2","3","4","5","6","7","8","9","10","11"};

// Get room version capabilities
RoomVersionInfo getVersionInfo(const std::string& version);
std::string getLatestStableVersion();

// Check if version supports feature
bool versionSupports(const std::string& version, const std::string& feature);
bool supportsKnockJoinRule(const std::string& version);
bool supportsRestrictedJoinRule(const std::string& version);
bool supportsEventRedaction(const std::string& version);

// Upgrade recommendation
std::string getRecommendedVersion(const std::string& currentVersion);
bool shouldUpgradeRoom(const std::string& currentVersion);

} // namespace pcp
