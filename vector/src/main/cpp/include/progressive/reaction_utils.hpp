#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>

namespace progressive {

struct ReactionInfo {
    std::string emoji;         // "👍"
    int count = 0;
    bool addedByMe = false;
    bool synced = true;        // from ReactionInfoData.synced — false if pending echo
    std::vector<std::string> userIds;  // who reacted
    int64_t firstTimestamp = 0;
};

struct ReactionSummary {
    std::string eventId;
    std::vector<ReactionInfo> reactions;
    int totalReactions = 0;
    std::string topEmoji;      // most used reaction
    int uniqueReactors = 0;
    bool showAll = false;      // from ReactionsSummaryData.showAll — expand collapsed
};

// ---- Reaction Event ----
// Original Kotlin: ReactionEvent data class

struct ReactionEvent {
    std::string eventId;
    std::string senderId;
    std::string key;
    int64_t timestamp = 0;
    std::string relatesToEventId;
};

// ---- Reaction Group ----
// Original Kotlin: ReactionGroup data class

struct ReactionGroup {
    std::string key;
    int count = 0;
    std::vector<std::string> senders;
    int totalCount = 0;
    bool isMyReaction = false;
};

// ---- Reaction List ----
// Original Kotlin: ReactionList data class

struct ReactionList {
    std::vector<ReactionGroup> groups;
    int totalReactions = 0;
};

// ---- Reaction Display Order ----
// Original Kotlin: ReactionDisplayOrder enum

enum class ReactionDisplayOrder {
    COUNT_DESC = 0,
    RECENT_FIRST = 1,
    EMOJI_ORDER = 2,
};

// ---- Reaction Picker Config ----
// Original Kotlin: ReactionPickerConfig data class

struct ReactionPickerConfig {
    int maxDisplayed = 3;
    bool showAllButton = true;
};

// ---- Reaction Emoji Category ----
// Original Kotlin: ReactionEmojiCategory enum

enum class ReactionEmojiCategory {
    SMILEYS = 0,
    GESTURES = 1,
    PEOPLE = 2,
    ANIMALS = 3,
    FOOD = 4,
    TRAVEL = 5,
    ACTIVITIES = 6,
    OBJECTS = 7,
    SYMBOLS = 8,
    FLAGS = 9,
    CUSTOM = 10,
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

// ---- Reaction Grouping & Management ----

// Group reactions by key (emoji).
// Original Kotlin: groupReactions()
ReactionList groupReactions(const std::vector<ReactionEvent>& reactions, const std::string& myUserId);

// Compute reaction groups with counts from events.
// Original Kotlin: computeReactionGroups()
std::vector<ReactionGroup> computeReactionGroups(const std::vector<ReactionEvent>& reactions, const std::string& myUserId);

// Find my reaction to an event.
// Original Kotlin: findMyReaction()
ReactionEvent findMyReaction(const std::vector<ReactionEvent>& reactions, const std::string& myUserId);

// Format a reaction count for display (e.g. "53" → "53", "1024" → "1k").
// Original Kotlin: formatReactionCount()
std::string formatReactionCount(int count);

// Check if a reaction is allowed (not server notice, not encrypted, etc.).
// Original Kotlin: isReactionAllowed()
bool isReactionAllowed(const std::string& eventType, const std::string& roomId);

// Sort reaction groups by display order.
// Original Kotlin: sortReactionGroups()
void sortReactionGroups(std::vector<ReactionGroup>& groups, ReactionDisplayOrder order);

// Get the top N reactions from a list.
// Original Kotlin: getTopReactions()
std::vector<ReactionGroup> getTopReactions(const std::vector<ReactionGroup>& groups, int maxDisplayed);

// Check if a user has reacted to an event.
// Original Kotlin: hasUserReacted()
bool hasUserReacted(const std::vector<ReactionEvent>& reactions, const std::string& userId);

// Build reaction event JSON for adding a reaction.
// Original Kotlin: addReaction()
std::string addReaction(const std::string& relatesToEventId, const std::string& emojiKey);

// Build reaction event JSON for removing a reaction (redact).
// Original Kotlin: removeReaction()
std::string removeReaction(const std::string& reactionEventId);

// Toggle a reaction — add if missing, remove if present.
// Returns the event JSON. outAction is set to "add" or "remove".
// Original Kotlin: toggleReaction()
std::string toggleReaction(const std::string& relatesToEventId, const std::string& emojiKey,
                            const std::vector<ReactionEvent>& existingReactions,
                            const std::string& myUserId, std::string& outAction);

// Build a full m.reaction event JSON.
// Original Kotlin: buildReactionEvent()
std::string buildReactionEvent(const std::string& relatesToEventId, const std::string& emojiKey,
                                const std::string& senderId);

// Parse a m.reaction event from JSON content.
// Original Kotlin: parseReactionEvent()
ReactionEvent parseReactionEvent(const std::string& eventId, const std::string& eventContentJson,
                                  const std::string& senderId, int64_t timestamp, const std::string& relatesToEventId);

} // namespace progressive
