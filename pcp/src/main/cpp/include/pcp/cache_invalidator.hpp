#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string invalidateByKey(const std::string& json);
std::string invalidateByPrefix(const std::string& json);
std::string isStale(const std::string& json);
std::string getTtl(const std::string& json);
std::string resetCache(const std::string& json);
