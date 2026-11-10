#pragma once
#include <string>
#include <vector>
#include <cstdint>

namespace pcp {

struct RoomSummaryDisplay {
    std::string roomId;
    std::string displayName;
    std::string lastMessage;        // preview text
    std::string lastMessageSender;  // who sent it
    std::string lastMessagePrefix;  // "You: " or sender name
    int64_t lastMessageTs = 0;
    std::string formattedTime;      // "12:34", "Yesterday"
    int notificationCount = 0;
    int highlightCount = 0;
    bool isDirect = false;
    bool isMuted = false;
    bool hasDraft = false;
    bool isEncrypted = false;
    bool isFavourite = false;
    std::string avatarUrl;
    std::string membership;         // "join", "invite"
    std::string typingText;         // "Alice is typing..."
};

// Format room summary for room list display
RoomSummaryDisplay formatRoomSummary(const std::string& roomId, const std::string& displayName,
                                       const std::string& lastMessageBody, const std::string& lastSenderName,
                                       int64_t lastMsgTs, int notifCount, int highlightCount,
                                       bool isDirect, const std::string& typingUsers = "");

// Format notification badge text ("5", "99+", "")
std::string formatNotificationBadge(int count, int highlightCount);

// Format last message preview (truncated + prefix)
std::string formatLastMessagePreview(const std::string& body, const std::string& senderName,
                                       bool isOwnMessage);

// Format typing indicator for room list
std::string formatRoomListTyping(const std::vector<std::string>& typingUserIds,
                                   const std::vector<std::string>& typingUserNames,
                                   int maxNames = 2);

// Format room name with encryption icon
std::string formatRoomName(const std::string& name, bool isEncrypted, bool isDirect);

// Get room list sort key (for ordering)
std::string buildRoomSortKey(int64_t lastActivityMs, bool isFavourite, bool hasHighlight);

} // namespace pcp
