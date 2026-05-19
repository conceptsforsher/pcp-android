#include "progressive/content_guard.hpp"
#include <sstream>
#include <unordered_set>
#include <algorithm>
#include <cctype>

namespace progressive {

// ==== Emoji Detection ====
// Unicode emoji ranges from https://unicode.org/emoji/charts/full-emoji-list.html

bool isEmojiCodePoint(int cp) {
    // Basic emoji ranges
    if (cp >= 0x1F600 && cp <= 0x1F64F) return true; // Emoticons
    if (cp >= 0x1F300 && cp <= 0x1F5FF) return true; // Misc Symbols & Pictographs
    if (cp >= 0x1F680 && cp <= 0x1F6FF) return true; // Transport & Map
    if (cp >= 0x1F700 && cp <= 0x1F77F) return true; // Alchemical Symbols
    if (cp >= 0x1F780 && cp <= 0x1F7FF) return true; // Geometric Shapes Extended
    if (cp >= 0x1F800 && cp <= 0x1F8FF) return true; // Supplemental Arrows-C
    if (cp >= 0x1F900 && cp <= 0x1F9FF) return true; // Supplemental Symbols & Pictographs
    if (cp >= 0x1FA00 && cp <= 0x1FA6F) return true; // Chess Symbols
    if (cp >= 0x1FA70 && cp <= 0x1FAFF) return true; // Symbols & Pictographs Extended-A
    if (cp >= 0x2600 && cp <= 0x26FF) return true;   // Misc symbols
    if (cp >= 0x2700 && cp <= 0x27BF) return true;   // Dingbats
    if (cp >= 0xFE00 && cp <= 0xFE0F) return true;   // Variation Selectors
    if (cp >= 0x1F000 && cp <= 0x1F02F) return true; // Mahjong Tiles
    if (cp >= 0x1F0A0 && cp <= 0x1F0FF) return true; // Playing Cards
    if (cp >= 0x1F100 && cp <= 0x1F1FF) return true; // Enclosed Alphanumeric Supplement
    if (cp >= 0x1F200 && cp <= 0x1F2FF) return true; // Enclosed Ideographic Supplement
    if (cp >= 0x1F600 && cp <= 0x1F64F) return true; // Emoticons (redundant, kept for clarity)
    if (cp >= 0x2300 && cp <= 0x23FF) return true;   // Misc Technical
    if (cp >= 0x2B50) {
        if (cp == 0x2B50 || cp == 0x2B55) return true; // Star symbols
    }
    if (cp >= 0x200D) {
        // Zero-width joiner for combined emoji (e.g., family, skin tones)
        if (cp == 0x200D) return true;
    }
    // Flags
    if (cp >= 0x1F1E6 && cp <= 0x1F1FF) return true;

    return false;
}

int countEmojis(const std::string& text) {
    int count = 0;
    size_t i = 0;
    while (i < text.size()) {
        unsigned char b = static_cast<unsigned char>(text[i]);
        int cp = -1;

        // UTF-8 decoding
        if (b < 0x80) {
            cp = b;
            i += 1;
        } else if ((b & 0xE0) == 0xC0 && i + 1 < text.size()) {
            cp = ((b & 0x1F) << 6) | (text[i + 1] & 0x3F);
            i += 2;
        } else if ((b & 0xF0) == 0xE0 && i + 2 < text.size()) {
            cp = ((b & 0x0F) << 12) | ((text[i + 1] & 0x3F) << 6) | (text[i + 2] & 0x3F);
            i += 3;
        } else if ((b & 0xF8) == 0xF0 && i + 3 < text.size()) {
            cp = ((b & 0x07) << 18) | ((text[i + 1] & 0x3F) << 12) |
                 ((text[i + 2] & 0x3F) << 6) | (text[i + 3] & 0x3F);
            i += 4;
        } else {
            i++;
            continue;
        }

        if (cp >= 0 && isEmojiCodePoint(cp)) count++;
    }
    return count;
}

int countUniqueEmojis(const std::string& text) {
    std::unordered_set<int> seen;
    size_t i = 0;
    while (i < text.size()) {
        unsigned char b = static_cast<unsigned char>(text[i]);
        int cp = -1;

        if (b < 0x80) { cp = b; i += 1; }
        else if ((b & 0xE0) == 0xC0 && i + 1 < text.size()) {
            cp = ((b & 0x1F) << 6) | (text[i + 1] & 0x3F); i += 2;
        } else if ((b & 0xF0) == 0xE0 && i + 2 < text.size()) {
            cp = ((b & 0x0F) << 12) | ((text[i + 1] & 0x3F) << 6) | (text[i + 2] & 0x3F); i += 3;
        } else if ((b & 0xF8) == 0xF0 && i + 3 < text.size()) {
            cp = ((b & 0x07) << 18) | ((text[i + 1] & 0x3F) << 12) |
                 ((text[i + 2] & 0x3F) << 6) | (text[i + 3] & 0x3F); i += 4;
        } else { i++; continue; }

        if (cp >= 0 && isEmojiCodePoint(cp)) seen.insert(cp);
    }
    return static_cast<int>(seen.size());
}

EmojiCheck checkEmojiAttack(const std::string& text, int maxEmojis, int maxUniqueEmojis) {
    EmojiCheck result;
    result.totalEmojis = countEmojis(text);
    result.uniqueEmojis = countUniqueEmojis(text);

    if (maxEmojis > 0 && result.totalEmojis > maxEmojis) {
        result.isAttack = true;
        result.limitExceeded = maxEmojis;
        result.label = "(emoji attack)";
    } else if (maxUniqueEmojis > 0 && result.uniqueEmojis > maxUniqueEmojis) {
        result.isAttack = true;
        result.limitExceeded = maxUniqueEmojis;
        result.label = "(emoji attack)";
    }

    return result;
}

// ==== Media Collapse ====

MediaGroup checkMediaCollapse(
    const std::vector<std::string>& mediaTypes,
    int threshold)
{
    MediaGroup group;
    group.threshold = threshold;

    if (threshold <= 0 || mediaTypes.empty()) return group;

    for (const auto& type : mediaTypes) {
        if (type == "image") group.imageCount++;
        else if (type == "video") group.videoCount++;
        else if (type == "file") group.fileCount++;
    }

    group.totalMedia = static_cast<int>(mediaTypes.size());
    group.shouldCollapse = group.totalMedia >= threshold;

    if (group.shouldCollapse) {
        group.label = formatMediaCollapseLabel(group.totalMedia);
    }

    return group;
}

std::vector<ContentMediaItem> groupMedia(
    const std::vector<std::string>& mediaTypes,
    int threshold)
{
    std::vector<ContentMediaItem> result;
    if (threshold <= 0) {
        for (const auto& t : mediaTypes) result.push_back({t, false, 0});
        return result;
    }

    // Count consecutive media items
    int consecutiveMedia = 0;
    for (size_t i = 0; i < mediaTypes.size(); ++i) {
        bool isMedia = (mediaTypes[i] == "image" || mediaTypes[i] == "video" || mediaTypes[i] == "file");
        if (isMedia) {
            consecutiveMedia++;
        } else {
            // Flush pending media count
            if (consecutiveMedia >= threshold) {
                result.push_back({"image", true, consecutiveMedia});
            } else if (consecutiveMedia > 0) {
                for (int j = 0; j < consecutiveMedia; ++j)
                    result.push_back({"image", false, 0}); // simplified
            }
            consecutiveMedia = 0;
            result.push_back({mediaTypes[i], false, 0});
        }
    }

    // Flush remaining
    if (consecutiveMedia >= threshold) {
        result.push_back({"image", true, consecutiveMedia});
    } else if (consecutiveMedia > 0) {
        for (int j = 0; j < consecutiveMedia; ++j)
            result.push_back({"image", false, 0});
    }

    return result;
}

std::string formatMediaCollapseLabel(int count) {
    if (count <= 0) return "";
    return std::to_string(count) + " media omitted";
}

// ==================== Content Trust & Guard Policies ====================

// Original Kotlin: evaluateContent — run all validation rules
ContentGuardResult evaluateContent(const std::string& contentBody,
                                   const std::string& senderId,
                                   const std::string& mxcUrl,
                                   const ContentGuardConfig& config) {
    ContentGuardResult result;

    // Check explicit block list first
    for (const auto& blocked : config.explicitBlocked) {
        if (!mxcUrl.empty() && mxcUrl == blocked) {
            result.decision = ContentTrustDecision::BLOCK;
            result.reason = "Content URL is in block list.";
            result.violatedRules.push_back("explicit_block_list");
            return result;
        }
        if (!senderId.empty() && senderId == blocked) {
            result.decision = ContentTrustDecision::BLOCK;
            result.reason = "Sender is in block list.";
            result.violatedRules.push_back("sender_block_list");
            return result;
        }
    }

    // Check explicit allow list
    for (const auto& allowed : config.explicitAllowed) {
        if (!mxcUrl.empty() && mxcUrl == allowed) {
            result.decision = ContentTrustDecision::ALLOW;
            result.reason = "Content URL is in allow list.";
            return result;
        }
    }

    // Apply trust policy
    switch (config.policy) {
        case ContentTrustPolicy::TRUST_ALL:
            result.decision = ContentTrustDecision::ALLOW;
            result.reason = "Trust-all policy in effect.";
            break;

        case ContentTrustPolicy::BLOCK_ALL:
            result.decision = ContentTrustDecision::BLOCK;
            result.reason = "Block-all policy in effect.";
            break;

        case ContentTrustPolicy::SCAN_REQUIRED:
            if (!config.scanEnabled) {
                result.decision = ContentTrustDecision::SCAN_PENDING;
                result.reason = "Scan is required but not enabled.";
                result.scanRequired = true;
            } else {
                result.decision = ContentTrustDecision::SCAN_PENDING;
                result.reason = "Scan required before content can be accessed.";
                result.scanRequired = true;
            }
            break;

        case ContentTrustPolicy::MODERATION_REQUIRED:
            if (!config.moderationEnabled) {
                result.decision = ContentTrustDecision::QUARANTINE;
                result.reason = "Moderation is required but not enabled — content quarantined.";
            } else {
                result.decision = ContentTrustDecision::QUARANTINE;
                result.reason = "Awaiting moderation review.";
            }
            break;
    }

    // Run validation rules
    for (const auto& rule : config.rules) {
        if (!rule.enabled) continue;

        bool matched = false;
        auto lowerContent = [](const std::string& s) -> std::string {
            std::string r = s;
            std::transform(r.begin(), r.end(), r.begin(), ::tolower);
            return r;
        };

        if (rule.ruleType == "keyword") {
            auto lower = lowerContent(contentBody);
            auto condLower = lowerContent(rule.condition);
            matched = lower.find(condLower) != std::string::npos;
        } else if (rule.ruleType == "sender") {
            matched = (senderId == rule.condition);
        } else if (rule.ruleType == "domain") {
            matched = mxcUrl.find(rule.condition) != std::string::npos;
        }

        if (matched) {
            result.violatedRules.push_back(rule.ruleId);

            switch (rule.action) {
                case ContentRuleAction::BLOCK:
                    result.decision = ContentTrustDecision::BLOCK;
                    result.reason = "Blocked by rule: " + rule.description;
                    break;
                case ContentRuleAction::WARN:
                    if (result.decision == ContentTrustDecision::ALLOW) {
                        result.warningType = ContentWarningType::CUSTOM;
                    }
                    break;
                case ContentRuleAction::QUARANTINE:
                    result.decision = ContentTrustDecision::QUARANTINE;
                    result.reason = "Quarantined by rule: " + rule.description;
                    break;
                case ContentRuleAction::REPORT:
                    // Report does not change decision but flags content
                    break;
                default:
                    break;
            }
        }
    }

    return result;
}

// Original Kotlin: isContentBlocked
bool isContentBlocked(const ContentGuardResult& result) {
    return result.decision == ContentTrustDecision::BLOCK;
}

// Original Kotlin: isContentAllowed
bool isContentAllowed(const ContentGuardResult& result) {
    return result.decision == ContentTrustDecision::ALLOW;
}

// Original Kotlin: shouldPromptUser
bool shouldPromptUser(const ContentGuardResult& result) {
    switch (result.decision) {
        case ContentTrustDecision::BLOCK:
        case ContentTrustDecision::QUARANTINE:
        case ContentTrustDecision::SCAN_PENDING:
            return true;
        default:
            return false;
    }
}

// Original Kotlin: getWarningMessage
std::string getWarningMessage(ContentWarningType type, const std::string& customText) {
    switch (type) {
        case ContentWarningType::EXPLICIT:
            return "This content may contain explicit material. View at your discretion.";
        case ContentWarningType::VIOLENCE:
            return "This content may contain depictions of violence.";
        case ContentWarningType::SPAM:
            return "This content has been flagged as potential spam.";
        case ContentWarningType::PHISHING:
            return "Warning: This content may be attempting to phish your information.";
        case ContentWarningType::NSFW:
            return "This content is not safe for work. Proceed with caution.";
        case ContentWarningType::CUSTOM:
            return customText.empty()
                ? "This content has been flagged. Proceed with caution."
                : customText;
        default:
            return "Content warning: proceed with caution.";
    }
}

// Original Kotlin: addToBlockList
void addToBlockList(ContentGuardConfig& config, const std::string& pattern) {
    config.explicitBlocked.insert(pattern);
}

// Original Kotlin: removeFromBlockList
void removeFromBlockList(ContentGuardConfig& config, const std::string& pattern) {
    config.explicitBlocked.erase(pattern);
}

} // namespace progressive
