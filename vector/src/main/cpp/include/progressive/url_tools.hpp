#ifndef PROGRESSIVE_URL_TOOLS_HPP
#define PROGRESSIVE_URL_TOOLS_HPP

#include <string>
#include <vector>

namespace progressive {

struct UrlParts {
    std::string protocol;     // "https"
    std::string host;         // "matrix.example.com"
    std::string port;         // "8448" or "" for default
    std::string path;         // "/_matrix/client/v3/..."
    std::string query;        // "?access_token=..."
    std::string fragment;     // "#anchor"
    bool valid = false;
};

// Parse a URL into its components.
UrlParts parseUrlParts(const std::string& url);

// Check if a string is likely a URL (starts with http/https/ftp/matrix).
bool isLikelyUrl(const std::string& text);

// Extract the first URL from plain text.
std::string extractFirstUrl(const std::string& text);

// Extract all URLs from plain text.
std::vector<std::string> extractAllUrls(const std::string& text);

// Get domain name from a URL (e.g. "https://example.com/path" → "example.com").
std::string getDomain(const std::string& url);

// Check if URL uses HTTPS.
bool isHttps(const std::string& url);

// Check if URL is a Matrix URL (matrix://, matrix:u/, or matrix.to).
bool isMatrixUrl(const std::string& url);

// Build a Matrix.to URL from a room ID or alias.
std::string buildMatrixToUrl(const std::string& roomIdOrAlias);

// Encode a string for URL query parameters.
std::string urlEncode(const std::string& input);

// Decode a URL-encoded string.
std::string urlDecode(const std::string& input);

// Extract MXC server name and media ID from mxc:// URL.
struct MxcInfo { std::string serverName; std::string mediaId; };
MxcInfo parseMxcUrl(const std::string& mxcUrl);

// Check if a string is a valid URL (can be parsed).
// From: org.matrix.android.sdk.internal.util.UrlUtils.kt (50L)
bool isValidUrl(const std::string& url);

// Ensure string starts with "https://" if it doesn't already have a protocol.
// "matrix.org" → "https://matrix.org", "http://..." → unchanged, "" → ""
std::string ensureProtocol(const std::string& url);

// Ensure string ends with "/" (unless empty).
// "https://example.com" → "https://example.com/"
std::string ensureTrailingSlash(const std::string& url);

} // namespace progressive

#endif // PROGRESSIVE_URL_TOOLS_HPP
