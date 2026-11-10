#pragma once
#include <string>
#include <vector>

namespace pcp {

struct ServerNoticeContent {
    std::string noticeType;     // "m.server_notice"
    std::string adminContact;   // admin contact URI
    std::string limitType;      // "monthly_active_user" etc.
    std::string message;
};

// Parse server notice from event content
ServerNoticeContent parseServerNotice(const std::string& json);

// Check if event is a server notice
bool isServerNoticeEvent(const std::string& json);

// Format server notice for display
std::string formatServerNotice(const ServerNoticeContent& notice);

// Build server notice room tag
std::string buildServerNoticeTag();

} // namespace pcp
