#include "text_drawable.hpp"
#include <algorithm>
#include <cmath>
#include <android/log.h>

#define TD_LOG_TAG "TextDrawable"
#define TD_LOGI(...) __android_log_print(ANDROID_LOG_INFO, TD_LOG_TAG, __VA_ARGS__)

namespace pcp::textdrawable {

// Material Design color palette for avatars
std::vector<ColorConfig> TextDrawable::s_palette = {
    {244, 67, 54},    // Red
    {233, 30, 99},    // Pink
    {156, 39, 176},    // Purple
    {103, 58, 183},    // Deep Purple
    {63, 81, 181},     // Indigo
    {33, 150, 243},    // Blue
    {3, 169, 244},     // Light Blue
    {0, 188, 212},     // Cyan
    {0, 150, 136},     // Teal
    {76, 175, 80},     // Green
    {139, 195, 74},    // Light Green
    {205, 220, 57},    // Lime
    {255, 235, 59},    // Yellow
    {255, 193, 7},     // Amber
    {255, 152, 0},     // Orange
    {255, 87, 34},     // Deep Orange
    {121, 85, 72},     // Brown
    {158, 158, 158},   // Grey
    {96, 125, 139},    // Blue Grey
};

TextDrawable::TextDrawable() = default;
TextDrawable::~TextDrawable() = default;

std::string TextDrawable::extractInitials(const std::string& name) const {
    if (name.empty()) return "?";

    // Handle display names like "John Doe" -> "JD"
    // Also handle emoji-only names, etc.
    std::string initials;
    bool newWord = true;

    for (size_t i = 0; i < name.size() && initials.size() < 2; i++) {
        char c = name[i];
        if (c == ' ' || c == '-' || c == '_' || c == '.') {
            newWord = true;
        } else if (newWord && isalnum(static_cast<unsigned char>(c))) {
            initials += static_cast<char>(toupper(static_cast<unsigned char>(c)));
            newWord = false;
        }
    }

    // Fallback: first character
    if (initials.empty() && !name.empty()) {
        char c = name[0];
        if (isalnum(static_cast<unsigned char>(c))) {
            initials = static_cast<char>(toupper(static_cast<unsigned char>(c)));
        } else {
            initials = "#";
        }
    }

    return initials;
}

ColorConfig TextDrawable::colorForName(const std::string& name) const {
    if (s_palette.empty()) return {128, 128, 128};

    // Simple hash-based color selection
    size_t hash = std::hash<std::string>{}(name);
    return s_palette[hash % s_palette.size()];
}

std::vector<uint8_t> TextDrawable::generate(const std::string& text,
                                              const DrawableConfig& config) {
    DrawableConfig cfg = config.width == 0 ? m_config : config;
    if (cfg.width == 0) cfg.width = 128;
    if (cfg.height == 0) cfg.height = 128;
    if (cfg.fontSize == 0) cfg.fontSize = 56;

    // Create RGBA bitmap
    size_t pixelCount = cfg.width * cfg.height;
    std::vector<uint8_t> bitmap(pixelCount * 4, 255);

    // Fill background
    for (size_t i = 0; i < pixelCount; i++) {
        bitmap[i * 4 + 0] = cfg.background.r;
        bitmap[i * 4 + 1] = cfg.background.g;
        bitmap[i * 4 + 2] = cfg.background.b;
        bitmap[i * 4 + 3] = cfg.background.a;
    }

    // Draw rounded rect if needed
    if (cfg.borderRadius > 0.0f) {
        float radius = cfg.borderRadius * std::min(cfg.width, cfg.height);
        drawRoundedRect(bitmap, 0, 0, cfg.width, cfg.height, radius, cfg.background);
    }

    // Draw text centered
    drawText(bitmap, text, cfg.fontSize, cfg.foreground);

    return bitmap;
}

std::vector<uint8_t> TextDrawable::generateAvatar(const std::string& name,
                                                     const DrawableConfig& config) {
    std::string initials = extractInitials(name);
    ColorConfig bg = colorForName(name);
    DrawableConfig cfg = config.width == 0 ? m_config : config;
    cfg.background = bg;
    cfg.foreground = {255, 255, 255, 255};
    cfg.borderRadius = 0.5f; // Circle by default
    return generate(initials, cfg);
}

std::vector<uint8_t> TextDrawable::generateGroupAvatar(const std::vector<std::string>& names,
                                                          const DrawableConfig& config) {
    // For group avatars, show first 4 initials in a 2x2 grid
    DrawableConfig cfg = config.width == 0 ? m_config : config;
    cfg.borderRadius = 0.5f;
    ColorConfig bg = colorForName(names.empty() ? "group" : names[0]);
    cfg.background = bg;
    cfg.foreground = {255, 255, 255, 255};

    std::string text;
    for (size_t i = 0; i < names.size() && i < 4; i++) {
        text += extractInitials(names[i]);
    }
    return generate(text, cfg);
}

bool TextDrawable::saveToFile(const std::string& path,
                                const std::vector<uint8_t>& imageData) {
    // Save as PNG (would need libpng or stb_image_write)
    return false;
}

void TextDrawable::drawCircle(std::vector<uint8_t>& bitmap, int cx, int cy,
                                int radius, ColorConfig color) {
    // Bresenham circle algorithm (stub)
    (void)bitmap; (void)cx; (void)cy; (void)radius; (void)color;
}

void TextDrawable::drawRoundedRect(std::vector<uint8_t>& bitmap, int x, int y,
                                     int w, int h, float radius, ColorConfig color) {
    (void)bitmap; (void)x; (void)y; (void)w; (void)h; (void)radius; (void)color;
}

void TextDrawable::drawText(std::vector<uint8_t>& bitmap, const std::string& text,
                              int fontSize, ColorConfig color) {
    // Would use FreeType or system font rendering
    (void)bitmap; (void)text; (void)fontSize; (void)color;
}

} // namespace pcp::textdrawable
