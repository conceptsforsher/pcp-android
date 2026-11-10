#pragma once
#include <string>
#include <cstdint>

std::string verifyEventSignature(const std::string& json);
std::string signEvent(const std::string& json);
std::string parseSignatures(const std::string& json);
