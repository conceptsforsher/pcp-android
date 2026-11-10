#pragma once
#include <string>
#include <cstdint>

std::string validateWidgetUrl(const std::string& json);
std::string isAuthorizedDomain(const std::string& json);
std::string parseWidgetParams(const std::string& json);
std::string sanitizeWidgetHtml(const std::string& json);
std::string getWidgetCapabilities(const std::string& json);
