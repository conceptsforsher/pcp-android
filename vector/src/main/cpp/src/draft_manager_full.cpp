#include "progressive/draft_manager_full.hpp"
#include <sstream>
#include <algorithm>
#include <ctime>

namespace progressive {

// ====== Enum ======

const char* draftTypeToString(DraftType type) {
    switch (type) {
        case DraftType::REGULAR: return "regular";
        case DraftType::QUOTE: return "quote";
        case DraftType::EDIT: return "edit";
        case DraftType::REPLY: return "reply";
        case DraftType::VOICE: return "voice";
    }
    return "regular";
}

DraftType draftTypeFromString(const std::string& s) {
    if (s == "regular") return DraftType::REGULAR;
    if (s == "quote") return DraftType::QUOTE;
    if (s == "edit") return DraftType::EDIT;
    if (s == "reply") return DraftType::REPLY;
    if (s == "voice") return DraftType::VOICE;
    return DraftType::REGULAR;
}

// ====== UserDraft ======
// Original: isValid() — Regular: content.isNotBlank(), rest: true

bool UserDraft::checkValid() const {
    if (type == DraftType::REGULAR) return !content.empty() && content.find_first_not_of(" \t\n\r") != std::string::npos;
    return true;
}

// ====== Constructor ======

FullDraftManager::FullDraftManager() {}

// ====== Draft Lifecycle ======
// Original: DraftService.saveDraft / deleteDraft / getDraft

void FullDraftManager::saveDraft(const std::string& roomId, const UserDraft& draft) {
    if (draft.content.size() > static_cast<size_t>(config_.maxDraftLength)) {
        UserDraft truncated = draft;
        truncated.content = draft.content.substr(0, config_.maxDraftLength);
        truncated.savedAtMs = static_cast<int64_t>(std::time(nullptr)) * 1000;
        drafts_[roomId] = truncated;
    } else {
        UserDraft saved = draft;
        saved.savedAtMs = static_cast<int64_t>(std::time(nullptr)) * 1000;
        saved.roomId = roomId;
        drafts_[roomId] = saved;
    }
}

void FullDraftManager::deleteDraft(const std::string& roomId) {
    drafts_.erase(roomId);
}

bool FullDraftManager::getDraft(const std::string& roomId, UserDraft& out) const {
    auto it = drafts_.find(roomId);
    if (it == drafts_.end()) return false;
    out = it->second;
    return true;
}

bool FullDraftManager::hasDraft(const std::string& roomId) const {
    return drafts_.find(roomId) != drafts_.end();
}

// ====== Live Draft ======
// Original: auto-save with character threshold + space detection

void FullDraftManager::setLiveDraftConfig(const LiveDraftConfig& config) { config_ = config; }
LiveDraftConfig FullDraftManager::getLiveDraftConfig() const { return config_; }

bool FullDraftManager::qualifiesForAutoSave(const std::string& text) const {
    if (!config_.enabled) return false;
    if (text.empty()) return false;

    // Count non-whitespace characters
    int charCount = 0;
    bool hasSpace = false;
    for (char c : text) {
        if (c != ' ' && c != '\t' && c != '\n' && c != '\r') charCount++;
        else hasSpace = true;
    }

    return charCount >= config_.characterThreshold && hasSpace;
}

bool FullDraftManager::autoSaveIfQualified(const std::string& roomId, const std::string& text) {
    if (!qualifiesForAutoSave(text)) return false;

    UserDraft draft;
    draft.type = DraftType::REGULAR;
    draft.content = config_.draftPrefix + text;
    draft.roomId = roomId;
    draft.isValid = true;
    saveDraft(roomId, draft);
    return true;
}

std::string FullDraftManager::stripDraftPrefix(const std::string& text) const {
    if (!config_.finalEditRemovesPrefix) return text;

    if (text.rfind(config_.draftPrefix, 0) == 0) {
        return text.substr(config_.draftPrefix.size());
    }
    return text;
}

// ====== Draft Types ======

UserDraft FullDraftManager::buildRegular(const std::string& content) {
    UserDraft d;
    d.type = DraftType::REGULAR;
    d.content = content;
    d.isValid = d.checkValid();
    return d;
}

UserDraft FullDraftManager::buildQuote(const std::string& linkedEventId, const std::string& content) {
    UserDraft d;
    d.type = DraftType::QUOTE;
    d.linkedEventId = linkedEventId;
    d.content = content;
    d.isValid = true;
    return d;
}

UserDraft FullDraftManager::buildEdit(const std::string& linkedEventId, const std::string& content) {
    UserDraft d;
    d.type = DraftType::EDIT;
    d.linkedEventId = linkedEventId;
    d.content = content;
    d.isValid = true;
    return d;
}

UserDraft FullDraftManager::buildReply(const std::string& linkedEventId, const std::string& content) {
    UserDraft d;
    d.type = DraftType::REPLY;
    d.linkedEventId = linkedEventId;
    d.content = content;
    d.isValid = true;
    return d;
}

UserDraft FullDraftManager::buildVoice(const std::string& content) {
    UserDraft d;
    d.type = DraftType::VOICE;
    d.content = content;
    d.isValid = true;
    return d;
}

// ====== Validation ======

bool FullDraftManager::isValidDraft(const UserDraft& draft) { return draft.checkValid(); }
bool FullDraftManager::isContentTooLong(const std::string& content) const { return content.size() > static_cast<size_t>(config_.maxDraftLength); }

// ====== Queries ======

std::vector<std::string> FullDraftManager::getRoomsWithDrafts() const {
    std::vector<std::string> rooms;
    for (const auto& [roomId, draft] : drafts_) rooms.push_back(roomId);
    return rooms;
}

void FullDraftManager::clearAll() { drafts_.clear(); }

// ====== Serialization ======

std::string FullDraftManager::draftToJson(const UserDraft& draft) const {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    std::ostringstream os;
    os << R"({"type":")" << draftTypeToString(draft.type)
       << R"(","content":")" << esc(draft.content)
       << R"(","room_id":")" << esc(draft.roomId)
       << R"(","linked_event_id":")" << esc(draft.linkedEventId)
       << R"(","saved_at":)" << draft.savedAtMs
       << R"(,"is_valid":)" << (draft.checkValid() ? "true" : "false")
       << "}";
    return os.str();
}

std::string FullDraftManager::configToJson() const {
    std::ostringstream os;
    os << R"({"enabled":)" << (config_.enabled ? "true" : "false")
       << R"(,"char_threshold":)" << config_.characterThreshold
       << R"(,"update_interval_ms":)" << config_.updateIntervalMs
       << R"(,"draft_prefix":")" << config_.draftPrefix
       << R"(","remove_prefix":)" << (config_.finalEditRemovesPrefix ? "true" : "false")
       << "}";
    return os.str();
}

} // namespace progressive
