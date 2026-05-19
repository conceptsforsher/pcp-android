#ifndef PROGRESSIVE_REACTION_UTILS_HPP
#define PROGRESSIVE_REACTION_UTILS_HPP

#include <string>
#include <vector>
#include <unordered_map>

namespace progressive {

#ifndef PROGRESSIVE_REACTION_INFO_DEFINED
#define PROGRESSIVE_REACTION_INFO_DEFINED
struct ReactionInfo {
    std::string emoji;         // "👍"
    int count = 0;
    bool addedByMe = false;
    bool synced = true;        // from ReactionInfoData.synced — false if pending echo
    std::vector<std::string> userIds;  // who reacted
    int64_t firstTimestamp = 0;
};
#endif

struct ReactionSummary {
    std::string eventId;
    std::vector<ReactionInfo> reactions;
    int totalReactions = 0;
    std::string topEmoji;      // most used reaction
    int uniqueReactors = 0;
    bool showAll = false;      // from ReactionsSummaryData.showAll — expand collapsed
};

// Aggregate reactions from raw event data.
ReactionSummary aggregateReactions(
    const std::string& eventId,
    const std::vector<std::string>& reactionEmojis,
    const std::vector<std::string>& reactorIds,
    const std::vector<int64_t>& timestamps,
    const std::string& myUserId
);

// Get quick reaction emojis (commonly used for quick access).
std::vector<std::string> getQuickReactions();

// Check if an emoji is a valid reaction (not too long, visible char).
bool isValidReactionEmoji(const std::string& emoji);

// Format reaction summary as a compact string: "👍 5, ❤️ 3, 🚀 2"
std::string formatReactionSummary(const ReactionSummary& summary);

// Format reaction summary for accessibility.
std::string formatReactionAccessibility(const ReactionSummary& summary);

// Parse emoji reaction key from Matrix event content.
std::string extractReactionKey(const std::string& eventContentJson);

// Check if two emojis are the same (handles variation selectors).
bool isSameEmoji(const std::string& a, const std::string& b);

// Format ReactionSummary as JSON with synced and showAll fields.
std::string reactionSummaryToJson(const ReactionSummary& summary);

} // namespace progressive

#endif // PROGRESSIVE_REACTION_UTILS_HPP
