#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>

namespace pcp::textdrawable {

struct ColorConfig {
    uint8_t r = 128, g = 128, b = 128, a = 255;
};

struct DrawableConfig {
    int width = 128;
    int height = 128;
    int fontSize = 56;
    float borderRadius = 0.0f; // 0 = square, 0.5 = circle
    bool bold = true;
    ColorConfig foreground;
    ColorConfig background;
};

class TextDrawable {
public:
    TextDrawable();
    ~TextDrawable();

    void setConfig(const DrawableConfig& config) { m_config = config; }
    const DrawableConfig& config() const { return m_config; }

    std::string extractInitials(const std::string& name) const;
    ColorConfig colorForName(const std::string& name) const;

    std::vector<uint8_t> generate(const std::string& text,
                                    const DrawableConfig& config = {});
    std::vector<uint8_t> generateAvatar(const std::string& name,
                                          const DrawableConfig& config = {});
    std::vector<uint8_t> generateGroupAvatar(const std::vector<std::string>& names,
                                               const DrawableConfig& config = {});

    bool saveToFile(const std::string& path, const std::vector<uint8_t>& imageData);

private:
    DrawableConfig m_config;

    static std::vector<ColorConfig> s_palette;
    void drawCircle(std::vector<uint8_t>& bitmap, int cx, int cy, int radius, ColorConfig color);
    void drawRoundedRect(std::vector<uint8_t>& bitmap, int x, int y, int w, int h,
                          float radius, ColorConfig color);
    void drawText(std::vector<uint8_t>& bitmap, const std::string& text,
                  int fontSize, ColorConfig color);
};

} // namespace pcp::textdrawable
