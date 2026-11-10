#include "autocomplete.hpp"
#include <algorithm>
#include <cctype>
#include <sstream>
#include <android/log.h>

#define AC_LOG_TAG "Autocomplete"
#define AC_LOGI(...) __android_log_print(ANDROID_LOG_INFO, AC_LOG_TAG, __VA_ARGS__)

namespace pcp::autocomplete {

AutocompleteEngine::AutocompleteEngine() = default;
AutocompleteEngine::~AutocompleteEngine() = default;

void AutocompleteEngine::addItems(const std::vector<CompletionItem>& items) {
    auto& source = getSource(items.empty() ? CompletionType::COMMAND : items[0].type);
    source.insert(source.end(), items.begin(), items.end());
}

void AutocompleteEngine::removeItem(const std::string& id, CompletionType type) {
    auto& source = getSource(type);
    source.erase(std::remove_if(source.begin(), source.end(),
        [&](auto& i) { return i.id == id; }), source.end());
}

void AutocompleteEngine::clear(CompletionType type) {
    getSource(type).clear();
}

std::vector<CompletionItem> AutocompleteEngine::query(const std::string& prefix,
                                                        CompletionType type,
                                                        int maxResults) {
    auto& source = getSource(type);
    std::vector<CompletionItem> results = filterAndSort(source, prefix, maxResults);

    // Add recent items that match
    for (auto& recent : m_recentItems) {
        if (recent.type == type && startsWith(recent.display, prefix)) {
            recent.isRecent = true;
            results.push_back(recent);
        }
    }

    return results;
}

void AutocompleteEngine::recordSelection(const std::string& id, CompletionType type) {
    auto& source = getSource(type);
    for (auto& item : source) {
        if (item.id == id) {
            item.usageCount++;
            m_recentItems.push_back(item);
            if (m_recentItems.size() > static_cast<size_t>(m_recentLimit)) {
                m_recentItems.erase(m_recentItems.begin());
            }
            break;
        }
    }
}

void AutocompleteEngine::onQuery(std::function<void(const std::vector<CompletionItem>&)> cb) {
    m_queryCallback = std::move(cb);
}

std::vector<CompletionItem>& AutocompleteEngine::getSource(CompletionType type) {
    switch (type) {
        case CompletionType::COMMAND: return m_commands;
        case CompletionType::EMOJI: return m_emojis;
        case CompletionType::MEMBER: return m_members;
        case CompletionType::ROOM: return m_rooms;
    }
    return m_commands;
}

std::vector<CompletionItem> AutocompleteEngine::filterAndSort(
    const std::vector<CompletionItem>& source,
    const std::string& prefix, int maxResults) {
    std::vector<CompletionItem> results;
    std::string lowerPrefix = prefix;
    std::transform(lowerPrefix.begin(), lowerPrefix.end(), lowerPrefix.begin(), ::tolower);

    for (const auto& item : source) {
        std::string lowerDisplay = item.display;
        std::transform(lowerDisplay.begin(), lowerDisplay.end(), lowerDisplay.begin(), ::tolower);

        if (lowerDisplay.find(lowerPrefix) != std::string::npos) {
            results.push_back(item);
            // Score based on match position and usage count
            results.back().score = (item.display.find(prefix) == 0 ? 100 : 50)
                                    + item.usageCount * 10;
        }
    }

    std::sort(results.begin(), results.end(),
              [](auto& a, auto& b) { return a.score > b.score; });

    if (static_cast<int>(results.size()) > maxResults) {
        results.resize(maxResults);
    }
    return results;
}

int AutocompleteEngine::levenshteinDistance(const std::string& s1, const std::string& s2) const {
    // Classic Levenshtein distance
    size_t m = s1.size(), n = s2.size();
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1));
    for (size_t i = 0; i <= m; i++) dp[i][0] = i;
    for (size_t j = 0; j <= n; j++) dp[0][j] = j;
    for (size_t i = 1; i <= m; i++) {
        for (size_t j = 1; j <= n; j++) {
            dp[i][j] = std::min({
                dp[i-1][j] + 1,
                dp[i][j-1] + 1,
                dp[i-1][j-1] + (s1[i-1] != s2[j-1])
            });
        }
    }
    return dp[m][n];
}

bool AutocompleteEngine::startsWith(const std::string& str, const std::string& prefix) const {
    if (prefix.size() > str.size()) return false;
    return str.compare(0, prefix.size(), prefix) == 0;
}

// ── CommandRegistry ──

CommandRegistry::CommandRegistry() {
    // Built-in slash commands
    registerCommand({"/join", "Join a room", "#room:server", true});
    registerCommand({"/leave", "Leave the current room", "", false});
    registerCommand({"/invite", "Invite a user to the room", "@user:server", true});
    registerCommand({"/kick", "Kick a user from the room", "@user:server", true});
    registerCommand({"/ban", "Ban a user from the room", "@user:server", true});
    registerCommand({"/unban", "Unban a user from the room", "@user:server", true});
    registerCommand({"/ignore", "Ignore a user", "@user:server", true});
    registerCommand({"/unignore", "Unignore a user", "@user:server", true});
    registerCommand({"/me", "Send an emote message", "message", true});
    registerCommand({"/notice", "Send a notice message", "message", true});
    registerCommand({"/shrug", "Send: shrug", "", false});
    registerCommand({"/tableflip", "Send table flip emoticon", "", false});
    registerCommand({"/topic", "Set the room topic", "topic text", true});
    registerCommand({"/nick", "Set your display name", "new name", true});
    registerCommand({"/myroomnick", "Set display name in this room", "new name", true});
    registerCommand({"/roomavatar", "Set the room avatar", "mxc://...", true});
    registerCommand({"/myavatar", "Set your avatar", "mxc://...", true});
    registerCommand({"/op", "Grant moderator privileges", "@user:server", true});
    registerCommand({"/deop", "Revoke moderator privileges", "@user:server", true});
    registerCommand({"/devtools", "Open developer tools", "", false});
    registerCommand({"/rageshake", "Send a bug report", "description", false});
    registerCommand({"/verify", "Verify a user's device", "@user:server", true});
    registerCommand({"/discardsession", "Force discarding an Olm session", "", false});
    registerCommand({"/markdown", "Toggle markdown", "", false});
    registerCommand({"/rainbow", "Send rainbow text", "message", true});
    registerCommand({"/rainbowme", "Send rainbow emote", "message", true});
}

void CommandRegistry::registerCommand(const SlashCommand& cmd) {
    m_commands.push_back(cmd);
}

bool CommandRegistry::isCommand(const std::string& text) const {
    return !text.empty() && text[0] == '/';
}

std::string CommandRegistry::parseCommand(const std::string& text) const {
    size_t space = text.find(' ');
    return space != std::string::npos ? text.substr(0, space) : text;
}

std::string CommandRegistry::parseArgs(const std::string& text) const {
    size_t space = text.find(' ');
    return space != std::string::npos ? text.substr(space + 1) : "";
}

const SlashCommand* CommandRegistry::findCommand(const std::string& name) const {
    for (auto& cmd : m_commands) {
        if (cmd.command == name) return &cmd;
    }
    return nullptr;
}

std::vector<SlashCommand> CommandRegistry::matchingCommands(const std::string& prefix) const {
    std::vector<SlashCommand> results;
    for (auto& cmd : m_commands) {
        if (cmd.command.find(prefix) == 0) results.push_back(cmd);
    }
    return results;
}

// ── EmojiProvider ──

EmojiProvider::EmojiProvider() { loadEmojiData(); }

void EmojiProvider::loadEmojiData() {
    // Common emoji data (would be loaded from a JSON asset at runtime)
    struct { const char* uni; const char* sc; const char* cat; } builtin[] = {
        {"😀", "grinning", "people"}, {"😂", "joy", "people"},
        {"❤️", "heart", "symbols"}, {"👍", "+1", "people"},
        {"🔥", "fire", "nature"}, {"🎉", "tada", "objects"},
        {"🤔", "thinking", "people"}, {"😊", "blush", "people"},
        {"👋", "wave", "people"}, {"✅", "white_check_mark", "symbols"},
        {"❌", "x", "symbols"}, {"⭐", "star", "nature"},
        {"📎", "paperclip", "objects"}, {"🔒", "lock", "objects"},
        {"🎵", "musical_note", "objects"}, {"📷", "camera", "objects"},
        {"🎤", "microphone", "objects"}, {"📱", "iphone", "objects"},
        {"💬", "speech_balloon", "objects"}, {"🔔", "bell", "objects"},
        {"🎯", "dart", "objects"}, {"💯", "100", "symbols"},
    };
    for (auto& e : builtin) {
        EmojiData d{e.uni, e.sc, e.cat, {}, static_cast<int>(m_emojis.size())};
        m_emojis.push_back(d);
        m_shortcodeMap[e.sc] = d;
    }
}

std::vector<EmojiData> EmojiProvider::search(const std::string& query, int maxResults) {
    std::vector<EmojiData> results;
    std::string q = query;
    std::transform(q.begin(), q.end(), q.begin(), ::tolower);
    for (auto& emoji : m_emojis) {
        std::string sc = emoji.shortcode;
        std::transform(sc.begin(), sc.end(), sc.begin(), ::tolower);
        if (sc.find(q) != std::string::npos) {
            results.push_back(emoji);
            if (static_cast<int>(results.size()) >= maxResults) break;
        }
    }
    return results;
}

EmojiData EmojiProvider::findByShortcode(const std::string& shortcode) {
    return m_shortcodeMap[shortcode];
}

std::string EmojiProvider::shortcodeToUnicode(const std::string& shortcode) {
    auto it = m_shortcodeMap.find(shortcode);
    return it != m_shortcodeMap.end() ? it->second.unicode : shortcode;
}

} // namespace pcp::autocomplete
