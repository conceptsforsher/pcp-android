#ifndef PCP_TDLIB_PARSER_HPP
#define PCP_TDLIB_PARSER_HPP

#include "pcp/tdlib_types.hpp"
#include "pcp/tdlib_client.hpp"

namespace pcp {

class TdParser {
public:
    static TdUser parseUser(const TlObject& json);
    static TdChat parseChat(const TlObject& json);
    static TdMessage parseMessage(const TlObject& json);
    static TdStory parseStory(const TlObject& json);
    static TdSession parseSession(const TlObject& json);
    static TdProxy parseProxy(const TlObject& json);
    static TdNotificationSettings parseNotificationSettings(const TlObject& json);
    static TdError parseError(const TlObject& json);

    static TdConnectionState parseConnectionState(const std::string& type);
    static ContentType parseContentType(const TlObject& content);
};

} // namespace pcp
#endif
