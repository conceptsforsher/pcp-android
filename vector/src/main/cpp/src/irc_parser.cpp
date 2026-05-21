#include "progressive/irc_parser.hpp"
#include <cstring>
#include <cstdlib>
#include <algorithm>

namespace progressive {
namespace irc {

IrcMessage IrcParser::parse(const std::string& raw) {
    IrcMessage msg;
    std::string line = raw;

    while (!line.empty() && (line.back() == '\r' || line.back() == '\n')) {
        line.pop_back();
    }

    if (line.empty()) return msg;

    size_t pos = 0;

    if (line[0] == ':') {
        size_t spacePos = line.find(' ');
        if (spacePos != std::string::npos) {
            parsePrefix(line.substr(1, spacePos - 1), msg.prefix);
            pos = spacePos + 1;
        }
    }

    while (pos < line.length() && line[pos] == ' ') pos++;

    size_t cmdEnd = line.find(' ', pos);
    std::string cmdStr;
    if (cmdEnd != std::string::npos) {
        cmdStr = line.substr(pos, cmdEnd - pos);
        pos = cmdEnd + 1;
    } else {
        cmdStr = line.substr(pos);
        pos = line.length();
    }

    msg.rawCommand = cmdStr;
    msg.command = parseCommand(cmdStr, &msg.numericCode);

    while (pos < line.length()) {
        while (pos < line.length() && line[pos] == ' ') pos++;
        if (pos >= line.length()) break;

        if (line[pos] == ':') {
            msg.trailing = line.substr(pos + 1);
            break;
        }

        size_t spacePos = line.find(' ', pos);
        if (spacePos != std::string::npos) {
            msg.params.push_back(line.substr(pos, spacePos - pos));
            pos = spacePos + 1;
        } else {
            msg.params.push_back(line.substr(pos));
            break;
        }
    }

    return msg;
}

IrcCommand IrcParser::parseCommand(const std::string& cmd, int* numericCode) {
    static const char* cmdTable[] = {
        "PRIVMSG", "NOTICE", "JOIN", "PART", "QUIT", "KICK",
        "MODE", "TOPIC", "NICK", "PING", "PONG",
        "001", "332", "353", "366", "372", "375",
        "433", "482", nullptr
    };
    static const IrcCommand cmdMap[] = {
        IrcCommand::PRIVMSG, IrcCommand::NOTICE, IrcCommand::JOIN,
        IrcCommand::PART, IrcCommand::QUIT, IrcCommand::KICK,
        IrcCommand::MODE, IrcCommand::TOPIC, IrcCommand::NICK,
        IrcCommand::PING, IrcCommand::PONG,
        IrcCommand::RPL_WELCOME, IrcCommand::RPL_TOPIC,
        IrcCommand::RPL_NAMREPLY, IrcCommand::RPL_ENDOFNAMES,
        IrcCommand::RPL_MOTD, IrcCommand::RPL_MOTDSTART,
        IrcCommand::ERR_NICKNAMEINUSE, IrcCommand::ERR_CHANOPRIVSNEEDED,
    };

    std::string upper = cmd;
    std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);

    for (int i = 0; cmdTable[i] != nullptr; ++i) {
        if (upper == cmdTable[i]) {
            return cmdMap[i];
        }
    }

    if (std::all_of(cmd.begin(), cmd.end(), ::isdigit)) {
        if (numericCode) *numericCode = std::atoi(cmd.c_str());
    }

    return IrcCommand::UNKNOWN;
}

void IrcParser::parsePrefix(const std::string& prefixStr, IrcPrefix& prefix) {
    size_t excl = prefixStr.find('!');
    size_t at = prefixStr.find('@');

    if (excl != std::string::npos) {
        prefix.nick = prefixStr.substr(0, excl);
        if (at != std::string::npos && at > excl) {
            prefix.user = prefixStr.substr(excl + 1, at - excl - 1);
            prefix.host = prefixStr.substr(at + 1);
        } else {
            prefix.user = prefixStr.substr(excl + 1);
        }
    } else if (at != std::string::npos) {
        prefix.nick = prefixStr.substr(0, at);
        prefix.host = prefixStr.substr(at + 1);
    } else {
        prefix.nick = prefixStr;
    }
}

std::string IrcParser::commandToString(IrcCommand cmd) {
    switch (cmd) {
        case IrcCommand::PRIVMSG: return "PRIVMSG";
        case IrcCommand::NOTICE: return "NOTICE";
        case IrcCommand::JOIN: return "JOIN";
        case IrcCommand::PART: return "PART";
        case IrcCommand::QUIT: return "QUIT";
        case IrcCommand::KICK: return "KICK";
        case IrcCommand::MODE: return "MODE";
        case IrcCommand::TOPIC: return "TOPIC";
        case IrcCommand::NICK: return "NICK";
        case IrcCommand::PING: return "PING";
        case IrcCommand::PONG: return "PONG";
        default: return "";
    }
}

} // namespace irc
} // namespace progressive
