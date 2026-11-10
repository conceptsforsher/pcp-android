#pragma once
#include <string>
#include <cstdint>

std::string buildPusherRequest(const std::string& pushKey, const std::string& appId, const std::string& appName, const std::string& deviceName, const std::string& profileTag, const std::string& lang, const std::string& url, const std::string& format="event_id_only");
std::string parsePusherResponse(const std::string& json);
