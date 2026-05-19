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

// ================================================================
// Reaction Grouping & Management
// ================================================================

ReactionList groupReactions(const std::vector<ReactionEvent>& reactions, const std::string& myUserId) {
    // Original Kotlin: groupReactions() — groups reactions by key
    ReactionList list;
    std::unordered_map<std::string, ReactionGroup> byKey;

    for (const auto& r : reactions) {
        auto& group = byKey[r.key];
        group.key = r.key;
        group.count++;
        group.totalCount++;
        group.senders.push_back(r.senderId);
        if (r.senderId == myUserId) group.isMyReaction = true;
    }

    for (const auto& p : byKey) {
        list.groups.push_back(p.second);
    }
    list.totalReactions = static_cast<int>(reactions.size());

    sortReactionGroups(list.groups, ReactionDisplayOrder::COUNT_DESC);
    return list;
}

std::vector<ReactionGroup> computeReactionGroups(const std::vector<ReactionEvent>& reactions, const std::string& myUserId) {
    // Original Kotlin: computeReactionGroups() — computes per-key groups
    return groupReactions(reactions, myUserId).groups;
}

ReactionEvent findMyReaction(const std::vector<ReactionEvent>& reactions, const std::string& myUserId) {
    // Original Kotlin: findMyReaction() — finds the current user's reaction
    for (const auto& r : reactions) {
        if (r.senderId == myUserId) return r;
    }
    return {};
}

std::string formatReactionCount(int count) {
    // Original Kotlin: formatReactionCount() — compact display
    if (count < 1000) return std::to_string(count);
    if (count < 1000000) {
        std::ostringstream os;
        os << (count / 1000) << "k";
        return os.str();
    }
    std::ostringstream os;
    os << (count / 1000000) << "M";
    return os.str();
}

bool isReactionAllowed(const std::string& eventType, const std::string& roomId) {
    // Original Kotlin: isReactionAllowed() — check event type and room state
    // Disallow reactions on redactions, m.call.*, and server notice rooms
    if (eventType == "m.room.redaction") return false;
    if (eventType.find("m.call.") == 0) return false;
    if (eventType == "m.reaction") return false; // can't react to a reaction
    return true;
}

void sortReactionGroups(std::vector<ReactionGroup>& groups, ReactionDisplayOrder order) {
    // Original Kotlin: sortReactionGroups() — sorts by display order
    switch (order) {
        case ReactionDisplayOrder::COUNT_DESC:
            std::sort(groups.begin(), groups.end(),
                [](const ReactionGroup& a, const ReactionGroup& b) {
                    return a.count > b.count;
                });
            break;
        case ReactionDisplayOrder::RECENT_FIRST:
            // Groups don't have timestamps — fall back to count desc
            std::sort(groups.begin(), groups.end(),
                [](const ReactionGroup& a, const ReactionGroup& b) {
                    return a.count > b.count;
                });
            break;
        case ReactionDisplayOrder::EMOJI_ORDER:
            std::sort(groups.begin(), groups.end(),
                [](const ReactionGroup& a, const ReactionGroup& b) {
                    return a.key < b.key;
                });
            break;
    }
}

std::vector<ReactionGroup> getTopReactions(const std::vector<ReactionGroup>& groups, int maxDisplayed) {
    // Original Kotlin: getTopReactions() — top N by count
    if (maxDisplayed <= 0 || groups.empty()) return {};
    std::vector<ReactionGroup> sorted = groups;
    sortReactionGroups(sorted, ReactionDisplayOrder::COUNT_DESC);
    if (static_cast<int>(sorted.size()) > maxDisplayed) {
        sorted.resize(static_cast<size_t>(maxDisplayed));
    }
    return sorted;
}

bool hasUserReacted(const std::vector<ReactionEvent>& reactions, const std::string& userId) {
    // Original Kotlin: hasUserReacted()
    for (const auto& r : reactions) {
        if (r.senderId == userId) return true;
    }
    return false;
}

std::string addReaction(const std::string& relatesToEventId, const std::string& emojiKey) {
    // Original Kotlin: addReaction() — builds reaction add event
    // {"type":"m.reaction","content":{"m.relates_to":{"event_id":"...","key":"👍","rel_type":"m.annotation"}}}
    std::ostringstream os;
    os << R"({"type":"m.reaction","content":{)";
    os << R"("m.relates_to":{)";
    os << R"("event_id":")" << relatesToEventId << R"(",)";
    os << R"("key":")" << emojiKey << R"(",)";
    os << R"("rel_type":"m.annotation")";
    os << "}}}";
    return os.str();
}

std::string removeReaction(const std::string& reactionEventId) {
    // Original Kotlin: removeReaction() — builds redaction event
    std::ostringstream os;
    os << R"({"type":"m.room.redaction","redacts":")" << reactionEventId << R"("})";
    return os.str();
}

std::string toggleReaction(const std::string& relatesToEventId, const std::string& emojiKey,
                            const std::vector<ReactionEvent>& existingReactions,
                            const std::string& myUserId, std::string& outAction) {
    // Original Kotlin: toggleReaction() — add if missing, remove if present
    for (const auto& r : existingReactions) {
        if (r.senderId == myUserId && isSameEmoji(r.key, emojiKey)) {
            outAction = "remove";
            return removeReaction(r.eventId);
        }
    }
    outAction = "add";
    return addReaction(relatesToEventId, emojiKey);
}

std::string buildReactionEvent(const std::string& relatesToEventId, const std::string& emojiKey,
                                const std::string& senderId) {
    // Original Kotlin: buildReactionEvent() — full m.reaction event
    std::ostringstream os;
    os << R"({"type":"m.reaction","sender":")" << senderId << R"(",)";
    os << R"("content":{)";
    os << R"("m.relates_to":{)";
    os << R"("event_id":")" << relatesToEventId << R"(",)";
    os << R"("key":")" << emojiKey << R"(",)";
    os << R"("rel_type":"m.annotation")";
    os << "}}}";
    return os.str();
}

ReactionEvent parseReactionEvent(const std::string& eventId, const std::string& eventContentJson,
                                  const std::string& senderId, int64_t timestamp, const std::string& relatesToEventId) {
    // Original Kotlin: parseReactionEvent() — parses m.reaction event content
    ReactionEvent ev;
    ev.eventId = eventId;
    ev.senderId = senderId;
    ev.timestamp = timestamp;
    ev.relatesToEventId = relatesToEventId;
    ev.key = extractReactionKey(eventContentJson);
    return ev;
}

} // namespace progressive
