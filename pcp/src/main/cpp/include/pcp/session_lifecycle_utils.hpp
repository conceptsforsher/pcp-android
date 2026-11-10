#pragma once
#include <string>
#include <cstdint>
namespace pcp {
std::string buildLogoutRequest();
std::string buildLogoutAllRequest();
std::string parseSessionState(const std::string& json);
bool isSessionSoftLoggedOut(const std::string& errorJson);
}
