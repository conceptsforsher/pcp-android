#ifndef PROGRESSIVE_PUSH_RULES_HPP
#define PROGRESSIVE_PUSH_RULES_HPP

#include <string>
#include <vector>

namespace progressive {

// ---- Push Rule Condition Parser ----
// Replaces the SDK's limited condition parser that falls back to "UNSUPPORTED".
// Handles ALL known Matrix push rule condition types with human-readable descriptions.

struct PushCondition {
    std::string kind;              // "event_match", "contains_display_name", etc.
    std::string key;               // "content.body", "room_id", etc.
    std::string pattern;           // match pattern or value
    std::string description;       // human-readable: "Message contains 'hello'"
    bool isSupported = true;       // always true for our parser
};

struct PushRule {
    std::string ruleId;
    std::string kind;              // "override", "underride", "content", "room", "sender"
    bool enabled = true;
    std::vector<PushCondition> conditions;
    std::vector<std::string> actions; // "notify", "dont_notify", "coalesce"
    bool shouldNotify = true;
    bool shouldHighlight = false;
    std::string soundName;
};

struct PushRuleSet {
    std::vector<PushRule> rules;
    int totalRules = 0;
    int enabledRules = 0;
};

// Parse push rules from the /pushrules API response.
PushRuleSet parsePushRules(const std::string& apiResponseJson);

// Parse a single condition from JSON and produce a human-readable description.
PushCondition parsePushCondition(const std::string& conditionJson);

// Build a human-readable description for a condition.
std::string describePushCondition(const PushCondition& condition);

// Format all conditions of a rule as text for UI display.
std::string formatPushRuleConditions(const std::vector<PushCondition>& conditions);

// Format a push rule for UI item display.
std::string formatPushRuleItem(const PushRule& rule);

// Check if a push rule matches the standard types known to Matrix.
bool isKnownPushRuleKind(const std::string& kind);

// Get a localized description for a push rule kind.
std::string getRuleKindDescription(const std::string& kind, bool enabled);

// ---- MSC3061 Shared History Detection ----

// Check if a room key was shared under MSC3061 (world_readable or shared visibility).
bool isMsc3061SharedKey(const std::string& roomKeyContentJson);

// Format MSC3061 key sharing status for UI display.
std::string formatMsc3061Status(bool isShared, const std::string& visibilitySetting);

// Check room visibility setting for history sharing eligibility.
bool canShareHistory(const std::string& roomVisibility);

// ==== Push Rules Domain Models ====
//
// Original Kotlin: pushrules/RuleIds.kt, Kind.kt, RuleScope.kt, Action.kt

namespace RuleIds {
    constexpr const char* DISABLE_ALL = ".m.rule.master";
    constexpr const char* SUPPRESS_BOTS = ".m.rule.suppress_notices";
    constexpr const char* INVITE_ME = ".m.rule.invite_for_me";
    constexpr const char* PEOPLE_JOIN_LEAVE = ".m.rule.member_event";
    constexpr const char* CONTAIN_DISPLAY_NAME = ".m.rule.contains_display_name";
    constexpr const char* TOMBSTONE = ".m.rule.tombstone";
    constexpr const char* ROOM_NOTIF = ".m.rule.roomnotif";
    constexpr const char* CONTAIN_USER_NAME = ".m.rule.contains_user_name";
    constexpr const char* KEYWORDS = "_keywords";
    constexpr const char* CALL = ".m.rule.call";
    constexpr const char* ONE_TO_ONE_ENCRYPTED = ".m.rule.encrypted_room_one_to_one";
    constexpr const char* ONE_TO_ONE = ".m.rule.room_one_to_one";
    constexpr const char* ALL_OTHER = ".m.rule.message";
    constexpr const char* ENCRYPTED = ".m.rule.encrypted";
    constexpr const char* POLL_START = ".m.rule.poll_start";
    constexpr const char* POLL_END = ".m.rule.poll_end";
    constexpr const char* FALLBACK = ".m.rule.fallback";
    constexpr const char* REACTION = ".m.rule.reaction";
}

enum class ConditionKind {
    EVENT_MATCH = 0,             // "event_match"
    CONTAINS_DISPLAY_NAME = 1,   // "contains_display_name"
    ROOM_MEMBER_COUNT = 2,       // "room_member_count"
    SENDER_NOTIFICATION_PERM = 3,// "sender_notification_permission"
    UNRECOGNISED = 4
};

namespace RuleScope { constexpr const char* GLOBAL = "global"; }

// Push rule action types
enum class PushActionType {
    NOTIFY = 0, DONT_NOTIFY = 1, COALESCE = 2,
    SET_TWEAK_SOUND = 3, SET_TWEAK_HIGHLIGHT = 4
};

struct PushAction {
    PushActionType type = PushActionType::NOTIFY;
    std::string sound;           // for SET_TWEAK_SOUND ("default", "ring", or custom URI)
    bool highlight = true;       // for SET_TWEAK_HIGHLIGHT
};

// ==== Push Rules REST Models ====
//
// Original Kotlin: pushrules/rest/RuleSet.kt, rest/PushRule.kt, rest/PushCondition.kt

struct RestPushCondition {
    std::string kind;            // "kind" — "event_match", "contains_display_name", etc.
    std::string key;             // "key" — dot-separated field path
    std::string pattern;         // "pattern" — glob pattern
    std::string iz;              // "is" — value for room_member_count conditions

    ConditionKind getKind() const;
    bool isValid() const { return !kind.empty(); }
};

struct RestPushRule {
    std::vector<std::string> actionsRaw;     // "actions" — raw JSON list
    bool isDefault = false;                  // "default"
    bool enabled = true;                     // "enabled"
    std::string ruleId;                      // "rule_id"
    std::vector<RestPushCondition> conditions; // "conditions"
    std::string pattern;                     // "pattern" — for content rules

    bool shouldNotify() const;
    bool shouldNotNotify() const;
};

enum class RuleSetKey { CONTENT = 0, OVERRIDE = 1, ROOM = 2, SENDER = 3, UNDERRIDE = 4 };

struct RuleSet {
    std::vector<RestPushRule> contentRules;    // "content"
    std::vector<RestPushRule> overrideRules;   // "override"
    std::vector<RestPushRule> roomRules;       // "room"
    std::vector<RestPushRule> senderRules;     // "sender"
    std::vector<RestPushRule> underrideRules;  // "underride"

    // Original Kotlin: getAllRules() ordered by priority
    std::vector<RestPushRule> getAllRules() const;
    RestPushRule findDefaultRule(const std::string& ruleId, RuleSetKey* outKind = nullptr) const;
};

RuleSet parseRuleSet(const std::string& json);
std::string ruleSetToJson(const RuleSet& rs);

// ==== Push Notification Evaluation ====

struct PushEvaluation {
    bool shouldNotify = true;      // false = dont_notify
    bool shouldHighlight = false;  // true = @room or display_name matched
    bool isNoisy = true;           // false = silent notification
    std::string matchedRuleId;     // which rule triggered the result
    std::string matchedAction;     // "notify", "dont_notify", "coalesce"
};

// Evaluate whether a Matrix event should trigger a push notification.
// eventJson: the full event JSON (with content, type, sender, room_id)
// rules: the push rule set from /pushrules API
// myDisplayName: the user's display name for .m.rule.contains_display_name
// myUserId: the user's MXID for sender and room rules
PushEvaluation evaluatePushNotification(
    const std::string& eventJson,
    const PushRuleSet& rules,
    const std::string& myDisplayName,
    const std::string& myUserId);

// Check if a single condition matches an event.
bool conditionMatches(const PushCondition& condition,
    const std::string& eventType, const std::string& sender,
    const std::string& roomId, const std::string& body,
    const std::string& myDisplayName, const std::string& myUserId);

} // namespace progressive

#endif // PROGRESSIVE_PUSH_RULES_HPP
