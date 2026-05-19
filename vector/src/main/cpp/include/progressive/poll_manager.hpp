#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>

namespace progressive {

// ================================================================
// Poll Manager — full Matrix poll lifecycle
//
// Ported from Element Android:
//   PollManager.kt, PollCreateViewModel.kt, PollVoteHandler.kt
//   PollResponse.kt, PollEnd.kt
//
// MSC3381: Extensible Events — Polls
//   m.poll.start (unstable: org.matrix.msc3381.poll.start)
//   m.poll.response (unstable: org.matrix.msc3381.poll.response)
//   m.poll.end (unstable: org.matrix.msc3381.poll.end)
// ================================================================

// ---- Poll Kind ----

enum class PollKind {
    DISCLOSED = 0,        // Votes are visible during polling
    UNDISCLOSED = 1,      // Votes hidden until poll ends
};

using PollType = PollKind;  // alias for JNI compat

// ---- Poll Option ----

struct PollOptionFull {
    std::string id;            // "option_1", "option_2", etc.
    std::string text;          // Option text (max 340 chars)
    int voteCount = 0;         // Number of votes for this option
    double percentage = 0.0;   // % of total votes
    std::vector<std::string> voterIds; // Who voted for this option
};

// ---- Poll Content (m.poll.start) ----

struct PollContent {
    std::string pollId;                // Unique poll ID
    std::string question;              // Poll question text
    std::vector<PollOptionFull> options;   // 2-20 options
    PollKind kind = PollKind::DISCLOSED;
    int maxSelections = 1;             // How many options can be selected (default: 1)
    int64_t createdAtMs = 0;
    int64_t closedAtMs = 0;            // When poll was closed (0 = still open)
    bool unstable = false;             // Uses unstable prefix (msc3388)
    bool valid = false;

    // Derived
    int totalVotes() const;
    bool isClosed() const { return closedAtMs > 0; }
    int voterCount() const;            // Unique voters
};

// ---- Poll Vote (m.poll.response) ----

struct PollVote {
    std::string pollId;
    std::string voterId;
    std::string voterName;
    std::vector<std::string> selectedOptionIds; // Which options user selected
    int64_t timestampMs = 0;
    bool valid = false;
};

// ---- Poll End (m.poll.end) ----

struct PollEnd {
    std::string pollId;
    int64_t closedAtMs = 0;
    std::string reasonText;            // Optional reason for closing
};

// ---- Poll Result ----

struct PollResultFull {
    PollContent poll;
    std::vector<PollOptionFull> results;      // Options with vote counts
    int totalVotes = 0;
    int totalVoters = 0;
    std::string myVote;                   // Which option(s) current user voted for
    bool hasVoted = false;
    bool isClosed = false;
};

// ---- Poll Event Formatting ----

struct PollEventDisplay {
    std::string question;
    std::string plainText;               // "Question?\nOption 1: 5 votes (50%)\nOption 2: 3 votes (30%)..."
    std::string htmlBody;                // Rich HTML with progress bars
    int totalVotes = 0;
    bool isClosed = false;
    std::string closedBy;
    int winnerOption = -1;               // Index of winning option (or -1)
};

// ---- Poll Manager ----

class PollManager {
public:
    PollManager();

    // ====== Poll Creation ======

    // Build m.poll.start event content.
    // Validation: question non-empty, 2-20 options, each option 1-340 chars.
    std::string buildPollStartContent(const std::string& question,
                                       const std::vector<std::string>& optionTexts,
                                       PollKind kind, int maxSelections,
                                       bool unstable, std::string& error);

    // Parse m.poll.start content.
    PollContent parsePollStartContent(const std::string& contentJson, bool unstable);

    // Validate poll configuration.
    bool isValidPollQuestion(const std::string& question);
    bool isValidPollOption(const std::string& text);
    bool isValidMaxSelections(int selections, int optionCount);

    // ====== Vote Casting ======

    // Build m.poll.response event content.
    std::string buildPollResponseContent(const std::string& pollId,
                                          const std::vector<std::string>& selectedOptionIds,
                                          bool unstable);

    // Parse m.poll.response content.
    PollVote parsePollResponseContent(const std::string& contentJson, const std::string& voterId,
                                       const std::string& voterName, bool unstable);

    // ====== Poll Ending ======

    // Build m.poll.end event content.
    std::string buildPollEndContent(const std::string& pollId, const std::string& reason, bool unstable);

    // Parse m.poll.end content.
    PollEnd parsePollEndContent(const std::string& contentJson, bool unstable);

    // ====== Vote Tallying ======

    // Tally all votes for a poll (from stored vote events).
    // Takes the poll content and a list of all votes.
    PollResultFull tallyVotes(const PollContent& poll, const std::vector<PollVote>& votes);

    // Set which option the current user voted for.
    void setMyVote(PollResultFull& result, const std::string& userId);

    // ====== Display Formatting ======

    // Format poll results for timeline display.
    PollEventDisplay formatPollEvent(const PollResultFull& result);

    // Format as plain text.
    std::string formatPollPlainText(const PollEventDisplay& display);

    // Format as rich HTML (with colored progress bars).
    std::string formatPollHtml(const PollEventDisplay& display);

    // Get winner option text (or "Tie").
    std::string getWinnerText(const PollResultFull& result) const;

    // ====== Poll State ======

    // Check if a poll event type string is a poll event.
    bool isPollEvent(const std::string& eventType) const;

    // Get poll event type description.
    std::string getPollEventDescription(const std::string& eventType) const;

private:
    // Generate unique poll ID.
    std::string generatePollId() const;

    // Generate option IDs (option_1, option_2, ...).
    std::string optionIdFromIndex(int index) const;
};

inline const char* pollTypeToString(PollType t) {
    switch (t) {
        case PollType::DISCLOSED: return "disclosed";
        case PollType::UNDISCLOSED: return "undisclosed";
    }
    return "unknown";
}

inline PollType pollTypeFromString(const std::string& s) {
    if (s == "undisclosed") return PollType::UNDISCLOSED;
    return PollType::DISCLOSED;
}

} // namespace progressive
