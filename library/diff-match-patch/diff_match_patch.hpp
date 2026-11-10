#pragma once
#include <string>
#include <vector>
#include <cstdint>

namespace pcp::diff {

enum class DiffOperation { EQUAL, INSERT, DELETE };

struct Diff {
    DiffOperation operation;
    std::string text;
};

struct Patch {
    std::vector<Diff> diffs;
    int start1 = 0, start2 = 0;
    int length1 = 0, length2 = 0;
};

class DiffMatchPatch {
public:
    DiffMatchPatch();
    ~DiffMatchPatch();

    // Core diff algorithm (Myers)
    std::vector<Diff> diff(const std::string& text1, const std::string& text2,
                           bool checkLines = true, float deadline = 0.0f);
    std::vector<Diff> diffLinesToChars(const std::string& text1,
                                         const std::string& text2);
    std::string diffToText(const std::vector<Diff>& diffs);
    std::vector<Diff> diffFromText(const std::string& text);

    // Semantic diff refinement
    void diffCleanupSemantic(std::vector<Diff>& diffs);
    void diffCleanupEfficiency(std::vector<Diff>& diffs);
    void diffCleanupMerge(std::vector<Diff>& diffs);

    // Character-level diff
    std::vector<Diff> diffCharsToLines(const std::vector<Diff>& diffs,
                                          const std::vector<std::string>& lines);

    // Patch generation
    std::vector<Patch> patchMake(const std::string& text1, const std::string& text2);
    std::vector<Patch> patchMake(const std::vector<Diff>& diffs);
    std::string patchToText(const std::vector<Patch>& patches);
    std::vector<Patch> patchFromText(const std::string& text);

    // Apply patch
    std::pair<std::string, std::vector<bool>> patchApply(
        const std::vector<Patch>& patches, const std::string& text);

    // Levenshtein distance
    int levenshtein(const std::string& s1, const std::string& s2);

    // Settings
    void setTimeout(float seconds) { m_timeout = seconds; }
    float timeout() const { return m_timeout; }
    void setEditCost(int cost) { m_editCost = cost; }
    int editCost() const { return m_editCost; }
    void setMatchThreshold(float threshold) { m_matchThreshold = threshold; }
    float matchThreshold() const { return m_matchThreshold; }
    void setPatchDeleteThreshold(float threshold) { m_patchDeleteThreshold = threshold; }
    int patchMargin() const { return m_patchMargin; }

private:
    float m_timeout = 1.0f;
    int m_editCost = 4;
    float m_matchThreshold = 0.5f;
    float m_patchDeleteThreshold = 0.5f;
    int m_patchMargin = 4;

    // Internal diff computation
    void diffCompute(const std::string& text1, const std::string& text2,
                     bool checkLines, float deadline,
                     std::vector<Diff>& diffs);

    // Match: locate best instance of pattern in text near loc
    int matchMain(const std::string& text, const std::string& pattern, int loc);

    // Bitap algorithm for fuzzy matching
    int bitapMatch(const std::string& text, const std::string& pattern, int loc);
};

} // namespace pcp::diff
