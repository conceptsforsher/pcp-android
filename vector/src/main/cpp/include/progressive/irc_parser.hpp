#ifndef PROGRESSIVE_IRC_PARSER_HPP
#define PROGRESSIVE_IRC_PARSER_HPP

#include "irc_types.hpp"
#include <string>

namespace progressive {
namespace irc {

class IrcParser {
public:
    static IrcMessage parse(const std::string& raw);
    static IrcCommand parseCommand(const std::string& cmd, int* numericCode = nullptr);
    static std::string commandToString(IrcCommand cmd);

private:
    static void parsePrefix(const std::string& prefixStr, IrcPrefix& prefix);
};

} // namespace irc
} // namespace progressive

#endif // PROGRESSIVE_IRC_PARSER_HPP
