#ifndef PCP_TDLIB_CACHE_HPP
#define PCP_TDLIB_CACHE_HPP

#include "pcp/tdlib_types.hpp"
#include <string>
#include <vector>
#include <mutex>

namespace pcp {

class TdCache {
public:
    TdCache(const std::string& dbPath);
    ~TdCache();

    bool open();
    void close();

    // Messages
    void insertMessage(const TdMessage& msg);
    std::vector<TdMessage> getMessages(int64_t chatId, int limit = 50, int64_t fromId = 0);
    void deleteMessage(int64_t chatId, int64_t msgId);
    void deleteMessages(int64_t chatId, const std::vector<int64_t>& ids);
    int messageCount(int64_t chatId);

    // Chats
    void upsertChat(const TdChat& chat);
    std::vector<TdChat> getAllChats();
    TdChat getChat(int64_t chatId);

    // Users
    void upsertUser(const TdUser& user);
    TdUser getUser(int64_t userId);

    // Stories
    void insertStory(const TdStory& story);
    std::vector<TdStory> getActiveStories();

private:
    void createTables();
    void execute(const std::string& sql);
    std::string dbPath;
    void* db = nullptr; // sqlite3*
    std::mutex mtx;
};

} // namespace pcp
#endif
