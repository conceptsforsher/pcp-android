#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace pcp {

struct ScheduledMessage {
    std::string id;
    std::string roomId;
    std::string body;
    std::string formattedBody;
    int64_t scheduledAtMs = 0;      // Unix millis when to send
    int64_t createdAtMs = 0;
    bool sent = false;
    bool cancelled = false;
};

class MessageScheduler {
public:
    MessageScheduler();

    // Schedule a message for future delivery.
    // Returns the scheduled message ID.
    std::string schedule(const std::string& roomId, const std::string& body,
                         const std::string& formattedBody, int64_t triggerAtMs);

    // Cancel a scheduled message.
    void cancel(const std::string& id);

    // Get all pending (not yet sent, not cancelled) messages.
    std::vector<ScheduledMessage> getPending() const;

    // Get all messages due for sending now (scheduledAtMs <= now).
    // Marks them as sent after returning.
    std::vector<ScheduledMessage> getDue();

    // Get all scheduled messages for a specific room.
    std::vector<ScheduledMessage> getForRoom(const std::string& roomId) const;

    // Serialize all messages to JSON for storage.
    std::string toJson() const;

    // Load messages from JSON.
    void fromJson(const std::string& json);

    // Clean up old sent/cancelled messages.
    void cleanup(int64_t olderThanMs = 0);

private:
    std::vector<ScheduledMessage> messages_;
    std::string generateId() const;
    int64_t nowMs() const;
};

} // namespace pcp
