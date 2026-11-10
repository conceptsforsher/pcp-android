#pragma once
#include <string>
#include <cstdint>

std::string eventId;(const std::string& json);
std::string senderId;(const std::string& json);
std::string senderName;(const std::string& json);
std::string body;           // plain text body(const std::string& json);
std::string formattedBody;  // HTML body(const std::string& json);
std::string replyToEventId;(const std::string& json);
std::string encryptionInfo;(const std::string& json);
std::string topReaction;    // most used emoji(const std::string& json);
std::string EventDisplayType detectEventType(const std(const std::string& json);
std::string string& eventJson);(const std::string& json);
std::string EventDisplayInfo formatEventDisplay(const std(const std::string& json);
std::string string& eventJson, const std(const std::string& json);
std::string string& senderName,(const std::string& json);
std::string formatSenderDisplay(const std(const std::string& json);
std::string string& name, const std(const std::string& json);
std::string string& userId,(const std::string& json);
std::string formatEventBubbleTime(int64_t timestampMs);(const std::string& json);
std::string formatEditedNotice();(const std::string& json);
std::string formatReplyPreview(const std(const std::string& json);
std::string string& originalBody, const std(const std::string& json);
std::string string& originalSender);(const std::string& json);
std::string getEventTypeIcon(EventDisplayType type);(const std::string& json);
