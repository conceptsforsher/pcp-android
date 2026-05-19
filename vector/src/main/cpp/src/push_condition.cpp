#include "progressive/push_condition.hpp"
#include <sstream>
#include <cctype>

namespace progressive {

// ==== Glob to Regex (from Glob.kt:24-38) ====
// Original Kotlin:
//   fun String.simpleGlobToRegExp(): String = buildString {
//       forEach { char -> when (char) {
//           '*' -> append(".*")
//           '?' -> append(".")
//           '.' -> append("\\.")
//           '\\' -> append("\\\\")
//           else -> append(char)
//       }}
//   }

std::string globToRegex(const std::string& glob) {
    std::ostringstream regex;
    for (char c : glob) {
        switch (c) {
            case '*': regex << ".*"; break;
            case '?': regex << "."; break;
            case '.': regex << "\\."; break;
            case '\\': regex << "\\\\"; break;
            default: regex << c; break;
        }
    }
    return regex.str();
}

bool hasSpecialGlobChar(const std::string& pattern) {
    return pattern.find('*') != std::string::npos || pattern.find('?') != std::string::npos;
}

// ==== JSON Field Extraction (from EventMatchCondition.kt:86-104) ====
// Original Kotlin:
//   private fun extractField(jsonObject: Map<*, *>, fieldPath: String): String? {
//       val fieldParts = fieldPath.split(".")
//       var jsonElement: Map<*, *> = jsonObject
//       fieldParts.forEachIndexed { index, pathSegment ->
//           if (index == fieldParts.lastIndex) return jsonElement[pathSegment]?.toString()
//           else jsonElement = jsonElement[pathSegment] as? Map<*, *> ?: return null
//       }
//   }

std::string extractJsonField(const std::string& json, const std::string& fieldPath) {
    if (fieldPath.empty()) return "";

    // Split field path by "."
    std::vector<std::string> parts;
    {
        size_t start = 0;
        while (start < fieldPath.size()) {
            auto dot = fieldPath.find('.', start);
            if (dot == std::string::npos) {
                parts.push_back(fieldPath.substr(start));
                break;
            }
            parts.push_back(fieldPath.substr(start, dot - start));
            start = dot + 1;
        }
    }
    if (parts.empty()) return "";

    // Navigate JSON structure
    size_t searchStart = 0;
    for (size_t i = 0; i < parts.size(); ++i) {
        const auto& part = parts[i];
        bool isLast = (i == parts.size() - 1);

        // Find the key in current scope
        std::string keySearch = "\"" + part + "\":";
        auto keyPos = json.find(keySearch, searchStart);
        if (keyPos == std::string::npos) {
            keySearch = "\"" + part + "\": ";
            keyPos = json.find(keySearch, searchStart);
        }
        if (keyPos == std::string::npos) return "";

        size_t valueStart = keyPos + keySearch.size();

        if (isLast) {
            // Last part — extract string value
            while (valueStart < json.size() && json[valueStart] == ' ') valueStart++;
            if (valueStart < json.size() && json[valueStart] == '"') {
                // String value
                valueStart++;
                auto end = json.find('"', valueStart);
                if (end != std::string::npos) return json.substr(valueStart, end - valueStart);
            } else {
                // Numeric or boolean value — read until comma or brace
                size_t end = valueStart;
                while (end < json.size() && json[end] != ',' && json[end] != '}' && json[end] != ']') end++;
                return json.substr(valueStart, end - valueStart);
            }
            return "";
        } else {
            // Navigate into nested object
            while (valueStart < json.size() && json[valueStart] != '{') valueStart++;
            if (valueStart >= json.size()) return "";
            searchStart = valueStart + 1;
        }
    }
    return "";
}

// ==== Simple Regex Match ====
// Implements basic regex matching without external library.
// Supports: . (any char), .* (any sequence), \\ (escaped chars), literal chars.
// Case-insensitive.

static bool regexMatchImpl(const std::string& text, size_t ti, const std::string& pattern, size_t pi) {
    if (pi >= pattern.size()) return ti >= text.size();

    // Handle .* — greedy match
    if (pi + 1 < pattern.size() && pattern[pi] == '.' && pattern[pi + 1] == '*') {
        // Try matching from current position forward
        for (size_t k = ti; k <= text.size(); ++k) {
            if (regexMatchImpl(text, k, pattern, pi + 2)) return true;
        }
        return false;
    }

    // Handle escape sequences
    if (pattern[pi] == '\\' && pi + 1 < pattern.size()) {
        if (ti < text.size() && std::tolower(static_cast<unsigned char>(text[ti])) ==
            std::tolower(static_cast<unsigned char>(pattern[pi + 1]))) {
            return regexMatchImpl(text, ti + 1, pattern, pi + 2);
        }
        return false;
    }

    // Handle . (any char)
    if (pattern[pi] == '.') {
        return ti < text.size() && regexMatchImpl(text, ti + 1, pattern, pi + 1);
    }

    // Literal match (case-insensitive)
    if (ti < text.size() && std::tolower(static_cast<unsigned char>(text[ti])) ==
        std::tolower(static_cast<unsigned char>(pattern[pi]))) {
        return regexMatchImpl(text, ti + 1, pattern, pi + 1);
    }

    return false;
}

bool simpleRegexMatch(const std::string& text, const std::string& regexPattern) {
    return regexMatchImpl(text, 0, regexPattern, 0);
}

bool simpleRegexContainsMatch(const std::string& text, const std::string& regexPattern) {
    for (size_t i = 0; i <= text.size(); ++i) {
        if (regexMatchImpl(text, i, regexPattern, 0)) return true;
    }
    return false;
}

// ==== Event Match Condition Evaluator (from EventMatchCondition.kt:41-84) ====
// Original Kotlin:
//   fun isSatisfied(event: Event): Boolean {
//       val value = extractField(rawJson, key) ?: return false
//       return if (key == "content.body") {
//           val modPattern = if (pattern.startsWith("*") && pattern.endsWith("*"))
//               pattern.removePrefix("*").removeSuffix("*").simpleGlobToRegExp()
//           else "(\\W|^)" + pattern.simpleGlobToRegExp() + "(\\W|$)"
//           regex.containsMatchIn(value)
//       } else {
//           regex.matches(value)
//       }
//   }

bool evaluateEventMatchCondition(
    const std::string& eventJson,
    const std::string& key,
    const std::string& pattern)
{
    std::string value = extractJsonField(eventJson, key);
    if (value.empty()) return false;

    // Convert glob pattern to regex
    std::string modPattern;

    // Original: special handling for content.body — word boundary matching
    if (key == "content.body") {
        if (!pattern.empty() && pattern.front() == '*' && pattern.back() == '*') {
            // Original: removePrefix("*").removeSuffix("*")
            // Leading/trailing stars don't affect containsMatchIn result
            modPattern = globToRegex(pattern.substr(1, pattern.size() - 2));
        } else {
            // Word boundary: (\\W|^)pattern(\\W|$)
            modPattern = "(\\W|^)" + globToRegex(pattern) + "(\\W|$)";
        }
        // Original: regex.containsMatchIn(value) — match anywhere in the value
        return simpleRegexContainsMatch(value, modPattern);
    } else {
        // Original: regex.matches(value) — match the ENTIRE value
        // For patterns without special glob chars, prepend and append .*
        if (!hasSpecialGlobChar(pattern)) {
            modPattern = ".*" + globToRegex(pattern) + ".*";
        } else {
            modPattern = globToRegex(pattern);
        }
        return simpleRegexMatch(value, modPattern);
    }
}

// ==== Generic Push Condition Evaluator ====

EventPushCondition evaluateEventPushCondition(
    const EventPushCondition& condition,
    const std::string& eventJson)
{
    EventPushCondition result = condition;

    // Original: when (kind) { "event_match" -> ... }
    if (condition.kind == "event_match") {
        result.isSatisfied = evaluateEventMatchCondition(eventJson, condition.key, condition.pattern);
    }
    // Other condition kinds (room_member_count, sender_notification_permission)
    // require server-side evaluation or room state — not evaluable from event alone.

    return result;
}

std::string pushConditionToJson(const EventPushCondition& condition) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"kind": ")" << esc(condition.kind) << R"(",)";
    json << R"("key": ")" << esc(condition.key) << R"(",)";
    json << R"("pattern": ")" << esc(condition.pattern) << R"(",)";
    json << R"("isSatisfied": )" << (condition.isSatisfied ? "true" : "false") << "}";
    return json.str();
}

// ==== Contains Display Name Condition (from ContainsDisplayNameCondition.kt:32-46) ====
// Original Kotlin:
//   fun isSatisfied(event: Event, displayName: String): Boolean {
//       val message = when (event.type) { EventType.MESSAGE -> event.content.toModel<MessageContent>() else -> null } ?: return false
//       return message.body.caseInsensitiveFind(displayName)
//   }

bool caseInsensitiveFind(const std::string& text, const std::string& search) {
    if (search.empty()) return false;
    if (text.size() < search.size()) return false;

    // Case-insensitive substring search
    for (size_t i = 0; i <= text.size() - search.size(); ++i) {
        bool match = true;
        for (size_t j = 0; j < search.size(); ++j) {
            if (std::tolower(static_cast<unsigned char>(text[i + j])) !=
                std::tolower(static_cast<unsigned char>(search[j]))) {
                match = false;
                break;
            }
        }
        if (match) return true;
    }
    return false;
}

bool evaluateDisplayNameCondition(const std::string& eventJson, const std::string& displayName) {
    if (displayName.empty()) return false;

    // Extract msgtype to verify it's a message
    // Original: when (event.type) { EventType.MESSAGE -> ... else -> null }
    // We check for "msgtype" field which indicates a message event
    if (eventJson.find("\"msgtype\"") == std::string::npos) return false;

    // Extract the body field
    std::string body = extractJsonField(eventJson, "content.body");
    if (body.empty()) return false;

    // Original: message.body.caseInsensitiveFind(displayName)
    return caseInsensitiveFind(body, displayName);
}

// ==== Push Rule & Rule Set (from EventPushRule.kt:62-142 + RuleSet.kt:39-70) ====

EventPushRule parseEventPushRule(const std::string& json) {
    EventPushRule rule;

    auto extractStr = [&](const std::string& key) -> std::string {
        auto search = "\"" + key + "\":\"";
        auto pos = json.find(search);
        if (pos == std::string::npos) {
            search = "\"" + key + "\": \"";
            pos = json.find(search);
        }
        if (pos == std::string::npos) return "";
        pos += search.size();
        auto end = json.find('"', pos);
        return (end != std::string::npos) ? json.substr(pos, end - pos) : "";
    };

    rule.ruleId = extractStr("rule_id");
    rule.pattern = extractStr("pattern");
    rule.enabled = json.find("\"enabled\": false") == std::string::npos;
    rule.isDefault = json.find("\"default\": true") != std::string::npos;

    // Parse actions array
    // Original: actions: List<Any> — can be strings or Action objects
    auto actsPos = json.find("\"actions\"");
    if (actsPos != std::string::npos) {
        auto bracket = json.find('[', actsPos);
        if (bracket != std::string::npos) {
            size_t pos = bracket + 1;
            while (pos < json.size()) {
                if (json[pos] == '"') {
                    size_t end = json.find('"', pos + 1);
                    if (end != std::string::npos) {
                        std::string action = json.substr(pos + 1, end - pos - 1);
                        rule.actions.push_back(action);
                        pos = end + 1;
                        continue;
                    }
                }
                if (json[pos] == ']') break;
                pos++;
            }
        }
    }

    // Determine notify/highlight from actions
    rule.shouldNotify = true;
    for (const auto& a : rule.actions) {
        if (a == "dont_notify") rule.shouldNotify = false;
        if (a == "notify") rule.shouldNotify = true;
        if (a == "highlight") rule.shouldHighlight = true;
        if (a == "sound") {
            // Sound value is in a separate object — skip complex parsing
        }
    }

    return rule;
}

EventPushRule setEventPushRuleSound(const EventPushRule& rule, const std::string& sound) {
    EventPushRule updated = rule;
    updated.notificationSound = sound;
    return updated;
}

EventPushRule setEventPushRuleHighlight(const EventPushRule& rule, bool highlight) {
    EventPushRule updated = rule;
    updated.shouldHighlight = highlight;
    return updated;
}

EventPushRule setEventPushRuleNotify(const EventPushRule& rule, bool notify) {
    // Original: if (notify) add ACTION_NOTIFY, remove ACTION_DONT_NOTIFY
    EventPushRule updated = rule;
    updated.shouldNotify = notify;
    // Remove existing notify/dont_notify
    std::vector<std::string> newActions;
    for (const auto& a : updated.actions) {
        if (a != "notify" && a != "dont_notify") newActions.push_back(a);
    }
    if (notify) newActions.push_back("notify");
    updated.actions = newActions;
    return updated;
}

std::string pushRuleToJson(const EventPushRule& rule) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"ruleId": ")" << esc(rule.ruleId) << R"(",)";
    json << R"("enabled": )" << (rule.enabled ? "true" : "false") << ",";
    json << R"("isDefault": )" << (rule.isDefault ? "true" : "false") << ",";
    json << R"("shouldNotify": )" << (rule.shouldNotify ? "true" : "false") << ",";
    json << R"("shouldHighlight": )" << (rule.shouldHighlight ? "true" : "false") << ",";
    json << R"("notificationSound": ")" << esc(rule.notificationSound) << R"(")";
    json << "}";
    return json.str();
}

} // namespace progressive
