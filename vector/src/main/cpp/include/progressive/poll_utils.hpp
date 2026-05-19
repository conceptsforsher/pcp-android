#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include <functional>

namespace progressive {

// ================================================================
// Protocol constants — MSC3381 Extensible Events: Polls
// ================================================================

constexpr const char* POLL_START_STABLE                = "m.poll.start";
constexpr const char* POLL_START_UNSTABLE              = "org.matrix.msc3381.poll.start";
constexpr const char* POLL_RESPONSE_STABLE             = "m.poll.response";
constexpr const char* POLL_RESPONSE_UNSTABLE           = "org.matrix.msc3381.poll.response";
constexpr const char* POLL_END_STABLE                  = "m.poll.end";
constexpr const char* POLL_END_UNSTABLE                = "org.matrix.msc3381.poll.end";

constexpr const char* POLL_KIND_DISCLOSED_STABLE       = "m.poll.disclosed";
constexpr const char* POLL_KIND_UNDISCLOSED_STABLE     = "m.poll.undisclosed";
constexpr const char* POLL_KIND_DISCLOSED_UNSTABLE     = "org.matrix.msc3381.poll.disclosed";
constexpr const char* POLL_KIND_UNDISCLOSED_UNSTABLE   = "org.matrix.msc3381.poll.undisclosed";

constexpr const char* POLL_MSG_TYPE                   = "m.text";
constexpr const char* POLL_TEXT_KEY_STABLE             = "m.text";
constexpr const char* POLL_TEXT_KEY_UNSTABLE           = "org.matrix.msc1767.text";

// ================================================================
// Poll data models — ported from Element Android SDK
// ================================================================

// ---- PollType ----
// Original Kotlin: PollType.kt
enum class PollType {
    DISCLOSED = 0,
    UNDISCLOSED = 1
};

// ---- PollAnswer ----
// Original Kotlin: PollAnswer.kt
struct PollAnswer {
    std::string id;                         // option UUID
    std::string text;                       // "m.text" — stable
    std::string unstableText;               // "org.matrix.msc1767.text" — unstable

    std::string getBestText() const {
        // Original Kotlin: return answer ?: unstableAnswer
        return text.empty() ? unstableText : text;
    }
};

// ---- PollQuestion ----
// Original Kotlin: PollQuestion.kt
struct PollQuestion {
    std::string text;                       // "m.text" — stable
    std::string unstableText;               // "org.matrix.msc1767.text" — unstable

    std::string getBestText() const {
        // Original Kotlin: return question ?: unstableQuestion
        return text.empty() ? unstableText : text;
    }
};

// Original Kotlin: PollType.kt — string conversion helpers
inline const char* pollTypeToString(PollType t) {
    switch (t) {
        case PollType::DISCLOSED:    return "disclosed";
        case PollType::UNDISCLOSED:  return "undisclosed";
    }
    return "disclosed";
}

inline PollType pollTypeFromString(const std::string& s) {
    // Original Kotlin: kind in listOf(UNDISCLOSED_UNSTABLE, UNDISCLOSED)
    if (s == "undisclosed" || s == "m.poll.undisclosed" || s == "org.matrix.msc3381.poll.undisclosed")
        return PollType::UNDISCLOSED;
    return PollType::DISCLOSED;
}

inline const char* pollTypeToStableString(PollType t) {
    switch (t) {
        case PollType::DISCLOSED:    return POLL_KIND_DISCLOSED_STABLE;
        case PollType::UNDISCLOSED:  return POLL_KIND_UNDISCLOSED_STABLE;
    }
    return POLL_KIND_DISCLOSED_STABLE;
}

inline const char* pollTypeToUnstableString(PollType t) {
    switch (t) {
        case PollType::DISCLOSED:    return POLL_KIND_DISCLOSED_UNSTABLE;
        case PollType::UNDISCLOSED:  return POLL_KIND_UNDISCLOSED_UNSTABLE;
    }
    return POLL_KIND_DISCLOSED_UNSTABLE;
}

// ---- PollCreationInfo ----
// Original Kotlin: PollCreationInfo.kt
struct PollCreationInfo {
    PollQuestion question;
    std::vector<PollAnswer> answers;
    int maxSelections = 1;
    PollType kind = PollType::DISCLOSED;

    // Original Kotlin: fun isUndisclosed()
    bool isUndisclosed() const { return kind == PollType::UNDISCLOSED; }
};

// ---- PollResponse ----
// Original Kotlin: PollResponse.kt
struct PollResponse {
    std::vector<std::string> answers;       // selected option IDs ("answers" array in JSON)
};

// ---- VoteInfo ----
// Original Kotlin: PollSummaryContent.VoteInfo
struct VoteInfo {
    std::string userId;
    std::string option;                     // selected answer ID
    int64_t voteTimestamp = 0;
};

// ---- VoteSummary ----
// Original Kotlin: PollSummaryContent.VoteSummary
struct VoteSummary {
    int total = 0;
    double percentage = 0.0;                // 0.0 – 1.0
};

// ---- PollSummaryContent ----
// Original Kotlin: PollSummaryContent.kt
struct PollSummaryContent {
    std::string myVote;                     // current user's selected option ID
    std::vector<VoteInfo> votes;            // one entry per voter (latest vote wins)
    std::unordered_map<std::string, VoteSummary> votesSummary;  // optionId -> summary
    int totalVotes = 0;                     // total unique voters
    int winnerVoteCount = 0;                // max votes for any option
};

// ---- PollResponseAggregatedSummary ----
// Original Kotlin: PollResponseAggregatedSummary.kt
struct PollResponseAggregatedSummary {
    PollSummaryContent aggregatedContent;
    int64_t closedTime = 0;                 // 0 = not closed (or null)
    int nbOptions = 0;
    std::vector<std::string> sourceEvents;
    std::vector<std::string> localEchos;
    std::vector<std::string> encryptedRelatedEventIds;

    // Original Kotlin: closedTime != null means closed
    bool isClosed() const { return closedTime > 0; }
};

// ---- MessagePollContent ----
// Original Kotlin: MessagePollContent.kt
struct MessagePollContent {
    std::string body;                               // fallback plain-text body
    PollCreationInfo creationInfo;                  // stable: "m.poll.start"
    PollCreationInfo unstableCreationInfo;          // unstable: "org.matrix.msc3381.poll.start"
    std::string relatesTo;                          // "m.relates_to".eventId

    // Original Kotlin: getBestPollCreationInfo()
    const PollCreationInfo& getBestCreationInfo() const {
        return creationInfo.question.getBestText().empty()
                   ? unstableCreationInfo
                   : creationInfo;
    }

    bool isUndisclosed() const {
        return getBestCreationInfo().isUndisclosed();
    }
};

// ---- MessagePollResponseContent ----
// Original Kotlin: MessagePollResponseContent.kt
struct MessagePollResponseContent {
    std::string body;                               // fallback plain-text body
    std::vector<std::string> answers;               // selected option IDs
    std::string relatesTo;                          // "m.relates_to".eventId
};

// ---- MessageEndPollContent ----
// Original Kotlin: MessageEndPollContent.kt
struct MessageEndPollContent {
    std::string body;                               // fallback plain-text body
    std::string text;                               // "m.text" — stable close reason
    std::string unstableText;                       // "org.matrix.msc1767.text" — unstable
    std::string relatesTo;                          // "m.relates_to".eventId

    // Original Kotlin: getBestText()
    std::string getBestText() const {
        return text.empty() ? unstableText : text;
    }
};

// ================================================================
// Legacy structures (retained for backward compatibility)
// ================================================================

struct PollOption {
    std::string id;            // option UUID
    std::string text;          // "Option A"
    int voteCount = 0;
    bool isWinner = false;
    double percentage = 0.0;  // 0.0-100.0
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

// ================================================================
// Legacy functions
// ================================================================

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

// ================================================================
// Poll builders — build MSC3381 poll event JSON content
// ================================================================

// Original Kotlin: PollCreateViewModel.kt — buildPollStartEvent()
// Builds JSON content for an m.poll.start event.
// Format: {"m.poll.start": {"question": {...}, "kind": "...", "max_selections": N, "answers": [...]}}
std::string buildPollStartContent(
    const std::string& question,
    const std::vector<std::string>& optionTexts,
    PollType kind = PollType::DISCLOSED,
    int maxSelections = 1,
    bool unstable = false,
    std::string* error = nullptr
);

// Original Kotlin: PollVoteHandler.kt — buildPollResponseEvent()
// Builds JSON content for an m.poll.response event.
// Format: {"m.poll.response": {"answers": ["id1", "id2", ...]}}
std::string buildPollResponseContent(
    const std::vector<std::string>& selectedOptionIds,
    const std::string& relatesToEventId = "",
    bool unstable = false
);

// Original Kotlin: PollEndEvent.kt — buildPollEndEvent()
// Builds JSON content for an m.poll.end event.
// Format: {"m.poll.end": {}}
std::string buildPollEndContent(
    const std::string& relatesToEventId,
    const std::string& reasonText = "",
    bool unstable = false
);

// ================================================================
// Poll parsers — parse MSC3381 poll event JSON content
// ================================================================

// Original Kotlin: MessagePollContent.kt — parse poll start
MessagePollContent parsePollContent(const std::string& contentJson);

// Original Kotlin: MessagePollResponseContent.kt — parse vote
MessagePollResponseContent parsePollResponseContent(const std::string& contentJson);

// Original Kotlin: MessageEndPollContent.kt — parse end event
MessageEndPollContent parsePollEndContent(const std::string& contentJson);

// ================================================================
// Poll aggregation — compute results from accumulated responses
// ================================================================

// Original Kotlin: DefaultPollAggregationProcessor.kt — calculate vote totals
// Takes the poll answers and all user votes (each entry = list of selected option IDs for one voter).
// Optionally sets myVote for the specified user.
PollSummaryContent aggregatePollResults(
    const std::vector<PollAnswer>& answers,
    const std::vector<VoteInfo>& allVotes,
    const std::string& myUserId
);

// Original Kotlin: compute winner from votesSummary
int computePollWinnerCount(const PollSummaryContent& summary);

// Original Kotlin: PollResponseAggregatedSummary.isClosed()
bool isPollClosed(int64_t closedTimeMs);

// Original Kotlin: format human-readable poll results string
std::string formatPollResults(
    const PollSummaryContent& summary,
    const std::vector<PollAnswer>& answers,
    bool includePercentages = true
);

} // namespace progressive
