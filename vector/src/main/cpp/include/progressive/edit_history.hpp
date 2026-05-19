#ifndef PROGRESSIVE_EDIT_HISTORY_HPP
#define PROGRESSIVE_EDIT_HISTORY_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// ---- Message Edit History ----
// Ported from: im.vector.app.features.home.room.detail.timeline.action.MessageActionsViewModel.kt
//              (ViewEditHistory action handling)
//              org.matrix.android.sdk.api.session.room.model.EditAggregatedSummary (Realm model)

struct EditEntry {
    std::string editEventId;
    std::string editorId;
    std::string editorName;
    std::string newBody;           // the edited body text
    std::string previousBody;      // body before this edit
    int64_t editedAtMs = 0;
    bool isLocalEcho = false;      // edit hasn't been confirmed by server yet
};

struct EditHistory {
    std::string originalEventId;
    std::string originalBody;
    std::string originalSenderId;
    std::string originalSenderName;
    int64_t originalSentAtMs = 0;
    int editCount = 0;
    std::vector<EditEntry> edits;  // sorted newest first
    bool hasPendingEdits = false;  // local echo edits not yet synced
};

// Parse edit history from event annotations summary JSON.
// Original Kotlin (MessageActionsViewModel.kt:364-366):
//   if (timelineEvent.hasBeenEdited()) {
//       add(EventSharedAction.ViewEditHistory(informationData))
//   }
EditHistory parseEditHistory(const std::string& originalEventId, const std::string& originalBody,
    const std::string& originalSenderId, const std::string& originalSenderName,
    int64_t originalSentAtMs, const std::string& annotationsJson);

// Format edit history for display in bottom sheet.
// Original Kotlin (ViewEditHistoryBottomSheet):
//   For each edition, shows: "Editor name" + timestamp + new body preview
std::string formatEditHistory(const EditHistory& history);

// Format a single edit entry for display.
std::string formatEditEntry(const EditEntry& entry, int index);

// Detect if there are edits that haven't been synced.
bool hasPendingEdits(const EditHistory& history);

// Get the most recent edit body (what's currently displayed).
std::string getCurrentBody(const EditHistory& history);

// Get the edit count badge text: "Edited" or "Edited 3 times".
std::string getEditBadgeText(int editCount);

// Get the edit count badge text with natural language variants.
// 1 → "(edited)", 2 → "(edited twice)", 3 → "(edited 3 times)", 5 → "(edited 5 times)"
std::string getEditCountBadge(int editCount);

// Compute a short diff summary between two edit bodies.
// "Hello world" → "Hello everyone" returns "+2/-2 chars"
std::string computeEditDiffSummary(const std::string& oldBody, const std::string& newBody);

// Format a one-line edit summary for stacking display.
// "Edited by Alice: +5/-2 chars (changed 'world' to 'everyone')"
std::string formatEditSummary(const EditEntry& entry);

// Check if edit stacking should be collapsed (2+ edits → collapsed by default).
bool shouldCollapseEdits(const EditHistory& history, int threshold = 2);

// Format the expanded edit list for display when uncollapsed.
std::string formatExpandedEditList(const EditHistory& history);

} // namespace progressive

#endif // PROGRESSIVE_EDIT_HISTORY_HPP
