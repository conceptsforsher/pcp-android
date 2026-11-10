#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string countPendingInvites(const std::string& json);
std::string getOldestInvite(const std::string& json);
std::string hasInviteFrom(const std::string& json);
std::string formatInviteSummary(const std::string& json);
std::string clearInviteCache(const std::string& json);
