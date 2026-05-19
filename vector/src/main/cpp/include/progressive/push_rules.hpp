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

// ==== Push Rule Action Tweak ====
//
// Original Kotlin: Action.kt
//   Action.Sound(sound: String) → sound tweak
//   Action.Highlight(highlight: Boolean) → highlight tweak

struct PushRuleActionTweak {
    std::string tweakKey;    // "sound", "highlight", or custom
    std::string value;       // for sound: "default", "ring"; for highlight: "true"/"false"
    bool highlight = false;  // parsed highlight boolean
};

// ==== Push Notification Action Constants ====
//
// Original Kotlin: Action.kt companion object
//   ACTION_NOTIFY          = "notify"
//   ACTION_DONT_NOTIFY     = "dont_notify"
//   ACTION_COALESCE        = "coalesce"
//   ACTION_OBJECT_SET_TWEAK_KEY = "set_tweak"
//   ACTION_OBJECT_SET_TWEAK_VALUE_SOUND = "sound"
//   ACTION_OBJECT_SET_TWEAK_VALUE_HIGHLIGHT = "highlight"

namespace PushActionConst {
    constexpr const char* NOTIFY       = "notify";
    constexpr const char* DONT_NOTIFY  = "dont_notify";
    constexpr const char* COALESCE     = "coalesce";
    constexpr const char* SET_TWEAK    = "set_tweak";
    constexpr const char* TWEAK_SOUND     = "sound";
    constexpr const char* TWEAK_HIGHLIGHT = "highlight";
    constexpr const char* VALUE_KEY    = "value";
    constexpr const char* VALUE_DEFAULT = "default";
    constexpr const char* VALUE_RING   = "ring";
}

// ==== Pusher Data Models ====
//
// Original Kotlin:
//   pushers/Pusher.kt (Pusher data class, PusherData data class, PusherState enum)
//   pushers/HttpPusher.kt (HttpPusher data class)
//   internal/session/pushers/JsonPusher.kt (JsonPusher data class)
//   internal/session/pushers/JsonPusherData.kt (JsonPusherData data class)

enum class PusherState {
    UNREGISTERED,
    REGISTERING,
    UNREGISTERING,
    REGISTERED,
    FAILED_TO_REGISTER
};

struct PusherData {
    std::string url;          // "url" — push gateway URL
    std::string format;       // "format" — "event_id_only" or empty
    std::string brand;        // "brand" — email branding (for email pushers)
    std::string lang;         // "lang" — preferred language

    // Original Kotlin: PusherData.isValid()
    bool isValid() const { return !url.empty(); }
};

struct Pusher {
    std::string pushKey;            // "pushkey" — FCM token or email
    std::string kind;               // "kind" — "http" or "email"
    std::string appId;              // "app_id" — reverse-DNS app identifier
    std::string appDisplayName;     // "app_display_name"
    std::string deviceDisplayName;  // "device_display_name"
    std::string profileTag;         // "profile_tag" — device-specific rule set
    std::string lang;               // "lang" — e.g. "en" or "en-US"
    PusherData data;                // "data" — url/format
    std::string deviceId;           // "org.matrix.msc3881.device_id"
    bool enabled = true;            // "org.matrix.msc3881.enabled"
    bool append = false;            // "append"
    PusherState state = PusherState::UNREGISTERED;

    // Original Kotlin: Pusher companion KIND_ constants
    static constexpr const char* KIND_HTTP  = "http";
    static constexpr const char* KIND_EMAIL = "email";
    static constexpr const char* APP_ID_EMAIL = "m.email";
};

// Build JSON for POST /pushers/set API.
// Original Kotlin: JsonPusher (137 lines) — serialises to JSON for /pushers/set
std::string buildPusherJson(const Pusher& pusher);

// Parse a single pusher from JSON.
// Original Kotlin: GetPushersResponse.kt + JsonPusher.kt
Pusher parsePusher(const std::string& json);

// ==== Push Rules Response ====
//
// Original Kotlin: GetPushRulesResponse.kt (33 lines)
//   /pushrules response: { "global": RuleSet, ...device_specific }

struct PushRulesResponse {
    RuleSet global;                                          // "global"
    std::vector<RuleSet> deviceSpecificSets;                  // future device-specific rules
    std::string rawJson;                                     // original response for caching

    bool isEmpty() const;
    size_t totalRuleCount() const;
};

PushRulesResponse parsePushRulesResponse(const std::string& json);

// ==== Push Rule Matching ====
//
// Original Kotlin:
//   EventMatchCondition.kt (105 lines) — isSatisfied() with glob matching
//   PushRuleFinder.kt (35 lines) — fulfilledBingRule()
//   internal/util/Glob.kt (39 lines) — simpleGlobToRegExp()

// Glob pattern matching. Supports * (any chars), ? (any single char).
// Original Kotlin: Glob.kt :: simpleGlobToRegExp — converts glob to regex
// // Matches are case-insensitive.
bool globMatch(const std::string& pattern, const std::string& text);

// Convert a glob pattern to a regex-compatible string.
// Original Kotlin: simpleGlobToRegExp(glob: String): String
// // * → .*   ? → .   . → \\.   \\ → \\\\
std::string globToRegex(const std::string& glob);

// Evaluate all conditions of a push rule against an event.
// Returns true if ALL conditions are satisfied (AND logic).
// Original Kotlin: PushRuleFinder.kt :: fulfilledBingRule()
bool evaluatePushConditions(
    const std::vector<RestPushCondition>& conditions,
    const std::string& eventJson,
    const std::string& myDisplayName,
    const std::string& myUserId);

// Check if a rule kind implicitly matches an event property.
// e.g., "room" rules match events in the specified room.
// Original Kotlin: room-specific rules have implicit room ID match.
bool kindMatches(const std::string& kind,
    const std::string& roomId, const std::string& ruleId,
    const std::string& senderId, const std::string& myUserId);

// Match a single push rule against an event.
// Returns true if the rule's conditions AND implicit kind matching are satisfied.
bool matchPushRule(const RestPushRule& rule, const std::string& eventJson,
    const std::string& myDisplayName, const std::string& myUserId,
    const std::string& roomId);

// ==== Push Rule Content Builder ====
//
// Original Kotlin:
//   UpdatePushRuleEnableStatusTask.kt (47 lines) — PUT with EnabledBody
//   UpdatePushRuleActionsTask.kt (55 lines) — PUT with actions
//   AddPushRuleTask.kt (42 lines) — PUT to add rule

struct PushRuleContentBuilder {
    // Build JSON body: { "enabled": true/false }
    // Original Kotlin: EnabledBody.kt (25 lines)
    static std::string buildEnabledBody(bool enabled);

    // Build JSON body: { "actions": [...] }
    // Original Kotlin: UpdatePushRuleActionsTask.kt :: updateRuleActions()
    static std::string buildActionsBody(const std::vector<std::string>& actions);

    // Build JSON body for adding a content rule:
    // { "pattern": "...", "actions": [...], "conditions": [...], "enabled": true }
    // Original Kotlin: AddPushRuleTask.kt :: addRule()
    static std::string buildAddRuleBody(const std::string& ruleId,
        const std::string& pattern, const std::vector<std::string>& actions,
        const std::vector<std::string>& conditionsJson);

    // Build JSON body for setting rule actions:
    // { "actions": [...] }
    // Original Kotlin: updateRuleActions() — PUT /pushrules/global/{kind}/{ruleId}/actions
    static std::string buildSetActionsBody(const std::vector<PushRuleActionTweak>& tweaks,
        bool notify, bool highlight, const std::string& soundName);

    // Build JSON for enable/disable PUT.
    static std::string buildEnablePushRuleAction(bool enabled);

    // Build JSON for adding a push rule via PUT.
    static std::string buildAddPushRuleAction(const std::string& ruleId,
        const std::string& pattern, const RestPushRule& rule);

    // Build JSON for setting push rule actions array.
    static std::string buildSetPushRuleActions(const std::vector<std::string>& actions);
};

} // namespace progressive

#endif // PROGRESSIVE_PUSH_RULES_HPP
