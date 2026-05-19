#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <cstdint>

namespace progressive {

// ---- Existing types ----

struct SpellCandidate {
    std::string word;
    int distance = 0;   // lower = better match
    double score = 0.0;  // 1.0 = perfect
};

class SpellChecker {
public:
    void loadDictionary(const std::string& words);
    [[nodiscard]] bool isKnown(const std::string& word) const;
    [[nodiscard]] std::vector<SpellCandidate> suggest(const std::string& word, int maxResults = 5) const;
    void addWord(const std::string& word);
    void removeWord(const std::string& word);
    [[nodiscard]] size_t wordCount() const { return dictionary_.size(); }
    void clear();

    static int editDistance(const std::string& a, const std::string& b);
    static double jaroWinkler(const std::string& a, const std::string& b);
    static std::string soundex(const std::string& word);
    static bool phoneticMatch(const std::string& a, const std::string& b);

private:
    std::unordered_set<std::string> dictionary_;
};

struct TypoResult {
    bool hasTypo = false;
    std::string word;
    std::vector<SpellCandidate> suggestions;
};

std::vector<TypoResult> detectTypos(const std::string& sentence, const SpellChecker& checker);
std::vector<std::string> tokenizeForSpellcheck(const std::string& sentence);
bool looksTypo(const std::string& word);

// ---- New types and functions ----

// Original Kotlin: SpellCheck.kt — language configuration
struct SpellCheckLanguage {
    std::string code;      // e.g. "en", "ru", "fr"
    std::string name;      // e.g. "English", "Russian", "French"
    std::vector<std::string> dictionary; // word list
};

// Original Kotlin: SpellCheck.kt — result for a single spelling check
struct SpellCheckResult {
    std::string word;
    std::vector<std::string> suggestions;
    bool isCorrect = false;
    int position = -1; // byte offset in original text
};

// Original Kotlin: SpellCheck.kt — configuration
struct SpellCheckConfig {
    bool enabled = true;
    std::vector<std::string> languages; // e.g. {"en", "ru"}
    bool ignoreCapitalized = true;
    bool ignoreNumbers = true;
    bool ignoreUrls = true;
    int maxSuggestions = 5;
};

// Original Kotlin: SpellCheck.kt — check spelling of text with config
// Returns a list of results for misspelled words.
std::vector<SpellCheckResult> checkSpelling(
    const std::string& text,
    const SpellCheckConfig& config,
    SpellChecker& checker);

// Original Kotlin: SpellCheck.kt — get suggestions using Levenshtein distance
std::vector<std::string> getSpellSuggestions(
    const std::string& word,
    const std::unordered_set<std::string>& dictionary,
    int maxSuggestions = 5);

// Original Kotlin: SpellCheck.kt — compute Levenshtein distance
int computeLevenshteinDistance(const std::string& a, const std::string& b);

// Original Kotlin: SpellCheck.kt — dictionary lookup helper
bool isWordInDictionary(const std::string& word, const std::unordered_set<std::string>& dictionary);

// Load dictionary from word list vector
void loadDictionary(const std::string& wordList, std::unordered_set<std::string>& outDict);

// Original Kotlin: SpellCheck.kt — list of available languages
std::vector<SpellCheckLanguage> getSupportedLanguages();

// Original Kotlin: SpellCheck.kt — basic language detection from text sample
std::string detectLanguage(const std::string& text);

// Original Kotlin: SpellCheck.kt — position-based error mark
struct SpellCheckMark {
    std::string word;
    int startPos = 0;
    int endPos = 0;
    std::vector<std::string> suggestions;
};

// Original Kotlin: SpellCheck.kt — find and mark spelling errors in text with positions
std::vector<SpellCheckMark> markSpellingErrors(
    const std::string& text,
    const SpellCheckConfig& config,
    SpellChecker& checker);

// Original Kotlin: SpellCheck.kt — build word list from frequency data
// Input: space-separated or newline-separated words, optionally with counts.
std::vector<std::string> buildDictionaryWordList(const std::string& frequencyData);

} // namespace progressive
