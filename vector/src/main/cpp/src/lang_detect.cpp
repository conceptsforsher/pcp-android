#include "progressive/lang_detect.hpp"
#include <sstream>
#include <algorithm>
#include <chrono>
#include <unordered_map>

namespace progressive {

// ---- Language Detection ----

static int countInRange(const std::string& text, int low, int high) {
    int count = 0;
    for (unsigned char c : text) {
        if (c >= low && c <= high) count++;
    }
    return count;
}

LanguageResult detectLanguageByAlphabet(const std::string& text) {
    if (text.empty()) return {"", "", 0.0f};

    int totalChars = 0;
    for (unsigned char c : text) {
        if (c > 32) totalChars++; // exclude whitespace
    }
    if (totalChars == 0) return {"en", "English", 0.5f};

    struct Script {
        const char* code;
        const char* name;
        int low, high;
        int count;
    };

    std::vector<Script> scripts = {
        {"ru", "Russian",     0x0400, 0x04FF, countInRange(text, 0x0400, 0x04FF)},
        {"en", "English",     0x0041, 0x007A, countInRange(text, 'A', 'z')},
        {"zh", "Chinese",     0x4E00, 0x9FFF, countInRange(text, 0x4E00, 0x9FFF)},
        {"ja", "Japanese",    0x3040, 0x30FF, countInRange(text, 0x3040, 0x30FF)},
        {"ko", "Korean",      0xAC00, 0xD7AF, countInRange(text, 0xAC00, 0xD7AF)},
        {"ar", "Arabic",      0x0600, 0x06FF, countInRange(text, 0x0600, 0x06FF)},
        {"he", "Hebrew",      0x0590, 0x05FF, countInRange(text, 0x0590, 0x05FF)},
        {"hi", "Hindi",       0x0900, 0x097F, countInRange(text, 0x0900, 0x097F)},
        {"th", "Thai",        0x0E00, 0x0E7F, countInRange(text, 0x0E00, 0x0E7F)},
        {"el", "Greek",       0x0370, 0x03FF, countInRange(text, 0x0370, 0x03FF)},
        {"uk", "Ukrainian",   0x0400, 0x04FF, countInRange(text, 0x0400, 0x04FF)}, // same range as RU, differentiated by i/i
        {"de", "German",      0x0041, 0x007A, 0}, // same as English, differentiated by umlauts
    };

    // Count special chars for German
    int deCount = 0;
    std::string umlauts = "\u00C4\u00E4\u00D6\u00F6\u00DC\u00FC\u00DF"; // ÄäÖöÜüß
    for (unsigned char c : text) {
        if (umlauts.find(std::string(1, c)) != std::string::npos) deCount++;
    }

    // Count special chars for Ukrainian vs Russian
    int uaCount = 0;
    std::string uaChars = "\u0404\u0454\u0406\u0456\u0407\u0457"; // ЄєІіЇї
    for (unsigned char c : text) {
        if (uaChars.find(std::string(1, c)) != std::string::npos) uaCount++;
    }

    // Find the script with most characters
    Script* best = &scripts[0];
    for (auto& s : scripts) {
        if (s.code == std::string("de")) {
            s.count = scripts[1].count + deCount; // same as English + umlauts
        }
        if (s.count > best->count) best = &s;
    }

    // If Cyrillic dominant, check if Ukrainian
    if (best->code == std::string("ru") && uaCount > 0) {
        return {"uk", "Ukrainian", static_cast<float>(best->count) / static_cast<float>(totalChars)};
    }

    float confidence = totalChars > 0
        ? static_cast<float>(best->count) / static_cast<float>(totalChars)
        : 0.0f;

    if (confidence < 0.15f) return {"en", "English", 0.3f}; // default to English
    return {best->code, best->name, confidence};
}

LanguageResult detectLanguage(const std::string& text, DetectionMethod method) {
    switch (method) {
        case DetectionMethod::Llm:
            // LLM detection would be async — for now fallback to alphabet
        case DetectionMethod::Alphabet:
        default:
            return detectLanguageByAlphabet(text);
    }
}

std::string getLanguageLabel(const std::string& langCode) {
    static const std::unordered_map<std::string, std::string> labels = {
        {"en", "EN"}, {"ru", "RU"}, {"zh", "ZH"}, {"ja", "JA"}, {"ko", "KO"},
        {"ar", "AR"}, {"he", "HE"}, {"hi", "HI"}, {"th", "TH"}, {"el", "EL"},
        {"uk", "UK"}, {"de", "DE"}, {"fr", "FR"}, {"es", "ES"}, {"it", "IT"},
        {"pt", "PT"}, {"pl", "PL"}, {"nl", "NL"}, {"sv", "SV"}, {"tr", "TR"},
    };
    auto it = labels.find(langCode);
    if (it != labels.end()) return it->second;

    // Uppercase the code
    std::string upper = langCode;
    std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
    return upper;
}

bool isScript(const std::string& text, const std::string& scriptName) {
    auto result = detectLanguageByAlphabet(text);
    return result.langCode == scriptName;
}

// ---- LanguageHideManager ----

void LanguageHideManager::hideLanguage(const std::string& langCode, const std::string& roomId,
                                       const std::string& userId, bool specificUser, int minutes) {
    LanguageHideRule rule;
    rule.langCode = langCode;
    rule.langLabel = getLanguageLabel(langCode);
    rule.roomId = roomId;
    rule.userId = userId;
    rule.specificUserOnly = specificUser;
    rule.hiddenUntilMs = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count() + (minutes * 60 * 1000LL);
    rules_.push_back(rule);
}

bool LanguageHideManager::isHidden(const std::string& langCode, const std::string& roomId,
                                   const std::string& userId) const {
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();

    for (const auto& r : rules_) {
        if (r.hiddenUntilMs <= now) continue;
        if (r.langCode != langCode) continue;
        if (r.roomId != roomId) continue;
        if (r.specificUserOnly && r.userId != userId) continue;
        return true;
    }
    return false;
}

std::vector<LanguageHideRule> LanguageHideManager::getActiveRules() const {
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    std::vector<LanguageHideRule> result;
    for (const auto& r : rules_) {
        if (r.hiddenUntilMs > now) result.push_back(r);
    }
    return result;
}

void LanguageHideManager::cleanExpired() {
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    rules_.erase(std::remove_if(rules_.begin(), rules_.end(),
        [&](const LanguageHideRule& r) { return r.hiddenUntilMs <= now; }
    ), rules_.end());
}

std::string LanguageHideManager::exportJson() const {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    auto active = getActiveRules();
    std::ostringstream json;
    json << "[";
    for (size_t i = 0; i < active.size(); ++i) {
        if (i > 0) json << ",";
        json << R"({"langCode": ")" << esc(active[i].langCode) << R"(")";
        json << R"(,"langLabel": ")" << esc(active[i].langLabel) << R"(")";
        json << R"(,"roomId": ")" << esc(active[i].roomId) << R"(")";
        json << R"(,"userId": ")" << esc(active[i].userId) << R"(")";
        json << R"(,"specificUserOnly": )" << (active[i].specificUserOnly ? "true" : "false") << "}";
    }
    json << "]";
    return json.str();
}

void LanguageHideManager::clear() {
    rules_.clear();
}

// ---- ChatPushDownManager ----

void ChatPushDownManager::pushDown(const std::string& roomId, int minutes) {
    ChatPushDown entry;
    entry.roomId = roomId;
    entry.hiddenUntilMs = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count() + (minutes * 60 * 1000LL);
    entries_.push_back(entry);
}

bool ChatPushDownManager::isPushedDown(const std::string& roomId) const {
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    for (const auto& e : entries_) {
        if (e.roomId == roomId && e.hiddenUntilMs > now) return true;
    }
    return false;
}

void ChatPushDownManager::restore(const std::string& roomId) {
    entries_.erase(std::remove_if(entries_.begin(), entries_.end(),
        [&](const ChatPushDown& e) { return e.roomId == roomId; }
    ), entries_.end());
}

std::vector<ChatPushDown> ChatPushDownManager::getActive() const {
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    std::vector<ChatPushDown> result;
    for (const auto& e : entries_) {
        if (e.hiddenUntilMs > now) result.push_back(e);
    }
    return result;
}

void ChatPushDownManager::cleanExpired() {
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    entries_.erase(std::remove_if(entries_.begin(), entries_.end(),
        [&](const ChatPushDown& e) { return e.hiddenUntilMs <= now; }
    ), entries_.end());
}

std::string ChatPushDownManager::exportJson() const {
    auto active = getActive();
    std::ostringstream json;
    json << "[";
    for (size_t i = 0; i < active.size(); ++i) {
        if (i > 0) json << ",";
        json << R"({"roomId": ")" << active[i].roomId << R"(")";
        json << R"(,"hiddenUntilMs": )" << active[i].hiddenUntilMs << "}";
    }
    json << "]";
    return json.str();
}

void ChatPushDownManager::clear() {
    entries_.clear();
}

// ---- EmojiBlacklist ----

void EmojiBlacklist::add(const std::string& emoji) {
    if (std::find(blocked_.begin(), blocked_.end(), emoji) == blocked_.end()) {
        blocked_.push_back(emoji);
    }
}

void EmojiBlacklist::remove(const std::string& emoji) {
    blocked_.erase(std::remove(blocked_.begin(), blocked_.end(), emoji), blocked_.end());
}

bool EmojiBlacklist::isBlocked(const std::string& emoji) const {
    return std::find(blocked_.begin(), blocked_.end(), emoji) != blocked_.end();
}

std::vector<std::string> EmojiBlacklist::getAll() const {
    return blocked_;
}

void EmojiBlacklist::clear() {
    blocked_.clear();
}

std::string EmojiBlacklist::exportJson() const {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };
    std::ostringstream json;
    json << "[";
    for (size_t i = 0; i < blocked_.size(); ++i) {
        if (i > 0) json << ",";
        json << R"(")" << esc(blocked_[i]) << R"(")";
    }
    json << "]";
    return json.str();
}

void EmojiBlacklist::importJson(const std::string& json) {
    clear();
    size_t pos = 0;
    while (true) {
        pos = json.find('"', pos);
        if (pos == std::string::npos) break;
        ++pos;
        auto end = json.find('"', pos);
        if (end == std::string::npos) break;
        if (end > pos) blocked_.push_back(json.substr(pos, end - pos));
        pos = end + 1;
    }
}

} // namespace progressive
