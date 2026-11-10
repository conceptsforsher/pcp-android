#pragma once
#include <string>
#include <cstdint>

std::string parseCrossSigningKey(const std::string& json);
std::string buildSelfSigningEvent(const std::string& json);
std::string verifyUserSignature(const std::string& json);
std::string crossSignCheck(const std::string& json);
std::string getMasterKey(const std::string& json);
