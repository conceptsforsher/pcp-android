#pragma once
#ifndef PROGRESSIVE_CONTENT_GUARD_HPP
#define PROGRESSIVE_CONTENT_GUARD_HPP

#include <string>
#include <string_view>
#include <vector>
#include <unordered_set>
#include <cstdint>
#include <functional>

namespace progressive {

// ---- Content Guard: Emoji Attack + Media Collapse Protection ----
// Protects against client slowdown/crash from malicious or accidental
// content flooding.

// ==== Emoji Attack Protection ====
// Detects when a message contains an excessive number of emoji that
// could cause rendering performance issues or crashes.

struct EmojiCheck {
    int totalEmojis = 0;         // total emoji count in the message
    int uniqueEmojis = 0;        // how many different emoji types
    bool isAttack = false;       // exceeds threshold
    int limitExceeded = 0;       // max configured limit
    std::string label;           // "(emoji attack)" or empty
};

// Count emoji characters in a string.
// Detects Unicode emoji ranges: flags, symbols, emoticons, etc.
int countEmojis(const std::string& text);

// Count unique emoji types in a string.
int countUniqueEmojis(const std::string& text);

// Check if a message constitutes an emoji attack.
// @param text  The message body
// @param maxEmojis  Max allowed emoji before flagging (0 = disabled)
// @param maxUniqueEmojis  Max unique emoji types (0 = disabled)
EmojiCheck checkEmojiAttack(const std::string& text, int maxEmojis = 50, int maxUniqueEmojis = 20);

// Get the emoji attack warning label.
inline std::string getEmojiAttackLabel() { return "(emoji attack)"; }

// ==== Media Collage / Grouping ====
// Collapses consecutive images into a clickable "N media omitted" label
// when the count exceeds the configured threshold.

struct MediaGroup {
    int imageCount = 0;          // total consecutive images
    int videoCount = 0;          // total consecutive videos
    int fileCount = 0;           // total consecutive files
    int totalMedia = 0;          // total media items
    bool shouldCollapse = false; // exceeds threshold
    int threshold = 0;           // configured threshold
    std::string label;           // "5 media omitted" or empty
};

// Check if consecutive media items should be collapsed.
// @param mediaTypes  Ordered list of media types ("image", "video", "file")
// @param threshold  Collapse if >= this many consecutive items (0 = never)
MediaGroup checkMediaCollapse(
    const std::vector<std::string>& mediaTypes,
    int threshold = 10
);

// Group consecutive media types and return collapse instructions.
// Returns a list where consecutive media of the same type beyond threshold
// are replaced with a collapse marker.
struct ContentMediaItem {
    std::string mediaType;       // "image", "video", "file", "text"
    bool isCollapseMarker = false; // this is a placeholder
    int collapseCount = 0;       // how many items are collapsed
};

std::vector<ContentMediaItem> groupMedia(
    const std::vector<std::string>& mediaTypes,
    int threshold = 10
);

// Format the media collapse label.
std::string formatMediaCollapseLabel(int count);

// Check if a Unicode codepoint is an emoji.
bool isEmojiCodePoint(int codepoint);

// ==================== Content Trust & Guard Policies ====================

// Original Kotlin: enum class ContentTrustPolicy
enum class ContentTrustPolicy : int {
    TRUST_ALL = 0,
    SCAN_REQUIRED = 1,
    MODERATION_REQUIRED = 2,
    BLOCK_ALL = 3
};

// Original Kotlin: enum class ContentTrustDecision
enum class ContentTrustDecision : int {
    ALLOW = 0,
    BLOCK = 1,
    QUARANTINE = 2,
    SCAN_PENDING = 3
};

// Original Kotlin: enum class ContentWarningType
enum class ContentWarningType : int {
    EXPLICIT = 0,
    VIOLENCE = 1,
    SPAM = 2,
    PHISHING = 3,
    NSFW = 4,
    CUSTOM = 5
};

// Original Kotlin: enum class ContentRuleAction
enum class ContentRuleAction : int {
    BLOCK = 0,
    WARN = 1,
    ALLOW = 2,
    QUARANTINE = 3,
    REPORT = 4
};

// Original Kotlin: data class ContentValidationRule
struct ContentValidationRule {
    std::string ruleId;
    std::string ruleType;       // "keyword", "regex", "domain", "sender", "metadata"
    std::string condition;      // the matching condition/expression
    std::string description;    // human-readable rule description
    ContentRuleAction action = ContentRuleAction::BLOCK;
    bool enabled = true;
};

// Original Kotlin: data class ContentGuardConfig
struct ContentGuardConfig {
    ContentTrustPolicy policy = ContentTrustPolicy::TRUST_ALL;
    std::vector<ContentValidationRule> rules;
    bool scanEnabled = false;
    bool moderationEnabled = false;
    std::unordered_set<std::string> explicitAllowed;  // mxc URLs or patterns
    std::unordered_set<std::string> explicitBlocked;
};

// Original Kotlin: data class ContentGuardResult
struct ContentGuardResult {
    ContentTrustDecision decision = ContentTrustDecision::ALLOW;
    std::string reason;
    std::vector<std::string> violatedRules;  // rule IDs that were triggered
    bool scanRequired = false;
    ContentWarningType warningType = ContentWarningType::CUSTOM;
};

// Original Kotlin: evaluateContent — run all rules against content
ContentGuardResult evaluateContent(const std::string& contentBody,
                                   const std::string& senderId,
                                   const std::string& mxcUrl,
                                   const ContentGuardConfig& config);

// Original Kotlin: isContentBlocked — quick check
bool isContentBlocked(const ContentGuardResult& result);

// Original Kotlin: isContentAllowed — quick check
bool isContentAllowed(const ContentGuardResult& result);

// Original Kotlin: shouldPromptUser — show warning before viewing
bool shouldPromptUser(const ContentGuardResult& result);

// Original Kotlin: getWarningMessage — localized warning text
std::string getWarningMessage(ContentWarningType type, const std::string& customText = "");

// Original Kotlin: addToBlockList — manage explicit block list
void addToBlockList(ContentGuardConfig& config, const std::string& pattern);

// Original Kotlin: removeFromBlockList — remove from block list
void removeFromBlockList(ContentGuardConfig& config, const std::string& pattern);

} // namespace progressive

#endif // PROGRESSIVE_CONTENT_GUARD_HPP
