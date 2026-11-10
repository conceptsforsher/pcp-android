#pragma once
#include <string>
#include <cstdint>

std::string userId;(const std::string& json);
std::string displayName;(const std::string& json);
std::string avatarUrl;(const std::string& json);
std::string membership;     // "join", "invite", "leave", "ban", "knock"(const std::string& json);
std::string reason;         // reason for membership change(const std::string& json);
std::string nextBatch;      // pagination token(const std::string& json);
std::string MemberInfo parseMemberEvent(const std(const std::string& json);
std::string string& userId, const std(const std::string& json);
std::string string& eventJson,(const std::string& json);
std::string const std(const std::string& json);
std::string string& prevContentJson = "");(const std::string& json);
std::string buildMemberContent(const MemberInfo& member, const std(const std::string& json);
std::string string& reason = "");(const std::string& json);
std::string MemberList parseMemberList(const std::string& json);
std::string const std(const std::string& json);
std::string string& membership = "",(const std::string& json);
std::string const std(const std::string& json);
std::string string& nameQuery = "",(const std::string& json);
std::string formatMemberDisplayName(const MemberInfo& member);(const std::string& json);
std::string memberListToJson(const MemberList& list);(const std::string& json);
std::string memberToJson(const MemberInfo& member);(const std::string& json);
