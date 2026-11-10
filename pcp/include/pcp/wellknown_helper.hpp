#pragma once
#include <string>

namespace pcp {

struct WellKnownResult {
    std::string baseUrl;            // homeserver base URL
    std::string identityServer;     // identity server URL
    bool isValid = false;
    std::string error;
};

// Parse .well-known response
WellKnownResult parseWellKnown(const std::string& json);

// Build .well-known request URL
std::string buildWellKnownUrl(const std::string& domain);

// Check if server supports a feature from .well-known
bool supportsFeature(const std::string& json, const std::string& feature);

// Extract homeserver URL from MXID
std::string extractServerFromMxid(const std::string& mxid);

// Build login URL from well-known
std::string buildLoginUrl(const WellKnownResult& wellKnown);

} // namespace pcp
