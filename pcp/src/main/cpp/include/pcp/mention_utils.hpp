#pragma once
#include <string>
#include <cstdint>

std::string extractMentions(const std::string& text);
std::string buildMentionHtml(const std::string& userId, const std::string& displayName);
bool hasRoomMention(const std::string& text);
