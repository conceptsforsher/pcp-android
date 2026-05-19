#pragma once
#ifndef PROGRESSIVE_CONTENT_FILTER_HPP
#define PROGRESSIVE_CONTENT_FILTER_HPP

#include <string>
#include <string_view>
#include <vector>
#include <unordered_set>
#include <cstdint>
#include <functional>

namespace progressive {

// ---- Forbidden Keywords Filter ----

class KeywordFilter {
public:
    // Load keywords from a comma- or newline-separated string.
    void loadKeywords(const std::string& raw);

    // Check if text contains any forbidden keyword (case-insensitive).
    // Returns the first matched keyword, or empty string if clean.
    std::string check(const std::string& text) const;

    // Add a single keyword.
    void addKeyword(const std::string& keyword);

    // Remove a single keyword.
    void removeKeyword(const std::string& keyword);

    // Export all keywords as a comma-separated string.
    std::string exportKeywords() const;

    // Clear all keywords.
    void clear();

    size_t count() const { return keywords_.size(); }

private:
    std::vector<std::string> keywords_;
    static std::string toLower(const std::string& s);
};

// ---- Image Security Policy ----

struct ImagePolicy {
    bool blockAllRemote = false;   // block ALL images from internet
    bool allowAvatars = true;      // exception: allow avatars even if blockAllRemote
    bool allowStickers = false;    // exception: allow stickers
    bool allowEmoji = true;        // exception: allow emoji

    // Check if an image URL should be blocked under this policy.
    // Returns true if the image should be blocked.
    bool shouldBlock(const std::string& mxcUrl, const std::string& imageType) const;
    // imageType: "avatar", "sticker", "emoji", "attachment", "preview"
};

// ---- Media Sending Policy ----

struct MediaSendPolicy {
    bool sendOriginalSize = true;      // don't compress
    bool skipPreviewGeneration = false; // don't generate thumbnail for RAM saving
};

// ==================== Content Filtering Engine ====================

// Original Kotlin: enum class ContentFilterType
enum class ContentFilterType : int {
    KEYWORD = 0,
    REGEX = 1,
    URL = 2,
    DOMAIN = 3,
    FILE_TYPE = 4,
    SENDER = 5
};

// Original Kotlin: enum class ContentReplaceStrategy
enum class ContentReplaceStrategy : int {
    REDACT = 0,        // replace with [REDACTED]
    REPLACE_TEXT = 1,  // replace with custom text
    BLUR = 2,          // mark as blurred
    HIDE = 3           // remove entirely
};

// Original Kotlin: data class ContentFilterRule
struct ContentFilterRule {
    std::string pattern;
    bool isRegex = false;
    bool caseSensitive = true;
    bool matchWholeWord = false;
    std::string appliesTo;       // "BODY", "FILENAME", "SENDER", "URL"
    ContentFilterType filterType = ContentFilterType::KEYWORD;
    ContentReplaceStrategy replaceStrategy = ContentReplaceStrategy::REDACT;
    std::string replacementText; // used with REPLACE_TEXT strategy
    bool enabled = true;
};

// Original Kotlin: data class FilterMatch
struct FilterMatch {
    ContentFilterRule rule;
    size_t matchStart = 0;
    size_t matchEnd = 0;
    std::string matchText;
    std::string replacementText;
    int action = 0;              // ContentRuleAction ordinal (see content_guard.hpp)
};

// Original Kotlin: data class ContentFilterResult
struct ContentFilterResult {
    bool matched = false;
    std::vector<ContentFilterRule> matchedRules;
    std::string filteredContent;
    std::vector<FilterMatch> allMatches;
};

// Original Kotlin: data class ContentCleanupResult
struct ContentCleanupResult {
    std::string cleanText;
    int matchesApplied = 0;
    bool contentChanged = false;
};

// Original Kotlin: data class ContentFilterConfig
struct ContentFilterConfig {
    std::vector<ContentFilterRule> rules;
    bool enabled = true;
    bool appliesToSent = true;       // filter outgoing messages
    bool appliesToReceived = true;   // filter incoming messages
};

// Original Kotlin: applyContentFilters — run all filters against content
ContentFilterResult applyContentFilters(const std::string& content,
                                         const std::string& senderId,
                                         const std::string& mxcUrl,
                                         const ContentFilterConfig& config);

// Original Kotlin: createFilterRule — build a rule from parameters
ContentFilterRule createFilterRule(const std::string& pattern,
                                    ContentFilterType type,
                                    const std::string& appliesTo,
                                    bool caseSensitive = true,
                                    bool isRegex = false);

// Original Kotlin: validateFilterPattern — test if a regex/pattern is valid
bool validateFilterPattern(const std::string& pattern, bool isRegex);

// Original Kotlin: optimizeFilterRules — merge/optimize duplicate or overlapping rules
void optimizeFilterRules(std::vector<ContentFilterRule>& rules);

// Original Kotlin: applyReplacement — apply replacement strategy to matched text
std::string applyReplacement(const std::string& text,
                             const FilterMatch& match,
                             ContentReplaceStrategy strategy,
                             const std::string& customReplaceText = "");

// Original Kotlin: findAllFilterMatches — find all matches of a rule in text
std::vector<FilterMatch> findAllFilterMatches(const std::string& text,
                                               const ContentFilterRule& rule);

} // namespace progressive

#endif // PROGRESSIVE_CONTENT_FILTER_HPP
