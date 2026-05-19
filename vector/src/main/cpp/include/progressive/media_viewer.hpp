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
//   8. MediaViewerItem / MediaViewerList
//   9. Download state tracking
//  10. Cache path computation
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

// ---- Media Download State ----

// Original Kotlin: MediaViewerViewModel.kt — download progress tracking
enum class MediaDownloadState {
    NOT_DOWNLOADED = 0,
    DOWNLOADING = 1,
    DOWNLOADED = 2,
    FAILED = 3
};

// Original Kotlin: MediaViewerViewModel.kt
struct MediaDownloadProgress {
    int64_t bytesDownloaded = 0;
    int64_t totalBytes = 0;
    double percentage = 0.0;        // 0.0 - 100.0
};

// ---- Media Viewer Item ----

// Original Kotlin: MediaViewerItem.kt, MediaViewerListItem.kt
struct MediaViewerItem {
    std::string url;                // mxc:// or http URL
    std::string mimeType;
    std::string title;
    std::string description;
    int width = 0;
    int height = 0;
    int64_t fileSize = 0;
    bool isDownloaded = false;
    std::string thumbnailUrl;
    std::string authorInfo;         // sender display name or user ID
};

// Original Kotlin: MediaViewerList.kt
struct MediaViewerList {
    std::vector<MediaViewerItem> items;
    int startIndex = 0;             // which item to show first
    std::string roomTitle;
};

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

// ---- Conversion helpers ----

// Convert raw EXIF orientation value (1-8) to enum.
ExifOrientation exifFromRaw(int rawValue);

// Get rotation degrees from EXIF orientation (0/90/180/270).
int exifRotationDegrees(ExifOrientation orient);

// Check if EXIF orientation requires flip.
bool exifRequiresFlip(ExifOrientation orient);

// ---- MXC URL Utilities ----

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
void applyExifRotation(MediaInfo& info);

// Get the display dimensions after EXIF rotation.
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

// ---- Media Viewer List Builder ----

// Original Kotlin: MediaViewerViewModel.kt — build viewer list from timeline events
MediaViewerList buildMediaViewerList(
    const std::vector<std::string>& eventContentsJson,   // array of content JSON strings
    const std::string& roomTitle,
    int startIndex = 0
);

// Original Kotlin: ImageViewerViewModel.kt — find adjacent media events
// Returns (prevIndex, nextIndex) relative to the current item.
// (-1 means not found)
struct AdjacentMedia {
    int prevIndex = -1;
    int nextIndex = -1;
};
AdjacentMedia findAdjacentMedia(const MediaViewerList& list, int currentIndex);

// ---- Media Download Utils ----

// Original Kotlin: MediaDownloader.kt, MediaViewerViewModel.kt
bool isMediaDownloadable(const std::string& mxcUrl);

// Original Kotlin: FileService.kt, MediaCache.kt
// Compute cache file path for a media item.
// Uses cacheDir/mxc_media/{serverName}_{mediaId}_{filename}
std::string getMediaCachePath(const std::string& mxcUrl, const std::string& cacheDir,
                              const std::string& fileName = "");

} // namespace progressive
