#ifndef PROGRESSIVE_PUSH_CONDITION_HPP
#define PROGRESSIVE_PUSH_CONDITION_HPP

#include <string>
#include <vector>

namespace progressive {

// ---- Push Rule Condition Evaluator ----
// Faithful port from original Kotlin:
//   org.matrix.android.sdk.api.session.pushrules.EventMatchCondition.kt (105 lines)
//   org.matrix.android.sdk.internal.util.Glob.kt (39 lines)
//
// Evaluates whether a Matrix event matches a push notification rule condition.
// Conditions use glob-style patterns matched against dot-separated event fields.

// Convert a glob pattern to a regular expression.
// Original Kotlin (Glob.kt:simpleGlobToRegExp):
//   * → .*   ? → .   . → \\.   \\ → \\\\
std::string globToRegex(const std::string& glob);

// Check if a glob pattern contains special characters (* or ?).
// Original: contains("*") || contains("?")
bool hasSpecialGlobChar(const std::string& pattern);

// Evaluate a push rule condition against an event.
// Original Kotlin (EventMatchCondition.kt:isSatisfied):
//   - Extracts field via dot-separated key (e.g., "content.body")
//   - For "content.body": matches any word-boundary substring
//   - For other fields: matches the entire value
//   - Matching is case-insensitive
//
// @param eventJson  The full event JSON
// @param key        Dot-separated field path, e.g. "content.body"
// @param pattern    Glob-style pattern, e.g. "*hello*"
// @return true if the condition matches
bool evaluateEventMatchCondition(
    const std::string& eventJson,
    const std::string& key,
    const std::string& pattern
);

// Extract a value from a JSON object using a dot-separated path.
// "content.body" → event["content"]["body"]
// Original Kotlin (EventMatchCondition.kt:extractField)
std::string extractJsonField(const std::string& json, const std::string& fieldPath);

// Check if a string matches a regex pattern (case-insensitive).
// Simple implementation without external regex library.
bool simpleRegexMatch(const std::string& text, const std::string& regexPattern);

// Check if a regex pattern matches ANYWHERE in the text (contains match).
bool simpleRegexContainsMatch(const std::string& text, const std::string& regexPattern);

// Structure representing a push rule condition.
struct EventPushCondition {
    std::string kind;        // "event_match", "room_member_count", "sender_notification_permission"
    std::string key;         // for event_match: "content.body", "sender", "room_id", "type"
    std::string pattern;     // glob pattern
    bool isSatisfied = false;
};

// Evaluate a generic push condition against event JSON.
EventPushCondition evaluateEventPushCondition(
    const EventPushCondition& condition,
    const std::string& eventJson
);

// Format push condition as JSON for Kotlin UI.
std::string pushConditionToJson(const EventPushCondition& condition);

// ---- Push Rule & Rule Set (from EventPushRule.kt 142L + RuleSet.kt 75L) ----
// Models for Matrix push rules as defined in the spec:
//   https://matrix.org/docs/spec/client_server/latest#push-rules

struct EventPushRule {
    std::string ruleId;
    bool enabled = true;
    bool isDefault = false;
    std::string pattern;                     // glob pattern for content rules
    std::vector<std::string> actions;        // "notify", "dont_notify", "coalesce", "highlight"
    std::vector<EventPushCondition> conditions;   // for override/sender/underride rules
    bool shouldHighlight = false;            // action contains "highlight"
    bool shouldNotify = true;                // action contains "notify" (not "dont_notify")
    std::string notificationSound;           // "default" or custom sound name
};

// Parse a push rule from JSON.
EventPushRule parseEventPushRule(const std::string& json);

// Set notification sound on a push rule.
// Original: fun setNotificationSound(sound: String): EventPushRule
EventPushRule setEventPushRuleSound(const EventPushRule& rule, const std::string& sound);

// Set highlight on a push rule.
// Original: fun setHighlight(highlight: Boolean): EventPushRule
EventPushRule setEventPushRuleHighlight(const EventPushRule& rule, bool highlight);

// Set notify/dont_notify on a push rule.
// Original: fun setNotify(notify: Boolean): EventPushRule
EventPushRule setEventPushRuleNotify(const EventPushRule& rule, bool notify);

// Check if rule should notify.
// Original: fun shouldNotify() = actions.contains(ACTION_NOTIFY)
inline bool pushRuleShouldNotify(const EventPushRule& rule) { return rule.shouldNotify; }

// Check if rule should not notify (empty actions or contains dont_notify).
inline bool pushRuleShouldNotNotify(const EventPushRule& rule) { return !rule.shouldNotify; }

// Format push rule as JSON.
std::string pushRuleToJson(const EventPushRule& rule);

// ---- Contains Display Name Condition ----
// Faithful port from org.matrix.android.sdk.api.session.pushrules.ContainsDisplayNameCondition.kt (47L)
//
// Checks if the user's display name appears in the message body.
// Used by the default "contains_display_name" push rule for @mentions.
// Matching is case-insensitive and matches at word boundaries.

// Check if displayName is found in text (case-insensitive).
// Original Kotlin: message.body.caseInsensitiveFind(displayName)
bool caseInsensitiveFind(const std::string& text, const std::string& search);

// Check if a message mentions a user by display name.
// Reads content.body from event JSON, checks case-insensitive match.
bool evaluateDisplayNameCondition(const std::string& eventJson, const std::string& displayName);

} // namespace progressive

#endif // PROGRESSIVE_PUSH_CONDITION_HPP
