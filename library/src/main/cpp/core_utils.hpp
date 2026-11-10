#pragma once

/**
 * core_utils.hpp - Core utility functions for the PCP chat client library
 *
 * Provides UUID generation, SHA256 hashing, Base64 encoding/decoding,
 * string manipulation, time formatting, math helpers, and color utilities.
 * All functions live in pcp::library namespace.
 *
 * Dependencies: nlohmann/json (for JSON needs), <android/log.h> for logging.
 */

#include <string>
#include <vector>
#include <cstdint>
#include <chrono>
#include <cmath>
#include <optional>
#include <functional>
#include <unordered_map>
#include <sstream>
#include <iomanip>
#include <random>
#include <algorithm>
#include <cctype>
#include <array>

namespace pcp {
namespace library {

// ============================================================================
// UUID Generation
// ============================================================================

/** Generates a v4 UUID (random). Returns a lowercase string like "550e8400-e29b-41d4-a716-446655440000". */
std::string generateUUID();

/** Generates a v5 UUID (SHA1-based namespace). namespaceUUID is the namespace, name is the input. */
std::string generateUUIDv5(const std::string& namespaceUUID, const std::string& name);

/** Validates whether a string is a properly formatted UUID (v1-v5, case-insensitive). */
bool isValidUUID(const std::string& uuid);

/** Returns UUID variant and version info as a string. */
std::string getUUIDInfo(const std::string& uuid);

// ============================================================================
// SHA256 Hashing
// ============================================================================

/** SHA-256 digest context for streaming hashes. */
class SHA256Context {
public:
    SHA256Context();
    ~SHA256Context() = default;

    /** Reset context for new hash computation. */
    void reset();

    /** Feed data into the hash. */
    void update(const uint8_t* data, size_t length);
    void update(const std::string& data);

    /** Finalize and return the 32-byte hash. */
    std::array<uint8_t, 32> finalize();
    std::string finalizeHex();

private:
    uint32_t state[8];
    uint64_t bitCount;
    uint8_t buffer[64];
    size_t bufferLen;

    void transform(const uint8_t block[64]);
    void processBlock();
};

/** Compute SHA-256 hash of a string, returned as hex. */
std::string sha256(const std::string& input);
std::string sha256(const std::vector<uint8_t>& input);

/** HMAC-SHA256 implementation. */
std::string hmacSha256(const std::string& key, const std::string& message);

/** PBKDF2-HMAC-SHA256 key derivation. */
std::vector<uint8_t> pbkdf2Sha256(const std::string& password, const std::string& salt,
                                    size_t keyLength, unsigned int iterations);

// ============================================================================
// Base64 Encoding / Decoding
// ============================================================================

/** Encode raw bytes to Base64 string. urlSafe uses - and _ instead of + and /, and omits padding. */
std::string base64Encode(const uint8_t* data, size_t length, bool urlSafe = false);
std::string base64Encode(const std::string& data, bool urlSafe = false);
std::string base64Encode(const std::vector<uint8_t>& data, bool urlSafe = false);

/** Decode Base64 string to raw bytes. Returns empty vector on error. */
std::vector<uint8_t> base64Decode(const std::string& encoded);

/** Check if a string is valid Base64. */
bool isBase64(const std::string& str);

// ============================================================================
// String Utilities
// ============================================================================

/** Trim whitespace from start and end. */
std::string trim(const std::string& str);
std::string trimLeft(const std::string& str);
std::string trimRight(const std::string& str);

/** Split string by delimiter. */
std::vector<std::string> split(const std::string& str, const std::string& delimiter);
std::vector<std::string> split(const std::string& str, char delimiter);

/** Join strings with separator. */
std::string join(const std::vector<std::string>& parts, const std::string& separator);

/** Case conversion. */
std::string toUpper(const std::string& str);
std::string toLower(const std::string& str);

/** Check prefix/suffix/containment. */
bool startsWith(const std::string& str, const std::string& prefix);
bool endsWith(const std::string& str, const std::string& suffix);
bool contains(const std::string& str, const std::string& substring);

/** Replace all occurrences of 'from' with 'to'. */
std::string replaceAll(const std::string& str, const std::string& from, const std::string& to);

/** Normalize newlines to \n. Handles \r\n and \r. */
std::string normalizeNewlines(const std::string& str);

/** Escape HTML special characters. */
std::string escapeHtml(const std::string& str);

/** Unescape HTML entities. */
std::string unescapeHtml(const std::string& str);

/** URL encode/decode. */
std::string urlEncode(const std::string& str);
std::string urlDecode(const std::string& str);

/** Returns a random alphanumeric string of given length. */
std::string randomString(size_t length);

/** Truncate string to maxLen, appending "..." if truncated. */
std::string truncate(const std::string& str, size_t maxLen);

/** Case-insensitive comparison. */
bool iequals(const std::string& a, const std::string& b);

// ============================================================================
// Time Utilities
// ============================================================================

/** Format a time_point as ISO 8601 string (UTC). */
std::string formatTimestamp(const std::chrono::system_clock::time_point& tp);
std::string formatTimestamp(int64_t millisSinceEpoch);

/** Parse an ISO 8601 string into a time_point. Throws on invalid input. */
std::chrono::system_clock::time_point parseISO8601(const std::string& iso8601);

/** Format a duration in human-readable form (e.g., "2h 30m 15s"). */
std::string formatDuration(std::chrono::seconds duration);
std::string formatDuration(int64_t totalSeconds);

/** Relative time formatting. "just now", "5m ago", "2h ago", "3d ago", etc. */
std::string formatRelativeTime(const std::chrono::system_clock::time_point& past,
                                const std::chrono::system_clock::time_point& now =
                                    std::chrono::system_clock::now());
std::string formatRelativeTime(int64_t millisSinceEpoch);

/** "time ago" short form: "5m", "2h", "3d", "1w", "1mo", "1y". */
std::string timeAgo(const std::chrono::system_clock::time_point& past,
                     const std::chrono::system_clock::time_point& now =
                         std::chrono::system_clock::now());

/** Check if a given time_point falls on today/yesterday (local time). */
bool isToday(const std::chrono::system_clock::time_point& tp);
bool isYesterday(const std::chrono::system_clock::time_point& tp);

/** Get current time as milliseconds since epoch. */
int64_t nowMillis();

// ============================================================================
// Math Utilities
// ============================================================================

template<typename T>
T clamp(const T& value, const T& minVal, const T& maxVal) {
    return std::max(minVal, std::min(value, maxVal));
}

template<typename T>
T lerp(const T& a, const T& b, float t) {
    return static_cast<T>(a + (b - a) * t);
}

template<typename T>
T mapRange(const T& value, const T& inMin, const T& inMax, const T& outMin, const T& outMax) {
    return outMin + (outMax - outMin) * (static_cast<float>(value - inMin) / static_cast<float>(inMax - inMin));
}

/** Euclidean distance between two points. */
float distance(float x1, float y1, float x2, float y2);
double distance(double x1, double y1, double x2, double y2);

/** Angle in radians between two points relative to horizontal. Returns [-PI, PI]. */
float angle(float x1, float y1, float x2, float y2);

/** Conversion between degrees and radians. */
constexpr float toRadians(float degrees) { return degrees * M_PI / 180.0f; }
constexpr float toDegrees(float radians) { return radians * 180.0f / M_PI; }
constexpr double toRadians(double degrees) { return degrees * M_PI / 180.0; }
constexpr double toDegrees(double radians) { return radians * 180.0 / M_PI; }

/** Round to specified number of decimal places. */
float roundTo(float value, int decimalPlaces);
double roundTo(double value, int decimalPlaces);

/** Check if unsigned integer is a power of two. */
template<typename T>
std::enable_if_t<std::is_unsigned_v<T>, bool> isPowerOfTwo(T value) {
    return value != 0 && (value & (value - 1)) == 0;
}

/** Compute the next power of two >= value. */
template<typename T>
std::enable_if_t<std::is_unsigned_v<T>, T> nextPowerOfTwo(T value) {
    if (value == 0) return 1;
    value--;
    for (unsigned int shift = 1; shift < sizeof(T) * 8; shift <<= 1) {
        value |= value >> shift;
    }
    return value + 1;
}

/** Smoothstep interpolation (Hermite). */
float smoothstep(float edge0, float edge1, float x);
double smoothstep(double edge0, double edge1, double x);

/** Easing functions for animations. */
namespace easing {
    float linear(float t);
    float easeInQuad(float t);
    float easeOutQuad(float t);
    float easeInOutQuad(float t);
    float easeInCubic(float t);
    float easeOutCubic(float t);
    float easeInOutCubic(float t);
    float easeInSine(float t);
    float easeOutSine(float t);
    float easeInOutSine(float t);
    float easeOutBounce(float t);
    float easeInOutElastic(float t);
}

// ============================================================================
// Color Utilities
// ============================================================================

/** Represents an RGBA color (0-255 per channel). Optionally parsed from hex, named, or array. */
struct Color {
    uint8_t r = 0, g = 0, b = 0, a = 255;

    Color() = default;
    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) : r(r), g(g), b(b), a(a) {}

    bool operator==(const Color& other) const {
        return r == other.r && g == other.g && b == other.b && a == other.a;
    }
    bool operator!=(const Color& other) const { return !(*this == other); }
};

/** Represents HSV color. */
struct HSV {
    float h = 0.0f;   // 0-360
    float s = 0.0f;   // 0-1
    float v = 0.0f;   // 0-1
};

/** Parse a hex color string (#RGB, #RRGGBB, #RRGGBBAA). Returns nullopt on failure. */
std::optional<Color> parseColor(const std::string& hexString);

/** Convert Color to hex string (#RRGGBB or #RRGGBBAA if alpha != 255). */
std::string toHexString(const Color& color);
std::string toHexString(const Color& color, bool includeAlpha);

/** Blend two colors by t (0.0 = color a, 1.0 = color b). */
Color blend(const Color& a, const Color& b, float t);

/** Darken a color by a factor (0.0 = no change, 1.0 = black). */
Color darken(const Color& color, float factor);

/** Lighten a color by a factor (0.0 = no change, 1.0 = white). */
Color lighten(const Color& color, float factor);

/** Convert between RGB and HSV. */
HSV toHSV(const Color& color);
Color fromHSV(const HSV& hsv);

/** Compute WCAG contrast ratio between two colors. */
float contrastRatio(const Color& foreground, const Color& background);

/** Check if a color is considered "light" (luminance > 0.5). */
bool isLightColor(const Color& color);

/** Returns black or white text color based on contrast against background. */
Color getTextColor(const Color& backgroundColor);

/** Adjust the alpha channel. Returns new Color with given alpha. */
Color adjustAlpha(const Color& color, uint8_t alpha);

/** Convert a 32-bit ARGB integer to Color. */
Color fromARGB(uint32_t argb);

/** Convert Color to 32-bit ARGB integer. */
uint32_t toARGB(const Color& color);

/** Compute relative luminance per sRGB specification. */
float luminance(const Color& color);

/** Generate a complementary color. */
Color complementary(const Color& color);

/** Generate a triad of colors spaced 120 degrees apart. */
std::array<Color, 3> triadic(const Color& color);

/** Generate analogous colors. */
std::array<Color, 2> analogous(const Color& color);

/** Linear interpolation in CIELAB space (perceptually uniform). */
Color blendLab(const Color& a, const Color& b, float t);

} // namespace library
} // namespace pcp
