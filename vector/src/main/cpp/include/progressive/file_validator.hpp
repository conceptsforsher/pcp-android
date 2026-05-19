#ifndef PROGRESSIVE_FILE_VALIDATOR_HPP
#define PROGRESSIVE_FILE_VALIDATOR_HPP

#include <string>
#include <cstdint>

namespace progressive {

struct FileConstraints {
    int64_t maxSizeBytes = 100 * 1024 * 1024; // 100MB
    std::string allowedExtensions;             // "jpg,png,gif,mp4" (comma-separated)
    bool allowAllExtensions = true;
};

struct FileValidation {
    bool valid = false;
    std::string fileName;
    std::string mimeType;
    std::string extension;
    int64_t fileSize = 0;
    std::string errorMessage;
    bool isImage = false;
    bool isVideo = false;
    bool isAudio = false;
    bool isDocument = false;
};

// Validate a file for upload.
FileValidation validateFile(const std::string& fileName, const std::string& mimeType,
                            int64_t fileSize, const FileConstraints& constraints);

// Get file extension from name.
std::string getFileExtension(const std::string& fileName);

// Get MIME type from file extension.
std::string getMimeFromExtension(const std::string& extension);

// Check if MIME type is an image.
bool isImageMime(const std::string& mimeType);

// Check if MIME type is a video.
bool isVideoMime(const std::string& mimeType);

// Check if MIME type is audio.
bool isAudioMime(const std::string& mimeType);

// Format file size as human-readable.
std::string formatFileSize(int64_t bytes);

// Check if file extension is in allowed list.
bool isExtensionAllowed(const std::string& extension, const std::string& allowedList);

// Get a user-friendly file type category.
std::string getFileTypeCategory(const std::string& mimeType, const std::string& extension);

// Generate a safe filename (strip path, remove special chars).
std::string sanitizeFileName(const std::string& fileName);

} // namespace progressive

#endif // PROGRESSIVE_FILE_VALIDATOR_HPP
