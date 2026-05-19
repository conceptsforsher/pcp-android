#ifndef PROGRESSIVE_URL_PREVIEW_HPP
#define PROGRESSIVE_URL_PREVIEW_HPP

#include <string>
#include <vector>
#include "progressive/link_preview.hpp"

namespace progressive {

// ---- URL Preview / OpenGraph Parser ----
// Ported from: im.vector.app.features.html.EventHtmlRenderer.kt
//              im.vector.app.features.home.room.detail.timeline.helper.UrlPreviewer.kt
//              org.matrix.android.sdk.api.session.room.model.urlpreview.UrlPreview

struct UrlPreview {
    std::string url;               // original URL
    std::string title;             // og:title or <title>
    std::string description;       // og:description or <meta description>
    std::string imageUrl;          // og:image
    std::string siteName;          // og:site_name
    std::string type;              // og:type (website, article, video, etc.)
    int64_t imageWidth = 0;        // og:image:width
    int64_t imageHeight = 0;       // og:image:height
    bool hasImage = false;
    bool hasTitle = false;
    bool valid = false;            // at least title or description present
};

// Parse OpenGraph tags from HTML content.
// Original Kotlin (UrlPreviewer.kt):
//   fun parseFromHtml(html: String, baseUrl: String): UrlPreview?
UrlPreview parseUrlPreview(const std::string& html, const std::string& baseUrl);

// Extract the title from HTML <title> tag.
std::string extractHtmlTitle(const std::string& html);

// Extract meta description from HTML (non-OG fallback).
std::string extractMetaDescription(const std::string& html);

// Resolve relative URLs to absolute (for og:image which may be /path/to/img.jpg).
std::string resolveUrl(const std::string& baseUrl, const std::string& relative);

// isImageUrl is declared in progressive/link_preview.hpp

// Extract all URLs from HTML (for additional link preview candidates).
std::vector<std::string> extractUrls(const std::string& html);

// Strip HTML tags from a string (for cleaning up description text).
std::string stripHtmlTags(const std::string& html);

// Truncate description to max length, ending at word boundary.
std::string truncateDescription(const std::string& text, size_t maxLen = 200);

// Format URL preview as JSON for the Kotlin UI layer.
std::string urlPreviewToJson(const UrlPreview& preview);

} // namespace progressive

#endif // PROGRESSIVE_URL_PREVIEW_HPP
