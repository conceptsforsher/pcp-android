#pragma once

#include <string>
#include <cstdint>

namespace progressive {

// ---- Thumbnail Method ----

// Original Kotlin: ThumbnailMethod.kt
enum class ThumbnailMethod {
    SCALE = 0,
    CROP = 1
};

constexpr const char* thumbnailMethodToString(ThumbnailMethod m) {
    switch (m) {
        case ThumbnailMethod::SCALE: return "scale";
        case ThumbnailMethod::CROP:  return "crop";
    }
    return "scale";
}

inline ThumbnailMethod thumbnailMethodFromString(const std::string& s) {
    if (s == "crop") return ThumbnailMethod::CROP;
    return ThumbnailMethod::SCALE;
}

// ---- Thumbnail Format ----

// Original Kotlin: ThumbnailFormat.kt
enum class ThumbnailFormat {
    PNG = 0,
    JPEG = 1,
    WEBP = 2
};

constexpr const char* thumbnailFormatToString(ThumbnailFormat f) {
    switch (f) {
        case ThumbnailFormat::PNG:  return "png";
        case ThumbnailFormat::JPEG: return "jpeg";
        case ThumbnailFormat::WEBP: return "webp";
    }
    return "jpeg";
}

inline ThumbnailFormat thumbnailFormatFromString(const std::string& s) {
    if (s == "png")  return ThumbnailFormat::PNG;
    if (s == "webp") return ThumbnailFormat::WEBP;
    return ThumbnailFormat::JPEG;
}

// ---- Thumbnail Request ----

// Original Kotlin: ThumbnailRequest.kt
struct ThumbnailRequest {
    std::string mxcUrl;
    int desiredWidth = 320;
    int desiredHeight = 240;
    ThumbnailMethod method = ThumbnailMethod::SCALE;
    ThumbnailFormat format = ThumbnailFormat::JPEG;
    bool animated = false;
};

// ---- Thumbnail Generation Config ----

// Original Kotlin: ThumbnailGenerationConfig.kt
struct ThumbnailGenerationConfig {
    bool enabled = true;
    int maxWidth = 800;
    int maxHeight = 600;
    int quality = 80;               // 1-100
    bool allowAnimated = true;
};

// ---- Existing: Image Size ----

struct ImageSize {
    int width = 0;
    int height = 0;
    bool valid = false;
};

// ---- Existing: Thumbnail Params ----

struct ThumbnailParams {
    int sourceW = 0;
    int sourceH = 0;
    int maxW = 800;      // max thumbnail width
    int maxH = 600;      // max thumbnail height
    bool upscale = false; // allow upscaling
    int quality = 80;    // JPEG quality 1-100
};

struct ThumbnailResult {
    int targetW = 0;
    int targetH = 0;
    double scale = 1.0;     // scale factor
    int64_t estimatedBytes = 0;
};

// ---- New: Thumbnail URL Builder (enhanced) ----

// Original Kotlin: MxcUrl.kt / ImageViewer.kt
// Build full thumbnail URL from a ThumbnailRequest (includes format param).
std::string buildThumbnailUrl(const ThumbnailRequest& request);

// ---- New: Thumbnail Response Parser ----

// Original Kotlin: ImageUploadService.kt — parse thumbnail response
// Returns (width, height, mimeType) from thumbnail response JSON.
struct ThumbnailResponseInfo {
    int width = 0;
    int height = 0;
    std::string mimeType;
    int64_t sizeBytes = 0;
};
ThumbnailResponseInfo parseThumbnailResponse(const std::string& responseJson);

// ---- New: Optimal Size Computation ----

// Original Kotlin: ImageUtils.kt — compute best thumbnail size for display
ThumbnailRequest getOptimalThumbnailSize(const std::string& mxcUrl, int displayWidth, int displayHeight);

// ---- New: Availability Check ----

// Original Kotlin: ImageUtils.kt — check if image can have thumbnail
bool isThumbnailAvailable(const std::string& mimeType);

// Original Kotlin: ThumbnailGeneration.kt — decision logic
bool shouldGenerateThumbnail(const ThumbnailGenerationConfig& config,
                             int sourceW, int sourceH, const std::string& mimeType);

// ---- Existing preserved functions ----

// Compute thumbnail dimensions preserving aspect ratio.
ThumbnailResult computeThumbnail(const ThumbnailParams& params);

// Compute scale factor to fit within max dimensions.
double computeFitScale(int srcW, int srcH, int maxW, int maxH, bool upscale);

// Check if an image needs to be thumbnailed (is larger than max).
bool needsThumbnail(int srcW, int srcH, int maxW, int maxH);

// Estimate JPEG file size from dimensions and quality.
int64_t estimateJpegSize(int w, int h, int quality);

// Compute resize dimensions for a given width (preserving AR).
ImageSize resizeToWidth(const ImageSize& src, int targetW);

// Compute resize dimensions for a given height (preserving AR).
ImageSize resizeToHeight(const ImageSize& src, int targetH);

// Get Matrix thumbnail server URL: mxc://server/id → thumbnail URL.
std::string buildThumbnailUrl(const std::string& mxcUri, int w, int h,
    const std::string& method = "scale", bool animated = false);

// Parse image dimensions from a JSON metadata response.
ImageSize parseImageSize(const std::string& infoJson);

} // namespace progressive
