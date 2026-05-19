#include "progressive/thumbnail.hpp"
#include <sstream>
#include <cmath>
#include <algorithm>

namespace progressive {

ThumbnailResult computeThumbnail(const ThumbnailParams& params) {
    ThumbnailResult result;

    double scale = computeFitScale(params.sourceW, params.sourceH,
                                    params.maxW, params.maxH, params.upscale);

    result.scale = scale;
    result.targetW = static_cast<int>(params.sourceW * scale);
    result.targetH = static_cast<int>(params.sourceH * scale);

    // Ensure at least 1x1
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
    // Rough estimate: pixels * quality_factor * compression_ratio
    int pixels = w * h;
    double qFactor = quality / 100.0;
    // JPEG compression: ~0.5-2.0 bits per pixel depending on quality
    double bpp = 0.5 + qFactor * 1.5; // 0.5 (q=0) → 2.0 (q=100) bits per pixel
    return static_cast<int64_t>((pixels * bpp) / 8.0); // bits → bytes
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

std::string buildThumbnailUrl(const std::string& mxcUri, int w, int h,
                               const std::string& method, bool animated) {
    // mxc://server/id → /_matrix/media/v3/thumbnail/server/id?width=w&height=h&method=method
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

ImageSize parseImageSize(const std::string& infoJson) {
    ImageSize size;

    // Parse "w": XXX from JSON
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
