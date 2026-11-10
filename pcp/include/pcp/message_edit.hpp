#pragma once
#include <string>
#include <cstdint>

std::string canEdit(const std::string& json);
std::string parseEditInfo(const std::string& json);
std::string getEditDiff(const std::string& json);
std::string formatEditNotice(const std::string& json);
std::string getLatestEdit(const std::string& json);
