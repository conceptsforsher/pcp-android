#pragma once
#include <string>
#include <cstdint>

namespace pcp {

struct SessionInfo {
    std::string userId;
    std::string deviceId;
    std::string accessToken;
    std::string homeServer;
    bool isTokenValid = true;
    int64_t lastSyncMs = 0;
};

// Build whoami request
std::string buildWhoamiRequest();

// Parse whoami response
SessionInfo parseWhoamiResponse(const std::string& json);

// Format session info for debug
std::string formatSessionDebug(const SessionInfo& info);

} // namespace pcp
