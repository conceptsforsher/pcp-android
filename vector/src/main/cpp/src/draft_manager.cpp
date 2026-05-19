#include "progressive/draft_manager.hpp"
#include <sstream>
#include <chrono>
#include <algorithm>

namespace progressive {

// ---- DraftManager ----

void DraftManager::saveDraft(const MessageDraft& draft) {
    MessageDraft copy = draft;
    if (copy.savedAtMs == 0) {
        copy.savedAtMs = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
    }
    drafts_[draft.roomId] = copy;
}

const MessageDraft* DraftManager::getDraft(const std::string& roomId) const {
    auto it = drafts_.find(roomId);
    if (it != drafts_.end()) return &it->second;
    return nullptr;
}

bool DraftManager::hasDraft(const std::string& roomId) const {
    return drafts_.find(roomId) != drafts_.end();
}

void DraftManager::deleteDraft(const std::string& roomId) {
    drafts_.erase(roomId);
}

std::vector<std::string> DraftManager::getRoomsWithDrafts() const {
    std::vector<std::pair<std::string, int64_t>> sorted;
    for (const auto& p : drafts_) {
        sorted.push_back({p.first, p.second.savedAtMs});
    }
    std::sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) {
        return a.second > b.second; // most recent first
    });
    std::vector<std::string> result;
    for (const auto& p : sorted) result.push_back(p.first);
    return result;
}

std::string DraftManager::exportJson() const {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };
    std::ostringstream json;
    json << "[";
    size_t i = 0;
    for (const auto& p : drafts_) {
        if (i++ > 0) json << ",";
        const auto& d = p.second;
        json << R"({"roomId": ")" << esc(d.roomId) << R"(")";
        json << R"(,"text": ")" << esc(d.text) << R"(")";
        json << R"(,"savedAtMs": )" << d.savedAtMs;
        json << R"(,"cursorPosition": )" << d.cursorPosition;
        if (d.isReply) json << R"(,"replyToEventId": ")" << esc(d.replyToEventId) << R"(")";
        if (d.isEdit) json << R"(,"editEventId": ")" << esc(d.editEventId) << R"(")";
        json << "}";
    }
    json << "]";
    return json.str();
}

void DraftManager::importJson(const std::string& json) {
    size_t pos = 0;
    while (true) {
        pos = json.find("\"roomId\"", pos);
        if (pos == std::string::npos) break;
        auto objStart = json.rfind('{', pos);
        if (objStart == std::string::npos) break;
        int depth = 0; auto objEnd = objStart;
        while (objEnd < json.size()) {
            if (json[objEnd] == '{') ++depth;
            else if (json[objEnd] == '}') --depth;
            if (depth == 0) break;
            ++objEnd;
        }
        if (objEnd >= json.size()) break;

        std::string obj = json.substr(objStart, objEnd - objStart + 1);
        MessageDraft d;
        auto extract = [&](const std::string& key) -> std::string {
            auto s = obj.find('"' + key + "\": \"");
            if (s == std::string::npos) return {};
            s += key.size() + 4;
            auto e = obj.find('"', s);
            return e != std::string::npos ? obj.substr(s, e - s) : std::string{};
        };
        d.roomId = extract("roomId");
        d.text = extract("text");
        d.replyToEventId = extract("replyToEventId");
        d.editEventId = extract("editEventId");
        if (!d.roomId.empty()) drafts_[d.roomId] = d;
        pos = objEnd + 1;
    }
}

void DraftManager::clear() {
    drafts_.clear();
}

// ---- Typing Indicator ----

TypingIndication computeTypingIndicator(const std::vector<ComposerTypingState>& typists, int64_t nowMs) {
    TypingIndication result;

    std::vector<std::string> activeNames;
    for (const auto& t : typists) {
        if (!isTypingExpired(t, nowMs)) {
            activeNames.push_back(t.displayName);
            result.typistIds.push_back(t.userId);
        }
    }

    result.typistCount = static_cast<int>(activeNames.size());
    result.text = formatTypingText(activeNames);
    return result;
}

bool isTypingExpired(const ComposerTypingState& state, int64_t nowMs, int64_t timeoutMs) {
    if (!state.isActive) return true;
    return (nowMs - state.lastTypedAtMs) > timeoutMs;
}

std::string formatTypingText(const std::vector<std::string>& names) {
    if (names.empty()) return "";
    if (names.size() == 1) return names[0] + " is typing...";
    if (names.size() == 2) return names[0] + " and " + names[1] + " are typing...";
    return std::to_string(names.size()) + " people are typing...";
}

} // namespace progressive
