#include "progressive/spellcheck.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <cmath>

namespace progressive {

// ---- SpellChecker (existing implementation) ----

void SpellChecker::loadDictionary(const std::string& words) {
    std::istringstream stream(words);
    std::string word;
    while (stream >> word) {
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        dictionary_.insert(word);
    }
}

bool SpellChecker::isKnown(const std::string& word) const {
    auto lower = word;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return dictionary_.find(lower) != dictionary_.end();
}

std::vector<SpellCandidate> SpellChecker::suggest(const std::string& word, int maxResults) const {
    std::vector<SpellCandidate> candidates;
    auto lower = word;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

    if (word.size() < 2) return candidates;

    for (const auto& dictWord : dictionary_) {
        int dist = editDistance(lower, dictWord);
        int maxDist = std::max(2, static_cast<int>(word.size()) / 3);
        if (dist <= maxDist) {
            double jw = jaroWinkler(lower, dictWord);
            SpellCandidate c;
            c.word = dictWord;
            c.distance = dist;
            c.score = jw;
            candidates.push_back(c);
        }
    }

    std::sort(candidates.begin(), candidates.end(), [](const SpellCandidate& a, const SpellCandidate& b) {
        if (a.score != b.score) return a.score > b.score;
        return a.distance < b.distance;
    });

    if (static_cast<int>(candidates.size()) > maxResults) {
        candidates.resize(maxResults);
    }

    return candidates;
}

void SpellChecker::addWord(const std::string& word) {
    auto lower = word;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    dictionary_.insert(lower);
}

void SpellChecker::removeWord(const std::string& word) {
    auto lower = word;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    dictionary_.erase(lower);
}

void SpellChecker::clear() {
    dictionary_.clear();
}

int SpellChecker::editDistance(const std::string& a, const std::string& b) {
    int n = static_cast<int>(a.size());
    int m = static_cast<int>(b.size());

    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(m + 1, 0));

    for (int i = 0; i <= n; ++i) dp[i][0] = i;
    for (int j = 0; j <= m; ++j) dp[0][j] = j;

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            int cost = (a[i - 1] == b[j - 1]) ? 0 : 1;
            dp[i][j] = std::min({
                dp[i - 1][j] + 1,
                dp[i][j - 1] + 1,
                dp[i - 1][j - 1] + cost
            });
            if (i > 1 && j > 1 && a[i - 1] == b[j - 2] && a[i - 2] == b[j - 1]) {
                dp[i][j] = std::min(dp[i][j], dp[i - 2][j - 2] + cost);
            }
        }
    }

    return dp[n][m];
}

double SpellChecker::jaroWinkler(const std::string& a, const std::string& b) {
    if (a == b) return 1.0;
    if (a.empty() || b.empty()) return 0.0;

    int matchDistance = std::max(static_cast<int>(a.size()), static_cast<int>(b.size())) / 2 - 1;
    matchDistance = std::max(matchDistance, 0);

    std::vector<bool> aMatch(a.size(), false);
    std::vector<bool> bMatch(b.size(), false);

    int matches = 0;
    for (size_t i = 0; i < a.size(); ++i) {
        int start = std::max(0, static_cast<int>(i) - matchDistance);
        int end = std::min(static_cast<int>(b.size()), static_cast<int>(i) + matchDistance + 1);
        for (int j = start; j < end; ++j) {
            if (!bMatch[j] && a[i] == b[j]) {
                aMatch[i] = true;
                bMatch[j] = true;
                matches++;
                break;
            }
        }
    }

    if (matches == 0) return 0.0;

    int transpositions = 0;
    size_t k = 0;
    for (size_t i = 0; i < a.size(); ++i) {
        if (aMatch[i]) {
            while (!bMatch[k]) ++k;
            if (a[i] != b[k]) transpositions++;
            ++k;
        }
    }
    transpositions /= 2;

    double jaro = (static_cast<double>(matches) / a.size() +
                   static_cast<double>(matches) / b.size() +
                   static_cast<double>(matches - transpositions) / matches) / 3.0;

    int prefix = 0;
    for (size_t i = 0; i < (a.size() < (size_t)4 ? a.size() : (size_t)4) && i < b.size(); ++i) {
        if (a[i] == b[i]) prefix++;
        else break;
    }

    return jaro + prefix * 0.1 * (1.0 - jaro);
}

std::string SpellChecker::soundex(const std::string& word) {
    if (word.empty()) return {};
    std::string result(1, std::toupper(word[0]));

    static const char* codes[] = {
        "AEIOUHWY", "BFPV", "CGJKQSXZ", "DT", "L", "MN", "R"
    };

    char prevCode = 0;
    for (size_t i = 1; i < word.size() && result.size() < 4; ++i) {
        char c = std::toupper(word[i]);
        char code = '0';
        for (int j = 0; j < 7; ++j) {
            if (strchr(codes[j], c)) {
                code = '0' + j;
                break;
            }
        }
        if (code != '0' && code != prevCode) {
            result += code;
            prevCode = code;
        }
    }

    while (result.size() < 4) result += '0';
    return result;
}

bool SpellChecker::phoneticMatch(const std::string& a, const std::string& b) {
    return soundex(a) == soundex(b);
}

// ---- Typo Detection (existing) ----

std::vector<TypoResult> detectTypos(const std::string& sentence, const SpellChecker& checker) {
    auto words = tokenizeForSpellcheck(sentence);
    std::vector<TypoResult> results;

    for (const auto& word : words) {
        if (word.size() < 2) continue;
        if (std::all_of(word.begin(), word.end(), ::isdigit)) continue;

        if (!checker.isKnown(word)) {
            TypoResult tr;
            tr.hasTypo = true;
            tr.word = word;
            tr.suggestions = checker.suggest(word, 3);
            if (!tr.suggestions.empty()) {
                results.push_back(tr);
            }
        }
    }

    return results;
}

std::vector<std::string> tokenizeForSpellcheck(const std::string& sentence) {
    std::vector<std::string> words;
    std::string current;
    for (char c : sentence) {
        if (std::isalpha(static_cast<unsigned char>(c)) || c == '\'') {
            current += c;
        } else {
            if (!current.empty()) {
                words.push_back(current);
                current.clear();
            }
        }
    }
    if (!current.empty()) words.push_back(current);
    return words;
}

bool looksTypo(const std::string& word) {
    if (word.size() < 3) return false;

    int consonants = 0;
    for (char c : word) {
        char upper = std::toupper(c);
        if (upper >= 'A' && upper <= 'Z' &&
            upper != 'A' && upper != 'E' && upper != 'I' &&
            upper != 'O' && upper != 'U' && upper != 'Y') {
            consonants++;
            if (consonants >= 3) return true;
        } else {
            consonants = 0;
        }
    }

    return false;
}

// ============================================================================
// New: checkSpelling
// ============================================================================

// Original Kotlin: SpellCheck.kt — check spelling with config
// Returns results for each misspelled word.
std::vector<SpellCheckResult> checkSpelling(
    const std::string& text,
    const SpellCheckConfig& config,
    SpellChecker& checker) {
    std::vector<SpellCheckResult> results;
    if (!config.enabled || text.empty()) return results;

    auto words = tokenizeForSpellcheck(text);

    // Re-tokenize with positions
    struct WordPos {
        std::string word;
        int start = 0;
        int end = 0;
    };
    std::vector<WordPos> wordPositions;
    {
        std::string current;
        int wordStart = -1;
        for (int i = 0; i < static_cast<int>(text.size()); ++i) {
            char c = text[i];
            if (std::isalpha(static_cast<unsigned char>(c)) || c == '\'') {
                if (current.empty()) wordStart = i;
                current += c;
            } else {
                if (!current.empty()) {
                    wordPositions.push_back({current, wordStart, i});
                    current.clear();
                    wordStart = -1;
                }
            }
        }
        if (!current.empty()) {
            wordPositions.push_back({current, wordStart, static_cast<int>(text.size())});
        }
    }

    for (const auto& wp : wordPositions) {
        if (wp.word.size() < 2) continue;

        // Skip numbers
        if (config.ignoreNumbers && std::all_of(wp.word.begin(), wp.word.end(), ::isdigit))
            continue;

        // Skip capitalized words
        if (config.ignoreCapitalized && std::isupper(static_cast<unsigned char>(wp.word[0])))
            continue;

        // Skip URLs
        if (config.ignoreUrls) {
            if (wp.word.find("://") != std::string::npos ||
                wp.word.rfind("www.", 0) == 0 ||
                wp.word.find(".com") != std::string::npos ||
                wp.word.find(".org") != std::string::npos)
                continue;
        }

        if (!checker.isKnown(wp.word)) {
            SpellCheckResult result;
            result.word = wp.word;
            result.isCorrect = false;
            result.position = wp.start;

            // Get suggestions
            auto candidates = checker.suggest(wp.word, config.maxSuggestions);
            for (const auto& c : candidates) {
                result.suggestions.push_back(c.word);
            }

            results.push_back(result);
        }
    }

    return results;
}

// ============================================================================
// New: getSpellSuggestions (Levenshtein-based, standalone)
// ============================================================================

// Original Kotlin: SpellCheck.kt — Levenshtein-based suggestion finder
std::vector<std::string> getSpellSuggestions(
    const std::string& word,
    const std::unordered_set<std::string>& dictionary,
    int maxSuggestions) {
    std::vector<std::string> suggestions;
    if (word.size() < 2 || dictionary.empty()) return suggestions;

    auto lower = word;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

    struct Candidate {
        std::string word;
        int distance = 999;
    };
    std::vector<Candidate> candidates;

    int maxDist = std::max(2, static_cast<int>(word.size()) / 3);

    for (const auto& dictWord : dictionary) {
        int dist = computeLevenshteinDistance(lower, dictWord);
        if (dist <= maxDist) {
            candidates.push_back({dictWord, dist});
        }
    }

    // Sort by distance ascending
    std::sort(candidates.begin(), candidates.end(), [](const Candidate& a, const Candidate& b) {
        return a.distance < b.distance;
    });

    for (int i = 0; i < static_cast<int>(candidates.size()) && i < maxSuggestions; ++i) {
        suggestions.push_back(candidates[i].word);
    }

    return suggestions;
}

// ============================================================================
// New: computeLevenshteinDistance
// ============================================================================

// Original Kotlin: SpellCheck.kt — Levenshtein edit distance
int computeLevenshteinDistance(const std::string& a, const std::string& b) {
    return SpellChecker::editDistance(a, b);
}

// ============================================================================
// New: isWordInDictionary
// ============================================================================

// Original Kotlin: SpellCheck.kt — simple dictionary lookup
bool isWordInDictionary(const std::string& word, const std::unordered_set<std::string>& dictionary) {
    auto lower = word;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return dictionary.find(lower) != dictionary.end();
}

// ============================================================================
// New: loadDictionary (free function overload)
// ============================================================================

// Original Kotlin: SpellCheck.kt — load dictionary word list into set
void loadDictionary(const std::string& wordList, std::unordered_set<std::string>& outDict) {
    std::istringstream stream(wordList);
    std::string word;
    while (stream >> word) {
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        outDict.insert(word);
    }
}

// ============================================================================
// New: getSupportedLanguages
// ============================================================================

// Original Kotlin: SpellCheck.kt — available language dictionaries
std::vector<SpellCheckLanguage> getSupportedLanguages() {
    // Return built-in list of common languages
    return {
        {"en", "English", {}},
        {"en_us", "English (US)", {}},
        {"en_gb", "English (UK)", {}},
        {"fr", "French", {}},
        {"de", "German", {}},
        {"es", "Spanish", {}},
        {"it", "Italian", {}},
        {"pt", "Portuguese", {}},
        {"pt_br", "Portuguese (Brazil)", {}},
        {"nl", "Dutch", {}},
        {"pl", "Polish", {}},
        {"ru", "Russian", {}},
        {"uk", "Ukrainian", {}},
        {"tr", "Turkish", {}},
        {"sv", "Swedish", {}},
        {"nb", "Norwegian (Bokmål)", {}},
        {"da", "Danish", {}},
        {"fi", "Finnish", {}},
        {"cs", "Czech", {}},
        {"sk", "Slovak", {}},
        {"hu", "Hungarian", {}},
        {"ro", "Romanian", {}},
        {"el", "Greek", {}},
        {"bg", "Bulgarian", {}},
        {"ja", "Japanese", {}},
        {"ko", "Korean", {}},
        {"zh", "Chinese (Simplified)", {}},
        {"zh_tw", "Chinese (Traditional)", {}},
        {"ar", "Arabic", {}},
        {"he", "Hebrew", {}},
        {"hi", "Hindi", {}},
        {"th", "Thai", {}},
        {"vi", "Vietnamese", {}},
        {"id", "Indonesian", {}},
    };
}

// ============================================================================
// New: detectLanguage
// ============================================================================

// Original Kotlin: SpellCheck.kt — basic language detection from text sample
// Uses common word heuristics (simplified)
std::string detectLanguage(const std::string& text) {
    if (text.empty()) return "en";

    // Common stop words for major languages
    struct LangSig {
        std::string code;
        std::vector<std::string> stopwords;
    };

    static const std::vector<LangSig> signatures = {
        {"en", {"the", "is", "are", "and", "or", "but", "in", "on", "at", "to", "for", "of", "a", "an", "this", "that", "it", "you", "he", "she", "they", "we", "be", "have", "has", "do", "does", "not"}},
        {"fr", {"le", "la", "les", "de", "du", "des", "et", "ou", "est", "sont", "dans", "sur", "pour", "avec", "que", "qui", "une", "pas", "ne", "ce", "il", "elle", "nous", "vous", "ils", "elles"}},
        {"de", {"der", "die", "das", "und", "oder", "ist", "sind", "in", "auf", "zu", "für", "mit", "ein", "eine", "nicht", "es", "ich", "du", "er", "sie", "wir", "ihr", "von", "an", "bei"}},
        {"es", {"el", "la", "los", "las", "de", "del", "y", "o", "es", "está", "son", "en", "para", "por", "con", "que", "una", "un", "no", "se", "lo", "yo", "tú", "él", "ella", "nosotros"}},
        {"it", {"il", "la", "le", "i", "di", "da", "e", "è", "sono", "in", "su", "per", "con", "che", "una", "un", "non", "si", "lo", "io", "tu", "lui", "lei", "noi", "voi", "loro"}},
        {"pt", {"o", "a", "os", "as", "de", "do", "da", "e", "ou", "é", "são", "em", "para", "com", "que", "um", "uma", "não", "se", "ele", "ela", "nós", "você", "eles", "elas"}},
        {"nl", {"de", "het", "een", "en", "of", "is", "zijn", "in", "op", "te", "voor", "met", "dat", "die", "dit", "niet", "ik", "je", "hij", "ze", "wij", "jullie", "van", "aan"}},
        {"ru", {"и", "в", "не", "на", "я", "что", "с", "он", "это", "а", "по", "как", "но", "к", "у", "из", "о", "то", "мы", "ты", "вы", "они", "за", "от", "для"}},
        {"uk", {"і", "в", "не", "на", "я", "що", "з", "він", "це", "а", "по", "як", "але", "до", "у", "з", "про", "то", "ми", "ти", "ви", "вони", "за", "від", "для"}},
        {"pl", {"i", "w", "nie", "na", "ja", "że", "z", "on", "to", "a", "po", "jak", "ale", "do", "u", "ze", "o", "to", "my", "ty", "wy", "oni", "za", "od", "dla"}},
        {"tr", {"ve", "bir", "bu", "de", "da", "için", "ile", "mi", "ne", "o", "ben", "sen", "onlar", "biz", "siz", "değil", "var", "yok", "çok", "daha", "ama", "kadar", "gibi"}},
    };

    // Tokenize and lowercase
    std::vector<std::string> words;
    {
        std::string current;
        for (size_t i = 0; i < text.size(); ++i) {
            char c = static_cast<char>(std::tolower(static_cast<unsigned char>(text[i])));
            if (std::isalpha(static_cast<unsigned char>(c))) {
                current += c;
            } else {
                if (!current.empty()) {
                    words.push_back(current);
                    current.clear();
                }
            }
        }
        if (!current.empty()) words.push_back(current);
    }

    // Score each language by matching stopwords
    int bestScore = 0;
    std::string bestLang = "en";

    for (const auto& sig : signatures) {
        int score = 0;
        for (const auto& word : words) {
            for (const auto& sw : sig.stopwords) {
                if (word == sw) {
                    score += 2; // stopword match is strong signal
                    break;
                }
            }
        }
        if (score > bestScore) {
            bestScore = score;
            bestLang = sig.code;
        }
    }

    return bestLang;
}

// ============================================================================
// New: markSpellingErrors
// ============================================================================

// Original Kotlin: SpellCheck.kt — mark errors with position information
std::vector<SpellCheckMark> markSpellingErrors(
    const std::string& text,
    const SpellCheckConfig& config,
    SpellChecker& checker) {
    std::vector<SpellCheckMark> marks;
    if (!config.enabled || text.empty()) return marks;

    // Tokenize and track positions
    std::string current;
    int wordStart = -1;

    for (int i = 0; i < static_cast<int>(text.size()); ++i) {
        char c = text[i];
        if (std::isalpha(static_cast<unsigned char>(c)) || c == '\'') {
            if (current.empty()) wordStart = i;
            current += c;
        } else {
            if (!current.empty()) {
                bool skip = false;
                if (current.size() < 2) skip = true;
                if (config.ignoreNumbers && std::all_of(current.begin(), current.end(), ::isdigit))
                    skip = true;
                if (config.ignoreCapitalized && std::isupper(static_cast<unsigned char>(current[0])))
                    skip = true;
                if (config.ignoreUrls && (current.find("://") != std::string::npos || current.rfind("www.", 0) == 0))
                    skip = true;

                if (!skip && !checker.isKnown(current)) {
                    SpellCheckMark mark;
                    mark.word = current;
                    mark.startPos = wordStart;
                    mark.endPos = i;
                    auto candidates = checker.suggest(current, config.maxSuggestions);
                    for (const auto& c : candidates)
                        mark.suggestions.push_back(c.word);
                    if (!mark.suggestions.empty())
                        marks.push_back(mark);
                }
                current.clear();
                wordStart = -1;
            }
        }
    }

    // Handle last word
    if (!current.empty()) {
        bool skip = false;
        if (current.size() < 2) skip = true;
        if (config.ignoreNumbers && std::all_of(current.begin(), current.end(), ::isdigit)) skip = true;
        if (config.ignoreCapitalized && std::isupper(static_cast<unsigned char>(current[0]))) skip = true;
        if (config.ignoreUrls && (current.find("://") != std::string::npos || current.rfind("www.", 0) == 0)) skip = true;

        if (!skip && !checker.isKnown(current)) {
            SpellCheckMark mark;
            mark.word = current;
            mark.startPos = wordStart;
            mark.endPos = static_cast<int>(text.size());
            auto candidates = checker.suggest(current, config.maxSuggestions);
            for (const auto& c : candidates)
                mark.suggestions.push_back(c.word);
            if (!mark.suggestions.empty())
                marks.push_back(mark);
        }
    }

    return marks;
}

// ============================================================================
// New: buildDictionaryWordList
// ============================================================================

// Original Kotlin: SpellCheck.kt — create word list from frequency data
// Parses space/newline separated words, optionally with count suffixes like "word:12345"
std::vector<std::string> buildDictionaryWordList(const std::string& frequencyData) {
    std::vector<std::string> words;
    std::istringstream stream(frequencyData);
    std::string token;

    while (stream >> token) {
        // Strip trailing punctuation from individual words
        while (!token.empty() && !std::isalpha(static_cast<unsigned char>(token.back()))) {
            token.pop_back();
        }
        while (!token.empty() && !std::isalpha(static_cast<unsigned char>(token.front()))) {
            token.erase(0, 1);
        }

        if (!token.empty() && token.size() >= 2) {
            std::transform(token.begin(), token.end(), token.begin(), ::tolower);
            words.push_back(token);
        }
    }

    return words;
}

} // namespace progressive
