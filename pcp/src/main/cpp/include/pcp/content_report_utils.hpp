#pragma once
#include <string>
#include <cstdint>

std::string buildReport(const std::string& json);
std::string parseReportResponse(const std::string& json);
std::string validateReportReason(const std::string& json);
std::string getReportCategories(const std::string& json);
std::string formatReportNotice(const std::string& json);
