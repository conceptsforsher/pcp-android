#include "pcp/test_provider.hpp"
#include "pcp/string_utils.hpp"
#include <sstream>
#include <ctime>

namespace pcp {

TestProvider::TestProvider() { initTestData(); }

void TestProvider::initTestData() {
    rooms_ = {
        {"!test1:localhost", "General Chat", "Welcome to PCP — test mode", "", 5, 3, false,
         "Hey everyone! Welcome to the test room 👋", "2026-05-30T10:00:00Z"},
        {"!test2:localhost", "Encrypted Room 🔒", "This room is end-to-end encrypted", "", 3, 0, true,
         "This room is encrypted. Your messages are safe.", "2026-05-30T09:30:00Z"},
        {"!test3:localhost", "Bot Testing 🤖", "Test bots and integrations here", "", 4, 7, false,
         "!help — try bot commands here", "2026-05-30T11:00:00Z"},
        {"!test4:localhost", "Announcements 📢", "Important updates and news", "", 10, 0, false,
         "Server maintenance scheduled for tonight", "2026-05-29T18:00:00Z"},
        {"!test5:localhost", "Random / Off-Topic", "Anything goes. Share memes and fun stuff.", "", 8, 2, false,
         "Check out this cool project: https://pcp.chat", "2026-05-30T08:00:00Z"}
    };

    messages_ = {
        // Room 1
        {"$ev1", "!test1:localhost", "@alice:localhost", "Alice", "Hey everyone! Welcome to the test room 👋", "2026-05-30T10:00:00Z", 1748601600, false, false, "m.text"},
        {"$ev2", "!test1:localhost", "@bob:localhost", "Bob", "Hi Alice! This test mode is really useful for debugging.", "2026-05-30T10:01:00Z", 1748601660, false, false, "m.text"},
        {"$ev3", "!test1:localhost", "@carol:localhost", "Carol", "Agreed! I can test all the features without an account.", "2026-05-30T10:02:00Z", 1748601720, false, false, "m.text"},
        {"$ev4", "!test1:localhost", "@me:localhost", "You", "Testing message composition...", "2026-05-30T10:03:00Z", 1748601780, false, true, "m.text"},
        {"$ev5", "!test1:localhost", "@alice:localhost", "Alice", "Here is a longer message to test how the text rendering works with different lengths. It contains multiple sentences and should wrap properly in the message bubble. This helps test the layout system.", "2026-05-30T10:04:00Z", 1748601840, false, false, "m.text"},
        {"$ev6", "!test1:localhost", "@bob:localhost", "Bob", "Markdown test: **bold**, *italic*, `code`, ~~strikethrough~~", "2026-05-30T10:05:00Z", 1748601900, false, false, "m.text"},
        {"$ev7", "!test1:localhost", "@carol:localhost", "Carol", "Image test:", "2026-05-30T10:06:00Z", 1748601960, false, false, "m.image"},
        // Room 2 (encrypted)
        {"$ev8", "!test2:localhost", "@dave:localhost", "Dave", "Welcome to the encrypted room 🔒", "2026-05-30T09:30:00Z", 1748599800, true, false, "m.text"},
        {"$ev9", "!test2:localhost", "@eve:localhost", "Eve", "All messages here are end-to-end encrypted", "2026-05-30T09:31:00Z", 1748599860, true, false, "m.text"},
        {"$ev10", "!test2:localhost", "@me:localhost", "You", "Testing encrypted message sending", "2026-05-30T09:32:00Z", 1748599920, true, true, "m.text"},
        {"$ev11", "!test2:localhost", "@dave:localhost", "Dave", "The encryption shield shows green when verified", "2026-05-30T09:33:00Z", 1748599980, true, false, "m.text"},
        // Room 3
        {"$ev12", "!test3:localhost", "@frank:localhost", "Frank", "!help — try bot commands", "2026-05-30T11:00:00Z", 1748605200, false, false, "m.text"},
        {"$ev13", "!test3:localhost", "@bot:localhost", "TestBot", "Available commands: !help, !time, !weather, !flip, !roll", "2026-05-30T11:00:01Z", 1748605201, false, false, "m.text"},
        {"$ev14", "!test3:localhost", "@me:localhost", "You", "!time", "2026-05-30T11:01:00Z", 1748605260, false, true, "m.text"},
        {"$ev15", "!test3:localhost", "@bot:localhost", "TestBot", "Current time: 2026-05-30 11:01 UTC", "2026-05-30T11:01:01Z", 1748605261, false, false, "m.notice"},
        // Room 4
        {"$ev16", "!test4:localhost", "@admin:localhost", "Admin", "Server maintenance scheduled for tonight at 02:00 UTC", "2026-05-29T18:00:00Z", 1748548800, false, false, "m.text"},
        {"$ev17", "!test4:localhost", "@admin:localhost", "Admin", "Expected downtime: 15 minutes. All services will be back online by 02:15 UTC.", "2026-05-29T18:01:00Z", 1748548860, false, false, "m.text"},
        // Room 5
        {"$ev18", "!test5:localhost", "@grace:localhost", "Grace", "Check out this cool project: https://pcp.chat", "2026-05-30T08:00:00Z", 1748594400, false, false, "m.text"},
        {"$ev19", "!test5:localhost", "@heidi:localhost", "Heidi", "Nice! I've been using it for a while. The C++ native core is really fast.", "2026-05-30T08:05:00Z", 1748594700, false, false, "m.text"},
        {"$ev20", "!test5:localhost", "@ivan:localhost", "Ivan", "Has anyone tried the new encrypted search feature? It works with encrypted rooms!", "2026-05-30T08:10:00Z", 1748595000, false, false, "m.text"}
    };
}

std::string TestProvider::getRoomsJson() const {
    std::ostringstream out;
    out << "[";
    for (size_t i = 0; i < rooms_.size(); i++) {
        if (i > 0) out << ",";
        auto& r = rooms_[i];
        out << "{"
            << "\"roomId\":\"" << escapeJson(r.roomId) << "\","
            << "\"name\":\"" << escapeJson(r.name) << "\","
            << "\"topic\":\"" << escapeJson(r.topic) << "\","
            << "\"memberCount\":" << r.memberCount << ","
            << "\"unreadCount\":" << r.unreadCount << ","
            << "\"isEncrypted\":" << (r.isEncrypted ? "true" : "false") << ","
            << "\"lastMessage\":\"" << escapeJson(r.lastMessage) << "\","
            << "\"lastTimestamp\":\"" << escapeJson(r.lastTimestamp) << "\""
            << "}";
    }
    out << "]";
    return out.str();
}

std::string TestProvider::getMessagesJson(const std::string& roomId, int limit, int offset) const {
    std::ostringstream out;
    out << "[";
    int skipped = 0, added = 0;
    for (size_t i = 0; i < messages_.size(); i++) {
        auto& m = messages_[i];
        if (m.roomId != roomId) continue;
        if (skipped < offset) { skipped++; continue; }
        if (added >= limit) break;
        if (added > 0) out << ",";
        out << "{"
            << "\"eventId\":\"" << escapeJson(m.eventId) << "\","
            << "\"senderId\":\"" << escapeJson(m.senderId) << "\","
            << "\"senderName\":\"" << escapeJson(m.senderName) << "\","
            << "\"body\":\"" << escapeJson(m.body) << "\","
            << "\"timestamp\":\"" << escapeJson(m.timestamp) << "\","
            << "\"originServerTs\":" << m.originServerTs << ","
            << "\"isEncrypted\":" << (m.isEncrypted ? "true" : "false") << ","
            << "\"sentByMe\":" << (m.sentByMe ? "true" : "false") << ","
            << "\"msgType\":\"" << escapeJson(m.msgType) << "\""
            << "}";
        added++;
    }
    out << "]";
    return out.str();
}

std::string TestProvider::getRoomJson(const std::string& roomId) const {
    for (auto& r : rooms_) {
        if (r.roomId == roomId) {
            std::ostringstream out;
            out << "{"
                << "\"roomId\":\"" << escapeJson(r.roomId) << "\","
                << "\"name\":\"" << escapeJson(r.name) << "\","
                << "\"topic\":\"" << escapeJson(r.topic) << "\","
                << "\"memberCount\":" << r.memberCount
                << "}";
            return out.str();
        }
    }
    return "{}";
}

std::string TestProvider::searchMessages(const std::string& term, const std::string& roomId) const {
    std::ostringstream out;
    out << "[";
    bool first = true;
    for (auto& m : messages_) {
        if (!roomId.empty() && m.roomId != roomId) continue;
        if (m.body.find(term) == std::string::npos &&
            m.senderName.find(term) == std::string::npos) continue;
        if (!first) out << ",";
        first = false;
        out << "{"
            << "\"eventId\":\"" << escapeJson(m.eventId) << "\","
            << "\"roomId\":\"" << escapeJson(m.roomId) << "\","
            << "\"senderName\":\"" << escapeJson(m.senderName) << "\","
            << "\"body\":\"" << escapeJson(m.body) << "\""
            << "}";
    }
    out << "]";
    return out.str();
}

std::string TestProvider::getProfileJson() const {
    return "{"
           "\"userId\":\"@me:localhost\","
           "\"displayName\":\"You (Test Mode)\","
           "\"avatarUrl\":\"\""
           "}";
}

} // namespace pcp
