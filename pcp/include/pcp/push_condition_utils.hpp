#pragma once
#include <string>
#include <vector>

namespace pcp {

enum class PushConditionKind { EVENT_MATCH, ROOM_MEMBER_COUNT, CONTAINS_DISPLAY_NAME, UNDEFINED };

struct PushCondition {
    PushConditionKind kind = PushConditionKind::UNDEFINED;
    std::string key;            // e.g. "content.body", "room_id"
    std::string pattern;        // glob pattern to match
    std::string is;             // "2" or ">2" or "==2"
};

// Parse push condition from rule JSON
PushCondition parsePushCondition(const std::string& json);

// Check if condition is satisfied
bool evaluatePushCondition(const PushCondition& cond, const std::string& eventContent,
                             const std::string& myDisplayName, int roomMemberCount);

// Build push condition JSON
std::string buildPushCondition(const PushCondition& cond);

// Get condition kind from string
PushConditionKind conditionKindFromString(const std::string& s);
std::string conditionKindToString(PushConditionKind k);

// Format condition for display
std::string formatPushCondition(const PushCondition& cond);

} // namespace pcp
