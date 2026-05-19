#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>

namespace progressive {

// ============================================================
// URL PARTS — legacy wrapper (kept for backward compat)
// ============================================================

struct UrlParts {
    std::string protocol;
    std::string host;
    std::string port;
    std::string path;
    std::string query;
    std::string fragment;
    bool valid = false;
};

UrlParts parseUrlParts(const std::string& url);
bool isLikelyUrl(const std::string& text);
std::string extractFirstUrl(const std::string& text);
std::vector<std::string> extractAllUrls(const std::string& text);
std::string getDomain(const std::string& url);
bool isHttps(const std::string& url);
bool isMatrixUrl(const std::string& url);
std::string buildMatrixToUrl(const std::string& roomIdOrAlias);
std::string urlEncode(const std::string& input);
std::string urlDecode(const std::string& input);

struct MxcInfo { std::string serverName; std::string mediaId; };
MxcInfo parseMxcUrl(const std::string& mxcUrl);
bool isValidUrl(const std::string& url);
std::string ensureProtocol(const std::string& url);
std::string ensureTrailingSlash(const std::string& url);

// ============================================================
// URL COMPONENTS — RFC 3986 compliant URL representation
// Original Kotlin:
//   java.net.URI / okhttp3.HttpUrl
// ============================================================

struct UrlComponents {
    std::string scheme;
    std::string user;
    std::string password;
    std::string host;
    int port = -1;  // -1 = use default for scheme
    std::string path;    // "/" if empty
    std::string query;   // without "?" prefix
    std::string fragment; // without "#" prefix
    bool valid = false;
};

// Full RFC 3986 URL parser. Handles authority, userinfo, percent-encoding.
UrlComponents parseUrl(const std::string& url);

// Assemble a URL from components. Returns "" if invalid.
std::string buildUrl(const UrlComponents& comp);

// ============================================================
// URL NORMALIZATION — canonicalize URLs for comparison
// ============================================================

struct UrlNormalization {
    static constexpr int DEFAULT_HTTP_PORT = 80;
    static constexpr int DEFAULT_HTTPS_PORT = 443;

    bool lowercaseScheme = true;
    bool lowercaseHost = true;
    bool removeDefaultPort = true;
    bool removeFragment = false;
    bool sortQueryParams = true;
    bool removeTrailingSlash = true;
    bool removeDotSegments = true;
};

// Normalize a URL for comparison. Returns canonical form.
std::string normalizeUrl(const std::string& url,
                         const UrlNormalization& opts = UrlNormalization{});

// Check if two URLs share the same origin (scheme + host + port).
bool isSameOrigin(const std::string& a, const std::string& b);

// Check if URL is relative (no scheme).
bool isRelativeUrl(const std::string& url);

// Check if URL is absolute (has scheme).
bool isAbsoluteUrl(const std::string& url);

// Resolve a relative URL against a base URL.
// e.g. resolveRelativeUrl("/foo", "https://example.com/bar") → "https://example.com/foo"
std::string resolveRelativeUrl(const std::string& relative, const std::string& base);

// ============================================================
// URL QUERY — parse and manipulate query strings
// ============================================================

struct UrlQuery {
    std::vector<std::pair<std::string, std::string>> params;

    bool empty() const { return params.empty(); }
    size_t size() const { return params.size(); }
    void clear() { params.clear(); }
};

// Parse a query string (with or without leading '?') into key-value pairs.
UrlQuery parseQueryString(const std::string& query);

// Build a query string from key-value pairs (without leading '?').
std::string buildQueryString(const UrlQuery& query);

// Set a query parameter (adds if not present, replaces if present).
void setQueryParam(UrlQuery& query, const std::string& key, const std::string& value);

// Get the first value for a key. Returns empty string if not found.
std::string getQueryParam(const UrlQuery& query, const std::string& key);

// Remove all parameters with the given key.
void removeQueryParam(UrlQuery& query, const std::string& key);

// Check if the URL uses a secure transport (https, wss, mtls).
bool isSecureUrl(const std::string& url);

// Convert an HTTP URL to HTTPS.
std::string ensureHttps(const std::string& url);

// ============================================================
// URI ENCODING — percent-encoding per RFC 3986
// ============================================================

namespace UrlEncoding {
    // Encode a string for use in a URI component.
    // Preserves unreserved chars: A-Z a-z 0-9 - _ . ~
    std::string encodeURIComponent(const std::string& input);

    // Decode a percent-encoded URI component.
    std::string decodeURIComponent(const std::string& input);
}

// ============================================================
// URL DOMAIN — extract and validate domain information
// ============================================================

struct UrlDomain {
    std::string tld;          // top-level domain, e.g. "com"
    std::string domain;       // registered domain, e.g. "example.com"
    std::string subdomain;    // subdomain portion, e.g. "www" or "matrix"
    bool valid = false;
};

// Extract domain components from a URL.
UrlDomain extractDomain(const std::string& url);

// Validate that a domain string is well-formed.
bool isValidDomain(const std::string& domain);

// ============================================================
// URL REDIRECT CHAIN — follow and detect redirect loops
// ============================================================

struct UrlRedirectChain {
    std::vector<std::string> urls;  // chain of URLs visited
    int maxHops = 10;               // max redirects before giving up
    bool followMetaRefresh = false;
    int timeoutSeconds = 30;
};

// Follow a redirect chain starting from a URL.
// Returns the final URL in the chain (or empty if loop detected).
// This is a stub for client-side use; real redirect following requires HTTP.
std::string followRedirects(const std::string& url,
                            const UrlRedirectChain& chain = UrlRedirectChain{});

// Detect if a URL chain contains a redirect loop.
bool isRedirectLoop(const std::vector<std::string>& history, const std::string& newUrl);

// ============================================================
// URL BLACKLIST — pattern-based URL filtering
// ============================================================

struct UrlBlacklistEntry {
    std::string pattern;
    bool isRegex = false;
    std::string category;  // "phishing", "malware", "spam", etc.
};

// Check if a URL matches any entry in a blacklist.
bool isUrlBlacklisted(const std::string& url,
                      const std::vector<UrlBlacklistEntry>& blacklist);

} // namespace progressive
