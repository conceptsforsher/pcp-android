#pragma once
#include <string>
#include <cstdint>

std::string parseReadMarker(const std::string& json);
std::string updateReadMarker(const std::string& json);
std::string buildReadMarkerEvent(const std::string& json);
