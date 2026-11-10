#pragma once
#include <string>
#include <cstdint>

std::string parseOpenIdToken(const std::string& json);
std::string buildOpenIdRequest(const std::string& json);
std::string validateToken(const std::string& json);
std::string getMatrixId(const std::string& json);
std::string formatTokenInfo(const std::string& json);
