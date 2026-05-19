#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <cmath>

namespace progressive {

// ==== Media Utilities ====
//
// Thumbnail size calculation, EXIF orientation handling,
// image dimension estimation, and media filename sanitization.

// ==== Thumbnail Size Calculator ====

// Calculate optimal thumbnail dimensions for a given max size.
// Maintains aspect ratio.
inline void calculateThumbnailSize(
    int origW, int origH, int maxW, int maxH,
    int& outW, int& outH)
{
    if (origW <= 0 || origH <= 0) { outW = maxW; outH = maxH; return; }

    double ratio = (double)origW / origH;

    if (origW > maxW || origH > maxH) {
        if (ratio > 1.0) {
            outW = maxW;
            outH = (int)(maxW / ratio);
        } else {
            outH = maxH;
            outW = (int)(maxH * ratio);
        }
    } else {
        outW = origW;
        outH = origH;
    }
}

// ==== EXIF Orientation ====

// Apply EXIF orientation to swap/correct width and height.
// EXIF values: 1=normal, 6=90°CW, 8=90°CCW, 3=180°
inline void applyExifOrientation(int& width, int& height, int exifOrientation) {
    switch (exifOrientation) {
        case 5: case 6: case 7: case 8:
            // Swapped dimensions
            std::swap(width, height);
            break;
        default:
            break;
    }
}

// ==== Filename Sanitizer ====

// Sanitize a filename for safe storage and upload.
// Removes path separators, control characters, and limits length.
inline std::string sanitizeFilename(const std::string& filename, int maxLength = 255) {
    std::string result;
    for (char c : filename) {
        if (c == '/' || c == '\\' || c == ':' || c == '*' || c == '?' ||
            c == '"' || c == '<' || c == '>' || c == '|') continue;
        if (c < 32) continue; // Control characters
        result += c;
    }
    if ((int)result.size() > maxLength) {
        // Truncate but preserve extension
        auto dot = result.rfind('.');
        if (dot != std::string::npos && dot > 0) {
            std::string ext = result.substr(dot);
            int nameLen = maxLength - (int)ext.size();
            if (nameLen > 0) result = result.substr(0, nameLen) + ext;
        } else {
            result = result.substr(0, maxLength);
        }
    }
    return result.empty() ? "file" : result;
}

// ==== File Size Formatting ====

// Format file size in human-readable format.
// 1024 → "1.0 KB", 1048576 → "1.0 MB", etc.
inline std::string formatFileSize(int64_t bytes) {
    if (bytes < 1024) return std::to_string(bytes) + " B";

    const char* units[] = {"B", "KB", "MB", "GB", "TB"};
    int unit = 0;
    double size = (double)bytes;

    while (size >= 1024.0 && unit < 4) {
        size /= 1024.0;
        unit++;
    }

    char buf[32];
    if (size >= 100.0 || unit == 0) {
        snprintf(buf, sizeof(buf), "%.0f %s", size, units[unit]);
    } else if (size >= 10.0) {
        snprintf(buf, sizeof(buf), "%.1f %s", size, units[unit]);
    } else {
        snprintf(buf, sizeof(buf), "%.2f %s", size, units[unit]);
    }
    return buf;
}

// ==== Duration Formatting ====

// Format milliseconds as human-readable duration.
// 65000 → "1:05", 3661000 → "1:01:01"
inline std::string formatDuration(int64_t ms) {
    int totalSec = (int)(ms / 1000);
    int hours = totalSec / 3600;
    int minutes = (totalSec % 3600) / 60;
    int seconds = totalSec % 60;

    char buf[16];
    if (hours > 0) {
        snprintf(buf, sizeof(buf), "%d:%02d:%02d", hours, minutes, seconds);
    } else {
        snprintf(buf, sizeof(buf), "%d:%02d", minutes, seconds);
    }
    return buf;
}

// ==== Audio Waveform Generator ====

// Generate a simple waveform from audio samples.
// samples: raw audio samples (16-bit PCM)
// numBars: desired number of waveform bars
// Returns: normalized bar heights (0-1024)
inline std::vector<int> generateWaveform(const std::vector<int16_t>& samples, int numBars = 50) {
    std::vector<int> waveform(numBars, 0);
    if (samples.empty() || numBars <= 0) return waveform;

    int samplesPerBar = (int)samples.size() / numBars;
    if (samplesPerBar <= 0) samplesPerBar = 1;

    int maxAmp = 0;
    for (int i = 0; i < numBars; i++) {
        int64_t sum = 0;
        int count = 0;
        for (int j = i * samplesPerBar; j < std::min((int)samples.size(), (i+1)*samplesPerBar); j++) {
            sum += std::abs(samples[j]);
            count++;
        }
        int avg = count > 0 ? (int)(sum / count) : 0;
        waveform[i] = avg;
        if (avg > maxAmp) maxAmp = avg;
    }

    // Normalize to 0-1024
    if (maxAmp > 0) {
        for (int& v : waveform) {
            v = (v * 1024) / maxAmp;
        }
    }

    return waveform;
}

// ==== Message Preview Trimmer ====

// Trim a message body to a preview length.
// Truncates at word boundaries when possible.
inline std::string trimPreview(const std::string& text, int maxLen = 150,
    const std::string& suffix = "...")
{
    if ((int)text.size() <= maxLen) return text;

    // Try to break at the last space within limit
    auto pos = text.rfind(' ', maxLen);
    if (pos != std::string::npos && pos > maxLen / 2) {
        return text.substr(0, pos) + suffix;
    }

    return text.substr(0, maxLen) + suffix;
}

} // namespace progressive
