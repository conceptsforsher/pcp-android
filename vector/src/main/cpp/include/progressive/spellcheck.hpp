#ifndef PROGRESSIVE_SPELLCHECK_HPP
#define PROGRESSIVE_SPELLCHECK_HPP

#include <string>
#include <vector>
#include <unordered_set>

namespace progressive {

// ---- Simple Spell Checker (Damerau-Levenshtein) ----

struct SpellCandidate {
    std::string word;
    int distance = 0;   // lower = better match
    double score = 0.0;  // 1.0 = perfect
};

class SpellChecker {
public:
    // Load a dictionary word list (one word per line).
    void loadDictionary(const std::string& words);

    // Check if a word is in the dictionary.
    bool isKnown(const std::string& word) const;

    // Find corrections for a misspelled word.
    std::vector<SpellCandidate> suggest(const std::string& word, int maxResults = 5) const;

    // Add a custom word to the dictionary.
    void addWord(const std::string& word);

    // Remove a custom word.
    void removeWord(const std::string& word);

    size_t wordCount() const { return dictionary_.size(); }
    void clear();

    // Compute Damerau-Levenshtein edit distance.
    static int editDistance(const std::string& a, const std::string& b);

    // Compute Jaro-Winkler similarity (0.0 to 1.0).
    static double jaroWinkler(const std::string& a, const std::string& b);

    // Check if two strings are phonetically similar (Soundex).
    static std::string soundex(const std::string& word);
    static bool phoneticMatch(const std::string& a, const std::string& b);

private:
    std::unordered_set<std::string> dictionary_;
};

// ---- Typo Detection ----

struct TypoResult {
    bool hasTypo = false;
    std::string word;
    std::vector<SpellCandidate> suggestions;
};

// Detect typos in a sentence and suggest corrections.
std::vector<TypoResult> detectTypos(const std::string& sentence, const SpellChecker& checker);

// Tokenize a sentence into words (respects punctuation).
std::vector<std::string> tokenizeForSpellcheck(const std::string& sentence);

// Check if a word is likely misspelled (heuristic: uncommon letter patterns).
bool looksTypo(const std::string& word);

} // namespace progressive

#endif // PROGRESSIVE_SPELLCHECK_HPP
