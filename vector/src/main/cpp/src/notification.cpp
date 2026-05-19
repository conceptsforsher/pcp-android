#include "progressive/notification.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

namespace progressive {

// ---- NotificationKeywords ----

std::string NotificationKeywords::toLower(const std::string& s) {
    std::string r = s;
    std::transform(r.begin(), r.end(), r.begin(), ::tolower);
    return r;
}

void NotificationKeywords::addKeyword(const std::string& keyword, bool caseSensitive) {
    // Remove existing
    auto lower = toLower(keyword);
    keywords_.erase(std::remove_if(keywords_.begin(), keywords_.end(),
        [&](const NotificationKeyword& k) { return toLower(k.keyword) == lower; }
    ), keywords_.end());
    keywords_.push_back({keyword, true, caseSensitive});
}

void NotificationKeywords::removeKeyword(const std::string& keyword) {
    auto lower = toLower(keyword);
    keywords_.erase(std::remove_if(keywords_.begin(), keywords_.end(),
        [&](const NotificationKeyword& k) { return toLower(k.keyword) == lower; }
    ), keywords_.end());
}

void NotificationKeywords::setEnabled(const std::string& keyword, bool enabled) {
    auto lower = toLower(keyword);
    for (auto& k : keywords_) {
        if (toLower(k.keyword) == lower) {
            k.enabled = enabled;
            return;
        }
    }
}

std::string NotificationKeywords::check(const std::string& body) const {
    for (const auto& k : keywords_) {
        if (!k.enabled) continue;
        if (k.caseSensitive) {
            if (body.find(k.keyword) != std::string::npos) return k.keyword;
        } else {
            if (toLower(body).find(toLower(k.keyword)) != std::string::npos) return k.keyword;
        }
    }
    return {};
}

std::string NotificationKeywords::exportJson() const {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    std::ostringstream json;
    json << "[";
    for (size_t i = 0; i < keywords_.size(); ++i) {
        if (i > 0) json << ",";
        const auto& k = keywords_[i];
        json << R"({"keyword": ")" << esc(k.keyword) << R"(")";
        json << R"(,"enabled": )" << (k.enabled ? "true" : "false");
        json << R"(,"caseSensitive": )" << (k.caseSensitive ? "true" : "false") << "}";
    }
    json << "]";
    return json.str();
}

void NotificationKeywords::importJson(const std::string& json) {
    clear();
    size_t pos = 0;
    while (true) {
        pos = json.find('{', pos);
        if (pos == std::string::npos) break;

        int depth = 0;
        auto end = pos;
        while (end < json.size()) {
            if (json[end] == '{') ++depth;
            else if (json[end] == '}') --depth;
            if (depth == 0) break;
            ++end;
        }
        if (end >= json.size()) break;

        std::string obj = json.substr(pos, end - pos + 1);

        NotificationKeyword kw;
        // Extract keyword field
        auto kwSearch = std::string("\"keyword\": \"");
        auto kwPos = obj.find(kwSearch);
        if (kwPos != std::string::npos) {
            kwPos += kwSearch.size();
            auto kwEnd = obj.find('"', kwPos);
            if (kwEnd != std::string::npos) kw.keyword = obj.substr(kwPos, kwEnd - kwPos);
        }
        kw.enabled = obj.find("\"enabled\": true") != std::string::npos;
        kw.caseSensitive = obj.find("\"caseSensitive\": true") != std::string::npos;

        if (!kw.keyword.empty()) keywords_.push_back(kw);

        pos = end + 1;
    }
}

void NotificationKeywords::clear() {
    keywords_.clear();
}

// ---- Reaction Preview ----

std::string formatReactionPreview(const ReactionPreview& reaction) {
    std::ostringstream out;
    out << reaction.reactorName << " reacted: " << reaction.reactionEmoji;
    if (!reaction.sourceBody.empty()) {
        out << " to '" << truncateForReactionPreview(reaction.sourceBody) << "'";
    }
    return out.str();
}

std::string truncateForReactionPreview(const std::string& body, int maxLen) {
    if (static_cast<int>(body.size()) <= maxLen) return body;
    // Truncate at word boundary
    auto truncated = body.substr(0, maxLen);
    auto lastSpace = truncated.rfind(' ');
    if (lastSpace != std::string::npos && lastSpace > maxLen / 2) {
        truncated = truncated.substr(0, lastSpace);
    }
    return truncated + "...";
}

} // namespace progressive
