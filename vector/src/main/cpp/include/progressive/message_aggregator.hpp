#ifndef PROGRESSIVE_MESSAGE_AGGREGATOR_HPP
#define PROGRESSIVE_MESSAGE_AGGREGATOR_HPP

#include <string>
#include <vector>

namespace progressive {

struct AggregatedMessage {
    std::string eventId;
    std::string roomId;
    std::string roomName;
    std::string accountId;
    std::string accountIndex;    // "1", "2", "3"
    std::string senderName;
    std::string body;
    std::string msgType;
    std::string timestamp;
    int64_t originServerTs = 0;
    bool isEncrypted = false;
};

class MessageAggregator {
public:
    void addMessage(const AggregatedMessage& msg);

    // Get all messages sorted by timestamp (most recent first)
    std::vector<AggregatedMessage> getAllMessages() const;

    void removeRoom(const std::string& roomId);
    void removeAccount(const std::string& accountId);
    void clear();

    size_t count() const { return messages_.size(); }

    std::string exportJson() const;

private:
    std::vector<AggregatedMessage> messages_;
};

} // namespace progressive

#endif // PROGRESSIVE_MESSAGE_AGGREGATOR_HPP
