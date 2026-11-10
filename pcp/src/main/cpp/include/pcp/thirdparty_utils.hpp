#pragma once
#include <string>
#include <cstdint>

std::string parseProtocolConfig(const std::string& json);
std::string buildProtocolLookup(const std::string& json);
std::string parseLocationResponse(const std::string& json);
std::string mergeProtocolResults(const std::string& json);
