#pragma once
#include <string>
#include <cstdint>

std::string parseThreadInfo(const std::string& json);
std::string buildThreadRoot(const std::string& json);
std::string getThreadParent(const std::string& json);
std::string countThreadMessages(const std::string& json);
std::string sortThreadsByActivity(const std::string& json);
