#ifndef PCP_TEST_PROVIDER_HPP
#define PCP_TEST_PROVIDER_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace pcp {

struct TestRoom {
    std::string roomId;
    std::string name;
    std::string topic;
    std::string avatarUrl;
    int memberCount;
    int unreadCount;
    bool isEncrypted;
    std::string lastMessage;
    std::string lastTimestamp;
};

struct TestMessage {
    std::string eventId;
    std::string roomId;
    std::string senderId;
    std::string senderName;
    std::string body;
    std::string timestamp;
    int64_t originServerTs;
    bool isEncrypted;
    bool sentByMe;
    std::string msgType; // m.text, m.image, m.file
};

// Provides pre-built test data for offline testing without an account.
class TestProvider {
public:
    TestProvider();

    // Get all test rooms
    std::string getRoomsJson() const;

    // Get messages for a room
    std::string getMessagesJson(const std::string& roomId, int limit = 20, int offset = 0) const;

    // Get a specific room
    std::string getRoomJson(const std::string& roomId) const;

    // Search messages in test rooms
    std::string searchMessages(const std::string& term, const std::string& roomId = "") const;

    // Get test user profile
    std::string getProfileJson() const;

private:
    std::vector<TestRoom> rooms_;
    std::vector<TestMessage> messages_;
    void initTestData();
};

} // namespace pcp

#endif
