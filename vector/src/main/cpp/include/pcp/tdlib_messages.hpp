#ifndef PCP_TDLIB_MESSAGES_HPP
#define PCP_TDLIB_MESSAGES_HPP

#include "pcp/tdlib_client.hpp"
#include <functional>
#include <string>
#include <vector>

namespace pcp {

struct ChatInfo {
    long long id = 0;
    std::string title;
    std::string type;
    long long lastMessageId = 0;
    std::string lastMessageText;
    long long lastMessageTime = 0;
    int unreadCount = 0;
    bool isGroup = false;
    bool isChannel = false;
    bool isPinned = false;
    bool isMarkedUnread = false;
};

struct MessageInfo {
    long long id = 0;
    long long chatId = 0;
    std::string senderId;
    std::string senderName;
    int date = 0;
    bool isOutgoing = false;
    std::string contentType;
    std::string text;
    std::string rawJson;
};

struct UserStatusInfo {
    long long userId = 0;
    std::string type;  // userStatusOnline/Offline/Recently/LastWeek/LastMonth/Empty
    int wasOnline = 0;  // unix timestamp for Offline
    bool isOnline = false;
    bool isPremium = false;
    std::string emojiStatus;
};

using ChatListCallback = std::function<void(const std::vector<ChatInfo>& chats)>;
using MessageCallback = std::function<void(const MessageInfo& msg)>;
using MessagesCallback = std::function<void(const std::vector<MessageInfo>& msgs, long long totalCount)>;

class TdLibMessages {
public:
    explicit TdLibMessages(TdLibClient& client);
    ~TdLibMessages();

    void sendText(long long chatId, const std::string& text, long long threadId = 0,
                  long long replyTo = 0);
    void sendPhoto(long long chatId, const std::string& path, const std::string& caption = "",
                   long long threadId = 0);
    void sendVideo(long long chatId, const std::string& path, const std::string& caption = "",
                   int duration = 0, int width = 0, int height = 0, long long threadId = 0);
    void sendVoice(long long chatId, const std::string& path, int duration = 0);
    void sendDocument(long long chatId, const std::string& path, const std::string& caption = "");
    void sendAnimation(long long chatId, const std::string& path, const std::string& caption = "",
                       int duration = 0, int width = 0, int height = 0);
    void sendVideoNote(long long chatId, const std::string& path, int duration = 0, int length = 0);
    void sendMessageAlbum(long long chatId, const std::vector<std::string>& filePaths,
                          const std::vector<std::string>& captions, bool isVideo = true);
    void sendAudio(long long chatId, const std::string& path, const std::string& title = "",
                   const std::string& performer = "", int duration = 0);
    void sendSticker(long long chatId, int stickerFileId);
    void sendDice(long long chatId, const std::string& emoji = "\xF0\x9F\x8E\xB2");
    void sendLocation(long long chatId, double lat, double lon, int livePeriod = 0);
    void sendPoll(long long chatId, const std::string& question,
                  const std::vector<std::string>& options, bool anonymous = true,
                  bool quiz = false, int correctOption = -1);
    void sendContact(long long chatId, const std::string& phone, const std::string& firstName,
                     const std::string& lastName = "");

    void getChatHistory(long long chatId, int limit = 50, long long fromMessageId = 0);
    void loadChats(int limit = 100);
    void openChat(long long chatId);
    void closeChat(long long chatId);
    void deleteMessages(long long chatId, const std::vector<long long>& msgIds, bool revoke = true);
    void editMessage(long long chatId, long long msgId, const std::string& newText);
    void viewMessages(long long chatId, const std::vector<long long>& msgIds);
    void searchMessages(long long chatId, const std::string& query, int limit = 50);

    void forwardMessages(long long toChatId, long long fromChatId,
                        const std::vector<long long>& msgIds, bool sendCopy = false);

    void setMessageReaction(long long chatId, long long msgId, const std::string& reaction);
    void removeMessageReaction(long long chatId, long long msgId, const std::string& reaction);

    void setPollVote(long long chatId, long long msgId, const std::vector<int>& optionIds);
    void stopPoll(long long chatId, long long msgId);

    void pinMessage(long long chatId, long long msgId, bool disableNotification = false);
    void unpinMessage(long long chatId, long long msgId = 0);
    void unpinAllMessages(long long chatId);

    void getScheduledMessages(long long chatId);
    void sendScheduled(long long chatId, const std::string& text, int scheduleDate);

    void getChatDraftMessage(long long chatId, long long threadId = 0);
    void setChatDraftMessage(long long chatId, long long threadId, const std::string& text);
    void clearChatDraftMessage(long long chatId, long long threadId = 0);

    void sendTextWithTtl(long long chatId, const std::string& text, int ttlSeconds);
    void sendTextSilent(long long chatId, const std::string& text, long long threadId = 0);

    void importContacts(const std::vector<std::pair<std::string, std::string>>& contacts);
    void searchContacts(const std::string& query = "", int limit = 100);
    void removeContacts(const std::vector<long long>& userIds);

    void getNotificationSettings();
    void setNotificationSettings(int muteFor, bool showPreview, long long soundId = 0);

    void downloadFile(int fileId, int priority = 1, long long offset = 0, long long limit = 0, bool synchronous = false);
    void cancelDownloadFile(int fileId);
    void getFile(int fileId);
    void deleteFile(int fileId);

    void getMessageThread(long long chatId, long long msgId);
    void getMessageThreadHistory(long long chatId, long long msgId, long long fromMsgId = 0, int limit = 50);
    void sendThreadReply(long long chatId, long long threadId, const std::string& text, long long replyTo = 0);

    void getForumTopics(long long chatId, const std::string& query = "", int limit = 100);
    void getForumTopic(long long chatId, long long topicId);
    void createForumTopic(long long chatId, const std::string& name, int iconColor = 0x6FB9F0);
    void deleteForumTopic(long long chatId, long long topicId);
    void toggleForumTopicClosed(long long chatId, long long topicId, bool closed);
    void toggleForumTopicPinned(long long chatId, long long topicId, bool pinned);

    void getStickerSet(long long setId);
    void searchStickerSet(const std::string& name);
    void getInstalledStickerSets();
    void getRecentStickers();
    void searchStickers(const std::string& emoji, int limit = 20);

    void getActiveStories(int limit = 100);
    void getStory(long long chatId, long long storyId);
    void openStory(long long chatId, long long storyId);
    void closeStory(long long chatId, long long storyId);
    void setStoryReaction(long long chatId, long long storyId, const std::string& reaction);
    void deleteStory(long long chatId, long long storyId);
    void sendStory(const std::string& filePath, bool isVideo, const std::string& caption = "");

    void getUser(long long userId);
    void getUserFullInfo(long long userId);
    void getMe();
    void setBio(const std::string& bio);
    void setName(const std::string& firstName, const std::string& lastName);
    void setUsername(const std::string& username);
    void setProfilePhoto(const std::string& photoPath);
    void getUserProfilePhotos(long long userId, int offset = 0, int limit = 100);
    void getActiveSessions();
    void terminateSession(long long sessionId);
    void terminateAllOtherSessions();
    void getConnectedWebsites();
    void getGroupsInCommon(long long userId, long long offsetChatId = 0);
    void blockUser(long long userId);
    void unblockUser(long long userId);
    void getBlockedUsers(int offset = 0, int limit = 100);
    void getPremiumState();

    void setEmojiStatus(long long customEmojiId, int duration = 0);
    void clearEmojiStatus();
    void getDefaultEmojiStatuses();
    void getThemedEmojiStatuses();
    void getRecentEmojiStatuses();
    void setCloseFriends(const std::vector<long long>& userIds);
    void getCloseFriends();

    void createNewSecretChat(long long userId);
    void openSecretChat(long long secretChatId);
    void closeSecretChat(long long secretChatId);
    void getSecretChat(long long secretChatId);
    void sendChatSetTtlMessage(long long chatId, int ttl);
    void setSecretChatTtl(long long secretChatId, int ttl);

    void getBackgrounds(bool dark = false);
    void getChatBackground(long long chatId);
    void setChatBackground(long long chatId, long long bgId);
    void deleteChatBackground(long long chatId);

    void setChatFolder(int folderId, const std::string& title, const std::vector<long long>& pinnedIds,
                       const std::vector<long long>& includedIds, const std::vector<long long>& excludedIds);
    void getChatFolders();
    void deleteChatFolder(int folderId);

    void getChatMember(long long chatId, long long userId);
    void searchChatMembers(long long chatId, const std::string& query, int limit = 50);
    void getChatAdministrators(long long chatId);
    void addChatMember(long long chatId, long long userId, int forwardLimit = 100);
    void joinChatByInviteLink(const std::string& inviteLink);
    void checkChatInviteLink(const std::string& inviteLink);
    void getChatInviteLink(long long chatId, bool createsJoinRequest = true);
    void searchPublicChat(const std::string& username);
    void searchPublicChats(const std::string& query);
    void createNewBasicGroupChat(const std::vector<long long>& userIds, const std::string& title = "");
    void createNewSupergroupChat(const std::string& title, bool isChannel = false, const std::string& description = "");
    void setChatMemberStatus(long long chatId, long long userId, const std::string& status);
    void reportChat(long long chatId, const std::vector<long long>& messageIds, const std::string& reason = "chatReportReasonSpam");

    void getBotCommands(long long botUserId, long long scopeChatId);
    void getInlineQueryResults(long long botUserId, long long chatId, const std::string& query, const std::string& offset = "");
    void sendInlineQueryResult(long long chatId, long long queryId, const std::string& resultId, bool hideViaBot = false);
    void answerCallbackQuery(long long callbackQueryId, const std::string& text = "", bool showAlert = false);
    void toggleBotIsPaused(long long botUserId, bool isPaused);
    void canBotSendMessages(long long botUserId);

    void updateChatList() { loadChats(); }

    // Calls
    void createCall(long long userId, bool isVideo);
    void acceptCall(int callId);
    void discardCall(int callId, bool isDisconnected);

    // Live Location
    void editLiveLocation(long long chatId, long long msgId, double lat, double lon,
                          int heading = 0, int proximityAlertRadius = 0);
    void stopLiveLocation(long long chatId, long long msgId);

    // Story Privacy & Stealth
    void setStoryPrivacy(bool everyone, bool contacts, bool closeFriends,
                         const std::vector<long long>& selectedUserIds);
    void getStoryPrivacy();
    void activateStealthMode();
    void getChatsToSendStories();
    void canSendStoryToChat(long long chatId);
    void getStoryNotificationExceptions();
    void setStoryNotificationExceptions(const std::vector<long long>& userIds, bool enabled);

    void addProxy(const std::string& server, int port, bool enable, const std::string& proxyType,
                  const std::string& username = "", const std::string& password = "", const std::string& secret = "");
    void editProxy(int proxyId, const std::string& server, int port, bool enable,
                   const std::string& proxyType);
    void enableProxy(int proxyId);
    void disableProxy();
    void removeProxy(int proxyId);
    void getProxies();
    void pingProxy(int proxyId);
    void testProxy(const std::string& server, int port, const std::string& proxyType,
                   int dcId = 2, double timeout = 10.0);

    TlObject makeFileLocal(const std::string& path);
    TlObject makeFormattedText(const std::string& text);

    static MessageInfo parseMessageInfo(const TlObject& msg);
    static UserStatusInfo parseUserStatus(const TlObject& user);

private:
    TdLibClient& client;
};

} // namespace pcp
#endif
