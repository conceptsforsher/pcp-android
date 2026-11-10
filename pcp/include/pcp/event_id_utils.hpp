#pragma once
#include <string>
#include <cstdint>

std::string validateEventId(const std::string& json);
std::string parseEventId(const std::string& json);
std::string buildEventId(const std::string& json);
std::string compareEventIds(const std::string& json);
