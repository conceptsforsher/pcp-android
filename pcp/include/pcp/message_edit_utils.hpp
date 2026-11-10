#pragma once
#include <string>
#include <cstdint>

std::string originalEventId;  // event being edited(const std::string& json);
std::string newBody;(const std::string& json);
std::string newFormattedBody;(const std::string& json);
std::string fallbackText;     // " * edited message"(const std::string& json);
std::string buildEditRelation(const std(const std::string& json);
std::string string& originalEventId);(const std::string& json);
std::string buildEditContent(const EditInfo& info);(const std::string& json);
std::string buildEditFallback(const std(const std::string& json);
std::string string& newBody, const std(const std::string& json);
std::string string& originalBody);(const std::string& json);
std::string EditInfo parseEditInfo(const std::string& json);
std::string bool isEditEvent(const std::string& json);
std::string getEditOriginalEventId(const std::string& json);
std::string formatEditHistory(int editCount);(const std::string& json);
std::string buildUnsendContent(const std(const std::string& json);
std::string string& eventId, const std(const std::string& json);
std::string string& reason = "");(const std::string& json);
std::string getLatestEditEventId(const std(const std::string& json);
std::string string& aggregateJson);(const std::string& json);
