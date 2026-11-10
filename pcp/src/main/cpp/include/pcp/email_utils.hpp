#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string parseEmail(const std::string& json);
std::string validateEmail(const std::string& json);
std::string buildEmailAuth(const std::string& json);
std::string sendVerificationEmail(const std::string& json);
std::string formatEmailNotice(const std::string& json);
