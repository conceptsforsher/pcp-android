#ifndef PCP_IRC_TYPES_HPP
#define PCP_IRC_TYPES_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace pcp {
namespace irc {

enum class IrcCommand {
    PRIVMSG,
    NOTICE,
    JOIN,
    PART,
    QUIT,
    KICK,
    MODE,
    TOPIC,
    NICK,
    PING,
    PONG,
    RPL_WELCOME,
    RPL_TOPIC,
    RPL_NAMREPLY,
    RPL_ENDOFNAMES,
    RPL_MOTD,
    RPL_MOTDSTART,
    ERR_NICKNAMEINUSE,
    ERR_CHANOPRIVSNEEDED,
    UNKNOWN
};

struct IrcPrefix {
    std::string nick;
    std::string user;
    std::string host;
};

struct IrcMessage {
    IrcPrefix prefix;
    IrcCommand command;
    std::string rawCommand;
    std::vector<std::string> params;
    std::string trailing;
    int numericCode = 0;

    std::string toJson() const;
    bool isNumeric() const { return numericCode > 0; }
};

struct IrcChannel {
    std::string name;
    std::string topic;
    std::vector<std::string> users;
};

enum class IrcConnectionState {
    DISCONNECTED,
    CONNECTING,
    CONNECTED,
    REGISTERED,
    ERROR
};

struct IrcServerConfig {
    std::string host;
    int port = 6667;
    bool tls = false;
    std::string nick;
    std::string user;
    std::string realName;
    std::string password;
};

} // namespace irc
} // namespace pcp

#endif // PCP_IRC_TYPES_HPP
