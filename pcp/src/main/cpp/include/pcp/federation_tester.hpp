#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string testFederation(const std::string& json);
std::string parseServerVersion(const std::string& json);
std::string checkConnectivity(const std::string& json);
std::string getServerKeys(const std::string& json);
std::string formatServerDiagnostic(const std::string& json);
