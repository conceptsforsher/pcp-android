#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// ================================================================
// Media Viewer — image/video viewer utilities
//
// Ported from Element Android:
//   MediaViewerViewModel.kt, ImageViewerViewModel.kt
//   VideoMediaViewerActivity.kt
//   MxcUrl.kt, MediaContent.kt
//
// Covers:
//   1. EXIF orientation parsing and rotation
//   2. Thumbnail URL generation (MXC → HTTP)
//   3. Image info extraction
//   4. Video info (duration, resolution)
//   5. Media type detection
//   6. Zoom/pan constraints
//   7. Media size formatting
// ================================================================

// ---- Media Type ----

enum class MediaType {
    UNKNOWN = 0,
    IMAGE = 1,
    VIDEO = 2,
    AUDIO = 3,
    FILE = 4,
    STICKER = 5,
    GIF = 6,
};

// ---- EXIF Orientation ----

enum class ExifOrientation {
    NORMAL = 0,          // 1 — No rotation
    FLIP_H = 1,          // 2 — Flip horizontal
    ROTATE_180 = 2,      // 3 — 180° rotation
    FLIP_V = 3,          // 4 — Flip vertical
    ROTATE_90_FLIP = 4,  // 5 — 90° CW + flip horizontal
    ROTATE_90 = 5,       // 6 — 90° CW
    ROTATE_270_FLIP = 6, // 7 — 270° CW + flip horizontal
    ROTATE_270 = 7,      // 8 — 270° CW (or 90° CCW)
};

// Convert raw EXIF orientation value (1-8) to enum.
ExifOrientation exifFromRaw(int rawValue);

// Get rotation degrees from EXIF orientation (0/90/180/270).
int exifRotationDegrees(ExifOrientation orient);

// Check if EXIF orientation requires flip.
bool exifRequiresFlip(ExifOrientation orient);

// ---- Media Info ----

struct MediaInfo {
    std::string mxcUrl;              // mxc://server/media_id
    std::string downloadUrl;         // Resolved HTTP download URL
    std::string thumbnailUrl;        // Resolved HTTP thumbnail URL
    std::string mimeType;            // "image/jpeg", "video/mp4", etc.
    std::string fileName;            // Original file name
    std::string body;                // Message body text
    MediaType type = MediaType::IMAGE;
    int width = 0;                   // Pixel width
    int height = 0;                  // Pixel height
    int64_t sizeBytes = 0;           // File size in bytes
    int durationMs = 0;              // Video/audio duration in ms
    ExifOrientation exifOrientation = ExifOrientation::NORMAL;
    bool isEncrypted = false;        // E2EE encrypted file
    bool hasThumbnail = false;       // Has thumbnail available
};

// ---- Thumbnail Config ----

struct ThumbnailConfig {
    int width = 320;                 // Target width
    int height = 240;                // Target height
    std::string method = "scale";    // "scale" or "crop"
    bool animated = false;           // Preserve animation (GIF)
};

// ---- Zoom / Pan State ----

struct ViewportState {
    double scale = 1.0;              // Current zoom level
    double minScale = 0.5;           // Minimum zoom (fit)
    double maxScale = 5.0;           // Maximum zoom
    double offsetX = 0.0;            // Pan X offset
    double offsetY = 0.0;            // Pan Y offset
    int viewportWidth = 1080;        // Screen width
    int viewportHeight = 1920;       // Screen height
    int mediaWidth = 1920;           // Media width (after rotation)
    int mediaHeight = 1080;          // Media height (after rotation)
};

// ---- Media Viewer Utilities ----

// Resolve MXC URL to HTTP download URL.
// mxc://example.org/abc123 → https://example.org/_matrix/media/r0/download/example.org/abc123
std::string resolveMxcDownloadUrl(const std::string& mxcUrl, const std::string& homeServer);

// Resolve MXC URL to HTTP thumbnail URL with config.
std::string resolveMxcThumbnailUrl(const std::string& mxcUrl, const std::string& homeServer,
                                    const ThumbnailConfig& config);

// Extract MXC server name from URL.
std::string extractMxcServerName(const std::string& mxcUrl);

// Extract MXC media ID from URL.
std::string extractMxcMediaId(const std::string& mxcUrl);

// Build MXC URL from server name and media ID.
std::string buildMxcUrl(const std::string& serverName, const std::string& mediaId);

// ---- Media Type Detection ----

MediaType detectMediaType(const std::string& mimeType);
bool isMimeTypeImage(const std::string& mimeType);
bool isMimeTypeVideo(const std::string& mimeType);
bool isMimeTypeAudio(const std::string& mimeType);

// ---- Media Info Extraction ----

// Parse media info from Matrix event content JSON.
MediaInfo parseMediaInfo(const std::string& contentJson);

// Get a human-readable media type name.
std::string getMediaTypeName(MediaType type);
std::string getMediaTypeNameFromMime(const std::string& mimeType);

// ---- Media Size Formatting ----

// Format file size as human-readable string.
// 1024 → "1 KB", 1048576 → "1 MB", 1073741824 → "1 GB"
std::string formatMediaSize(int64_t bytes);
std::string formatMediaSizeDouble(double bytes);

// Format video duration as "MM:SS" or "HH:MM:SS".
std::string formatMediaDuration(int durationMs);

// ---- EXIF / Rotation ----

// Apply EXIF rotation to get actual display dimensions.
// If EXIF says rotate 90°, swap width/height.
void applyExifRotation(MediaInfo& info);

// Get the display dimensions after EXIF rotation.
// Returns (displayWidth, displayHeight).
struct DisplayDimensions {
    int width = 0;
    int height = 0;
};
DisplayDimensions getDisplayDimensions(const MediaInfo& info);

// ---- Zoom / Pan ----

// Calculate initial fit-to-screen scale.
double calculateFitScale(int mediaWidth, int mediaHeight, int viewportWidth, int viewportHeight);

// Calculate viewport state for media display.
ViewportState calculateViewport(const MediaInfo& info, int viewportWidth, int viewportHeight);

// Clamp zoom level to min/max bounds.
double clampZoom(double scale, const ViewportState& state);

// Clamp pan offset to keep media visible.
void clampPan(ViewportState& state);

// ---- Thumbnail / Cache ----

// Build thumbnail cache key from MXC URL and config.
std::string buildThumbnailCacheKey(const std::string& mxcUrl, const ThumbnailConfig& config);

// Check if thumbnail is available for this media type.
bool canGenerateThumbnail(const std::string& mimeType);

// Get recommended thumbnail size for the given screen density.
ThumbnailConfig getRecommendedThumbnailConfig(int screenDensity = 320);

} // namespace progressive
