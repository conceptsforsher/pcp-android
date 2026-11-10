#ifndef PCP_TDLIB_NATIVE_CLIENT_HPP
#define PCP_TDLIB_NATIVE_CLIENT_HPP

#include "pcp/tdlib_client.hpp"
#include "pcp/tdlib_auth.hpp"
#include "pcp/tdlib_messages.hpp"
#include "pcp/tdlib_parser.hpp"
#include "pcp/tdlib_event_dispatcher.hpp"
#include "pcp/tdlib_cache.hpp"
#include "pcp/tdlib_connection.hpp"
#include "pcp/tdlib_file_manager.hpp"
#include "pcp/tdlib_types.hpp"
#include <memory>
#include <string>
#include <functional>
#include <atomic>

namespace pcp {

class TdNativeClient {
public:
    TdNativeClient(int apiId, const std::string& apiHash,
                   const std::string& dbDir, const std::string& filesDir);
    ~TdNativeClient();

    TdNativeClient(const TdNativeClient&) = delete;
    TdNativeClient& operator=(const TdNativeClient&) = delete;

    // Lifecycle
    bool initialize();
    void shutdown();
    bool isReady() const { return ready && auth->isReady(); }

    // Auth
    void sendPhone(const std::string& phone);
    void sendCode(const std::string& code);
    void sendPassword(const std::string& password);
    void logout();

    // Messaging
    void sendText(int64_t chatId, const std::string& text, int64_t threadId = 0, int64_t replyTo = 0);
    void sendPhoto(int64_t chatId, const std::string& path, const std::string& caption = "");
    void sendVoice(int64_t chatId, const std::string& path, int duration = 0);
    void sendSticker(int64_t chatId, int fileId);
    void sendDice(int64_t chatId, const std::string& emoji);
    void sendPoll(int64_t chatId, const std::string& question,
                  const std::vector<std::string>& options, bool anonymous = true);
    void sendLocation(int64_t chatId, double lat, double lon, int livePeriod = 0);
    void sendVideo(int64_t chatId, const std::string& path, const std::string& caption = "",
                   int duration = 0, int width = 0, int height = 0);
    void sendDocument(int64_t chatId, const std::string& path, const std::string& caption = "");
    void sendVideoNote(int64_t chatId, const std::string& path, int duration = 0, int length = 240);
    void sendAnimation(int64_t chatId, const std::string& path, const std::string& caption = "",
                       int duration = 0, int width = 0, int height = 0);
    void sendAudio(int64_t chatId, const std::string& path, const std::string& title = "",
                   const std::string& performer = "", int duration = 0);

    // Message operations
    void loadChats(int limit = 100);
    void getChatHistory(int64_t chatId, int limit = 50, int64_t fromId = 0);
    void deleteMessages(int64_t chatId, const std::vector<int64_t>& ids, bool revoke = true);
    void editMessage(int64_t chatId, int64_t msgId, const std::string& newText);
    void forwardMessages(int64_t toChatId, int64_t fromChatId,
                        const std::vector<int64_t>& msgIds, bool sendCopy = false);
    void setReaction(int64_t chatId, int64_t msgId, const std::string& reaction);
    void pinMessage(int64_t chatId, int64_t msgId, bool disableNotification = false);
    void unpinMessage(int64_t chatId, int64_t msgId = 0);
    void markAsRead(int64_t chatId, const std::vector<int64_t>& msgIds);
    void getScheduledMessages(int64_t chatId);
    void sendScheduled(int64_t chatId, const std::string& text, int scheduleDate);
    void sendTextWithTtl(int64_t chatId, const std::string& text, int ttlSeconds);
    void sendTextSilent(int64_t chatId, const std::string& text, int64_t threadId = 0);
    void setChatDraftMessage(int64_t chatId, int64_t threadId, const std::string& text);
    void clearChatDraftMessage(int64_t chatId, int64_t threadId = 0);

    // Chats
    void setChatTitle(int64_t chatId, const std::string& title);
    void openChat(int64_t chatId);
    void createGroup(const std::string& title);
    void createChannel(const std::string& title);
    void leaveChat(int64_t chatId);

    // Threads & Forums
    void getMessageThread(int64_t chatId, int64_t msgId);
    void getThreadHistory(int64_t chatId, int64_t msgId, int64_t fromId = 0, int limit = 50);
    void createForumTopic(int64_t chatId, const std::string& name, int iconColor = 0x6FB9F0);
    void deleteForumTopic(int64_t chatId, int64_t topicId);
    void getForumTopics(int64_t chatId, const std::string& query = "", int limit = 100);

    // Stories
    void getActiveStories(int limit = 100);

    // Profile & Settings
    void getMe();
    void getUser(int64_t userId);
    void setBio(const std::string& bio);
    void setName(const std::string& first, const std::string& last);
    void setUsername(const std::string& username);
    void setProfilePhoto(const std::string& path);
    void setEmojiStatus(int64_t customEmojiId, int duration = 0);
    void clearEmojiStatus();

    // Security & Privacy
    void getActiveSessions();
    void terminateSession(int64_t sessionId);
    void terminateAllOtherSessions();
    void blockUser(int64_t userId);
    void unblockUser(int64_t userId);
    void getBlockedUsers(int offset = 0, int limit = 100);

    // Files
    void downloadFile(int fileId, int priority = 1);
    void cancelDownload(int fileId);
    void getFileInfo(int fileId);
    void deleteFile(int fileId);

    // Queries
    std::vector<TdChat> getCachedChats() { return cache->getAllChats(); }
    std::vector<TdMessage> getCachedMessages(int64_t chatId, int limit = 50, int64_t fromId = 0);
    TdConnectionState getConnectionState() { return connection->currentState(); }

    // Event dispatcher
    TdEventDispatcher& events() { return *dispatcher; }
    TdConnectionManager& connMgr() { return *connection; }
    TdFileManager& fileMgr() { return *fileManager; }

private:
    void setupCallbacks();

    std::unique_ptr<TdLibClient> client;
    std::unique_ptr<TdLibAuth> auth;
    std::unique_ptr<TdLibMessages> messages;
    std::unique_ptr<TdParser> parser;
    std::unique_ptr<TdEventDispatcher> dispatcher;
    std::unique_ptr<TdCache> cache;
    std::unique_ptr<TdConnectionManager> connection;
    std::unique_ptr<TdFileManager> fileManager;

    std::atomic<bool> ready{false};
    int apiId;
    std::string apiHash;
    std::string dbDir;
    std::string filesDir;
};

} // namespace pcp
#endif
