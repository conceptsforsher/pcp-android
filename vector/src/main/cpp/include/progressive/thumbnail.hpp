#ifndef PROGRESSIVE_THUMBNAIL_HPP
#define PROGRESSIVE_THUMBNAIL_HPP

#include <string>
#include <cstdint>

namespace progressive {

struct ImageSize {
    int width = 0;
    int height = 0;
    bool valid = false;
};

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

#endif // PROGRESSIVE_THUMBNAIL_HPP
