#pragma once
#include <string>
#include <cstdint>

std::string buildFilter(const std::string& json);
std::string getLazyMembers(const std::string& json);
std::string isLazyLoaded(const std::string& json);
std::string optimizeMemberQuery(const std::string& json);
std::string formatLazyStatus(const std::string& json);
