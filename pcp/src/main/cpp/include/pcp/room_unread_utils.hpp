#pragma once
#include <string>
#include <cstdint>
namespace pcp {
std::string buildUnreadMarker(const std::string& eventId, int unreadCount);
bool hasUnreadMessages(const std::string& roomId, int notifCount);
std::string formatUnreadBadge(int count, int highlight);
}
