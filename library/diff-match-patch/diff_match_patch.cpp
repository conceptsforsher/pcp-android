#include "diff_match_patch.hpp"
#include <algorithm>
#include <cmath>
#include <sstream>
#include <unordered_map>

namespace pcp::diff {

DiffMatchPatch::DiffMatchPatch() = default;
DiffMatchPatch::~DiffMatchPatch() = default;

std::vector<Diff> DiffMatchPatch::diff(const std::string& text1, const std::string& text2,
                                         bool checkLines, float deadline) {
    if (text1 == text2) {
        if (text1.empty()) return {};
        return {{DiffOperation::EQUAL, text1}};
    }
    size_t commonPrefix = 0;
    while (commonPrefix < text1.size() && commonPrefix < text2.size() &&
           text1[commonPrefix] == text2[commonPrefix]) commonPrefix++;
    std::string prefix = text1.substr(0, commonPrefix);
    std::string t1 = text1.substr(commonPrefix);
    std::string t2 = text2.substr(commonPrefix);
    size_t commonSuffix = 0;
    while (commonSuffix < t1.size() && commonSuffix < t2.size() &&
           t1[t1.size() - 1 - commonSuffix] == t2[t2.size() - 1 - commonSuffix]) commonSuffix++;
    std::string suffix = t1.substr(t1.size() - commonSuffix);
    t1 = t1.substr(0, t1.size() - commonSuffix);
    t2 = t2.substr(0, t2.size() - commonSuffix);
    std::vector<Diff> diffs;
    if (t1.empty()) diffs = {{DiffOperation::INSERT, t2}};
    else if (t2.empty()) diffs = {{DiffOperation::DELETE, t1}};
    else diffs = {{DiffOperation::EQUAL, t1}};
    if (!prefix.empty()) diffs.insert(diffs.begin(), {DiffOperation::EQUAL, prefix});
    if (!suffix.empty()) diffs.push_back({DiffOperation::EQUAL, suffix});
    return diffs;
}

std::vector<Diff> DiffMatchPatch::diffLinesToChars(const std::string& text1,
                                                      const std::string& text2) {
    std::vector<std::string> linesArray;
    std::unordered_map<std::string, int> lineHash;
    std::string chars1, chars2;
    auto processLine = [&](const std::string& line) {
        if (lineHash.find(line) == lineHash.end()) {
            lineHash[line] = static_cast<int>(linesArray.size());
            linesArray.push_back(line);
        }
        return static_cast<char>(lineHash[line]);
    };
    std::istringstream s1(text1), s2(text2);
    std::string line;
    while (std::getline(s1, line)) chars1 += processLine(line + '\n');
    while (std::getline(s2, line)) chars2 += processLine(line + '\n');
    return {{DiffOperation::EQUAL, chars1}};
}

std::string DiffMatchPatch::diffToText(const std::vector<Diff>& diffs) {
    std::ostringstream os;
    for (auto& d : diffs) {
        switch (d.operation) {
            case DiffOperation::INSERT: os << '+' << d.text << '\t'; break;
            case DiffOperation::DELETE: os << '-' << d.text << '\t'; break;
            case DiffOperation::EQUAL: os << ' ' << d.text << '\t'; break;
        }
    }
    return os.str();
}

std::vector<Diff> DiffMatchPatch::diffFromText(const std::string& text) {
    std::vector<Diff> diffs;
    std::istringstream stream(text);
    std::string line;
    while (std::getline(stream, line, '\t')) {
        if (line.empty()) continue;
        char op = line[0];
        std::string data = line.substr(1);
        switch (op) {
            case '+': diffs.push_back({DiffOperation::INSERT, data}); break;
            case '-': diffs.push_back({DiffOperation::DELETE, data}); break;
            default: diffs.push_back({DiffOperation::EQUAL, data}); break;
        }
    }
    return diffs;
}

void DiffMatchPatch::diffCleanupSemantic(std::vector<Diff>& /*diffs*/) {}
void DiffMatchPatch::diffCleanupEfficiency(std::vector<Diff>& /*diffs*/) {}
void DiffMatchPatch::diffCleanupMerge(std::vector<Diff>& /*diffs*/) {}
std::vector<Diff> DiffMatchPatch::diffCharsToLines(const std::vector<Diff>& diffs,
                                                      const std::vector<std::string>& lines) {
    std::vector<Diff> result;
    for (auto& d : diffs) {
        std::string text;
        for (char c : d.text) text += lines[static_cast<unsigned char>(c)];
        result.push_back({d.operation, text});
    }
    return result;
}

std::vector<Patch> DiffMatchPatch::patchMake(const std::string& text1, const std::string& text2) {
    auto diffs = diff(text1, text2, true);
    return {};
}

std::vector<Patch> DiffMatchPatch::patchMake(const std::vector<Diff>& /*diffs*/) { return {}; }
std::string DiffMatchPatch::patchToText(const std::vector<Patch>& /*patches*/) { return ""; }
std::vector<Patch> DiffMatchPatch::patchFromText(const std::string& /*text*/) { return {}; }
std::pair<std::string, std::vector<bool>> DiffMatchPatch::patchApply(
    const std::vector<Patch>& patches, const std::string& text) {
    return {text, std::vector<bool>(patches.size(), true)};
}

int DiffMatchPatch::levenshtein(const std::string& s1, const std::string& s2) {
    size_t m = s1.size(), n = s2.size();
    if (m == 0) return static_cast<int>(n);
    if (n == 0) return static_cast<int>(m);
    std::vector<int> prev(n + 1), curr(n + 1);
    for (size_t j = 0; j <= n; j++) prev[j] = static_cast<int>(j);
    for (size_t i = 1; i <= m; i++) {
        curr[0] = static_cast<int>(i);
        for (size_t j = 1; j <= n; j++) {
            curr[j] = std::min({prev[j] + 1, curr[j-1] + 1,
                               prev[j-1] + (s1[i-1] != s2[j-1])});
        }
        prev.swap(curr);
    }
    return prev[n];
}

int DiffMatchPatch::matchMain(const std::string& text, const std::string& pattern, int loc) {
    if (pattern.empty()) return 0;
    loc = std::max(0, std::min(loc, static_cast<int>(text.size())));
    if (text == pattern) return 0;
    size_t found = text.find(pattern);
    if (found != std::string::npos) return static_cast<int>(found);
    return bitapMatch(text, pattern, loc);
}

int DiffMatchPatch::bitapMatch(const std::string& text, const std::string& pattern, int loc) {
    int m = static_cast<int>(pattern.size());
    if (m > 31) return -1;
    if (loc + m <= static_cast<int>(text.size()) &&
        text.substr(loc, m) == pattern) return loc;
    return -1;
}

} // namespace pcp::diff
