#ifndef PROGRESSIVE_MEDIA_UTILS_HPP
#define PROGRESSIVE_MEDIA_UTILS_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

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

// ---- Blurhash Utilities ----

struct BlurhashResult {
    bool valid = false;
    std::string hash;
    int componentsX = 4;
    int componentsY = 3;
};

// Validate a blurhash string.
bool isValidBlurhash(const std::string& hash);

// Parse blurhash info from Matrix event content.
BlurhashResult parseBlurhash(const std::string& contentJson);

} // namespace progressive

#endif // PROGRESSIVE_MEDIA_UTILS_HPP
