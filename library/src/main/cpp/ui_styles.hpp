#pragma once

/**
 * ui_styles.hpp - Material Design 3 style system for PCP chat client.
 *
 * Provides a comprehensive Material Design 3 theming engine with:
 * - Color roles (primary, secondary, tertiary, error, surface, etc.)
 * - Typography scale (display, headline, title, body, label)
 * - Spacing tokens (Material 3 density scale)
 * - Elevation/shadow tokens
 * - Shape tokens (corner radii)
 * - Dark/light theme support
 * - Dynamic color generation from a seed color
 * - Contrast checking and accessibility helpers
 * - Component-level tokens (button, input, card, dialog, etc.)
 */

#include <string>
#include <unordered_map>
#include <vector>
#include <cmath>
#include <memory>
#include "core_utils.hpp"

namespace pcp {
namespace library {

// ============================================================================
// Forward declarations
// ============================================================================

struct ColorScheme;
struct Typography;
struct Spacing;
struct Elevation;
struct Shape;
struct Theme;

// ============================================================================
// Color Scheme (Material 3 Color Roles)
// ============================================================================

/**
 * Material Design 3 color roles.
 * Each theme (light/dark) defines a full set of these.
 */
struct ColorScheme {
    // Core colors
    Color primary;
    Color onPrimary;
    Color primaryContainer;
    Color onPrimaryContainer;

    Color secondary;
    Color onSecondary;
    Color secondaryContainer;
    Color onSecondaryContainer;

    Color tertiary;
    Color onTertiary;
    Color tertiaryContainer;
    Color onTertiaryContainer;

    Color error;
    Color onError;
    Color errorContainer;
    Color onErrorContainer;

    // Surface colors
    Color background;
    Color onBackground;
    Color surface;
    Color onSurface;
    Color surfaceVariant;
    Color onSurfaceVariant;
    Color surfaceTint;

    // Inverse
    Color inverseSurface;
    Color inverseOnSurface;
    Color inversePrimary;

    // Utility surfaces
    Color outline;
    Color outlineVariant;
    Color shadow;
    Color scrim;

    // Chat-specific additions
    Color mention;
    Color onMention;
    Color reply;
    Color onReply;
    Color linkColor;
    Color bubbleSent;
    Color onBubbleSent;
    Color bubbleReceived;
    Color onBubbleReceived;
    Color codeBlock;
    Color onCodeBlock;
    Color divider;
    Color success;
    Color onSuccess;
    Color warning;
    Color onWarning;

    /** Check if two ColorSchemes are equal. */
    bool operator==(const ColorScheme& other) const;
    bool operator!=(const ColorScheme& other) const { return !(*this == other); }
};

// ============================================================================
// Typography
// ============================================================================

/** Font weight enum matching CSS/Android conventions. */
enum class FontWeight {
    Thin = 100, ExtraLight = 200, Light = 300, Regular = 400,
    Medium = 500, SemiBold = 600, Bold = 700, ExtraBold = 800, Black = 900
};

/** Font style. */
enum class FontStyle { Normal, Italic };

/** A single typography style definition. */
struct TextStyle {
    std::string fontFamily = "sans-serif";
    float fontSize = 14.0f;         // in sp (scalable pixels)
    FontWeight fontWeight = FontWeight::Regular;
    FontStyle fontStyle = FontStyle::Normal;
    float letterSpacing = 0.0f;     // in em
    float lineHeight = 1.4f;        // multiplier
    bool isMonospace = false;

    TextStyle() = default;
    TextStyle(const std::string& family, float size, FontWeight weight = FontWeight::Regular,
              FontStyle style = FontStyle::Normal, float spacing = 0.0f, float lineH = 1.4f)
        : fontFamily(family), fontSize(size), fontWeight(weight), fontStyle(style),
          letterSpacing(spacing), lineHeight(lineH) {}
};

/** Material Design 3 typography scale. */
struct Typography {
    // Display styles
    TextStyle displayLarge;    // ~57sp, Regular — for hero text
    TextStyle displayMedium;   // ~45sp, Regular
    TextStyle displaySmall;    // ~36sp, Regular

    // Headline styles
    TextStyle headlineLarge;   // ~32sp, Regular
    TextStyle headlineMedium;  // ~28sp, Regular
    TextStyle headlineSmall;   // ~24sp, Regular

    // Title styles
    TextStyle titleLarge;      // ~22sp, Regular
    TextStyle titleMedium;     // ~16sp, Medium — for app bars
    TextStyle titleSmall;      // ~14sp, Medium — for tabs, lists

    // Body styles
    TextStyle bodyLarge;       // ~16sp, Regular
    TextStyle bodyMedium;      // ~14sp, Regular — default body text
    TextStyle bodySmall;       // ~12sp, Regular — captions

    // Label styles
    TextStyle labelLarge;      // ~14sp, Medium — buttons
    TextStyle labelMedium;     // ~12sp, Medium
    TextStyle labelSmall;      // ~11sp, Medium — overlines, small labels

    // Chat-specific additions
    TextStyle messageText;     // body text in chat bubbles
    TextStyle messageTimestamp; // timestamps in chat
    TextStyle codeText;        // monospace for code blocks
    TextStyle notificationText; // notification messages
};

// ============================================================================
// Spacing (Material 3 Density Scale)
// ============================================================================

/**
 * Spacing tokens following Material Design 3 density guidelines.
 * Values in dp (density-independent pixels).
 * Base grid is 4dp.
 */
struct Spacing {
    float none = 0.0f;          // 0dp
    float extraSmall = 4.0f;    // 4dp  — between icons and text
    float small = 8.0f;         // 8dp  — between related elements
    float medium = 12.0f;       // 12dp — between items in a list
    float base = 16.0f;         // 16dp — screen edges, card padding
    float large = 24.0f;        // 24dp — between sections
    float extraLarge = 32.0f;   // 32dp — between unrelated sections
    float xxl = 48.0f;          // 48dp — large section separators
    float xxxl = 64.0f;         // 64dp — hero section padding

    // Specific layout tokens
    float screenHorizontal = 16.0f;
    float screenVertical = 12.0f;
    float cardPadding = 16.0f;
    float buttonPaddingHorizontal = 24.0f;
    float buttonPaddingVertical = 10.0f;
    float inputPaddingHorizontal = 12.0f;
    float inputPaddingVertical = 8.0f;
    float listItemPadding = 16.0f;
    float avatarSize = 40.0f;
    float avatarSizeSmall = 32.0f;
    float avatarSizeLarge = 56.0f;
    float iconSize = 24.0f;
    float iconSizeSmall = 18.0f;
    float iconSizeLarge = 36.0f;
    float bottomNavHeight = 56.0f;
    float topBarHeight = 56.0f;
    float snackbarMinHeight = 48.0f;
    float dialogMinWidth = 280.0f;
    float dialogMaxWidth = 560.0f;
    float chatBubbleMaxWidth = 300.0f;
    float chatBubbleRadius = 18.0f;
    float mentionBadgeSize = 18.0f;
    float progressBarHeight = 4.0f;
    float thumbSize = 40.0f;
    float waveformBarWidth = 3.0f;
    float waveformBarGap = 1.0f;

    /** Return spacing scaled by a factor (for accessibility or density modes). */
    Spacing scaled(float factor) const;
};

// ============================================================================
// Elevation / Shadows
// ============================================================================

/**
 * Elevation levels following Material Design 3.
 * Each level defines a shadow (elevation in dp) and optionally a surface color shift.
 */
struct ElevationLevel {
    float elevation = 0.0f;     // dp (affects shadow intensity)
    float alphaMultiplier = 1.0f; // surface color opacity adjustment

    ElevationLevel() = default;
    ElevationLevel(float elev, float alpha = 1.0f) : elevation(elev), alphaMultiplier(alpha) {}
};

struct Elevation {
    ElevationLevel level0{0.0f};     // Flat
    ElevationLevel level1{1.0f};     // Cards, buttons (resting)
    ElevationLevel level2{3.0f};     // FAB (resting), cards (hover)
    ElevationLevel level3{6.0f};     // FAB (pressed), app bar
    ElevationLevel level4{8.0f};     // Dialogs, menus
    ElevationLevel level5{12.0f};    // Drawers, bottom sheets
    ElevationLevel level6{16.0f};    // Modals (dimming overlay)
    ElevationLevel level7{24.0f};    // Toasts, tooltips
};

// ============================================================================
// Shape (Corner Radii)
// ============================================================================

struct Shape {
    float none = 0.0f;       // 0dp  — sharp corners
    float extraSmall = 4.0f; // 4dp  — chips, small inputs
    float small = 8.0f;       // 8dp  — cards, dialogs, buttons
    float medium = 12.0f;     // 12dp — FAB, bottom sheets
    float large = 16.0f;      // 16dp — modal sheets
    float extraLarge = 28.0f; // 28dp — large overlays
    float full = 9999.0f;     // Pill shape / fully rounded

    // Component-specific
    float button = 20.0f;
    float chip = 8.0f;
    float card = 12.0f;
    float dialog = 28.0f;
    float snackbar = 4.0f;
    float textField = 4.0f;
    float badge = 9999.0f;
    float avatar = 9999.0f;
    float chatBubble = 18.0f;
    float progressBar = 9999.0f;
    float tabBar = 0.0f;
};

// ============================================================================
// Theme
// ============================================================================

/**
 * Complete Material Design 3 theme encompassing colors, typography,
 * spacing, elevation, and shapes.
 */
struct Theme {
    std::string name = "Default";
    ColorScheme colors;
    Typography typography;
    Spacing spacing;
    Elevation elevation;
    Shape shape;
    bool isDark = false;

    /** The seed color used to generate this theme (if generated). */
    Color seedColor;

    /** Generate a dark variant of this theme. */
    Theme dark() const;

    /** Generate a high-contrast variant. */
    Theme highContrast() const;

    /** Check contrast for a specific color pair against WCAG requirements. */
    bool passesContrast(const Color& foreground, const Color& background,
                         float minimumRatio = 4.5f) const;

    /** Get suggested text color for the given surface (auto-picks onSurface/onPrimary/etc). */
    Color textColorForSurface(const Color& surfaceColor) const;

    /** Format to JSON for serialization. */
    std::string toJson() const;

    /** Load from JSON. */
    static Theme fromJson(const std::string& json);
};

// ============================================================================
// Theme Builder / Manager
// ============================================================================

/**
 * Generates Material Design 3 color schemes from a seed color using
 * tonal palette generation (HCT color space approximation).
 */
class ThemeBuilder {
public:
    ThemeBuilder() = default;

    /**
     * Generate a complete light theme from a seed color.
     * Uses HCT-like tonal palette generation.
     */
    static Theme generateLight(const Color& seed);

    /**
     * Generate a complete dark theme from a seed color.
     */
    static Theme generateDark(const Color& seed);

    /**
     * Generate both light and dark themes from a seed color.
     */
    static std::pair<Theme, Theme> generatePair(const Color& seed);

    /**
     * Generate a tonal palette: an array of 15 tones (0, 10, 20, ..., 100)
     * derived from the given hue and chroma.
     */
    static std::array<Color, 15> tonalPalette(float hue, float chroma);

    /**
     * Get a specific tonal value from a palette.
     * Tonal values are 0-100 in steps of ~7 (Material You convention).
     */
    static Color tonalColor(float hue, float chroma, float tone);

    /**
     * Build a Typography scale using the given font family as base.
     */
    static Typography buildTypography(const std::string& fontFamily = "sans-serif");

    /**
     * Generate a color scheme using Material 3 token naming from core colors.
     */
    static ColorScheme buildColorScheme(const Color& primary,
                                          const Color& secondary,
                                          const Color& tertiary,
                                          const Color& error,
                                          bool isDark);

    /**
     * Check whether two colors have sufficient contrast for accessibility.
     */
    static bool hasSufficientContrast(const Color& a, const Color& b,
                                       float minRatio = 4.5f);

    /**
     * Find a color with sufficient contrast against a background.
     * Tries the foreground first, then darkens/lightens until contrast passes.
     */
    static Color ensureContrast(const Color& foreground, const Color& background,
                                  float minRatio = 4.5f);

    /**
     * Returns the Material You predefined colors for a given primary.
     */
    static ColorScheme materialYouLight(const Color& seed);
    static ColorScheme materialYouDark(const Color& seed);

private:
    /**
     * HCT (Hue, Chroma, Tone) color system helpers.
     */
    struct Cam16 {
        float hue = 0.0f;
        float chroma = 0.0f;
        float j = 0.0f;      // lightness
        float q = 0.0f;      // brightness
        float m = 0.0f;      // colorfulness
        float s = 0.0f;      // saturation
    };

    static Cam16 toCam16(const Color& color);
    static Color fromCam16(const Cam16& cam);
    static float lerpTone(float tone);
};

/**
 * Global theme manager singleton.
 * Manages the current active theme and provides component-level tokens.
 */
class ThemeManager {
public:
    static ThemeManager& instance();

    /** Set the active theme. */
    void setTheme(const Theme& theme);
    void setTheme(Theme&& theme);

    /** Get the current theme. */
    const Theme& getTheme() const;
    Theme& mutableTheme();

    /** Toggle between light and dark variants. */
    void setDarkMode(bool dark);

    /** Check if dark mode is active. */
    bool isDarkMode() const;

    /**
     * Get a component-level color token.
     * "button_bg", "button_text", "input_bg", "input_border",
     * "card_bg", "bubble_sent_bg", "bubble_received_bg",
     * "nav_bar_bg", "status_bar_bg", "badge_mention",
     * "divider_color", "toolbar_bg", "toolbar_text", etc.
     */
    Color getColor(const std::string& token) const;
    Color getColor(const std::string& token, const Color& fallback) const;

    /** Get a component-level spacing token. */
    float getSpacing(const std::string& token) const;
    float getSpacing(const std::string& token, float fallback) const;

    /** Get a shape (corner radius) token. */
    float getShape(const std::string& token) const;

    /** Get an elevation level. */
    ElevationLevel getElevation(const std::string& token) const;

    /** Register a custom component token (for extensibility). */
    void setColorToken(const std::string& token, const Color& color);
    void setSpacingToken(const std::string& token, float value);

    /** Get the theme as JSON. */
    std::string toJson() const;

private:
    ThemeManager();
    Theme currentTheme_;
    Theme lightTheme_;
    Theme darkTheme_;

    // Component token overrides
    std::unordered_map<std::string, Color> colorTokens_;
    std::unordered_map<std::string, float> spacingTokens_;

    void rebuildComponentTokens();
    void registerDefaultTokens();
};

} // namespace library
} // namespace pcp
