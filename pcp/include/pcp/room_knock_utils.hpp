#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string buildKnockRequest(const std::string& roomId, const std::vector<std::string>& via={}, const std::string& reason="");
std::string formatKnockResponse(const std::string& json);
