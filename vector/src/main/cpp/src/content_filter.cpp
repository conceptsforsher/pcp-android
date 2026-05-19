#include "progressive/content_filter.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

namespace progressive {

// ---- KeywordFilter ----

std::string KeywordFilter::toLower(const std::string& s) {
    std::string r = s;
    std::transform(r.begin(), r.end(), r.begin(), ::tolower);
    return r;
}

void KeywordFilter::loadKeywords(const std::string& raw) {
    keywords_.clear();
    std::istringstream stream(raw);
    std::string token;
    // Split by comma or newline
    while (std::getline(stream, token, ',')) {
        // Also split by newline within token
        std::istringstream lineStream(token);
        std::string lineToken;
        while (std::getline(lineStream, lineToken, '\n')) {
            auto trimmed = lineToken;
            // Trim whitespace
            while (!trimmed.empty() && (trimmed.front() == ' ' || trimmed.front() == '\t' || trimmed.front() == '\r'))
                trimmed.erase(0, 1);
            while (!trimmed.empty() && (trimmed.back() == ' ' || trimmed.back() == '\t' || trimmed.back() == '\r'))
                trimmed.pop_back();
            if (!trimmed.empty()) {
                keywords_.push_back(toLower(trimmed));
            }
        }
    }
}

std::string KeywordFilter::check(const std::string& text) const {
    if (text.empty()) return {};
    auto lower = toLower(text);
    for (const auto& kw : keywords_) {
        if (lower.find(kw) != std::string::npos) {
            return kw;
        }
    }
    return {};
}

void KeywordFilter::addKeyword(const std::string& keyword) {
    auto kw = toLower(keyword);
    // Check not already present
    for (const auto& existing : keywords_) {
        if (existing == kw) return;
    }
    keywords_.push_back(kw);
}

void KeywordFilter::removeKeyword(const std::string& keyword) {
    auto kw = toLower(keyword);
    keywords_.erase(std::remove(keywords_.begin(), keywords_.end(), kw), keywords_.end());
}

std::string KeywordFilter::exportKeywords() const {
    std::ostringstream oss;
    for (size_t i = 0; i < keywords_.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << keywords_[i];
    }
    return oss.str();
}

void KeywordFilter::clear() {
    keywords_.clear();
}

// ---- ImagePolicy ----

bool ImagePolicy::shouldBlock(const std::string& mxcUrl, const std::string& imageType) const {
    if (!blockAllRemote) return false;

    // Local-only images are never blocked
    if (mxcUrl.empty()) return false;

    // Check exceptions
    if (imageType == "avatar" && allowAvatars) return false;
    if (imageType == "sticker" && allowStickers) return false;
    if (imageType == "emoji" && allowEmoji) return false;

    // Block everything else from internet
    return true;
}

} // namespace progressive
