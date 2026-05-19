#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>

namespace progressive {

// ================================================================
// Draft Manager — message draft storage & auto-save
//
// Faithful port from Element Android original sources:
//   DraftService.kt — saveDraft, deleteDraft, getDraft, getDraftLive
//   UserDraft.kt — Regular(content), Quote(linkedEventId, content),
//     Edit(linkedEventId, content), Reply(linkedEventId, content),
//     Voice(content), isValid() (Regular: content.isNotBlank)
//
// Covers:
//   1. Draft types (Regular, Quote, Edit, Reply, Voice)
//   2. Save/delete/get draft per room
//   3. Live draft auto-save with character threshold
//   4. Draft validation
//   5. Draft prefix stripping on send
// ================================================================

// ---- Draft Type ----
// Original: UserDraft sealed interface (Regular, Quote, Edit, Reply, Voice)

enum class DraftType {
    REGULAR = 0,         // Normal message
    QUOTE = 1,           // Quoting another message
    EDIT = 2,            // Editing a message
    REPLY = 3,           // Replying to a message
    VOICE = 4,           // Voice message draft
};

const char* draftTypeToString(DraftType type);
DraftType draftTypeFromString(const std::string& s);

// ---- User Draft ----
// Original: UserDraft.kt (Regular, Quote, Edit, Reply, Voice)
// Original: isValid() — Regular: content.isNotBlank(), rest: true

struct UserDraft {
    DraftType type = DraftType::REGULAR;
    std::string content;             // Draft text/content
    std::string linkedEventId;       // For Quote/Edit/Reply — target event
    std::string roomId;
    int64_t savedAtMs = 0;          // When last saved
    bool isValid = false;

    // Original: isValid() check
    bool checkValid() const;
};

// ---- Live Draft Config ----
// Original: live draft settings (character threshold, update interval)

struct LiveDraftConfig {
    bool enabled = false;                // Master toggle
    int characterThreshold = 20;         // Min chars (with at least one space)
    int updateIntervalMs = 3000;         // Auto-save interval
    std::string draftPrefix = "draft: "; // Prefix for auto-saved drafts
    bool finalEditRemovesPrefix = true;  // Strip prefix on final send
    int maxDraftsPerRoom = 1;            // Only 1 draft per room
    int maxDraftLength = 65535;          // Max content length
};

// ---- Draft Manager ----

class FullDraftManager {
public:
    FullDraftManager();

    // ====== Draft Lifecycle ======
    // Original: DraftService.saveDraft(draft) / deleteDraft() / getDraft()

    // Save a draft for a room.
    void saveDraft(const std::string& roomId, const UserDraft& draft);

    // Delete the draft for a room.
    void deleteDraft(const std::string& roomId);

    // Get the current draft for a room.
    bool getDraft(const std::string& roomId, UserDraft& out) const;

    // Check if a room has a saved draft.
    bool hasDraft(const std::string& roomId) const;

    // ====== Live Draft (Auto-Save) ======

    // Configure live draft settings.
    void setLiveDraftConfig(const LiveDraftConfig& config);

    // Get live draft config.
    LiveDraftConfig getLiveDraftConfig() const;

    // Check if typed text qualifies for auto-save.
    // Original: requires at least characterThreshold chars and one space
    bool qualifiesForAutoSave(const std::string& text) const;

    // Auto-save if threshold met. Returns true if saved.
    bool autoSaveIfQualified(const std::string& roomId, const std::string& text);

    // Strip the draft prefix from text on final send.
    std::string stripDraftPrefix(const std::string& text) const;

    // ====== Draft Types ======

    // Build a regular draft.
    static UserDraft buildRegular(const std::string& content);

    // Build a quote draft.
    static UserDraft buildQuote(const std::string& linkedEventId, const std::string& content);

    // Build an edit draft.
    static UserDraft buildEdit(const std::string& linkedEventId, const std::string& content);

    // Build a reply draft.
    static UserDraft buildReply(const std::string& linkedEventId, const std::string& content);

    // Build a voice draft.
    static UserDraft buildVoice(const std::string& content);

    // ====== Validation ======

    // Check if content is valid for a draft type.
    static bool isValidDraft(const UserDraft& draft);

    // Check if content exceeds max length.
    bool isContentTooLong(const std::string& content) const;

    // ====== Queries ======

    // Get all rooms with drafts.
    std::vector<std::string> getRoomsWithDrafts() const;

    // Get total draft count.
    int totalDrafts() const { return static_cast<int>(drafts_.size()); }

    // Clear all drafts.
    void clearAll();

    // ====== Serialization ======

    // Export draft as JSON.
    std::string draftToJson(const UserDraft& draft) const;

    // Export live draft config as JSON.
    std::string configToJson() const;

private:
    std::unordered_map<std::string, UserDraft> drafts_; // roomId → draft
    LiveDraftConfig config_;
};

} // namespace progressive
