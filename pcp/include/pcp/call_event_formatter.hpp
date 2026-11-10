#pragma once
#include <string>
#include <cstdint>

std::string callId;(const std::string& json);
std::string callerName;(const std::string& json);
std::string formattedText;      // display text(const std::string& json);
std::string iconName;           // UI icon(const std::string& json);
std::string CallEventType parseCallEventType(const std::string& json);
std::string CallEventDisplay formatCallEvent(const std(const std::string& json);
std::string string& json, const std(const std::string& json);
std::string string& callerName,(const std::string& json);
std::string const std(const std::string& json);
std::string string& myUserId);(const std::string& json);
std::string formatCallDuration(int seconds);(const std::string& json);
std::string formatCallEventText(CallEventType type, const std(const std::string& json);
std::string string& caller, bool isVideo,(const std::string& json);
std::string getCallEventIcon(CallEventType type, bool isVideo);(const std::string& json);
std::string bool isCallEvent(const std::string& json);
