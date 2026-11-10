#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string sendKnock(const std::string& json);
std::string parseKnockResponse(const std::string& json);
std::string canKnock(const std::string& json);
std::string getPendingKnocks(const std::string& json);
std::string cancelKnock(const std::string& json);
