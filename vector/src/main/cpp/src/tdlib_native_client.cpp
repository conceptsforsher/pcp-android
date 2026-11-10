#include "pcp/tdlib_native_client.hpp"
#include <android/log.h>

#define TAG "TdNativeClient"

namespace pcp {

TdNativeClient::TdNativeClient(int apiId_, const std::string& apiHash_,
                               const std::string& dbDir_, const std::string& filesDir_)
    : apiId(apiId_), apiHash(apiHash_), dbDir(dbDir_), filesDir(filesDir_) {

    client = std::make_unique<TdLibClient>(apiId, apiHash, dbDir, filesDir);
    auth = std::make_unique<TdLibAuth>(*client);
    messages = std::make_unique<TdLibMessages>(*client);
    parser = std::make_unique<TdParser>();
    dispatcher = std::make_unique<TdEventDispatcher>();
    cache = std::make_unique<TdCache>(dbDir + "/td_cache.db");
    connection = std::make_unique<TdConnectionManager>();
    fileManager = std::make_unique<TdFileManager>();
}

TdNativeClient::~TdNativeClient() {
    shutdown();
}

bool TdNativeClient::initialize() {
    if (!cache->open()) {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "Failed to open cache database");
    }

    setupCallbacks();
    client->initialize();
    return true;
}

void TdNativeClient::shutdown() {
    ready = false;
    client->close();
    cache->close();
}

void TdNativeClient::setupCallbacks() {
    // Auth callback
    client->setAuthCallback([this](const std::string& type, const TlObject& state) {
        auth->handleAuthState(type, state);
        dispatcher->onAuthChanged(type, state.toJson());

        if (type == "authorizationStateReady") {
            ready = true;
            messages->loadChats(100);
        }
    });

    // Connection callback
    client->setConnectionCallback([this](const std::string& state) {
        auto cs = TdParser::parseConnectionState(state);
        connection->onStateChanged(cs);
        dispatcher->onConnectionChanged(cs);
    });

    // Update callback — parse and dispatch typed events
    client->setUpdateCallback([this](const std::string& type, const TlObject& data) {
        if (type == "updateNewMessage") {
            TlObject msgObj = data.optObject("message");
            if (!msgObj.type.empty()) {
                TdMessage msg = parser->parseMessage(msgObj);
                cache->insertMessage(msg);
                dispatcher->onNewMessage(msg);
            }
        } else if (type == "updateNewChat") {
            TlObject chatObj = data.optObject("chat");
            if (!chatObj.type.empty()) {
                TdChat chat = parser->parseChat(chatObj);
                cache->upsertChat(chat);
                dispatcher->onChatUpdated(chat);
            }
        } else if (type == "updateChatLastMessage" || type == "updateChatTitle") {
            int64_t chatId = data.optLong("chat_id", 0);
            if (chatId > 0) dispatcher->onChatListChanged();
        } else if (type == "updateDeleteMessages") {
            int64_t chatId = data.optLong("chat_id", 0);
            std::vector<int64_t> ids;
            auto idArr = data.optArray("message_ids");
            for (auto& i : idArr) ids.push_back(i.optLong("", 0));
            cache->deleteMessages(chatId, ids);
            dispatcher->onMessageDeleted(chatId, ids);
        } else if (type == "updateUserStatus") {
            int64_t uid = data.optLong("user_id", 0);
            TlObject status = data.optObject("status");
            UserStatus us = UserStatus::EMPTY;
            if (status.type == "userStatusOnline") us = UserStatus::ONLINE;
            else if (status.type == "userStatusRecently") us = UserStatus::RECENTLY;
            else if (status.type == "userStatusLastWeek") us = UserStatus::LAST_WEEK;
            else if (status.type == "userStatusLastMonth") us = UserStatus::LAST_MONTH;
            else if (status.type == "userStatusOffline") us = UserStatus::OFFLINE;
            dispatcher->onUserStatus(uid, us);
        } else if (type == "updateFile") {
            TlObject file = data.optObject("file");
            int fileId = file.optInt("id", 0);
            int64_t size = file.optLong("size", 0);
            TlObject local = file.optObject("local");
            int64_t downloaded = local.optLong("downloaded_size", 0);
            bool completed = local.optBool("is_downloading_completed", false);
            fileManager->onFileUpdate(fileId, size, downloaded, completed);
            dispatcher->onFileProgress(fileId, size, downloaded, completed);
        } else if (type == "updateActiveStories") {
            // Parse and dispatch
        }
    });

    // Response callback — for direct API responses (messages list, chats list, etc.)
    client->setResponseCallback([this](const std::string& type, const TlObject& data) {
        if (type == "messages") {
            auto msgArr = data.optArray("messages");
            for (auto& mObj : msgArr) {
                TdMessage msg = parser->parseMessage(mObj);
                cache->insertMessage(msg);
            }
        } else if (type == "chats") {
            // Chat list loaded
            dispatcher->onChatListChanged();
        } else if (type == "user") {
            TdUser user = parser->parseUser(data);
            cache->upsertUser(user);
        }
    });
}

// --- Auth ---
void TdNativeClient::sendPhone(const std::string& phone) { auth->sendPhone(phone); }
void TdNativeClient::sendCode(const std::string& code) { auth->sendCode(code); }
void TdNativeClient::sendPassword(const std::string& pw) { auth->sendPassword(pw); }
void TdNativeClient::logout() { auth->logout(); }

// --- Messaging ---
void TdNativeClient::sendText(int64_t chatId, const std::string& text, int64_t threadId, int64_t replyTo) {
    messages->sendText(chatId, text, threadId, replyTo);
}
void TdNativeClient::sendPhoto(int64_t chatId, const std::string& path, const std::string& caption) {
    messages->sendPhoto(chatId, path, caption);
}
void TdNativeClient::sendVoice(int64_t chatId, const std::string& path, int duration) {
    messages->sendVoice(chatId, path, duration);
}
void TdNativeClient::sendSticker(int64_t chatId, int fileId) {
    messages->sendSticker(chatId, fileId);
}
void TdNativeClient::sendDice(int64_t chatId, const std::string& emoji) {
    messages->sendDice(chatId, emoji);
}
void TdNativeClient::sendPoll(int64_t chatId, const std::string& question,
                              const std::vector<std::string>& options, bool anonymous) {
    messages->sendPoll(chatId, question, options, anonymous);
}
void TdNativeClient::sendLocation(int64_t chatId, double lat, double lon, int livePeriod) {
    messages->sendLocation(chatId, lat, lon, livePeriod);
}
void TdNativeClient::sendVideo(int64_t chatId, const std::string& path, const std::string& caption,
                               int duration, int width, int height) {
    messages->sendVideo(chatId, path, caption, duration, width, height);
}
void TdNativeClient::sendDocument(int64_t chatId, const std::string& path, const std::string& caption) {
    messages->sendDocument(chatId, path, caption);
}
void TdNativeClient::sendVideoNote(int64_t chatId, const std::string& path, int duration, int length) {
    messages->sendVideoNote(chatId, path, duration, length);
}
void TdNativeClient::sendAnimation(int64_t chatId, const std::string& path, const std::string& caption,
                                   int duration, int width, int height) {
    messages->sendAnimation(chatId, path, caption, duration, width, height);
}
void TdNativeClient::sendAudio(int64_t chatId, const std::string& path, const std::string& title,
                               const std::string& performer, int duration) {
    messages->sendAudio(chatId, path, title, performer, duration);
}

// --- Message Operations ---
void TdNativeClient::loadChats(int limit) { messages->loadChats(limit); }
void TdNativeClient::getChatHistory(int64_t chatId, int limit, int64_t fromId) {
    messages->getChatHistory(chatId, limit, fromId);
}
void TdNativeClient::deleteMessages(int64_t chatId, const std::vector<int64_t>& ids, bool revoke) {
    messages->deleteMessages(chatId, ids, revoke);
}
void TdNativeClient::editMessage(int64_t chatId, int64_t msgId, const std::string& newText) {
    messages->editMessage(chatId, msgId, newText);
}
void TdNativeClient::forwardMessages(int64_t toChatId, int64_t fromChatId,
                                     const std::vector<int64_t>& msgIds, bool sendCopy) {
    messages->forwardMessages(toChatId, fromChatId, msgIds, sendCopy);
}
void TdNativeClient::setReaction(int64_t chatId, int64_t msgId, const std::string& reaction) {
    messages->setMessageReaction(chatId, msgId, reaction);
}
void TdNativeClient::getScheduledMessages(int64_t chatId) {
    messages->getScheduledMessages(chatId);
}
void TdNativeClient::sendScheduled(int64_t chatId, const std::string& text, int scheduleDate) {
    messages->sendScheduled(chatId, text, scheduleDate);
}
void TdNativeClient::sendTextWithTtl(int64_t chatId, const std::string& text, int ttlSeconds) {
    messages->sendTextWithTtl(chatId, text, ttlSeconds);
}
void TdNativeClient::sendTextSilent(int64_t chatId, const std::string& text, int64_t threadId) {
    messages->sendTextSilent(chatId, text, threadId);
}
void TdNativeClient::setChatDraftMessage(int64_t chatId, int64_t threadId, const std::string& text) {
    messages->setChatDraftMessage(chatId, threadId, text);
}
void TdNativeClient::clearChatDraftMessage(int64_t chatId, int64_t threadId) {
    messages->clearChatDraftMessage(chatId, threadId);
}
void TdNativeClient::pinMessage(int64_t chatId, int64_t msgId, bool disableNotification) {
    messages->pinMessage(chatId, msgId, disableNotification);
}
void TdNativeClient::unpinMessage(int64_t chatId, int64_t msgId) {
    messages->unpinMessage(chatId, msgId);
}
void TdNativeClient::markAsRead(int64_t chatId, const std::vector<int64_t>& msgIds) {
    messages->viewMessages(chatId, msgIds);
}

// --- Chats ---
void TdNativeClient::setChatTitle(int64_t chatId, const std::string& title) { /* via editForumTopic in messages */ }
void TdNativeClient::openChat(int64_t chatId) { messages->openChat(chatId); }
void TdNativeClient::createGroup(const std::string& title) { messages->createNewBasicGroupChat({}, title); }
void TdNativeClient::createChannel(const std::string& title) { messages->createNewSupergroupChat(title, true); }
void TdNativeClient::leaveChat(int64_t chatId) { /* leaveChat in messages */ }

// --- Threads ---
void TdNativeClient::getMessageThread(int64_t chatId, int64_t msgId) {
    messages->getMessageThread(chatId, msgId);
}
void TdNativeClient::getThreadHistory(int64_t chatId, int64_t msgId, int64_t fromId, int limit) {
    messages->getMessageThreadHistory(chatId, msgId, fromId, limit);
}
void TdNativeClient::createForumTopic(int64_t chatId, const std::string& name, int iconColor) {
    messages->createForumTopic(chatId, name, iconColor);
}
void TdNativeClient::deleteForumTopic(int64_t chatId, int64_t topicId) {
    messages->deleteForumTopic(chatId, topicId);
}
void TdNativeClient::getForumTopics(int64_t chatId, const std::string& query, int limit) {
    messages->getForumTopics(chatId, query, limit);
}

// --- Stories ---
void TdNativeClient::getActiveStories(int limit) { messages->getActiveStories(limit); }

// --- Profile ---
void TdNativeClient::getMe() { messages->getMe(); }
void TdNativeClient::getUser(int64_t userId) { messages->getUser(userId); }
void TdNativeClient::setBio(const std::string& bio) { messages->setBio(bio); }
void TdNativeClient::setName(const std::string& first, const std::string& last) {
    messages->setName(first, last);
}
void TdNativeClient::setUsername(const std::string& u) { messages->setUsername(u); }
void TdNativeClient::setProfilePhoto(const std::string& path) { messages->setProfilePhoto(path); }
void TdNativeClient::setEmojiStatus(int64_t id, int duration) { messages->setEmojiStatus(id, duration); }
void TdNativeClient::clearEmojiStatus() { messages->clearEmojiStatus(); }

// --- Security ---
void TdNativeClient::getActiveSessions() { messages->getActiveSessions(); }
void TdNativeClient::terminateSession(int64_t sessionId) { messages->terminateSession(sessionId); }
void TdNativeClient::terminateAllOtherSessions() { messages->terminateAllOtherSessions(); }
void TdNativeClient::blockUser(int64_t userId) { messages->blockUser(userId); }
void TdNativeClient::unblockUser(int64_t userId) { messages->unblockUser(userId); }
void TdNativeClient::getBlockedUsers(int offset, int limit) { messages->getBlockedUsers(offset, limit); }

// --- Files ---
void TdNativeClient::downloadFile(int fileId, int priority) {
    messages->downloadFile(fileId, priority);
    fileManager->addDownload(fileId);
}
void TdNativeClient::cancelDownload(int fileId) {
    messages->cancelDownloadFile(fileId);
    fileManager->removeDownload(fileId);
}
void TdNativeClient::getFileInfo(int fileId) { messages->getFile(fileId); }
void TdNativeClient::deleteFile(int fileId) { messages->deleteFile(fileId); }

std::vector<TdMessage> TdNativeClient::getCachedMessages(int64_t chatId, int limit, int64_t fromId) {
    return cache->getMessages(chatId, limit, fromId);
}

} // namespace pcp
