#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string parseWidgetConfig(const std::string& json);
std::string getWidgetUrl(const std::string& json);
std::string validateWidgetDomain(const std::string& json);
std::string sendWidgetMessage(const std::string& json);
std::string formatWidgetLoad(const std::string& json);
