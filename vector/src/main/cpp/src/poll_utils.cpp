#include "progressive/poll_utils.hpp"
#include <sstream>
#include <algorithm>
#include <chrono>
#include <random>
#include <cmath>

namespace progressive {

// ================================================================
// Internal JSON helpers
// ================================================================

static std::string extractStr(const std::string& json, const std::string& key) {
    auto pp = json.find('"' + key + '"');
    if (pp == std::string::npos) return "";
    pp = json.find(':', pp);
    if (pp == std::string::npos) return "";
    pp++;
    while (pp < json.size() && (json[pp] == ' ' || json[pp] == '\t' || json[pp] == '\n')) pp++;
    if (pp >= json.size() || json[pp] != '"') return "";
    pp++;
    size_t e = pp;
    while (e < json.size() && json[e] != '"') {
        if (json[e] == '\\' && e + 1 < json.size()) e++;
        e++;
    }
    return json.substr(pp, e - pp);
}

static int64_t extractInt(const std::string& json, const std::string& key) {
    auto pp = json.find('"' + key + '"');
    if (pp == std::string::npos) return 0;
    pp = json.find(':', pp);
    if (pp == std::string::npos) return 0;
    pp++;
    while (pp < json.size() && (json[pp] == ' ' || json[pp] == '\t' || json[pp] == '\n')) pp++;
    int64_t v = 0;
    while (pp < json.size() && json[pp] >= '0' && json[pp] <= '9') { v = v * 10 + (json[pp] - '0'); pp++; }
    return v;
}

static std::string htmlEscape(const std::string& s) {
    std::string out;
    out.reserve(s.size());
    for (char c : s) {
        switch (c) {
            case '&':  out += "&amp;"; break;
            case '<':  out += "&lt;"; break;
            case '>':  out += "&gt;"; break;
            case '"':  out += "&quot;"; break;
            case '\'': out += "&#39;"; break;
            default:   out += c; break;
        }
    }
    return out;
}

static std::string jsonEscape(const std::string& s) {
    std::string out;
    out.reserve(s.size());
    for (char c : s) {
        if (c == '"') out += "\\\"";
        else if (c == '\\') out += "\\\\";
        else if (c == '\n') out += "\\n";
        else if (c == '\r') out += "\\r";
        else if (c == '\t') out += "\\t";
        else out += c;
    }
    return out;
}

// Extract a nested JSON object by key and return the inner { ... } string.
static std::string extractObject(const std::string& json, const std::string& key) {
    auto pp = json.find('"' + key + '"');
    if (pp == std::string::npos) return "";
    pp = json.find(':', pp);
    if (pp == std::string::npos) return "";
    pp++;
    while (pp < json.size() && (json[pp] == ' ' || json[pp] == '\t' || json[pp] == '\n')) pp++;
    if (pp >= json.size() || json[pp] != '{') return "";
    size_t start = pp;
    int depth = 0;
    while (pp < json.size()) {
        if (json[pp] == '{') depth++;
        else if (json[pp] == '}') {
            depth--;
            if (depth == 0) return json.substr(start, pp - start + 1);
        }
        pp++;
    }
    return "";
}

// Extract a JSON array by key and return the [ ... ] string.
static std::string extractArray(const std::string& json, const std::string& key) {
    auto pp = json.find('"' + key + '"');
    if (pp == std::string::npos) return "";
    pp = json.find(':', pp);
    if (pp == std::string::npos) return "";
    pp++;
    while (pp < json.size() && (json[pp] == ' ' || json[pp] == '\t' || json[pp] == '\n')) pp++;
    if (pp >= json.size() || json[pp] != '[') return "";
    size_t start = pp;
    int depth = 0;
    while (pp < json.size()) {
        if (json[pp] == '[' || json[pp] == '{') depth++;
        else if (json[pp] == ']' || json[pp] == '}') {
            depth--;
            if (depth == 0) return json.substr(start, pp - start + 1);
        }
        pp++;
    }
    return "";
}

// Parse elements of a JSON array of objects: [{"id":"A","m.text":"Red"}, ...]
// Returns each element's inner { ... } string.
static std::vector<std::string> parseArrayElements(const std::string& arrayJson) {
    std::vector<std::string> out;
    size_t pos = 1; // skip opening '['
    while (pos < arrayJson.size()) {
        // Skip whitespace and comma
        while (pos < arrayJson.size() && (arrayJson[pos] == ' ' || arrayJson[pos] == ',' || arrayJson[pos] == '\t' || arrayJson[pos] == '\n')) pos++;
        if (pos >= arrayJson.size() || arrayJson[pos] == ']') break;

        size_t objStart = pos;
        int depth = 0;
        while (pos < arrayJson.size()) {
            if (arrayJson[pos] == '{') depth++;
            else if (arrayJson[pos] == '}') {
                depth--;
                if (depth == 0) { pos++; break; }
            }
            pos++;
        }
        out.push_back(arrayJson.substr(objStart, pos - objStart));
    }
    return out;
}

// Parse elements of a JSON array of strings: ["id1", "id2", ...]
static std::vector<std::string> parseStringArray(const std::string& arrayJson) {
    std::vector<std::string> out;
    size_t pos = 1; // skip opening '['
    while (pos < arrayJson.size()) {
        while (pos < arrayJson.size() && (arrayJson[pos] == ' ' || arrayJson[pos] == ',' || arrayJson[pos] == '\t' || arrayJson[pos] == '\n')) pos++;
        if (pos >= arrayJson.size() || arrayJson[pos] == ']') break;
        if (arrayJson[pos] == '"') {
            pos++;
            size_t e = pos;
            while (e < arrayJson.size() && arrayJson[e] != '"') e++;
            out.push_back(arrayJson.substr(pos, e - pos));
            pos = e + 1;
        } else {
            pos++;
        }
    }
    return out;
}

// ================================================================
// Legacy functions
// ================================================================

PollResult computePollResults(
    const std::string& question,
    const std::vector<std::string>& optionIds,
    const std::vector<std::string>& optionTexts,
    const std::vector<std::vector<std::string>>& votes
) {
    PollResult result;
    result.question = question;

    int totalVotes = 0;
    for (const auto& v : votes) totalVotes += static_cast<int>(v.size());
    result.totalVotes = totalVotes;

    for (size_t i = 0; i < optionIds.size(); ++i) {
        PollOption opt;
        opt.id = optionIds[i];
        opt.text = i < optionTexts.size() ? optionTexts[i] : "";
        opt.voteCount = i < votes.size() ? static_cast<int>(votes[i].size()) : 0;
        opt.percentage = totalVotes > 0 ? (opt.voteCount * 100.0) / totalVotes : 0.0;
        result.options.push_back(opt);
    }

    auto winners = findWinners(result.options);
    if (!winners.empty()) {
        for (auto& opt : result.options) {
            for (auto* w : winners) {
                if (opt.id == w->id) {
                    opt.isWinner = true;
                    result.winnerId = opt.id;
                    result.winnerText = opt.text;
                }
            }
        }
    }

    return result;
}

bool isPollEnded(int64_t closeTimestampMs) {
    if (closeTimestampMs <= 0) return false;
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    return now >= closeTimestampMs;
}

std::vector<const PollOption*> findWinners(const std::vector<PollOption>& options) {
    std::vector<const PollOption*> winners;
    int maxVotes = 0;
    for (const auto& o : options) {
        if (o.voteCount > maxVotes) maxVotes = o.voteCount;
    }
    if (maxVotes == 0) return winners;
    for (const auto& o : options) {
        if (o.voteCount == maxVotes) winners.push_back(&o);
    }
    return winners;
}

std::string formatPollAsText(const PollResult& result) {
    std::ostringstream out;
    out << "Poll: " << result.question << "\n";
    for (size_t i = 0; i < result.options.size(); ++i) {
        const auto& o = result.options[i];
        out << (i + 1) << ". " << o.text << " — " << o.voteCount << " votes";
        if (result.totalVotes > 0) out << " (" << static_cast<int>(o.percentage) << "%)";
        if (o.isWinner) out << " ★";
        out << "\n";
    }
    out << "Total: " << result.totalVotes << " votes\n";
    return out.str();
}

std::string formatPollAsHtml(const PollResult& result) {
    std::ostringstream html;
    html << "<div class=\"mx_Poll\">\n";
    html << "  <div class=\"mx_Poll_question\">" << result.question << "</div>\n";

    int maxVotes = 0;
    for (const auto& o : result.options) {
        if (o.voteCount > maxVotes) maxVotes = o.voteCount;
    }

    for (const auto& o : result.options) {
        int barWidth = maxVotes > 0 ? (o.voteCount * 100) / maxVotes : 0;
        html << "  <div class=\"mx_Poll_option\">\n";
        html << "    <div class=\"mx_Poll_text\">" << o.text << "</div>\n";
        html << "    <div class=\"mx_Poll_bar\" style=\"width:" << barWidth << "%\"></div>\n";
        html << "    <span>" << o.voteCount << " (" << static_cast<int>(o.percentage) << "%)</span>\n";
        if (o.isWinner) html << "    <span class=\"mx_Poll_winner\">★</span>\n";
        html << "  </div>\n";
    }

    html << "</div>\n";
    return html.str();
}

std::string pollResultToJson(const PollResult& result) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };
    std::ostringstream json;
    json << "{";
    json << R"("question": ")" << esc(result.question) << R"(",)";
    json << R"("totalVotes": )" << result.totalVotes << ",";
    json << R"("isEnded": )" << (result.isEnded ? "true" : "false") << ",";
    json << R"("options": [)";
    for (size_t i = 0; i < result.options.size(); ++i) {
        if (i > 0) json << ",";
        const auto& o = result.options[i];
        json << R"({"id": ")" << o.id << R"(")";
        json << R"(,"text": ")" << esc(o.text) << R"(")";
        json << R"(,"votes": )" << o.voteCount;
        json << R"(,"percentage": )" << o.percentage;
        json << R"(,"winner": )" << (o.isWinner ? "true" : "false") << "}";
    }
    json << "]}";
    return json.str();
}

bool isValidPollQuestion(const std::string& question) {
    return !question.empty() && question.size() <= 200;
}

bool isValidPollOptions(const std::vector<std::string>& options) {
    if (options.size() < 2 || options.size() > 20) return false;
    for (const auto& opt : options) {
        if (opt.empty() || opt.size() > 100) return false;
    }
    return true;
}

std::string generatePollOptionId() {
    static const char chars[] = "abcdefghijklmnopqrstuvwxyz0123456789";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 35);
    std::string id(8, 'a');
    for (int i = 0; i < 8; ++i) id[i] = chars[dis(gen)];
    return id;
}

bool hasVoted(const std::string& userId, const std::vector<std::string>& optionVoters) {
    return std::find(optionVoters.begin(), optionVoters.end(), userId) != optionVoters.end();
}

// ================================================================
// Poll builders — build MSC3381 poll event JSON content
// ================================================================

// Original Kotlin: PollCreateViewModel.kt — buildPollStartEvent()
std::string buildPollStartContent(
    const std::string& question,
    const std::vector<std::string>& optionTexts,
    PollType kind,
    int maxSelections,
    bool unstable,
    std::string* error
) {
    if (question.empty() || question.size() > 340) {
        if (error) *error = "Invalid question (empty or > 340 chars)";
        return "";
    }
    if (optionTexts.size() < 2 || optionTexts.size() > 20) {
        if (error) *error = "Must have 2-20 options (got " + std::to_string(optionTexts.size()) + ")";
        return "";
    }
    for (size_t i = 0; i < optionTexts.size(); i++) {
        if (optionTexts[i].empty() || optionTexts[i].size() > 340) {
            if (error) *error = "Invalid option " + std::to_string(i + 1) + " (empty or > 340 chars)";
            return "";
        }
    }
    if (maxSelections < 1 || maxSelections > static_cast<int>(optionTexts.size())) {
        if (error) *error = "Invalid max_selections: " + std::to_string(maxSelections);
        return "";
    }

    // Original Kotlin: prefix = unstable ? unstablePollStart : stablePollStart
    const char* prefix = unstable ? "org.matrix.msc3381.poll" : "m.poll";

    std::ostringstream os;
    // Original Kotlin: JSON structure per MSC3381
    // {"m.poll.start": {"question": {"m.text": "...", "org.matrix.msc1767.text": "..."}, "kind": "...", ...}}
    os << R"({)";
    os << '"' << prefix << R"(.start": {)";

    // question object
    os << R"("question": {)";
    os << R"(")" << (unstable ? POLL_TEXT_KEY_UNSTABLE : POLL_TEXT_KEY_STABLE)
       << R"(": ")" << jsonEscape(question) << '"';
    if (unstable) {
        os << R"(, ")" << POLL_TEXT_KEY_STABLE << R"(": ")" << jsonEscape(question) << '"';
    }
    os << R"(})"; // end question

    // kind
    os << ',';
    os << R"("kind": ")" << (unstable ? pollTypeToUnstableString(kind) : pollTypeToStableString(kind)) << '"';

    // max_selections
    os << ',' << R"("max_selections": )" << maxSelections;

    // answers
    os << ',' << R"("answers": [)";
    for (size_t i = 0; i < optionTexts.size(); i++) {
        if (i > 0) os << ',';
        os << R"({)";
        // Original Kotlin: answer IDs are generated sequentially (1-based index as string)
        os << R"("id": ")" << (i + 1) << '"';
        os << ',' << '"' << (unstable ? POLL_TEXT_KEY_UNSTABLE : POLL_TEXT_KEY_STABLE)
           << R"(": ")" << jsonEscape(optionTexts[i]) << '"';
        if (unstable) {
            os << ',' << '"' << POLL_TEXT_KEY_STABLE
               << R"(": ")" << jsonEscape(optionTexts[i]) << '"';
        }
        os << R"(})";
    }
    os << R"(])";

    os << R"(})"; // end m.poll.start
    os << R"(})";
    return os.str();
}

// Original Kotlin: PollVoteHandler.kt — build poll response event
std::string buildPollResponseContent(
    const std::vector<std::string>& selectedOptionIds,
    const std::string& relatesToEventId,
    bool unstable
) {
    if (selectedOptionIds.empty()) return "";

    const char* prefix = unstable ? "org.matrix.msc3381.poll" : "m.poll";

    std::ostringstream os;
    // Original Kotlin: {"m.poll.response": {"answers": ["id1", ...]}}
    os << R"({)";
    os << '"' << prefix << R"(.response": {)";

    os << R"("answers": [)";
    for (size_t i = 0; i < selectedOptionIds.size(); i++) {
        if (i > 0) os << ',';
        os << '"' << jsonEscape(selectedOptionIds[i]) << '"';
    }
    os << R"(])";

    os << R"(})"; // end m.poll.response

    if (!relatesToEventId.empty()) {
        os << ',' << R"("m.relates_to": {)";
        os << R"("event_id": ")" << jsonEscape(relatesToEventId) << '"';
        os << ',' << R"("rel_type": "m.reference")";
        os << R"(})";
    }

    os << R"(})";
    return os.str();
}

// Original Kotlin: PollEndEvent.kt — build poll end event
std::string buildPollEndContent(
    const std::string& relatesToEventId,
    const std::string& reasonText,
    bool unstable
) {
    const char* prefix = unstable ? "org.matrix.msc3381.poll" : "m.poll";

    std::ostringstream os;
    // Original Kotlin: {"m.poll.end": {}}
    os << R"({)";
    os << '"' << prefix << R"(.end": {})";

    // reason text
    if (!reasonText.empty()) {
        os << ',' << '"' << (unstable ? POLL_TEXT_KEY_UNSTABLE : POLL_TEXT_KEY_STABLE)
           << R"(": ")" << jsonEscape(reasonText) << '"';
        if (unstable) {
            os << ',' << '"' << POLL_TEXT_KEY_STABLE
               << R"(": ")" << jsonEscape(reasonText) << '"';
        }
    }

    // relates_to
    if (!relatesToEventId.empty()) {
        os << ',' << R"("m.relates_to": {)";
        os << R"("event_id": ")" << jsonEscape(relatesToEventId) << '"';
        os << ',' << R"("rel_type": "m.reference")";
        os << R"(})";
    }

    os << R"(})";
    return os.str();
}

// ================================================================
// Poll parsers — parse MSC3381 poll event JSON content
// ================================================================

// Original Kotlin: MessagePollContent.kt — parse poll start
MessagePollContent parsePollContent(const std::string& contentJson) {
    MessagePollContent msg;

    // Try stable prefix first, then unstable
    // Original Kotlin: val creationInfo = pollCreationInfo ?: unstablePollCreationInfo
    for (int pass = 0; pass < 2; pass++) {
        const char* prefix = (pass == 0) ? "m.poll" : "org.matrix.msc3381.poll";
        PollCreationInfo info;

        // Extract m.poll.start object
        std::string startObjKey = std::string(prefix) + ".start";
        auto startObj = extractObject(contentJson, startObjKey);
        if (startObj.empty()) continue;

        // parse kind
        auto kindStr = extractStr(startObj, "kind");
        info.kind = pollTypeFromString(kindStr);

        // parse max_selections
        info.maxSelections = static_cast<int>(extractInt(startObj, "max_selections"));
        if (info.maxSelections == 0) info.maxSelections = 1;

        // parse question
        auto questionObj = extractObject(startObj, "question");
        if (!questionObj.empty()) {
            // Original Kotlin: question object has { "m.text": "...", "org.matrix.msc1767.text": "..." }
            info.question.text = extractStr(questionObj, "m.text");
            info.question.unstableText = extractStr(questionObj, "org.matrix.msc1767.text");
        }

        // parse answers array
        auto answersArray = extractArray(startObj, "answers");
        if (!answersArray.empty()) {
            auto elements = parseArrayElements(answersArray);
            for (const auto& elem : elements) {
                PollAnswer answer;
                answer.id = extractStr(elem, "id");
                answer.text = extractStr(elem, "m.text");
                answer.unstableText = extractStr(elem, "org.matrix.msc1767.text");
                if (!answer.id.empty()) {
                    info.answers.push_back(answer);
                }
            }
        }

        // Original Kotlin: Assign to stable or unstable slot
        if (pass == 0) {
            msg.creationInfo = info;
        } else {
            msg.unstableCreationInfo = info;
        }
    }

    // parse relates_to
    auto relatesObj = extractObject(contentJson, "m.relates_to");
    if (!relatesObj.empty()) {
        msg.relatesTo = extractStr(relatesObj, "event_id");
    }

    // parse fallback body
    msg.body = extractStr(contentJson, "body");

    return msg;
}

// Original Kotlin: MessagePollResponseContent.kt — parse vote
MessagePollResponseContent parsePollResponseContent(const std::string& contentJson) {
    MessagePollResponseContent msg;

    // Try stable prefix first, then unstable
    // Original Kotlin: val response = response ?: unstableResponse
    bool found = false;
    for (int pass = 0; pass < 2 && !found; pass++) {
        const char* prefix = (pass == 0) ? "m.poll" : "org.matrix.msc3381.poll";
        std::string responseKey = std::string(prefix) + ".response";
        auto responseObj = extractObject(contentJson, responseKey);
        if (responseObj.empty()) continue;

        // parse answers array
        auto answersArray = extractArray(responseObj, "answers");
        if (!answersArray.empty()) {
            msg.answers = parseStringArray(answersArray);
            if (!msg.answers.empty()) found = true;
        }
    }

    // parse relates_to
    auto relatesObj = extractObject(contentJson, "m.relates_to");
    if (!relatesObj.empty()) {
        msg.relatesTo = extractStr(relatesObj, "event_id");
    }

    // parse fallback body
    msg.body = extractStr(contentJson, "body");

    return msg;
}

// Original Kotlin: MessageEndPollContent.kt — parse end event
MessageEndPollContent parsePollEndContent(const std::string& contentJson) {
    MessageEndPollContent msg;

    // parse text / reason
    // Original Kotlin: val text = text ?: unstableText
    msg.text = extractStr(contentJson, "m.text");
    msg.unstableText = extractStr(contentJson, "org.matrix.msc1767.text");

    // Also look inside m.poll.end or org.matrix.msc3381.poll.end nested objects
    for (int pass = 0; pass < 2; pass++) {
        const char* prefix = (pass == 0) ? "m.poll" : "org.matrix.msc3381.poll";
        std::string endKey = std::string(prefix) + ".end";
        auto endObj = extractObject(contentJson, endKey);
        if (!endObj.empty()) {
            auto t = extractStr(endObj, "m.text");
            if (!t.empty()) msg.text = t;
            t = extractStr(endObj, "org.matrix.msc1767.text");
            if (!t.empty()) msg.unstableText = t;
        }
    }

    // parse relates_to
    auto relatesObj = extractObject(contentJson, "m.relates_to");
    if (!relatesObj.empty()) {
        msg.relatesTo = extractStr(relatesObj, "event_id");
    }

    // parse fallback body
    msg.body = extractStr(contentJson, "body");

    return msg;
}

// ================================================================
// Poll aggregation
// ================================================================

// Original Kotlin: DefaultPollAggregationProcessor.kt — groupBy and compute summary
PollSummaryContent aggregatePollResults(
    const std::vector<PollAnswer>& answers,
    const std::vector<VoteInfo>& allVotes,
    const std::string& myUserId
) {
    PollSummaryContent summary;

    // Deduplicate: keep only the latest vote per user
    // Original Kotlin: existingVotes.indexOfFirst { it.userId == senderId }
    // If existing, compare voteTimestamp; keep latest.
    std::unordered_map<std::string, VoteInfo> latestVotes;
    for (const auto& vote : allVotes) {
        auto it = latestVotes.find(vote.userId);
        if (it == latestVotes.end() || vote.voteTimestamp > it->second.voteTimestamp) {
            latestVotes[vote.userId] = vote;
        }
    }

    // Flatten to list
    for (const auto& [uid, v] : latestVotes) {
        summary.votes.push_back(v);
    }

    summary.totalVotes = static_cast<int>(summary.votes.size());

    // Build votesSummary: optionId -> { total, percentage }
    // Original Kotlin: groupBy { it.option } and mapValues
    std::unordered_map<std::string, int> optionCounts;
    for (const auto& vote : summary.votes) {
        optionCounts[vote.option]++;
    }

    for (const auto& answer : answers) {
        VoteSummary vs;
        auto it = optionCounts.find(answer.id);
        if (it != optionCounts.end()) {
            vs.total = it->second;
            vs.percentage = summary.totalVotes > 0
                                ? static_cast<double>(vs.total) / summary.totalVotes
                                : 0.0;
        }
        summary.votesSummary[answer.id] = vs;
    }

    // Compute winnerVoteCount
    // Original Kotlin: winnerVoteCount = newVotesSummary.maxOf { it.value.total }
    summary.winnerVoteCount = 0;
    for (const auto& [optId, vs] : summary.votesSummary) {
        if (vs.total > summary.winnerVoteCount) {
            summary.winnerVoteCount = vs.total;
        }
    }

    // Set myVote
    // Original Kotlin: myVote = existingVotes.find { it.userId == session.myUserId }?.option
    auto it = latestVotes.find(myUserId);
    if (it != latestVotes.end()) {
        summary.myVote = it->second.option;
    }

    return summary;
}

// Original Kotlin: compute winner count from votesSummary
int computePollWinnerCount(const PollSummaryContent& summary) {
    return summary.winnerVoteCount;
}

// Original Kotlin: PollResponseAggregatedSummary.isClosed()
bool isPollClosed(int64_t closedTimeMs) {
    // Original Kotlin: closedTime != null → closed
    if (closedTimeMs <= 0) return false;
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    return closedTimeMs <= now;
}

// Original Kotlin: format poll results as human-readable string
std::string formatPollResults(
    const PollSummaryContent& summary,
    const std::vector<PollAnswer>& answers,
    bool includePercentages
) {
    std::ostringstream os;

    // Build a map from answer id to text for display
    std::unordered_map<std::string, std::string> answerTexts;
    for (const auto& a : answers) {
        answerTexts[a.id] = a.getBestText();
    }

    // Find winner option IDs
    int maxCount = summary.winnerVoteCount;
    std::vector<std::string> winnerIds;
    if (maxCount > 0) {
        for (const auto& [optId, vs] : summary.votesSummary) {
            if (vs.total == maxCount) winnerIds.push_back(optId);
        }
    }

    os << summary.totalVotes << " vote" << (summary.totalVotes != 1 ? "s" : "") << "\n";

    for (const auto& answer : answers) {
        auto it = summary.votesSummary.find(answer.id);
        int count = (it != summary.votesSummary.end()) ? it->second.total : 0;
        double pct = (it != summary.votesSummary.end()) ? it->second.percentage : 0.0;

        os << answer.getBestText() << ": " << count;
        if (includePercentages && summary.totalVotes > 0) {
            os << " (" << static_cast<int>(std::round(pct * 100.0)) << "%)";
        }

        if (std::find(winnerIds.begin(), winnerIds.end(), answer.id) != winnerIds.end() && maxCount > 0) {
            os << " ★";
        }
        os << "\n";
    }

    return os.str();
}

} // namespace progressive
