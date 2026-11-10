#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string translatePushBody(const std::string& json);
std::string formatPushTitle(const std::string& json);
std::string getLocalizedAction(const std::string& json);
std::string parsePushTemplate(const std::string& json);
std::string buildLocalizedPush(const std::string& json);
