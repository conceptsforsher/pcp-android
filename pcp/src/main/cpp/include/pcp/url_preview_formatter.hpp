#pragma once
#include <string>
#include <vector>

namespace pcp {

struct UrlPreviewCard {
    std::string url;
    std::string title;
    std::string description;
    std::string imageUrl;
    std::string siteName;
    bool hasPreview = false;
};

// Extract first URL from text
std::string extractFirstUrl(const std::string& text);

// Parse preview from Matrix URL preview event
UrlPreviewCard parseUrlPreviewCard(const std::string& json);

// Format preview as HTML card
std::string formatUrlPreviewCard(const UrlPreviewCard& card);

// Check if URL should show preview
bool shouldShowUrlPreview(const std::string& url);

} // namespace pcp
