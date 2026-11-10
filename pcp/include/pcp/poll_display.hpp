#pragma once
#include <string>
#include <cstdint>

std::string id;             // "option_1"(const std::string& json);
std::string text;(const std::string& json);
std::string pollId;(const std::string& json);
std::string question;(const std::string& json);
std::string winnerText;     // "Option A won"(const std::string& json);
std::string PollDisplayInfo parsePollForDisplay(const std(const std::string& json);
std::string string& pollStartJson,(const std::string& json);
std::string const std(const std::string& json);
std::string string& pollResponseJson = "",(const std::string& json);
std::string const std(const std::string& json);
std::string string& myUserId = "");(const std::string& json);
std::string formatPollResults(const PollDisplayInfo& poll);(const std::string& json);
std::string formatPollBar(const std(const std::string& json);
std::string string& text, int votes, int total, double pct, int barWidth = 20);(const std::string& json);
std::string getPollWinner(const PollDisplayInfo& poll);(const std::string& json);
std::string bool isPollEnded(const std(const std::string& json);
std::string string& pollEndJson);(const std::string& json);
