#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string shareSecret(const std::string& json);
std::string requestSecret(const std::string& json);
std::string cancelSecretRequest(const std::string& json);
std::string parseSecretEvent(const std::string& json);
std::string formatSecretNotice(const std::string& json);
