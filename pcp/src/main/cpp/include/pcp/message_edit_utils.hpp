#pragma once
#include <string>
#include <cstdint>

std::string originalEventId(const std::string& json);
std::string newBody(const std::string& json);
std::string newFormattedBody(const std::string& json);
std::string fallbackText(const std::string& json);
std::string buildEditRelation(const std::string& json);
std::string string(const std::string& json);
std::string buildEditContent(const std::string& json);
std::string buildEditFallback(const std::string& json);
std::string string(const std::string& json);
std::string string(const std::string& json);
std::string EditInfo(const std::string& json);
std::string bool(const std::string& json);
std::string getEditOriginalEventId(const std::string& json);
std::string formatEditHistory(const std::string& json);
std::string buildUnsendContent(const std::string& json);
std::string string(const std::string& json);
std::string string(const std::string& json);
std::string getLatestEditEventId(const std::string& json);
std::string string(const std::string& json);

