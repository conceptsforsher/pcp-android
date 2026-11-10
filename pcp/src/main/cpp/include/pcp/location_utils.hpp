#pragma once
#include <string>
#include <cstdint>

std::string parseGeoUri(const std::string& json);
std::string buildLocationEvent(const std::string& json);
std::string calculateDistance(const std::string& json);
std::string isValidGeoCoordinates(const std::string& json);
std::string renderStaticMapUrl(const std::string& json);
