#include "progressive/reaction_utils.hpp"
#include <sstream>
#include <algorithm>
#include <unordered_set>

namespace progressive {

ReactionSummary aggregateReactions(
    const std::string& eventId,
    const std::vector<std::string>& reactionEmojis,
    const std::vector<std::string>& reactorIds,
    const std::vector<int64_t>& timestamps,
    const std::string& myUserId
) {
    ReactionSummary summary;
    summary.eventId = eventId;

    // Aggregate by emoji
    std::unordered_map<std::string, ReactionInfo> byEmoji;
    std::unordered_set<std::string> uniqueReactors;

    for (size_t i = 0; i < reactionEmojis.size(); ++i) {
        const auto& emoji = reactionEmojis[i];
        const auto& userId = i < reactorIds.size() ? reactorIds[i] : "";
        int64_t ts = i < timestamps.size() ? timestamps[i] : 0;

        auto& info = byEmoji[emoji];
        if (info.emoji.empty()) info.emoji = emoji;
        info.count++;
        info.userIds.push_back(userId);
        if (info.firstTimestamp == 0 || ts < info.firstTimestamp) info.firstTimestamp = ts;
        if (userId == myUserId) info.addedByMe = true;

        uniqueReactors.insert(userId);
    }

    summary.totalReactions = static_cast<int>(reactionEmojis.size());
    summary.uniqueReactors = static_cast<int>(uniqueReactors.size());

    // Sort by count descending
    for (const auto& p : byEmoji) {
        summary.reactions.push_back(p.second);
    }
    std::sort(summary.reactions.begin(), summary.reactions.end(),
        [](const ReactionInfo& a, const ReactionInfo& b) {
            return a.count > b.count;
        }
    );

    if (!summary.reactions.empty()) {
        summary.topEmoji = summary.reactions[0].emoji;
    }

    return summary;
}

std::vector<std::string> getQuickReactions() {
    // Most commonly used reaction emojis
    return {"👍", "👎", "😄", "🎉", "😕", "❤️", "🚀", "👀", "🔥", "💯", "✅", "❌"};
}

bool isValidReactionEmoji(const std::string& emoji) {
    if (emoji.empty() || emoji.size() > 20) return false;
    // Must contain at least one non-ASCII character (emoji range)
    for (char c : emoji) {
        if (static_cast<unsigned char>(c) >= 0xF0) return true; // 4-byte UTF-8
        if (static_cast<unsigned char>(c) >= 0xE0) return true; // 3-byte UTF-8
    }
    // Allow ASCII emoticons like ":)" as reactions
    return emoji.size() >= 2;
}

std::string formatReactionSummary(const ReactionSummary& summary) {
    std::ostringstream out;
    for (size_t i = 0; i < summary.reactions.size(); ++i) {
        if (i > 0) out << ", ";
        out << summary.reactions[i].emoji << " " << summary.reactions[i].count;
    }
    return out.str();
}

std::string formatReactionAccessibility(const ReactionSummary& summary) {
    if (summary.totalReactions == 0) return "No reactions";

    std::ostringstream out;
    for (size_t i = 0; i < summary.reactions.size(); ++i) {
        const auto& r = summary.reactions[i];
        if (i > 0) {
            out << (i == summary.reactions.size() - 1 ? " and " : ", ");
        }
        out << r.count << " " << r.emoji;
    }
    out << " reactions";
    return out.str();
}

std::string extractReactionKey(const std::string& eventContentJson) {
    // Matrix reaction content: {"m.relates_to": {"key": "👍", ...}}
    auto keyPos = eventContentJson.find("\"key\":");
    if (keyPos == std::string::npos) return {};

    keyPos += 6;
    while (keyPos < eventContentJson.size() && eventContentJson[keyPos] == ' ') ++keyPos;

    if (keyPos >= eventContentJson.size() || eventContentJson[keyPos] != '"') return {};
    ++keyPos;

    auto end = eventContentJson.find('"', keyPos);
    if (end == std::string::npos) return {};

    return eventContentJson.substr(keyPos, end - keyPos);
}

bool isSameEmoji(const std::string& a, const std::string& b) {
    if (a == b) return true;
    // Strip variation selectors (U+FE0F)
    auto strip = [](std::string s) -> std::string {
        std::string result;
        for (size_t i = 0; i < s.size(); ++i) {
            // Skip variation selector-16 (EF B8 8F)
            if (i + 2 < s.size() &&
                static_cast<unsigned char>(s[i]) == 0xEF &&
                static_cast<unsigned char>(s[i+1]) == 0xB8 &&
                static_cast<unsigned char>(s[i+2]) == 0x8F) {
                i += 2;
                continue;
            }
            result += s[i];
        }
        return result;
    };
    return strip(a) == strip(b);
}

std::string reactionSummaryToJson(const ReactionSummary& summary) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"eventId": ")" << esc(summary.eventId) << R"(",)";
    json << R"("totalReactions": )" << summary.totalReactions << ",";
    json << R"("topEmoji": ")" << esc(summary.topEmoji) << R"(",)";
    json << R"("uniqueReactors": )" << summary.uniqueReactors << ",";
    json << R"("showAll": )" << (summary.showAll ? "true" : "false") << ",";
    json << R"("reactions": [)";
    for (size_t i = 0; i < summary.reactions.size(); ++i) {
        if (i > 0) json << ",";
        const auto& r = summary.reactions[i];
        json << R"({"emoji": ")" << esc(r.emoji) << R"(",)";
        json << R"("count": )" << r.count << ",";
        json << R"("addedByMe": )" << (r.addedByMe ? "true" : "false") << ",";
        json << R"("synced": )" << (r.synced ? "true" : "false") << "}";
    }
    json << "]}";
    return json.str();
}

} // namespace progressive
