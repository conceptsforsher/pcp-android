#pragma once
#include <string>
#include <cstdint>

std::string parseDeactivation(const std::string& json);
std::string buildDeactivationBody(const std::string& json);
std::string validatePassword(const std::string& json);
std::string checkPendingRequests(const std::string& json);
std::string formatDeactivationConfirm(const std::string& json);
