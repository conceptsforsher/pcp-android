#ifndef PROGRESSIVE_LINK_PREVIEW_HPP
#define PROGRESSIVE_LINK_PREVIEW_HPP

#include <string>

namespace progressive {

struct LinkPreview {
    std::string url;
    std::string title;           // og:title or <title>
    std::string description;     // og:description
    std::string imageUrl;        // og:image
    std::string siteName;        // og:site_name
    std::string faviconUrl;
    int imageWidth = 0;
    int imageHeight = 0;
    bool valid = false;
};

// Parse OpenGraph and Twitter Card meta tags from HTML.
LinkPreview parseLinkPreview(const std::string& url, const std::string& htmlContent);

// Extract <title> from HTML.
std::string extractTitle(const std::string& html);

// Extract meta tag content: <meta property="og:title" content="...">
std::string extractMeta(const std::string& html, const std::string& property);

// Check if a URL is eligible for link preview (not an image, not local).
bool isPreviewableUrl(const std::string& url);

// Check if a URL points to an image (by extension).
bool isImageUrl(const std::string& url);

// Get the expected preview image dimensions from og:image:width/height.
void extractImageDimensions(const std::string& html, int& width, int& height);

// Format link preview as JSON.
std::string linkPreviewToJson(const LinkPreview& preview);

// Truncate preview text (title max 100, description max 200).
std::string truncatePreviewText(const std::string& text, int maxLen);

} // namespace progressive

#endif // PROGRESSIVE_LINK_PREVIEW_HPP
