#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string parseTopic(const std::string& json);
std::string extractTopicLinks(const std::string& json);
std::string formatTopicHtml(const std::string& json);
std::string truncateTopic(const std::string& json);
std::string getTopicFallback(const std::string& json);
