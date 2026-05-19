#include "progressive/color_utils.hpp"
#include <sstream>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <regex>

namespace progressive {

std::string RgbaColor::toHex() const {
    std::ostringstream out;
    out << "#" << std::hex << std::setfill('0')
        << std::setw(2) << r << std::setw(2) << g << std::setw(2) << b;
    return out.str();
}

double RgbaColor::relativeLuminance() const {
    // WCAG 2.1 relative luminance formula
    auto srgbToLinear = [](int c) -> double {
        double s = c / 255.0;
        if (s <= 0.04045) return s / 12.92;
        return std::pow((s + 0.055) / 1.055, 2.4);
    };
    return 0.2126 * srgbToLinear(r) + 0.7152 * srgbToLinear(g) + 0.0722 * srgbToLinear(b);
}

RgbaColor parseColor(const std::string& input) {
    RgbaColor color;
    if (input.empty()) return color;

    // #RGB
    std::regex hex3(R"(^#([0-9A-Fa-f]{3})$)");
    std::smatch m;
    if (std::regex_match(input, m, hex3)) {
        std::string hex = m[1];
        color.r = std::stoi(std::string(2, hex[0]), nullptr, 16) * 17;
        color.g = std::stoi(std::string(2, hex[1]), nullptr, 16) * 17;
        color.b = std::stoi(std::string(2, hex[2]), nullptr, 16) * 17;
        color.valid = true;
        return color;
    }

    // #RRGGBB
    std::regex hex6(R"(^#([0-9A-Fa-f]{6})$)");
    if (std::regex_match(input, m, hex6)) {
        std::string hex = m[1];
        color.r = std::stoi(hex.substr(0, 2), nullptr, 16);
        color.g = std::stoi(hex.substr(2, 2), nullptr, 16);
        color.b = std::stoi(hex.substr(4, 2), nullptr, 16);
        color.valid = true;
        return color;
    }

    // #AARRGGBB
    std::regex hex8(R"(^#([0-9A-Fa-f]{8})$)");
    if (std::regex_match(input, m, hex8)) {
        std::string hex = m[1];
        color.a = std::stoi(hex.substr(0, 2), nullptr, 16);
        color.r = std::stoi(hex.substr(2, 2), nullptr, 16);
        color.g = std::stoi(hex.substr(4, 2), nullptr, 16);
        color.b = std::stoi(hex.substr(6, 2), nullptr, 16);
        color.valid = true;
        return color;
    }

    // rgb(r, g, b) or rgba(r, g, b, a)
    std::regex rgbaRe(R"(rgba?\((\d+),\s*(\d+),\s*(\d+)(?:,\s*([0-9.]+))?\))");
    if (std::regex_search(input, m, rgbaRe)) {
        color.r = std::stoi(m[1]);
        color.g = std::stoi(m[2]);
        color.b = std::stoi(m[3]);
        if (m[4].matched) {
            double a = std::stod(m[4]);
            color.a = static_cast<int>(a * 255.0);
        }
        color.valid = true;
        return color;
    }

    return color;
}

double contrastRatio(const RgbaColor& fg, const RgbaColor& bg) {
    double l1 = fg.relativeLuminance();
    double l2 = bg.relativeLuminance();
    double lighter = std::max(l1, l2);
    double darker  = std::min(l1, l2);
    return (lighter + 0.05) / (darker + 0.05);
}

bool isWcagAaNormal(const RgbaColor& fg, const RgbaColor& bg) {
    return contrastRatio(fg, bg) >= 4.5;
}

bool isWcagAaLarge(const RgbaColor& fg, const RgbaColor& bg) {
    return contrastRatio(fg, bg) >= 3.0;
}

bool isWcagAaaNormal(const RgbaColor& fg, const RgbaColor& bg) {
    return contrastRatio(fg, bg) >= 7.0;
}

std::string getWcagRating(const RgbaColor& fg, const RgbaColor& bg, bool isLargeText) {
    double ratio = contrastRatio(fg, bg);
    if (ratio >= 7.0) return "AAA";
    if (ratio >= 4.5) return isLargeText ? "AAA" : "AA";
    if (ratio >= 3.0) return isLargeText ? "AA" : "FAIL";
    return "FAIL";
}

RgbaColor suggestTextColor(const RgbaColor& bg) {
    // White text on dark backgrounds, black on light
    double lum = bg.relativeLuminance();
    return lum > 0.179 ? RgbaColor{0, 0, 0, 255, true} : RgbaColor{255, 255, 255, 255, true};
}

RgbaColor blendColors(const RgbaColor& bg, const RgbaColor& fg) {
    if (fg.a == 255) return fg;
    if (fg.a == 0) return bg;
    double alpha = fg.a / 255.0;
    return {
        static_cast<int>(fg.r * alpha + bg.r * (1.0 - alpha)),
        static_cast<int>(fg.g * alpha + bg.g * (1.0 - alpha)),
        static_cast<int>(fg.b * alpha + bg.b * (1.0 - alpha)),
        255, true
    };
}

RgbaColor lighten(const RgbaColor& color, double factor) {
    factor = std::max(0.0, std::min(1.0, factor));
    return {
        static_cast<int>(color.r + (255 - color.r) * factor),
        static_cast<int>(color.g + (255 - color.g) * factor),
        static_cast<int>(color.b + (255 - color.b) * factor),
        color.a, true
    };
}

RgbaColor darken(const RgbaColor& color, double factor) {
    factor = std::max(0.0, std::min(1.0, factor));
    return {
        static_cast<int>(color.r * (1.0 - factor)),
        static_cast<int>(color.g * (1.0 - factor)),
        static_cast<int>(color.b * (1.0 - factor)),
        color.a, true
    };
}

RgbaColor hslToRgb(double h, double s, double l) {
    // h: 0-360, s: 0-1, l: 0-1
    double c = (1.0 - std::abs(2.0 * l - 1.0)) * s;
    double x = c * (1.0 - std::abs(std::fmod(h / 60.0, 2.0) - 1.0));
    double m = l - c / 2.0;

    double r1 = 0, g1 = 0, b1 = 0;
    if (h < 60)      { r1 = c; g1 = x; }
    else if (h < 120) { r1 = x; g1 = c; }
    else if (h < 180) { g1 = c; b1 = x; }
    else if (h < 240) { g1 = x; b1 = c; }
    else if (h < 300) { r1 = x; b1 = c; }
    else              { r1 = c; b1 = x; }

    return {
        static_cast<int>((r1 + m) * 255),
        static_cast<int>((g1 + m) * 255),
        static_cast<int>((b1 + m) * 255),
        255, true
    };
}

} // namespace progressive
