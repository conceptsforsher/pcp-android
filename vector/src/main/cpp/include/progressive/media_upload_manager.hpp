#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// ================================================================
// Media Upload Pipeline — full file upload & compression
//
// Faithful port from Element Android original sources:
//   MediaContentAttachmentData.kt — attachment model (size, duration,
//     height, width, exifOrientation, name, mimeType, type,
//     getSafeMimeType via normalizeMimeType)
//   FileUploader.kt — uploadFile, uploadByteArray, max size check,
//     ContentUploadResponse (content_uri)
//   SendService.kt — sendMedia, sendMedias, compressBeforeSending
//   ContentUploadResponse.kt — content_uri (MXC URL)
//   ChunkedUpload (existing) — large file chunking
//
// Covers:
//   1. Media attachment data model
//   2. MIME type normalization
//   3. EXIF orientation handling
//   4. File size validation (server max)
//   5. Upload progress tracking
//   6. Upload response parsing (MXC URI)
//   7. Compression pre-processing
//   8. Multi-file batch upload
// ================================================================

// ---- Attachment Type ----
// Original: MediaContentAttachmentData.Type (FILE, IMAGE, AUDIO, VIDEO, VOICE_MESSAGE)

enum class MediaAttachmentType {
    FILE = 0,
    IMAGE = 1,
    AUDIO = 2,
    VIDEO = 3,
    VOICE_MESSAGE = 4,
};

const char* attachmentTypeToString(MediaAttachmentType type);
MediaAttachmentType attachmentTypeFromString(const std::string& s);

// ---- Content Attachment Data ----
// Original: MediaContentAttachmentData.kt (size, duration, date, height, width,
//   exifOrientation, name, queryUri, mimeType, type, waveform)
// Original: getSafeMimeType() → normalizeMimeType

struct MediaContentAttachmentData {
    using ContentAttachmentData = MediaContentAttachmentData;  // JNI compat
    int64_t size = 0;                // File size in bytes
    int64_t duration = 0;            // Audio/video duration in ms
    int64_t date = 0;                // File modification date (epoch ms)
    int64_t height = 0;              // Image/video height
    int64_t width = 0;               // Image/video width
    int exifOrientation = 0;         // Original: ExifInterface.ORIENTATION_UNDEFINED
    std::string name;                // File name
    std::string mimeType;            // "image/jpeg", "video/mp4", etc.
    MediaAttachmentType type = MediaAttachmentType::FILE;
    std::vector<int> waveform;       // Audio waveform data (optional)
    bool valid = false;

    // Original: getSafeMimeType() → normalizeMimeType
    std::string getSafeMimeType() const;

    // Original: normalizeMimeType — "image/jpg" → "image/jpeg"
    static std::string normalizeMimeType(const std::string& mime);

    // Detect attachment type from MIME type.
    static MediaAttachmentType detectType(const std::string& mimeType);
};

// ---- Upload Config ----

struct UploadConfig {
    int64_t maxFileSize = 104857600;     // 100 MB default
    bool compressImages = true;          // Original: compressBeforeSending
    int maxImageDimension = 4096;        // Max width/height after compression
    int jpegQuality = 80;                // JPEG compression quality (0-100)
    std::string uploadUrl;               // Server upload endpoint
    std::string filename;                // Optional filename override
    bool useChunkedUpload = false;       // For files > chunk threshold
    int64_t chunkSize = 1048576;         // 1 MB chunks
};

// ---- Upload Progress ----
// Original: ProgressRequestBody.Listener

struct MediaUploadProgress {
    int64_t totalBytes = 0;
    int64_t uploadedBytes = 0;
    float percent = 0.0f;            // 0.0 - 100.0
    bool isComplete = false;
    bool hasError = false;
    std::string errorMessage;
    std::string contentUri;          // MXC URI when complete
    int64_t startedAtMs = 0;
    int64_t lastUpdateMs = 0;
};

// ---- Content Upload Response ----
// Original: ContentUploadResponse.kt (content_uri)

struct ContentUploadResponse {
    std::string contentUri;          // mxc://server/media_id
    bool success = false;
    std::string errorMessage;
};

// ---- Media Upload Manager ----

class MediaUploadManager {
public:
    MediaUploadManager();

    // ====== Upload Config ======

    void setUploadUrl(const std::string& url);
    void setMaxFileSize(int64_t bytes);
    void setCompressImages(bool compress);
    void setJpegQuality(int quality);

    // ====== File Validation ======
    // Original: FileUploader — maxUploadFileSize check, M_TOO_LARGE

    // Check if a file size is within the server limit.
    bool isFileSizeValid(int64_t fileSize) const;

    // Get the maximum allowed file size.
    int64_t getMaxFileSize() const { return config_.maxFileSize; }

    // Format file size warning message.
    std::string formatSizeLimitWarning(int64_t fileSize, int64_t maxSize) const;

    // ====== Upload Response Parsing ======
    // Original: ContentUploadResponse.kt — content_uri

    // Parse upload response to get MXC URI.
    ContentUploadResponse parseUploadResponse(const std::string& json);

    // Build the upload request content.
    std::string buildUploadRequest(const std::string& filename, const std::string& mimeType,
                                    int64_t fileSize) const;

    // ====== Progress Tracking ======
    // Original: ProgressRequestBody.Listener

    // Update upload progress.
    void updateProgress(int64_t uploadedBytes);

    // Get current progress.
    MediaUploadProgress getProgress() const;

    // Reset progress for a new upload.
    void resetProgress(int64_t totalBytes);

    // ====== Content Building ======
    // Original: SendService.sendMedia — build message content from attachment

    // Build image message content JSON.
    std::string buildImageContent(const MediaContentAttachmentData& attachment,
                                   const std::string& mxcUrl,
                                   const std::string& body = "") const;

    // Build video message content JSON.
    std::string buildVideoContent(const MediaContentAttachmentData& attachment,
                                   const std::string& mxcUrl,
                                   const std::string& body = "") const;

    // Build audio message content JSON.
    std::string buildAudioContent(const MediaContentAttachmentData& attachment,
                                   const std::string& mxcUrl,
                                   const std::string& body = "") const;

    // Build file message content JSON.
    std::string buildFileContent(const MediaContentAttachmentData& attachment,
                                  const std::string& mxcUrl,
                                  const std::string& body = "") const;

    // Build generic media content JSON.
    std::string buildMediaContent(const MediaContentAttachmentData& attachment,
                                   const std::string& mxcUrl,
                                   const std::string& body = "") const;

    // ====== EXIF / Compression ======
    // Original: ExifInterface orientation constants

    // Get rotation degrees from EXIF orientation.
    static int exifToRotationDegrees(int exifOrientation);

    // Check if EXIF orientation requires dimension swap.
    static bool exifRequiresSwap(int exifOrientation);

    // Get recommended compressed dimensions.
    void getCompressedDimensions(int originalWidth, int originalHeight,
                                  int maxDimension, int& outWidth, int& outHeight) const;

    // ====== Serialization ======

    // Export attachment data as JSON.
    std::string attachmentToJson(const MediaContentAttachmentData& attachment) const;

    // Export upload progress as JSON.
    std::string progressToJson() const;

private:
    UploadConfig config_;
    MediaUploadProgress progress_;
};

} // namespace progressive
