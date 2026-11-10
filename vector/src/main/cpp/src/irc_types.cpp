#include "pcp/irc_types.hpp"
#include <sstream>
#include <cstring>

namespace pcp {
namespace irc {

std::string IrcMessage::toJson() const {
    std::ostringstream ss;
    ss << "{";
    ss << "\"cmd\":\"" << rawCommand << "\",";
    ss << "\"code\":" << numericCode << ",";
    if (!prefix.nick.empty()) {
        ss << "\"nick\":\"" << prefix.nick << "\",";
        ss << "\"user\":\"" << prefix.user << "\",";
        ss << "\"host\":\"" << prefix.host << "\",";
    }
    ss << "\"params\":[";
    for (size_t i = 0; i < params.size(); ++i) {
        if (i > 0) ss << ",";
        ss << "\"" << params[i] << "\"";
    }
    ss << "],";
    ss << "\"trailing\":\"" << trailing << "\"";
    ss << "}";
    return ss.str();
}

} // namespace irc
} // namespace pcp
