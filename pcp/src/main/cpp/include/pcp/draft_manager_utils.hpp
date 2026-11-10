#pragma once
#include <string>
#include <cstdint>

std::string roomId(const std::string& json);
std::string body(const std::string& json);
std::string formattedBody(const std::string& json);
std::string replyEventId(const std::string& json);
std::string threadRootEventId(const std::string& json);
std::string buildDraftKey(const std::string& json);
std::string string(const std::string& json);
std::string serializeDraft(const std::string& json);
std::string MessageDraft(const std::string& json);
std::string string(const std::string& json);
std::string string(const std::string& json);
std::string formatDraftPreview(const std::string& json);
std::string buildClearDraftContent(const std::string& json);

