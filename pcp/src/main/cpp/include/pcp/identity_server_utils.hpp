#pragma once
#include <string>
#include <cstdint>

std::string buildIdServerBindRequest(const std::string& sid, const std::string& clientSecret, const std::string& mxid);
std::string buildIdServerLookupRequest(const std::string& address, const std::string& medium);
