#include "progressive/input_tools.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

namespace progressive {

// ---- SymbolBar ----

void SymbolBar::addSymbol(const std::string& symbol, const std::string& label) {
    SymbolEntry entry;
    entry.symbol = symbol;
    entry.label = label.empty() ? symbol : label;
    entry.order = static_cast<int>(symbols_.size());
    symbols_[symbol] = entry;
}

void SymbolBar::removeSymbol(const std::string& symbol) {
    symbols_.erase(symbol);
}

void SymbolBar::setOrder(const std::string& symbol, int order) {
    auto it = symbols_.find(symbol);
    if (it != symbols_.end()) it->second.order = order;
}

std::vector<SymbolEntry> SymbolBar::getSymbols() const {
    std::vector<SymbolEntry> result;
    for (const auto& [_, e] : symbols_) result.push_back(e);
    std::sort(result.begin(), result.end(), [](const SymbolEntry& a, const SymbolEntry& b) {
        return a.order < b.order;
    });
    return result;
}

std::string SymbolBar::exportJson() const {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    auto sorted = getSymbols();
    std::ostringstream json;
    json << "[";
    for (size_t i = 0; i < sorted.size(); ++i) {
        if (i > 0) json << ",";
        json << R"({"symbol": ")" << esc(sorted[i].symbol) << R"(")";
        if (!sorted[i].label.empty() && sorted[i].label != sorted[i].symbol) {
            json << R"(,"label": ")" << esc(sorted[i].label) << R"(")";
        }
        json << "}";
    }
    json << "]";
    return json.str();
}

void SymbolBar::importJson(const std::string& json) {
    clear();
    size_t pos = 0;
    int order = 0;
    while (true) {
        pos = json.find('{', pos);
        if (pos == std::string::npos) break;
        int depth = 0;
        auto end = pos;
        while (end < json.size()) {
            if (json[end] == '{') ++depth;
            else if (json[end] == '}') --depth;
            if (depth == 0) break;
            ++end;
        }
        if (end >= json.size()) break;

        std::string obj = json.substr(pos, end - pos + 1);

        // Extract symbol
        auto symSearch = std::string("\"symbol\": \"");
        auto symPos = obj.find(symSearch);
        if (symPos == std::string::npos) { pos = end + 1; continue; }
        symPos += symSearch.size();
        auto symEnd = obj.find('"', symPos);
        if (symEnd == std::string::npos) { pos = end + 1; continue; }
        auto symbol = obj.substr(symPos, symEnd - symPos);

        // Extract optional label
        auto lblSearch = std::string("\"label\": \"");
        auto lblPos = obj.find(lblSearch);
        std::string label;
        if (lblPos != std::string::npos) {
            lblPos += lblSearch.size();
            auto lblEnd = obj.find('"', lblPos);
            if (lblEnd != std::string::npos) label = obj.substr(lblPos, lblEnd - lblPos);
        }

        SymbolEntry entry{symbol, label.empty() ? symbol : label, order++};
        symbols_[symbol] = entry;

        pos = end + 1;
    }
}

void SymbolBar::clear() {
    symbols_.clear();
}

// ---- ReplacementEngine ----

std::string ReplacementEngine::toLower(const std::string& s) {
    std::string r = s;
    std::transform(r.begin(), r.end(), r.begin(), ::tolower);
    return r;
}

void ReplacementEngine::addRule(const std::string& pattern, const std::string& replacement, bool exactMatch) {
    auto lower = toLower(pattern);
    // Remove existing rule with same pattern
    rules_.erase(std::remove_if(rules_.begin(), rules_.end(),
        [&](const ReplacementRule& r) { return toLower(r.pattern) == lower; }
    ), rules_.end());
    rules_.push_back({pattern, replacement, true, exactMatch});
}

void ReplacementEngine::removeRule(const std::string& pattern) {
    auto lower = toLower(pattern);
    rules_.erase(std::remove_if(rules_.begin(), rules_.end(),
        [&](const ReplacementRule& r) { return toLower(r.pattern) == lower; }
    ), rules_.end());
}

void ReplacementEngine::setEnabled(const std::string& pattern, bool enabled) {
    auto lower = toLower(pattern);
    for (auto& r : rules_) {
        if (toLower(r.pattern) == lower) {
            r.enabled = enabled;
            return;
        }
    }
}

std::string ReplacementEngine::apply(const std::string& text) const {
    std::string result = text;
    for (const auto& rule : rules_) {
        if (!rule.enabled) continue;
        if (rule.exactMatch) {
            // Replace only if exact URL match
            auto lower = toLower(result);
            if (lower == toLower(rule.pattern)) {
                result = rule.replacement;
            }
        } else {
            // Replace substring (case-insensitive)
            auto lowerResult = toLower(result);
            auto lowerPattern = toLower(rule.pattern);
            size_t pos = 0;
            while ((pos = lowerResult.find(lowerPattern, pos)) != std::string::npos) {
                result.replace(pos, rule.pattern.size(), rule.replacement);
                lowerResult = toLower(result);
                pos += rule.replacement.size();
            }
        }
    }
    return result;
}

const ReplacementRule* ReplacementEngine::check(const std::string& text) const {
    for (const auto& rule : rules_) {
        if (!rule.enabled) continue;
        auto lower = toLower(text);
        auto lowerPattern = toLower(rule.pattern);
        if (lower.find(lowerPattern) != std::string::npos) {
            return &rule;
        }
    }
    return nullptr;
}

std::string ReplacementEngine::exportJson() const {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    std::ostringstream json;
    json << "[";
    for (size_t i = 0; i < rules_.size(); ++i) {
        if (i > 0) json << ",";
        const auto& r = rules_[i];
        json << R"({"pattern": ")" << esc(r.pattern) << R"(")";
        json << R"(,"replacement": ")" << esc(r.replacement) << R"(")";
        json << R"(,"enabled": )" << (r.enabled ? "true" : "false");
        json << R"(,"exactMatch": )" << (r.exactMatch ? "true" : "false") << "}";
    }
    json << "]";
    return json.str();
}

void ReplacementEngine::importJson(const std::string& json) {
    clear();
    size_t pos = 0;
    while (true) {
        pos = json.find('{', pos);
        if (pos == std::string::npos) break;
        int depth = 0;
        auto end = pos;
        while (end < json.size()) {
            if (json[end] == '{') ++depth;
            else if (json[end] == '}') --depth;
            if (depth == 0) break;
            ++end;
        }
        if (end >= json.size()) break;

        std::string obj = json.substr(pos, end - pos + 1);
        ReplacementRule rule;

        auto searchPattern = std::string("\"pattern\": \"");
        auto pp = obj.find(searchPattern);
        if (pp != std::string::npos) {
            pp += searchPattern.size();
            auto pe = obj.find('"', pp);
            if (pe != std::string::npos) rule.pattern = obj.substr(pp, pe - pp);
        }

        auto searchRepl = std::string("\"replacement\": \"");
        auto rp = obj.find(searchRepl);
        if (rp != std::string::npos) {
            rp += searchRepl.size();
            auto re = obj.find('"', rp);
            if (re != std::string::npos) rule.replacement = obj.substr(rp, re - rp);
        }

        rule.enabled    = obj.find("\"enabled\": true") != std::string::npos;
        rule.exactMatch = obj.find("\"exactMatch\": true") != std::string::npos;

        if (!rule.pattern.empty()) rules_.push_back(rule);
        pos = end + 1;
    }
}

void ReplacementEngine::clear() {
    rules_.clear();
}

} // namespace progressive
