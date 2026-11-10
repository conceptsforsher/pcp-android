#pragma once
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <algorithm>
#include <unordered_map>

namespace pcp::autocomplete {

enum class CompletionType { COMMAND, EMOJI, MEMBER, ROOM };

struct CompletionItem {
    std::string id;
    std::string display;
    std::string subtitle;
    std::string iconUrl;
    std::string replacement;
    CompletionType type = CompletionType::COMMAND;
    int score = 0;
    int usageCount = 0;
    bool isRecent = false;
    std::string category;
};

class AutocompleteEngine {
public:
    AutocompleteEngine();
    ~AutocompleteEngine();

    void addItems(const std::vector<CompletionItem>& items);
    void removeItem(const std::string& id, CompletionType type);
    void clear(CompletionType type);

    std::vector<CompletionItem> query(const std::string& prefix,
                                       CompletionType type,
                                       int maxResults = 10);

    void recordSelection(const std::string& id, CompletionType type);
    void setRecentLimit(int limit) { m_recentLimit = limit; }

    void onQuery(std::function<void(const std::vector<CompletionItem>&)> cb);

private:
    std::vector<CompletionItem> m_commands;
    std::vector<CompletionItem> m_emojis;
    std::vector<CompletionItem> m_members;
    std::vector<CompletionItem> m_rooms;

    std::vector<CompletionItem> m_recentItems;
    int m_recentLimit = 20;
    std::function<void(const std::vector<CompletionItem>&)> m_queryCallback;

    std::vector<CompletionItem>& getSource(CompletionType type);
    std::vector<CompletionItem> filterAndSort(const std::vector<CompletionItem>& source,
                                                const std::string& prefix,
                                                int maxResults);
    int levenshteinDistance(const std::string& s1, const std::string& s2) const;
    bool startsWith(const std::string& str, const std::string& prefix) const;
};

// ── Command definitions (Slash commands) ──

struct SlashCommand {
    std::string command;
    std::string description;
    std::string parameters;
    bool requiresParams = false;
};

class CommandRegistry {
public:
    CommandRegistry();
    void registerCommand(const SlashCommand& cmd);
    bool isCommand(const std::string& text) const;
    std::string parseCommand(const std::string& text) const;
    std::string parseArgs(const std::string& text) const;
    const SlashCommand* findCommand(const std::string& name) const;
    std::vector<SlashCommand> matchingCommands(const std::string& prefix) const;

private:
    std::vector<SlashCommand> m_commands;
};

// ── Emoji data ──

struct EmojiData {
    std::string unicode;
    std::string shortcode;
    std::string category;
    std::vector<std::string> keywords;
    int sortOrder = 0;
};

class EmojiProvider {
public:
    EmojiProvider();
    void loadEmojiData();
    std::vector<EmojiData> search(const std::string& query, int maxResults = 20);
    EmojiData findByShortcode(const std::string& shortcode);
    std::string shortcodeToUnicode(const std::string& shortcode);
    size_t totalEmojis() const { return m_emojis.size(); }

private:
    std::vector<EmojiData> m_emojis;
    std::unordered_map<std::string, EmojiData> m_shortcodeMap;
};

} // namespace pcp::autocomplete
