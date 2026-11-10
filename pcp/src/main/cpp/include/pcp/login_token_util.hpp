#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string parseLoginToken(const std::string& json);
std::string validateTokenExpiry(const std::string& json);
std::string buildTokenLogin(const std::string& json);
std::string getTokenUser(const std::string& json);
std::string formatTokenError(const std::string& json);
