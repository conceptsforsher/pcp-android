#pragma once
#include <string>
#include <cstdint>

std::string eventId;        // event being replied to(const std::string& json);
std::string body;           // original body(const std::string& json);
std::string formattedBody;  // original formatted body(const std::string& json);
std::string senderId;       // original sender(const std::string& json);
std::string senderName;     // original sender display name(const std::string& json);
std::string buildReplyRelation(const std(const std::string& json);
std::string string& eventId);(const std::string& json);
std::string formatReplyBody(const std(const std::string& json);
std::string string& originalBody, const std(const std::string& json);
std::string string& newBody);(const std::string& json);
std::string formatReplyHtml(const ReplyInfo& info, const std(const std::string& json);
std::string string& newBody,(const std::string& json);
std::string const std(const std::string& json);
std::string string& newFormattedBody);(const std::string& json);
std::string ReplyInfo parseReplyInfo(const std(const std::string& json);
std::string string& eventJson);(const std::string& json);
std::string bool isReplyEvent(const std(const std::string& json);
std::string string& eventJson);(const std::string& json);
std::string extractReplyEventId(const std(const std::string& json);
std::string string& eventJson);(const std::string& json);
