#ifndef PROGRESSIVE_COLOR_UTILS_HPP
#define PROGRESSIVE_COLOR_UTILS_HPP

#include <string>

namespace progressive {

struct RgbaColor {
    int r = 0, g = 0, b = 0, a = 255;
    bool valid = false;

    std::string toHex() const;
    double relativeLuminance() const;
};

// Parse color strings: "#RGB", "#RRGGBB", "#AARRGGBB", "rgb(r,g,b)", "rgba(r,g,b,a)"
RgbaColor parseColor(const std::string& input);

// Calculate contrast ratio between two colors (WCAG 2.1).
// Returns value between 1.0 (same color) and 21.0 (black/white).
double contrastRatio(const RgbaColor& fg, const RgbaColor& bg);

// Check if contrast meets WCAG AA for normal text (4.5:1).
bool isWcagAaNormal(const RgbaColor& fg, const RgbaColor& bg);

// Check if contrast meets WCAG AA for large text (3:1).
bool isWcagAaLarge(const RgbaColor& fg, const RgbaColor& bg);

// Check if contrast meets WCAG AAA for normal text (7:1).
bool isWcagAaaNormal(const RgbaColor& fg, const RgbaColor& bg);

// Get WCAG rating: "AAA", "AA", "AA Large", "FAIL".
std::string getWcagRating(const RgbaColor& fg, const RgbaColor& bg, bool isLargeText = false);

// Suggest a text color (black or white) for a given background.
RgbaColor suggestTextColor(const RgbaColor& bg);

// Blend two colors with alpha.
RgbaColor blendColors(const RgbaColor& bg, const RgbaColor& fg);

// Lighten a color by a factor (0.0 = no change, 1.0 = white).
RgbaColor lighten(const RgbaColor& color, double factor);

// Darken a color by a factor (0.0 = no change, 1.0 = black).
RgbaColor darken(const RgbaColor& color, double factor);

// Convert HSL to RGB.
RgbaColor hslToRgb(double h, double s, double l);

} // namespace progressive

#endif // PROGRESSIVE_COLOR_UTILS_HPP
