#pragma once
#include <string>
#include <cstdint>

std::string sessionId;(const std::string& json);
std::string senderKey;(const std::string& json);
std::string claimedKey;(const std::string& json);
std::string OlmSessionInfo parseOlmSession(const std::string& json);
std::string formatOlmSession(const OlmSessionInfo& session);(const std::string& json);
std::string buildOlmSessionId(const std(const std::string& json);
std::string string& senderKey, const std(const std::string& json);
std::string string& chainIndex = "0");(const std::string& json);
std::string formatKeyFingerprint(const std(const std::string& json);
std::string string& key, int groupSize = 4);(const std::string& json);
std::string bool isValidOlmMessage(const std(const std::string& json);
std::string string& messageJson);(const std::string& json);
