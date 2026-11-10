#pragma once
#include <string>
#include <cstdint>

std::string parseIgnoreList(const std::string& json);
std::string buildIgnoreEvent(const std::string& json);
std::string checkUserIgnored(const std::string& json);
std::string formatIgnoredNotice(const std::string& json);
std::string mergeIgnoreLists(const std::string& json);
