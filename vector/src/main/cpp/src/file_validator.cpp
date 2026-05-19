#include "progressive/file_validator.hpp"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <unordered_map>

namespace progressive {

static const std::unordered_map<std::string, std::string> EXT_TO_MIME = {
    {"jpg", "image/jpeg"}, {"jpeg", "image/jpeg"}, {"png", "image/png"},
    {"gif", "image/gif"}, {"webp", "image/webp"}, {"bmp", "image/bmp"},
    {"svg", "image/svg+xml"}, {"ico", "image/x-icon"}, {"heic", "image/heic"},
    {"mp4", "video/mp4"}, {"webm", "video/webm"}, {"mov", "video/quicktime"},
    {"avi", "video/x-msvideo"}, {"mkv", "video/x-matroska"}, {"3gp", "video/3gpp"},
    {"mp3", "audio/mpeg"}, {"ogg", "audio/ogg"}, {"opus", "audio/opus"},
    {"wav", "audio/wav"}, {"flac", "audio/flac"}, {"aac", "audio/aac"},
    {"m4a", "audio/mp4"}, {"amr", "audio/amr"},
    {"pdf", "application/pdf"}, {"doc", "application/msword"},
    {"docx", "application/vnd.openxmlformats-officedocument.wordprocessingml.document"},
    {"xls", "application/vnd.ms-excel"}, {"ppt", "application/vnd.ms-powerpoint"},
    {"txt", "text/plain"}, {"csv", "text/csv"}, {"json", "application/json"},
    {"xml", "application/xml"}, {"html", "text/html"}, {"zip", "application/zip"},
    {"rar", "application/x-rar-compressed"}, {"7z", "application/x-7z-compressed"},
    {"tar", "application/x-tar"}, {"gz", "application/gzip"},
    {"apk", "application/vnd.android.package-archive"}
};

FileValidation validateFile(const std::string& fileName, const std::string& mimeType,
                            int64_t fileSize, const FileConstraints& constraints) {
    FileValidation result;
    result.fileName = fileName;
    result.fileSize = fileSize;
    result.mimeType = mimeType;

    // Check size
    if (fileSize > constraints.maxSizeBytes) {
        std::ostringstream msg;
        msg << "File exceeds maximum size of " << formatFileSize(constraints.maxSizeBytes);
        result.errorMessage = msg.str();
        return result;
    }

    // Check extension
    result.extension = getFileExtension(fileName);
    if (result.extension.empty()) {
        result.errorMessage = "File has no extension.";
        return result;
    }

    if (!constraints.allowAllExtensions) {
        if (!isExtensionAllowed(result.extension, constraints.allowedExtensions)) {
            result.errorMessage = "File extension ." + result.extension + " is not allowed.";
            return result;
        }
    }

    // Classify
    if (mimeType.empty()) {
        result.mimeType = getMimeFromExtension(result.extension);
    }
    result.isImage = isImageMime(result.mimeType);
    result.isVideo = isVideoMime(result.mimeType);
    result.isAudio = isAudioMime(result.mimeType);
    result.isDocument = !result.isImage && !result.isVideo && !result.isAudio;

    result.valid = true;
    return result;
}

std::string getFileExtension(const std::string& fileName) {
    auto dot = fileName.rfind('.');
    if (dot == std::string::npos || dot == fileName.size() - 1) return {};
    auto ext = fileName.substr(dot + 1);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return ext;
}

std::string getMimeFromExtension(const std::string& extension) {
    auto lower = extension;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    auto it = EXT_TO_MIME.find(lower);
    return it != EXT_TO_MIME.end() ? it->second : "application/octet-stream";
}

bool isImageMime(const std::string& mimeType) {
    return mimeType.rfind("image/", 0) == 0;
}

bool isVideoMime(const std::string& mimeType) {
    return mimeType.rfind("video/", 0) == 0;
}

bool isAudioMime(const std::string& mimeType) {
    return mimeType.rfind("audio/", 0) == 0;
}

std::string formatFileSize(int64_t bytes) {
    if (bytes < 1024) return std::to_string(bytes) + " B";
    double kb = bytes / 1024.0;
    if (kb < 1024) {
        std::ostringstream out;
        out << std::fixed << std::setprecision(1) << kb << " KB";
        return out.str();
    }
    double mb = kb / 1024.0;
    if (mb < 1024) {
        std::ostringstream out;
        out << std::fixed << std::setprecision(1) << mb << " MB";
        return out.str();
    }
    std::ostringstream out;
    out << std::fixed << std::setprecision(2) << (mb / 1024.0) << " GB";
    return out.str();
}

bool isExtensionAllowed(const std::string& extension, const std::string& allowedList) {
    if (extension.empty() || allowedList.empty()) return false;
    auto lower = extension;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

    std::istringstream stream(allowedList);
    std::string token;
    while (std::getline(stream, token, ',')) {
        // Trim spaces
        while (!token.empty() && token.front() == ' ') token.erase(0, 1);
        while (!token.empty() && token.back() == ' ') token.pop_back();
        auto lowerToken = token;
        std::transform(lowerToken.begin(), lowerToken.end(), lowerToken.begin(), ::tolower);
        if (lower == lowerToken) return true;
    }
    return false;
}

std::string getFileTypeCategory(const std::string& mimeType, const std::string& extension) {
    if (isImageMime(mimeType)) return "Image";
    if (isVideoMime(mimeType)) return "Video";
    if (isAudioMime(mimeType)) return "Audio";
    if (extension == "pdf") return "PDF";
    if (extension == "zip" || extension == "rar" || extension == "7z") return "Archive";
    if (extension == "doc" || extension == "docx" || extension == "txt") return "Document";
    return "File";
}

std::string sanitizeFileName(const std::string& fileName) {
    std::string result;
    for (char c : fileName) {
        // Keep only safe characters
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
            (c >= '0' && c <= '9') || c == '.' || c == '-' || c == '_' ||
            c == ' ' || c == '(' || c == ')') {
            result += c;
        }
    }
    // Remove leading/trailing whitespace
    while (!result.empty() && result.front() == ' ') result.erase(0, 1);
    while (!result.empty() && result.back() == ' ') result.pop_back();
    if (result.empty()) result = "file";
    return result;
}

} // namespace progressive
