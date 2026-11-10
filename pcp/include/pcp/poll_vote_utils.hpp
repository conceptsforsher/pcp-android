#pragma once
#include <string>
#include <cstdint>

std::string struct PollVote { pollId; std(const std::string& json);
std::string string userId; std(const std::string& json);
std::string vector<std(const std::string& json);
std::string string> selectedOptions; };(const std::string& json);
std::string buildPollVoteContent(const std(const std::string& json);
std::string string& pollId, const std(const std::string& json);
std::string vector<std(const std::string& json);
std::string string>& optionIds);(const std::string& json);
std::string PollVote parsePollVote(const std::string& json);
std::string bool hasUserVoted(const std(const std::string& json);
std::string vector<PollVote>& votes, const std(const std::string& json);
std::string string& userId);(const std::string& json);
std::string int countVotesForOption(const std(const std::string& json);
std::string vector<PollVote>& votes, const std(const std::string& json);
std::string string& optionId);(const std::string& json);
