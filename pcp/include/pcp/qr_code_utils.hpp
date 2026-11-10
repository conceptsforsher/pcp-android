#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string parseQrContent(const std::string& json);
std::string buildQrPayload(const std::string& json);
std::string verifyQrSignature(const std::string& json);
std::string isValidQrFormat(const std::string& json);
std::string formatQrHtml(const std::string& json);
