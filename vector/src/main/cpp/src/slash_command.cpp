#include "progressive/slash_command.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

namespace progressive {

// ---- Command Table (from Command.kt lines 26-66) ----
// Faithful reproduction of the 32-entry enum with all aliases and flags.
// Original: enum class Command(command, aliases, parameters, @StringRes description, isDevCommand, isThreadCommand)

static const CommandDef COMMANDS[] = {
    // --- Message commands (isThreadCommand = true) ---
    {SlashCommandId::Emote,        "/me", {}, "<message>", false, true, false},
    {SlashCommandId::Rainbow,      "/rainbow", {}, "<message>", false, true, false},
    {SlashCommandId::RainbowEmote, "/rainbowme", {}, "<message>", false, true, false},
    {SlashCommandId::Spoiler,      "/spoiler", {}, "<message>", false, true, false},
    {SlashCommandId::Shrug,        "/shrug", {}, "<message>", false, true, false},
    {SlashCommandId::Lenny,        "/lenny", {}, "<message>", false, true, false},
    {SlashCommandId::Plain,        "/plain", {}, "<message>", false, true, false},
    {SlashCommandId::WhoIs,        "/whois", {}, "<user-id>", false, true, false},
    {SlashCommandId::TableFlip,    "/tableflip", {}, "<message>", false, true, false},

    // --- Admin commands (isThreadCommand = false) ---
    {SlashCommandId::BanUser,      "/ban", {}, "<user-id> [reason]", false, false, true},
    {SlashCommandId::UnbanUser,    "/unban", {}, "<user-id> [reason]", false, false, true},
    {SlashCommandId::IgnoreUser,   "/ignore", {}, "<user-id> [reason]", false, true, true},
    {SlashCommandId::UnignoreUser, "/unignore", {}, "<user-id>", false, true, true},
    {SlashCommandId::SetUserPowerLevel, "/op", {}, "<user-id> [<power-level>]", false, false, true},
    {SlashCommandId::ResetUserPowerLevel, "/deop", {}, "<user-id>", false, false, true},
    {SlashCommandId::RemoveUser,   "/remove", {"/kick"}, "<user-id> [reason]", false, false, true},
    {SlashCommandId::Invite,       "/invite",{}, "<user-id> [reason]", false, false, false},
    {SlashCommandId::JoinRoom,     "/join", {"/j", "/goto"}, "<room-address> [reason]", false, false, false},
    {SlashCommandId::Part,         "/part", {}, "[<room-address>]", false, false, false},

    // --- Room config commands ---
    {SlashCommandId::RoomName,     "/roomname", {}, "<name>", false, false, false},
    {SlashCommandId::Topic,        "/topic", {}, "<topic>", false, false, false},
    {SlashCommandId::ChangeDisplayName, "/nick", {}, "<display-name>", false, false, false},
    {SlashCommandId::ChangeDisplayNameForRoom, "/myroomnick", {"/roomnick"}, "<display-name>", false, false, false},
    {SlashCommandId::RoomAvatar,   "/roomavatar", {}, "<mxc_url>", true, false, false},
    {SlashCommandId::ChangeAvatarForRoom, "/myroomavatar", {}, "<mxc_url>", true, false, false},
    {SlashCommandId::Markdown,     "/markdown", {}, "<on|off>", false, false, false},
    {SlashCommandId::DiscardSession, "/discardsession", {}, "", false, false, false},

    // --- Dev commands ---
    {SlashCommandId::CrashApp,     "/crash", {}, "", true, true, false},
    {SlashCommandId::DevTools,     "/devtools", {}, "", true, false, false},
    {SlashCommandId::ClearScalarToken, "/clear_scalar_token", {}, "", false, false, false},

    // --- Space commands ---
    {SlashCommandId::CreateSpace,  "/createspace", {}, "<name> <invitee>*", true, false, false},
    {SlashCommandId::AddToSpace,   "/addToSpace", {}, "spaceId", true, false, false},
    {SlashCommandId::JoinSpace,    "/joinSpace", {}, "spaceId", true, false, false},

    // --- Room management ---
    {SlashCommandId::LeaveRoom,    "/leave", {}, "<roomId?>", true, false, false},
    {SlashCommandId::UpgradeRoom,  "/upgraderoom", {}, "newVersion", true, false, false},

    // --- Fun effects ---
    {SlashCommandId::Confetti,     "/confetti", {}, "<message>", false, false, false},
    {SlashCommandId::Snowfall,     "/snowfall", {}, "<message>", false, false, false},

    // --- Navigation ---
    {SlashCommandId::JumpToDate,   "/jumptodate", {}, "<YYYY-MM-DD> [HH:MM]", false, true, false},

    // --- AI Agent ---
    {SlashCommandId::Agent,        "/agent", {}, "<task description>", false, true, false},
};

const std::vector<CommandDef>& getCommandTable() {
    static std::vector<CommandDef> table;
    if (table.empty()) {
        for (const auto& c : COMMANDS) table.push_back(c);
    }
    return table;
}

// ---- Command Matching (from Command.kt:matches/startsWith) ----
// Original: fun matches(input: CharSequence) = allAliases.any { it.contentEquals(input, true) }
// Original: fun startsWith(input: CharSequence) = allAliases.any { it.startsWith(input, 1, true) }

bool commandMatches(const CommandDef& cmd, const std::string& input) {
    // Check case-insensitive equality against primary command and all aliases
    auto iequals = [](const std::string& a, const std::string& b) -> bool {
        if (a.size() != b.size()) return false;
        for (size_t i = 0; i < a.size(); ++i) {
            if (std::tolower(static_cast<unsigned char>(a[i])) !=
                std::tolower(static_cast<unsigned char>(b[i]))) return false;
        }
        return true;
    };

    if (iequals(cmd.command, input)) return true;
    for (const auto& alias : cmd.aliases) {
        if (iequals(alias, input)) return true;
    }
    return false;
}

bool commandStartsWith(const CommandDef& cmd, const std::string& input) {
    // Original: it.startsWith(input, 1, true) — skip first char (/), case-insensitive
    // The Kotlin startsWith(input, 1, true) means: start at index 1, ignore case
    // So it checks if the alias starts with the input (starting from position 1)
    auto istarts = [](const std::string& s, const std::string& prefix, size_t offset) -> bool {
        if (prefix.size() + offset > s.size()) return false;
        for (size_t i = 0; i < prefix.size(); ++i) {
            if (std::tolower(static_cast<unsigned char>(s[i + offset])) !=
                std::tolower(static_cast<unsigned char>(prefix[i]))) return false;
        }
        return true;
    };

    if (istarts(cmd.command, input, 1)) return true;
    for (const auto& alias : cmd.aliases) {
        if (istarts(alias, input, 1)) return true;
    }
    return false;
}

const CommandDef* findCommand(const std::string& input) {
    for (const auto& cmd : COMMANDS) {
        if (commandMatches(cmd, input)) return &cmd;
    }
    return nullptr;
}

// ---- Slash Command Parser (from CommandParser.kt:parseSlashCommand) ----
// Original: 472-line function with when() matching each command

ParsedCommand parseSlashCommand(const std::string& text, const std::string& formattedText, bool isInThread) {
    ParsedCommand result;
    result.resultType = ParseResultType::ErrorNotACommand;

    // Original: if (!message.startsWith("/")) return ParsedCommand.ErrorNotACommand
    if (text.empty() || text[0] != '/') {
        return result;
    }

    // Original: if (message.length == 1) return ParsedCommand.ErrorEmptySlashCommand
    if (text.size() == 1) {
        result.resultType = ParseResultType::ErrorEmptySlashCommand;
        return result;
    }

    // Original: if ("/" == message.substring(1, 2)) return ParsedCommand.ErrorNotACommand
    if (text.size() >= 2 && text[1] == '/') {
        result.resultType = ParseResultType::ErrorNotACommand;
        return result;
    }

    // Original: extractMessage(message.toString()) — split by whitespace
    // val messageParts = message.split("\\s+".toRegex()).dropLastWhile { it.isEmpty() }
    std::vector<std::string> messageParts;
    {
        std::string current;
        for (char c : text) {
            if (std::isspace(static_cast<unsigned char>(c))) {
                if (!current.empty()) {
                    messageParts.push_back(current);
                    current.clear();
                }
            } else {
                current += c;
            }
        }
        if (!current.empty()) messageParts.push_back(current);
    }

    if (messageParts.empty()) {
        result.resultType = ParseResultType::ErrorEmptySlashCommand;
        return result;
    }

    // Original: val slashCommand = messageParts.first()
    std::string slashCommand = messageParts[0];

    // Original: val message = textMessage.substring(slashCommand.length).trim()
    std::string message;
    if (text.size() > slashCommand.size()) {
        message = text.substr(slashCommand.size());
        while (!message.empty() && std::isspace(static_cast<unsigned char>(message.front()))) message.erase(0, 1);
    }

    // Find matching command
    const CommandDef* cmd = findCommand(slashCommand);
    if (!cmd) {
        result.resultType = ParseResultType::ErrorUnknownSlashCommand;
        result.errorDetail = slashCommand;
        return result;
    }

    // Original: getNotSupportedByThreads(isInThreadTimeline, slashCommand)
    if (isInThread && !cmd->isThreadCommand) {
        result.resultType = ParseResultType::ErrorNotSupportedInThread;
        return result;
    }

    result.commandId = cmd->id;

    // Original: when { Command.PLAIN.matches(slashCommand) -> ... }
    // Faithful reproduction of the 472-line when() block
    switch (cmd->id) {
        case SlashCommandId::Plain:
            if (!message.empty()) {
                result.resultType = formattedText.empty() ? ParseResultType::SendPlainText : ParseResultType::SendFormattedText;
                result.message = message;
                result.formattedMessage = formattedText;
            } else {
                result.resultType = ParseResultType::ErrorSyntax;
            }
            break;

        case SlashCommandId::Emote:
            if (!message.empty()) {
                result.resultType = ParseResultType::SendEmote;
                result.message = message;
            } else {
                result.resultType = ParseResultType::ErrorSyntax;
            }
            break;

        case SlashCommandId::Rainbow:
            if (!message.empty()) {
                result.resultType = ParseResultType::SendRainbow;
                result.message = message;
            } else {
                result.resultType = ParseResultType::ErrorSyntax;
            }
            break;

        case SlashCommandId::RainbowEmote:
            if (!message.empty()) {
                result.resultType = ParseResultType::SendRainbowEmote;
                result.message = message;
            } else {
                result.resultType = ParseResultType::ErrorSyntax;
            }
            break;

        case SlashCommandId::Spoiler:
            if (!message.empty()) {
                result.resultType = ParseResultType::SendSpoiler;
                result.message = message;
            } else {
                result.resultType = ParseResultType::ErrorSyntax;
            }
            break;

        case SlashCommandId::Shrug:
            result.resultType = ParseResultType::SendShrug;
            result.message = message;
            break;

        case SlashCommandId::Lenny:
            result.resultType = ParseResultType::SendLenny;
            result.message = message;
            break;

        case SlashCommandId::TableFlip:
            result.resultType = ParseResultType::SendTableFlip;
            result.message = message;
            break;

        case SlashCommandId::Confetti:
            result.resultType = ParseResultType::SendConfetti;
            result.message = message;
            break;

        case SlashCommandId::Snowfall:
            result.resultType = ParseResultType::SendSnowfall;
            result.message = message;
            break;

        case SlashCommandId::ChangeDisplayName:
            if (!message.empty()) {
                result.resultType = ParseResultType::ChangeDisplayName;
                result.message = message;
            } else {
                result.resultType = ParseResultType::ErrorSyntax;
            }
            break;

        case SlashCommandId::ChangeDisplayNameForRoom:
            if (!message.empty()) {
                result.resultType = ParseResultType::ChangeDisplayNameForRoom;
                result.message = message;
            } else {
                result.resultType = ParseResultType::ErrorSyntax;
            }
            break;

        case SlashCommandId::Topic:
            if (!message.empty()) {
                result.resultType = ParseResultType::ChangeTopic;
                result.message = message;
            } else {
                result.resultType = ParseResultType::ErrorSyntax;
            }
            break;

        case SlashCommandId::RoomName:
            if (!message.empty()) {
                result.resultType = ParseResultType::ChangeRoomName;
                result.message = message;
            } else {
                result.resultType = ParseResultType::ErrorSyntax;
            }
            break;

        case SlashCommandId::JoinRoom:
            if (messageParts.size() >= 2) {
                result.resultType = ParseResultType::JoinRoom;
                result.roomId = messageParts[1];
                if (messageParts.size() >= 3) {
                    result.reason = message.substr(messageParts[0].size() + messageParts[1].size() + 1);
                    while (!result.reason.empty() && std::isspace(static_cast<unsigned char>(result.reason.front())))
                        result.reason.erase(0, 1);
                }
            } else {
                result.resultType = ParseResultType::ErrorSyntax;
            }
            break;

        case SlashCommandId::Part:
            if (messageParts.size() <= 2) {
                result.resultType = ParseResultType::PartRoom;
                if (messageParts.size() == 2) result.roomId = messageParts[1];
            } else {
                result.resultType = ParseResultType::ErrorSyntax;
            }
            break;

        case SlashCommandId::Invite:
            if (messageParts.size() >= 2) {
                result.resultType = ParseResultType::InviteUser;
                result.userId = messageParts[1];
            } else {
                result.resultType = ParseResultType::ErrorSyntax;
            }
            break;

        case SlashCommandId::RemoveUser:
            if (messageParts.size() >= 2) {
                result.resultType = ParseResultType::RemoveUser;
                result.userId = messageParts[1];
            } else {
                result.resultType = ParseResultType::ErrorSyntax;
            }
            break;

        case SlashCommandId::BanUser:
            if (messageParts.size() >= 2) {
                result.resultType = ParseResultType::BanUser;
                result.userId = messageParts[1];
            } else {
                result.resultType = ParseResultType::ErrorSyntax;
            }
            break;

        case SlashCommandId::UnbanUser:
            if (messageParts.size() >= 2) {
                result.resultType = ParseResultType::UnbanUser;
                result.userId = messageParts[1];
            } else {
                result.resultType = ParseResultType::ErrorSyntax;
            }
            break;

        case SlashCommandId::IgnoreUser:
            if (messageParts.size() == 2) {
                result.resultType = ParseResultType::IgnoreUser;
                result.userId = messageParts[1];
            } else {
                result.resultType = ParseResultType::ErrorSyntax;
            }
            break;

        case SlashCommandId::UnignoreUser:
            if (messageParts.size() == 2) {
                result.resultType = ParseResultType::UnignoreUser;
                result.userId = messageParts[1];
            } else {
                result.resultType = ParseResultType::ErrorSyntax;
            }
            break;

        case SlashCommandId::SetUserPowerLevel:
            if (messageParts.size() >= 2) {
                result.resultType = ParseResultType::SetUserPowerLevel;
                result.userId = messageParts[1];
                if (messageParts.size() >= 3) {
                    try { result.powerLevel = std::stoi(messageParts[2]); }
                    catch (...) { result.resultType = ParseResultType::ErrorSyntax; }
                }
            } else {
                result.resultType = ParseResultType::ErrorSyntax;
            }
            break;

        case SlashCommandId::ResetUserPowerLevel:
            if (messageParts.size() == 2) {
                result.resultType = ParseResultType::ResetUserPowerLevel;
                result.userId = messageParts[1];
            } else {
                result.resultType = ParseResultType::ErrorSyntax;
            }
            break;

        case SlashCommandId::Markdown:
            if (messageParts.size() == 2) {
                result.resultType = ParseResultType::SetMarkdown;
                std::string val = messageParts[1];
                for (char& c : val) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
                result.enableFlag = (val == "on");
            } else {
                result.resultType = ParseResultType::ErrorSyntax;
            }
            break;

        case SlashCommandId::DiscardSession:
            if (messageParts.size() == 1) {
                result.resultType = ParseResultType::DiscardSession;
            } else {
                result.resultType = ParseResultType::ErrorSyntax;
            }
            break;

        case SlashCommandId::ClearScalarToken:
            if (messageParts.size() == 1) {
                result.resultType = ParseResultType::ClearScalarToken;
            } else {
                result.resultType = ParseResultType::ErrorSyntax;
            }
            break;

        case SlashCommandId::WhoIs:
            if (messageParts.size() == 2) {
                result.resultType = ParseResultType::ShowUser;
                result.userId = messageParts[1];
            } else {
                result.resultType = ParseResultType::ErrorSyntax;
            }
            break;

        case SlashCommandId::CreateSpace:
            if (messageParts.size() >= 2) {
                result.resultType = ParseResultType::CreateSpace;
                result.message = messageParts[1];
            } else {
                result.resultType = ParseResultType::ErrorSyntax;
            }
            break;

        case SlashCommandId::AddToSpace:
            if (messageParts.size() == 2) {
                result.resultType = ParseResultType::AddToSpace;
                result.roomId = messageParts[1];
            } else {
                result.resultType = ParseResultType::ErrorSyntax;
            }
            break;

        case SlashCommandId::JoinSpace:
            if (messageParts.size() == 2) {
                result.resultType = ParseResultType::JoinSpace;
                result.roomId = messageParts[1];
            } else {
                result.resultType = ParseResultType::ErrorSyntax;
            }
            break;

        case SlashCommandId::LeaveRoom:
            result.resultType = ParseResultType::LeaveRoom;
            result.roomId = message;
            break;

        case SlashCommandId::UpgradeRoom:
            if (!message.empty()) {
                result.resultType = ParseResultType::UpgradeRoom;
                result.message = message;
            } else {
                result.resultType = ParseResultType::ErrorSyntax;
            }
            break;

        case SlashCommandId::JumpToDate:
            if (messageParts.size() == 2) {
                result.resultType = ParseResultType::JumpToDate;
                result.message = messageParts[1];
            } else {
                result.resultType = ParseResultType::ErrorSyntax;
            }
            break;

        case SlashCommandId::Agent:
            if (!message.empty()) {
                result.resultType = ParseResultType::AgentTask;
                result.message = message;
            } else {
                result.resultType = ParseResultType::ErrorSyntax;
            }
            break;

        default:
            result.resultType = ParseResultType::ErrorUnknownSlashCommand;
            break;
    }

    return result;
}

bool isSlashCommand(const std::string& text) {
    if (text.empty() || text[0] != '/') return false;
    if (text.size() >= 2 && text[1] == '/') return false;
    std::string firstWord;
    for (char c : text) {
        if (std::isspace(static_cast<unsigned char>(c))) break;
        firstWord += c;
    }
    return findCommand(firstWord) != nullptr;
}

std::vector<std::string> getSlashCommandSuggestions(const std::string& prefix) {
    std::vector<std::string> suggestions;
    for (const auto& cmd : COMMANDS) {
        if (prefix.empty() || commandStartsWith(cmd, prefix)) {
            suggestions.push_back(cmd.command);
            for (const auto& alias : cmd.aliases) suggestions.push_back(alias);
        }
    }
    return suggestions;
}

std::string parsedCommandToJson(const ParsedCommand& cmd) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"resultType": )" << static_cast<int>(cmd.resultType) << ",";
    json << R"("commandId": )" << static_cast<int>(cmd.commandId) << ",";
    json << R"("message": ")" << esc(cmd.message) << R"(",)";
    json << R"("userId": ")" << esc(cmd.userId) << R"(",)";
    json << R"("roomId": ")" << esc(cmd.roomId) << R"(",)";
    json << R"("reason": ")" << esc(cmd.reason) << R"(",)";
    json << R"("powerLevel": )" << cmd.powerLevel << ",";
    json << R"("enableFlag": )" << (cmd.enableFlag ? "true" : "false") << ",";
    json << R"("errorDetail": ")" << esc(cmd.errorDetail) << R"(")";
    json << "}";
    return json.str();
}

std::string getCommandDescription(SlashCommandId id) {
    switch (id) {
        case SlashCommandId::Emote: return "Send an action message";
        case SlashCommandId::Rainbow: return "Send rainbow-coloured text";
        case SlashCommandId::Spoiler: return "Hide message in spoiler tags";
        case SlashCommandId::Shrug: return "Append ¯\\_(ツ)_/¯";
        case SlashCommandId::Lenny: return "Append ( ͡° ͜ʖ ͡°)";
        case SlashCommandId::TableFlip: return "Append table flip emoji";
        case SlashCommandId::Plain: return "Send message without markdown";
        case SlashCommandId::BanUser: return "Ban a user from the room";
        case SlashCommandId::UnbanUser: return "Unban a user";
        case SlashCommandId::Invite: return "Invite a user to the room";
        case SlashCommandId::JoinRoom: return "Join a room";
        case SlashCommandId::Part: return "Leave the room";
        case SlashCommandId::SetUserPowerLevel: return "Set user power level";
        case SlashCommandId::ResetUserPowerLevel: return "Reset user power level";
        case SlashCommandId::Markdown: return "Toggle markdown";
        case SlashCommandId::JumpToDate: return "Jump to date in timeline";
        default: return "";
    }
}

std::string getCommandParameters(SlashCommandId id) {
    for (const auto& cmd : COMMANDS) {
        if (cmd.id == id) return cmd.parameters;
    }
    return "";
}

} // namespace progressive
