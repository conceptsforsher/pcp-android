#pragma once
#include <string>
namespace pcp {
int parseEventVersion(const std::string& json);
bool isEventVersionSupported(int version);
std::string getLatestEventVersion();
}
