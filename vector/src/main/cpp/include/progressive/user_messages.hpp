#ifndef PROGRESSIVE_USER_MESSAGES_HPP
#define PROGRESSIVE_USER_MESSAGES_HPP

#include <string>
#include <vector>

namespace progressive {

struct UserMessage {
    std::string eventId;
    std::string roomId;
    std::string roomName;
    std::string body;          // message body
    std::string formattedBody; // HTML if available
    std::string timestamp;     // ISO 8601
    int64_t originServerTs = 0;
    std::string msgType;       // m.text, m.image, etc.
    bool isEncrypted = false;
};

// Filter and format user messages for profile display.
// Returns top N messages sorted by recency.
std::vector<const UserMessage*> getLatestMessages(
    const std::vector<UserMessage>& allMessages,
    int limit = 20
);

// Format a batch of user messages as JSON for Kotlin consumption.
std::string userMessagesToJson(const std::vector<const UserMessage*>& messages);

// Format a single user message as preview text: "roomName: first 80 chars of body"
std::string formatUserMessagePreview(const UserMessage& msg, int maxLen = 80);

} // namespace progressive

#endif // PROGRESSIVE_USER_MESSAGES_HPP
