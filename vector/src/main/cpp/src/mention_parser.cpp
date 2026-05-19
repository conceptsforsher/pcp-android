#include "progressive/mention_parser.hpp"
#include <regex>
#include <algorithm>

namespace progressive {

ParsedMentions parseMentions(const std::string& body, const std::string& formattedBody) {
    ParsedMentions result;

    // Check for @room/@here/@channel
    auto lower = body;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    if (lower.find("@room") != std::string::npos ||
        lower.find("@here") != std::string::npos ||
        lower.find("@channel") != std::string::npos ||
        lower.find("@everyone") != std::string::npos) {
        result.hasEveryone = true;
    }

    // Parse Matrix pills from formattedBody: <a href="https://matrix.to/#/@user:server">Name</a>
    if (!formattedBody.empty()) {
        std::regex pillRe(R"RE(<a\s+href="https://matrix\.to/#/(@[^"]+)">([^<]+)</a>)RE");
        for (auto it = std::sregex_iterator(formattedBody.begin(), formattedBody.end(), pillRe);
             it != std::sregex_iterator(); ++it) {
            Mention m;
            m.userId = (*it)[1];
            m.displayName = (*it)[2];
            m.isRoom = (m.userId[1] == '!');
            m.startPos = it->position();
            m.endPos = m.startPos + it->length();
            result.mentions.push_back(m);
        }
    }

    // Also parse plain text @user:server mentions
    auto plainMentions = extractAtMentions(body);
    for (const auto& pm : plainMentions) {
        // Check if already captured by pill parsing
        bool already = false;
        for (const auto& m : result.mentions) {
            if (m.userId == pm.userId) { already = true; break; }
        }
        if (!already) result.mentions.push_back(pm);
    }

    return result;
}

std::vector<Mention> extractAtMentions(const std::string& text) {
    std::vector<Mention> mentions;
    // Match @user:server (with domain required)
    std::regex atRe(R"(@[A-Za-z0-9._=\-/]+:[A-Za-z0-9.\-]+(?::\d+)?)");
    for (auto it = std::sregex_iterator(text.begin(), text.end(), atRe);
         it != std::sregex_iterator(); ++it) {
        Mention m;
        m.userId = it->str();
        m.startPos = it->position();
        m.endPos = m.startPos + m.userId.size();
        mentions.push_back(m);
    }
    return mentions;
}

bool containsMention(const std::string& text, const std::string& userId) {
    auto mentions = extractAtMentions(text);
    for (const auto& m : mentions) {
        if (m.userId == userId) return true;
    }
    return false;
}

std::string buildUserPill(const std::string& userId, const std::string& displayName) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "&quot;"; else if (c == '<') out += "&lt;";
            else if (c == '>') out += "&gt;"; else out += c; }
        return out;
    };
    return "<a href=\"https://matrix.to/#/" + esc(userId) + "\">" + esc(displayName) + "</a>";
}

std::string buildRoomPill(const std::string& roomId) {
    return "<a href=\"https://matrix.to/#/" + roomId + "\">" + roomId + "</a>";
}

std::string buildEventPill(const std::string& eventId, const std::string& roomId) {
    return "<a href=\"https://matrix.to/#/" + roomId + "/" + eventId + "\">" + eventId + "</a>";
}

bool isMatrixPill(const std::string& html) {
    return html.find("<a href=\"https://matrix.to/#/") != std::string::npos;
}

std::string stripPills(const std::string& html) {
    std::regex pillRe(R"RE(<a\s+href="https://matrix\.to/#/[^"]+">([^<]+)</a>)RE");
    return std::regex_replace(html, pillRe, "$1");
}

std::string resolveMentionDisplayName(const std::string& userId,
    const std::vector<std::pair<std::string, std::string>>& knownUsers) {
    for (const auto& [id, name] : knownUsers) {
        if (id == userId) return name;
    }
    return userId; // fallback to MXID
}

} // namespace progressive
