#include "pcp/spoiler_utils.hpp"
#include <regex>

namespace pcp {

SpoilerMatch parseSpoilerFromHtml(const std::string& html) {
    SpoilerMatch match;
    std::regex spoilerRegex(
        R"regex(<span\s+data-mx-spoiler(?:="([^"]*)")?\s*>(.*?)</span>)regex",
        std::regex::icase
    );
    std::smatch m;
    if (std::regex_search(html, m, spoilerRegex)) {
        match.reason = (!m[1].matched || m[1].str().empty()) ? "spoiler" : m[1].str();
        match.contentStart = static_cast<int>(m.position(2));
        match.contentEnd = static_cast<int>(m.position(2) + m[2].length());
    }
    return match;
}

std::string buildSpoilerHtml(const std::string& content, const std::string& reason) {
    if (reason.empty()) {
        return "<span data-mx-spoiler>" + content + "</span>";
    }
    return "<span data-mx-spoiler=\"" + reason + "\">" + content + "</span>";
}

std::string extractSpoilerReason(const std::string& html) {
    std::regex reasonRegex(R"regex(data-mx-spoiler="([^"]*)")regex", std::regex::icase);
    std::smatch m;
    if (std::regex_search(html, m, reasonRegex)) {
        return m[1].str();
    }
    if (html.find("data-mx-spoiler") != std::string::npos) {
        return "spoiler";
    }
    return "";
}

bool hasSpoiler(const std::string& html) {
    return html.find("data-mx-spoiler") != std::string::npos;
}

std::string formatSpoilerPlain(const std::string& content, const std::string& reason) {
    if (reason.empty()) {
        return "(spoiler) " + content;
    }
    return "(spoiler: " + reason + ") " + content;
}

} // namespace pcp
