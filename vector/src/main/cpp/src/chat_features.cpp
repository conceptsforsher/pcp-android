#include "progressive/chat_features.hpp"
#include <sstream>
#include <ctime>
#include <vector>
#include <algorithm>

namespace progressive {

// ---- Timezone ----

std::vector<TimezoneInfo> getCommonTimezones() {
    // Major timezones grouped by UTC offset
    std::vector<TimezoneInfo> zones = {
        {"Pacific/Midway",     "UTC-11 Midway", -660},
        {"Pacific/Honolulu",   "UTC-10 Hawaii", -600},
        {"America/Anchorage",  "UTC-9 Alaska", -540},
        {"America/Los_Angeles","UTC-8 Pacific", -480},
        {"America/Denver",     "UTC-7 Mountain", -420},
        {"America/Chicago",    "UTC-6 Central", -360},
        {"America/New_York",   "UTC-5 Eastern", -300},
        {"America/Halifax",    "UTC-4 Atlantic", -240},
        {"America/Sao_Paulo",  "UTC-3 Brasilia", -180},
        {"Atlantic/South_Georgia","UTC-2 S. Georgia", -120},
        {"Atlantic/Azores",    "UTC-1 Azores", -60},
        {"UTC",                "UTC+0 Universal", 0},
        {"Europe/London",      "UTC+0 London", 0},
        {"Europe/Paris",       "UTC+1 Paris/CET", 60},
        {"Europe/Helsinki",    "UTC+2 Helsinki/EET", 120},
        {"Europe/Moscow",      "UTC+3 Moscow", 180},
        {"Asia/Dubai",         "UTC+4 Dubai", 240},
        {"Asia/Karachi",       "UTC+5 Karachi", 300},
        {"Asia/Dhaka",         "UTC+6 Dhaka", 360},
        {"Asia/Bangkok",       "UTC+7 Bangkok", 420},
        {"Asia/Shanghai",      "UTC+8 China", 480},
        {"Asia/Tokyo",         "UTC+9 Tokyo", 540},
        {"Australia/Sydney",   "UTC+10 Sydney", 600},
        {"Pacific/Noumea",     "UTC+11 Noumea", 660},
        {"Pacific/Auckland",   "UTC+12 Auckland", 720},
    };
    return zones;
}

bool isValidTimezoneId(const std::string& id) {
    auto zones = getCommonTimezones();
    for (const auto& z : zones) {
        if (z.id == id) return true;
    }
    return !id.empty(); // Allow custom timezone strings too
}

std::string getDeviceTimezoneId() {
    // Use C library timezone
    tzset();
    // Try to get timezone name via environment or locale
    // On Android, this is typically set to "UTC" or the system timezone
    const char* tz = getenv("TZ");
    if (tz && tz[0]) return std::string(tz);

    // Fallback: compute offset and match
    time_t now = time(nullptr);
    struct tm local;
    localtime_r(&now, &local);

    // Try to deduce from common timezones
    for (const auto& z : getCommonTimezones()) {
        // Simple offset matching (not DST-aware but good enough)
        if (z.utcOffsetMinutes == -local.tm_gmtoff / 60) {
            return z.id;
        }
    }

    return "UTC";
}

std::string formatTimestampInTimezone(int64_t utcEpochMs, const std::string& timezoneId) {
    if (utcEpochMs <= 0) return "";

    // Save old TZ
    const char* oldTz = getenv("TZ");
    std::string oldTzStr = oldTz ? oldTz : "";

    // Set target timezone
    setenv("TZ", timezoneId.c_str(), 1);
    tzset();

    time_t ts = utcEpochMs / 1000;
    struct tm result;
    localtime_r(&ts, &result);

    // Restore old TZ
    if (!oldTzStr.empty()) {
        setenv("TZ", oldTzStr.c_str(), 1);
    } else {
        unsetenv("TZ");
    }
    tzset();

    // Get current time for context (today vs this year vs older)
    time_t now = time(nullptr);
    struct tm nowTm;
    localtime_r(&now, &nowTm);

    char buf[64];
    if (result.tm_year == nowTm.tm_year && result.tm_mon == nowTm.tm_mon && result.tm_mday == nowTm.tm_mday) {
        // Today: just time
        strftime(buf, sizeof(buf), "%H:%M", &result);
    } else if (result.tm_year == nowTm.tm_year) {
        // This year: month day time
        strftime(buf, sizeof(buf), "%b %d %H:%M", &result);
    } else {
        // Older: full date
        strftime(buf, sizeof(buf), "%b %d, %Y %H:%M", &result);
    }

    return std::string(buf);
}

// ---- EXIF ----

bool fileHasMetadata(const std::string& mimeType) {
    // Types that commonly carry EXIF/XMP/metadata
    static const char* types[] = {
        "image/jpeg", "image/jpg", "image/tiff", "image/tif",
        "image/heic", "image/heif", "image/webp", "image/png",
        "image/raw", "image/dng", "audio/mpeg", "audio/mp3",
        "audio/mp4", "audio/m4a", "video/mp4", "video/quicktime",
        "video/x-m4v"
    };
    for (const auto& t : types) {
        if (mimeType == t) return true;
    }
    return false;
}

std::string getStrippableMimeTypes() {
    return "image/jpeg, image/tiff, image/heic, image/webp, image/png, "
           "audio/mpeg, audio/mp4, video/mp4, video/quicktime";
}

} // namespace progressive
