#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include "progressive/link_preview.hpp"

namespace progressive {

// ---- URL Preview Type ----

// Original Kotlin: UrlPreview.kt — preview type classification
enum class UrlPreviewType {
    OG_ARTICLE = 0,
    OG_IMAGE = 1,
    OG_VIDEO = 2,
    TWITTER_CARD = 3,
    O_EMBED = 4,
    LINK = 5           // plain link, no rich preview
};

// ---- URL Preview Data ----

// Original Kotlin: UrlPreviewData.kt — full preview data model
struct UrlPreviewData {
    std::string url;
    std::string title;
    std::string description;
    std::string imageUrl;
    int imageWidth = 0;
    int imageHeight = 0;
    std::string siteName;
    std::string type;           // og:type value
    std::string mimeType;       // e.g. "text/html", "image/jpeg"
    UrlPreviewType previewType = UrlPreviewType::LINK;
};

// ---- URL Match ----

// Original Kotlin: UrlUtils.kt — URL match with position info
struct UrlMatch {
    std::string url;
    int startIndex = 0;
    int endIndex = 0;
};

// ---- Existing UrlPreview struct (preserved) ----

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

// ---- OpenGraph / Twitter Card Parsers ----

// Original Kotlin: org.matrix.android.sdk.api.session.room.model.urlpreview.UrlPreview
// Parse OpenGraph tags from HTML content.
// Original Kotlin (UrlPreviewer.kt):
//   fun parseFromHtml(html: String, baseUrl: String): UrlPreview?
UrlPreview parseUrlPreview(const std::string& html, const std::string& baseUrl);

// Original Kotlin: OpenGraphParser.kt
// Extract only OG tags as a UrlPreviewData struct (full fields).
UrlPreviewData parseOpenGraphTags(const std::string& html, const std::string& baseUrl);

// Original Kotlin: TwitterCardParser.kt
// Extract Twitter Card tags from HTML.
UrlPreviewData parseTwitterCardTags(const std::string& html, const std::string& baseUrl);

// Original Kotlin: OEmbedParser.kt
// Parse oEmbed JSON response: {"title":..., "thumbnail_url":..., ...}
UrlPreviewData parseOEmbedResponse(const std::string& oembedJson, const std::string& originalUrl);

// Original Kotlin: UrlPreview.kt — detect preview type from parsed tags
UrlPreviewType detectUrlPreviewType(const UrlPreviewData& data);

// ---- URL Preview Request / Response ----

// Original Kotlin: PreviewUrlService.kt — build request JSON for GET /preview_url
std::string buildUrlPreviewRequest(const std::string& url, int64_t ts = 0);

// Original Kotlin: PreviewUrlService.kt — parse server /preview_url response
UrlPreviewData parseUrlPreviewResponse(const std::string& responseJson);

// ---- URL Utilities ----

// Original Kotlin: UrlPreviewUtils.kt — check if URL supports preview
bool isUrlPreviewable(const std::string& url);

// Original Kotlin: EventHtmlRenderer.kt — generate HTML for preview display
std::string formatUrlPreviewHtml(const UrlPreviewData& preview);

// Original Kotlin: UrlUtils.kt — find first URL in plain text
UrlMatch extractFirstUrl(const std::string& text);

// Original Kotlin: UrlUtils.kt — find all URLs in plain text
std::vector<UrlMatch> extractAllUrls(const std::string& text);

// ---- Existing preserved functions ----

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
