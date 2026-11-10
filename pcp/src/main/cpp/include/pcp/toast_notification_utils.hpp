#pragma once
#include <string>

namespace pcp {
std::string buildToastMessage(const std::string& title, const std::string& body);
std::string buildDelayedToast(int delayMs, const std::string& message);
std::string formatToastTime(int delayMs);
}
