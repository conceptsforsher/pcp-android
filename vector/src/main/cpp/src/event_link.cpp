#include "progressive/event_link.hpp"
#include <sstream>
#include <regex>
#include <ctime>

namespace progressive {

// ---- Event Link Extraction ----

MatrixToLink parseMatrixToUrl(const std::string& url) {
    // https://matrix.to/#/!room:server/$event:server
    MatrixToLink result;
    std::regex toRe(R"(matrix\.to/#/([^?]+))");
    std::smatch match;
    if (std::regex_search(url, match, toRe)) {
        std::string path = match[1];
        auto dollarPos = path.find("$");
        if (dollarPos != std::string::npos) {
            result.eventId = path.substr(dollarPos);
            if (dollarPos > 0 && path[dollarPos - 1] == '/') {
                result.roomId = path.substr(0, dollarPos - 1);
            }
        }
    }
    return result;
}

std::vector<EventLink> extractEventLinks(const std::string& body) {
    std::vector<EventLink> links;

    // Pattern 1: $eventId or $eventId:server (standalone)
    std::regex dollarRe(R"(\$[A-Za-z0-9_\-+/=]+\.[A-Za-z0-9_\-+/=:]+|\$[A-Za-z0-9_\-+/=]{10,})");
    std::smatch match;
    std::string::const_iterator searchStart(body.begin());
    while (std::regex_search(searchStart, body.end(), match, dollarRe)) {
        EventLink link;
        link.eventId = match[0];
        link.originalText = match[0];
        link.startPos = match.position() + (searchStart - body.begin());
        link.endPos = link.startPos + match.length();
        links.push_back(link);
        searchStart = match.suffix().first;
    }

    // Pattern 2: full matrix.to URL
    std::regex urlRe(R"(https?://matrix\.to/#/[^\s]+)");
    searchStart = body.begin();
    while (std::regex_search(searchStart, body.end(), match, urlRe)) {
        auto parsed = parseMatrixToUrl(match[0]);
        if (!parsed.eventId.empty()) {
            // Check if this URL's eventId is already covered by a dollar link
            bool duplicate = false;
            size_t urlStart = match.position() + (searchStart - body.begin());
            size_t urlEnd = urlStart + match.length();
            for (const auto& existing : links) {
                if (existing.startPos >= urlStart && existing.endPos <= urlEnd) {
                    duplicate = true;
                    break;
                }
            }
            if (!duplicate) {
                EventLink link;
                link.eventId = parsed.eventId;
                link.originalText = match[0];
                link.startPos = urlStart;
                link.endPos = urlEnd;
                links.push_back(link);
            }
        }
        searchStart = match.suffix().first;
    }

    // Sort by position
    std::sort(links.begin(), links.end(), [](const EventLink& a, const EventLink& b) {
        return a.startPos < b.startPos;
    });

    return links;
}

std::string formatResolvedEventText(const std::string& senderName, const std::string& body) {
    std::ostringstream out;
    out << senderName << ": " << body;
    return out.str();
}

bool isExpandedText(const std::string& text) {
    // Check if text contains our resolution markers
    return text.find("\u25B8 ") != std::string::npos; // ▸ marker
}

// ---- Timestamps with Seconds ----

std::string formatTimestamp(int64_t epochMs, bool includeSeconds) {
    if (epochMs <= 0) return "";
    time_t ts = epochMs / 1000;
    struct tm result;
    localtime_r(&ts, &result);

    char buf[16];
    if (includeSeconds) {
        strftime(buf, sizeof(buf), "%H:%M:%S", &result);
    } else {
        strftime(buf, sizeof(buf), "%H:%M", &result);
    }
    return std::string(buf);
}

std::string formatFullTimestamp(int64_t epochMs) {
    if (epochMs <= 0) return "";
    time_t ts = epochMs / 1000;
    struct tm result;
    localtime_r(&ts, &result);
    char buf[64];
    strftime(buf, sizeof(buf), "%B %d, %Y %H:%M:%S", &result);
    return std::string(buf);
}

} // namespace progressive
