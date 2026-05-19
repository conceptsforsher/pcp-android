#ifndef PROGRESSIVE_LANG_DETECT_HPP
#define PROGRESSIVE_LANG_DETECT_HPP

#include <string>
#include <vector>

namespace progressive {

enum class DetectionMethod { Alphabet, Llm };

struct LanguageResult {
    std::string langCode;     // "en", "ru", "zh", etc.
    std::string langName;     // "English", "Russian"
    float confidence = 0.0f;  // 0.0-1.0
};

// Detect language of text using alphabet analysis.
// Checks which character ranges are most prevalent.
LanguageResult detectLanguageByAlphabet(const std::string& text);

// Detect language using heuristics (fast, no API needed).
LanguageResult detectLanguage(const std::string& text, DetectionMethod method = DetectionMethod::Alphabet);

// Get language display label: "EN", "RU", etc.
std::string getLanguageLabel(const std::string& langCode);

// Check if text is mostly in a given script.
bool isScript(const std::string& text, const std::string& scriptName);

// ---- Language-Based Message Hide ----

struct LanguageHideRule {
    std::string langCode;       // "ru"
    std::string langLabel;      // "RU"
    bool specificUserOnly = false;
    std::string userId;         // if specificUserOnly
    std::string roomId;         // which room
    int64_t hiddenUntilMs = 0;
};

class LanguageHideManager {
public:
    void hideLanguage(const std::string& langCode, const std::string& roomId,
                      const std::string& userId, bool specificUser, int minutes);
    bool isHidden(const std::string& langCode, const std::string& roomId,
                  const std::string& userId) const;
    std::vector<LanguageHideRule> getActiveRules() const;
    void cleanExpired();
    std::string exportJson() const;
    void clear();

private:
    std::vector<LanguageHideRule> rules_;
};

// ---- Chat Push-Down (temporary hide from top) ----

struct ChatPushDown {
    std::string roomId;
    int64_t hiddenUntilMs = 0;
};

class ChatPushDownManager {
public:
    void pushDown(const std::string& roomId, int minutes);
    bool isPushedDown(const std::string& roomId) const;
    void restore(const std::string& roomId);
    std::vector<ChatPushDown> getActive() const;
    void cleanExpired();
    std::string exportJson() const;
    void clear();

private:
    std::vector<ChatPushDown> entries_;
};

// ---- Emoji Blacklist ----

class EmojiBlacklist {
public:
    void add(const std::string& emoji);
    void remove(const std::string& emoji);
    bool isBlocked(const std::string& emoji) const;
    std::vector<std::string> getAll() const;
    void clear();
    std::string exportJson() const;
    void importJson(const std::string& json);

private:
    std::vector<std::string> blocked_;
};

} // namespace progressive

#endif // PROGRESSIVE_LANG_DETECT_HPP
