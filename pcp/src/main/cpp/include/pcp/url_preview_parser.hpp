#pragma once
#include <string>
#include <vector>

namespace pcp {

struct UrlPreview {
    std::string url;
    std::string title;
    std::string description;
    std::string imageUrl;
    std::string siteName;
    std::string type;           // "website", "article", "video", "image"
    int imageWidth = 0;
    int imageHeight = 0;
    int64_t contentLength = 0;  // bytes
};

// Build URL preview request for Matrix server
std::string buildPreviewRequest(const std::string& url, int64_t ts);

// Parse URL preview response
UrlPreview parseUrlPreview(const std::string& json);

// Check if URL is eligible for preview
bool isUrlPreviewable(const std::string& url);

// Extract the first URL from text that is previewable
std::string extractPreviewableUrl(const std::string& text);

// Format preview for timeline display
std::string formatUrlPreviewHtml(const UrlPreview& preview);

} // namespace pcp
