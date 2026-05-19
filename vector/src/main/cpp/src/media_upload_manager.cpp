#include "progressive/media_upload_manager.hpp"
#include <sstream>
#include <algorithm>
#include <ctime>

namespace progressive {

// ====== Enum conversions ======

const char* attachmentTypeToString(MediaAttachmentType type) {
    switch (type) {
        case MediaAttachmentType::FILE: return "file";
        case MediaAttachmentType::IMAGE: return "image";
        case MediaAttachmentType::AUDIO: return "audio";
        case MediaAttachmentType::VIDEO: return "video";
        case MediaAttachmentType::VOICE_MESSAGE: return "voice_message";
        default: return "file";
    }
}

MediaAttachmentType attachmentTypeFromString(const std::string& s) {
    if (s == "image") return MediaAttachmentType::IMAGE;
    if (s == "video") return MediaAttachmentType::VIDEO;
    if (s == "audio") return MediaAttachmentType::AUDIO;
    if (s == "voice_message") return MediaAttachmentType::VOICE_MESSAGE;
    return MediaAttachmentType::FILE;
}

// ====== MediaContentAttachmentData ======
// Original: normalizeMimeType — "image/jpg" → "image/jpeg"

std::string MediaContentAttachmentData::getSafeMimeType() const {
    return normalizeMimeType(mimeType);
}

std::string MediaContentAttachmentData::normalizeMimeType(const std::string& mime) {
    if (mime == "image/jpg") return "image/jpeg";
    return mime;
}

MediaAttachmentType MediaContentAttachmentData::detectType(const std::string& mimeType) {
    if (mimeType.compare(0, 6, "image/") == 0) return MediaAttachmentType::IMAGE;
    if (mimeType.compare(0, 6, "video/") == 0) return MediaAttachmentType::VIDEO;
    if (mimeType.compare(0, 6, "audio/") == 0) return MediaAttachmentType::AUDIO;
    if (mimeType.find("voice") != std::string::npos ||
        mimeType.find("ogg") != std::string::npos) return MediaAttachmentType::AUDIO;
    return MediaAttachmentType::FILE;
}

// ====== Constructor ======

MediaUploadManager::MediaUploadManager() {}

void MediaUploadManager::setUploadUrl(const std::string& url) { config_.uploadUrl = url; }
void MediaUploadManager::setMaxFileSize(int64_t bytes) { config_.maxFileSize = bytes; }
void MediaUploadManager::setCompressImages(bool compress) { config_.compressImages = compress; }
void MediaUploadManager::setJpegQuality(int quality) { config_.jpegQuality = quality; }

// ====== File Validation ======

bool MediaUploadManager::isFileSizeValid(int64_t fileSize) const {
    return fileSize <= config_.maxFileSize;
}

std::string MediaUploadManager::formatSizeLimitWarning(int64_t fileSize, int64_t maxSize) const {
    std::ostringstream os;
    os << "File too large: ";

    auto fmtSize = [](int64_t bytes) -> std::string {
        if (bytes < 1024) return std::to_string(bytes) + " B";
        double kb = bytes / 1024.0;
        if (kb < 1024) { std::ostringstream s; s.precision(1); s << std::fixed << kb; return s.str() + " KB"; }
        double mb = kb / 1024.0;
        std::ostringstream s; s.precision(1); s << std::fixed << mb; return s.str() + " MB";
    };

    os << fmtSize(fileSize) << " exceeds the maximum of " << fmtSize(maxSize);
    return os.str();
}

// ====== Upload Response ======
// Original: ContentUploadResponse.kt — content_uri

ContentUploadResponse MediaUploadManager::parseUploadResponse(const std::string& json) {
    ContentUploadResponse resp;

    // Extract content_uri
    auto pp = json.find("\"content_uri\"");
    if (pp == std::string::npos) {
        resp.success = false;
        resp.errorMessage = "Missing content_uri in response";
        return resp;
    }
    pp = json.find('"', pp + 13);
    if (pp == std::string::npos) {
        resp.success = false;
        resp.errorMessage = "Malformed content_uri";
        return resp;
    }
    pp++;
    size_t e = pp;
    while (e < json.size() && json[e] != '"') e++;
    resp.contentUri = json.substr(pp, e - pp);
    resp.success = !resp.contentUri.empty();
    return resp;
}

std::string MediaUploadManager::buildUploadRequest(const std::string& filename, const std::string& mimeType,
                                                    int64_t fileSize) const {
    (void)fileSize;
    std::ostringstream os;
    os << R"({"filename":")" << filename << R"(")";
    if (!mimeType.empty()) os << R"(,"content_type":")" << mimeType << R"(")";
    os << "}";
    return os.str();
}

// ====== Progress Tracking ======

void MediaUploadManager::updateProgress(int64_t uploadedBytes) {
    progress_.uploadedBytes = uploadedBytes;
    if (progress_.totalBytes > 0) {
        progress_.percent = (static_cast<float>(uploadedBytes) / progress_.totalBytes) * 100.0f;
    }
    progress_.lastUpdateMs = static_cast<int64_t>(std::time(nullptr)) * 1000;
}

MediaUploadProgress MediaUploadManager::getProgress() const {
    return progress_;
}

void MediaUploadManager::resetProgress(int64_t totalBytes) {
    progress_ = MediaUploadProgress{};
    progress_.totalBytes = totalBytes;
    progress_.startedAtMs = static_cast<int64_t>(std::time(nullptr)) * 1000;
}

// ====== Content Building ======
// Original: SendService.sendMedia — builds event content for different media types

std::string MediaUploadManager::buildImageContent(const MediaContentAttachmentData& attachment,
                                                    const std::string& mxcUrl,
                                                    const std::string& body) const {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    std::string b = body.empty() ? attachment.name : body;
    int w = static_cast<int>(attachment.width);
    int h = static_cast<int>(attachment.height);

    if (config_.compressImages && w > 0 && h > 0) {
        getCompressedDimensions(w, h, config_.maxImageDimension, w, h);
    }

    std::ostringstream os;
    os << R"({"msgtype":"m.image")";
    os << R"(,"body":")" << esc(b) << R"(")";
    os << R"(,"url":")" << esc(mxcUrl) << R"(")";
    os << R"(,"info":{)";
    os << R"("w":)" << w << R"(,"h":)" << h;
    os << R"(,"size":)" << attachment.size;
    os << R"(,"mimetype":")" << esc(attachment.getSafeMimeType()) << R"(")";
    if (!attachment.name.empty()) os << R"(,"filename":")" << esc(attachment.name) << R"(")";
    os << "}";
    if (attachment.exifOrientation > 0) os << R"(,"org.matrix.msc2705.orientation":)" << attachment.exifOrientation;
    os << R"(,"filename":")" << esc(attachment.name) << R"(")";
    os << "}";
    return os.str();
}

std::string MediaUploadManager::buildVideoContent(const MediaContentAttachmentData& attachment,
                                                    const std::string& mxcUrl,
                                                    const std::string& body) const {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    std::string b = body.empty() ? attachment.name : body;

    std::ostringstream os;
    os << R"({"msgtype":"m.video")";
    os << R"(,"body":")" << esc(b) << R"(")";
    os << R"(,"url":")" << esc(mxcUrl) << R"(")";
    os << R"(,"info":{)";
    os << R"("w":)" << static_cast<int>(attachment.width)
       << R"(,"h":)" << static_cast<int>(attachment.height);
    os << R"(,"size":)" << attachment.size;
    os << R"(,"duration":)" << attachment.duration;
    os << R"(,"mimetype":")" << esc(attachment.getSafeMimeType()) << R"(")";
    if (!attachment.name.empty()) os << R"(,"filename":")" << esc(attachment.name) << R"(")";
    os << "}}";
    return os.str();
}

std::string MediaUploadManager::buildAudioContent(const MediaContentAttachmentData& attachment,
                                                    const std::string& mxcUrl,
                                                    const std::string& body) const {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    std::string b = body.empty() ? attachment.name : body;
    bool isVoice = attachment.type == MediaAttachmentType::VOICE_MESSAGE;

    std::ostringstream os;
    os << R"({"msgtype":"m.audio")";
    os << R"(,"body":")" << esc(b) << R"(")";
    os << R"(,"url":")" << esc(mxcUrl) << R"(")";
    os << R"(,"info":{)";
    os << R"("size":)" << attachment.size;
    os << R"(,"duration":)" << attachment.duration;
    os << R"(,"mimetype":")" << esc(attachment.getSafeMimeType()) << R"(")";
    if (isVoice) os << R"(,"org.matrix.msc3245.voice":{})";
    if (!attachment.name.empty()) os << R"(,"filename":")" << esc(attachment.name) << R"(")";
    os << "}}";
    return os.str();
}

std::string MediaUploadManager::buildFileContent(const MediaContentAttachmentData& attachment,
                                                   const std::string& mxcUrl,
                                                   const std::string& body) const {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    std::string b = body.empty() ? attachment.name : body;

    std::ostringstream os;
    os << R"({"msgtype":"m.file")";
    os << R"(,"body":")" << esc(b) << R"(")";
    os << R"(,"url":")" << esc(mxcUrl) << R"(")";
    os << R"(,"info":{)";
    os << R"("size":)" << attachment.size;
    os << R"(,"mimetype":")" << esc(attachment.getSafeMimeType()) << R"(")";
    if (!attachment.name.empty()) os << R"(,"filename":")" << esc(attachment.name) << R"(")";
    os << "}}";
    return os.str();
}

std::string MediaUploadManager::buildMediaContent(const MediaContentAttachmentData& attachment,
                                                    const std::string& mxcUrl,
                                                    const std::string& body) const {
    switch (attachment.type) {
        case MediaAttachmentType::IMAGE: return buildImageContent(attachment, mxcUrl, body);
        case MediaAttachmentType::VIDEO: return buildVideoContent(attachment, mxcUrl, body);
        case MediaAttachmentType::AUDIO:
        case MediaAttachmentType::VOICE_MESSAGE: return buildAudioContent(attachment, mxcUrl, body);
        default: return buildFileContent(attachment, mxcUrl, body);
    }
}

// ====== EXIF / Compression ======
// Original: ExifInterface.ORIENTATION_* constants

int MediaUploadManager::exifToRotationDegrees(int exifOrientation) {
    switch (exifOrientation) {
        case 3: return 180;
        case 6: return 90;
        case 8: return 270;
        default: return 0;
    }
}

bool MediaUploadManager::exifRequiresSwap(int exifOrientation) {
    return exifOrientation == 5 || exifOrientation == 6 ||
           exifOrientation == 7 || exifOrientation == 8;
}

void MediaUploadManager::getCompressedDimensions(int originalWidth, int originalHeight,
                                                   int maxDimension, int& outWidth, int& outHeight) const {
    if (originalWidth <= maxDimension && originalHeight <= maxDimension) {
        outWidth = originalWidth;
        outHeight = originalHeight;
        return;
    }

    double ratio = static_cast<double>(originalWidth) / originalHeight;
    if (originalWidth > originalHeight) {
        outWidth = maxDimension;
        outHeight = static_cast<int>(maxDimension / ratio);
    } else {
        outHeight = maxDimension;
        outWidth = static_cast<int>(maxDimension * ratio);
    }
}

// ====== Serialization ======

std::string MediaUploadManager::attachmentToJson(const MediaContentAttachmentData& attachment) const {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    std::ostringstream os;
    os << R"({"size":)" << attachment.size
       << R"(,"duration":)" << attachment.duration
       << R"(,"height":)" << attachment.height
       << R"(,"width":)" << attachment.width
       << R"(,"exif_orientation":)" << attachment.exifOrientation
       << R"(,"name":")" << esc(attachment.name)
       << R"(","mime_type":")" << esc(attachment.getSafeMimeType())
       << R"(","type":")" << attachmentTypeToString(attachment.type)
       << R"(")";
    os << "}";
    return os.str();
}

std::string MediaUploadManager::progressToJson() const {
    std::ostringstream os;
    os << R"({"total_bytes":)" << progress_.totalBytes
       << R"(,"uploaded_bytes":)" << progress_.uploadedBytes
       << R"(,"percent":)" << progress_.percent
       << R"(,"is_complete":)" << (progress_.isComplete ? "true" : "false")
       << R"(,"content_uri":")" << progress_.contentUri << R"(")";
    if (progress_.hasError) os << R"(,"error":")" << progress_.errorMessage << R"(")";
    os << "}";
    return os.str();
}

} // namespace progressive
