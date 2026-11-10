/**
 * ui_styles.cpp - Material Design 3 style system implementation.
 *
 * Implements color scheme generation from seed colors using HCT-like
 * tonal palette generation, typography scaling, theme management,
 * and component-level token resolution.
 */

#include "ui_styles.hpp"
#include "core_utils.hpp"

#include <android/log.h>
#include <nlohmann/json.hpp>

#include <algorithm>
#include <cmath>
#include <mutex>
#include <sstream>
#include <stdexcept>

#define LOG_TAG "PcpStyles"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

namespace pcp {
namespace library {

using json = nlohmann::json;

// ============================================================================
// Internal helpers
// ============================================================================

namespace {

/** Convert sRGB component to linear. */
inline float srgbToLinear(float c) {
    if (c <= 0.04045f) return c / 12.92f;
    return std::pow((c + 0.055f) / 1.055f, 2.4f);
}

/** Convert linear component to sRGB. */
inline float linearToSrgb(float c) {
    if (c <= 0.0031308f) return c * 12.92f;
    return 1.055f * std::pow(c, 1.0f / 2.4f) - 0.055f;
}

/** Compute relative luminance from sRGB. */
float computeLuminance(const Color& c) {
    float r = srgbToLinear(c.r / 255.0f);
    float g = srgbToLinear(c.g / 255.0f);
    float b = srgbToLinear(c.b / 255.0f);
    return 0.2126f * r + 0.7152f * g + 0.0722f * b;
}

/** Convert RGB to XYZ (D65 illuminant). */
void rgbToXyz(float r, float g, float b, float& x, float& y, float& z) {
    float rl = srgbToLinear(r);
    float gl = srgbToLinear(g);
    float bl = srgbToLinear(b);
    x = 0.4124564f * rl + 0.3575761f * gl + 0.1804375f * bl;
    y = 0.2126729f * rl + 0.7151522f * gl + 0.0721750f * bl;
    z = 0.0193339f * rl + 0.1191920f * gl + 0.9503041f * bl;
}

/** Convert XYZ to RGB. */
void xyzToRgb(float x, float y, float z, float& r, float& g, float& b) {
    float rl =  3.2404542f * x - 1.5371385f * y - 0.4985314f * z;
    float gl = -0.9692660f * x + 1.8760108f * y + 0.0415560f * z;
    float bl =  0.0556434f * x - 0.2040259f * y + 1.0572252f * z;
    r = linearToSrgb(rl);
    g = linearToSrgb(gl);
    b = linearToSrgb(bl);
}

/** Clamp float to [0, 1]. */
inline float clamp01(float v) { return std::max(0.0f, std::min(1.0f, v)); }

/** Clamp uint8. */
inline uint8_t clampU8(int v) { return static_cast<uint8_t>(std::max(0, std::min(255, v))); }

/** Compute contrast ratio between two luminance values. */
float contrastFromLuminance(float lumFg, float lumBg) {
    float lighter = std::max(lumFg, lumBg);
    float darker  = std::min(lumFg, lumBg);
    return (lighter + 0.05f) / (darker + 0.05f);
}

/** Tonal palette interpolation curves (Material You style). */
std::array<float, 15> buildToneCurve(float hue, float chroma) {
    // Tones: 0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 95, 98, 99, 100
    // We produce 15 tones at roughly 0, 7, 14, 21, 29, 36, 43, 50, 57, 64, 71, 79, 86, 93, 100
    std::array<float, 15> tones;
    constexpr float toneSteps[] = {0.0f, 7.0f, 14.0f, 21.0f, 29.0f, 36.0f, 43.0f,
                                    50.0f, 57.0f, 64.0f, 71.0f, 79.0f, 86.0f, 93.0f, 100.0f};
    for (int i = 0; i < 15; i++) {
        // Map tone to perceptual lightness (simplified HCT approximation)
        float t = toneSteps[i] / 100.0f;
        // Chroma scaling: at low/high tones, chroma decreases
        float chromaScale = 1.0f - 2.0f * std::abs(t - 0.5f);
        chromaScale = chromaScale * chromaScale; // Squared for smoother falloff
        tones[i] = chroma * std::max(0.0f, chromaScale);
    }
    return tones;
}

/** Simple HCT-like color generation from hue/chroma/tone. */
Color colorFromHct(float hueDeg, float chroma, float tone) {
    // tone: 0-100, maps to perceived lightness
    // Simplified implementation: use HSL-like model with tone as lightness
    float h = hueDeg / 360.0f;
    float l = tone / 100.0f;
    // Chroma modulates saturation; limit so we don't exceed sRGB gamut
    float s = std::min(1.0f, chroma / 150.0f);

    // HSL to RGB
    auto hueToRgb = [](float p, float q, float t) -> float {
        if (t < 0.0f) t += 1.0f;
        if (t > 1.0f) t -= 1.0f;
        if (t < 1.0f/6.0f) return p + (q - p) * 6.0f * t;
        if (t < 1.0f/2.0f) return q;
        if (t < 2.0f/3.0f) return p + (q - p) * (2.0f/3.0f - t) * 6.0f;
        return p;
    };

    float q = l < 0.5f ? l * (1.0f + s) : l + s - l * s;
    float p = 2.0f * l - q;
    float r = hueToRgb(p, q, h + 1.0f/3.0f);
    float g = hueToRgb(p, q, h);
    float bVal = hueToRgb(p, q, h - 1.0f/3.0f);

    return Color(
        clampU8(static_cast<int>(r * 255.0f)),
        clampU8(static_cast<int>(g * 255.0f)),
        clampU8(static_cast<int>(bVal * 255.0f))
    );
}

} // anonymous namespace

// ============================================================================
// ColorScheme operators
// ============================================================================

bool ColorScheme::operator==(const ColorScheme& other) const {
    return primary == other.primary && onPrimary == other.onPrimary
        && primaryContainer == other.primaryContainer && onPrimaryContainer == other.onPrimaryContainer
        && secondary == other.secondary && onSecondary == other.onSecondary
        && secondaryContainer == other.secondaryContainer && onSecondaryContainer == other.onSecondaryContainer
        && tertiary == other.tertiary && onTertiary == other.onTertiary
        && tertiaryContainer == other.tertiaryContainer && onTertiaryContainer == other.onTertiaryContainer
        && error == other.error && onError == other.onError
        && errorContainer == other.errorContainer && onErrorContainer == other.onErrorContainer
        && background == other.background && onBackground == other.onBackground
        && surface == other.surface && onSurface == other.onSurface
        && surfaceVariant == other.surfaceVariant && onSurfaceVariant == other.onSurfaceVariant
        && surfaceTint == other.surfaceTint
        && inverseSurface == other.inverseSurface && inverseOnSurface == other.inverseOnSurface
        && inversePrimary == other.inversePrimary
        && outline == other.outline && outlineVariant == other.outlineVariant
        && shadow == other.shadow && scrim == other.scrim
        && mention == other.mention && onMention == other.onMention
        && reply == other.reply && onReply == other.onReply
        && linkColor == other.linkColor
        && bubbleSent == other.bubbleSent && onBubbleSent == other.onBubbleSent
        && bubbleReceived == other.bubbleReceived && onBubbleReceived == other.onBubbleReceived
        && codeBlock == other.codeBlock && onCodeBlock == other.onCodeBlock
        && divider == other.divider
        && success == other.success && onSuccess == other.onSuccess
        && warning == other.warning && onWarning == other.onWarning;
}

// ============================================================================
// Spacing scaled()
// ============================================================================

Spacing Spacing::scaled(float factor) const {
    Spacing s = *this;
    s.none = 0.0f; // never scale zero
    s.extraSmall *= factor; s.small *= factor; s.medium *= factor;
    s.base *= factor; s.large *= factor; s.extraLarge *= factor;
    s.xxl *= factor; s.xxxl *= factor;
    s.screenHorizontal *= factor; s.screenVertical *= factor;
    s.cardPadding *= factor; s.buttonPaddingHorizontal *= factor;
    s.buttonPaddingVertical *= factor; s.inputPaddingHorizontal *= factor;
    s.inputPaddingVertical *= factor; s.listItemPadding *= factor;
    s.avatarSize *= factor; s.avatarSizeSmall *= factor;
    s.avatarSizeLarge *= factor; s.iconSize *= factor;
    s.iconSizeSmall *= factor; s.iconSizeLarge *= factor;
    s.bottomNavHeight *= factor; s.topBarHeight *= factor;
    s.snackbarMinHeight *= factor; s.dialogMinWidth *= factor;
    s.dialogMaxWidth *= factor; s.chatBubbleMaxWidth *= factor;
    s.chatBubbleRadius *= factor; s.mentionBadgeSize *= factor;
    s.progressBarHeight *= factor; s.thumbSize *= factor;
    s.waveformBarWidth *= factor; s.waveformBarGap *= factor;
    return s;
}

// ============================================================================
// Theme methods
// ============================================================================

Theme Theme::dark() const {
    Theme d = *this;
    d.isDark = true;
    d.name = name + " Dark";

    // Swap surface colors for dark theme
    std::swap(d.colors.background, d.colors.onBackground);
    std::swap(d.colors.surface, d.colors.onSurface);
    std::swap(d.colors.surfaceVariant, d.colors.onSurfaceVariant);

    // Darken surface colors
    d.colors.background = darken(d.colors.background, 0.85f);
    d.colors.surface = darken(d.colors.surface, 0.82f);
    d.colors.surfaceVariant = darken(d.colors.surfaceVariant, 0.80f);

    // Lighten on-surface text for readability
    d.colors.onBackground = lighten(d.colors.onBackground, 0.95f);
    d.colors.onSurface = lighten(d.colors.onSurface, 0.90f);
    d.colors.onSurfaceVariant = lighten(d.colors.onSurfaceVariant, 0.85f);

    // Adjust containers for dark
    d.colors.primaryContainer = darken(d.colors.primaryContainer, 0.40f);
    d.colors.secondaryContainer = darken(d.colors.secondaryContainer, 0.40f);
    d.colors.tertiaryContainer = darken(d.colors.tertiaryContainer, 0.40f);

    d.colors.outline = lighten(d.colors.outline, 0.30f);
    d.colors.outlineVariant = lighten(d.colors.outlineVariant, 0.15f);
    d.colors.divider = lighten(d.colors.divider, 0.20f);

    d.colors.shadow = Color(0, 0, 0, 180);
    d.colors.scrim = Color(0, 0, 0, 150);

    return d;
}

Theme Theme::highContrast() const {
    Theme hc = *this;
    hc.name = name + " High Contrast";

    // Boost contrast: darken backgrounds, lighten foregrounds
    hc.colors.onPrimary = getTextColor(hc.colors.primary);
    hc.colors.onSecondary = getTextColor(hc.colors.secondary);
    hc.colors.onTertiary = getTextColor(hc.colors.tertiary);
    hc.colors.onError = getTextColor(hc.colors.error);

    hc.colors.onBackground = isDark ? Color(255, 255, 255) : Color(0, 0, 0);
    hc.colors.onSurface = isDark ? Color(255, 255, 255) : Color(0, 0, 0);
    hc.colors.onSurfaceVariant = isDark ? Color(230, 230, 230) : Color(30, 30, 30);

    hc.colors.outline = isDark ? Color(200, 200, 200) : Color(80, 80, 80);
    hc.colors.divider = hc.colors.outline;

    return hc;
}

bool Theme::passesContrast(const Color& foreground, const Color& background,
                            float minimumRatio) const {
    float ratio = contrastRatio(foreground, background);
    return ratio >= minimumRatio;
}

Color Theme::textColorForSurface(const Color& surfaceColor) const {
    return getTextColor(surfaceColor);
}

std::string Theme::toJson() const {
    json j;
    j["name"] = name;
    j["isDark"] = isDark;
    j["seedColor"] = toHexString(seedColor, true);

    // Colors
    auto colorToJson = [](const Color& c) -> json {
        return json{{"r", c.r}, {"g", c.g}, {"b", c.b}, {"a", c.a}};
    };

    json colorsJson;
    colorsJson["primary"] = colorToJson(colors.primary);
    colorsJson["onPrimary"] = colorToJson(colors.onPrimary);
    colorsJson["primaryContainer"] = colorToJson(colors.primaryContainer);
    colorsJson["onPrimaryContainer"] = colorToJson(colors.onPrimaryContainer);
    colorsJson["secondary"] = colorToJson(colors.secondary);
    colorsJson["onSecondary"] = colorToJson(colors.onSecondary);
    colorsJson["secondaryContainer"] = colorToJson(colors.secondaryContainer);
    colorsJson["onSecondaryContainer"] = colorToJson(colors.onSecondaryContainer);
    colorsJson["tertiary"] = colorToJson(colors.tertiary);
    colorsJson["onTertiary"] = colorToJson(colors.onTertiary);
    colorsJson["tertiaryContainer"] = colorToJson(colors.tertiaryContainer);
    colorsJson["onTertiaryContainer"] = colorToJson(colors.onTertiaryContainer);
    colorsJson["error"] = colorToJson(colors.error);
    colorsJson["onError"] = colorToJson(colors.onError);
    colorsJson["errorContainer"] = colorToJson(colors.errorContainer);
    colorsJson["onErrorContainer"] = colorToJson(colors.onErrorContainer);
    colorsJson["background"] = colorToJson(colors.background);
    colorsJson["onBackground"] = colorToJson(colors.onBackground);
    colorsJson["surface"] = colorToJson(colors.surface);
    colorsJson["onSurface"] = colorToJson(colors.onSurface);
    colorsJson["surfaceVariant"] = colorToJson(colors.surfaceVariant);
    colorsJson["onSurfaceVariant"] = colorToJson(colors.onSurfaceVariant);
    colorsJson["outline"] = colorToJson(colors.outline);
    colorsJson["divider"] = colorToJson(colors.divider);
    colorsJson["success"] = colorToJson(colors.success);
    colorsJson["warning"] = colorToJson(colors.warning);
    j["colors"] = colorsJson;

    // Typography (simplified — store key styles)
    auto textStyleToJson = [](const TextStyle& ts) -> json {
        return json{
            {"fontFamily", ts.fontFamily}, {"fontSize", ts.fontSize},
            {"fontWeight", static_cast<int>(ts.fontWeight)},
            {"lineHeight", ts.lineHeight}, {"letterSpacing", ts.letterSpacing}
        };
    };
    json typoJson;
    typoJson["bodyLarge"] = textStyleToJson(typography.bodyLarge);
    typoJson["bodyMedium"] = textStyleToJson(typography.bodyMedium);
    typoJson["titleLarge"] = textStyleToJson(typography.titleLarge);
    typoJson["titleMedium"] = textStyleToJson(typography.titleMedium);
    j["typography"] = typoJson;

    // Spacing
    json spacingJson;
    spacingJson["base"] = spacing.base;
    spacingJson["small"] = spacing.small;
    spacingJson["large"] = spacing.large;
    spacingJson["screenHorizontal"] = spacing.screenHorizontal;
    spacingJson["screenVertical"] = spacing.screenVertical;
    spacingJson["cardPadding"] = spacing.cardPadding;
    j["spacing"] = spacingJson;

    // Shape
    json shapeJson;
    shapeJson["small"] = shape.small;
    shapeJson["medium"] = shape.medium;
    shapeJson["large"] = shape.large;
    shapeJson["button"] = shape.button;
    shapeJson["card"] = shape.card;
    shapeJson["dialog"] = shape.dialog;
    shapeJson["chatBubble"] = shape.chatBubble;
    j["shape"] = shapeJson;

    return j.dump(2);
}

Theme Theme::fromJson(const std::string& jsonStr) {
    Theme t;
    try {
        json j = json::parse(jsonStr);

        t.name = j.value("name", "Default");
        t.isDark = j.value("isDark", false);

        auto jsonToColor = [](const json& cj) -> Color {
            return Color(
                static_cast<uint8_t>(cj.value("r", 0)),
                static_cast<uint8_t>(cj.value("g", 0)),
                static_cast<uint8_t>(cj.value("b", 0)),
                static_cast<uint8_t>(cj.value("a", 255))
            );
        };

        if (j.contains("seedColor")) {
            t.seedColor = jsonToColor(j["seedColor"]);
        }

        if (j.contains("colors")) {
            auto& c = j["colors"];
            t.colors.primary = jsonToColor(c["primary"]);
            t.colors.onPrimary = jsonToColor(c["onPrimary"]);
            t.colors.primaryContainer = jsonToColor(c.value("primaryContainer", json::object()));
            t.colors.onPrimaryContainer = jsonToColor(c.value("onPrimaryContainer", json::object()));
            t.colors.secondary = jsonToColor(c["secondary"]);
            t.colors.onSecondary = jsonToColor(c["onSecondary"]);
            t.colors.secondaryContainer = jsonToColor(c.value("secondaryContainer", json::object()));
            t.colors.onSecondaryContainer = jsonToColor(c.value("onSecondaryContainer", json::object()));
            t.colors.tertiary = jsonToColor(c.value("tertiary", json::object()));
            t.colors.onTertiary = jsonToColor(c.value("onTertiary", json::object()));
            t.colors.error = jsonToColor(c.value("error", json::object()));
            t.colors.onError = jsonToColor(c.value("onError", json::object()));
            t.colors.background = jsonToColor(c.value("background", json::object()));
            t.colors.onBackground = jsonToColor(c.value("onBackground", json::object()));
            t.colors.surface = jsonToColor(c.value("surface", json::object()));
            t.colors.onSurface = jsonToColor(c.value("onSurface", json::object()));
            t.colors.surfaceVariant = jsonToColor(c.value("surfaceVariant", json::object()));
            t.colors.onSurfaceVariant = jsonToColor(c.value("onSurfaceVariant", json::object()));
            t.colors.outline = jsonToColor(c.value("outline", json::object()));
            t.colors.divider = jsonToColor(c.value("divider", json::object()));
            t.colors.success = jsonToColor(c.value("success", json::object()));
            t.colors.warning = jsonToColor(c.value("warning", json::object()));
        }

        if (j.contains("typography")) {
            auto jsonToTextStyle = [](const json& tj, const TextStyle& fallback) -> TextStyle {
                if (tj.is_null()) return fallback;
                TextStyle ts;
                ts.fontFamily = tj.value("fontFamily", fallback.fontFamily);
                ts.fontSize = tj.value("fontSize", fallback.fontSize);
                ts.fontWeight = static_cast<FontWeight>(tj.value("fontWeight", static_cast<int>(fallback.fontWeight)));
                ts.lineHeight = tj.value("lineHeight", fallback.lineHeight);
                ts.letterSpacing = tj.value("letterSpacing", fallback.letterSpacing);
                return ts;
            };
            auto& tp = j["typography"];
            t.typography.bodyLarge = jsonToTextStyle(tp.value("bodyLarge", json::object()), t.typography.bodyLarge);
            t.typography.bodyMedium = jsonToTextStyle(tp.value("bodyMedium", json::object()), t.typography.bodyMedium);
            t.typography.titleLarge = jsonToTextStyle(tp.value("titleLarge", json::object()), t.typography.titleLarge);
            t.typography.titleMedium = jsonToTextStyle(tp.value("titleMedium", json::object()), t.typography.titleMedium);
        }

        if (j.contains("spacing")) {
            auto& sp = j["spacing"];
            t.spacing.base = sp.value("base", t.spacing.base);
            t.spacing.small = sp.value("small", t.spacing.small);
            t.spacing.large = sp.value("large", t.spacing.large);
            t.spacing.screenHorizontal = sp.value("screenHorizontal", t.spacing.screenHorizontal);
            t.spacing.screenVertical = sp.value("screenVertical", t.spacing.screenVertical);
            t.spacing.cardPadding = sp.value("cardPadding", t.spacing.cardPadding);
        }

        if (j.contains("shape")) {
            auto& sh = j["shape"];
            t.shape.small = sh.value("small", t.shape.small);
            t.shape.medium = sh.value("medium", t.shape.medium);
            t.shape.large = sh.value("large", t.shape.large);
            t.shape.button = sh.value("button", t.shape.button);
            t.shape.card = sh.value("card", t.shape.card);
            t.shape.dialog = sh.value("dialog", t.shape.dialog);
            t.shape.chatBubble = sh.value("chatBubble", t.shape.chatBubble);
        }

    } catch (const json::exception& e) {
        LOGE("Failed to parse theme JSON: %s", e.what());
    }
    return t;
}

// ============================================================================
// ThemeBuilder static methods
// ============================================================================

std::array<Color, 15> ThemeBuilder::tonalPalette(float hue, float chroma) {
    std::array<Color, 15> palette;
    constexpr float tones[] = {0.0f, 7.0f, 14.0f, 21.0f, 29.0f, 36.0f, 43.0f,
                                50.0f, 57.0f, 64.0f, 71.0f, 79.0f, 86.0f, 93.0f, 100.0f};
    for (int i = 0; i < 15; i++) {
        palette[i] = tonalColor(hue, chroma, tones[i]);
    }
    return palette;
}

Color ThemeBuilder::tonalColor(float hue, float chroma, float tone) {
    // Map tone to effective chroma (reduced at extremes)
    float t = tone / 100.0f;
    float effectiveChroma = chroma * (1.0f - std::pow(2.0f * t - 1.0f, 2.0f) * 0.6f);
    effectiveChroma = std::max(0.0f, effectiveChroma);
    return colorFromHct(hue, effectiveChroma, tone);
}

ThemeBuilder::Cam16 ThemeBuilder::toCam16(const Color& color) {
    Cam16 cam;
    // Simplified: use HSV for approximate hue/chroma
    HSV hsv = toHSV(color);
    cam.hue = hsv.h;
    cam.chroma = hsv.s * 100.0f; // approximate
    cam.j = hsv.v * 100.0f;      // approximate lightness
    cam.q = hsv.v * 100.0f;
    cam.m = hsv.s * 100.0f;
    cam.s = hsv.s * 100.0f;
    return cam;
}

Color ThemeBuilder::fromCam16(const Cam16& cam) {
    float s = std::min(1.0f, cam.chroma / 100.0f);
    float v = std::min(1.0f, cam.j / 100.0f);
    return fromHSV(HSV{cam.hue, s, v});
}

float ThemeBuilder::lerpTone(float tone) {
    return clamp01(tone / 100.0f);
}

Typography ThemeBuilder::buildTypography(const std::string& fontFamily) {
    Typography t;

    // Display
    t.displayLarge  = TextStyle(fontFamily, 57.0f, FontWeight::Regular);
    t.displayMedium = TextStyle(fontFamily, 45.0f, FontWeight::Regular);
    t.displaySmall  = TextStyle(fontFamily, 36.0f, FontWeight::Regular);

    // Headline
    t.headlineLarge  = TextStyle(fontFamily, 32.0f, FontWeight::Regular);
    t.headlineMedium = TextStyle(fontFamily, 28.0f, FontWeight::Regular);
    t.headlineSmall  = TextStyle(fontFamily, 24.0f, FontWeight::Regular);

    // Title
    t.titleLarge  = TextStyle(fontFamily, 22.0f, FontWeight::Regular);
    t.titleMedium = TextStyle(fontFamily, 16.0f, FontWeight::Medium, FontStyle::Normal, 0.15f);
    t.titleSmall  = TextStyle(fontFamily, 14.0f, FontWeight::Medium, FontStyle::Normal, 0.1f);

    // Body
    t.bodyLarge  = TextStyle(fontFamily, 16.0f, FontWeight::Regular, FontStyle::Normal, 0.5f, 1.5f);
    t.bodyMedium = TextStyle(fontFamily, 14.0f, FontWeight::Regular, FontStyle::Normal, 0.25f, 1.4f);
    t.bodySmall  = TextStyle(fontFamily, 12.0f, FontWeight::Regular, FontStyle::Normal, 0.4f, 1.3f);

    // Label
    t.labelLarge  = TextStyle(fontFamily, 14.0f, FontWeight::Medium, FontStyle::Normal, 0.1f);
    t.labelMedium = TextStyle(fontFamily, 12.0f, FontWeight::Medium, FontStyle::Normal, 0.5f);
    t.labelSmall  = TextStyle(fontFamily, 11.0f, FontWeight::Medium, FontStyle::Normal, 0.5f);

    // Chat-specific
    t.messageText     = TextStyle(fontFamily, 15.0f, FontWeight::Regular, FontStyle::Normal, 0.25f, 1.45f);
    t.messageTimestamp = TextStyle(fontFamily, 11.0f, FontWeight::Regular, FontStyle::Normal, 0.3f);
    t.codeText         = TextStyle("monospace", 13.0f, FontWeight::Regular, FontStyle::Normal, 0.0f, 1.5f);
    t.codeText.isMonospace = true;
    t.notificationText = TextStyle(fontFamily, 13.0f, FontWeight::Regular, FontStyle::Italic, 0.25f);

    return t;
}

ColorScheme ThemeBuilder::buildColorScheme(const Color& primary,
                                            const Color& secondary,
                                            const Color& tertiary,
                                            const Color& error,
                                            bool isDark) {
    ColorScheme cs;

    cs.primary = primary;
    cs.onPrimary = getTextColor(primary);

    cs.primaryContainer = blend(primary, isDark ? Color(0, 0, 0) : Color(255, 255, 255), 0.7f);
    cs.onPrimaryContainer = getTextColor(cs.primaryContainer);

    cs.secondary = secondary;
    cs.onSecondary = getTextColor(secondary);

    cs.secondaryContainer = blend(secondary, isDark ? Color(0, 0, 0) : Color(255, 255, 255), 0.7f);
    cs.onSecondaryContainer = getTextColor(cs.secondaryContainer);

    cs.tertiary = tertiary;
    cs.onTertiary = getTextColor(tertiary);

    cs.tertiaryContainer = blend(tertiary, isDark ? Color(0, 0, 0) : Color(255, 255, 255), 0.7f);
    cs.onTertiaryContainer = getTextColor(cs.tertiaryContainer);

    cs.error = error;
    cs.onError = getTextColor(error);
    cs.errorContainer = blend(error, isDark ? Color(0, 0, 0) : Color(255, 255, 255), 0.7f);
    cs.onErrorContainer = getTextColor(cs.errorContainer);

    if (isDark) {
        cs.background = Color(28, 27, 31);
        cs.onBackground = Color(230, 225, 229);
        cs.surface = Color(28, 27, 31);
        cs.onSurface = Color(230, 225, 229);
        cs.surfaceVariant = Color(73, 69, 79);
        cs.onSurfaceVariant = Color(202, 196, 208);
        cs.surfaceTint = primary;
    } else {
        cs.background = Color(255, 251, 254);
        cs.onBackground = Color(28, 27, 31);
        cs.surface = Color(255, 251, 254);
        cs.onSurface = Color(28, 27, 31);
        cs.surfaceVariant = Color(231, 224, 236);
        cs.onSurfaceVariant = Color(73, 69, 79);
        cs.surfaceTint = primary;
    }

    cs.inverseSurface = isDark ? Color(230, 225, 229) : Color(49, 48, 51);
    cs.inverseOnSurface = isDark ? Color(49, 48, 51) : Color(244, 239, 244);
    cs.inversePrimary = blend(primary, isDark ? Color(255, 255, 255) : Color(0, 0, 0), 0.4f);

    cs.outline = isDark ? Color(147, 143, 153) : Color(121, 116, 126);
    cs.outlineVariant = isDark ? Color(73, 69, 79) : Color(202, 196, 208);
    cs.shadow = Color(0, 0, 0, isDark ? 80 : 40);
    cs.scrim = Color(0, 0, 0, isDark ? 120 : 60);

    // Chat-specific defaults
    cs.mention = blend(primary, Color(255, 180, 0), 0.3f);
    cs.onMention = getTextColor(cs.mention);
    cs.reply = blend(secondary, Color(0, 128, 255), 0.3f);
    cs.onReply = getTextColor(cs.reply);
    cs.linkColor = blend(primary, Color(0, 100, 255), 0.3f);

    cs.bubbleSent = primaryContainer;
    cs.onBubbleSent = onPrimaryContainer;
    cs.bubbleReceived = isDark ? Color(50, 50, 55) : Color(240, 240, 245);
    cs.onBubbleReceived = isDark ? Color(230, 225, 229) : Color(28, 27, 31);

    cs.codeBlock = isDark ? Color(35, 35, 40) : Color(245, 245, 250);
    cs.onCodeBlock = isDark ? Color(220, 220, 230) : Color(50, 50, 60);
    cs.divider = isDark ? Color(60, 60, 65) : Color(220, 220, 225);

    cs.success = Color(56, 142, 60);
    cs.onSuccess = Color(255, 255, 255);
    cs.warning = Color(245, 124, 0);
    cs.onWarning = Color(255, 255, 255);

    return cs;
}

bool ThemeBuilder::hasSufficientContrast(const Color& a, const Color& b,
                                          float minRatio) {
    float ratio = contrastRatio(a, b);
    return ratio >= minRatio;
}

Color ThemeBuilder::ensureContrast(const Color& foreground, const Color& background,
                                     float minRatio) {
    if (hasSufficientContrast(foreground, background, minRatio)) {
        return foreground;
    }
    // Darken or lighten until contrast passes
    Color adjusted = foreground;
    for (int i = 0; i < 20; i++) {
        float lumFg = computeLuminance(adjusted);
        float lumBg = computeLuminance(background);
        if (lumFg > lumBg) {
            adjusted = lighten(adjusted, 0.05f);
        } else {
            adjusted = darken(adjusted, 0.05f);
        }
        if (hasSufficientContrast(adjusted, background, minRatio)) {
            return adjusted;
        }
    }
    // Fallback: return black or white
    return lumBg > 0.5f ? Color(0, 0, 0) : Color(255, 255, 255);
}

ColorScheme ThemeBuilder::materialYouLight(const Color& seed) {
    HSV hsv = toHSV(seed);
    float hue = hsv.h;
    float chroma = hsv.s * 120.0f;

    Color primary = tonalColor(hue, chroma, 40.0f);
    Color secondary = tonalColor(hue, chroma * 0.6f, 40.0f);
    Color tertiary = tonalColor(hue + 60.0f, chroma * 0.5f, 40.0f);
    Color error = Color(186, 26, 26);

    return buildColorScheme(primary, secondary, tertiary, error, false);
}

ColorScheme ThemeBuilder::materialYouDark(const Color& seed) {
    HSV hsv = toHSV(seed);
    float hue = hsv.h;
    float chroma = hsv.s * 120.0f;

    Color primary = tonalColor(hue, chroma, 80.0f);
    Color secondary = tonalColor(hue, chroma * 0.6f, 80.0f);
    Color tertiary = tonalColor(hue + 60.0f, chroma * 0.5f, 80.0f);
    Color error = Color(242, 139, 130);

    return buildColorScheme(primary, secondary, tertiary, error, true);
}

Theme ThemeBuilder::generateLight(const Color& seed) {
    Theme t;
    t.name = "Material You Light";
    t.seedColor = seed;
    t.isDark = false;
    t.colors = materialYouLight(seed);
    t.typography = buildTypography();
    // Spacing, elevation, shape use defaults from structs
    return t;
}

Theme ThemeBuilder::generateDark(const Color& seed) {
    Theme t;
    t.name = "Material You Dark";
    t.seedColor = seed;
    t.isDark = true;
    t.colors = materialYouDark(seed);
    t.typography = buildTypography();
    return t;
}

std::pair<Theme, Theme> ThemeBuilder::generatePair(const Color& seed) {
    return {generateLight(seed), generateDark(seed)};
}

// ============================================================================
// ThemeManager
// ============================================================================

static std::mutex gThemeMutex;
static std::unique_ptr<ThemeManager> gThemeInstance;

ThemeManager::ThemeManager() {
    // Start with a default purple Material You theme
    Color defaultSeed(103, 80, 164); // Purple 500
    lightTheme_ = ThemeBuilder::generateLight(defaultSeed);
    darkTheme_ = ThemeBuilder::generateDark(defaultSeed);
    currentTheme_ = lightTheme_;
    registerDefaultTokens();
    rebuildComponentTokens();
}

ThemeManager& ThemeManager::instance() {
    if (!gThemeInstance) {
        std::lock_guard<std::mutex> lock(gThemeMutex);
        if (!gThemeInstance) {
            gThemeInstance.reset(new ThemeManager());
        }
    }
    return *gThemeInstance;
}

void ThemeManager::setTheme(const Theme& theme) {
    std::lock_guard<std::mutex> lock(gThemeMutex);
    currentTheme_ = theme;
    if (theme.isDark) {
        darkTheme_ = theme;
    } else {
        lightTheme_ = theme;
    }
    rebuildComponentTokens();
}

void ThemeManager::setTheme(Theme&& theme) {
    std::lock_guard<std::mutex> lock(gThemeMutex);
    bool wasDark = theme.isDark;
    currentTheme_ = std::move(theme);
    if (wasDark) {
        darkTheme_ = currentTheme_;
    } else {
        lightTheme_ = currentTheme_;
    }
    rebuildComponentTokens();
}

const Theme& ThemeManager::getTheme() const {
    // read-only access; external caller expected to hold lock if needed
    return currentTheme_;
}

Theme& ThemeManager::mutableTheme() {
    return currentTheme_;
}

void ThemeManager::setDarkMode(bool dark) {
    std::lock_guard<std::mutex> lock(gThemeMutex);
    if (dark && !currentTheme_.isDark) {
        currentTheme_ = darkTheme_;
        currentTheme_.isDark = true;
    } else if (!dark && currentTheme_.isDark) {
        currentTheme_ = lightTheme_;
        currentTheme_.isDark = false;
    }
    rebuildComponentTokens();
}

bool ThemeManager::isDarkMode() const {
    return currentTheme_.isDark;
}

void ThemeManager::registerDefaultTokens() {
    // Component color tokens — set defaults in case rebuild is called
}

void ThemeManager::rebuildComponentTokens() {
    const auto& cs = currentTheme_.colors;
    const auto& sp = currentTheme_.spacing;
    const auto& sh = currentTheme_.shape;

    // Color tokens
    colorTokens_["button_bg"]          = cs.primary;
    colorTokens_["button_text"]        = cs.onPrimary;
    colorTokens_["button_bg_disabled"] = blend(cs.surface, cs.onSurface, 0.12f);
    colorTokens_["button_text_disabled"] = blend(cs.onSurface, cs.surface, 0.38f);
    colorTokens_["input_bg"]           = cs.surfaceVariant;
    colorTokens_["input_border"]       = cs.outline;
    colorTokens_["input_text"]         = cs.onSurface;
    colorTokens_["input_placeholder"]  = cs.onSurfaceVariant;
    colorTokens_["card_bg"]            = cs.surface;
    colorTokens_["card_border"]        = cs.outlineVariant;
    colorTokens_["bubble_sent_bg"]     = cs.bubbleSent;
    colorTokens_["bubble_sent_text"]   = cs.onBubbleSent;
    colorTokens_["bubble_received_bg"] = cs.bubbleReceived;
    colorTokens_["bubble_received_text"] = cs.onBubbleReceived;
    colorTokens_["nav_bar_bg"]         = cs.surface;
    colorTokens_["status_bar_bg"]      = blend(cs.surface, cs.primary, 0.08f);
    colorTokens_["badge_mention"]      = cs.mention;
    colorTokens_["badge_mention_text"] = cs.onMention;
    colorTokens_["divider_color"]      = cs.divider;
    colorTokens_["toolbar_bg"]         = cs.surface;
    colorTokens_["toolbar_text"]       = cs.onSurface;
    colorTokens_["dialog_bg"]          = cs.surface;
    colorTokens_["dialog_text"]        = cs.onSurface;
    colorTokens_["snackbar_bg"]        = cs.inverseSurface;
    colorTokens_["snackbar_text"]      = cs.inverseOnSurface;
    colorTokens_["overlay_scrim"]      = cs.scrim;
    colorTokens_["link_color"]         = cs.linkColor;
    colorTokens_["error_text"]         = cs.error;
    colorTokens_["success_text"]       = cs.success;
    colorTokens_["warning_text"]       = cs.warning;
    colorTokens_["code_block_bg"]      = cs.codeBlock;
    colorTokens_["code_block_text"]    = cs.onCodeBlock;
    colorTokens_["reply_bar_bg"]       = cs.reply;
    colorTokens_["reply_bar_text"]     = cs.onReply;

    // Spacing tokens
    spacingTokens_["screen_horizontal"]  = sp.screenHorizontal;
    spacingTokens_["screen_vertical"]    = sp.screenVertical;
    spacingTokens_["card_padding"]       = sp.cardPadding;
    spacingTokens_["button_padding_h"]   = sp.buttonPaddingHorizontal;
    spacingTokens_["button_padding_v"]   = sp.buttonPaddingVertical;
    spacingTokens_["input_padding_h"]    = sp.inputPaddingHorizontal;
    spacingTokens_["input_padding_v"]    = sp.inputPaddingVertical;
    spacingTokens_["list_item_padding"]  = sp.listItemPadding;
    spacingTokens_["avatar_size"]        = sp.avatarSize;
    spacingTokens_["avatar_size_small"]  = sp.avatarSizeSmall;
    spacingTokens_["avatar_size_large"]  = sp.avatarSizeLarge;
    spacingTokens_["icon_size"]          = sp.iconSize;
    spacingTokens_["icon_size_small"]    = sp.iconSizeSmall;
    spacingTokens_["icon_size_large"]    = sp.iconSizeLarge;
    spacingTokens_["bottom_nav_height"]  = sp.bottomNavHeight;
    spacingTokens_["top_bar_height"]     = sp.topBarHeight;
    spacingTokens_["chat_bubble_max"]    = sp.chatBubbleMaxWidth;
    spacingTokens_["chat_bubble_radius"] = sp.chatBubbleRadius;
}

Color ThemeManager::getColor(const std::string& token) const {
    return getColor(token, Color(128, 128, 128));
}

Color ThemeManager::getColor(const std::string& token, const Color& fallback) const {
    auto it = colorTokens_.find(token);
    if (it != colorTokens_.end()) {
        return it->second;
    }
    return fallback;
}

float ThemeManager::getSpacing(const std::string& token) const {
    return getSpacing(token, 0.0f);
}

float ThemeManager::getSpacing(const std::string& token, float fallback) const {
    auto it = spacingTokens_.find(token);
    if (it != spacingTokens_.end()) {
        return it->second;
    }
    return fallback;
}

float ThemeManager::getShape(const std::string& token) const {
    const auto& sh = currentTheme_.shape;
    if (token == "none") return sh.none;
    if (token == "extra_small" || token == "xs") return sh.extraSmall;
    if (token == "small") return sh.small;
    if (token == "medium") return sh.medium;
    if (token == "large") return sh.large;
    if (token == "extra_large" || token == "xl") return sh.extraLarge;
    if (token == "full") return sh.full;
    if (token == "button") return sh.button;
    if (token == "chip") return sh.chip;
    if (token == "card") return sh.card;
    if (token == "dialog") return sh.dialog;
    if (token == "snackbar") return sh.snackbar;
    if (token == "text_field") return sh.textField;
    if (token == "badge") return sh.badge;
    if (token == "avatar") return sh.avatar;
    if (token == "chat_bubble") return sh.chatBubble;
    if (token == "progress_bar") return sh.progressBar;
    if (token == "tab_bar") return sh.tabBar;
    return sh.small; // default
}

ElevationLevel ThemeManager::getElevation(const std::string& token) const {
    const auto& el = currentTheme_.elevation;
    if (token == "level0" || token == "flat") return el.level0;
    if (token == "level1" || token == "resting") return el.level1;
    if (token == "level2") return el.level2;
    if (token == "level3" || token == "app_bar") return el.level3;
    if (token == "level4" || token == "dialog") return el.level4;
    if (token == "level5" || token == "drawer") return el.level5;
    if (token == "level6" || token == "modal") return el.level6;
    if (token == "level7" || token == "toast") return el.level7;
    return el.level1;
}

void ThemeManager::setColorToken(const std::string& token, const Color& color) {
    std::lock_guard<std::mutex> lock(gThemeMutex);
    colorTokens_[token] = color;
}

void ThemeManager::setSpacingToken(const std::string& token, float value) {
    std::lock_guard<std::mutex> lock(gThemeMutex);
    spacingTokens_[token] = value;
}

std::string ThemeManager::toJson() const {
    return currentTheme_.toJson();
}

} // namespace library
} // namespace pcp
