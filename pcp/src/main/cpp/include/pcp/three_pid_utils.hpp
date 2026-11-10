#pragma once
#include <string>
#include <vector>
namespace pcp {
struct ThreePid { std::string medium; std::string address; bool validated=false; int64_t addedAtMs=0; };
std::string buildEmailRequest(const std::string& email, const std::string& clientSecret, int sendAttempt);
std::string buildPhoneRequest(const std::string& phone, const std::string& clientSecret, int sendAttempt);
ThreePid parseThreePid(const std::string& json);
bool isEmailBound(const std::string& json, const std::string& email);
}
