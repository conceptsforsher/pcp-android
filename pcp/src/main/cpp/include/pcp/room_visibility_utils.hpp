#pragma once
#include <string>
#include <cstdint>

std::string parseVisibility(const std::string& json);
std::string isVisibleRoom(const std::string& json);
std::string buildVisibilityEvent(const std::string& json);
