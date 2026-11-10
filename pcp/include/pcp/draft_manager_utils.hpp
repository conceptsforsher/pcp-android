#pragma once
#include <string>
#include <cstdint>

std::string roomId;(const std::string& json);
std::string body;(const std::string& json);
std::string formattedBody;(const std::string& json);
std::string replyEventId;        // m.in_reply_to eventId(const std::string& json);
std::string threadRootEventId;   // thread root(const std::string& json);
std::string buildDraftKey(const std(const std::string& json);
std::string string& roomId);(const std::string& json);
std::string serializeDraft(const MessageDraft& draft);(const std::string& json);
std::string MessageDraft deserializeDraft(const std(const std::string& json);
std::string string& json, const std(const std::string& json);
std::string string& roomId);(const std::string& json);
std::string formatDraftPreview(const MessageDraft& draft);(const std::string& json);
std::string buildClearDraftContent();(const std::string& json);
