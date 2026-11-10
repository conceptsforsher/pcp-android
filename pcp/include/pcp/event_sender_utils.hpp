#pragma once
#include <string>
#include <cstdint>

std::string getSenderDisplayName(const std::string& eventJson, const std::string& fallback="Unknown");
bool isOwnEvent(const std::string& senderId, const std::string& myUserId);
std::string formatSenderAvatarUrl(const std::string& mxcUrl, const std::string& homeserver, int size=32);
