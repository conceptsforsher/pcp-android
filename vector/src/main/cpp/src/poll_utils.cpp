#include "progressive/poll_utils.hpp"
#include <sstream>
#include <algorithm>
#include <chrono>
#include <random>

namespace progressive {

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

} // namespace progressive
