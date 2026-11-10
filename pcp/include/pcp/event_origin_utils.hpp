#pragma once
#include <string>
#include <cstdint>

std::string parseOrigin(const std::string& json);
std::string parseOriginServerTimestamp(const std::string& json);
std::string validateOrigin(const std::string& json);
