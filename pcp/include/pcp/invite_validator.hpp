#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string validateInvite(const std::string& json);
std::string isExpired(const std::string& json);
std::string canAccept(const std::string& json);
std::string getInviteDetails(const std::string& json);
std::string formatInviteSummary(const std::string& json);
