#pragma once
#include <string>
namespace pcp {
struct WhoisResult { std::string userId; std::string displayName; std::string avatarUrl; bool isActive=false; int64_t lastSeenMs=0; };
std::string buildWhoisRequest(const std::string& userId);
WhoisResult parseWhoisResponse(const std::string& json, const std::string& userId);
std::string formatWhoisResult(const WhoisResult& w);
}
