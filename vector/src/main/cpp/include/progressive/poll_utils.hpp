#ifndef PROGRESSIVE_POLL_UTILS_HPP
#define PROGRESSIVE_POLL_UTILS_HPP

#include <string>
#include <vector>

namespace progressive {

struct PollOption {
    std::string id;            // option UUID
    std::string text;          // "Option A"
    int voteCount = 0;
    bool isWinner = false;
    double percentage = 0.0;   // 0.0-100.0
};

struct PollResult {
    std::string question;
    std::vector<PollOption> options;
    int totalVotes = 0;
    bool isEnded = false;
    bool isClosed = false;     // undisclosed until ended
    std::string winnerId;
    std::string winnerText;
};

// Compute poll results from raw vote data.
PollResult computePollResults(
    const std::string& question,
    const std::vector<std::string>& optionIds,
    const std::vector<std::string>& optionTexts,
    const std::vector<std::vector<std::string>>& votes // per-option: list of voter IDs
);

// Check if a poll has ended (based on close timestamp).
bool isPollEnded(int64_t closeTimestampMs);

// Determine the winner(s) of a poll.
std::vector<const PollOption*> findWinners(const std::vector<PollOption>& options);

// Format poll as a Matrix message (plain text).
std::string formatPollAsText(const PollResult& result);

// Format poll as HTML.
std::string formatPollAsHtml(const PollResult& result);

// Format poll results as JSON.
std::string pollResultToJson(const PollResult& result);

// Validate a poll question (1-200 characters).
bool isValidPollQuestion(const std::string& question);

// Validate poll options (2-20 options, 1-100 chars each).
bool isValidPollOptions(const std::vector<std::string>& options);

// Generate a random poll option ID.
std::string generatePollOptionId();

// Check if user has already voted.
bool hasVoted(const std::string& userId, const std::vector<std::string>& optionVoters);

} // namespace progressive

#endif // PROGRESSIVE_POLL_UTILS_HPP
