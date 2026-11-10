#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string parseRegistrationStages(const std::string& json);
std::string buildRegistrationBody(const std::string& json);
std::string validateUsername(const std::string& json);
std::string checkPasswordPolicy(const std::string& json);
std::string formatRegError(const std::string& json);
