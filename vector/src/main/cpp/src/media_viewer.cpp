#include "progressive/media_viewer.hpp"
#include <sstream>
#include <algorithm>
#include <cmath>
#include <cctype>

namespace progressive {

// ====== JSON helpers ======

static std::string extractStr(const std::string& json, const std::string& key) {
    auto pp = json.find("\"" + key + "\"");
    if (pp == std::string::npos) return "";
    pp = json.find('"', pp + key.size() + 2);
    if (pp == std::string::npos) return "";
    pp++;
    size_t e = pp;
    while (e < json.size() && json[e] != '"') e++;
    return json.substr(pp, e - pp);
}

static int64_t extractInt(const std::string& json, const std::string& key) {
    auto pp = json.find("\"" + key + "\"");
    if (pp == std::string::npos) return 0;
    pp = json.find(':', pp);
    if (pp == std::string::npos) return 0;
    pp++;
    while (pp < json.size() && (json[pp] == ' ' || json[pp] == '\t')) pp++;
    int64_t v = 0;
    while (pp < json.size() && json[pp] >= '0' && json[pp] <= '9') { v=v*10+(json[pp]-'0'); pp++; }
    return v;
}

static bool extractBool(const std::string& json, const std::string& key) {
    auto pp = json.find("\"" + key + "\"");
    if (pp == std::string::npos) return false;
    pp = json.find(':', pp);
    if (pp == std::string::npos) return false;
    pp++;
    while (pp < json.size() && (json[pp] == ' ' || json[pp] == '\t')) pp++;
    if (pp < json.size() && json[pp] == 't' && json.substr(pp, 4) == "true") return true;
    return false;
}

// ====== EXIF Orientation ======

ExifOrientation exifFromRaw(int rawValue) {
    switch (rawValue) {
        case 1: return ExifOrientation::NORMAL;
        case 2: return ExifOrientation::FLIP_H;
        case 3: return ExifOrientation::ROTATE_180;
        case 4: return ExifOrientation::FLIP_V;
        case 5: return ExifOrientation::ROTATE_90_FLIP;
        case 6: return ExifOrientation::ROTATE_90;
        case 7: return ExifOrientation::ROTATE_270_FLIP;
        case 8: return ExifOrientation::ROTATE_270;
        default: return ExifOrientation::NORMAL;
    }
}

int exifRotationDegrees(ExifOrientation orient) {
    switch (orient) {
        case ExifOrientation::ROTATE_180: return 180;
        case ExifOrientation::ROTATE_90: case ExifOrientation::ROTATE_90_FLIP: return 90;
        case ExifOrientation::ROTATE_270: case ExifOrientation::ROTATE_270_FLIP: return 270;
        default: return 0;
    }
}

bool exifRequiresFlip(ExifOrientation orient) {
    return orient == ExifOrientation::FLIP_H || orient == ExifOrientation::FLIP_V ||
           orient == ExifOrientation::ROTATE_90_FLIP || orient == ExifOrientation::ROTATE_270_FLIP;
}

// ====== MXC URL Utilities ======

std::string resolveMxcDownloadUrl(const std::string& mxcUrl, const std::string& homeServer) {
    auto server = extractMxcServerName(mxcUrl);
    auto mediaId = extractMxcMediaId(mxcUrl);
    if (server.empty() || mediaId.empty()) return "";

    std::string base = homeServer;
    if (base.empty()) base = "https://" + server;

    // Remove trailing slash
    while (!base.empty() && base.back() == '/') base.pop_back();

    return base + "/_matrix/media/r0/download/" + server + "/" + mediaId;
}

std::string resolveMxcThumbnailUrl(const std::string& mxcUrl, const std::string& homeServer,
                                    const ThumbnailConfig& config) {
    auto server = extractMxcServerName(mxcUrl);
    auto mediaId = extractMxcMediaId(mxcUrl);
    if (server.empty() || mediaId.empty()) return "";

    std::string base = homeServer;
    if (base.empty()) base = "https://" + server;
    while (!base.empty() && base.back() == '/') base.pop_back();

    std::ostringstream url;
    url << base << "/_matrix/media/r0/thumbnail/" << server << "/" << mediaId
        << "?width=" << config.width << "&height=" << config.height
        << "&method=" << config.method;
    if (config.animated) url << "&animated=true";
    return url.str();
}

std::string extractMxcServerName(const std::string& mxcUrl) {
    if (mxcUrl.compare(0, 6, "mxc://") != 0) return "";
    auto end = mxcUrl.find('/', 6);
    if (end == std::string::npos) return mxcUrl.substr(6);
    return mxcUrl.substr(6, end - 6);
}

std::string extractMxcMediaId(const std::string& mxcUrl) {
    if (mxcUrl.compare(0, 6, "mxc://") != 0) return "";
    auto end = mxcUrl.find('/', 6);
    if (end == std::string::npos) return "";
    return mxcUrl.substr(end + 1);
}

std::string buildMxcUrl(const std::string& serverName, const std::string& mediaId) {
    return "mxc://" + serverName + "/" + mediaId;
}

// ====== Media Type Detection ======

MediaType detectMediaType(const std::string& mimeType) {
    if (mimeType.compare(0, 6, "image/") == 0) {
        if (mimeType.find("gif") != std::string::npos) return MediaType::GIF;
        return MediaType::IMAGE;
    }
    if (mimeType.compare(0, 6, "video/") == 0) return MediaType::VIDEO;
    if (mimeType.compare(0, 6, "audio/") == 0) return MediaType::AUDIO;
    return MediaType::FILE;
}

bool isMimeTypeImage(const std::string& mimeType) { return mimeType.compare(0, 6, "image/") == 0; }
bool isMimeTypeVideo(const std::string& mimeType) { return mimeType.compare(0, 6, "video/") == 0; }
bool isMimeTypeAudio(const std::string& mimeType) { return mimeType.compare(0, 6, "audio/") == 0; }

// ====== Media Info Parsing ======

MediaInfo parseMediaInfo(const std::string& contentJson) {
    MediaInfo info;
    info.mxcUrl = extractStr(contentJson, "url");
    info.mimeType = extractStr(contentJson, "mimetype");
    if (info.mimeType.empty()) info.mimeType = extractStr(contentJson, "mime_type");
    info.fileName = extractStr(contentJson, "filename");
    if (info.fileName.empty()) info.fileName = extractStr(contentJson, "body");

    info.body = extractStr(contentJson, "body");

    // Parse info object
    auto infoBlock = contentJson;
    auto infoPos = infoBlock.find("\"info\"");
    if (infoPos != std::string::npos) {
        infoPos = infoBlock.find('{', infoPos);
        if (infoPos != std::string::npos) {
            int depth = 0;
            size_t start = infoPos;
            infoPos++;
            while (infoPos < infoBlock.size() && depth >= 0) {
                if (infoBlock[infoPos] == '{') depth++;
                else if (infoBlock[infoPos] == '}') depth--;
                if (depth == -1) break;
                infoPos++;
            }
            auto infoJson = infoBlock.substr(start, infoPos - start);
            info.width = static_cast<int>(extractInt(infoJson, "w"));
            if (info.width == 0) info.width = static_cast<int>(extractInt(infoJson, "width"));
            info.height = static_cast<int>(extractInt(infoJson, "h"));
            if (info.height == 0) info.height = static_cast<int>(extractInt(infoJson, "height"));
            info.sizeBytes = extractInt(infoJson, "size");
            info.durationMs = static_cast<int>(extractInt(infoJson, "duration"));
            info.hasThumbnail = infoJson.find("\"thumbnail_url\"") != std::string::npos ||
                                infoJson.find("\"thumbnail_info\"") != std::string::npos;

            auto thumbUrl = extractStr(infoJson, "thumbnail_url");
            if (!thumbUrl.empty()) info.thumbnailUrl = thumbUrl;
        }
    }

    // Thumbnail from top-level
    if (info.thumbnailUrl.empty()) {
        info.thumbnailUrl = extractStr(contentJson, "thumbnail_url");
    }

    // EXIF orientation
    int exifRaw = 1;
    auto exifPos = contentJson.find("\"org.matrix.msc2705.orientation\"");
    if (exifPos != std::string::npos) {
        exifRaw = static_cast<int>(extractInt(contentJson, "org.matrix.msc2705.orientation"));
    }
    if (exifRaw == 0 || exifRaw == 1) {
        exifRaw = static_cast<int>(extractInt(contentJson, "orientation"));
    }
    info.exifOrientation = exifFromRaw(std::max(1, exifRaw));

    // Type detection
    info.type = detectMediaType(info.mimeType);

    return info;
}

std::string getMediaTypeName(MediaType type) {
    switch (type) {
        case MediaType::IMAGE: return "Image";
        case MediaType::VIDEO: return "Video";
        case MediaType::AUDIO: return "Audio";
        case MediaType::FILE: return "File";
        case MediaType::STICKER: return "Sticker";
        case MediaType::GIF: return "GIF";
        default: return "Unknown";
    }
}

std::string getMediaTypeNameFromMime(const std::string& mimeType) {
    return getMediaTypeName(detectMediaType(mimeType));
}

// ====== Size Formatting ======

std::string formatMediaSize(int64_t bytes) {
    if (bytes < 1024) return std::to_string(bytes) + " B";
    double kb = static_cast<double>(bytes) / 1024.0;
    if (kb < 1024) {
        std::ostringstream os; os.precision(1); os << std::fixed << kb;
        return os.str() + " KB";
    }
    double mb = kb / 1024.0;
    if (mb < 1024) {
        std::ostringstream os; os.precision(1); os << std::fixed << mb;
        return os.str() + " MB";
    }
    double gb = mb / 1024.0;
    std::ostringstream os; os.precision(2); os << std::fixed << gb;
    return os.str() + " GB";
}

std::string formatMediaSizeDouble(double bytes) { return formatMediaSize(static_cast<int64_t>(bytes)); }

std::string formatMediaDuration(int durationMs) {
    int totalSec = durationMs / 1000;
    int hours = totalSec / 3600;
    int mins = (totalSec % 3600) / 60;
    int secs = totalSec % 60;

    std::ostringstream os;
    if (hours > 0) {
        os << hours << ":" << (mins < 10 ? "0" : "") << mins << ":" << (secs < 10 ? "0" : "") << secs;
    } else {
        os << mins << ":" << (secs < 10 ? "0" : "") << secs;
    }
    return os.str();
}

// ====== EXIF Rotation ======

void applyExifRotation(MediaInfo& info) {
    int deg = exifRotationDegrees(info.exifOrientation);
    if (deg == 90 || deg == 270) {
        std::swap(info.width, info.height);
    }
}

DisplayDimensions getDisplayDimensions(const MediaInfo& info) {
    DisplayDimensions d;
    d.width = info.width;
    d.height = info.height;

    int deg = exifRotationDegrees(info.exifOrientation);
    if (deg == 90 || deg == 270) {
        std::swap(d.width, d.height);
    }

    return d;
}

// ====== Zoom / Pan ======

double calculateFitScale(int mediaWidth, int mediaHeight, int viewportWidth, int viewportHeight) {
    if (mediaWidth <= 0 || mediaHeight <= 0) return 1.0;

    double scaleX = static_cast<double>(viewportWidth) / mediaWidth;
    double scaleY = static_cast<double>(viewportHeight) / mediaHeight;
    return std::min(scaleX, scaleY);
}

ViewportState calculateViewport(const MediaInfo& info, int viewportWidth, int viewportHeight) {
    ViewportState state;
    auto dims = getDisplayDimensions(info);
    state.mediaWidth = dims.width;
    state.mediaHeight = dims.height;
    state.viewportWidth = viewportWidth;
    state.viewportHeight = viewportHeight;

    if (dims.width == 0 || dims.height == 0) {
        state.scale = 1.0;
        return state;
    }

    double fitScale = calculateFitScale(dims.width, dims.height, viewportWidth, viewportHeight);
    state.minScale = fitScale * 0.5;
    state.maxScale = std::max(5.0, fitScale * 5.0);
    state.scale = fitScale;

    // Center the media
    state.offsetX = (viewportWidth - dims.width * fitScale) / 2.0;
    state.offsetY = (viewportHeight - dims.height * fitScale) / 2.0;

    return state;
}

double clampZoom(double scale, const ViewportState& state) {
    return std::max(state.minScale, std::min(scale, state.maxScale));
}

void clampPan(ViewportState& state) {
    double scaledW = state.mediaWidth * state.scale;
    double scaledH = state.mediaHeight * state.scale;

    double maxOffsetX = std::max(0.0, (scaledW - state.viewportWidth) / 2.0);
    double maxOffsetY = std::max(0.0, (scaledH - state.viewportHeight) / 2.0);

    state.offsetX = std::max(-maxOffsetX, std::min(state.offsetX, maxOffsetX));
    state.offsetY = std::max(-maxOffsetY, std::min(state.offsetY, maxOffsetY));
}

// ====== Thumbnail / Cache ======

std::string buildThumbnailCacheKey(const std::string& mxcUrl, const ThumbnailConfig& config) {
    std::ostringstream key;
    key << mxcUrl << "_" << config.width << "x" << config.height << "_" << config.method;
    if (config.animated) key << "_anim";
    return key.str();
}

bool canGenerateThumbnail(const std::string& mimeType) {
    return isMimeTypeImage(mimeType) || isMimeTypeVideo(mimeType);
}

ThumbnailConfig getRecommendedThumbnailConfig(int screenDensity) {
    ThumbnailConfig cfg;
    if (screenDensity >= 480) { cfg.width = 640; cfg.height = 480; }
    else if (screenDensity >= 320) { cfg.width = 320; cfg.height = 240; }
    else { cfg.width = 160; cfg.height = 120; }
    cfg.method = "scale";
    return cfg;
}

// ================================================================
// Media Viewer List Builder
// ================================================================

// Original Kotlin: MediaViewerViewModel.kt — build viewer list from timeline events
MediaViewerList buildMediaViewerList(
    const std::vector<std::string>& eventContentsJson,
    const std::string& roomTitle,
    int startIndex) {

    MediaViewerList list;
    list.roomTitle = roomTitle;
    list.startIndex = startIndex;

    for (const auto& contentJson : eventContentsJson) {
        MediaInfo info = parseMediaInfo(contentJson);

        // Original Kotlin: filter out non-media events
        if (info.mxcUrl.empty()) continue;
        if (info.type != MediaType::IMAGE && info.type != MediaType::VIDEO &&
            info.type != MediaType::GIF && info.type != MediaType::STICKER) continue;

        MediaViewerItem item;
        item.url = info.mxcUrl;
        item.mimeType = info.mimeType;
        item.title = info.fileName.empty() ? info.body : info.fileName;
        item.description = info.body;
        item.width = info.width;
        item.height = info.height;
        item.fileSize = info.sizeBytes;
        item.isDownloaded = !info.downloadUrl.empty();
        item.thumbnailUrl = info.thumbnailUrl;
        // Original Kotlin: authorInfo set from sender display name (caller provides via body or content)
        item.authorInfo = extractStr(contentJson, "sender_display_name");

        list.items.push_back(std::move(item));
    }

    return list;
}

// Original Kotlin: ImageViewerViewModel.kt — find adjacent media
AdjacentMedia findAdjacentMedia(const MediaViewerList& list, int currentIndex) {
    AdjacentMedia adj;

    // Previous: search backwards for first media item
    for (int i = currentIndex - 1; i >= 0; i--) {
        if (!list.items[i].url.empty()) {
            adj.prevIndex = i;
            break;
        }
    }

    // Next: search forwards
    int sz = static_cast<int>(list.items.size());
    for (int i = currentIndex + 1; i < sz; i++) {
        if (!list.items[i].url.empty()) {
            adj.nextIndex = i;
            break;
        }
    }

    return adj;
}

// ================================================================
// Media Download Utils
// ================================================================

// Original Kotlin: MediaDownloader.kt
bool isMediaDownloadable(const std::string& mxcUrl) {
    // mxc:// URLs are downloadable from the homeserver
    if (mxcUrl.empty()) return false;
    if (mxcUrl.compare(0, 6, "mxc://") == 0) return true;
    // HTTP URLs are also downloadable
    if (mxcUrl.compare(0, 7, "http://") == 0 || mxcUrl.compare(0, 8, "https://") == 0) return true;
    return false;
}

// Original Kotlin: FileService.kt, MediaCache.kt
std::string getMediaCachePath(const std::string& mxcUrl, const std::string& cacheDir,
                              const std::string& fileName) {
    std::ostringstream path;

    // Remove trailing slash from cacheDir
    std::string dir = cacheDir;
    while (!dir.empty() && dir.back() == '/') dir.pop_back();
    path << dir << "/mxc_media/";

    auto serverName = extractMxcServerName(mxcUrl);
    auto mediaId = extractMxcMediaId(mxcUrl);

    // Original Kotlin: cache path = cacheDir/mxc_media/{serverName}_{mediaId}_{filename}
    if (!fileName.empty()) {
        // Extract base name from path (handle / separators)
        auto lastSlash = fileName.rfind('/');
        auto baseName = (lastSlash != std::string::npos) ? fileName.substr(lastSlash + 1) : fileName;
        path << serverName << "_" << mediaId << "_" << baseName;
    } else {
        path << serverName << "_" << mediaId;
    }

    return path.str();
}

} // namespace progressive
