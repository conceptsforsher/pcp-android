#ifndef PROGRESSIVE_CHAT_FEATURES_HPP
#define PROGRESSIVE_CHAT_FEATURES_HPP

#include <string>
#include <cstdint>
#include <vector>

namespace progressive {

// ---- Timezone Display ----

struct TimezoneInfo {
    std::string id;         // e.g. "Europe/London", "America/New_York"
    std::string displayName; // e.g. "UTC+1 London", "UTC-5 New York"
    int utcOffsetMinutes = 0;
};

// Get a list of common timezones for the user to choose from
std::vector<TimezoneInfo> getCommonTimezones();

// Convert a UTC epoch timestamp (ms) to a formatted string in the given timezone.
// format: "12:34" for today, "May 12 12:34" for this year, "May 12, 2025 12:34" for older
std::string formatTimestampInTimezone(
    int64_t utcEpochMs,
    const std::string& timezoneId  // e.g. "Europe/Moscow"
);

// Get current timezone ID of the device
std::string getDeviceTimezoneId();

// Check if a timezone is valid
bool isValidTimezoneId(const std::string& id);

// ---- EXIF / Metadata Stripping ----

struct ExifStripResult {
    bool success = false;
    int64_t originalSize = 0;
    int64_t strippedSize = 0;
    std::string error;
};

// Check if a file likely contains EXIF/metadata based on MIME type.
bool fileHasMetadata(const std::string& mimeType);

// Get list of MIME types that support metadata stripping
std::string getStrippableMimeTypes();

} // namespace progressive

#endif // PROGRESSIVE_CHAT_FEATURES_HPP
