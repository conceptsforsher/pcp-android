#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string parseVerificationRequest(const std::string& json);
std::string acceptRequest(const std::string& json);
std::string declineRequest(const std::string& json);
std::string getRequestStatus(const std::string& json);
std::string formatRequestBanner(const std::string& json);
