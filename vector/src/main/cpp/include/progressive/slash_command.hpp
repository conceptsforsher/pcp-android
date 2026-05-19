#ifndef PROGRESSIVE_SLASH_COMMAND_HPP
#define PROGRESSIVE_SLASH_COMMAND_HPP

#include <string>
#include <vector>

namespace progressive {

// ---- Slash Command Parser ----
// Faithful port from original Kotlin:
//   im.vector.app.features.command.Command.kt          (74 lines, 32 commands)
//   im.vector.app.features.command.CommandParser.kt    (472 lines, full parser)
//   im.vector.app.features.command.ParsedCommand.kt    (69 lines, sealed result types)
//
// Command.kt structure:
//   enum class Command(command, aliases, parameters, @StringRes description, isDevCommand, isThreadCommand)
//   - 32 predefined slash commands
//   - allAliases = arrayOf(command, *aliases.orEmpty())
//   - matches(input) = allAliases.any { it.contentEquals(input, true) }
//   - startsWith(input) = allAliases.any { it.startsWith(input, 1, true) }
//
// CommandParser.kt structure:
//   - extractMessage(): split by whitespace, get parts list + trimmed message
//   - getNotSupportedByThreads(): filter commands by isThreadCommand flag
//   - parseSlashCommand(): 472-line when() matching each command
//   - Handles: // escape, thread restrictions, dev commands, MXC URL validation

// ---- Command Definitions (from Command.kt) ----
// Original: enum class Command(command, aliases, parameters, description, isDevCommand, isThreadCommand)

enum class SlashCommandId {
    // Message commands (isMessage = true, isThreadCommand = true)
    Emote,              // /me <message> — action/emote, aliases: none
    Rainbow,            // /rainbow <message> — rainbow text
    RainbowEmote,       // /rainbowme <message> — rainbow action
    Spoiler,            // /spoiler <message> — ||spoiler|| text
    Shrug,              // /shrug <message> — appends ¯\_(ツ)_/¯
    Lenny,              // /lenny <message> — appends ( ͡° ͜ʖ ͡°)
    Plain,              // /plain <message> — plain text, no markdown
    WhoIs,              // /whois <user-id> — show user info
    TableFlip,          // /tableflip <message> — appends table flip

    // Admin commands (isAdmin = true, isThreadCommand = false)
    BanUser,            // /ban <user-id> [reason]
    UnbanUser,          // /unban <user-id> [reason]
    IgnoreUser,         // /ignore <user-id> [reason]
    UnignoreUser,       // /unignore <user-id>
    SetUserPowerLevel,  // /op <user-id> [<power-level>]
    ResetUserPowerLevel,// /deop <user-id>
    RemoveUser,         // /remove | /kick <user-id> [reason]
    Invite,             // /invite <user-id> [reason]
    JoinRoom,           // /join | /j | /goto <room-address> [reason]
    Part,               // /part [<room-address>]
    Kick,               // /kick (same as RemoveUser)

    // Room config commands (isThreadCommand = false)
    RoomName,           // /roomname <name>
    Topic,              // /topic <topic>
    ChangeDisplayName,  // /nick <display-name>
    ChangeDisplayNameForRoom, // /myroomnick | /roomnick <display-name>
    RoomAvatar,         // /roomavatar <mxc_url> (dev command)
    ChangeAvatarForRoom,     // /myroomavatar <mxc_url> (dev command)

    // Settings commands (isThreadCommand = false)
    Markdown,           // /markdown <on|off>
    DiscardSession,     // /discardsession

    // Dev commands
    CrashApp,           // /crash (dev command)
    DevTools,           // /devtools
    ClearScalarToken,   // /clear_scalar_token

    // Space commands (dev commands)
    CreateSpace,        // /createspace <name> <invitee>*
    AddToSpace,         // /addToSpace spaceId
    JoinSpace,          // /joinSpace spaceId

    // Room management
    LeaveRoom,          // /leave <roomId?>
    UpgradeRoom,        // /upgraderoom newVersion

    // Fun effects
    Confetti,           // /confetti <message>
    Snowfall,           // /snowfall <message>

    // Navigation
    JumpToDate,         // /jumptodate <YYYY-MM-DD> [HH:MM] (Labs-gated)

    // AI Agent
    Agent,              // /agent <task> — LLM-powered chat automation
};

struct CommandDef {
    SlashCommandId id;
    std::string command;             // primary command e.g. "/me"
    std::vector<std::string> aliases; // e.g. "/j", "/goto" for JOIN_ROOM
    std::string parameters;          // e.g. "<message>" or "<user-id> [reason]"
    bool isDevCommand = false;       // only available in developer mode
    bool isThreadCommand = true;     // works in thread timeline
    bool isAdminCommand = false;     // requires elevated power level
};

// Get the full command table (matching Command.kt enum values).
const std::vector<CommandDef>& getCommandTable();

// Find a command by its primary name or alias.
// Original: Command.values().find { it.matches(input) }
const CommandDef* findCommand(const std::string& input);

// Check if input matches a command (case-insensitive).
// Original: fun matches(input: CharSequence) = allAliases.any { it.contentEquals(input, true) }
bool commandMatches(const CommandDef& cmd, const std::string& input);

// Check if input starts with a command (for autocomplete).
// Original: fun startsWith(input: CharSequence) = allAliases.any { it.startsWith(input, 1, true) }
bool commandStartsWith(const CommandDef& cmd, const std::string& input);

// ---- Parsed Command Result (from ParsedCommand.kt) ----

enum class ParseResultType {
    ErrorNotACommand,           // doesn't start with /
    ErrorEmptySlashCommand,     // just "/" with nothing after
    ErrorUnknownSlashCommand,   // unknown command
    ErrorSyntax,                // correct command but wrong parameters
    ErrorNotSupportedInThread,  // command doesn't work in thread timeline
    SendPlainText,              // /plain — send text without markdown
    SendFormattedText,          // /plain with HTML body
    SendEmote,                  // /me — action message
    SendRainbow,                // /rainbow — rainbow text
    SendRainbowEmote,           // /rainbowme — rainbow action
    SendSpoiler,                // /spoiler — ||spoiler||
    SendShrug,                  // /shrug — ¯\_(ツ)_/¯
    SendTableFlip,              // /tableflip
    SendLenny,                  // /lenny
    SendConfetti,               // /confetti
    SendSnowfall,               // /snowfall
    ChangeDisplayName,          // /nick
    ChangeDisplayNameForRoom,   // /myroomnick /roomnick
    ChangeRoomAvatar,           // /roomavatar
    ChangeAvatarForRoom,        // /myroomavatar
    ChangeTopic,                // /topic
    ChangeRoomName,             // /roomname
    JoinRoom,                   // /join /j /goto
    PartRoom,                   // /part
    InviteUser,                 // /invite
    RemoveUser,                 // /remove /kick
    BanUser,                    // /ban
    UnbanUser,                  // /unban
    IgnoreUser,                 // /ignore
    UnignoreUser,               // /unignore
    SetUserPowerLevel,          // /op
    ResetUserPowerLevel,        // /deop
    SetMarkdown,                // /markdown on|off
    DiscardSession,             // /discardsession
    ClearScalarToken,           // /clear_scalar_token
    ShowUser,                   // /whois
    CreateSpace,                // /createspace
    AddToSpace,                 // /addToSpace
    JoinSpace,                  // /joinSpace
    LeaveRoom,                  // /leave
    UpgradeRoom,                // /upgraderoom
    JumpToDate,                 // /jumptodate
    AgentTask,                  // /agent — LLM agent task
};

struct ParsedCommand {
    ParseResultType resultType = ParseResultType::ErrorNotACommand;
    SlashCommandId commandId;        // which command was detected
    std::string message;             // message body (for Send* types)
    std::string formattedMessage;    // HTML body (for SendFormattedText)
    std::string userId;              // target user ID (for Ban, Invite, etc.)
    std::string roomId;              // target room (for Join, Part, etc.)
    std::string reason;              // optional reason string
    int powerLevel = -1;             // power level (for SetUserPowerLevel)
    bool enableFlag = false;         // on/off flag (for SetMarkdown)
    std::string errorDetail;         // error description
};

// Parse a message into a slash command result.
// Faithful port of CommandParser.kt:parseSlashCommand()
ParsedCommand parseSlashCommand(const std::string& text, const std::string& formattedText, bool isInThread);

// Check if a message starts with a known slash command.
bool isSlashCommand(const std::string& text);

// Get all available command prefix strings for autocomplete.
std::vector<std::string> getSlashCommandSuggestions(const std::string& prefix);

// Format a parsed command result as JSON.
std::string parsedCommandToJson(const ParsedCommand& cmd);

// Get a human-readable description for a command.
std::string getCommandDescription(SlashCommandId id);

// Get the parameter hint for a command (e.g. "<user-id> [reason]").
std::string getCommandParameters(SlashCommandId id);

} // namespace progressive

#endif // PROGRESSIVE_SLASH_COMMAND_HPP
