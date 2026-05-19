#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// ---- Media Type ----

// Original Kotlin: MimeTypeMap.kt, MediaContent.kt
enum class MediaType {
    IMAGE = 0,
    VIDEO = 1,
    AUDIO = 2,
    FILE = 3,
    STICKER = 4,
    GIF = 5,
    OTHER = 6
};

// ---- Media Metadata ----

// Original Kotlin: MediaInfo.kt / ImageInfo.kt / VideoInfo.kt
struct MediaMetadata {
    int width = 0;
    int height = 0;
    int64_t durationMs = 0;
    double frameRate = 0.0;
    int64_t bitRate = 0;
    std::string codec;
    int orientation = 1;         // EXIF orientation 1-8
};

// ---- Media Size Constraints ----

// Original Kotlin: MediaSizeUtils.kt, UploadEncryptionConfig.kt
struct MediaSizeConstraints {
    int maxWidth = 4096;
    int maxHeight = 4096;
    int64_t maxFileSize = 100 * 1024 * 1024;  // 100 MB default
    std::vector<std::string> allowedTypes;    // allowed MIME type prefixes (e.g. "image/")
};

// ---- Media Upload Utilities ----

struct MediaUploadConfig {
    std::string fileName;
    std::string mimeType;
    int64_t fileSize = 0;
    int maxThumbnailW = 800;
    int maxThumbnailH = 600;
    int jpegQuality = 80;
    bool generateThumbnail = true;
    bool stripExif = true;
    bool sendOriginalSize = false;
};

struct MediaUploadRequest {
    std::string mxcUri;            // upload target
    std::string contentUrl;        // final mxc:// URI
    std::string thumbnailUrl;      // mxc:// for thumbnail
    int64_t totalBytes = 0;        // upload size
    int64_t uploadedBytes = 0;     // progress
    double progress = 0.0;         // 0.0-1.0
    std::string mimeType;
    std::string fileName;
};

struct MediaDownloadInfo {
    std::string mxcUri;
    std::string mimeType;
    std::string fileName;
    int64_t fileSize = 0;
    int64_t downloadedBytes = 0;
    double progress = 0.0;
    bool isEncrypted = false;
    bool isComplete = false;
    std::string cachePath;         // local file path
};

// ---- Blurhash Utilities ----

struct BlurhashResult {
    bool valid = false;
    std::string hash;
    int componentsX = 4;
    int componentsY = 3;
};

// ---- Media Type Detection ----

// Original Kotlin: MimeTypeMap.kt — detect from both MIME and extension
MediaType detectMediaType(const std::string& mimeType);

// Overload that uses file extension as fallback when MIME is generic.
MediaType detectMediaType(const std::string& mimeType, const std::string& fileExtension);

// Original Kotlin: MimeTypeMap.kt — get MIME from file extension
std::string getMimeType(const std::string& extension);

// Original Kotlin: MimeTypeMap.kt — expanded MIME detection
bool isImageMimeType(const std::string& mimeType);
bool isVideoMimeType(const std::string& mimeType);
bool isAudioMimeType(const std::string& mimeType);

// Original Kotlin: FileUtils.kt — extract extension from filename
std::string getFileExtension(const std::string& fileName);

// ---- Media Metadata Parsing ----

// Original Kotlin: MediaInfo.kt — parse info JSON block
MediaMetadata parseMediaMetadata(const std::string& infoJson);

// ---- Media Size Validation ----

// Original Kotlin: MediaSizeUtils.kt
bool validateMediaSize(int width, int height, int64_t fileSize, const MediaSizeConstraints& constraints);

// Original Kotlin: UploadEncryptionConfig.kt — choose optimal format/compression
std::string getMediaOptimization(const std::string& mimeType, int quality);

// ---- Existing functions preserved ----

// Build Matrix upload request body (no file content, just metadata).
std::string buildMediaUploadBody(const MediaUploadConfig& config);

// Parse media upload response to extract content URI.
std::string parseUploadResponse(const std::string& responseJson);

// Parse media download info from API response.
MediaDownloadInfo parseMediaDownloadInfo(const std::string& mxcUri, const std::string& responseJson);

// Build thumbnail dimensions string: "800x600".
std::string buildThumbnailDimensions(const MediaUploadConfig& config);

// Compute upload progress percentage.
double computeUploadProgress(int64_t uploaded, int64_t total);

// Format file size for upload progress display.
std::string formatUploadProgress(int64_t uploaded, int64_t total);

// Check if a file should have a thumbnail generated.
bool shouldGenerateThumbnail(const std::string& mimeType);

// Get the Matrix content type for a given MIME type.
std::string getMatrixContentType(const std::string& mimeType);

// Map MIME type to Matrix msgtype.
std::string mimeToMsgType(const std::string& mimeType);

// Validate a blurhash string.
bool isValidBlurhash(const std::string& hash);

// Parse blurhash info from Matrix event content.
BlurhashResult parseBlurhash(const std::string& contentJson);

} // namespace progressive
