#include "progressive/content_filter.hpp"
#include "progressive/content_guard.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <regex>

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

// ==================== Content Filtering Engine ====================

// Original Kotlin: createFilterRule
ContentFilterRule createFilterRule(const std::string& pattern,
                                    ContentFilterType type,
                                    const std::string& appliesTo,
                                    bool caseSensitive,
                                    bool isRegex) {
    ContentFilterRule rule;
    rule.pattern = pattern;
    rule.filterType = type;
    rule.appliesTo = appliesTo;
    rule.caseSensitive = caseSensitive;
    rule.isRegex = isRegex;
    rule.matchWholeWord = false;
    rule.replaceStrategy = ContentReplaceStrategy::REDACT;
    return rule;
}

// Original Kotlin: validateFilterPattern
bool validateFilterPattern(const std::string& pattern, bool isRegex) {
    if (pattern.empty()) return false;

    if (isRegex) {
        try {
            std::regex re(pattern);
            (void)re; // suppress unused warning
            return true;
        } catch (const std::regex_error&) {
            return false;
        }
    }

    return true; // plain text patterns are always valid
}

// Original Kotlin: optimizeFilterRules — deduplicate and sort
void optimizeFilterRules(std::vector<ContentFilterRule>& rules) {
    if (rules.size() < 2) return;

    // Remove exact duplicates by (pattern, appliesTo, filterType)
    std::vector<ContentFilterRule> optimized;
    for (auto& rule : rules) {
        bool duplicate = false;
        for (const auto& existing : optimized) {
            if (existing.pattern == rule.pattern &&
                existing.appliesTo == rule.appliesTo &&
                existing.filterType == rule.filterType) {
                duplicate = true;
                break;
            }
        }
        if (!duplicate) {
            optimized.push_back(std::move(rule));
        }
    }

    // Sort by filter type for predictable behavior
    std::sort(optimized.begin(), optimized.end(),
              [](const ContentFilterRule& a, const ContentFilterRule& b) {
                  return static_cast<int>(a.filterType) < static_cast<int>(b.filterType);
              });

    rules = std::move(optimized);
}

// Original Kotlin: findAllFilterMatches
std::vector<FilterMatch> findAllFilterMatches(const std::string& text,
                                               const ContentFilterRule& rule) {
    std::vector<FilterMatch> matches;
    if (text.empty() || rule.pattern.empty()) return matches;

    auto lower = [](const std::string& s) -> std::string {
        std::string r = s;
        std::transform(r.begin(), r.end(), r.begin(), ::tolower);
        return r;
    };

    if (rule.isRegex) {
        try {
            std::regex re(rule.pattern,
                          rule.caseSensitive ? std::regex::ECMAScript
                                             : std::regex::ECMAScript | std::regex::icase);
            std::sregex_iterator it(text.begin(), text.end(), re);
            std::sregex_iterator end;
            for (; it != end; ++it) {
                FilterMatch match;
                match.rule = rule;
                match.matchStart = static_cast<size_t>(it->position());
                match.matchEnd = match.matchStart + it->length();
                match.matchText = it->str();
                match.replacementText = rule.replacementText;
                match.action = static_cast<int>(ContentRuleAction::BLOCK);
                matches.push_back(match);
            }
        } catch (const std::regex_error&) {
            // Invalid regex — no matches
        }
    } else {
        // Plain text search
        std::string searchIn = rule.caseSensitive ? text : lower(text);
        std::string searchFor = rule.caseSensitive ? rule.pattern : lower(rule.pattern);

        size_t pos = 0;
        while ((pos = searchIn.find(searchFor, pos)) != std::string::npos) {
            // Check whole-word match if enabled
            if (rule.matchWholeWord) {
                bool atStart = (pos == 0);
                bool atEnd = (pos + searchFor.size() >= searchIn.size());
                bool prevIsBoundary = atStart || !std::isalnum(static_cast<unsigned char>(searchIn[pos - 1]));
                bool nextIsBoundary = atEnd || !std::isalnum(static_cast<unsigned char>(searchIn[pos + searchFor.size()]));
                if (!prevIsBoundary || !nextIsBoundary) {
                    pos += searchFor.size();
                    continue;
                }
            }

            FilterMatch match;
            match.rule = rule;
            match.matchStart = pos;
            match.matchEnd = pos + searchFor.size();
            match.matchText = text.substr(pos, searchFor.size());
            match.replacementText = rule.replacementText;
            match.action = static_cast<int>(ContentRuleAction::BLOCK);
            matches.push_back(match);
            pos += searchFor.size();
        }
    }

    return matches;
}

// Original Kotlin: applyReplacement
std::string applyReplacement(const std::string& text,
                             const FilterMatch& match,
                             ContentReplaceStrategy strategy,
                             const std::string& customReplaceText) {
    switch (strategy) {
        case ContentReplaceStrategy::REDACT: {
            std::string redacted(text.size(), ' ');
            redacted.replace(match.matchStart,
                             match.matchEnd - match.matchStart,
                             "[REDACTED]");
            // Copy remaining content
            std::string result = text;
            result.replace(match.matchStart,
                          match.matchEnd - match.matchStart,
                          "[REDACTED]");
            return result;
        }
        case ContentReplaceStrategy::REPLACE_TEXT: {
            std::string result = text;
            result.replace(match.matchStart,
                          match.matchEnd - match.matchStart,
                          customReplaceText.empty()
                              ? match.replacementText
                              : customReplaceText);
            return result;
        }
        case ContentReplaceStrategy::BLUR: {
            std::string result = text;
            result.replace(match.matchStart,
                          match.matchEnd - match.matchStart,
                          "***");
            return result;
        }
        case ContentReplaceStrategy::HIDE: {
            std::string result = text;
            result.erase(match.matchStart,
                        match.matchEnd - match.matchStart);
            return result;
        }
    }
    return text;
}

// Original Kotlin: applyContentFilters
ContentFilterResult applyContentFilters(const std::string& content,
                                         const std::string& senderId,
                                         const std::string& mxcUrl,
                                         const ContentFilterConfig& config) {
    ContentFilterResult result;
    if (!config.enabled || content.empty()) {
        result.filteredContent = content;
        return result;
    }

    std::string workingContent = content;

    for (const auto& rule : config.rules) {
        if (!rule.enabled) continue;

        // Check if rule applies to the current context
        if (rule.appliesTo == "SENDER" && senderId.empty()) continue;
        if (rule.appliesTo == "URL" && mxcUrl.empty()) continue;

        // Determine the text to scan based on appliesTo
        std::string scanTarget;
        if (rule.appliesTo == "SENDER") {
            scanTarget = senderId;
        } else if (rule.appliesTo == "URL") {
            scanTarget = mxcUrl;
        } else {
            // "BODY", "FILENAME" — scan content body
            scanTarget = workingContent;
        }

        auto matches = findAllFilterMatches(scanTarget, rule);
        if (!matches.empty()) {
            result.matched = true;
            result.matchedRules.push_back(rule);
            for (auto& m : matches) {
                result.allMatches.push_back(m);
            }
            // Apply replacements to workingContent for BODY/FILENAME rules
            if (rule.appliesTo == "BODY" || rule.appliesTo == "FILENAME") {
                for (const auto& m : matches) {
                    workingContent = applyReplacement(workingContent, m, rule.replaceStrategy, rule.replacementText);
                }
            }
        }
    }

    result.filteredContent = workingContent;
    return result;
}

} // namespace progressive
