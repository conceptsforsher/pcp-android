#include "progressive/url_preview.hpp"
#include "progressive/link_preview.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <cstring>

namespace progressive {

// Helper: extract content of a named attribute from HTML
static std::string getAttr(const std::string& tag, const std::string& attr) {
    auto search = attr + "=\"";
    auto pos = tag.find(search);
    if (pos == std::string::npos) {
        search = attr + "='";
        pos = tag.find(search);
        if (pos == std::string::npos) return "";
        pos += search.size();
        auto end = tag.find('\'', pos);
        return tag.substr(pos, end - pos);
    }
    pos += search.size();
    auto end = tag.find('"', pos);
    return tag.substr(pos, end - pos);
}

// Helper: extract content between <tag> and </tag>
static std::string getTagContent(const std::string& html, const std::string& tagName) {
    auto openTag = "<" + tagName;
    auto pos = html.find(openTag);
    if (pos == std::string::npos) {
        openTag = "<" + tagName + " ";
        pos = html.find(openTag);
    }
    if (pos == std::string::npos) return "";

    auto tagEnd = html.find('>', pos);
    if (tagEnd == std::string::npos) return "";

    auto closeTag = "</" + tagName + ">";
    auto end = html.find(closeTag, tagEnd);
    if (end == std::string::npos) return "";

    return html.substr(tagEnd + 1, end - tagEnd - 1);
}

// Helper: extract a meta tag by property or name
static std::string getMeta(const std::string& html, const std::string& property, const std::string& attribute) {
    auto search = attribute + "=\"" + property + "\"";
    auto pos = html.find(search);
    if (pos == std::string::npos) {
        search = attribute + "='" + property + "'";
        pos = html.find(search);
    }
    if (pos == std::string::npos) return "";

    auto tagStart = html.rfind("<meta", pos);
    if (tagStart == std::string::npos) return "";
    auto tagEnd = html.find('>', pos);
    if (tagEnd == std::string::npos) return "";

    auto tag = html.substr(tagStart, tagEnd - tagStart + 1);
    return getAttr(tag, "content");
}

// Helper: trim whitespace
static std::string trimStr(const std::string& s) {
    auto start = s.begin();
    while (start != s.end() && std::isspace(static_cast<unsigned char>(*start))) ++start;
    auto end = s.end();
    while (end != start && std::isspace(static_cast<unsigned char>(*(end - 1)))) --end;
    return std::string(start, end);
}

// ================================================================
// Existing: UrlPreview parser
// ================================================================

UrlPreview parseUrlPreview(const std::string& html, const std::string& baseUrl) {
    UrlPreview preview;
    preview.url = baseUrl;

    preview.title = getMeta(html, "og:title", "property");
    preview.description = getMeta(html, "og:description", "property");
    preview.imageUrl = getMeta(html, "og:image", "property");
    preview.siteName = getMeta(html, "og:site_name", "property");
    preview.type = getMeta(html, "og:type", "property");

    auto w = getMeta(html, "og:image:width", "property");
    if (!w.empty()) preview.imageWidth = std::stoll(w);
    auto h = getMeta(html, "og:image:height", "property");
    if (!h.empty()) preview.imageHeight = std::stoll(h);

    if (preview.title.empty()) preview.title = getMeta(html, "twitter:title", "name");
    if (preview.description.empty()) preview.description = getMeta(html, "twitter:description", "name");
    if (preview.imageUrl.empty()) preview.imageUrl = getMeta(html, "twitter:image", "name");

    if (preview.title.empty()) preview.title = extractHtmlTitle(html);
    if (preview.description.empty()) preview.description = extractMetaDescription(html);

    preview.title = trimStr(preview.title);
    preview.description = trimStr(preview.description);

    if (!preview.imageUrl.empty()) {
        preview.imageUrl = resolveUrl(baseUrl, preview.imageUrl);
        preview.hasImage = true;
    }

    preview.hasTitle = !preview.title.empty();
    preview.valid = preview.hasTitle || !preview.description.empty();

    return preview;
}

// ================================================================
// OpenGraph tag parser (UrlPreviewData)
// ================================================================

// Original Kotlin: OpenGraphParser.kt
UrlPreviewData parseOpenGraphTags(const std::string& html, const std::string& baseUrl) {
    UrlPreviewData data;
    data.url = baseUrl;

    data.title = getMeta(html, "og:title", "property");
    data.description = getMeta(html, "og:description", "property");
    data.imageUrl = getMeta(html, "og:image", "property");
    data.siteName = getMeta(html, "og:site_name", "property");
    data.type = getMeta(html, "og:type", "property");

    auto w = getMeta(html, "og:image:width", "property");
    if (!w.empty()) data.imageWidth = std::stoi(w);
    auto h = getMeta(html, "og:image:height", "property");
    if (!h.empty()) data.imageHeight = std::stoi(h);

    data.title = trimStr(data.title);
    data.description = trimStr(data.description);

    // Resolve relative image URL
    if (!data.imageUrl.empty()) {
        data.imageUrl = resolveUrl(baseUrl, data.imageUrl);
    }

    // Determine preview type from og:type
    if (!data.type.empty()) {
        auto lowerType = data.type;
        std::transform(lowerType.begin(), lowerType.end(), lowerType.begin(), ::tolower);
        if (lowerType.find("article") != std::string::npos) {
            data.previewType = UrlPreviewType::OG_ARTICLE;
        } else if (lowerType == "video" || lowerType == "video.other" || lowerType == "video.movie") {
            data.previewType = UrlPreviewType::OG_VIDEO;
        } else {
            data.previewType = UrlPreviewType::OG_ARTICLE;
        }
    } else {
        data.previewType = UrlPreviewType::LINK;
    }

    // Set mimeType based on image URL extension
    if (!data.imageUrl.empty()) {
        auto dot = data.imageUrl.rfind('.');
        if (dot != std::string::npos) {
            auto ext = data.imageUrl.substr(dot);
            if (ext == ".jpg" || ext == ".jpeg") data.mimeType = "image/jpeg";
            else if (ext == ".png") data.mimeType = "image/png";
            else if (ext == ".gif") data.mimeType = "image/gif";
            else if (ext == ".webp") data.mimeType = "image/webp";
        }
    }

    return data;
}

// Original Kotlin: TwitterCardParser.kt
UrlPreviewData parseTwitterCardTags(const std::string& html, const std::string& baseUrl) {
    UrlPreviewData data;
    data.url = baseUrl;

    data.title = getMeta(html, "twitter:title", "name");
    data.description = getMeta(html, "twitter:description", "name");
    data.imageUrl = getMeta(html, "twitter:image", "name");
    data.siteName = getMeta(html, "twitter:site", "name");

    auto card = getMeta(html, "twitter:card", "name");
    data.title = trimStr(data.title);
    data.description = trimStr(data.description);

    if (!data.imageUrl.empty()) {
        data.imageUrl = resolveUrl(baseUrl, data.imageUrl);
    }

    // Original Kotlin: twitter:card values
    auto lowerCard = card;
    std::transform(lowerCard.begin(), lowerCard.end(), lowerCard.begin(), ::tolower);
    if (lowerCard == "summary_large_image" || lowerCard == "summary") {
        data.previewType = UrlPreviewType::TWITTER_CARD;
    } else if (lowerCard == "player") {
        data.previewType = UrlPreviewType::OG_VIDEO;
    } else {
        data.previewType = UrlPreviewType::TWITTER_CARD;
    }

    return data;
}

// ================================================================
// OEmbed Parser
// ================================================================

// Original Kotlin: OEmbedParser.kt — manual JSON parser for oEmbed response
UrlPreviewData parseOEmbedResponse(const std::string& oembedJson, const std::string& originalUrl) {
    UrlPreviewData data;
    data.url = originalUrl;
    data.previewType = UrlPreviewType::O_EMBED;

    auto esc = [](const std::string& raw) -> std::string {
        std::string out;
        for (size_t i = 0; i < raw.size(); i++) {
            if (raw[i] == '\\' && i + 1 < raw.size()) {
                char n = raw[i + 1];
                if (n == '"' || n == '\\' || n == '/') { out += n; i++; continue; }
                if (n == 'n') { out += '\n'; i++; continue; }
                if (n == 't') { out += '\t'; i++; continue; }
            }
            out += raw[i];
        }
        return out;
    };

    auto getField = [&](const std::string& json, const std::string& key) -> std::string {
        auto pp = json.find('"' + key + '"');
        if (pp == std::string::npos) return "";
        pp = json.find(':', pp);
        if (pp == std::string::npos) return "";
        pp++;
        while (pp < json.size() && (json[pp] == ' ' || json[pp] == '\t' || json[pp] == '\n')) pp++;
        if (pp >= json.size()) return "";
        if (json[pp] != '"') return "";
        pp++;
        size_t e = pp;
        while (e < json.size() && json[e] != '"') {
            if (json[e] == '\\' && e + 1 < json.size()) e++;
            e++;
        }
        return esc(json.substr(pp, e - pp));
    };

    auto getInt = [&](const std::string& json, const std::string& key) -> int {
        auto pp = json.find('"' + key + '"');
        if (pp == std::string::npos) return 0;
        pp = json.find(':', pp);
        if (pp == std::string::npos) return 0;
        pp++;
        while (pp < json.size() && (json[pp] == ' ' || json[pp] == '\t')) pp++;
        int v = 0;
        while (pp < json.size() && json[pp] >= '0' && json[pp] <= '9') { v = v * 10 + (json[pp] - '0'); pp++; }
        return v;
    };

    data.title = getField(oembedJson, "title");
    data.description = getField(oembedJson, "description");
    data.imageUrl = getField(oembedJson, "thumbnail_url");
    data.siteName = getField(oembedJson, "provider_name");
    data.type = getField(oembedJson, "type");  // "photo", "video", "link", "rich"
    data.imageWidth = getInt(oembedJson, "thumbnail_width");
    data.imageHeight = getInt(oembedJson, "thumbnail_height");

    return data;
}

// Original Kotlin: UrlPreview.kt — detect preview type
UrlPreviewType detectUrlPreviewType(const UrlPreviewData& data) {
    return data.previewType;
}

// ================================================================
// URL Preview Request Builder
// ================================================================

// Original Kotlin: PreviewUrlService.kt — build request for GET /preview_url
std::string buildUrlPreviewRequest(const std::string& url, int64_t ts) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };
    std::ostringstream json;
    json << R"({"url": ")" << esc(url) << R"(")";
    if (ts > 0) json << R"(, "ts": )" << ts;
    json << "}";
    return json.str();
}

// Original Kotlin: PreviewUrlService.kt — manual response parser
UrlPreviewData parseUrlPreviewResponse(const std::string& responseJson) {
    UrlPreviewData data;

    auto esc = [](const std::string& raw) -> std::string {
        std::string out;
        for (size_t i = 0; i < raw.size(); i++) {
            if (raw[i] == '\\' && i + 1 < raw.size()) {
                char n = raw[i + 1];
                if (n == '"' || n == '\\' || n == '/') { out += n; i++; continue; }
            }
            out += raw[i];
        }
        return out;
    };

    auto getStr = [&](const std::string& json, const std::string& key) -> std::string {
        auto pp = json.find('"' + key + '"');
        if (pp == std::string::npos) return "";
        pp = json.find(':', pp);
        if (pp == std::string::npos) return "";
        pp++;
        while (pp < json.size() && (json[pp] == ' ' || json[pp] == '\t' || json[pp] == '\n')) pp++;
        if (pp >= json.size() || json[pp] != '"') return "";
        pp++;
        size_t e = pp;
        while (e < json.size() && json[e] != '"') {
            if (json[e] == '\\' && e + 1 < json.size()) e++;
            e++;
        }
        return esc(json.substr(pp, e - pp));
    };

    auto getInt = [&](const std::string& json, const std::string& key) -> int {
        auto pp = json.find('"' + key + '"');
        if (pp == std::string::npos) return 0;
        pp = json.find(':', pp);
        if (pp == std::string::npos) return 0;
        pp++;
        while (pp < json.size() && (json[pp] == ' ' || json[pp] == '\t')) pp++;
        int v = 0;
        while (pp < json.size() && json[pp] >= '0' && json[pp] <= '9') { v = v * 10 + (json[pp] - '0'); pp++; }
        return v;
    };

    // Matrix /preview_url response format:
    // {"og:title": "...", "og:description": "...", "og:image": "...", ...}
    data.url = getStr(responseJson, "og:url");
    data.title = getStr(responseJson, "og:title");
    data.description = getStr(responseJson, "og:description");
    data.imageUrl = getStr(responseJson, "og:image");
    data.siteName = getStr(responseJson, "og:site_name");
    data.type = getStr(responseJson, "og:type");

    data.imageWidth = getInt(responseJson, "og:image:width");
    data.imageHeight = getInt(responseJson, "og:image:height");

    data.previewType = detectUrlPreviewType(data);

    return data;
}

// ================================================================
// URL Previewable Check
// ================================================================

// Original Kotlin: UrlPreviewUtils.kt
bool isUrlPreviewable(const std::string& url) {
    if (url.empty()) return false;

    // Skip Matrix URIs
    if (url.rfind("mxc://", 0) == 0) return false;
    if (url.rfind("matrix:", 0) == 0) return false;

    // Must be HTTP/HTTPS
    if (url.rfind("http://", 0) != 0 && url.rfind("https://", 0) != 0) return false;

    // Skip common non-previewable patterns
    auto dot = url.rfind('.');
    if (dot != std::string::npos) {
        auto ext = url.substr(dot);
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        // Direct media files — still could be previewed but usually not rich
        if (ext == ".jpg" || ext == ".jpeg" || ext == ".png" || ext == ".gif" ||
            ext == ".webp" || ext == ".mp4" || ext == ".mp3" || ext == ".pdf") {
            return false;
        }
    }

    return true;
}

// Original Kotlin: EventHtmlRenderer.kt — generate HTML for preview display
std::string formatUrlPreviewHtml(const UrlPreviewData& preview) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) {
            switch (c) {
                case '&':  out += "&amp;"; break;
                case '<':  out += "&lt;"; break;
                case '>':  out += "&gt;"; break;
                case '"':  out += "&quot;"; break;
                default:   out += c; break;
            }
        }
        return out;
    };

    std::ostringstream html;
    html << "<div class=\"mx_UrlPreview\">";

    if (!preview.imageUrl.empty()) {
        html << "<div class=\"mx_UrlPreview_image\">";
        html << "<img src=\"" << esc(preview.imageUrl) << "\"";
        if (preview.imageWidth > 0) html << " width=\"" << preview.imageWidth << "\"";
        if (preview.imageHeight > 0) html << " height=\"" << preview.imageHeight << "\"";
        html << " />";
        html << "</div>";
    }

    html << "<div class=\"mx_UrlPreview_body\">";
    if (!preview.siteName.empty()) {
        html << "<div class=\"mx_UrlPreview_site\">" << esc(preview.siteName) << "</div>";
    }
    if (!preview.title.empty()) {
        html << "<div class=\"mx_UrlPreview_title\">" << esc(preview.title) << "</div>";
    }
    if (!preview.description.empty()) {
        html << "<div class=\"mx_UrlPreview_desc\">" << esc(truncateDescription(preview.description, 200)) << "</div>";
    }
    html << "<div class=\"mx_UrlPreview_url\">" << esc(preview.url) << "</div>";
    html << "</div></div>";

    return html.str();
}

// ================================================================
// URL Extraction from Text
// ================================================================

// Original Kotlin: UrlUtils.kt — find first URL in plain text
UrlMatch extractFirstUrl(const std::string& text) {
    auto match = [](char c) -> bool {
        return std::isalnum(static_cast<unsigned char>(c)) ||
               c == '.' || c == '-' || c == '_' || c == '~' ||
               c == ':' || c == '/' || c == '?' || c == '#' ||
               c == '[' || c == ']' || c == '@' || c == '!' ||
               c == '$' || c == '&' || c == '\'' || c == '(' ||
               c == ')' || c == '*' || c == '+' || c == ',' ||
               c == ';' || c == '=' || c == '%';
    };

    auto endMatch = [](char c) -> bool {
        return c != '.' && c != ',' && c != ';' && c != ':' &&
               c != '!' && c != '?' && c != ')' && c != ']' &&
               c != '\'' && c != '"';
    };

    size_t i = 0;
    while (i < text.size()) {
        // Look for http:// or https://
        if (i + 7 <= text.size() && text.compare(i, 7, "http://") == 0) {
            size_t start = i;
            i += 7;
            while (i < text.size() && match(text[i])) i++;
            size_t end = i;
            // Trim trailing punctuation
            while (end > start && !endMatch(text[end - 1])) end--;
            if (end > start) {
                return {text.substr(start, end - start), static_cast<int>(start), static_cast<int>(end)};
            }
        } else if (i + 8 <= text.size() && text.compare(i, 8, "https://") == 0) {
            size_t start = i;
            i += 8;
            while (i < text.size() && match(text[i])) i++;
            size_t end = i;
            while (end > start && !endMatch(text[end - 1])) end--;
            if (end > start) {
                return {text.substr(start, end - start), static_cast<int>(start), static_cast<int>(end)};
            }
        } else {
            i++;
        }
    }

    return {"", -1, -1};
}

// Original Kotlin: UrlUtils.kt — find all URLs in plain text
std::vector<UrlMatch> extractAllUrls(const std::string& text) {
    std::vector<UrlMatch> result;

    auto match = [](char c) -> bool {
        return std::isalnum(static_cast<unsigned char>(c)) ||
               c == '.' || c == '-' || c == '_' || c == '~' ||
               c == ':' || c == '/' || c == '?' || c == '#' ||
               c == '[' || c == ']' || c == '@' || c == '!' ||
               c == '$' || c == '&' || c == '\'' || c == '(' ||
               c == ')' || c == '*' || c == '+' || c == ',' ||
               c == ';' || c == '=' || c == '%';
    };

    auto endMatch = [](char c) -> bool {
        return c != '.' && c != ',' && c != ';' && c != ':' &&
               c != '!' && c != '?' && c != ')' && c != ']' &&
               c != '\'' && c != '"';
    };

    size_t i = 0;
    while (i < text.size()) {
        size_t start = i;
        bool found = false;

        if (i + 7 <= text.size() && text.compare(i, 7, "http://") == 0) {
            i += 7; found = true;
        } else if (i + 8 <= text.size() && text.compare(i, 8, "https://") == 0) {
            i += 8; found = true;
        }

        if (found) {
            while (i < text.size() && match(text[i])) i++;
            size_t end = i;
            while (end > start && !endMatch(text[end - 1])) end--;
            if (end > start) {
                result.push_back({text.substr(start, end - start),
                                  static_cast<int>(start), static_cast<int>(end)});
            }
        } else {
            i++;
        }
    }

    return result;
}

// ================================================================
// Existing preserved functions
// ================================================================

std::string extractHtmlTitle(const std::string& html) {
    return getTagContent(html, "title");
}

std::string extractMetaDescription(const std::string& html) {
    return getMeta(html, "description", "name");
}

std::string resolveUrl(const std::string& baseUrl, const std::string& relative) {
    if (relative.empty()) return baseUrl;

    if (relative.find("http://") == 0 || relative.find("https://") == 0) return relative;
    if (relative.find("//") == 0) {
        auto proto = baseUrl.find("https://") == 0 ? "https:" : "http:";
        return proto + relative;
    }

    if (relative[0] == '/') {
        auto protoEnd = baseUrl.find("://");
        if (protoEnd == std::string::npos) return relative;
        auto domainEnd = baseUrl.find('/', protoEnd + 3);
        if (domainEnd == std::string::npos) return baseUrl + relative;
        return baseUrl.substr(0, domainEnd) + relative;
    }

    auto lastSlash = baseUrl.rfind('/');
    if (lastSlash == std::string::npos || lastSlash < 8) return baseUrl + "/" + relative;
    return baseUrl.substr(0, lastSlash + 1) + relative;
}

std::vector<std::string> extractUrls(const std::string& html) {
    std::vector<std::string> urls;
    for (const auto* attr : {"href=\"", "src=\"", "href='", "src='"}) {
        size_t pos = 0;
        while ((pos = html.find(attr, pos)) != std::string::npos) {
            pos += strlen(attr);
            auto end = html.find(attr[4] == '"' ? '"' : '\'', pos);
            if (end != std::string::npos) {
                auto url = html.substr(pos, end - pos);
                if (!url.empty() && url.find("http") == 0) {
                    urls.push_back(url);
                }
            }
        }
    }
    return urls;
}

std::string stripHtmlTags(const std::string& html) {
    std::string result;
    bool inTag = false;
    for (char c : html) {
        if (c == '<') inTag = true;
        else if (c == '>') inTag = false;
        else if (!inTag) result += c;
    }

    std::string clean;
    bool wasSpace = false;
    for (char c : result) {
        if (std::isspace(static_cast<unsigned char>(c))) {
            if (!wasSpace) clean += ' ';
            wasSpace = true;
        } else {
            clean += c;
            wasSpace = false;
        }
    }
    return clean;
}

std::string truncateDescription(const std::string& text, size_t maxLen) {
    if (text.size() <= maxLen) return text;

    auto pos = text.rfind(' ', maxLen);
    if (pos == std::string::npos || pos < maxLen / 2) pos = maxLen;

    return text.substr(0, pos) + "...";
}

std::string urlPreviewToJson(const UrlPreview& preview) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };
    std::ostringstream json;
    json << R"({"url": ")" << esc(preview.url) << R"(",)";
    json << R"("title": ")" << esc(preview.title) << R"(",)";
    json << R"("description": ")" << esc(preview.description) << R"(",)";
    json << R"("imageUrl": ")" << esc(preview.imageUrl) << R"(",)";
    json << R"("siteName": ")" << esc(preview.siteName) << R"(",)";
    json << R"("type": ")" << esc(preview.type) << R"(",)";
    json << R"("imageWidth": )" << preview.imageWidth << ",";
    json << R"("imageHeight": )" << preview.imageHeight << ",";
    json << R"("hasImage": )" << (preview.hasImage ? "true" : "false") << ",";
    json << R"("valid": )" << (preview.valid ? "true" : "false") << "}";
    return json.str();
}

} // namespace progressive
