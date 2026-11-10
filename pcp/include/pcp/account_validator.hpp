#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string validateEmail(const std::string& json);
std::string validatePhone(const std::string& json);
std::string checkMXID(const std::string& json);
std::string isValidHomeserver(const std::string& json);
std::string getValidationErrors(const std::string& json);
