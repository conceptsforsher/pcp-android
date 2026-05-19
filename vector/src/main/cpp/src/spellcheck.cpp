#include "progressive/spellcheck.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <cmath>

namespace progressive {

// ---- SpellChecker ----

void SpellChecker::loadDictionary(const std::string& words) {
    std::istringstream stream(words);
    std::string word;
    while (stream >> word) {
        // Convert to lowercase for case-insensitive comparison
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

        // Only suggest if reasonably close
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

    // Sort by score descending, then distance ascending
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
                dp[i - 1][j] + 1,        // deletion
                dp[i][j - 1] + 1,        // insertion
                dp[i - 1][j - 1] + cost  // substitution
            });
            // Transposition (Damerau extension)
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

    // Jaro distance
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

    // Winkler prefix bonus
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

// ---- Typo Detection ----

std::vector<TypoResult> detectTypos(const std::string& sentence, const SpellChecker& checker) {
    auto words = tokenizeForSpellcheck(sentence);
    std::vector<TypoResult> results;

    for (const auto& word : words) {
        // Skip very short words and numbers
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

    // Three consonants in a row is suspicious
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

} // namespace progressive
