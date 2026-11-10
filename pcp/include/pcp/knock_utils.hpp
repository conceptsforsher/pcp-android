#pragma once
#include <string>
#include <cstdint>

std::string parseKnockEvent(const std::string& json);
std::string buildKnockRequest(const std::string& json);
std::string checkKnockPermission(const std::string& json);
std::string formatKnockMessage(const std::string& json);
std::string getKnockRules(const std::string& json);
