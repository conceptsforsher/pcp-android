#include "progressive/media_utils.hpp"
#include "progressive/json_parser.hpp"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <cstring>
#include <unordered_map>

namespace progressive {

// ================================================================
// Internal MIME type lookup table
// ================================================================

// Original Kotlin: MimeTypeMap.kt
static const std::unordered_map<std::string, std::string>& getMimeTypeMap() {
    static const std::unordered_map<std::string, std::string> map = {
        // Images
        { "jpg", "image/jpeg" },  { "jpeg", "image/jpeg" },
        { "png", "image/png" },   { "gif", "image/gif" },
        { "webp", "image/webp" }, { "bmp", "image/bmp" },
        { "svg", "image/svg+xml" }, { "ico", "image/x-icon" },
        { "heic", "image/heic" }, { "heif", "image/heif" },
        { "avif", "image/avif" },
        // Videos
        { "mp4", "video/mp4" },   { "m4v", "video/mp4" },
        { "webm", "video/webm" }, { "mkv", "video/x-matroska" },
        { "avi", "video/x-msvideo" }, { "mov", "video/quicktime" },
        { "ogv", "video/ogg" },   { "3gp", "video/3gpp" },
        { "flv", "video/x-flv" }, { "wmv", "video/x-ms-wmv" },
        // Audio
        { "mp3", "audio/mpeg" },  { "m4a", "audio/mp4" },
        { "ogg", "audio/ogg" },   { "oga", "audio/ogg" },
        { "wav", "audio/wav" },   { "flac", "audio/flac" },
        { "opus", "audio/opus" }, { "aac", "audio/aac" },
        { "wma", "audio/x-ms-wma" }, { "mid", "audio/midi" },
        // Documents
        { "pdf", "application/pdf" },
        { "doc", "application/msword" },
        { "docx", "application/vnd.openxmlformats-officedocument.wordprocessingml.document" },
        { "xls", "application/vnd.ms-excel" },
        { "xlsx", "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet" },
        { "ppt", "application/vnd.ms-powerpoint" },
        { "pptx", "application/vnd.openxmlformats-officedocument.presentationml.presentation" },
        { "txt", "text/plain" },  { "html", "text/html" },
        { "csv", "text/csv" },    { "rtf", "application/rtf" },
        { "zip", "application/zip" }, { "tar", "application/x-tar" },
        { "gz", "application/gzip" }, { "7z", "application/x-7z-compressed" },
        { "apk", "application/vnd.android.package-archive" },
        // Contact / Calendar
        { "vcf", "text/vcard" },  { "vcard", "text/vcard" },
        { "ics", "text/calendar" },
    };
    return map;
}

// ================================================================
// JSON helpers (manual)
// ================================================================

static std::string extractStr(const std::string& json, const std::string& key) {
    auto pp = json.find('"' + key + '"');
    if (pp == std::string::npos) return "";
    pp = json.find(':', pp);
    if (pp == std::string::npos) return "";
    pp++;
    while (pp < json.size() && (json[pp] == ' ' || json[pp] == '\t' || json[pp] == '\n')) pp++;
    if (pp >= json.size() || json[pp] != '"') return "";
    pp++;
    size_t e = pp;
    while (e < json.size() && json[e] != '"') {
        if (json[e] == '\\' && e + 1 < json.size()) e++;
        e++;
    }
    return json.substr(pp, e - pp);
}

static int64_t extractInt(const std::string& json, const std::string& key) {
    auto pp = json.find('"' + key + '"');
    if (pp == std::string::npos) return 0;
    pp = json.find(':', pp);
    if (pp == std::string::npos) return 0;
    pp++;
    while (pp < json.size() && (json[pp] == ' ' || json[pp] == '\t' || json[pp] == '\n')) pp++;
    int64_t v = 0;
    while (pp < json.size() && json[pp] >= '0' && json[pp] <= '9') { v = v * 10 + (json[pp] - '0'); pp++; }
    return v;
}

static double extractDouble(const std::string& json, const std::string& key) {
    auto pp = json.find('"' + key + '"');
    if (pp == std::string::npos) return 0.0;
    pp = json.find(':', pp);
    if (pp == std::string::npos) return 0.0;
    pp++;
    while (pp < json.size() && (json[pp] == ' ' || json[pp] == '\t' || json[pp] == '\n')) pp++;
    std::string num;
    while (pp < json.size() && (std::isdigit(json[pp]) || json[pp] == '.' || json[pp] == '-' || json[pp] == 'e' || json[pp] == 'E')) {
        num += json[pp]; pp++;
    }
    if (num.empty()) return 0.0;
    return std::stod(num);
}

// ================================================================
// Media Type Detection
// ================================================================

// Original Kotlin: MimeTypeMap.kt, MediaContent.kt
MediaType detectMediaType(const std::string& mimeType) {
    if (mimeType.empty()) return MediaType::OTHER;
    auto lower = mimeType;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

    if (lower.rfind("image/", 0) == 0) {
        if (lower == "image/gif") return MediaType::GIF;
        return MediaType::IMAGE;
    }
    if (lower.rfind("video/", 0) == 0) return MediaType::VIDEO;
    if (lower.rfind("audio/", 0) == 0) return MediaType::AUDIO;

    return MediaType::FILE;
}

MediaType detectMediaType(const std::string& mimeType, const std::string& fileExtension) {
    auto type = detectMediaType(mimeType);

    // Original Kotlin: If MIME is generic (application/octet-stream), use extension
    if (type == MediaType::FILE && mimeType == "application/octet-stream") {
        std::string ext = fileExtension;
        if (ext.empty()) return type;
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        // Remove leading dot if present
        if (!ext.empty() && ext[0] == '.') ext = ext.substr(1);

        auto mime = getMimeType(ext);
        if (!mime.empty()) return detectMediaType(mime);
    }

    return type;
}

// ================================================================
// MIME Type Lookup
// ================================================================

// Original Kotlin: MimeTypeMap.kt
std::string getMimeType(const std::string& extension) {
    if (extension.empty()) return "";

    auto key = extension;
    std::transform(key.begin(), key.end(), key.begin(), ::tolower);
    // Remove leading dot
    if (!key.empty() && key[0] == '.') key = key.substr(1);

    // Special: sticker
    if (key == "sticker" || key == "stick") return "image/webp";

    const auto& map = getMimeTypeMap();
    auto it = map.find(key);
    if (it != map.end()) return it->second;

    return "application/octet-stream";
}

// Original Kotlin: MimeTypeMap.kt — expanded MIME detection
bool isImageMimeType(const std::string& mimeType) {
    if (mimeType.empty()) return false;
    auto lower = mimeType;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    if (lower.rfind("image/", 0) == 0 && lower != "image/svg+xml") return true;
    return false;
}

bool isVideoMimeType(const std::string& mimeType) {
    if (mimeType.empty()) return false;
    auto lower = mimeType;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    if (lower.rfind("video/", 0) == 0) return true;
    // Some containers may be labeled differently
    if (lower == "application/x-mpegURL" || lower == "application/vnd.apple.mpegurl") return true;
    return false;
}

bool isAudioMimeType(const std::string& mimeType) {
    if (mimeType.empty()) return false;
    auto lower = mimeType;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    if (lower.rfind("audio/", 0) == 0) return true;
    // Optional containers
    if (lower == "application/ogg") return true;
    return false;
}

// Original Kotlin: FileUtils.kt
std::string getFileExtension(const std::string& fileName) {
    auto dot = fileName.rfind('.');
    if (dot == std::string::npos) return "";
    return fileName.substr(dot);  // includes dot, e.g. ".jpg"
}

// ================================================================
// Media Metadata Parsing
// ================================================================

// Original Kotlin: MediaInfo.kt — parse info JSON block
MediaMetadata parseMediaMetadata(const std::string& infoJson) {
    MediaMetadata meta;
    if (infoJson.empty()) return meta;

    meta.width = static_cast<int>(extractInt(infoJson, "w"));
    if (meta.width == 0) meta.width = static_cast<int>(extractInt(infoJson, "width"));

    meta.height = static_cast<int>(extractInt(infoJson, "h"));
    if (meta.height == 0) meta.height = static_cast<int>(extractInt(infoJson, "height"));

    meta.durationMs = extractInt(infoJson, "duration");

    meta.frameRate = extractDouble(infoJson, "frame_rate");
    if (meta.frameRate == 0.0) meta.frameRate = extractDouble(infoJson, "fps");

    meta.bitRate = extractInt(infoJson, "bitrate");
    if (meta.bitRate == 0) meta.bitRate = extractInt(infoJson, "bit_rate");

    meta.codec = extractStr(infoJson, "codec");
    if (meta.codec.empty()) meta.codec = extractStr(infoJson, "codec_type");

    meta.orientation = static_cast<int>(extractInt(infoJson, "orientation"));
    if (meta.orientation == 0) meta.orientation = 1;

    return meta;
}

// ================================================================
// Media Size Validation
// ================================================================

// Original Kotlin: MediaSizeUtils.kt
bool validateMediaSize(int width, int height, int64_t fileSize, const MediaSizeConstraints& constraints) {
    if (width > constraints.maxWidth || height > constraints.maxHeight) return false;
    if (constraints.maxFileSize > 0 && fileSize > constraints.maxFileSize) return false;
    return true;
}

// Original Kotlin: UploadEncryptionConfig.kt
std::string getMediaOptimization(const std::string& mimeType, int quality) {
    if (mimeType.empty()) return "optimize=default,q=" + std::to_string(quality);

    auto lower = mimeType;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

    // Original Kotlin: Choose compression format for upload
    if (lower.rfind("image/", 0) == 0) {
        if (lower == "image/png") return "optimize=lossless";
        if (lower == "image/webp") return "optimize=webp,q=" + std::to_string(quality);
        if (lower == "image/gif") return "optimize=keep_animated";
        if (lower == "image/heic" || lower == "image/heif") return "optimize=convert_to_jpeg,q=" + std::to_string(quality);
        return "optimize=jpeg,q=" + std::to_string(quality);
    }
    if (lower.rfind("video/", 0) == 0) {
        return "optimize=h264,q=" + std::to_string(quality);
    }
    if (lower.rfind("audio/", 0) == 0) {
        return "optimize=aac,q=" + std::to_string(quality);
    }
    return "optimize=passthrough";
}

// ================================================================
// Existing: Media Upload
// ================================================================

std::string buildMediaUploadBody(const MediaUploadConfig& config) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << "{";
    if (!config.fileName.empty())
        json << R"("filename": ")" << esc(config.fileName) << R"(",)";
    json << R"("content_type": ")" << esc(config.mimeType) << R"(")";
    json << "}";
    return json.str();
}

std::string parseUploadResponse(const std::string& responseJson) {
    return parseJsonStringValue(responseJson, "content_uri");
}

MediaDownloadInfo parseMediaDownloadInfo(const std::string& mxcUri, const std::string& responseJson) {
    MediaDownloadInfo info;
    info.mxcUri = mxcUri;
    info.mimeType  = parseJsonStringValue(responseJson, "content_type");
    info.fileName  = parseJsonStringValue(responseJson, "filename");

    auto size = parseJsonStringValue(responseJson, "size");
    if (!size.empty()) info.fileSize = std::stoll(size);

    info.isComplete = true;
    return info;
}

std::string buildThumbnailDimensions(const MediaUploadConfig& config) {
    std::ostringstream out;
    out << config.maxThumbnailW << "x" << config.maxThumbnailH;
    return out.str();
}

double computeUploadProgress(int64_t uploaded, int64_t total) {
    if (total <= 0) return 0.0;
    return static_cast<double>(uploaded) / total * 100.0;
}

std::string formatUploadProgress(int64_t uploaded, int64_t total) {
    std::ostringstream out;
    if (uploaded < 1024 && total < 1024) {
        out << uploaded << " / " << total << " B";
    } else {
        out << std::fixed << std::setprecision(1)
            << (uploaded / 1024.0) << " / " << (total / 1024.0) << " KB";
    }
    out << " (" << static_cast<int>(computeUploadProgress(uploaded, total)) << "%)";
    return out.str();
}

bool shouldGenerateThumbnail(const std::string& mimeType) {
    return mimeType.rfind("image/", 0) == 0 || mimeType.rfind("video/", 0) == 0;
}

std::string getMatrixContentType(const std::string& mimeType) {
    if (mimeType == "image/jpeg") return "image/jpeg";
    if (mimeType == "image/png") return "image/png";
    if (mimeType == "image/gif") return "image/gif";
    if (mimeType == "image/webp") return "image/webp";
    if (mimeType == "video/mp4") return "video/mp4";
    if (mimeType == "video/webm") return "video/webm";
    if (mimeType == "audio/ogg") return "audio/ogg";
    if (mimeType == "audio/mp3" || mimeType == "audio/mpeg") return "audio/mpeg";
    if (mimeType == "application/pdf") return "application/pdf";
    return "application/octet-stream";
}

std::string mimeToMsgType(const std::string& mimeType) {
    if (mimeType.rfind("image/", 0) == 0) return "m.image";
    if (mimeType.rfind("video/", 0) == 0) return "m.video";
    if (mimeType.rfind("audio/", 0) == 0) return "m.audio";
    return "m.file";
}

// ================================================================
// Existing: Blurhash
// ================================================================

bool isValidBlurhash(const std::string& hash) {
    if (hash.empty() || hash.size() > 100) return false;
    for (char c : hash) {
        if (!((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') ||
              (c >= 'a' && c <= 'z') || c == '$' || c == '%' || c == '*' ||
              c == '+' || c == '-' || c == '.' || c == '/' || c == ':' ||
              c == ';' || c == '<' || c == '=' || c == '>' || c == '?' ||
              c == '@' || c == '[' || c == ']' || c == '^' || c == '_' ||
              c == '{' || c == '|' || c == '}' || c == '~')) {
            return false;
        }
    }
    return hash.size() >= 6;
}

BlurhashResult parseBlurhash(const std::string& contentJson) {
    BlurhashResult result;
    auto info = parseJsonStringValue(contentJson, "info");
    if (info.empty()) return result;

    std::string wrapped = "{" + info + "}";
    result.hash = parseJsonStringValue(wrapped, "blurhash");

    auto xStr = parseJsonStringValue(wrapped, "blurhash_x");
    auto yStr = parseJsonStringValue(wrapped, "blurhash_y");
    if (!xStr.empty()) result.componentsX = std::stoi(xStr);
    if (!yStr.empty()) result.componentsY = std::stoi(yStr);

    result.valid = isValidBlurhash(result.hash);
    return result;
}

} // namespace progressive
