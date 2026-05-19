#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// ================================================================
// Text Undo Manager — protect against accidental text deletion
//
// Labs feature: adds undo/redo stack to both plain and rich text editors.
// Protects against: select-all-then-paste, accidental cut, swipe-delete.
//
// Configurable via Labs:
//   - Undo depth (default: 50 entries)
//   - Auto-checkpoint before paste if text > threshold
//   - Checkpoint before select-all operations
// ================================================================

// ---- Undo Entry (one checkpoint) ----

struct UndoEntry {
    std::string text;                // Full text at this checkpoint
    int cursorPos = 0;               // Cursor position to restore
    int64_t timestampMs = 0;         // When checkpoint was created
    std::string description;         // "typed", "paste", "cut", "select_all"
};

// ---- Undo Config (all Labs-adjustable) ----

struct UndoConfig {
    bool enabled = true;
    int maxDepth = 50;               // Max undo entries (50 = ~250KB with 5KB messages)
    int autoCheckpointBytes = 100;   // Auto-save if text changed by > this many bytes
    bool checkpointBeforePaste = true;   // Save before paste (most common accident)
    bool checkpointOnSelectAll = true;   // Save before select-all
    bool restoreCursor = true;           // Restore cursor position on undo
    int debounceMs = 500;                // Don't checkpoint more often than this
};

// ---- Text Undo Manager ----

class TextUndoManager {
public:
    TextUndoManager();

    // ====== Config ======

    void setConfig(const UndoConfig& config);
    UndoConfig getConfig() const;

    // ====== Checkpoint (save state) ======

    // Save current text state. Called before risky operations.
    void checkpoint(const std::string& text, int cursorPos, const std::string& description = "");

    // Save if text changed enough (auto-checkpoint during typing).
    void autoCheckpoint(const std::string& text, int cursorPos);

    // ====== Undo/Redo ======

    // Undo to previous state. Returns (text, cursorPos).
    // Returns empty string if nothing to undo.
    std::string undo(std::string& outText, int& outCursorPos);

    // Redo after undo. Returns (text, cursorPos).
    std::string redo(std::string& outText, int& outCursorPos);

    // Check if undo is available.
    bool canUndo() const;

    // Check if redo is available.
    bool canRedo() const;

    // Get description of next undo action.
    std::string nextUndoDescription() const;

    // Get description of next redo action.
    std::string nextRedoDescription() const;

    // ====== Special Operations ======

    // Called when user selects all. Saves checkpoint if configured.
    void onSelectAll(const std::string& text, int cursorPos);

    // Called before paste. Saves checkpoint if pasted text is large.
    void onBeforePaste(const std::string& currentText, int cursorPos,
                        const std::string& pastedText);

    // Called before cut. Saves checkpoint.
    void onBeforeCut(const std::string& text, int cursorPos,
                      int selStart, int selEnd);

    // ====== State ======

    int undoCount() const { return static_cast<int>(undoStack_.size()); }
    int redoCount() const { return static_cast<int>(redoStack_.size()); }
    void clear();
    void reset();

    // ====== Serialization ======

    std::string stateToJson() const;
    std::string configToJson() const;

private:
    UndoConfig config_;
    std::vector<UndoEntry> undoStack_;
    std::vector<UndoEntry> redoStack_;
    std::string lastCheckpointText_;
    int64_t lastCheckpointMs_ = 0;

    void pushUndo(const std::string& text, int cursorPos, const std::string& description);
    void truncateStack();
};

} // namespace progressive
