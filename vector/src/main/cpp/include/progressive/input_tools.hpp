#ifndef PROGRESSIVE_INPUT_TOOLS_HPP
#define PROGRESSIVE_INPUT_TOOLS_HPP

#include <string>
#include <vector>
#include <unordered_map>

namespace progressive {

// ---- Custom Symbol Bar ----

struct SymbolEntry {
    std::string symbol;      // e.g. "→", "≈", "☭", "¯\_(ツ)_/¯"
    std::string label;       // optional display label
    int order = 0;           // display order
};

class SymbolBar {
public:
    void addSymbol(const std::string& symbol, const std::string& label = "");
    void removeSymbol(const std::string& symbol);
    void setOrder(const std::string& symbol, int order);

    // Get all symbols sorted by order.
    std::vector<SymbolEntry> getSymbols() const;

    // Get symbols as JSON array for UI.
    std::string exportJson() const;

    // Load from JSON.
    void importJson(const std::string& json);

    void clear();
    size_t count() const { return symbols_.size(); }

private:
    std::unordered_map<std::string, SymbolEntry> symbols_;
};

// ---- Auto-Replacement Rules ----

struct ReplacementRule {
    std::string pattern;     // "www.youtube.com"
    std::string replacement; // "redirect.invidious.io"
    bool enabled = true;
    bool exactMatch = false; // true = match whole URL, false = substring
};

class ReplacementEngine {
public:
    void addRule(const std::string& pattern, const std::string& replacement, bool exactMatch = false);
    void removeRule(const std::string& pattern);
    void setEnabled(const std::string& pattern, bool enabled);

    // Apply replacement rules to text.
    // Returns rewritten text.
    std::string apply(const std::string& text) const;

    // Check if any rule would apply (for UI preview).
    // Returns the matching rule or nullptr.
    const ReplacementRule* check(const std::string& text) const;

    std::string exportJson() const;
    void importJson(const std::string& json);
    void clear();
    size_t count() const { return rules_.size(); }

private:
    std::vector<ReplacementRule> rules_;
    static std::string toLower(const std::string& s);
};

} // namespace progressive

#endif // PROGRESSIVE_INPUT_TOOLS_HPP
