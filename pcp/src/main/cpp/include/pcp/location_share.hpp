#pragma once
#include <string>
#include <cstdint>

std::string parseLocationEvent(const std::string& json);
std::string isLiveLocation(const std::string& json);
std::string getExpiryTimestamp(const std::string& json);
std::string buildLocationUpdate(const std::string& json);
std::string formatLocationPin(const std::string& json);
