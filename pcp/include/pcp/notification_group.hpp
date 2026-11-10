#pragma once
#include <string>
#include <cstdint>

std::string groupByRoom(const std::string& json);
std::string isGrouped(const std::string& json);
std::string getGroupSummary(const std::string& json);
std::string shouldGroupMore(const std::string& json);
std::string formatGroupNotification(const std::string& json);
