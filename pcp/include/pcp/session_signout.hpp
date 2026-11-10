#pragma once
#include <string>
#include <cstdint>

std::string signOutSession(const std::string& json);
std::string clearLocalData(const std::string& json);
std::string getSessionList(const std::string& json);
std::string isCurrentSession(const std::string& json);
std::string formatSignoutConfirmation(const std::string& json);
