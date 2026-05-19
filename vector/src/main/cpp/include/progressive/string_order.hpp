#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>

namespace progressive {

// ---- String Order / Fractional Indexing ----
// Original Kotlin:
//   org.matrix.android.sdk.api.util.StringOrderUtils.kt (87 lines)
//   org.matrix.android.sdk.api.session.space.SpaceOrderUtils.kt (105 lines)
//
// Implements fractional indexing for reordering items (spaces, rooms, tags)
// without renumbering everything. Similar to JIRA rank or Notion ordering.
//
// Algorithm:
//   - Uses ASCII printable alphabet (0x20-0x7E, 95 chars) as base-95
//   - Converts order strings to BigIntegers
//   - Computes midpoints via linear interpolation: step = (right - left) / (count + 1)
//   - Converts back to order strings
//   - Each new order fits lexicographically between the surrounding orders

constexpr const char* DEFAULT_ORDER_ALPHABET =
    " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

std::vector<std::string> stringMidPoints(
    const std::string& left,
    const std::string& right,
    int count,
    const std::string& alphabet = DEFAULT_ORDER_ALPHABET);

std::string stringAverage(
    const std::string& left,
    const std::string& right,
    const std::string& alphabet = DEFAULT_ORDER_ALPHABET);

std::string stringToBigInt(const std::string& s, const std::string& alphabet);
std::string bigIntToString(const std::string& decimal, const std::string& alphabet);

struct ReorderCommand {
    std::string itemId;
    std::string newOrder;
};

std::vector<ReorderCommand> computeSpaceReorder(
    const std::vector<std::string>& orderedItemIds,
    const std::vector<std::string>& currentOrders,
    const std::string& movedItemId,
    int delta);

// ============================================================
// STRING COLLATION — locale-aware string comparison
// Original Kotlin:
//   java.text.Collator wrapper for locale-aware sorting
// ============================================================

enum class StringCollation {
    PRIMARY,    // base letters only (a == A, e == é)
    SECONDARY,  // accent-sensitive (a == A, e != é)
    TERTIARY,   // case-sensitive (a != A, e != é)
    IDENTICAL   // full Unicode equivalence
};

enum class StringNormalization {
    NFC,   // Canonical Decomposition then Canonical Composition
    NFD,   // Canonical Decomposition
    NFKC,  // Compatibility Decomposition then Canonical Composition
    NFKD   // Compatibility Decomposition
};

// Normalize a Unicode string using the specified form.
// Performs NF* decomposition/composition on common Unicode ranges.
std::string normalizeString(const std::string& input, StringNormalization form);

// Compare two strings using locale-aware collation strength.
// Returns <0 if a<b, 0 if equal, >0 if a>b.
int collateStrings(const std::string& a, const std::string& b,
                   StringCollation strength = StringCollation::TERTIARY,
                   const std::string& locale = "en_US");

// Convenience: compare with default collation.
int compareStrings(const std::string& a, const std::string& b);

// ============================================================
// STRING SORTER — locale-aware sort with strategies
// Original Kotlin:
//   Sort strategies used in room list ordering
// ============================================================

enum class SortStrategy {
    LOCALE_DEFAULT,       // default locale collation
    LOCALE_CASE_INSENSITIVE,
    NATURAL,              // natural ordering (file10 > file2)
    ASCII_BET,            // pure binary comparison
    LENGTH_ASCENDING,
    LENGTH_DESCENDING,
    LOCALE_SPECIFIC       // caller supplies locale
};

struct StringSorter {
    SortStrategy strategy = SortStrategy::LOCALE_DEFAULT;
    StringCollation strength = StringCollation::TERTIARY;
    std::string locale;
    bool ascending = true;

    bool operator()(const std::string& a, const std::string& b) const;
};

// Sort a vector of strings by locale.
std::vector<std::string> sortStringsByLocale(
    const std::vector<std::string>& strings,
    const std::string& locale = "en_US",
    StringCollation strength = StringCollation::TERTIARY);

// Get list of available locale names.
std::vector<std::string> getAvailableLocales();

// Get the system default locale.
std::string getDefaultLocale();

// ============================================================
// NATURAL SORT — human-friendly numeric ordering
// Original Kotlin:
//   Natural order comparator (file2 < file10)
// ============================================================

struct StringToken {
    std::string textPart;    // non-numeric portion
    int64_t numberPart = 0;  // numeric portion, 0 if none
    bool isNumber = false;   // true if this token is a number
};

// Split a string into alternating text/number tokens for natural sorting.
std::vector<StringToken> tokenizeForNaturalSort(const std::string& s);

// Compare two strings using natural ordering.
// Treats embedded numbers as numeric values so "file10" > "file2".
int naturalCompare(const std::string& a, const std::string& b);

// ============================================================
// UNICODE UTILITIES — character classification
// ============================================================

enum class UnicodeCategory {
    LETTER_UPPERCASE, LETTER_LOWERCASE, LETTER_TITLECASE,
    LETTER_MODIFIER, LETTER_OTHER,
    MARK_NONSPACING, MARK_SPACING, MARK_ENCLOSING,
    NUMBER_DECIMAL, NUMBER_LETTER, NUMBER_OTHER,
    PUNCTUATION_CONNECTOR, PUNCTUATION_DASH, PUNCTUATION_OPEN,
    PUNCTUATION_CLOSE, PUNCTUATION_INITIAL, PUNCTUATION_FINAL,
    PUNCTUATION_OTHER,
    SYMBOL_MATH, SYMBOL_CURRENCY, SYMBOL_MODIFIER, SYMBOL_OTHER,
    SEPARATOR_SPACE, SEPARATOR_LINE, SEPARATOR_PARAGRAPH,
    OTHER_CONTROL, OTHER_FORMAT, OTHER_SURROGATE, OTHER_PRIVATE, OTHER_NOT_ASSIGNED,
    UNKNOWN
};

// Classify a Unicode code point into its general category.
UnicodeCategory getUnicodeCategory(uint32_t codePoint);

bool isLetter(uint32_t codePoint);
bool isDigit(uint32_t codePoint);
bool isWhitespace(uint32_t codePoint);
bool isPunctuation(uint32_t codePoint);
bool isUpperCase(uint32_t codePoint);
bool isLowerCase(uint32_t codePoint);

// Case conversion for ASCII and common Latin-1 Supplement.
uint32_t toUpperCase(uint32_t codePoint);
uint32_t toLowerCase(uint32_t codePoint);

// ============================================================
// PREFIX TREE (TRIE) — efficient prefix matching
// ============================================================

class StringPrefixTree {
public:
    void insert(const std::string& prefix);
    bool search(const std::string& prefix) const;
    bool startsWith(const std::string& text) const;
    std::vector<std::string> complete(const std::string& prefix, int maxResults = 10) const;
    void insertPrefix(const std::string& prefix) { insert(prefix); }
    bool searchPrefix(const std::string& prefix) const { return search(prefix); }
    size_t size() const;
    void clear();

    struct TrieNode {
        bool isEnd = false;
        std::unordered_map<char, TrieNode> children;
    };

private:
    TrieNode root_;
    size_t count_ = 0;
};

// ============================================================
// STRING MATCHING — multiple match strategies
// ============================================================

enum class StringMatchType {
    EXACT,     // exact match, case-sensitive
    PREFIX,    // haystack starts with needle
    SUFFIX,    // haystack ends with needle
    CONTAINS,  // needle found anywhere in haystack
    FUZZY      // fuzzy/approximate matching
};

// Match a needle against a haystack with the given strategy.
// For EXACT/PREFIX/SUFFIX/CONTAINS: returns true/false.
// For FUZZY: returns true if similarity exceeds threshold.
struct StringFuzzyMatch {
    double score = 0.0;       // 0.0 to 1.0, higher = better match
    std::vector<int> alignments; // character indices in haystack matching needle
};

bool matchString(const std::string& haystack, const std::string& needle,
                 StringMatchType type, double fuzzyThreshold = 0.6);

StringFuzzyMatch fuzzyMatch(const std::string& haystack, const std::string& needle);

} // namespace progressive
