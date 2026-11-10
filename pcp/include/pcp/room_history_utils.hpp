#pragma once
#include <string>
#include <cstdint>

bool canUserReadHistory(const std::string& visibility, const std::string& membership, bool wasInvited=false);
std::string getDefaultHistoryVisibility();
std::string formatHistoryVisibility(const std::string& vis);
