#include "pcp/push_condition_utils.hpp"
#include <sstream>
#include <algorithm>

namespace pcp {

PushConditionKind conditionKindFromString(const std::string& s) {
    if (s == "event_match") return PushConditionKind::EVENT_MATCH;
    if (s == "room_member_count") return PushConditionKind::ROOM_MEMBER_COUNT;
    if (s == "contains_display_name") return PushConditionKind::CONTAINS_DISPLAY_NAME;
    return PushConditionKind::UNDEFINED;
}

std::string conditionKindToString(PushConditionKind k) {
    switch (k) {
        case PushConditionKind::EVENT_MATCH: return "event_match";
        case PushConditionKind::ROOM_MEMBER_COUNT: return "room_member_count";
        case PushConditionKind::CONTAINS_DISPLAY_NAME: return "contains_display_name";
        default: return "undefined";
    }
}

PushCondition parsePushCondition(const std::string& json) {
    PushCondition c;
    auto extract = [&](const std::string& key) -> std::string {
        auto p = json.find("\"" + key + "\":\"");
        if (p == std::string::npos) return "";
        p += key.size() + 4;
        auto e = json.find('"', p);
        if (e == std::string::npos) return "";
        return json.substr(p, e - p);
    };
    c.kind = conditionKindFromString(extract("kind"));
    c.key = extract("key");
    c.pattern = extract("pattern");
    c.is = extract("is");
    return c;
}

static bool globMatch(const std::string& pattern, const std::string& text) {
    size_t pi = 0, ti = 0, star = std::string::npos, match = 0;
    while (ti < text.size()) {
        if (pi < pattern.size() && (pattern[pi] == '?' || tolower(pattern[pi]) == tolower(text[ti])))
            { pi++; ti++; }
        else if (pi < pattern.size() && pattern[pi] == '*')
            { star = pi; match = ti; pi++; }
        else if (star != std::string::npos)
            { pi = star + 1; match++; ti = match; }
        else return false;
    }
    while (pi < pattern.size() && pattern[pi] == '*') pi++;
    return pi == pattern.size();
}

bool evaluatePushCondition(const PushCondition& cond, const std::string& content,
                             const std::string& displayName, int memberCount) {
    switch (cond.kind) {
        case PushConditionKind::EVENT_MATCH:
            if (cond.key == "content.body")
                return globMatch(cond.pattern, content);
            return false;
        case PushConditionKind::ROOM_MEMBER_COUNT:
            if (cond.is == "2") return memberCount <= 2;
            if (cond.is == ">2") return memberCount > 2;
            return false;
        case PushConditionKind::CONTAINS_DISPLAY_NAME: {
            std::string lower;
            std::transform(content.begin(), content.end(), std::back_inserter(lower), ::tolower);
            std::string dn;
            std::transform(displayName.begin(), displayName.end(), std::back_inserter(dn), ::tolower);
            return lower.find(dn) != std::string::npos;
        }
        default: return false;
    }
}

std::string buildPushCondition(const PushCondition& cond) {
    std::ostringstream os;
    os << R"({"kind":")" << conditionKindToString(cond.kind) << R"(")";
    if (!cond.key.empty()) os << R"(,"key":")" << cond.key << R"(")";
    if (!cond.pattern.empty()) os << R"(,"pattern":")" << cond.pattern << R"(")";
    if (!cond.is.empty()) os << R"(,"is":")" << cond.is << R"(")";
    os << "}";
    return os.str();
}

std::string formatPushCondition(const PushCondition& cond) {
    std::ostringstream os;
    if (cond.kind == PushConditionKind::CONTAINS_DISPLAY_NAME) return "Mentions me";
    if (cond.kind == PushConditionKind::ROOM_MEMBER_COUNT) return "Small room (" + cond.is + ")";
    if (cond.kind == PushConditionKind::EVENT_MATCH) return "Matches: " + cond.pattern;
    return "Unknown";
}

} // namespace pcp
