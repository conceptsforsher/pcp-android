#ifndef PROGRESSIVE_MEDIA_FILTER_HPP
#define PROGRESSIVE_MEDIA_FILTER_HPP

#include <string>
#include <vector>

namespace progressive {

struct MediaItem {
    std::string eventId;
    std::string mxcUri;
    std::string fileName;
    std::string mimeType;
    std::string extension;    // computed from fileName or mimeType
    int64_t fileSize = 0;
    int64_t timestamp = 0;    // origin_server_ts
    std::string senderName;
    bool isAvailable = true;  // still downloadable from server
};

// Filter media items by file extension (case-insensitive).
// Pass empty string to include all.
std::vector<const MediaItem*> filterByExtension(
    const std::vector<MediaItem>& items,
    const std::string& extension
);

// Detect expired files: items where isAvailable == false.
std::vector<const MediaItem*> findExpiredFiles(const std::vector<MediaItem>& items);

// Extract file extension from filename or MIME type.
std::string getFileExtension(const std::string& fileName, const std::string& mimeType);

// Check if an MXC URI is likely valid (basic format check).
bool isValidMxcUri(const std::string& uri);

// Get unique extensions from a list of media items.
std::vector<std::string> getAvailableExtensions(const std::vector<MediaItem>& items);

} // namespace progressive

#endif // PROGRESSIVE_MEDIA_FILTER_HPP
