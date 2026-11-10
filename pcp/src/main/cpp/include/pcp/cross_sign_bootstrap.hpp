#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string bootstrapCrossSign(const std::string& json);
std::string getBootstrapStatus(const std::string& json);
std::string isBootstrapped(const std::string& json);
std::string resetCrossSign(const std::string& json);
std::string formatBootstrapStep(const std::string& json);
