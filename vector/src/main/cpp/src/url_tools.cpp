#include "progressive/url_tools.hpp"
#include <sstream>
#include <regex>
#include <iomanip>

namespace progressive {

UrlParts parseUrlParts(const std::string& url) {
    UrlParts result;
    if (url.empty()) return result;

    // Protocol
    auto protoEnd = url.find("://");
    if (protoEnd == std::string::npos) return result;

    result.protocol = url.substr(0, protoEnd);
    auto rest = url.substr(protoEnd + 3);

    // Fragment
    auto fragPos = rest.find('#');
    if (fragPos != std::string::npos) {
        result.fragment = rest.substr(fragPos + 1);
        rest = rest.substr(0, fragPos);
    }

    // Query
    auto qPos = rest.find('?');
    if (qPos != std::string::npos) {
        result.query = rest.substr(qPos);
        rest = rest.substr(0, qPos);
    }

    // Path
    auto pathPos = rest.find('/');
    if (pathPos != std::string::npos) {
        result.path = rest.substr(pathPos);
        rest = rest.substr(0, pathPos);
    } else {
        result.path = "/";
    }

    // Host:port
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
    if (std::regex_search(text, match, urlRe)) {
        return match[0];
    }
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
                else {
                    val = -1;
                    break;
                }
            }
            if (val >= 0) {
                result += static_cast<char>(val);
                i += 2;
                continue;
            }
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

// ==== URL Utilities (from UrlUtils.kt:21-50) ====
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

} // namespace progressive
