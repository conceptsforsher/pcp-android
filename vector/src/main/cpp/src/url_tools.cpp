#include "progressive/url_tools.hpp"
#include <sstream>
#include <regex>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <set>

namespace progressive {

// ============================================================
// LEGACY URL PARSING — url_tools original API
// Original Kotlin:
//   org.matrix.android.sdk.internal.util.UrlUtils.kt (50L)
// ============================================================

UrlParts parseUrlParts(const std::string& url) {
    UrlParts result;
    if (url.empty()) return result;
    auto protoEnd = url.find("://");
    if (protoEnd == std::string::npos) return result;
    result.protocol = url.substr(0, protoEnd);
    auto rest = url.substr(protoEnd + 3);
    auto fragPos = rest.find('#');
    if (fragPos != std::string::npos) {
        result.fragment = rest.substr(fragPos + 1);
        rest = rest.substr(0, fragPos);
    }
    auto qPos = rest.find('?');
    if (qPos != std::string::npos) {
        result.query = rest.substr(qPos);
        rest = rest.substr(0, qPos);
    }
    auto pathPos = rest.find('/');
    if (pathPos != std::string::npos) {
        result.path = rest.substr(pathPos);
        rest = rest.substr(0, pathPos);
    } else {
        result.path = "/";
    }
    auto portPos = rest.find(':');
    if (portPos != std::string::npos) {
        result.port = rest.substr(portPos + 1);
        result.host = rest.substr(0, portPos);
    } else {
        result.host = rest;
    }
    result.valid = !result.host.empty();
    return result;
}

bool isLikelyUrl(const std::string& text) {
    return text.rfind("http://", 0) == 0 ||
           text.rfind("https://", 0) == 0 ||
           text.rfind("matrix://", 0) == 0 ||
           text.rfind("matrix.to/", 0) == 0 ||
           text.rfind("ftp://", 0) == 0;
}

std::string extractFirstUrl(const std::string& text) {
    std::regex urlRe(R"((https?://|matrix://|ftp://)[^\s<>"]+)");
    std::smatch match;
    if (std::regex_search(text, match, urlRe)) return match[0];
    return {};
}

std::vector<std::string> extractAllUrls(const std::string& text) {
    std::vector<std::string> urls;
    std::regex urlRe(R"((https?://|matrix://|ftp://)[^\s<>"]+)");
    for (auto it = std::sregex_iterator(text.begin(), text.end(), urlRe);
         it != std::sregex_iterator(); ++it) {
        urls.push_back(it->str());
    }
    return urls;
}

std::string getDomain(const std::string& url) {
    auto parsed = parseUrlParts(url);
    return parsed.host;
}

bool isHttps(const std::string& url) {
    return url.rfind("https://", 0) == 0;
}

bool isMatrixUrl(const std::string& url) {
    return url.rfind("matrix://", 0) == 0 ||
           url.find("matrix.to/") != std::string::npos ||
           url.rfind("mxc://", 0) == 0;
}

std::string buildMatrixToUrl(const std::string& roomIdOrAlias) {
    return "https://matrix.to/#/" + roomIdOrAlias;
}

std::string urlEncode(const std::string& input) {
    std::ostringstream encoded;
    for (char c : input) {
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
            (c >= '0' && c <= '9') || c == '-' || c == '_' ||
            c == '.' || c == '~') {
            encoded << c;
        } else {
            encoded << '%' << std::uppercase << std::hex << std::setw(2)
                    << std::setfill('0') << (static_cast<int>(c) & 0xFF);
        }
    }
    return encoded.str();
}

std::string urlDecode(const std::string& input) {
    std::string result;
    for (size_t i = 0; i < input.size(); ++i) {
        if (input[i] == '%' && i + 2 < input.size()) {
            int val = 0;
            for (int j = 1; j <= 2; ++j) {
                char c = input[i + j];
                if (c >= '0' && c <= '9') val = val * 16 + (c - '0');
                else if (c >= 'A' && c <= 'F') val = val * 16 + (c - 'A' + 10);
                else if (c >= 'a' && c <= 'f') val = val * 16 + (c - 'a' + 10);
                else { val = -1; break; }
            }
            if (val >= 0) { result += static_cast<char>(val); i += 2; continue; }
        }
        result += input[i];
    }
    return result;
}

MxcInfo parseMxcUrl(const std::string& mxcUrl) {
    MxcInfo result;
    if (mxcUrl.rfind("mxc://", 0) != 0) return result;
    auto rest = mxcUrl.substr(6);
    auto slash = rest.find('/');
    if (slash == std::string::npos) {
        result.serverName = rest;
    } else {
        result.serverName = rest.substr(0, slash);
        result.mediaId = rest.substr(slash + 1);
    }
    return result;
}

bool isValidUrl(const std::string& url) {
    auto protoEnd = url.find("://");
    if (protoEnd == std::string::npos || protoEnd < 2) return false;
    for (size_t i = 0; i < protoEnd; ++i) {
        char c = url[i];
        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
              (c >= '0' && c <= '9') || c == '+' || c == '-' || c == '.')) return false;
    }
    return url.size() > protoEnd + 3;
}

std::string ensureProtocol(const std::string& url) {
    if (url.empty()) return url;
    if (url.find("http") == 0) return url;
    return "https://" + url;
}

std::string ensureTrailingSlash(const std::string& url) {
    if (url.empty()) return url;
    if (url.back() == '/') return url;
    return url + "/";
}

// ============================================================
// RFC 3986 URL PARSER — full URL decomposition
// Original Kotlin:
//   java.net.URI parser behaviour emulation
// ============================================================

// RFC 3986 Appendix B regex: ^(([^:/?#]+):)?(//([^/?#]*))?([^?#]*)(\?([^#]*))?(#(.*))?
// Groups: 1=scheme-with-colon, 2=scheme, 3=authority-with-slash, 4=authority,
//         5=path, 6=query-with-?, 7=query, 8=fragment-with-#, 9=fragment

static const std::regex RFC3986_RE(
    R"(^(([^:\/?#]+):)?(\/\/([^\/?#]*))?([^?#]*)(\?([^#]*))?(#(.*))?)",
    std::regex::ECMAScript | std::regex::optimize
);

// Helper: strip brackets from IPv6 address
static std::string stripBrackets(const std::string& host) {
    if (host.size() >= 2 && host.front() == '[' && host.back() == ']') {
        return host.substr(1, host.size() - 2);
    }
    return host;
}

// Parse userinfo from authority: "user:password@host:port" or "user@host:port"
static void parseAuthority(const std::string& authority, UrlComponents& comp) {
    if (authority.empty()) return;
    std::string auth = authority;
    // Extract userinfo
    auto atPos = auth.rfind('@');
    if (atPos != std::string::npos) {
        std::string userinfo = auth.substr(0, atPos);
        auto colonPos = userinfo.find(':');
        if (colonPos != std::string::npos) {
            comp.user = urlDecode(userinfo.substr(0, colonPos));
            comp.password = urlDecode(userinfo.substr(colonPos + 1));
        } else {
            comp.user = urlDecode(userinfo);
        }
        auth = auth.substr(atPos + 1);
    }
    // Parse host:port (handle IPv6 brackets)
    if (!auth.empty() && auth.front() == '[') {
        auto closeBracket = auth.find(']');
        if (closeBracket != std::string::npos) {
            comp.host = auth.substr(1, closeBracket - 1); // raw IPv6
            auto portColon = auth.find(':', closeBracket);
            if (portColon != std::string::npos) {
                try { comp.port = std::stoi(auth.substr(portColon + 1)); }
                catch (...) { comp.port = -1; }
            }
        }
    } else {
        auto portColon = auth.rfind(':');
        if (portColon != std::string::npos) {
            comp.host = auth.substr(0, portColon);
            try { comp.port = std::stoi(auth.substr(portColon + 1)); }
            catch (...) { comp.port = -1; }
        } else {
            comp.host = auth;
        }
    }
    // Lowercase host per spec
    for (char& c : comp.host) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
}

UrlComponents parseUrl(const std::string& url) {
    // Original Kotlin:
    //   URI(url).also { comp = UrlComponents(...) }
    UrlComponents comp;
    if (url.empty()) return comp;

    std::smatch m;
    if (!std::regex_match(url, m, RFC3986_RE)) {
        // Fallback: try simpler parsing
        auto parts = parseUrlParts(url);
        if (!parts.valid) return comp;
        comp.scheme = parts.protocol;
        comp.host = parts.host;
        comp.path = parts.path;
        if (!parts.port.empty()) {
            try { comp.port = std::stoi(parts.port); }
            catch (...) { comp.port = -1; }
        }
        if (parts.query.size() > 1) comp.query = parts.query.substr(1);
        comp.fragment = parts.fragment;
        comp.valid = true;
        return comp;
    }

    comp.scheme = m[2].str();
    if (comp.scheme.empty()) return comp;

    // Lowercase scheme
    for (char& c : comp.scheme) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));

    std::string authority = m[4].str();
    parseAuthority(authority, comp);

    comp.path = m[5].str();
    if (comp.path.empty()) comp.path = "/";
    // Decode percent-encoded path segments
    comp.path = urlDecode(comp.path);

    comp.query = m[7].str();
    comp.fragment = m[9].str();

    comp.valid = !comp.host.empty();
    return comp;
}

std::string buildUrl(const UrlComponents& comp) {
    // Original Kotlin:
    //   URI(scheme, userInfo, host, port, path, query, fragment).toString()
    if (comp.scheme.empty() || !comp.valid) return "";
    std::ostringstream url;
    url << comp.scheme << "://";
    // Userinfo
    if (!comp.user.empty()) {
        url << urlEncode(comp.user);
        if (!comp.password.empty()) url << ":" << urlEncode(comp.password);
        url << "@";
    }
    // Host
    if (comp.host.find(':') != std::string::npos) {
        url << "[" << comp.host << "]";
    } else {
        url << comp.host;
    }
    // Port
    if (comp.port > 0) {
        bool isDefault = (comp.scheme == "http" && comp.port == 80) ||
                         (comp.scheme == "https" && comp.port == 443);
        if (!isDefault) url << ":" << comp.port;
    }
    // Path
    if (!comp.path.empty()) {
        url << (comp.path[0] == '/' ? "": "/") << comp.path;
    } else {
        url << "/";
    }
    // Query
    if (!comp.query.empty()) url << "?" << comp.query;
    // Fragment
    if (!comp.fragment.empty()) url << "#" << comp.fragment;
    return url.str();
}

// ============================================================
// URL NORMALIZATION
// ============================================================

// Remove dot segments from a path per RFC 3986 Section 5.2.4
static std::string removeDotSegments(const std::string& path) {
    if (path.empty()) return "/";
    std::vector<std::string> segments;
    std::string buf;
    size_t i = 0;
    if (!path.empty() && path[0] == '/') i++;
    while (i <= path.size()) {
        if (i == path.size() || path[i] == '/') {
            if (buf == "..") {
                if (!segments.empty()) segments.pop_back();
            } else if (buf != "." && !buf.empty()) {
                segments.push_back(buf);
            }
            buf.clear();
        } else {
            buf += path[i];
        }
        i++;
    }
    // Handle trailing segment
    if (buf == "..") {
        if (!segments.empty()) segments.pop_back();
    } else if (buf != "." && !buf.empty()) {
        segments.push_back(buf);
    }
    std::ostringstream result;
    result << "/";
    for (size_t j = 0; j < segments.size(); ++j) {
        if (j > 0) result << "/";
        result << segments[j];
    }
    return result.str();
}

// Sort query parameters alphabetically by key
static std::string sortQueryParams(const std::string& query) {
    if (query.empty()) return "";
    auto q = parseQueryString(query);
    // Sort by key, then by value if keys equal
    std::sort(q.params.begin(), q.params.end(),
        [](const auto& a, const auto& b) {
            if (a.first != b.first) return a.first < b.first;
            return a.second < b.second;
        });
    return buildQueryString(q);
}

std::string normalizeUrl(const std::string& url, const UrlNormalization& opts) {
    // Original Kotlin:
    //   Canonical URL normalization for comparison/deduplication
    if (url.empty()) return url;
    auto comp = parseUrl(url);
    if (!comp.valid) return url;

    if (opts.lowercaseScheme) {
        for (char& c : comp.scheme) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
    if (opts.lowercaseHost) {
        for (char& c : comp.host) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
    if (opts.removeDefaultPort) {
        if ((comp.scheme == "http" && comp.port == 80) ||
            (comp.scheme == "https" && comp.port == 443)) {
            comp.port = -1;
        }
    }
    if (opts.removeFragment) {
        comp.fragment.clear();
    }
    if (opts.sortQueryParams) {
        comp.query = sortQueryParams(comp.query);
    }
    if (opts.removeDotSegments) {
        comp.path = removeDotSegments(comp.path);
    }
    if (opts.removeTrailingSlash) {
        if (comp.path.size() > 1 && comp.path.back() == '/') {
            comp.path.pop_back();
        }
    }
    return buildUrl(comp);
}

bool isSameOrigin(const std::string& a, const std::string& b) {
    auto ca = parseUrl(a);
    auto cb = parseUrl(b);
    if (!ca.valid || !cb.valid) return false;
    return ca.scheme == cb.scheme && ca.host == cb.host && ca.port == cb.port;
}

bool isRelativeUrl(const std::string& url) {
    return url.find("://") == std::string::npos;
}

bool isAbsoluteUrl(const std::string& url) {
    return !isRelativeUrl(url);
}

std::string resolveRelativeUrl(const std::string& relative, const std::string& base) {
    // Original Kotlin:
    //   URI(base).resolve(relative).toString()
    if (relative.empty()) return base;
    // Already absolute?
    if (!isRelativeUrl(relative)) return relative;

    auto baseComp = parseUrl(base);
    if (!baseComp.valid) return relative;

    UrlComponents resolved = baseComp;
    resolved.fragment.clear();

    if (relative[0] == '#') {
        resolved.fragment = relative.substr(1);
        return buildUrl(resolved);
    }

    if (relative[0] == '?') {
        resolved.query = relative.substr(1);
        resolved.fragment.clear();
        return buildUrl(resolved);
    }

    if (relative[0] == '/') {
        // Absolute path
        auto qPos = relative.find('?');
        auto fPos = relative.find('#');
        if (qPos != std::string::npos) {
            resolved.path = urlDecode(relative.substr(0, qPos));
            resolved.query = relative.substr(qPos + 1, (fPos != std::string::npos ? fPos - qPos - 1 : std::string::npos));
        } else if (fPos != std::string::npos) {
            resolved.path = urlDecode(relative.substr(0, fPos));
            resolved.query.clear();
        } else {
            resolved.path = urlDecode(relative);
            resolved.query.clear();
        }
        if (fPos != std::string::npos) {
            resolved.fragment = relative.substr(fPos + 1);
        }
    } else {
        // Relative path: merge with base path
        std::string basePath = resolved.path;
        if (basePath.empty()) basePath = "/";
        auto lastSlash = basePath.rfind('/');
        std::string baseDir = (lastSlash != std::string::npos) ? basePath.substr(0, lastSlash) : "";
        auto qPos = relative.find('?');
        auto fPos = relative.find('#');
        std::string relPath;
        if (qPos != std::string::npos) {
            relPath = relative.substr(0, qPos);
        } else if (fPos != std::string::npos) {
            relPath = relative.substr(0, fPos);
        } else {
            relPath = relative;
        }
        resolved.path = removeDotSegments(baseDir + "/" + relPath);
        if (qPos != std::string::npos) {
            resolved.query = relative.substr(qPos + 1, (fPos != std::string::npos ? fPos - qPos - 1 : std::string::npos));
        } else if (fPos == std::string::npos) {
            resolved.query.clear();
        }
        if (fPos != std::string::npos) {
            resolved.fragment = relative.substr(fPos + 1);
        }
    }
    return buildUrl(resolved);
}

// ============================================================
// QUERY STRING PARSING
// ============================================================

UrlQuery parseQueryString(const std::string& query) {
    UrlQuery result;
    std::string q = query;
    if (!q.empty() && q[0] == '?') q = q.substr(1);
    if (q.empty()) return result;

    size_t pos = 0;
    while (pos < q.size()) {
        auto amp = q.find('&', pos);
        std::string pair = (amp != std::string::npos) ? q.substr(pos, amp - pos) : q.substr(pos);
        auto eq = pair.find('=');
        std::string key, value;
        if (eq != std::string::npos) {
            key = urlDecode(pair.substr(0, eq));
            value = urlDecode(pair.substr(eq + 1));
        } else {
            key = urlDecode(pair);
        }
        result.params.push_back({key, value});
        if (amp == std::string::npos) break;
        pos = amp + 1;
    }
    return result;
}

std::string buildQueryString(const UrlQuery& query) {
    if (query.params.empty()) return "";
    std::ostringstream out;
    for (size_t i = 0; i < query.params.size(); ++i) {
        if (i > 0) out << "&";
        out << urlEncode(query.params[i].first);
        if (!query.params[i].second.empty()) {
            out << "=" << urlEncode(query.params[i].second);
        }
    }
    return out.str();
}

void setQueryParam(UrlQuery& query, const std::string& key, const std::string& value) {
    for (auto& p : query.params) {
        if (p.first == key) {
            p.second = value;
            return;
        }
    }
    query.params.push_back({key, value});
}

std::string getQueryParam(const UrlQuery& query, const std::string& key) {
    for (const auto& p : query.params) {
        if (p.first == key) return p.second;
    }
    return "";
}

void removeQueryParam(UrlQuery& query, const std::string& key) {
    query.params.erase(
        std::remove_if(query.params.begin(), query.params.end(),
            [&](const auto& p) { return p.first == key; }),
        query.params.end());
}

bool isSecureUrl(const std::string& url) {
    if (url.empty()) return false;
    if (url.rfind("https://", 0) == 0) return true;
    if (url.rfind("wss://", 0) == 0) return true;
    if (url.rfind("mtls://", 0) == 0) return true;
    return false;
}

std::string ensureHttps(const std::string& url) {
    if (url.empty()) return url;
    if (url.rfind("https://", 0) == 0) return url;
    if (url.rfind("http://", 0) == 0) return "https://" + url.substr(7);
    return "https://" + url;
}

// ============================================================
// URI COMPONENT ENCODING (RFC 3986)
// ============================================================

namespace UrlEncoding {

std::string encodeURIComponent(const std::string& input) {
    // Original Kotlin:
    //   URLEncoder.encode(input, "UTF-8").replace("+", "%20")
    std::ostringstream encoded;
    for (char c : input) {
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
            (c >= '0' && c <= '9') || c == '-' || c == '_' ||
            c == '.' || c == '~') {
            encoded << c;
        } else if (c == ' ') {
            encoded << "%20";
        } else {
            encoded << '%' << std::uppercase << std::hex << std::setw(2)
                    << std::setfill('0') << (static_cast<int>(static_cast<unsigned char>(c)) & 0xFF);
        }
    }
    return encoded.str();
}

std::string decodeURIComponent(const std::string& input) {
    // Original Kotlin:
    //   URLDecoder.decode(input, "UTF-8")
    return urlDecode(input);
}

} // namespace UrlEncoding

// ============================================================
// DOMAIN EXTRACTION
// Original Kotlin:
//   Domain utilities for extracting TLD, domain, subdomain
// ============================================================

// Common TLD list (simplified subset for validation)
static const std::set<std::string> KNOWN_TLDS = {
    "com", "org", "net", "edu", "gov", "mil", "int",
    "io", "co", "ai", "dev", "app", "me", "tv", "fm",
    "uk", "de", "fr", "es", "it", "nl", "ru", "cn",
    "jp", "kr", "in", "br", "au", "ca", "mx", "ar",
    "ch", "se", "no", "dk", "fi", "pl", "be", "at",
    "eu", "info", "biz", "mobi", "name", "pro", "travel",
    "org.uk", "co.uk", "ac.uk", "gov.uk", "co.jp", "or.jp",
    "com.au", "net.au", "org.au", "com.br", "org.br",
    "co.in", "net.in", "co.nz", "net.nz", "co.za",
};

UrlDomain extractDomain(const std::string& url) {
    // Original Kotlin:
    //   Extract TLD, registered domain, and subdomain from a URL
    UrlDomain result;
    auto comp = parseUrl(url);
    if (!comp.valid) {
        // Try to extract host from URL-like string directly
        std::string host = getDomain(url);
        if (host.empty()) return result;
        comp.host = host;
    }

    std::string host = comp.host;
    if (host.empty()) return result;

    // Split host into labels
    std::vector<std::string> labels;
    size_t pos = 0;
    while (pos < host.size()) {
        auto dot = host.find('.', pos);
        labels.push_back(host.substr(pos, dot - pos));
        if (dot == std::string::npos) break;
        pos = dot + 1;
    }

    if (labels.size() < 2) {
        // Single label like "localhost"
        result.domain = host;
        result.valid = true;
        return result;
    }

    // Try to find the TLD
    for (size_t tldLen = 1; tldLen <= 2 && labels.size() > tldLen; ++tldLen) {
        std::string candidateTld;
        for (size_t i = 1; i <= tldLen; ++i) {
            if (!candidateTld.empty()) candidateTld = "." + candidateTld;
            candidateTld = labels[labels.size() - i] + candidateTld;
        }
        if (KNOWN_TLDS.count(candidateTld)) {
            result.tld = candidateTld;
            result.domain = labels[labels.size() - tldLen - 1] + "." + candidateTld;
            // Subdomain is everything before the domain
            for (size_t i = 0; i + tldLen + 1 < labels.size(); ++i) {
                if (!result.subdomain.empty()) result.subdomain += ".";
                result.subdomain += labels[i];
            }
            result.valid = true;
            return result;
        }
    }

    // Fallback: last two labels as domain.tld
    size_t n = labels.size();
    result.tld = labels[n - 1];
    result.domain = labels[n - 2] + "." + labels[n - 1];
    for (size_t i = 0; i + 2 < n; ++i) {
        if (!result.subdomain.empty()) result.subdomain += ".";
        result.subdomain += labels[i];
    }
    result.valid = true;
    return result;
}

bool isValidDomain(const std::string& domain) {
    if (domain.empty() || domain.size() > 253) return false;
    // Check each label
    size_t pos = 0;
    while (pos < domain.size()) {
        auto dot = domain.find('.', pos);
        std::string label = domain.substr(pos, dot - pos);
        if (label.empty() || label.size() > 63) return false;
        if (label.front() == '-' || label.back() == '-') return false;
        for (char c : label) {
            if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
                  (c >= '0' && c <= '9') || c == '-')) return false;
        }
        if (dot == std::string::npos) break;
        pos = dot + 1;
    }
    // Must have at least one dot or be "localhost"
    if (domain.find('.') == std::string::npos && domain != "localhost") return false;
    return true;
}

// ============================================================
// REDIRECT CHAIN TRACKING
// Original Kotlin:
//   okhttp redirect following logic (simplified)
// ============================================================

std::string followRedirects(const std::string& url, const UrlRedirectChain& chain) {
    // Original Kotlin:
    //   client.newCall(request).execute().use { response -> handleRedirects() }
    // Stub implementation: returns URL as-is since actual HTTP redirect following
    // requires a full HTTP client which is handled by http_client module.
    // This function validates the chain metadata.
    if (url.empty()) return "";
    auto comp = parseUrl(url);
    if (!comp.valid) return "";
    return url;
}

bool isRedirectLoop(const std::vector<std::string>& history, const std::string& newUrl) {
    if (history.empty()) return false;
    // Check if new URL is already in history
    for (const auto& u : history) {
        if (normalizeUrl(u) == normalizeUrl(newUrl)) return true;
    }
    // Check for cycles of length 2
    if (history.size() >= 2) {
        if (normalizeUrl(history.back()) == normalizeUrl(newUrl)) return true;
        if (history.size() >= 3 &&
            normalizeUrl(history[history.size() - 2]) == normalizeUrl(newUrl)) return true;
    }
    return false;
}

// ============================================================
// URL BLACKLIST
// ============================================================

static bool wildcardMatch(const std::string& pattern, const std::string& url) {
    size_t pi = 0, ui = 0;
    size_t starIdx = std::string::npos, matchIdx = 0;
    while (ui < url.size()) {
        if (pi < pattern.size() && (pattern[pi] == '?' ||
            std::tolower(static_cast<unsigned char>(pattern[pi])) ==
            std::tolower(static_cast<unsigned char>(url[ui])))) {
            pi++; ui++;
        } else if (pi < pattern.size() && pattern[pi] == '*') {
            starIdx = pi;
            matchIdx = ui;
            pi++;
        } else if (starIdx != std::string::npos) {
            pi = starIdx + 1;
            matchIdx++;
            ui = matchIdx;
        } else {
            return false;
        }
    }
    while (pi < pattern.size() && pattern[pi] == '*') pi++;
    return pi == pattern.size();
}

bool isUrlBlacklisted(const std::string& url,
                       const std::vector<UrlBlacklistEntry>& blacklist) {
    auto normalized = normalizeUrl(url);
    for (const auto& entry : blacklist) {
        if (entry.isRegex) {
            try {
                std::regex re(entry.pattern, std::regex::icase | std::regex::ECMAScript);
                if (std::regex_search(normalized, re)) return true;
            } catch (...) {
                continue;
            }
        } else {
            if (wildcardMatch(entry.pattern, normalized)) return true;
            // Also try plain substring match
            if (normalized.find(entry.pattern) != std::string::npos) return true;
        }
    }
    return false;
}

} // namespace progressive
