#ifndef PROGRESSIVE_DRAFT_MANAGER_HPP
#define PROGRESSIVE_DRAFT_MANAGER_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>

namespace progressive {

struct MessageDraft {
    std::string roomId;
    std::string text;
    std::string formattedText;
    std::string replyToEventId;  // if replying
    std::string editEventId;     // if editing
    int64_t savedAtMs = 0;
    int cursorPosition = 0;      // caret position
    bool isReply = false;
    bool isEdit = false;
    bool isQuote = false;
    int type = 0;
    std::string content;
    std::string linkedEventId;
    bool isValidDraft = false;
    bool hasAttachment = false;

    // Check if draft is valid (from UserDraft.kt:33-38).
    // Regular drafts: content must be non-blank. Quotes/Edits/Replies: always valid.
    bool isValid() const { return !text.empty() || isReply || isEdit || isQuote; }
};

class DraftManager {
public:
    // Save a draft for a room.
    void saveDraft(const MessageDraft& draft);

    // Load the draft for a room (returns empty if none).
    const MessageDraft* getDraft(const std::string& roomId) const;

    // Check if a room has a draft.
    bool hasDraft(const std::string& roomId) const;

    // Delete the draft for a room.
    void deleteDraft(const std::string& roomId);

    // Get all rooms with drafts (sorted by recency).
    std::vector<std::string> getRoomsWithDrafts() const;

    // Get draft count.
    int count() const { return static_cast<int>(drafts_.size()); }

    // Export all drafts as JSON.
    std::string exportJson() const;

    // Import drafts from JSON.
    void importJson(const std::string& json);

    // Clear all drafts.
    void clear();
    void autoSaveIfQualified(const std::string&, const std::string&) {}
    std::string stripDraftPrefix(const std::string& t) { return t; }

private:
    std::unordered_map<std::string, MessageDraft> drafts_;
};

// ---- Typing Indicator Logic ----

struct ComposerTypingState {
    std::string roomId;
    std::string userId;
    std::string displayName;
    int64_t startedAtMs = 0;
    int64_t lastTypedAtMs = 0;
    bool isActive = false;
};

struct TypingIndication {
    std::string text;            // "Alice is typing..." or "Alice, Bob are typing..."
    std::vector<std::string> typistIds;
    int typistCount = 0;
};

// Compute the typing indicator text from active typists.
TypingIndication computeTypingIndicator(const std::vector<ComposerTypingState>& typists, int64_t nowMs);

bool isTypingExpired(const ComposerTypingState& state, int64_t nowMs, int64_t timeoutMs = 30000);

// Format typing indicator: "Alice is typing...", "Alice and Bob are typing...", "3 people are typing..."
std::string formatTypingText(const std::vector<std::string>& names);

enum class DraftType { REGULAR = 0, REPLY = 1, EDIT = 2, QUOTE = 3 };
using UserDraft = MessageDraft;
} // namespace progressive

#endif // PROGRESSIVE_DRAFT_MANAGER_HPP
