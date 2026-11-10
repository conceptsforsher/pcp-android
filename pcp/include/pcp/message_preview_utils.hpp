#pragma once
#include <string>
#include <cstdint>

std::string formatMessagePreview(const std::string& body, int maxLen = 80);
std::string formatSenderPrefix(const std::string& senderName, bool isOwnMessage);
std::string getMessagePreviewIcon(const std::string& msgType);
std::string buildImagePreview(const std::string& body);
std::string buildVideoPreview(const std::string& body);
std::string buildFilePreview(const std::string& body, const std::string& filename);
std::string buildAudioPreview(const std::string& body);
std::string buildStickerPreview(const std::string& body);
std::string truncatePreview(const std::string& text, int maxLen = 80);
