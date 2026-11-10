#include "pcp/command_parser.hpp"
#include <regex>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>

namespace pcp {

// ---- Regex patterns (ported from MatrixPatterns.kt) ----

static bool isUserId(const std::string& s) {
    // @localpart:domain — case-insensitive match
    static const std::regex re(R"(@[A-Z0-9\x21-\x39\x3B-\x7F]+:[A-Z0-9.-]+(\:[0-9]{2,5})?)",
                                std::regex::icase);
    return std::regex_match(s, re);
}

static bool isMxcUrl(const std::string& s) {
    return s.size() >= 6 && s.substr(0, 6) == "mxc://";
}

static bool isEmail(const std::string& s) {
    return s.find('@') != std::string::npos;
}

// ---- JSON escaping helper ----

static std::string jsonEscape(const std::string& s) {
    std::string out;
    out.reserve(s.size() + 2);
    for (char c : s) {
        switch (c) {
            case '"':  out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\n': out += "\\n";  break;
            case '\r': out += "\\r";  break;
            case '\t': out += "\\t";  break;
            default:   out += c;
        }
    }
    return out;
}

// ---- Minimal JSON builder (avoids nlohmann dependency) ----

class JsonObj {
    std::string buf;
    bool first = true;
public:
    JsonObj() { buf = "{"; }
    void str(const char* key, const std::string& val) { comma(); buf += "\""; buf += key; buf += "\":\""; buf += jsonEscape(val); buf += "\""; }
    void integer(const char* key, int val) { comma(); buf += "\""; buf += key; buf += "\":"; buf += std::to_string(val); }
    void boolean(const char* key, bool val) { comma(); buf += "\""; buf += key; buf += "\":"; buf += (val ? "true" : "false"); }
    void nullableInt(const char* key, int val, bool hasVal) {
        comma(); buf += "\""; buf += key; buf += "\":";
        if (hasVal) buf += std::to_string(val); else buf += "null";
    }
    void nullableStr(const char* key, const std::string& val, bool hasVal) {
        comma(); buf += "\""; buf += key; buf += "\":";
        if (hasVal) { buf += "\""; buf += jsonEscape(val); buf += "\""; }
        else buf += "null";
    }
    void stringArray(const char* key, const std::vector<std::string>& arr) {
        comma(); buf += "\""; buf += key; buf += "\":[";
        for (size_t i = 0; i < arr.size(); i++) {
            if (i > 0) buf += ",";
            buf += "\""; buf += jsonEscape(arr[i]); buf += "\"";
        }
        buf += "]";
    }
    std::string finish() { buf += "}"; return buf; }
private:
    void comma() { if (!first) buf += ","; first = false; }
};

// ---- Command definitions ----

struct CmdDef {
    const char* name;
    const char* slash;
    const char* alias;  // comma-separated aliases, nullptr if none
    bool threadOk;
};

static const CmdDef COMMANDS[] = {
    {"PLAIN",                     "/plain",            nullptr,         true},
    {"CHANGE_DISPLAY_NAME",       "/nick",             nullptr,         false},
    {"CHANGE_DISPLAY_NAME_FOR_ROOM","/myroomnick",     "/roomnick",     false},
    {"ROOM_AVATAR",               "/roomavatar",       nullptr,         false},
    {"CHANGE_AVATAR_FOR_ROOM",    "/myroomavatar",     nullptr,         false},
    {"TOPIC",                     "/topic",            nullptr,         false},
    {"EMOTE",                     "/me",               nullptr,         true},
    {"RAINBOW",                   "/rainbow",          nullptr,         true},
    {"RAINBOW_EMOTE",             "/rainbowme",        nullptr,         true},
    {"JOIN_ROOM",                 "/join",             "/j,/goto",      false},
    {"PART",                      "/part",             nullptr,         false},
    {"ROOM_NAME",                 "/roomname",         nullptr,         false},
    {"INVITE",                    "/invite",           nullptr,         false},
    {"REMOVE_USER",               "/remove",           "/kick",         false},
    {"BAN_USER",                  "/ban",              nullptr,         false},
    {"UNBAN_USER",                "/unban",            nullptr,         false},
    {"IGNORE_USER",               "/ignore",           nullptr,         true},
    {"UNIGNORE_USER",             "/unignore",         nullptr,         true},
    {"SET_USER_POWER_LEVEL",      "/op",               nullptr,         false},
    {"RESET_USER_POWER_LEVEL",    "/deop",             nullptr,         false},
    {"MARKDOWN",                  "/markdown",         nullptr,         false},
    {"DEVTOOLS",                  "/devtools",         nullptr,         false},
    {"CLEAR_SCALAR_TOKEN",        "/clear_scalar_token",nullptr,        false},
    {"SPOILER",                   "/spoiler",          nullptr,         true},
    {"SHRUG",                     "/shrug",            nullptr,         true},
    {"LENNY",                     "/lenny",            nullptr,         true},
    {"TABLE_FLIP",                "/tableflip",        nullptr,         true},
    {"DISCARD_SESSION",           "/discardsession",   nullptr,         false},
    {"WHOIS",                     "/whois",            nullptr,         true},
    {"CONFETTI",                  "/confetti",         nullptr,         false},
    {"SNOWFALL",                  "/snowfall",         nullptr,         false},
    {"CREATE_SPACE",              "/createspace",      nullptr,         false},
    {"ADD_TO_SPACE",              "/addToSpace",       nullptr,         false},
    {"JOIN_SPACE",                "/joinSpace",        nullptr,         false},
    {"LEAVE_ROOM",                "/leave",            nullptr,         false},
    {"UPGRADE_ROOM",              "/upgraderoom",      nullptr,         false},
    {"CRASH_APP",                 "/crash",            nullptr,         true},
    // PCP commands
    {"LLM",                       "/llm",              nullptr,         true},
    {"LLMP",                      "/llmp",             nullptr,         true},
    {"AGENT",                     "/agent",            nullptr,         true},
    {"WEB",                       "/web",              "/search",       true},
    {"HIDE_EMOJI",                "/hideemoji",        nullptr,         true},
    {"STATS",                     "/stats",            nullptr,         true},
    {"REMIND",                    "/remind",           nullptr,         true},
    {"WEATHER",                   "/weather",          nullptr,         true},
    {"TRANSLATE",                 "/translate",        nullptr,         true},
    {"SCHEDULE",                  "/schedule",         nullptr,         true},
    {"SMSAGENT",                  "/smsagent",         nullptr,         true},
};
static const int CMD_COUNT = sizeof(COMMANDS) / sizeof(COMMANDS[0]);

// Case-insensitive string comparison
static bool iequals(const std::string& a, const std::string& b) {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); i++) {
        if (std::tolower(static_cast<unsigned char>(a[i])) !=
            std::tolower(static_cast<unsigned char>(b[i]))) return false;
    }
    return true;
}

// Check if a slash command string matches a command definition (including aliases)
static bool matchesCmd(const CmdDef& cmd, const std::string& input) {
    if (iequals(input, cmd.slash)) return true;
    if (cmd.alias) {
        std::string aliases(cmd.alias);
        size_t pos = 0;
        while (pos < aliases.size()) {
            size_t comma = aliases.find(',', pos);
            std::string alias = (comma == std::string::npos) ?
                aliases.substr(pos) : aliases.substr(pos, comma - pos);
            // trim
            size_t s = 0;
            while (s < alias.size() && alias[s] == ' ') s++;
            if (iequals(input, alias.substr(s))) return true;
            if (comma == std::string::npos) break;
            pos = comma + 1;
        }
    }
    return false;
}

// Find which command matches the slash prefix
static const CmdDef* findCommand(const std::string& slash) {
    for (int i = 0; i < CMD_COUNT; i++) {
        if (matchesCmd(COMMANDS[i], slash)) return &COMMANDS[i];
    }
    return nullptr;
}

// Split message into parts by whitespace
static bool splitMessage(const std::string& msg, std::vector<std::string>& parts, std::string& remainder) {
    // Split on whitespace
    std::istringstream iss(msg);
    std::string word;
    std::vector<std::string> allParts;
    while (iss >> word) {
        if (!word.empty()) allParts.push_back(word);
    }
    if (allParts.empty()) return false;
    
    // Find where the slash command ends in the original message
    size_t cmdEnd = 0;
    size_t idx = 0;
    for (size_t i = 0; i < allParts.size() && idx < msg.size(); i++) {
        while (idx < msg.size() && msg[idx] == ' ') idx++;
        if (i == 0) {
            // Skip the slash command
            while (idx < msg.size() && msg[idx] != ' ') idx++;
        }
        cmdEnd = idx;
    }
    // cmdEnd is at the end of the last word; remainder is everything after the first word
    size_t firstWordEnd = 0;
    size_t fi = 0;
    while (fi < msg.size() && msg[fi] == ' ') fi++;
    while (fi < msg.size() && msg[fi] != ' ') fi++;
    firstWordEnd = fi;
    
    parts = allParts;
    if (firstWordEnd < msg.size()) {
        while (firstWordEnd < msg.size() && msg[firstWordEnd] == ' ') firstWordEnd++;
        remainder = msg.substr(firstWordEnd);
    } else {
        remainder = "";
    }
    return true;
}

static std::string trimRemainder(const std::string& text, const std::vector<std::string>& parts, int takeCount) {
    // Compute total length of first 'takeCount' parts plus spaces between them
    int totalLen = 0;
    int limit = std::min(takeCount, (int)parts.size());
    for (int i = 0; i < limit; i++) {
        if (i > 0) totalLen++; // space
        totalLen += (int)parts[i].size();
    }
    if (totalLen >= (int)text.size()) return "";
    std::string sub = text.substr(totalLen);
    // trim
    size_t s = 0;
    while (s < sub.size() && sub[s] == ' ') s++;
    sub = sub.substr(s);
    return sub.empty() ? "" : sub;
}

std::string parseSlashCommand(const std::string& textMessage,
                               const std::string& formattedMessage,
                               bool isInThreadTimeline,
                               bool developerMode) {
    // Use formatted message if available, otherwise text
    const std::string& message = formattedMessage.empty() ? textMessage : formattedMessage;
    
    // Must start with "/"
    if (message.empty() || message[0] != '/') {
        return R"({"type":"error_not_a_command"})";
    }
    
    // "/" only
    if (message.size() == 1) {
        return R"({"type":"error_empty_slash_command"})";
    }
    
    // "//" — not a command
    if (message.size() >= 2 && message[1] == '/') {
        return R"({"type":"error_not_a_command"})";
    }
    
    // Split message
    std::vector<std::string> parts;
    std::string remainder;
    if (!splitMessage(message, parts, remainder)) {
        return R"({"type":"error_empty_slash_command"})";
    }
    
    const std::string& slashCommand = parts[0];
    
    // Find matching command
    const CmdDef* cmd = findCommand(slashCommand);
    if (!cmd) {
        JsonObj j;
        j.str("type", "error_unknown");
        j.str("cmd", slashCommand);
        return j.finish();
    }
    
    // Thread check
    if (isInThreadTimeline && !cmd->threadOk) {
        JsonObj j;
        j.str("type", "error_threads");
        j.str("cmd", cmd->name);
        return j.finish();
    }
    
    bool hasMsg = !remainder.empty();
    
    // ---- Dispatch by command name ----
    std::string cmdName(cmd->name);
    
    // PLAIN — send plain text
    if (cmdName == "PLAIN") {
        if (!hasMsg) {
            JsonObj j; j.str("type", "error_syntax"); j.str("cmd", cmdName); return j.finish();
        }
        if (!formattedMessage.empty()) {
            // Also extract plain text version
            std::vector<std::string> textParts;
            std::string textRemainder;
            splitMessage(textMessage, textParts, textRemainder);
            JsonObj j;
            j.str("type", "send_formatted_text");
            j.str("msg", textRemainder.empty() ? remainder : textRemainder);
            j.str("fmt", remainder);
            return j.finish();
        } else {
            JsonObj j;
            j.str("type", "send_plain_text");
            j.str("msg", remainder);
            return j.finish();
        }
    }
    
    // CHANGE_DISPLAY_NAME
    if (cmdName == "CHANGE_DISPLAY_NAME") {
        if (!hasMsg) {
            JsonObj j; j.str("type", "error_syntax"); j.str("cmd", cmdName); return j.finish();
        }
        JsonObj j; j.str("type", "change_display_name"); j.str("name", remainder); return j.finish();
    }
    
    // CHANGE_DISPLAY_NAME_FOR_ROOM
    if (cmdName == "CHANGE_DISPLAY_NAME_FOR_ROOM") {
        if (!hasMsg) {
            JsonObj j; j.str("type", "error_syntax"); j.str("cmd", cmdName); return j.finish();
        }
        JsonObj j; j.str("type", "change_display_name_room"); j.str("name", remainder); return j.finish();
    }
    
    // ROOM_AVATAR
    if (cmdName == "ROOM_AVATAR") {
        if (parts.size() < 2 || !isMxcUrl(parts[1])) {
            JsonObj j; j.str("type", "error_syntax"); j.str("cmd", cmdName); return j.finish();
        }
        JsonObj j; j.str("type", "change_room_avatar"); j.str("url", parts[1]); return j.finish();
    }
    
    // CHANGE_AVATAR_FOR_ROOM
    if (cmdName == "CHANGE_AVATAR_FOR_ROOM") {
        if (parts.size() < 2 || !isMxcUrl(parts[1])) {
            JsonObj j; j.str("type", "error_syntax"); j.str("cmd", cmdName); return j.finish();
        }
        JsonObj j; j.str("type", "change_avatar_room"); j.str("url", parts[1]); return j.finish();
    }
    
    // TOPIC
    if (cmdName == "TOPIC") {
        if (!hasMsg) {
            JsonObj j; j.str("type", "error_syntax"); j.str("cmd", cmdName); return j.finish();
        }
        JsonObj j; j.str("type", "change_topic"); j.str("topic", remainder); return j.finish();
    }
    
    // EMOTE
    if (cmdName == "EMOTE") {
        if (!hasMsg) {
            JsonObj j; j.str("type", "error_syntax"); j.str("cmd", cmdName); return j.finish();
        }
        JsonObj j; j.str("type", "send_emote"); j.str("msg", remainder); return j.finish();
    }
    
    // RAINBOW
    if (cmdName == "RAINBOW") {
        if (!hasMsg) {
            JsonObj j; j.str("type", "error_syntax"); j.str("cmd", cmdName); return j.finish();
        }
        JsonObj j; j.str("type", "send_rainbow"); j.str("msg", remainder); return j.finish();
    }
    
    // RAINBOW_EMOTE
    if (cmdName == "RAINBOW_EMOTE") {
        if (!hasMsg) {
            JsonObj j; j.str("type", "error_syntax"); j.str("cmd", cmdName); return j.finish();
        }
        JsonObj j; j.str("type", "send_rainbow_emote"); j.str("msg", remainder); return j.finish();
    }
    
    // JOIN_ROOM
    if (cmdName == "JOIN_ROOM") {
        if (parts.size() < 2 || parts[1].empty()) {
            JsonObj j; j.str("type", "error_syntax"); j.str("cmd", cmdName); return j.finish();
        }
        JsonObj j;
        j.str("type", "join_room");
        j.str("alias", parts[1]);
        std::string reason = trimRemainder(textMessage, parts, 2);
        j.nullableStr("reason", reason, !reason.empty());
        return j.finish();
    }
    
    // PART
    if (cmdName == "PART") {
        if (parts.size() == 1) {
            JsonObj j; j.str("type", "part_room"); j.nullableStr("alias", "", false); return j.finish();
        } else if (parts.size() == 2) {
            JsonObj j; j.str("type", "part_room"); j.str("alias", parts[1]); return j.finish();
        } else {
            JsonObj j; j.str("type", "error_syntax"); j.str("cmd", cmdName); return j.finish();
        }
    }
    
    // ROOM_NAME
    if (cmdName == "ROOM_NAME") {
        if (!hasMsg) {
            JsonObj j; j.str("type", "error_syntax"); j.str("cmd", cmdName); return j.finish();
        }
        JsonObj j; j.str("type", "change_room_name"); j.str("name", remainder); return j.finish();
    }
    
    // INVITE
    if (cmdName == "INVITE") {
        if (parts.size() < 2) {
            JsonObj j; j.str("type", "error_syntax"); j.str("cmd", cmdName); return j.finish();
        }
        const std::string& userId = parts[1];
        if (isUserId(userId)) {
            std::string reason = trimRemainder(textMessage, parts, 2);
            JsonObj j;
            j.str("type", "invite");
            j.str("user", userId);
            j.nullableStr("reason", reason, !reason.empty());
            return j.finish();
        } else if (isEmail(userId)) {
            JsonObj j; j.str("type", "invite_3pid_email"); j.str("email", userId); return j.finish();
        } else {
            // Let Kotlin side check isMsisdn — we pass as phone
            JsonObj j; j.str("type", "invite_3pid_msisdn"); j.str("phone", userId); return j.finish();
        }
    }
    
    // REMOVE_USER
    if (cmdName == "REMOVE_USER") {
        if (parts.size() < 2 || !isUserId(parts[1])) {
            JsonObj j; j.str("type", "error_syntax"); j.str("cmd", cmdName); return j.finish();
        }
        std::string reason = trimRemainder(textMessage, parts, 2);
        JsonObj j;
        j.str("type", "remove_user");
        j.str("user", parts[1]);
        j.nullableStr("reason", reason, !reason.empty());
        return j.finish();
    }
    
    // BAN_USER
    if (cmdName == "BAN_USER") {
        if (parts.size() < 2 || !isUserId(parts[1])) {
            JsonObj j; j.str("type", "error_syntax"); j.str("cmd", cmdName); return j.finish();
        }
        std::string reason = trimRemainder(textMessage, parts, 2);
        JsonObj j;
        j.str("type", "ban_user");
        j.str("user", parts[1]);
        j.nullableStr("reason", reason, !reason.empty());
        return j.finish();
    }
    
    // UNBAN_USER
    if (cmdName == "UNBAN_USER") {
        if (parts.size() < 2 || !isUserId(parts[1])) {
            JsonObj j; j.str("type", "error_syntax"); j.str("cmd", cmdName); return j.finish();
        }
        std::string reason = trimRemainder(textMessage, parts, 2);
        JsonObj j;
        j.str("type", "unban_user");
        j.str("user", parts[1]);
        j.nullableStr("reason", reason, !reason.empty());
        return j.finish();
    }
    
    // IGNORE_USER
    if (cmdName == "IGNORE_USER") {
        if (parts.size() < 2 || !isUserId(parts[1])) {
            JsonObj j; j.str("type", "error_syntax"); j.str("cmd", cmdName); return j.finish();
        }
        JsonObj j; j.str("type", "ignore_user"); j.str("user", parts[1]); return j.finish();
    }
    
    // UNIGNORE_USER
    if (cmdName == "UNIGNORE_USER") {
        if (parts.size() < 2 || !isUserId(parts[1])) {
            JsonObj j; j.str("type", "error_syntax"); j.str("cmd", cmdName); return j.finish();
        }
        JsonObj j; j.str("type", "unignore_user"); j.str("user", parts[1]); return j.finish();
    }
    
    // SET_USER_POWER_LEVEL
    if (cmdName == "SET_USER_POWER_LEVEL") {
        if (parts.size() < 2 || !isUserId(parts[1])) {
            JsonObj j; j.str("type", "error_syntax"); j.str("cmd", cmdName); return j.finish();
        }
        if (parts.size() >= 3) {
            try {
                int level = std::stoi(parts[2]);
                JsonObj j; j.str("type", "set_power"); j.str("user", parts[1]);
                j.integer("level", level); return j.finish();
            } catch (...) {
                JsonObj j; j.str("type", "error_syntax"); j.str("cmd", cmdName); return j.finish();
            }
        } else {
            JsonObj j; j.str("type", "error_syntax"); j.str("cmd", cmdName); return j.finish();
        }
    }
    
    // RESET_USER_POWER_LEVEL
    if (cmdName == "RESET_USER_POWER_LEVEL") {
        if (parts.size() < 2 || !isUserId(parts[1])) {
            JsonObj j; j.str("type", "error_syntax"); j.str("cmd", cmdName); return j.finish();
        }
        JsonObj j; j.str("type", "set_power"); j.str("user", parts[1]);
        j.nullableInt("level", 0, false); return j.finish();
    }
    
    // MARKDOWN
    if (cmdName == "MARKDOWN") {
        if (parts.size() < 2) {
            JsonObj j; j.str("type", "error_syntax"); j.str("cmd", cmdName); return j.finish();
        }
        std::string val = parts[1];
        std::transform(val.begin(), val.end(), val.begin(),
                       [](unsigned char c){ return std::tolower(c); });
        if (val == "on") {
            JsonObj j; j.str("type", "set_markdown"); j.boolean("enable", true); return j.finish();
        } else if (val == "off") {
            JsonObj j; j.str("type", "set_markdown"); j.boolean("enable", false); return j.finish();
        } else {
            JsonObj j; j.str("type", "error_syntax"); j.str("cmd", cmdName); return j.finish();
        }
    }
    
    // DEVTOOLS
    if (cmdName == "DEVTOOLS") {
        if (parts.size() == 1) return R"({"type":"devtools"})";
        JsonObj j; j.str("type", "error_syntax"); j.str("cmd", cmdName); return j.finish();
    }
    
    // CLEAR_SCALAR_TOKEN
    if (cmdName == "CLEAR_SCALAR_TOKEN") {
        if (parts.size() == 1) return R"({"type":"clear_scalar_token"})";
        JsonObj j; j.str("type", "error_syntax"); j.str("cmd", cmdName); return j.finish();
    }
    
    // SPOILER
    if (cmdName == "SPOILER") {
        if (!hasMsg) {
            JsonObj j; j.str("type", "error_syntax"); j.str("cmd", cmdName); return j.finish();
        }
        JsonObj j; j.str("type", "send_spoiler"); j.str("msg", remainder); return j.finish();
    }
    
    // SHRUG
    if (cmdName == "SHRUG") {
        JsonObj j; j.str("type", "send_shrug"); j.str("msg", remainder); return j.finish();
    }
    
    // LENNY
    if (cmdName == "LENNY") {
        JsonObj j; j.str("type", "send_lenny"); j.str("msg", remainder); return j.finish();
    }
    
    // TABLE_FLIP
    if (cmdName == "TABLE_FLIP") {
        JsonObj j; j.str("type", "send_tableflip"); j.str("msg", remainder); return j.finish();
    }
    
    // DISCARD_SESSION
    if (cmdName == "DISCARD_SESSION") {
        if (parts.size() == 1) return R"({"type":"discard_session"})";
        JsonObj j; j.str("type", "error_syntax"); j.str("cmd", cmdName); return j.finish();
    }
    
    // WHOIS
    if (cmdName == "WHOIS") {
        if (parts.size() < 2 || !isUserId(parts[1])) {
            JsonObj j; j.str("type", "error_syntax"); j.str("cmd", cmdName); return j.finish();
        }
        JsonObj j; j.str("type", "whois"); j.str("user", parts[1]); return j.finish();
    }
    
    // CONFETTI
    if (cmdName == "CONFETTI") {
        JsonObj j; j.str("type", "send_chat_effect"); j.str("effect", "CONFETTI"); j.str("msg", remainder); return j.finish();
    }
    
    // SNOWFALL
    if (cmdName == "SNOWFALL") {
        JsonObj j; j.str("type", "send_chat_effect"); j.str("effect", "SNOWFALL"); j.str("msg", remainder); return j.finish();
    }
    
    // CREATE_SPACE
    if (cmdName == "CREATE_SPACE") {
        if (parts.size() < 2) {
            JsonObj j; j.str("type", "error_syntax"); j.str("cmd", cmdName); return j.finish();
        }
        std::vector<std::string> invitees;
        for (size_t i = 2; i < parts.size(); i++) invitees.push_back(parts[i]);
        JsonObj j;
        j.str("type", "create_space");
        j.str("name", parts[1]);
        j.stringArray("invitees", invitees);
        return j.finish();
    }
    
    // ADD_TO_SPACE
    if (cmdName == "ADD_TO_SPACE") {
        if (parts.size() < 2) {
            JsonObj j; j.str("type", "error_syntax"); j.str("cmd", cmdName); return j.finish();
        }
        JsonObj j; j.str("type", "add_to_space"); j.str("space_id", parts.back()); return j.finish();
    }
    
    // JOIN_SPACE
    if (cmdName == "JOIN_SPACE") {
        if (parts.size() < 2) {
            JsonObj j; j.str("type", "error_syntax"); j.str("cmd", cmdName); return j.finish();
        }
        JsonObj j; j.str("type", "join_space"); j.str("id", parts.back()); return j.finish();
    }
    
    // LEAVE_ROOM
    if (cmdName == "LEAVE_ROOM") {
        JsonObj j; j.str("type", "leave_room"); j.str("room_id", remainder); return j.finish();
    }
    
    // UPGRADE_ROOM
    if (cmdName == "UPGRADE_ROOM") {
        if (!hasMsg) {
            JsonObj j; j.str("type", "error_syntax"); j.str("cmd", cmdName); return j.finish();
        }
        JsonObj j; j.str("type", "upgrade_room"); j.str("version", remainder); return j.finish();
    }
    
    // CRASH_APP
    if (cmdName == "CRASH_APP") {
        if (developerMode) {
            // In Kotlin this throws RuntimeException — we can't do that in C++
            // Return a special type for the Kotlin side to handle
            return R"({"type":"crash_app"})";
        }
        // Fall through to unknown
        JsonObj j; j.str("type", "error_unknown"); j.str("cmd", slashCommand); return j.finish();
    }
    
    // PCP native commands — pass through
    if (cmdName == "LLM") {
        JsonObj j; j.str("type", "llm"); j.str("query", remainder); return j.finish();
    }
    if (cmdName == "LLMP") {
        JsonObj j; j.str("type", "llmp"); j.str("query", remainder); return j.finish();
    }
    if (cmdName == "AGENT") {
        JsonObj j; j.str("type", "agent"); j.str("task", remainder); return j.finish();
    }
    if (cmdName == "WEB") {
        JsonObj j; j.str("type", "web"); j.str("query", remainder); return j.finish();
    }
    if (cmdName == "HIDE_EMOJI") {
        return R"({"type":"hide_emoji"})";
    }
    if (cmdName == "STATS") {
        return R"({"type":"stats"})";
    }
    if (cmdName == "REMIND") {
        JsonObj j; j.str("type", "remind"); j.str("msg", remainder); return j.finish();
    }
    if (cmdName == "WEATHER") {
        JsonObj j; j.str("type", "weather"); j.str("city", remainder); return j.finish();
    }
    if (cmdName == "TRANSLATE") {
        JsonObj j; j.str("type", "translate"); j.str("text", remainder); return j.finish();
    }
    if (cmdName == "SCHEDULE") {
        JsonObj j; j.str("type", "schedule"); j.str("msg", remainder); return j.finish();
    }
    if (cmdName == "SMSAGENT") {
        JsonObj j; j.str("type", "smsagent"); j.str("msg", remainder); return j.finish();
    }
    
    // Unknown command
    JsonObj j; j.str("type", "error_unknown"); j.str("cmd", slashCommand); return j.finish();
}

} // namespace pcp
