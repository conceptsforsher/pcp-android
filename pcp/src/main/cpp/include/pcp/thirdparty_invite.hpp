#pragma once
#include <string>
#include <cstdint>

std::string parseThirdpartyInvite(const std::string& json);
std::string acceptThirdparty(const std::string& json);
std::string rejectThirdparty(const std::string& json);
std::string isValidThirdparty(const std::string& json);
std::string formatThirdpartyNotice(const std::string& json);
