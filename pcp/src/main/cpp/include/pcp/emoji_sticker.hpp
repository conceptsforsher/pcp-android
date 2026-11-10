#pragma once
#include <string>
#include <cstdint>

std::string parseStickerPack(const std::string& json);
std::string getStickerUrl(const std::string& json);
std::string isAnimatedSticker(const std::string& json);
std::string searchStickers(const std::string& json);
std::string formatStickerHtml(const std::string& json);
