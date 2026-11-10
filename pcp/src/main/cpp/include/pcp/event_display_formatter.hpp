#pragma once
#include <string>
#include <cstdint>

std::string eventId(const std::string& json);
std::string senderId(const std::string& json);
std::string senderName(const std::string& json);
std::string body(const std::string& json);
std::string formattedBody(const std::string& json);
std::string replyToEventId(const std::string& json);
std::string encryptionInfo(const std::string& json);
std::string topReaction(const std::string& json);
std::string EventDisplayType(const std::string& json);
std::string string(const std::string& json);
std::string EventDisplayInfo(const std::string& json);
std::string string(const std::string& json);
std::string string(const std::string& json);
std::string formatSenderDisplay(const std::string& json);
std::string string(const std::string& json);
std::string string(const std::string& json);
std::string formatEventBubbleTime(const std::string& json);
std::string formatEditedNotice(const std::string& json);
std::string formatReplyPreview(const std::string& json);
std::string string(const std::string& json);
std::string string(const std::string& json);
std::string getEventTypeIcon(const std::string& json);

