#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string parseAttachment(const std::string& json);
std::string getThumbnailUrl(const std::string& json);
std::string isEncryptedAttachment(const std::string& json);
std::string decryptAttachment(const std::string& json);
std::string buildAttachmentEvent(const std::string& json);
