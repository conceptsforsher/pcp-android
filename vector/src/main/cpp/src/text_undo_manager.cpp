#include "progressive/text_undo_manager.hpp"
#include <sstream>
#include <algorithm>
#include <ctime>

namespace progressive {

// ====== Constructor ======

TextUndoManager::TextUndoManager() {}

void TextUndoManager::setConfig(const UndoConfig& config) { config_ = config; }
UndoConfig TextUndoManager::getConfig() const { return config_; }

// ====== Stack Management ======

void TextUndoManager::pushUndo(const std::string& text, int cursorPos, const std::string& description) {
    UndoEntry entry;
    entry.text = text;
    entry.cursorPos = cursorPos;
    entry.timestampMs = static_cast<int64_t>(std::time(nullptr)) * 1000;
    entry.description = description;
    undoStack_.push_back(entry);
    truncateStack();

    // Clear redo stack on new action
    redoStack_.clear();
}

void TextUndoManager::truncateStack() {
    while (static_cast<int>(undoStack_.size()) > config_.maxDepth) {
        undoStack_.erase(undoStack_.begin());
    }
}

// ====== Checkpoint ======

void TextUndoManager::checkpoint(const std::string& text, int cursorPos, const std::string& description) {
    if (!config_.enabled) return;

    auto now = static_cast<int64_t>(std::time(nullptr)) * 1000;

    // Debounce: don't checkpoint if too recent
    if (now - lastCheckpointMs_ < config_.debounceMs && !lastCheckpointText_.empty()) return;

    // Don't checkpoint if text hasn't changed
    if (text == lastCheckpointText_) return;

    lastCheckpointText_ = text;
    lastCheckpointMs_ = now;
    pushUndo(text, cursorPos, description.empty() ? "manual" : description);
}

void TextUndoManager::autoCheckpoint(const std::string& text, int cursorPos) {
    if (!config_.enabled) return;

    // Auto-checkpoint if text changed more than threshold
    int diff = static_cast<int>(text.size()) - static_cast<int>(lastCheckpointText_.size());
    if (std::abs(diff) >= config_.autoCheckpointBytes) {
        checkpoint(text, cursorPos, "typed");
    }
}

// ====== Undo/Redo ======

std::string TextUndoManager::undo(std::string& outText, int& outCursorPos) {
    if (undoStack_.empty()) return "";

    // Move current state to redo stack
    UndoEntry current;
    current.text = lastCheckpointText_;
    current.cursorPos = 0;
    current.timestampMs = static_cast<int64_t>(std::time(nullptr)) * 1000;
    current.description = "";
    redoStack_.push_back(current);

    // Pop undo entry
    auto entry = undoStack_.back();
    undoStack_.pop_back();

    outText = entry.text;
    outCursorPos = config_.restoreCursor ? entry.cursorPos : 0;
    lastCheckpointText_ = entry.text;
    lastCheckpointMs_ = entry.timestampMs;

    return entry.description;
}

std::string TextUndoManager::redo(std::string& outText, int& outCursorPos) {
    if (redoStack_.empty()) return "";

    // Save current to undo stack
    pushUndo(lastCheckpointText_, 0, "");

    auto entry = redoStack_.back();
    redoStack_.pop_back();

    outText = entry.text;
    outCursorPos = entry.cursorPos;
    lastCheckpointText_ = entry.text;
    lastCheckpointMs_ = entry.timestampMs;

    return entry.description;
}

bool TextUndoManager::canUndo() const {
    return !undoStack_.empty() && config_.enabled;
}

bool TextUndoManager::canRedo() const {
    return !redoStack_.empty() && config_.enabled;
}

std::string TextUndoManager::nextUndoDescription() const {
    if (undoStack_.empty()) return "";
    return undoStack_.back().description;
}

std::string TextUndoManager::nextRedoDescription() const {
    if (redoStack_.empty()) return "";
    return redoStack_.back().description;
}

// ====== Special Operations ======

void TextUndoManager::onSelectAll(const std::string& text, int cursorPos) {
    if (config_.checkpointOnSelectAll && config_.enabled) {
        checkpoint(text, cursorPos, "select_all");
    }
}

void TextUndoManager::onBeforePaste(const std::string& currentText, int cursorPos,
                                     const std::string& pastedText) {
    if (!config_.enabled || !config_.checkpointBeforePaste) return;

    // Only checkpoint if pasted text is large enough to be an accident
    // (small pastes like emoji are fine)
    if (pastedText.size() > static_cast<size_t>(config_.autoCheckpointBytes)) {
        checkpoint(currentText, cursorPos, "paste");
    }
}

void TextUndoManager::onBeforeCut(const std::string& text, int cursorPos,
                                   int selStart, int selEnd) {
    if (!config_.enabled) return;

    // Always checkpoint before cut (could lose data)
    int cutLen = selEnd - selStart;
    if (cutLen >= config_.autoCheckpointBytes) {
        checkpoint(text, cursorPos, "cut");
    }
}

void TextUndoManager::clear() {
    undoStack_.clear();
    redoStack_.clear();
}

void TextUndoManager::reset() {
    clear();
    lastCheckpointText_.clear();
    lastCheckpointMs_ = 0;
}

// ====== Serialization ======

std::string TextUndoManager::stateToJson() const {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    std::ostringstream os;
    os << R"({"can_undo":)" << (canUndo() ? "true" : "false")
       << R"(,"can_redo":)" << (canRedo() ? "true" : "false")
       << R"(,"undo_count":)" << undoCount()
       << R"(,"redo_count":)" << redoCount()
       << R"(,"next_undo":")" << esc(nextUndoDescription()) << R"(")"
       << R"(,"next_redo":")" << esc(nextRedoDescription()) << R"(")"
       << R"(,"enabled":)" << (config_.enabled ? "true" : "false")
       << "}";
    return os.str();
}

std::string TextUndoManager::configToJson() const {
    std::ostringstream os;
    os << R"({"enabled":)" << (config_.enabled ? "true" : "false")
       << R"(,"max_depth":)" << config_.maxDepth
       << R"(,"auto_checkpoint_bytes":)" << config_.autoCheckpointBytes
       << R"(,"checkpoint_before_paste":)" << (config_.checkpointBeforePaste ? "true" : "false")
       << R"(,"checkpoint_on_select_all":)" << (config_.checkpointOnSelectAll ? "true" : "false")
       << R"(,"restore_cursor":)" << (config_.restoreCursor ? "true" : "false")
       << R"(,"debounce_ms":)" << config_.debounceMs
       << "}";
    return os.str();
}

} // namespace progressive
