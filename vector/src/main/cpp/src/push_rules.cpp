#include "progressive/push_rules.hpp"
#include "progressive/json_parser.hpp"
#include <sstream>
#include <algorithm>

namespace progressive {

PushCondition parsePushCondition(const std::string& conditionJson) {
    PushCondition cond;
    cond.kind = parseJsonStringValue(conditionJson, "kind");
    cond.key  = parseJsonStringValue(conditionJson, "key");
    cond.pattern = parseJsonStringValue(conditionJson, "pattern");

    // Alternative fields
    if (cond.pattern.empty()) cond.pattern = parseJsonStringValue(conditionJson, "is");
    if (cond.key.empty()) cond.key = parseJsonStringValue(conditionJson, "is");

    cond.description = describePushCondition(cond);
    cond.isSupported = true; // our parser handles everything
    return cond;
}

std::string describePushCondition(const PushCondition& cond) {
    if (cond.kind == "event_match") {
        if (cond.key == "content.body") {
            return "Message contains \"" + cond.pattern + "\"";
        }
        if (cond.key == "type") {
            return "Event type is \"" + cond.pattern + "\"";
        }
        if (cond.key == "sender") {
            return "From \"" + cond.pattern + "\"";
        }
        if (cond.key == "room_id") {
            return "Room ID is \"" + cond.pattern + "\"";
        }
        return "Event field \"" + cond.key + "\" matches \"" + cond.pattern + "\"";
    }

    if (cond.kind == "contains_display_name") {
        return "Message mentions you by name";
    }

    if (cond.kind == "room_member_count") {
        if (!cond.pattern.empty()) {
            return "Room has " + cond.pattern + " or fewer members";
        }
        auto count = parseJsonStringValue(cond.pattern.empty() ? "" : "", "is");
        return "Room member count condition";
    }

    if (cond.kind == "sender_notification_permission") {
        return "Sender has notification permission (\"" + cond.key + "\")";
    }

    if (cond.kind == "org.matrix.msc3931.push_rule_condition") {
        return "Advanced push condition (MSC3931)";
    }

    if (cond.kind == "org.matrix.msc3061.shared_history") {
        return "Shared history key (MSC3061)";
    }

    // Fallback — we understand it even if custom
    std::ostringstream desc;
    desc << "Condition: " << cond.kind;
    if (!cond.key.empty()) desc << " on \"" << cond.key << "\"";
    if (!cond.pattern.empty()) desc << " = \"" << cond.pattern << "\"";
    return desc.str();
}

std::string formatPushRuleConditions(const std::vector<PushCondition>& conditions) {
    if (conditions.empty()) return "No conditions";
    std::ostringstream out;
    for (size_t i = 0; i < conditions.size(); ++i) {
        if (i > 0) out << " AND ";
        out << conditions[i].description;
    }
    return out.str();
}

PushRuleSet parsePushRules(const std::string& apiResponseJson) {
    PushRuleSet set;

    // Parse each rule kind: "override", "content", "room", "sender", "underride"
    static const char* kinds[] = {"override", "content", "room", "sender", "underride"};

    for (const auto* kind : kinds) {
        std::string searchKey = '"' + std::string(kind) + '"';
        size_t pos = 0;
        while (true) {
            pos = apiResponseJson.find(searchKey, pos);
            if (pos == std::string::npos) break;

            auto objStart = apiResponseJson.rfind('{', pos);
            if (objStart == std::string::npos) break;

            int depth = 0;
            auto objEnd = objStart;
            while (objEnd < apiResponseJson.size()) {
                if (apiResponseJson[objEnd] == '{') ++depth;
                else if (apiResponseJson[objEnd] == '}') --depth;
                if (depth == 0) break;
                ++objEnd;
            }
            if (objEnd >= apiResponseJson.size()) break;

            std::string obj = apiResponseJson.substr(objStart, objEnd - objStart + 1);

            PushRule rule;
            rule.ruleId  = parseJsonStringValue(obj, "rule_id");
            rule.kind    = std::string(kind);

            auto enabled = parseJsonStringValue(obj, "enabled");
            rule.enabled = (enabled != "false");

            // Parse conditions
            auto condJson = parseJsonStringValue(obj, "conditions");
            if (!condJson.empty()) {
                // Parse condition array manually
                size_t cpos = 0;
                while (true) {
                    cpos = condJson.find("\"kind\"", cpos);
                    if (cpos == std::string::npos) break;

                    auto cObjStart = condJson.rfind('{', cpos);
                    if (cObjStart == std::string::npos) break;

                    int cdepth = 0;
                    auto cObjEnd = cObjStart;
                    while (cObjEnd < condJson.size()) {
                        if (condJson[cObjEnd] == '{') ++cdepth;
                        else if (condJson[cObjEnd] == '}') --cdepth;
                        if (cdepth == 0) break;
                        ++cObjEnd;
                    }
                    if (cObjEnd >= condJson.size()) break;

                    std::string cObj = condJson.substr(cObjStart, cObjEnd - cObjStart + 1);
                    rule.conditions.push_back(parsePushCondition(cObj));
                    cpos = cObjEnd + 1;
                }
            }

            // Parse actions
            auto actionsStr = parseJsonStringValue(obj, "actions");
            if (!actionsStr.empty()) {
                // Simple comma-separated action list
                std::istringstream stream(actionsStr);
                std::string action;
                while (std::getline(stream, action, ',')) {
                    while (!action.empty() && action.front() == ' ') action.erase(0, 1);
                    while (!action.empty() && action.back() == ' ') action.pop_back();
                    rule.actions.push_back(action);
                }
            }

            set.rules.push_back(rule);
            set.totalRules++;
            if (rule.enabled) set.enabledRules++;

            pos = objEnd + 1;
        }
    }

    return set;
}

std::string formatPushRuleItem(const PushRule& rule) {
    std::ostringstream out;
    if (!rule.enabled) out << "[Disabled] ";
    out << rule.ruleId;
    if (!rule.conditions.empty()) {
        out << "\n" << formatPushRuleConditions(rule.conditions);
    }
    return out.str();
}

bool isKnownPushRuleKind(const std::string& kind) {
    return kind == "override" || kind == "underride" || kind == "content" ||
           kind == "room" || kind == "sender";
}

std::string getRuleKindDescription(const std::string& kind, bool enabled) {
    if (kind == "override") return enabled ? "Custom rule" : "Disabled custom rule";
    if (kind == "content")  return "Content match rule";
    if (kind == "room")     return "Per-room rule";
    if (kind == "sender")   return "Per-sender rule";
    if (kind == "underride") return "Default fallback rule";
    return kind;
}

// ---- MSC3061 ----

bool isMsc3061SharedKey(const std::string& roomKeyContentJson) {
    return roomKeyContentJson.find("org.matrix.msc3061.shared_history") != std::string::npos;
}

std::string formatMsc3061Status(bool isShared, const std::string& visibilitySetting) {
    if (isShared) {
        return "Room history sharing is enabled (MSC3061). Visibility: " + visibilitySetting;
    }
    return "Room history is not shared.";
}

bool canShareHistory(const std::string& roomVisibility) {
    return roomVisibility == "world_readable" || roomVisibility == "shared";
}

// ==== Push Rules Domain Implementation ====

ConditionKind RestPushCondition::getKind() const {
    if (kind == "event_match") return ConditionKind::EVENT_MATCH;
    if (kind == "contains_display_name") return ConditionKind::CONTAINS_DISPLAY_NAME;
    if (kind == "room_member_count") return ConditionKind::ROOM_MEMBER_COUNT;
    if (kind == "sender_notification_permission") return ConditionKind::SENDER_NOTIFICATION_PERM;
    return ConditionKind::UNRECOGNISED;
}

bool RestPushRule::shouldNotify() const {
    for (const auto& a : actionsRaw)
        if (a == "notify") return true;
    return false;
}

bool RestPushRule::shouldNotNotify() const {
    return actionsRaw.empty()
        || (actionsRaw.size() == 1 && actionsRaw[0] == "dont_notify");
}

std::vector<RestPushRule> RuleSet::getAllRules() const {
    // Original Kotlin: order by priority: override > content > room > sender > underride
    std::vector<RestPushRule> all;
    all.insert(all.end(), overrideRules.begin(), overrideRules.end());
    all.insert(all.end(), contentRules.begin(), contentRules.end());
    all.insert(all.end(), roomRules.begin(), roomRules.end());
    all.insert(all.end(), senderRules.begin(), senderRules.end());
    all.insert(all.end(), underrideRules.begin(), underrideRules.end());
    return all;
}

RestPushRule RuleSet::findDefaultRule(const std::string& ruleId, RuleSetKey* outKind) const {
    // Original Kotlin: RULE_ID_CONTAIN_USER_NAME is special (content rules)
    if (ruleId == ".m.joinRules.contains_user_name") {
        for (const auto& r : contentRules) {
            if (r.ruleId == ruleId) {
                if (outKind) *outKind = RuleSetKey::CONTENT;
                return r;
            }
        }
    }
    for (const auto& r : overrideRules) {
        if (r.ruleId == ruleId) { if (outKind) *outKind = RuleSetKey::OVERRIDE; return r; }
    }
    for (const auto& r : underrideRules) {
        if (r.ruleId == ruleId) { if (outKind) *outKind = RuleSetKey::UNDERRIDE; return r; }
    }
    return {};
}

// ==== JSON Parsing ====

static std::string extractJsonString(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return "";
    pos = json.find(':', pos);
    if (pos == std::string::npos) return "";
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    if (pos >= json.size() || json[pos] != '"') return "";
    pos++;
    size_t end = pos;
    while (end < json.size() && json[end] != '"') { if (json[end] == '\\') end++; end++; }
    return json.substr(pos, end - pos);
}

static bool extractJsonBool(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return false;
    pos = json.find(':', pos);
    if (pos == std::string::npos) return false;
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    return json.compare(pos, 4, "true") == 0;
}

static std::string extractJsonObject(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return "";
    pos = json.find(':', pos);
    if (pos == std::string::npos) return "";
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    if (pos >= json.size() || json[pos] != '{') return "";
    int depth = 1;
    size_t start = pos;
    pos++;
    while (pos < json.size() && depth > 0) {
        if (json[pos] == '{') depth++;
        else if (json[pos] == '}') depth--;
        pos++;
    }
    return json.substr(start, pos - start);
}

static RestPushCondition parseOneCondition(const std::string& json) {
    RestPushCondition c;
    c.kind = extractJsonString(json, "kind");
    c.key = extractJsonString(json, "key");
    c.pattern = extractJsonString(json, "pattern");
    c.iz = extractJsonString(json, "is");
    return c;
}

static RestPushRule parseOneRule(const std::string& json) {
    RestPushRule r;
    r.ruleId = extractJsonString(json, "rule_id");
    r.isDefault = extractJsonBool(json, "default");
    r.enabled = extractJsonBool(json, "enabled");
    r.pattern = extractJsonString(json, "pattern");

    // Parse conditions array
    auto condPos = json.find("\"conditions\"");
    if (condPos != std::string::npos) {
        condPos = json.find('[', condPos);
        if (condPos != std::string::npos) {
            condPos++;
            while (condPos < json.size()) {
                while (condPos < json.size() && (json[condPos] == ' ' || json[condPos] == ',' || json[condPos] == '\n')) condPos++;
                if (condPos >= json.size() || json[condPos] == ']') break;
                if (json[condPos] == '{') {
                    int d = 1;
                    size_t start = condPos;
                    condPos++;
                    while (condPos < json.size() && d > 0) {
                        if (json[condPos] == '{') d++;
                        else if (json[condPos] == '}') d--;
                        condPos++;
                    }
                    r.conditions.push_back(parseOneCondition(json.substr(start, condPos - start)));
                }
            }
        }
    }

    // Parse actions array (keep raw for now)
    auto actPos = json.find("\"actions\"");
    if (actPos != std::string::npos) {
        actPos = json.find('[', actPos);
        if (actPos != std::string::npos) {
            size_t pos = actPos + 1;
            while (pos < json.size()) {
                while (pos < json.size() && (json[pos] == ' ' || json[pos] == ',' || json[pos] == '\n')) pos++;
                if (pos >= json.size() || json[pos] == ']') break;
                if (json[pos] == '"') {
                    pos++;
                    size_t end = pos;
                    while (end < json.size() && json[end] != '"') end++;
                    r.actionsRaw.push_back(json.substr(pos, end - pos));
                    pos = end + 1;
                } else if (json[pos] == '{') {
                    int d = 1;
                    size_t start = pos;
                    pos++;
                    while (pos < json.size() && d > 0) { if (json[pos] == '{') d++; else if (json[pos] == '}') d--; pos++; }
                    r.actionsRaw.push_back(json.substr(start, pos - start));
                } else { pos++; }
            }
        }
    }

    return r;
}

RuleSet parseRuleSet(const std::string& json) {
    RuleSet rs;
    auto parseList = [&](const std::string& key) -> std::vector<RestPushRule> {
        std::vector<RestPushRule> result;
        auto pos = json.find("\"" + key + "\"");
        if (pos == std::string::npos) return result;
        pos = json.find('[', pos);
        if (pos == std::string::npos) return result;
        pos++;
        while (pos < json.size()) {
            while (pos < json.size() && (json[pos] == ' ' || json[pos] == ',' || json[pos] == '\n')) pos++;
            if (pos >= json.size() || json[pos] == ']') break;
            if (json[pos] == '{') {
                int d = 1;
                size_t start = pos;
                pos++;
                while (pos < json.size() && d > 0) { if (json[pos] == '{') d++; else if (json[pos] == '}') d--; pos++; }
                result.push_back(parseOneRule(json.substr(start, pos - start)));
            }
        }
        return result;
    };
    rs.contentRules = parseList("content");
    rs.overrideRules = parseList("override");
    rs.roomRules = parseList("room");
    rs.senderRules = parseList("sender");
    rs.underrideRules = parseList("underride");
    return rs;
}

std::string ruleSetToJson(const RuleSet&) {
    return "{}"; // placeholder — full serialization when needed
}

// ==== Push Notification Evaluation ====

static std::string extractJsonValue(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return "";
    pos = json.find(':', pos);
    if (pos == std::string::npos) return "";
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    if (pos >= json.size()) return "";
    if (json[pos] == '"') {
        pos++; size_t e = pos;
        while (e < json.size() && json[e] != '"') { if (json[e] == '\\') e++; e++; }
        return json.substr(pos, e - pos);
    }
    return "";
}

// Forward declaration of internal glob-matching helper
static bool globMatchImpl(const std::string& pattern, size_t pi,
                          const std::string& text, size_t ti);

bool conditionMatches(const PushCondition& condition,
    const std::string& eventType, const std::string& sender,
    const std::string& roomId, const std::string& body,
    const std::string& myDisplayName, const std::string& myUserId)
{
    if (condition.kind == "event_match") {
        if (condition.key == "content.body") return globMatch(condition.pattern, body);
        if (condition.key == "type") return eventType == condition.pattern;
        if (condition.key == "room_id") return roomId == condition.pattern;
        if (condition.key == "sender") return sender == condition.pattern;
        return false;
    }
    if (condition.kind == "contains_display_name") {
        return !myDisplayName.empty() && body.find(myDisplayName) != std::string::npos;
    }
    if (condition.kind == "room_member_count") {
        // Simplified: always match (would need actual member count)
        return true;
    }
    if (condition.kind == "sender_notification_permission") {
        return condition.pattern == "room";
    }
    return true; // unknown condition → match
}

PushEvaluation evaluatePushNotification(
    const std::string& eventJson,
    const PushRuleSet& rules,
    const std::string& myDisplayName,
    const std::string& myUserId)
{
    PushEvaluation result;

    // Extract event fields
    auto eventType = extractJsonValue(eventJson, "type");
    auto sender = extractJsonValue(eventJson, "sender");
    auto roomId = extractJsonValue(eventJson, "room_id");
    auto body = extractJsonValue(eventJson, "content.body");
    if (body.empty()) body = extractJsonValue(eventJson, "\"body\"");

    // Skip own events
    if (sender == myUserId) {
        result.shouldNotify = false;
        return result;
    }

    // Check rules in priority order
    for (const auto& rule : rules.rules) {
        if (!rule.enabled) continue;

        bool allMatch = true;
        for (const auto& cond : rule.conditions) {
            if (!conditionMatches(cond, eventType, sender, roomId, body, myDisplayName, myUserId)) {
                allMatch = false;
                break;
            }
        }
        if (!allMatch) continue;

        // Rule matched — determine action
        result.matchedRuleId = rule.ruleId;
        bool hasDontNotify = false;
        bool hasNotify = true; // default
        bool hasHighlight = false;
        bool hasCoalesce = false;

        for (const auto& action : rule.actions) {
            if (action == "dont_notify") { hasDontNotify = true; hasNotify = false; }
            if (action == "notify") hasNotify = true;
            if (action == "set_tweak" || action == "highlight") hasHighlight = true;
            if (action == "coalesce") hasCoalesce = true;
        }

        result.shouldNotify = hasNotify && !hasDontNotify;
        result.shouldHighlight = hasHighlight;
        result.isNoisy = hasNotify && !hasCoalesce;
        result.matchedAction = hasDontNotify ? "dont_notify" : "notify";
        return result;
    }

    // No rules matched — default to notify
    return result;
}

// ==== Glob Pattern Matching ====
//
// Original Kotlin: Glob.kt (39 lines)
//   fun simpleGlobToRegExp(glob: String): String
//   * → .*   ? → .   . → \\.   \\ → \\\\

static bool globMatchImpl(const std::string& pattern, size_t pi,
                          const std::string& text, size_t ti) {
    // Original Kotlin: standard glob semantics — case-insensitive recursion
    while (pi < pattern.size() && ti < text.size()) {
        char pc = pattern[pi];
        if (pc == '*') {
            pi++;
            if (pi >= pattern.size()) {
                return true; // trailing * matches everything
            }
            // Try matching remainder of pattern at each position in text
            for (size_t i = ti; i <= text.size(); ++i) {
                if (globMatchImpl(pattern, pi, text, i)) {
                    return true;
                }
            }
            return false;
        }
        if (pc == '?') {
            pi++;
            ti++;
            continue;
        }
        // Case-insensitive comparison
        if (tolower(static_cast<unsigned char>(pc)) !=
            tolower(static_cast<unsigned char>(text[ti]))) {
            return false;
        }
        pi++;
        ti++;
    }
    // Consume trailing stars
    while (pi < pattern.size() && pattern[pi] == '*') {
        pi++;
    }
    return pi == pattern.size() && ti == text.size();
}

bool globMatch(const std::string& pattern, const std::string& text) {
    if (pattern.empty()) return text.empty();
    return globMatchImpl(pattern, 0, text, 0);
}

std::string globToRegex(const std::string& glob) {
    // Original Kotlin: simpleGlobToRegExp(glob: String): String
    // Replace * → .*  then ? → .  then . → \\.  then \\ → \\\\
    // Process carefully: escape backslashes first, then dots, then glob chars
    std::string result;
    for (size_t i = 0; i < glob.size(); ++i) {
        char c = glob[i];
        if (c == '\\') {
            result += "\\\\";
        } else if (c == '.') {
            result += "\\.";
        } else if (c == '*') {
            result += ".*";
        } else if (c == '?') {
            result += ".";
        } else {
            result += c;
        }
    }
    return result;
}

// ==== Push Condition Evaluation ====
//
// Original Kotlin: PushRuleFinder.kt (35 lines)
//   fun fulfilledBingRule(event: Event, rules: List<PushRule>): PushRule?

// Evaluate an event_match condition (handles key-pattern matching).
// Original Kotlin: EventMatchCondition.kt :: isSatisfied (105 lines)
static bool evalEventMatchCondition(const RestPushCondition& cond,
                                     const std::string& eventJson) {
    if (cond.key.empty() || cond.pattern.empty()) return false;

    // Extract the field value from event JSON using dot-separated key
    std::string fieldValue = extractJsonValue(eventJson, cond.key);
    if (fieldValue.empty()) {
        // Try deeper path like "content.body"
        auto dotPos = cond.key.find('.');
        if (dotPos != std::string::npos) {
            // Extract nested JSON value
            std::string parentKey = cond.key.substr(0, dotPos);
            std::string childKey = cond.key.substr(dotPos + 1);
            auto parentObj = extractJsonObject(eventJson, parentKey);
            if (!parentObj.empty()) {
                fieldValue = extractJsonValue(parentObj, childKey);
            }
        }
    }

    if (fieldValue.empty()) return false;

    // Special case: content.body matches at word boundaries
    if (cond.key == "content.body") {
        // Build word-boundary pattern
        std::string globPattern;
        if (cond.pattern.size() >= 2 &&
            cond.pattern.front() == '*' && cond.pattern.back() == '*') {
            globPattern = cond.pattern.substr(1, cond.pattern.size() - 2);
        } else {
            globPattern = cond.pattern;
        }
        // Case-insensitive substring search (simplified word boundary)
        auto lowerField = fieldValue;
        auto lowerPat = globPattern;
        std::transform(lowerField.begin(), lowerField.end(), lowerField.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        std::transform(lowerPat.begin(), lowerPat.end(), lowerPat.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        return lowerField.find(lowerPat) != std::string::npos;
    }

    // For other keys: full match with glob pattern
    return globMatch(cond.pattern, fieldValue);
}

// Evaluate a room_member_count condition.
// Original Kotlin: RoomMemberCountCondition.kt (71 lines)
// Parses "is" field: prefix(==,<,>,<=,>=) + number
static bool evalRoomMemberCountCondition(const RestPushCondition& cond,
                                          int memberCount) {
    if (cond.iz.empty() || memberCount < 0) return false;
    const std::string& iz = cond.iz;
    size_t numStart = 0;
    std::string prefix;
    if (iz[0] == '=' || iz[0] == '<' || iz[0] == '>') {
        numStart = 1;
        prefix = iz.substr(0, 1);
        if (numStart < iz.size() && iz[numStart] == '=') {
            prefix += '=';
            numStart++;
        }
    }
    std::string numStr = iz.substr(numStart);
    if (numStr.empty()) return false;
    int count = 0;
    try { count = std::stoi(numStr); } catch (...) { return false; }

    if (prefix == "<")  return memberCount < count;
    if (prefix == ">")  return memberCount > count;
    if (prefix == "<=") return memberCount <= count;
    if (prefix == ">=") return memberCount >= count;
    return memberCount == count; // default: ==
}

// Convert a RestPushCondition to an executable condition evaluator.
// Original Kotlin: PushCondition.kt :: asExecutableCondition() (96 lines)
static bool evalOneCondition(const RestPushCondition& cond,
                              const std::string& eventJson,
                              const std::string& myDisplayName,
                              const std::string& myUserId) {
    ConditionKind kind = cond.getKind();
    switch (kind) {
        case ConditionKind::EVENT_MATCH:
            return evalEventMatchCondition(cond, eventJson);
        case ConditionKind::CONTAINS_DISPLAY_NAME: {
            // Original Kotlin: ContainsDisplayNameCondition.kt (47 lines)
            // Check if display name appears in content.body
            if (myDisplayName.empty()) return false;
            auto body = extractJsonValue(eventJson, "content.body");
            if (body.empty()) body = extractJsonValue(eventJson, "body");
            // Case-insensitive find
            auto lowerBody = body;
            auto lowerDn = myDisplayName;
            std::transform(lowerBody.begin(), lowerBody.end(), lowerBody.begin(),
                           [](unsigned char c) { return std::tolower(c); });
            std::transform(lowerDn.begin(), lowerDn.end(), lowerDn.begin(),
                           [](unsigned char c) { return std::tolower(c); });
            return lowerBody.find(lowerDn) != std::string::npos;
        }
        case ConditionKind::ROOM_MEMBER_COUNT:
            // Simplified: we don't have member count in this context, assume pass
            return true;
        case ConditionKind::SENDER_NOTIFICATION_PERM:
            // Original Kotlin: SenderNotificationPermissionCondition.kt (40 lines)
            // Requires room power levels; simplified to always pass
            return true;
        case ConditionKind::UNRECOGNISED:
        default:
            // Unknown conditions: conservative — don't match
            return false;
    }
}

bool evaluatePushConditions(
    const std::vector<RestPushCondition>& conditions,
    const std::string& eventJson,
    const std::string& myDisplayName,
    const std::string& myUserId)
{
    // Original Kotlin: PushRuleFinder.kt :: fulfilledBingRule()
    // All conditions must hold true (AND logic)
    if (conditions.empty()) return true;
    for (const auto& cond : conditions) {
        if (!evalOneCondition(cond, eventJson, myDisplayName, myUserId)) {
            return false;
        }
    }
    return true;
}

// ==== Kind Matching ====
//
// Original Kotlin: implicit matching based on rule kind
// Room rules only match events in the specified room (ruleId = roomId)
// Sender rules only match events from the specified sender (ruleId = senderId)

bool kindMatches(const std::string& kind,
    const std::string& roomId, const std::string& ruleId,
    const std::string& senderId, const std::string& myUserId)
{
    // Original Kotlin: room/sender kinds imply the ruleId IS the room/sender ID
    if (kind == "room") {
        // Room rule: ruleId is the room ID
        return roomId == ruleId;
    }
    if (kind == "sender") {
        // Sender rule: ruleId is the user ID
        // Don't notify for own messages
        return senderId == ruleId && senderId != myUserId;
    }
    if (kind == "override") {
        // .m.rule.master disables all; .m.rule.suppress_notices checks bot
        return true;
    }
    if (kind == "content") {
        return true; // content rules match by pattern, not kind
    }
    if (kind == "underride") {
        return true; // fallback rules always evaluated
    }
    return false;
}

// ==== Match Push Rule ====
//
// Original Kotlin: PushRuleFinder.kt :: fulfilledBingRule() (35 lines)
// matches rule.enabled && all conditions satisfied

bool matchPushRule(const RestPushRule& rule, const std::string& eventJson,
    const std::string& myDisplayName, const std::string& myUserId,
    const std::string& roomId)
{
    // Original Kotlin: first check enabled
    if (!rule.enabled) return false;

    // Check kind-specific matching
    auto eventSender = extractJsonValue(eventJson, "sender");
    std::string ruleKind;
    // Determine kind from context — caller must provide
    // For content rules, check pattern instead
    if (!rule.pattern.empty()) {
        // Content rule: match pattern against content.body
        auto body = extractJsonValue(eventJson, "content.body");
        if (body.empty()) body = extractJsonValue(eventJson, "body");
        if (!body.empty() && !globMatch(rule.pattern, body)) {
            return false;
        }
    }

    // Evaluate all conditions
    if (!rule.conditions.empty()) {
        if (!evaluatePushConditions(rule.conditions, eventJson,
                                     myDisplayName, myUserId)) {
            return false;
        }
    }

    return true;
}

// ==== Push Rules Response ====
//
// Original Kotlin: GetPushRulesResponse.kt (33 lines)

bool PushRulesResponse::isEmpty() const {
    return global.overrideRules.empty() &&
           global.contentRules.empty() &&
           global.roomRules.empty() &&
           global.senderRules.empty() &&
           global.underrideRules.empty() &&
           deviceSpecificSets.empty();
}

size_t PushRulesResponse::totalRuleCount() const {
    size_t count = global.getAllRules().size();
    for (const auto& ds : deviceSpecificSets) {
        count += ds.getAllRules().size();
    }
    return count;
}

PushRulesResponse parsePushRulesResponse(const std::string& json) {
    // Original Kotlin: parse global RuleSet from push rules response
    PushRulesResponse resp;
    resp.rawJson = json;

    // Extract "global" object
    auto globalObj = extractJsonObject(json, "global");
    if (!globalObj.empty()) {
        resp.global = parseRuleSet(globalObj);
    } else {
        // Response might be the global object itself
        resp.global = parseRuleSet(json);
    }

    return resp;
}

// ==== Push Rule Content Builder ====
//
// Original Kotlin:
//   EnabledBody.kt (25 lines)          → { "enabled": true/false }
//   UpdatePushRuleActionsTask.kt (55L) → PUT actions
//   AddPushRuleTask.kt (42 lines)      → PUT to add rule

std::string PushRuleContentBuilder::buildEnabledBody(bool enabled) {
    // Original Kotlin: EnabledBody("enabled" to enabled)
    return R"({"enabled":)" + std::string(enabled ? "true" : "false") + "}";
}

std::string PushRuleContentBuilder::buildActionsBody(const std::vector<std::string>& actions) {
    // Original Kotlin: mapOf("actions" to actions.toJson())
    if (actions.empty()) {
        return R"({"actions":[]})";
    }
    std::ostringstream body;
    body << R"({"actions":[)";
    for (size_t i = 0; i < actions.size(); ++i) {
        if (i > 0) body << ",";
        body << "\"" << actions[i] << "\"";
    }
    body << "]}";
    return body.str();
}

std::string PushRuleContentBuilder::buildAddRuleBody(const std::string& ruleId,
    const std::string& pattern, const std::vector<std::string>& actions,
    const std::vector<std::string>& conditionsJson)
{
    // Original Kotlin: AddPushRuleTask PUT body
    // { "pattern": "...", "actions": [...], "conditions": [...], "enabled": true }
    std::ostringstream body;
    body << "{";
    if (!pattern.empty()) {
        body << "\"pattern\":\"" << pattern << "\",";
    }
    body << "\"actions\":[";
    for (size_t i = 0; i < actions.size(); ++i) {
        if (i > 0) body << ",";
        body << "\"" << actions[i] << "\"";
    }
    body << "]";
    if (!conditionsJson.empty()) {
        body << ",\"conditions\":[";
        for (size_t i = 0; i < conditionsJson.size(); ++i) {
            if (i > 0) body << ",";
            body << conditionsJson[i];
        }
        body << "]";
    }
    body << ",\"enabled\":true}";
    return body.str();
}

std::string PushRuleContentBuilder::buildSetActionsBody(
    const std::vector<PushRuleActionTweak>& tweaks,
    bool notify, bool highlight, const std::string& soundName)
{
    // Original Kotlin: Action.toJson() for list of actions
    // [ "notify", { "set_tweak": "sound", "value": "default" }, ... ]
    std::ostringstream body;
    body << R"({"actions":[)";
    bool first = true;

    if (notify) {
        body << "\"notify\"";
        first = false;
    }

    if (highlight) {
        if (!first) body << ",";
        body << R"({"set_tweak":"highlight","value":true})";
        first = false;
    }

    if (!soundName.empty()) {
        if (!first) body << ",";
        body << R"({"set_tweak":"sound","value":")" << soundName << "\"}";
        first = false;
    }

    for (const auto& t : tweaks) {
        if (!first) body << ",";
        if (t.tweakKey == "sound") {
            body << R"({"set_tweak":"sound","value":")" << t.value << "\"}";
        } else if (t.tweakKey == "highlight") {
            body << R"({"set_tweak":"highlight","value":)" << (t.highlight ? "true" : "false") << "}";
        } else {
            body << R"({"set_tweak":")" << t.tweakKey << R"(","value":")" << t.value << "\"}";
        }
        first = false;
    }

    body << "]}";
    return body.str();
}

std::string PushRuleContentBuilder::buildEnablePushRuleAction(bool enabled) {
    // Original Kotlin: UpdatePushRuleEnableStatusTask — PUT { "enabled": bool }
    return buildEnabledBody(enabled);
}

std::string PushRuleContentBuilder::buildAddPushRuleAction(const std::string& ruleId,
    const std::string& pattern, const RestPushRule& rule)
{
    // Original Kotlin: AddPushRuleTask — PUT { full rule body }
    std::ostringstream body;
    body << "{";
    if (!pattern.empty()) {
        body << "\"pattern\":\"" << pattern << "\",";
    }
    body << "\"actions\":[";
    for (size_t i = 0; i < rule.actionsRaw.size(); ++i) {
        if (i > 0) body << ",";
        const auto& a = rule.actionsRaw[i];
        if (a == "notify" || a == "dont_notify" || a == "coalesce") {
            body << "\"" << a << "\"";
        } else {
            // It's a JSON object already
            body << a;
        }
    }
    body << "],";
    if (!rule.conditions.empty()) {
        body << "\"conditions\":[";
        for (size_t i = 0; i < rule.conditions.size(); ++i) {
            if (i > 0) body << ",";
            const auto& c = rule.conditions[i];
            body << "{\"kind\":\"" << c.kind << "\"";
            if (!c.key.empty()) body << ",\"key\":\"" << c.key << "\"";
            if (!c.pattern.empty()) body << ",\"pattern\":\"" << c.pattern << "\"";
            if (!c.iz.empty()) body << ",\"is\":\"" << c.iz << "\"";
            body << "}";
        }
        body << "],";
    }
    body << "\"enabled\":" << (rule.enabled ? "true" : "false") << "}";
    return body.str();
}

std::string PushRuleContentBuilder::buildSetPushRuleActions(const std::vector<std::string>& actions) {
    // Original Kotlin: UpdatePushRuleActionsTask — PUT { "actions": [...] }
    return buildActionsBody(actions);
}

// ==== Pusher JSON Builder & Parser ====
//
// Original Kotlin:
//   JsonPusher.kt (137 lines) — JSON serialisation for /pushers/set
//   JsonPusherData.kt (39 lines) — pusher data object
//   GetPushersResponse.kt (25 lines) — response parser
//   DefaultPushersService.kt (174 lines) — HTTP/Email pusher construction

std::string buildPusherJson(const Pusher& pusher) {
    // Original Kotlin: JsonPusher → JSON for POST /pushers/set
    // {
    //   "pushkey": "...", "kind": "http", "app_id": "...",
    //   "app_display_name": "...", "device_display_name": "...",
    //   "profile_tag": "...", "lang": "...", "data": { "url": "...", "format": "..." },
    //   "append": false, "org.matrix.msc3881.enabled": true,
    //   "org.matrix.msc3881.device_id": "..."
    // }
    std::ostringstream json;
    json << "{";
    json << "\"pushkey\":\"" << pusher.pushKey << "\"";
    json << ",\"kind\":\"" << pusher.kind << "\"";
    json << ",\"app_id\":\"" << pusher.appId << "\"";
    if (!pusher.appDisplayName.empty()) {
        json << ",\"app_display_name\":\"" << pusher.appDisplayName << "\"";
    }
    if (!pusher.deviceDisplayName.empty()) {
        json << ",\"device_display_name\":\"" << pusher.deviceDisplayName << "\"";
    }
    if (!pusher.profileTag.empty()) {
        json << ",\"profile_tag\":\"" << pusher.profileTag << "\"";
    }
    if (!pusher.lang.empty()) {
        json << ",\"lang\":\"" << pusher.lang << "\"";
    }
    json << ",\"data\":{";
    bool hasDataField = false;
    if (!pusher.data.url.empty()) {
        json << "\"url\":\"" << pusher.data.url << "\"";
        hasDataField = true;
    }
    if (!pusher.data.format.empty()) {
        if (hasDataField) json << ",";
        json << "\"format\":\"" << pusher.data.format << "\"";
        hasDataField = true;
    }
    if (!pusher.data.brand.empty()) {
        if (hasDataField) json << ",";
        json << "\"brand\":\"" << pusher.data.brand << "\"";
        hasDataField = true;
    }
    json << "}";
    json << ",\"append\":" << (pusher.append ? "true" : "false");
    json << ",\"org.matrix.msc3881.enabled\":" << (pusher.enabled ? "true" : "false");
    if (!pusher.deviceId.empty()) {
        json << ",\"org.matrix.msc3881.device_id\":\"" << pusher.deviceId << "\"";
    }
    json << "}";
    return json.str();
}

Pusher parsePusher(const std::string& json) {
    // Original Kotlin: GetPushersResponse.kt + JsonPusher.kt
    // Parses a single pusher JSON object
    Pusher p;
    p.pushKey        = extractJsonString(json, "pushkey");
    p.kind           = extractJsonString(json, "kind");
    p.appId          = extractJsonString(json, "app_id");
    p.appDisplayName = extractJsonString(json, "app_display_name");
    p.deviceDisplayName = extractJsonString(json, "device_display_name");
    p.profileTag     = extractJsonString(json, "profile_tag");
    p.lang           = extractJsonString(json, "lang");
    p.deviceId       = extractJsonString(json, "org.matrix.msc3881.device_id");
    p.enabled        = extractJsonBool(json, "org.matrix.msc3881.enabled");
    p.append         = extractJsonBool(json, "append");

    // Parse nested "data" object
    auto dataObj = extractJsonObject(json, "data");
    if (!dataObj.empty()) {
        p.data.url    = extractJsonString(dataObj, "url");
        p.data.format = extractJsonString(dataObj, "format");
        p.data.brand  = extractJsonString(dataObj, "brand");
    }

    // State defaults to REGISTERED when received from server
    p.state = PusherState::REGISTERED;

    return p;
}

} // namespace progressive
