#pragma once
#include <string>
#include <cstdint>

std::string parseReportEvent(const std::string& json);
std::string buildReportBody(const std::string& json);
std::string getReportReason(const std::string& json);
std::string validateServerReport(const std::string& json);
std::string formatReportConfirm(const std::string& json);
