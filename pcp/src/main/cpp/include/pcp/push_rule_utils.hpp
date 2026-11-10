#pragma once
#include <string>
#include <cstdint>

std::string parsePushRule(const std::string& json);
std::string matchPushRule(const std::string& json);
std::string buildPushRuleEvent(const std::string& json);
