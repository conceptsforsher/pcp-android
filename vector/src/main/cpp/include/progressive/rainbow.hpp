#ifndef PROGRESSIVE_RAINBOW_HPP
#define PROGRESSIVE_RAINBOW_HPP

#include <string>

namespace progressive {

struct RgbColor {
    int r = 0, g = 0, b = 0;
    std::string toHex() const;
};

// Generate rainbow-colored HTML from plain text.
// Each character gets a font color tag with a different hue from the CIELAB rainbow.
// Spaces are left uncolored (better than React-SDK).
// Emojis are treated as single grapheme clusters.
std::string generateRainbow(const std::string& text);

// CIELAB color space conversion used by the rainbow generator.
RgbColor labToRgb(int l, double a, double b);

// Generate CIELAB a,b coordinates from a hue angle.
std::pair<double, double> generateAB(double hue, float chroma = 1.0f);

// Convert a single character to a rainbow-colored HTML span.
std::string colorizeChar(const std::string& ch, int hueIndex, int totalChars);

} // namespace progressive

#endif // PROGRESSIVE_RAINBOW_HPP
