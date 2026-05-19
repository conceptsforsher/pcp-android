#include "progressive/poll_manager.hpp"
#include <sstream>
#include <algorithm>
#include <ctime>
#include <cmath>

namespace progressive {

// ====== JSON helpers (local) ======

static std::string extractStr(const std::string& json, const std::string& key) {
    auto pp = json.find("\"" + key + "\"");
    if (pp == std::string::npos) return "";
    pp = json.find('"', pp + key.size() + 2);
    if (pp == std::string::npos) return "";
    pp++;
    size_t e = pp;
    while (e < json.size() && json[e] != '"') e++;
    return json.substr(pp, e - pp);
}

static int64_t extractInt(const std::string& json, const std::string& key) {
    auto pp = json.find("\"" + key + "\"");
    if (pp == std::string::npos) return 0;
    pp = json.find(':', pp);
    if (pp == std::string::npos) return 0;
    pp++;
    while (pp < json.size() && (json[pp] == ' ' || json[pp] == '\t')) pp++;
    int64_t v = 0;
    while (pp < json.size() && json[pp] >= '0' && json[pp] <= '9') { v=v*10+(json[pp]-'0'); pp++; }
    return v;
}

static bool extractBool(const std::string& json, const std::string& key) {
    return json.find("\"" + key + "\":true") != std::string::npos;
}

// ====== PollOptionFull helpers ======

int PollContent::totalVotes() const {
    int total = 0;
    for (const auto& o : options) total += o.voteCount;
    return total;
}

int PollContent::voterCount() const {
    std::unordered_set<std::string> voters;
    for (const auto& o : options) {
        for (const auto& v : o.voterIds) voters.insert(v);
    }
    return static_cast<int>(voters.size());
}

// ====== Constructor ======

PollManager::PollManager() {}

std::string PollManager::generatePollId() const {
    auto now = static_cast<int64_t>(std::time(nullptr));
    std::ostringstream id;
    id << "poll_" << now;
    return id.str();
}

std::string PollManager::optionIdFromIndex(int index) const {
    std::string id;
    // Convert index to letters: 0→A, 1→B, ..., 25→Z, 26→AA, etc.
    int n = index;
    do {
        id.insert(0, 1, 'A' + (n % 26));
        n /= 26;
    } while (n > 0);
    return id;
}

// ====== Validation ======

bool PollManager::isValidPollQuestion(const std::string& question) {
    return !question.empty() && question.size() <= 340;
}

bool PollManager::isValidPollOption(const std::string& text) {
    return !text.empty() && text.size() <= 340;
}

bool PollManager::isValidMaxSelections(int selections, int optionCount) {
    return selections >= 1 && selections <= optionCount;
}

// ====== Poll Creation ======

std::string PollManager::buildPollStartContent(const std::string& question,
                                                const std::vector<std::string>& optionTexts,
                                                PollKind kind, int maxSelections,
                                                bool unstable, std::string& error) {
    if (!isValidPollQuestion(question)) {
        error = "Invalid question (empty or > 340 chars)";
        return "";
    }
    if (optionTexts.size() < 2 || optionTexts.size() > 20) {
        error = "Must have 2-20 options (got " + std::to_string(optionTexts.size()) + ")";
        return "";
    }
    for (size_t i = 0; i < optionTexts.size(); i++) {
        if (!isValidPollOption(optionTexts[i])) {
            error = "Invalid option " + std::to_string(i+1) + " (empty or > 340 chars)";
            return "";
        }
    }
    if (!isValidMaxSelections(maxSelections, static_cast<int>(optionTexts.size()))) {
        error = "Invalid max_selections: " + std::to_string(maxSelections);
        return "";
    }

    std::string prefix = unstable ? "org.matrix.msc3381.poll" : "m.poll";

    std::ostringstream os;
    os << R"({)";
    os << R"(")" << prefix << R"(.start":{})";
    os << R"(,")" << prefix << R"(.kind":")" << (kind == PollKind::DISCLOSED ? "disclosed" : "undisclosed") << R"(")";
    os << R"(,")" << prefix << R"(.max_selections":)" << maxSelections;
    os << R"(,")" << prefix << R"(.question":{)" << R"("body":")" << question << R"(")";
    os << R"(,"msgtype":"m.text")";
    os << R"(})"; // end question

    // Answers
    os << R"(,")" << prefix << R"(.answers":[)";
    for (size_t i = 0; i < optionTexts.size(); i++) {
        if (i > 0) os << ",";
        os << R"({")";
        os << R"("id":")" << optionIdFromIndex(static_cast<int>(i)) << R"(")";
        os << R"(,")" << prefix << R"(.org_text":")" << optionTexts[i] << R"(")";
        os << R"(})";
    }
    os << R"(])"; // end answers

    os << "}";
    return os.str();
}

PollContent PollManager::parsePollStartContent(const std::string& contentJson, bool unstable) {
    PollContent poll;
    poll.unstable = unstable;
    std::string prefix = unstable ? "org.matrix.msc3381.poll" : "m.poll";

    poll.question = extractStr(contentJson, prefix + ".question");
    if (poll.question.empty()) {
        // Try extracting from nested structure
        auto qObj = contentJson;
        auto qPos = qObj.find("\"" + prefix + ".question\"");
        if (qPos != std::string::npos) {
            qPos = qObj.find('{', qPos);
            if (qPos != std::string::npos) {
                qObj = qObj.substr(qPos);
                poll.question = extractStr(qObj, "body");
            }
        }
    }

    auto kindStr = extractStr(contentJson, prefix + ".kind");
    poll.kind = (kindStr == "undisclosed") ? PollKind::UNDISCLOSED : PollKind::DISCLOSED;

    poll.maxSelections = static_cast<int>(extractInt(contentJson, prefix + ".max_selections"));
    if (poll.maxSelections == 0) poll.maxSelections = 1;

    // Parse answers array: [{"id":"A","...":"Option text"}, ...]
    size_t pos = contentJson.find("\"" + prefix + ".answers\"");
    if (pos != std::string::npos) {
        pos = contentJson.find('[', pos);
        if (pos != std::string::npos) {
            pos++;
            while (pos < contentJson.size()) {
                // Skip whitespace and commas
                while (pos < contentJson.size() && (contentJson[pos] == ' ' || contentJson[pos] == ',' || contentJson[pos] == '\n')) pos++;
                if (pos >= contentJson.size() || contentJson[pos] == ']') break;

                size_t objStart = pos;
                int depth = 0;
                while (pos < contentJson.size()) {
                    if (contentJson[pos] == '{') depth++;
                    else if (contentJson[pos] == '}') depth--;
                    if (depth == 0 && contentJson[pos] == '}') { pos++; break; }
                    pos++;
                }
                std::string optionJson = contentJson.substr(objStart, pos - objStart);

                PollOptionFull opt;
                opt.id = extractStr(optionJson, "id");
                opt.text = extractStr(optionJson, prefix + ".org_text");
                if (!opt.id.empty() && !opt.text.empty()) {
                    poll.options.push_back(opt);
                }
                pos++;
            }
        }
    }

    poll.valid = !poll.question.empty() && poll.options.size() >= 2;
    return poll;
}

// ====== Vote Casting ======

std::string PollManager::buildPollResponseContent(const std::string& pollId,
                                                    const std::vector<std::string>& selectedOptionIds,
                                                    bool unstable) {
    std::string prefix = unstable ? "org.matrix.msc3381.poll" : "m.poll";

    std::ostringstream os;
    os << R"({)";
    os << R"(")" << prefix << R"(.response":{})";
    os << R"(,")" << prefix << R"(.org_selections":[)";
    for (size_t i = 0; i < selectedOptionIds.size(); i++) {
        if (i > 0) os << ",";
        os << R"(")" << selectedOptionIds[i] << R"(")";
    }
    os << R"(])";
    os << "}";
    return os.str();
}

PollVote PollManager::parsePollResponseContent(const std::string& contentJson,
                                                 const std::string& voterId,
                                                 const std::string& voterName,
                                                 bool unstable) {
    PollVote vote;
    vote.voterId = voterId;
    vote.voterName = voterName;
    std::string prefix = unstable ? "org.matrix.msc3381.poll" : "m.poll";

    // Parse selections array
    auto selKey = prefix + ".org_selections";
    size_t pos = contentJson.find("\"" + selKey + "\"");
    if (pos != std::string::npos) {
        pos = contentJson.find('[', pos);
        if (pos != std::string::npos) {
            pos++;
            while (pos < contentJson.size()) {
                while (pos < contentJson.size() && (contentJson[pos] == ' ' || contentJson[pos] == ',')) pos++;
                if (pos >= contentJson.size() || contentJson[pos] == ']') break;
                if (contentJson[pos] == '"') {
                    pos++;
                    size_t e = pos;
                    while (e < contentJson.size() && contentJson[e] != '"') e++;
                    vote.selectedOptionIds.push_back(contentJson.substr(pos, e - pos));
                    pos = e + 1;
                } else {
                    pos++;
                }
            }
        }
    }

    vote.valid = !vote.selectedOptionIds.empty();
    return vote;
}

// ====== Poll Ending ======

std::string PollManager::buildPollEndContent(const std::string& pollId, const std::string& reason, bool unstable) {
    std::string prefix = unstable ? "org.matrix.msc3381.poll" : "m.poll";
    std::ostringstream os;
    os << R"({)";
    os << R"(")" << prefix << R"(.end":{})";
    if (!reason.empty()) os << R"(,"reason":")" << reason << R"(")";
    os << "}";
    return os.str();
}

PollEnd PollManager::parsePollEndContent(const std::string& contentJson, bool unstable) {
    PollEnd end;
    std::string prefix = unstable ? "org.matrix.msc3381.poll" : "m.poll";
    end.reasonText = extractStr(contentJson, "reason");
    end.closedAtMs = static_cast<int64_t>(std::time(nullptr)) * 1000;
    return end;
}

// ====== Vote Tallying ======

PollResultFull PollManager::tallyVotes(const PollContent& poll, const std::vector<PollVote>& votes) {
    PollResultFull result;
    result.poll = poll;
    result.results = poll.options;
    result.isClosed = poll.isClosed();

    // Count votes per option
    for (const auto& vote : votes) {
        for (const auto& optId : vote.selectedOptionIds) {
            for (auto& opt : result.results) {
                if (opt.id == optId) {
                    opt.voteCount++;
                    opt.voterIds.push_back(vote.voterId);
                }
            }
            result.totalVotes++;
        }
    }

    // Count unique voters
    std::unordered_set<std::string> uniqueVoters;
    for (const auto& vote : votes) uniqueVoters.insert(vote.voterId);
    result.totalVoters = static_cast<int>(uniqueVoters.size());

    // Calculate percentages
    for (auto& opt : result.results) {
        if (result.totalVotes > 0) {
            opt.percentage = (static_cast<double>(opt.voteCount) / result.totalVotes) * 100.0;
        }
    }

    // Update poll options
    result.poll.options = result.results;

    return result;
}

void PollManager::setMyVote(PollResultFull& result, const std::string& userId) {
    for (const auto& opt : result.results) {
        for (const auto& voter : opt.voterIds) {
            if (voter == userId) {
                result.hasVoted = true;
                result.myVote += (result.myVote.empty() ? "" : ", ") + opt.text;
            }
        }
    }
}

// ====== Display Formatting ======

PollEventDisplay PollManager::formatPollEvent(const PollResultFull& result) {
    PollEventDisplay display;
    display.question = result.poll.question;
    display.totalVotes = result.totalVotes;
    display.isClosed = result.isClosed;
    display.totalVotes = result.totalVotes;

    // Find winner
    int maxVotes = 0;
    int winnerIdx = -1;
    bool isTie = false;
    for (size_t i = 0; i < result.results.size(); i++) {
        if (result.results[i].voteCount > maxVotes) {
            maxVotes = result.results[i].voteCount;
            winnerIdx = static_cast<int>(i);
            isTie = false;
        } else if (result.results[i].voteCount == maxVotes && maxVotes > 0) {
            isTie = true;
        }
    }
    if (isTie) winnerIdx = -1;
    display.winnerOption = winnerIdx;

    // Build plain text
    std::ostringstream text;
    text << result.poll.question << "\n";
    for (size_t i = 0; i < result.results.size(); i++) {
        text << result.results[i].text << ": " << result.results[i].voteCount;
        if (result.totalVoters > 0) {
            text << " votes (" << static_cast<int>(std::round(result.results[i].percentage)) << "%)";
        }
        if (static_cast<int>(i) == winnerIdx && result.isClosed && result.totalVoters > 0) {
            text << " ★";
        }
        text << "\n";
    }
    if (result.isClosed) {
        text << "Poll closed";
        if (result.totalVoters == 0) text << " · No votes";
    } else {
        text << result.totalVotes << " total votes";
    }
    display.plainText = text.str();

    // Build HTML
    std::ostringstream html;
    html << "<div class='poll'>";
    html << "<b>" << result.poll.question << "</b><br/>";
    for (size_t i = 0; i < result.results.size(); i++) {
        int pct = static_cast<int>(std::round(result.results[i].percentage));
        html << "<div style='margin:4px 0;'>"
             << "<span>" << result.results[i].text << "</span>"
             << "<span style='float:right;'>" << result.results[i].voteCount;
        if (static_cast<int>(i) == winnerIdx && result.isClosed && result.totalVoters > 0) {
            html << " ★";
        }
        html << "</span>"
             << "<div style='background:#ddd;height:20px;border-radius:10px;margin-top:4px;'>"
             << "<div style='background:" << (static_cast<int>(i) == winnerIdx ? "#4CAF50" : "#2196F3")
             << ";width:" << pct << "%;height:100%;border-radius:10px;min-width:" << (pct > 0 ? "8px" : "0") << ";'></div>"
             << "</div></div>";
    }
    html << "<small>" << result.totalVotes << " votes";
    if (result.isClosed) {
        html << " · Poll closed";
        if (result.totalVoters == 0) html << " · No votes";
    }
    html << "</small></div>";
    display.htmlBody = html.str();

    return display;
}

std::string PollManager::formatPollPlainText(const PollEventDisplay& display) {
    return display.plainText;
}

std::string PollManager::formatPollHtml(const PollEventDisplay& display) {
    return display.htmlBody;
}

std::string PollManager::getWinnerText(const PollResultFull& result) const {
    if (result.totalVoters == 0) return "No votes yet";
    if (result.totalVoters == 1) return "1 vote";

    int maxIdx = -1;
    int maxCount = 0;
    bool tie = false;
    for (size_t i = 0; i < result.results.size(); i++) {
        if (result.results[i].voteCount > maxCount) {
            maxCount = result.results[i].voteCount;
            maxIdx = static_cast<int>(i);
            tie = false;
        } else if (result.results[i].voteCount == maxCount && maxCount > 0) {
            tie = true;
        }
    }

    if (tie) return "Tie — " + std::to_string(maxCount) + " votes each";
    if (maxIdx >= 0 && maxIdx < static_cast<int>(result.results.size())) {
        return result.results[maxIdx].text + " — " + std::to_string(maxCount) + " vote" + (maxCount > 1 ? "s" : "");
    }
    return "No votes yet";
}

// ====== Poll State ======

bool PollManager::isPollEvent(const std::string& eventType) const {
    return eventType == "m.poll.start" || eventType == "m.poll.response" ||
           eventType == "m.poll.end" ||
           eventType == "org.matrix.msc3381.poll.start" ||
           eventType == "org.matrix.msc3381.poll.response" ||
           eventType == "org.matrix.msc3381.poll.end";
}

std::string PollManager::getPollEventDescription(const std::string& eventType) const {
    if (eventType.find("poll.start") != std::string::npos) return "Poll started";
    if (eventType.find("poll.response") != std::string::npos) return "Voted in poll";
    if (eventType.find("poll.end") != std::string::npos) return "Poll ended";
    return "Poll event";
}

} // namespace progressive
