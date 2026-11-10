#pragma once
#include <string>
#include <cstdint>

std::string ruleId;(const std::string& json);
std::string kind;           // "override", "underride", "content", "sender", "room"(const std::string& json);
std::string pattern;(const std::string& json);
std::string action;         // "notify", "dont_notify", "coalesce"(const std::string& json);
std::string soundName;(const std::string& json);
std::string std(const std::string& json);
std::string vector<PushRule> parsePushRules(const std::string& json);
std::string buildPushRuleContent(const std(const std::string& json);
std::string string& pattern, const std(const std::string& json);
std::string string& action,(const std::string& json);
std::string bool highlight, const std(const std::string& json);
std::string string& sound = "");(const std::string& json);
std::string bool matchesPushRule(const PushRule& rule, const std(const std::string& json);
std::string string& eventContent,(const std::string& json);
std::string const std(const std::string& json);
std::string string& senderId, const std(const std::string& json);
std::string string& roomId);(const std::string& json);
std::string getDefaultPushAction(bool isDirect, bool isMuted, bool hasHighlight);(const std::string& json);
