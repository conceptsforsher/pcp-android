#include <nlohmann/json.hpp>
#pragma once
#include <string>

namespace pcp {

struct SpoilerMatch {
    std::string reason;          // spoiler reason (e.g. "spoiler", "NSFW")
    int contentStart = 0;        // start of hidden content
    int contentEnd = 0;          // end of hidden content
};

// Parse spoiler from HTML (data-mx-spoiler attribute)
SpoilerMatch parseSpoilerFromHtml(const std::string& html);

// Build spoiler HTML: <span data-mx-spoiler="reason">content</span>
std::string buildSpoilerHtml(const std::string& content, const std::string& reason = "");

// Extract reason from spoiler HTML
std::string extractSpoilerReason(const std::string& html);

// Check if content contains a spoiler
bool hasSpoiler(const std::string& html);

// Format spoiler for plain text: "(spoiler: reason) ..."
std::string formatSpoilerPlain(const std::string& content, const std::string& reason);

} // namespace pcp
