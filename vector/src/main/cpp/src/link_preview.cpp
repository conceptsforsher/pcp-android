#include "progressive/link_preview.hpp"
#include <sstream>
#include <regex>

namespace progressive {

std::string extractTitle(const std::string& html) {
    std::regex titleRe(R"(<title[^>]*>([^<]+)</title>)", std::regex::icase);
    std::smatch match;
    if (std::regex_search(html, match, titleRe)) {
        return match[1];
    }
    return {};
}

std::string extractMeta(const std::string& html, const std::string& property) {
    // Try property="og:..." or name="twitter:..."
    std::vector<std::string> patterns = {
        "property=\"" + property + "\"",
        "name=\"" + property + "\"",
        "property='" + property + "'",
        "name='" + property + "'"
    };

    for (const auto& pat : patterns) {
        auto pos = html.find(pat);
        if (pos == std::string::npos) continue;

        // Find content="..." after this position
        auto contentPos = html.find("content=", pos);
        if (contentPos == std::string::npos) continue;

        contentPos += 8; // skip "content="
        char quote = html[contentPos];
        if (quote != '"' && quote != '\'') continue;

        ++contentPos;
        auto end = html.find(quote, contentPos);
        if (end == std::string::npos) continue;

        return html.substr(contentPos, end - contentPos);
    }
    return {};
}

LinkPreview parseLinkPreview(const std::string& url, const std::string& htmlContent) {
    LinkPreview preview;
    preview.url = url;

    if (htmlContent.empty()) return preview;

    preview.title = extractMeta(htmlContent, "og:title");
    if (preview.title.empty()) preview.title = extractMeta(htmlContent, "twitter:title");
    if (preview.title.empty()) preview.title = extractTitle(htmlContent);

    preview.description = extractMeta(htmlContent, "og:description");
    if (preview.description.empty()) preview.description = extractMeta(htmlContent, "twitter:description");

    preview.imageUrl = extractMeta(htmlContent, "og:image");
    if (preview.imageUrl.empty()) preview.imageUrl = extractMeta(htmlContent, "twitter:image");

    preview.siteName = extractMeta(htmlContent, "og:site_name");

    extractImageDimensions(htmlContent, preview.imageWidth, preview.imageHeight);

    // Truncate long text
    preview.title = truncatePreviewText(preview.title, 100);
    preview.description = truncatePreviewText(preview.description, 200);

    preview.valid = !preview.title.empty();
    return preview;
}

bool isPreviewableUrl(const std::string& url) {
    if (url.empty()) return false;
    return !isImageUrl(url) &&
           url.rfind("http://", 0) == 0 ||
           url.rfind("https://", 0) == 0;
}

bool isImageUrl(const std::string& url) {
    static const char* imageExts[] = {
        ".jpg", ".jpeg", ".png", ".gif", ".webp", ".bmp", ".svg", ".ico",
        ".tiff", ".tif", ".heic", ".heif"
    };
    auto lower = url;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    for (const auto& ext : imageExts) {
        if (lower.size() >= strlen(ext) &&
            lower.compare(lower.size() - strlen(ext), strlen(ext), ext) == 0) {
            return true;
        }
    }
    return false;
}

void extractImageDimensions(const std::string& html, int& width, int& height) {
    auto wStr = extractMeta(html, "og:image:width");
    auto hStr = extractMeta(html, "og:image:height");
    if (!wStr.empty()) width = std::stoi(wStr);
    if (!hStr.empty()) height = std::stoi(hStr);
}

std::string linkPreviewToJson(const LinkPreview& preview) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };
    std::ostringstream json;
    json << R"({"valid": )" << (preview.valid ? "true" : "false");
    json << R"(,"url": ")" << esc(preview.url) << R"(")";
    json << R"(,"title": ")" << esc(preview.title) << R"(")";
    json << R"(,"description": ")" << esc(preview.description) << R"(")";
    json << R"(,"imageUrl": ")" << esc(preview.imageUrl) << R"(")";
    json << R"(,"siteName": ")" << esc(preview.siteName) << R"(")";
    json << R"(,"imageWidth": )" << preview.imageWidth;
    json << R"(,"imageHeight": )" << preview.imageHeight << "}";
    return json.str();
}

std::string truncatePreviewText(const std::string& text, int maxLen) {
    if (static_cast<int>(text.size()) <= maxLen) return text;
    auto truncated = text.substr(0, maxLen - 3);
    auto lastSpace = truncated.rfind(' ');
    if (lastSpace != std::string::npos && lastSpace > maxLen / 2) {
        truncated = truncated.substr(0, lastSpace);
    }
    return truncated + "...";
}

} // namespace progressive
