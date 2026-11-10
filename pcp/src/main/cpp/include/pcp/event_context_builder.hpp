#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string buildEventContext(const std::string& json);
std::string getParentEvent(const std::string& json);
std::string getChildEvents(const std::string& json);
std::string isThreadRoot(const std::string& json);
std::string formatContextBreadcrumb(const std::string& json);
