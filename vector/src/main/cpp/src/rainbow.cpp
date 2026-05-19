#include "progressive/rainbow.hpp"
#include <sstream>
#include <cmath>
#include <iomanip>

namespace progressive {

std::string RgbColor::toHex() const {
    std::ostringstream out;
    out << "#"
        << std::hex << std::setfill('0') << std::setw(2) << r
        << std::setw(2) << g
        << std::setw(2) << b;
    return out.str();
}

static double adjustXYZ(double value) {
    if (value > 0.2069) return value * value * value;
    return 0.1284 * value - 0.01771;
}

static double gammaCorrection(double value) {
    if (value <= 0.0031308) return 12.92 * value;
    return 1.055 * std::pow(value, 1.0 / 2.4) - 0.055;
}

static int adjustRGB(double value) {
    double clipped = std::max(0.0, std::min(1.0, gammaCorrection(value)));
    return static_cast<int>(clipped * 255.0 + 0.5);
}

RgbColor labToRgb(int l, double a, double b) {
    double y = (l + 16.0) / 116.0;
    double x = adjustXYZ(y + a / 500.0) * 0.9505;
    double z = adjustXYZ(y - b / 200.0) * 1.0890;
    y = adjustXYZ(y);

    double red   = 3.24096994 * x - 1.53738318 * y - 0.49861076 * z;
    double green = -0.96924364 * x + 1.8759675 * y + 0.04155506 * z;
    double blue  = 0.05563008 * x - 0.20397696 * y + 1.05697151 * z;

    return {adjustRGB(red), adjustRGB(green), adjustRGB(blue)};
}

std::pair<double, double> generateAB(double hue, float chroma) {
    double a = chroma * 127.0 * std::cos(hue);
    double b = chroma * 127.0 * std::sin(hue);
    return {a, b};
}

std::string colorizeChar(const std::string& ch, int hueIndex, int totalChars) {
    if (ch == " " || ch.empty()) return ch;

    double frequency = 2.0 * M_PI / static_cast<double>(totalChars);
    double hue = hueIndex * frequency;
    auto ab = generateAB(hue, 1.0f);
    auto color = labToRgb(75, ab.first, ab.second);

    std::ostringstream out;
    out << "<font color=\"" << color.toHex() << "\">" << ch << "</font>";
    return out.str();
}

std::string generateRainbow(const std::string& text) {
    if (text.empty()) return {};

    std::ostringstream result;
    int colorIndex = 0;

    for (size_t i = 0; i < text.size(); ++i) {
        unsigned char c = text[i];
        std::string ch(1, c);

        // Handle multi-byte UTF-8 (emoji, CJK)
        if (c >= 0xC0 && i + 1 < text.size() && (text[i+1] & 0xC0) == 0x80) {
            // 2-byte sequence
            ch += text[++i];
            if (i + 1 < text.size() && (text[i+1] & 0xC0) == 0x80) {
                ch += text[++i];
                if (i + 1 < text.size() && (text[i+1] & 0xC0) == 0x80) {
                    ch += text[++i];
                }
            }
        }

        // Skip spaces (don't color them, but count them)
        if (ch == " ") {
            result << ch;
            colorIndex++;
            continue;
        }

        // Colorize the character
        result << colorizeChar(ch, colorIndex, static_cast<int>(text.size()));
        colorIndex++;
    }

    return result.str();
}

} // namespace progressive
