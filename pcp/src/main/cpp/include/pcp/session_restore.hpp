#pragma once
#include <string>
#include <vector>
#include <cstdint>

namespace pcp {

struct SessionData {
    std::string userId;
    std::string accessToken;
    std::string deviceId;
    std::string homeServer;
    std::string refreshToken;
    int64_t lastActiveMs = 0;
};

// Serialize session data for storage
std::string serializeSession(const SessionData& data);

// Deserialize session data
SessionData deserializeSession(const std::string& json);

// Check if session is expired (token invalid, needs re-auth)
bool isSessionExpired(const std::string& errorResponse);

// Extract session data from login response
SessionData extractSessionFromLogin(const std::string& loginResponseJson);

// Build session storage key
std::string buildSessionKey(const std::string& userId, const std::string& deviceId);

// Format session info for display
std::string formatSessionInfo(const SessionData& data);

} // namespace pcp
