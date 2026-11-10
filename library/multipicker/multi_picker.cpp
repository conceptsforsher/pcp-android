#include "multi_picker.hpp"
#include <algorithm>
#include <filesystem>
#include <regex>

namespace pcp::multipicker {

MultiPicker::MultiPicker(const PickerConfig& config) : m_config(config) {}
MultiPicker::~MultiPicker() = default;

void MultiPicker::open() {
    m_open = true;
    MP_LOGI("Picker opened: type=%d mode=%d", static_cast<int>(m_config.type),
            static_cast<int>(m_config.mode));
    scanFiles();
}

void MultiPicker::cancel() {
    m_open = false;
    if (m_cancelCallback) m_cancelCallback();
}

void MultiPicker::scanFiles() {
    // In real implementation: query Android MediaStore or SAF
    // For now, stub with proper structure
    MP_LOGI("Scanning files with filter: %zu mime types allowed",
            m_config.filter.allowedMimeTypes.size());
}

bool MultiPicker::matchFilter(const PickerItem& item) const {
    if (m_config.filter.allowedMimeTypes.empty()) return true;
    return std::find(m_config.filter.allowedMimeTypes.begin(),
                     m_config.filter.allowedMimeTypes.end(),
                     item.mimeType) != m_config.filter.allowedMimeTypes.end();
}

// ── MimeTypeUtil ──

std::map<std::string, std::string> MimeTypeUtil::s_mimeMap = {
    {".jpg", "image/jpeg"}, {".jpeg", "image/jpeg"}, {".png", "image/png"},
    {".gif", "image/gif"}, {".webp", "image/webp"}, {".bmp", "image/bmp"},
    {".svg", "image/svg+xml"}, {".heic", "image/heic"}, {".heif", "image/heif"},
    {".mp4", "video/mp4"}, {".webm", "video/webm"}, {".mkv", "video/x-matroska"},
    {".mov", "video/quicktime"}, {".avi", "video/x-msvideo"}, {".3gp", "video/3gpp"},
    {".mp3", "audio/mpeg"}, {".ogg", "audio/ogg"}, {".wav", "audio/wav"},
    {".flac", "audio/flac"}, {".aac", "audio/aac"}, {".m4a", "audio/mp4"},
    {".pdf", "application/pdf"}, {".doc", "application/msword"},
    {".docx", "application/vnd.openxmlformats-officedocument.wordprocessingml.document"},
    {".txt", "text/plain"}, {".json", "application/json"},
    {".zip", "application/zip"}, {".apk", "application/vnd.android.package-archive"},
};

std::string MimeTypeUtil::fromExtension(const std::string& extension) {
    auto it = s_mimeMap.find(extension);
    return it != s_mimeMap.end() ? it->second : "application/octet-stream";
}

std::string MimeTypeUtil::fromPath(const std::string& path) {
    size_t dot = path.rfind('.');
    if (dot == std::string::npos) return "application/octet-stream";
    return fromExtension(path.substr(dot));
}

bool MimeTypeUtil::isImage(const std::string& mimeType) {
    return mimeType.find("image/") == 0;
}

bool MimeTypeUtil::isVideo(const std::string& mimeType) {
    return mimeType.find("video/") == 0;
}

bool MimeTypeUtil::isAudio(const std::string& mimeType) {
    return mimeType.find("audio/") == 0;
}

std::string MimeTypeUtil::getCategory(const std::string& mimeType) {
    size_t slash = mimeType.find('/');
    return slash != std::string::npos ? mimeType.substr(0, slash) : "unknown";
}

// ── FileUtils ──

std::string FileUtils::getDisplayName(const std::string& path) {
    size_t slash = path.rfind('/');
    return slash != std::string::npos ? path.substr(slash + 1) : path;
}

int64_t FileUtils::getFileSize(const std::string& path) {
    try { return std::filesystem::file_size(path); }
    catch (...) { return -1; }
}

std::string FileUtils::getExtension(const std::string& path) {
    size_t dot = path.rfind('.');
    return dot != std::string::npos ? path.substr(dot) : "";
}

std::string FileUtils::formatFileSize(int64_t bytes) {
    const char* units[] = {"B", "KB", "MB", "GB", "TB"};
    int i = 0;
    double size = static_cast<double>(bytes);
    while (size >= 1024.0 && i < 4) { size /= 1024.0; i++; }
    char buf[64];
    snprintf(buf, sizeof(buf), "%.1f %s", size, units[i]);
    return buf;
}

bool FileUtils::isReadable(const std::string& path, int64_t maxSize) {
    try {
        if (!std::filesystem::exists(path)) return false;
        if (maxSize > 0 && std::filesystem::file_size(path) > maxSize) return false;
        return true;
    } catch (...) { return false; }
}

} // namespace pcp::multipicker
