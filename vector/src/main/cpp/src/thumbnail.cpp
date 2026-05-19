#include "progressive/thumbnail.hpp"
#include <sstream>
#include <cmath>
#include <algorithm>

namespace progressive {

// ================================================================
// Thumbnail URL Builder (enhanced — takes ThumbnailRequest)
// ================================================================

// Original Kotlin: MxcUrl.kt, ImageViewer.kt
std::string buildThumbnailUrl(const ThumbnailRequest& request) {
    auto mxcUri = request.mxcUrl;
    if (mxcUri.rfind("mxc://", 0) != 0) return {};

    auto rest = mxcUri.substr(6);
    auto slash = rest.find('/');
    if (slash == std::string::npos) return {};

    std::string server = rest.substr(0, slash);
    std::string mediaId = rest.substr(slash + 1);

    std::ostringstream url;
    url << "/_matrix/media/v3/thumbnail/" << server << "/" << mediaId
        << "?width=" << request.desiredWidth
        << "&height=" << request.desiredHeight
        << "&method=" << thumbnailMethodToString(request.method)
        << "&format=" << thumbnailFormatToString(request.format);
    if (request.animated) url << "&animated=true";

    return url.str();
}

// ================================================================
// Thumbnail Response Parser
// ================================================================

// Original Kotlin: ImageUploadService.kt
ThumbnailResponseInfo parseThumbnailResponse(const std::string& responseJson) {
    ThumbnailResponseInfo info;

    // Parse JSON manually for width/height/mimeType
    for (const auto* key : {"w", "width"}) {
        auto pp = responseJson.find('"' + std::string(key) + '"');
        if (pp != std::string::npos) {
            pp = responseJson.find(':', pp);
            if (pp != std::string::npos) {
                pp++;
                while (pp < responseJson.size() && (responseJson[pp] == ' ' || responseJson[pp] == '\t')) pp++;
                while (pp < responseJson.size() && std::isdigit(responseJson[pp])) {
                    info.width = info.width * 10 + (responseJson[pp] - '0');
                    pp++;
                }
            }
            break;
        }
    }

    for (const auto* key : {"h", "height"}) {
        auto pp = responseJson.find('"' + std::string(key) + '"');
        if (pp != std::string::npos) {
            pp = responseJson.find(':', pp);
            if (pp != std::string::npos) {
                pp++;
                while (pp < responseJson.size() && (responseJson[pp] == ' ' || responseJson[pp] == '\t')) pp++;
                while (pp < responseJson.size() && std::isdigit(responseJson[pp])) {
                    info.height = info.height * 10 + (responseJson[pp] - '0');
                    pp++;
                }
            }
            break;
        }
    }

    // Extract size
    {
        auto pp = responseJson.find("\"size\"");
        if (pp != std::string::npos) {
            pp = responseJson.find(':', pp);
            if (pp != std::string::npos) {
                pp++;
                while (pp < responseJson.size() && (responseJson[pp] == ' ' || responseJson[pp] == '\t')) pp++;
                while (pp < responseJson.size() && std::isdigit(responseJson[pp])) {
                    info.sizeBytes = info.sizeBytes * 10 + (responseJson[pp] - '0');
                    pp++;
                }
            }
        }
    }

    // Extract MIME type
    {
        auto pp = responseJson.find("\"content_type\"");
        if (pp == std::string::npos) pp = responseJson.find("\"mimetype\"");
        if (pp == std::string::npos) pp = responseJson.find("\"mime_type\"");
        if (pp != std::string::npos) {
            pp = responseJson.find('"', pp + 1);
            if (pp != std::string::npos) {
                pp = responseJson.find('"', pp + 1);
                if (pp != std::string::npos) {
                    pp++;
                    size_t e = pp;
                    while (e < responseJson.size() && responseJson[e] != '"') e++;
                    info.mimeType = responseJson.substr(pp, e - pp);
                }
            }
        }
    }

    return info;
}

// ================================================================
// Optimal Thumbnail Size Computation
// ================================================================

// Original Kotlin: ImageUtils.kt
ThumbnailRequest getOptimalThumbnailSize(const std::string& mxcUrl, int displayWidth, int displayHeight) {
    ThumbnailRequest req;
    req.mxcUrl = mxcUrl;
    req.method = ThumbnailMethod::SCALE;
    req.format = ThumbnailFormat::JPEG;

    // Original Kotlin: choose size slightly larger than display for retina/HiDPI
    // Typically 2x display size, capped at common server limits
    int optimalW = std::min(displayWidth * 2, 2048);
    int optimalH = std::min(displayHeight * 2, 2048);

    // Ensure minimum viable size
    req.desiredWidth = std::max(optimalW, 32);
    req.desiredHeight = std::max(optimalH, 32);

    return req;
}

// ================================================================
// Thumbnail Availability / Generation Decision
// ================================================================

// Original Kotlin: ImageUtils.kt
bool isThumbnailAvailable(const std::string& mimeType) {
    if (mimeType.empty()) return false;

    auto lower = mimeType;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

    // Images and videos can have thumbnails
    if (lower.rfind("image/", 0) == 0) return true;
    if (lower.rfind("video/", 0) == 0) return true;

    return false;
}

// Original Kotlin: ThumbnailGeneration.kt — decision logic
bool shouldGenerateThumbnail(const ThumbnailGenerationConfig& config,
                             int sourceW, int sourceH, const std::string& mimeType) {
    if (!config.enabled) return false;

    // Check if media type supports thumbnails
    if (!isThumbnailAvailable(mimeType)) return false;

    // Don't generate thumbnails if source is already small enough
    if (sourceW <= config.maxWidth && sourceH <= config.maxHeight) return false;

    // Animated check
    auto lower = mimeType;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    if (lower == "image/gif" && !config.allowAnimated) return false;

    return true;
}

// ================================================================
// Existing: Compute Thumbnail
// ================================================================

ThumbnailResult computeThumbnail(const ThumbnailParams& params) {
    ThumbnailResult result;

    double scale = computeFitScale(params.sourceW, params.sourceH,
                                    params.maxW, params.maxH, params.upscale);

    result.scale = scale;
    result.targetW = static_cast<int>(params.sourceW * scale);
    result.targetH = static_cast<int>(params.sourceH * scale);

    result.targetW = std::max(1, result.targetW);
    result.targetH = std::max(1, result.targetH);

    result.estimatedBytes = estimateJpegSize(result.targetW, result.targetH, params.quality);

    return result;
}

double computeFitScale(int srcW, int srcH, int maxW, int maxH, bool upscale) {
    if (srcW <= 0 || srcH <= 0) return 1.0;

    double scaleW = static_cast<double>(maxW) / srcW;
    double scaleH = static_cast<double>(maxH) / srcH;
    double scale = std::min(scaleW, scaleH);

    if (!upscale && scale > 1.0) scale = 1.0;

    return scale;
}

bool needsThumbnail(int srcW, int srcH, int maxW, int maxH) {
    return srcW > maxW || srcH > maxH;
}

int64_t estimateJpegSize(int w, int h, int quality) {
    int pixels = w * h;
    double qFactor = quality / 100.0;
    double bpp = 0.5 + qFactor * 1.5;
    return static_cast<int64_t>((pixels * bpp) / 8.0);
}

ImageSize resizeToWidth(const ImageSize& src, int targetW) {
    if (src.width <= 0) return {};
    double ratio = static_cast<double>(src.height) / src.width;
    return {targetW, static_cast<int>(targetW * ratio), true};
}

ImageSize resizeToHeight(const ImageSize& src, int targetH) {
    if (src.height <= 0) return {};
    double ratio = static_cast<double>(src.width) / src.height;
    return {static_cast<int>(targetH * ratio), targetH, true};
}

// ================================================================
// Existing: Build Thumbnail URL (simple overload)
// ================================================================

std::string buildThumbnailUrl(const std::string& mxcUri, int w, int h,
                               const std::string& method, bool animated) {
    if (mxcUri.rfind("mxc://", 0) != 0) return {};

    auto rest = mxcUri.substr(6);
    auto slash = rest.find('/');
    if (slash == std::string::npos) return {};

    std::string server = rest.substr(0, slash);
    std::string mediaId = rest.substr(slash + 1);

    std::ostringstream url;
    url << "/_matrix/media/v3/thumbnail/" << server << "/" << mediaId
        << "?width=" << w << "&height=" << h << "&method=" << method;
    if (animated) url << "&animated=true";

    return url.str();
}

// ================================================================
// Existing: Parse Image Size
// ================================================================

ImageSize parseImageSize(const std::string& infoJson) {
    ImageSize size;

    auto wPos = infoJson.find("\"w\":");
    if (wPos == std::string::npos) wPos = infoJson.find("\"width\":");
    if (wPos != std::string::npos) {
        wPos = infoJson.find_first_of("0123456789", wPos);
        if (wPos != std::string::npos) {
            auto end = wPos;
            while (end < infoJson.size() && std::isdigit(infoJson[end])) ++end;
            size.width = std::stoi(infoJson.substr(wPos, end - wPos));
        }
    }

    auto hPos = infoJson.find("\"h\":");
    if (hPos == std::string::npos) hPos = infoJson.find("\"height\":");
    if (hPos != std::string::npos) {
        hPos = infoJson.find_first_of("0123456789", hPos);
        if (hPos != std::string::npos) {
            auto end = hPos;
            while (end < infoJson.size() && std::isdigit(infoJson[end])) ++end;
            size.height = std::stoi(infoJson.substr(hPos, end - hPos));
        }
    }

    size.valid = size.width > 0 && size.height > 0;
    return size;
}

} // namespace progressive
