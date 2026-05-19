#pragma once

#include <string>
#include <cstdint>

namespace progressive {

// ==== Live Draft (Auto-Save Messages While Typing) ====
//
// Sends draft messages at configurable intervals as the user types.
// Uses m.replace (edit) events to update the draft without spamming the room.
// Final send clears the "draft: " prefix.
//
// WARNING: Client-side deletions via redaction are NOT guaranteed to be
// honoured by other clients. Sensitive content typed as draft may be
// readable by anyone in the room until final edit removes it.
// Once Matrix supports native ephemeral/draft messages, Progressive
// will migrate to the spec-compliant approach.

struct LiveDraftConfig {
    bool enabled = false;               // Master toggle
    int characterThreshold = 20;        // Min characters (with at least one space) to trigger
    int updateIntervalMs = 3000;        // How often to update the draft (millis)
    std::string draftPrefix = "draft: "; // Prefix added to draft messages
    bool finalEditRemovesPrefix = true;  // Always true — security feature

    bool isValid() const {
        return enabled && characterThreshold > 0 && updateIntervalMs > 0;
    }
};

// Check if the typed text qualifies for draft auto-send.
// Requires at least one space AND crossing the character threshold.
//
// Original logic:
//   1. Count characters
//   2. Check for any space character (0x20, \\u00A0, etc.)
//   3. Must exceed threshold

inline bool shouldAutoDraft(const std::string& text, int threshold) {
    if (threshold <= 0 || (int)text.size() < threshold) return false;
    for (char c : text) {
        if (c == ' ' || c == '\t' || c == '\n') return true;
    }
    return false;
}

// Compute the next update timestamp for the draft.
// Returns current time + interval in epoch millis.

inline int64_t nextDraftUpdateMs(int64_t currentTimeMs, int intervalMs) {
    return currentTimeMs + intervalMs;
}

// Build the initial draft message body.
// Prefix + the typed text. The prefix will be removed on final send.

inline std::string buildDraftMessage(const std::string& prefix, const std::string& text) {
    return prefix + text;
}

// Build the final message body (without draft prefix).
// Called when user explicitly sends.

inline std::string finalizeDraftMessage(const std::string& fullDraftText, const std::string& prefix) {
    if (fullDraftText.compare(0, prefix.size(), prefix) == 0) {
        return fullDraftText.substr(prefix.size());
    }
    return fullDraftText;
}

// Serialize LiveDraftConfig for preferences storage.
std::string liveDraftConfigToJson(const LiveDraftConfig& config);
LiveDraftConfig liveDraftConfigFromJson(const std::string& json);

// Format a warning message for the settings UI.
inline const char* liveDraftWarning() {
    return "Live drafts use edit events to update text as you type. "
           "Previous versions of your message are visible in the edit history. "
           "While the client deletes old drafts via redaction, other Matrix "
           "clients may not honour deletions and could display earlier versions. "
           "Avoid typing sensitive information before finalizing. "
           "Once Matrix supports native draft/ephemeral messages, "
           "Progressive will use the spec-compliant approach.";
}

} // namespace progressive
