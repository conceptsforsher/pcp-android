#include "progressive/media_filter.hpp"
#include <algorithm>
#include <cctype>
#include <sstream>
#include <unordered_set>

namespace progressive {

static std::string toLower(const std::string& s) {
    std::string r = s;
    std::transform(r.begin(), r.end(), r.begin(), ::tolower);
    return r;
}

bool isValidMxcUri(const std::string& uri) {
    // mxc://server-name/media-id
    return uri.rfind("mxc://", 0) == 0 && uri.size() > 6;
}

std::string getFileExtension(const std::string& fileName, const std::string& mimeType) {
    // Try filename first
    auto dot = fileName.rfind('.');
    if (dot != std::string::npos && dot < fileName.size() - 1) {
        auto ext = toLower(fileName.substr(dot + 1));
        // Remove query params
        auto q = ext.find('?');
        if (q != std::string::npos) ext = ext.substr(0, q);
        return ext;
    }
    // Try MIME type
    auto slash = mimeType.rfind('/');
    if (slash != std::string::npos && slash < mimeType.size() - 1) {
        auto ext = toLower(mimeType.substr(slash + 1));
        if (ext == "mpeg") return "mp3";
        if (ext == "wave" || ext == "x-wav") return "wav";
        if (ext == "quicktime") return "mov";
        if (ext == "x-matroska") return "mkv";
        return ext;
    }
    return "";
}

std::vector<const MediaItem*> filterByExtension(
    const std::vector<MediaItem>& items,
    const std::string& extension
) {
    std::vector<const MediaItem*> result;
    auto target = toLower(extension);
    for (const auto& item : items) {
        // Compute extension if not already set
        auto ext = item.extension.empty()
            ? getFileExtension(item.fileName, item.mimeType)
            : toLower(item.extension);
        if (target.empty() || ext == target) {
            result.push_back(&item);
        }
    }
    return result;
}

std::vector<const MediaItem*> findExpiredFiles(const std::vector<MediaItem>& items) {
    std::vector<const MediaItem*> result;
    for (const auto& item : items) {
        if (!item.isAvailable) {
            result.push_back(&item);
        }
    }
    return result;
}

std::vector<std::string> getAvailableExtensions(const std::vector<MediaItem>& items) {
    std::unordered_set<std::string> seen;
    std::vector<std::string> result;
    for (const auto& item : items) {
        auto ext = item.extension.empty()
            ? getFileExtension(item.fileName, item.mimeType)
            : toLower(item.extension);
        if (!ext.empty() && seen.insert(ext).second) {
            result.push_back(ext);
        }
    }
    std::sort(result.begin(), result.end());
    return result;
}

} // namespace progressive
