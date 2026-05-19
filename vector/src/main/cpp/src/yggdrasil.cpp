#include "progressive/yggdrasil.hpp"
#include <sstream>
#include <regex>
#include <cctype>
#include <algorithm>

namespace progressive {

bool isYggdrasilAddress(const std::string& addr) {
    // Yggdrasil uses 200::/7 prefix
    // Accept both bracketed and unbracketed forms
    std::string clean = addr;
    if (!clean.empty() && clean.front() == '[') {
        size_t end = clean.find(']');
        if (end != std::string::npos) clean = clean.substr(1, end - 1);
    }

    // IPv6 with 200::/7 prefix
    // 200::/7 covers 200:: to 3ff:... (200 = 0010 0000, /7 = first 7 bits: 0010 00xx)
    // So valid: 200::, 201::, ..., 3ff::
    if (clean.size() < 4) return false;
    auto lower = clean;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

    // Check if starts with 20x, 2xx, 30x-3fx
    if (lower[0] == '2') {
        if (lower[1] >= '0' && lower[1] <= 'f') return true; // 200:: to 2ff::
    }
    if (lower[0] == '3') {
        if (lower[1] >= '0' && lower[1] <= 'f') return true; // 300:: to 3ff::
    }
    return false;
}

bool isYggdrasilDomain(const std::string& hostname) {
    // .ygg is the Yggdrasil TLD
    if (hostname.size() < 4) return false;
    auto dot = hostname.rfind(".ygg");
    return dot != std::string::npos && dot == hostname.size() - 4;
}

std::string normalizeYggAddress(const std::string& addr) {
    std::string result = addr;
    // Strip brackets
    if (!result.empty() && result.front() == '[') {
        size_t end = result.find(']');
        if (end != std::string::npos) result = result.substr(1, end - 1);
    }
    // Lowercase
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

std::string buildYggHomeserverUrl(
    const std::string& yggAddress,
    int port,
    bool tls
) {
    std::ostringstream url;
    url << (tls ? "https" : "http") << "://";
    url << "[" << normalizeYggAddress(yggAddress) << "]";
    if (port > 0 && port != (tls ? 443 : 80)) {
        url << ":" << port;
    }
    return url.str();
}

std::string rewriteHomeserverUrl(
    const std::string& originalUrl,
    const std::string& yggAddress
) {
    if (yggAddress.empty()) return {};

    // Extract scheme and path from original
    bool tls = originalUrl.find("https://") == 0;
    int defaultPort = tls ? 443 : 80;

    // Try to extract port from original URL
    std::regex portRe(R"(https?://[^/:]+(?::(\d+))?)");
    std::smatch match;
    int port = defaultPort;
    if (std::regex_search(originalUrl, match, portRe)) {
        if (match[1].matched) {
            port = std::stoi(match[1]);
        }
    }

    return buildYggHomeserverUrl(yggAddress, port, tls);
}

} // namespace progressive
