#ifndef PCP_IRC_PARSER_HPP
#define PCP_IRC_PARSER_HPP

#include "irc_types.hpp"
#include <string>

namespace pcp {
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
} // namespace pcp

#endif // PCP_IRC_PARSER_HPP
