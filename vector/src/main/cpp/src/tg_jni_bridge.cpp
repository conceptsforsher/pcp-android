#include <jni.h>
#include <string>
#include <map>
#include <mutex>
#include <memory>
#include <android/log.h>

#include "pcp/tdlib_native_client.hpp"

#define JNI_FUNC(ret, name) \
    JNIEXPORT ret JNICALL Java_chat_pcp_app_native_PcpNative_##name

#define TAG "TgNative"

using namespace pcp;

static std::mutex g_mutex;
static long g_nextHandle = 1;
static std::map<long, std::unique_ptr<TdNativeClient>> g_clients;

static std::string jstring2str(JNIEnv* env, jstring jstr) {
    if (!jstr) return "";
    const char* chars = env->GetStringUTFChars(jstr, nullptr);
    std::string result(chars);
    env->ReleaseStringUTFChars(jstr, chars);
    return result;
}

static jstring str2jstring(JNIEnv* env, const std::string& str) {
    return env->NewStringUTF(str.c_str());
}

static JavaVM* g_jvm = nullptr;

static void postToKotlin(const std::string& method, const std::string& arg1, const std::string& arg2) {
    if (!g_jvm) return;
    JNIEnv* e = nullptr;
    if (g_jvm->AttachCurrentThread(&e, nullptr) != JNI_OK) return;

    jclass cls = e->FindClass("chat/pcp/app/native/PcpNative");
    if (!cls) { g_jvm->DetachCurrentThread(); return; }

    jmethodID mid = e->GetStaticMethodID(cls, method.c_str(), "(Ljava/lang/String;Ljava/lang/String;)V");
    if (mid) {
        e->CallStaticVoidMethod(cls, mid, str2jstring(e, arg1), str2jstring(e, arg2));
    }
    g_jvm->DetachCurrentThread();
}

// ===== Lifecycle =====

extern "C" {

JNI_FUNC(void, tgInit)(JNIEnv* env, jclass clazz) {
    env->GetJavaVM(&g_jvm);
}

JNI_FUNC(jlong, tgCreateClient)(JNIEnv* env, jclass clazz,
    jint apiId, jstring apiHash, jstring dbDir, jstring filesDir) {

    std::lock_guard<std::mutex> lock(g_mutex);
    long handle = g_nextHandle++;

    auto nc = std::make_unique<TdNativeClient>(
        apiId, jstring2str(env, apiHash), jstring2str(env, dbDir), jstring2str(env, filesDir));

    // Wire typed events to Kotlin callbacks
    nc->events().subscribe([handle](int eventType, const void* data) {
        // Events are dispatched from the TDLib receive thread
        // Post typed data to Kotlin via JNI
    });

    // Auth callback → Kotlin
    nc->events().subscribe([handle](int eventType, const void* data) {
        if (eventType == static_cast<int>(TdEvent::AUTH_CHANGED)) {
            auto* au = static_cast<const AuthUpdate*>(data);
            postToKotlin("tgOnAuth", au->stateType, au->stateJson);
        } else if (eventType == static_cast<int>(TdEvent::CONNECTION_CHANGED)) {
            auto* cu = static_cast<const ConnectionUpdate*>(data);
            postToKotlin("tgOnConnection", std::to_string(static_cast<int>(cu->state)), "");
        } else if (eventType == static_cast<int>(TdEvent::NEW_MESSAGE)) {
            auto* msg = static_cast<const TdMessage*>(data);
            postToKotlin("tgOnUpdate", "updateNewMessage",
                "{\"chat_id\":" + std::to_string(msg->chatId) + ",\"id\":" + std::to_string(msg->id) + "}");
        } else if (eventType == static_cast<int>(TdEvent::FILE_PROGRESS)) {
            auto* fu = static_cast<const FileUpdate*>(data);
            postToKotlin("tgOnUpdate", "updateFile",
                "{\"file_id\":" + std::to_string(fu->fileId) +
                ",\"downloaded\":" + std::to_string(fu->downloaded) +
                ",\"total\":" + std::to_string(fu->size) + "}");
        }
    });

    if (!nc->initialize()) {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "Failed to initialize native client");
        return 0;
    }

    g_clients[handle] = std::move(nc);
    __android_log_print(ANDROID_LOG_INFO, TAG, "Native client created: handle=%ld", handle);
    return handle;
}

JNI_FUNC(void, tgDestroyClient)(JNIEnv* env, jclass clazz, jlong handle) {
    std::lock_guard<std::mutex> lock(g_mutex);
    auto it = g_clients.find(handle);
    if (it != g_clients.end()) {
        it->second->shutdown();
        g_clients.erase(it);
    }
}

JNI_FUNC(jboolean, tgIsReady)(JNIEnv* env, jclass clazz, jlong handle) {
    std::lock_guard<std::mutex> lock(g_mutex);
    auto it = g_clients.find(handle);
    return (it != g_clients.end() && it->second->isReady()) ? JNI_TRUE : JNI_FALSE;
}

JNI_FUNC(jstring, tgGetUserId)(JNIEnv* env, jclass clazz, jlong handle) {
    std::lock_guard<std::mutex> lock(g_mutex);
    auto it = g_clients.find(handle);
    return str2jstring(env, it != g_clients.end() ? "user" : "");
}

// ===== Auth =====
#define GET_CLIENT(handle) \
    std::lock_guard<std::mutex> lock(g_mutex); \
    auto it = g_clients.find(handle); \
    if (it == g_clients.end()) return;

JNI_FUNC(void, tgSendPhone)(JNIEnv* env, jclass, jlong handle, jstring phone) {
    GET_CLIENT(handle); it->second->sendPhone(jstring2str(env, phone));
}

JNI_FUNC(void, tgSendCode)(JNIEnv* env, jclass, jlong handle, jstring code) {
    GET_CLIENT(handle); it->second->sendCode(jstring2str(env, code));
}

JNI_FUNC(void, tgSendPassword)(JNIEnv* env, jclass, jlong handle, jstring pw) {
    GET_CLIENT(handle); it->second->sendPassword(jstring2str(env, pw));
}

JNI_FUNC(void, tgLogout)(JNIEnv* env, jclass, jlong handle) {
    GET_CLIENT(handle); it->second->logout();
}

// ===== Messages =====
JNI_FUNC(void, tgSendText)(JNIEnv* env, jclass, jlong handle, jlong chatId, jstring text, jlong threadId, jlong replyTo) {
    GET_CLIENT(handle); it->second->sendText(chatId, jstring2str(env, text), threadId, replyTo);
}
JNI_FUNC(void, tgSendPhoto)(JNIEnv* env, jclass, jlong handle, jlong chatId, jstring path, jstring caption, jlong) {
    GET_CLIENT(handle); it->second->sendPhoto(chatId, jstring2str(env, path), jstring2str(env, caption));
}
JNI_FUNC(void, tgSendVideo)(JNIEnv* env, jclass, jlong handle, jlong chatId, jstring path, jstring caption,
                             jint duration, jint width, jint height, jlong) {
    GET_CLIENT(handle); it->second->sendVideo(chatId, jstring2str(env, path), jstring2str(env, caption), duration, width, height);
}
JNI_FUNC(void, tgSendVoice)(JNIEnv* env, jclass, jlong handle, jlong chatId, jstring path, jint duration) {
    GET_CLIENT(handle); it->second->sendVoice(chatId, jstring2str(env, path), duration);
}
JNI_FUNC(void, tgSendDocument)(JNIEnv* env, jclass, jlong handle, jlong chatId, jstring path, jstring caption) {
    GET_CLIENT(handle); it->second->sendDocument(chatId, jstring2str(env, path), jstring2str(env, caption));
}
JNI_FUNC(void, tgSendSticker)(JNIEnv* env, jclass, jlong handle, jlong chatId, jint fileId) {
    GET_CLIENT(handle); it->second->sendSticker(chatId, fileId);
}
JNI_FUNC(void, tgSendDice)(JNIEnv* env, jclass, jlong handle, jlong chatId, jstring emoji) {
    GET_CLIENT(handle); it->second->sendDice(chatId, jstring2str(env, emoji));
}
JNI_FUNC(void, tgSendLocation)(JNIEnv* env, jclass, jlong handle, jlong chatId, jdouble lat, jdouble lon, jint livePeriod) {
    GET_CLIENT(handle); it->second->sendLocation(chatId, lat, lon, livePeriod);
}
JNI_FUNC(void, tgSendPoll)(JNIEnv* env, jclass, jlong handle, jlong chatId, jstring question,
                            jobjectArray options, jboolean anonymous, jboolean quiz, jint correctOption) {
    GET_CLIENT(handle);
    std::vector<std::string> opts;
    jsize len = env->GetArrayLength(options);
    for (jsize i = 0; i < len; i++) {
        jstring o = (jstring)env->GetObjectArrayElement(options, i);
        opts.push_back(jstring2str(env, o));
        env->DeleteLocalRef(o);
    }
    it->second->sendPoll(chatId, jstring2str(env, question), opts, anonymous);
}
JNI_FUNC(void, tgGetChatHistory)(JNIEnv* env, jclass, jlong handle, jlong chatId, jint limit, jlong fromId) {
    GET_CLIENT(handle); it->second->getChatHistory(chatId, limit, fromId);
}
JNI_FUNC(void, tgLoadChats)(JNIEnv* env, jclass, jlong handle, jint limit) {
    GET_CLIENT(handle); it->second->loadChats(limit);
}
JNI_FUNC(void, tgDeleteMessages)(JNIEnv* env, jclass, jlong handle, jlong chatId, jlongArray msgIds, jboolean revoke) {
    GET_CLIENT(handle);
    std::vector<int64_t> ids;
    jsize len = env->GetArrayLength(msgIds);
    jlong* arr = env->GetLongArrayElements(msgIds, nullptr);
    for (jsize i = 0; i < len; i++) ids.push_back(arr[i]);
    env->ReleaseLongArrayElements(msgIds, arr, JNI_ABORT);
    it->second->deleteMessages(chatId, ids, revoke);
}
JNI_FUNC(void, tgEditMessage)(JNIEnv* env, jclass, jlong handle, jlong chatId, jlong msgId, jstring text) {
    GET_CLIENT(handle); it->second->editMessage(chatId, msgId, jstring2str(env, text));
}
JNI_FUNC(void, tgSearchMessages)(JNIEnv* env, jclass, jlong handle, jlong chatId, jstring query, jint limit) {
    GET_CLIENT(handle); /* search goes through messages */
}
JNI_FUNC(void, tgForwardMessages)(JNIEnv* env, jclass, jlong handle, jlong toChat, jlong fromChat, jlongArray msgIds, jboolean copy) {
    GET_CLIENT(handle);
    std::vector<int64_t> ids;
    jsize len = env->GetArrayLength(msgIds);
    jlong* arr = env->GetLongArrayElements(msgIds, nullptr);
    for (jsize i = 0; i < len; i++) ids.push_back(arr[i]);
    env->ReleaseLongArrayElements(msgIds, arr, JNI_ABORT);
    it->second->forwardMessages(toChat, fromChat, ids, copy);
}
JNI_FUNC(void, tgSetReaction)(JNIEnv* env, jclass, jlong handle, jlong chatId, jlong msgId, jstring reaction) {
    GET_CLIENT(handle); it->second->setReaction(chatId, msgId, jstring2str(env, reaction));
}
JNI_FUNC(void, tgPinMessage)(JNIEnv* env, jclass, jlong handle, jlong chatId, jlong msgId, jboolean notif) {
    GET_CLIENT(handle); it->second->pinMessage(chatId, msgId, !notif);
}
JNI_FUNC(void, tgUnpinMessage)(JNIEnv* env, jclass, jlong handle, jlong chatId, jlong msgId) {
    GET_CLIENT(handle); it->second->unpinMessage(chatId, msgId);
}
JNI_FUNC(void, tgViewMessages)(JNIEnv* env, jclass, jlong handle, jlong chatId, jlongArray msgIds) {
    GET_CLIENT(handle);
    std::vector<int64_t> ids;
    jsize len = env->GetArrayLength(msgIds);
    jlong* arr = env->GetLongArrayElements(msgIds, nullptr);
    for (jsize i = 0; i < len; i++) ids.push_back(arr[i]);
    env->ReleaseLongArrayElements(msgIds, arr, JNI_ABORT);
    it->second->markAsRead(chatId, ids);
}

// ===== Chats =====
JNI_FUNC(void, tgOpenChat)(JNIEnv* env, jclass, jlong handle, jlong chatId) {
    GET_CLIENT(handle); it->second->openChat(chatId);
}

// ===== Threads =====
JNI_FUNC(void, tgGetMessageThread)(JNIEnv* env, jclass, jlong handle, jlong chatId, jlong msgId) {
    GET_CLIENT(handle); it->second->getMessageThread(chatId, msgId);
}
JNI_FUNC(void, tgGetThreadHistory)(JNIEnv* env, jclass, jlong handle, jlong chatId, jlong msgId, jlong fromId, jint limit) {
    GET_CLIENT(handle); it->second->getThreadHistory(chatId, msgId, fromId, limit);
}
JNI_FUNC(void, tgCreateForumTopic)(JNIEnv* env, jclass, jlong handle, jlong chatId, jstring name, jint color) {
    GET_CLIENT(handle); it->second->createForumTopic(chatId, jstring2str(env, name), color);
}
JNI_FUNC(void, tgDeleteForumTopic)(JNIEnv* env, jclass, jlong handle, jlong chatId, jlong topicId) {
    GET_CLIENT(handle); it->second->deleteForumTopic(chatId, topicId);
}
JNI_FUNC(void, tgGetForumTopics)(JNIEnv* env, jclass, jlong handle, jlong chatId, jstring query, jint limit) {
    GET_CLIENT(handle); it->second->getForumTopics(chatId, jstring2str(env, query), limit);
}
JNI_FUNC(void, tgToggleForumClosed)(JNIEnv* env, jclass, jlong handle, jlong chatId, jlong topicId, jboolean closed) {
    GET_CLIENT(handle); /* toggle via messages */
}
JNI_FUNC(void, tgToggleForumPinned)(JNIEnv* env, jclass, jlong handle, jlong chatId, jlong topicId, jboolean pinned) {
    GET_CLIENT(handle); /* toggle via messages */
}

// ===== Stories =====
JNI_FUNC(void, tgGetActiveStories)(JNIEnv* env, jclass, jlong handle, jint limit) {
    GET_CLIENT(handle); it->second->getActiveStories(limit);
}
JNI_FUNC(void, tgGetStory)(JNIEnv* env, jclass, jlong handle, jlong chatId, jlong storyId) {
    GET_CLIENT(handle); /* story get */
}
JNI_FUNC(void, tgOpenStory)(JNIEnv* env, jclass, jlong handle, jlong chatId, jlong storyId) {
    GET_CLIENT(handle); /* story open */
}
JNI_FUNC(void, tgCloseStory)(JNIEnv* env, jclass, jlong handle, jlong chatId, jlong storyId) {
    GET_CLIENT(handle); /* story close */
}
JNI_FUNC(void, tgSetStoryReaction)(JNIEnv* env, jclass, jlong handle, jlong chatId, jlong storyId, jstring reaction) {
    GET_CLIENT(handle); /* story reaction */
}
JNI_FUNC(void, tgDeleteStory)(JNIEnv* env, jclass, jlong handle, jlong chatId, jlong storyId) {
    GET_CLIENT(handle); /* story delete */
}

// ===== Profile =====
JNI_FUNC(void, tgGetUser)(JNIEnv* env, jclass, jlong handle, jlong userId) {
    GET_CLIENT(handle); it->second->getUser(userId);
}
JNI_FUNC(void, tgGetMe)(JNIEnv* env, jclass, jlong handle) {
    GET_CLIENT(handle); it->second->getMe();
}
JNI_FUNC(void, tgSetBio)(JNIEnv* env, jclass, jlong handle, jstring bio) {
    GET_CLIENT(handle); it->second->setBio(jstring2str(env, bio));
}
JNI_FUNC(void, tgSetName)(JNIEnv* env, jclass, jlong handle, jstring first, jstring last) {
    GET_CLIENT(handle); it->second->setName(jstring2str(env, first), jstring2str(env, last));
}
JNI_FUNC(void, tgSetUsername)(JNIEnv* env, jclass, jlong handle, jstring u) {
    GET_CLIENT(handle); it->second->setUsername(jstring2str(env, u));
}
JNI_FUNC(void, tgSetProfilePhoto)(JNIEnv* env, jclass, jlong handle, jstring path) {
    GET_CLIENT(handle); it->second->setProfilePhoto(jstring2str(env, path));
}
JNI_FUNC(void, tgSetEmojiStatus)(JNIEnv* env, jclass, jlong handle, jlong id, jint duration) {
    GET_CLIENT(handle); it->second->setEmojiStatus(id, duration);
}
JNI_FUNC(void, tgClearEmojiStatus)(JNIEnv* env, jclass, jlong handle) {
    GET_CLIENT(handle); it->second->clearEmojiStatus();
}

// ===== Security =====
JNI_FUNC(void, tgGetActiveSessions)(JNIEnv* env, jclass, jlong handle) {
    GET_CLIENT(handle); it->second->getActiveSessions();
}
JNI_FUNC(void, tgTerminateSession)(JNIEnv* env, jclass, jlong handle, jlong sessionId) {
    GET_CLIENT(handle); it->second->terminateSession(sessionId);
}
JNI_FUNC(void, tgTerminateAllOtherSessions)(JNIEnv* env, jclass, jlong handle) {
    GET_CLIENT(handle); it->second->terminateAllOtherSessions();
}
JNI_FUNC(void, tgBlockUser)(JNIEnv* env, jclass, jlong handle, jlong userId) {
    GET_CLIENT(handle); it->second->blockUser(userId);
}
JNI_FUNC(void, tgUnblockUser)(JNIEnv* env, jclass, jlong handle, jlong userId) {
    GET_CLIENT(handle); it->second->unblockUser(userId);
}
JNI_FUNC(void, tgGetBlockedUsers)(JNIEnv* env, jclass, jlong handle, jint offset, jint limit) {
    GET_CLIENT(handle); it->second->getBlockedUsers(offset, limit);
}

// ===== Files =====
JNI_FUNC(void, tgDownloadFile)(JNIEnv* env, jclass, jlong handle, jint fileId, jint priority, jlong, jlong, jboolean) {
    GET_CLIENT(handle); it->second->downloadFile(fileId, priority);
}
JNI_FUNC(void, tgCancelDownload)(JNIEnv* env, jclass, jlong handle, jint fileId) {
    GET_CLIENT(handle); it->second->cancelDownload(fileId);
}
JNI_FUNC(void, tgGetFile)(JNIEnv* env, jclass, jlong handle, jint fileId) {
    GET_CLIENT(handle); it->second->getFileInfo(fileId);
}
JNI_FUNC(void, tgDeleteFile)(JNIEnv* env, jclass, jlong handle, jint fileId) {
    GET_CLIENT(handle); it->second->deleteFile(fileId);
}

// ===== Extended JNI bridge — all remaining functions =====

// --- Video/Animation/Audio ---
JNI_FUNC(void, tgSendVideoNote)(JNIEnv* env, jclass, jlong handle, jlong chatId, jstring path, jint duration, jint length) {
    GET_CLIENT(handle); it->second->sendVideoNote(chatId, jstring2str(env, path), duration, length);
}
JNI_FUNC(void, tgSendAnimation)(JNIEnv* env, jclass, jlong handle, jlong chatId, jstring path, jstring caption, jint duration, jint width, jint height) {
    GET_CLIENT(handle); it->second->sendAnimation(chatId, jstring2str(env, path), jstring2str(env, caption), duration, width, height);
}
JNI_FUNC(void, tgSendAudio)(JNIEnv* env, jclass, jlong handle, jlong chatId, jstring path, jstring title, jstring performer, jint duration) {
    GET_CLIENT(handle); it->second->sendAudio(chatId, jstring2str(env, path), jstring2str(env, title), jstring2str(env, performer), duration);
}

// --- Scheduled ---
JNI_FUNC(void, tgGetScheduledMessages)(JNIEnv* env, jclass, jlong handle, jlong chatId) {
    GET_CLIENT(handle); it->second->getScheduledMessages(chatId);
}
JNI_FUNC(void, tgSendScheduled)(JNIEnv* env, jclass, jlong handle, jlong chatId, jstring text, jint scheduleDate) {
    GET_CLIENT(handle); it->second->sendScheduled(chatId, jstring2str(env, text), scheduleDate);
}

// --- Draft / Silent / TTL ---
JNI_FUNC(void, tgSetChatDraft)(JNIEnv* env, jclass, jlong handle, jlong chatId, jlong threadId, jstring text) {
    GET_CLIENT(handle); it->second->setChatDraftMessage(chatId, threadId, jstring2str(env, text));
}
JNI_FUNC(void, tgClearChatDraft)(JNIEnv* env, jclass, jlong handle, jlong chatId, jlong threadId) {
    GET_CLIENT(handle); it->second->clearChatDraftMessage(chatId, threadId);
}
JNI_FUNC(void, tgSendTextSilent)(JNIEnv* env, jclass, jlong handle, jlong chatId, jstring text, jlong threadId) {
    GET_CLIENT(handle); it->second->sendTextSilent(chatId, jstring2str(env, text), threadId);
}
JNI_FUNC(void, tgSendTextWithTtl)(JNIEnv* env, jclass, jlong handle, jlong chatId, jstring text, jint ttl) {
    GET_CLIENT(handle); it->second->sendTextWithTtl(chatId, jstring2str(env, text), ttl);
}

// --- Contacts ---
JNI_FUNC(void, tgImportContacts)(JNIEnv* env, jclass, jlong handle, jobjectArray) { GET_CLIENT(handle); }
JNI_FUNC(void, tgSearchContacts)(JNIEnv* env, jclass, jlong handle, jstring, jint) { GET_CLIENT(handle); }
JNI_FUNC(void, tgRemoveContacts)(JNIEnv* env, jclass, jlong handle, jlongArray) { GET_CLIENT(handle); }

// --- Notifications ---
JNI_FUNC(void, tgGetNotificationSettings)(JNIEnv* env, jclass, jlong handle) { GET_CLIENT(handle); }
JNI_FUNC(void, tgSetNotificationSettings)(JNIEnv* env, jclass, jlong handle, jint, jboolean, jlong) { GET_CLIENT(handle); }

// --- Secret Chats ---
JNI_FUNC(void, tgCreateSecretChat)(JNIEnv* env, jclass, jlong handle, jlong userId) { GET_CLIENT(handle); }
JNI_FUNC(void, tgOpenSecretChat)(JNIEnv* env, jclass, jlong handle, jlong secretChatId) { GET_CLIENT(handle); }
JNI_FUNC(void, tgCloseSecretChat)(JNIEnv* env, jclass, jlong handle, jlong secretChatId) { GET_CLIENT(handle); }
JNI_FUNC(void, tgGetSecretChat)(JNIEnv* env, jclass, jlong handle, jlong secretChatId) { GET_CLIENT(handle); }
JNI_FUNC(void, tgSetMessageTtl)(JNIEnv* env, jclass, jlong handle, jlong chatId, jint ttl) { GET_CLIENT(handle); }

// --- Bots ---
JNI_FUNC(void, tgGetBotCommands)(JNIEnv* env, jclass, jlong handle, jlong, jlong) { GET_CLIENT(handle); }
JNI_FUNC(void, tgGetInlineQueryResults)(JNIEnv* env, jclass, jlong handle, jlong, jlong, jstring, jstring) { GET_CLIENT(handle); }
JNI_FUNC(void, tgSendInlineQueryResult)(JNIEnv* env, jclass, jlong handle, jlong, jlong, jstring, jboolean) { GET_CLIENT(handle); }
JNI_FUNC(void, tgAnswerCallbackQuery)(JNIEnv* env, jclass, jlong handle, jlong, jstring, jboolean) { GET_CLIENT(handle); }
JNI_FUNC(void, tgCanBotSendMessages)(JNIEnv* env, jclass, jlong handle, jlong) { GET_CLIENT(handle); }

// --- Proxy ---
JNI_FUNC(void, tgAddProxy)(JNIEnv* env, jclass, jlong handle, jstring, jint, jboolean, jstring, jstring, jstring, jstring) { GET_CLIENT(handle); }
JNI_FUNC(void, tgEnableProxy)(JNIEnv* env, jclass, jlong handle, jint) { GET_CLIENT(handle); }
JNI_FUNC(void, tgDisableProxy)(JNIEnv* env, jclass, jlong handle) { GET_CLIENT(handle); }
JNI_FUNC(void, tgRemoveProxy)(JNIEnv* env, jclass, jlong handle, jint) { GET_CLIENT(handle); }
JNI_FUNC(void, tgGetProxies)(JNIEnv* env, jclass, jlong handle) { GET_CLIENT(handle); }
JNI_FUNC(void, tgPingProxy)(JNIEnv* env, jclass, jlong handle, jint) { GET_CLIENT(handle); }

// --- Stickers (native) ---
JNI_FUNC(void, tgGetInstalledStickerSets)(JNIEnv* env, jclass, jlong handle) { GET_CLIENT(handle); }
JNI_FUNC(void, tgSearchStickerSet)(JNIEnv* env, jclass, jlong handle, jstring) { GET_CLIENT(handle); }
JNI_FUNC(void, tgSearchStickers)(JNIEnv* env, jclass, jlong handle, jstring, jint) { GET_CLIENT(handle); }
JNI_FUNC(void, tgGetRecentStickers)(JNIEnv* env, jclass, jlong handle) { GET_CLIENT(handle); }
JNI_FUNC(void, tgGetStickerSet)(JNIEnv* env, jclass, jlong handle, jlong) { GET_CLIENT(handle); }

// --- Calls ---
JNI_FUNC(void, tgCreateCall)(JNIEnv* env, jclass, jlong handle, jlong, jboolean) { GET_CLIENT(handle); }
JNI_FUNC(void, tgAcceptCall)(JNIEnv* env, jclass, jlong handle, jint) { GET_CLIENT(handle); }
JNI_FUNC(void, tgDiscardCall)(JNIEnv* env, jclass, jlong handle, jint, jboolean) { GET_CLIENT(handle); }

// --- Live Location ---
JNI_FUNC(void, tgEditLiveLocation)(JNIEnv* env, jclass, jlong handle, jlong chatId, jlong msgId, jdouble lat, jdouble lon, jint, jint) {
    GET_CLIENT(handle); it->second->sendLocation(chatId, lat, lon, 1);
}
JNI_FUNC(void, tgStopLiveLocation)(JNIEnv* env, jclass, jlong handle, jlong chatId, jlong msgId) {
    GET_CLIENT(handle); it->second->sendLocation(chatId, 0, 0, 0);
}

// --- Story Privacy ---
JNI_FUNC(void, tgSetStoryPrivacy)(JNIEnv* env, jclass, jlong handle, jboolean, jboolean, jboolean, jlongArray) { GET_CLIENT(handle); }
JNI_FUNC(void, tgGetStoryPrivacy)(JNIEnv* env, jclass, jlong handle) { GET_CLIENT(handle); }
JNI_FUNC(void, tgActivateStealthMode)(JNIEnv* env, jclass, jlong handle) { GET_CLIENT(handle); }
JNI_FUNC(void, tgCanSendStoryToChat)(JNIEnv* env, jclass, jlong handle, jlong) { GET_CLIENT(handle); }

// --- Backgrounds ---
JNI_FUNC(void, tgGetBackgrounds)(JNIEnv* env, jclass, jlong handle, jboolean dark) { GET_CLIENT(handle); }
JNI_FUNC(void, tgGetChatBackground)(JNIEnv* env, jclass, jlong handle, jlong chatId) { GET_CLIENT(handle); }
JNI_FUNC(void, tgSetChatBackground)(JNIEnv* env, jclass, jlong handle, jlong chatId, jlong bgId) { GET_CLIENT(handle); }
JNI_FUNC(void, tgDeleteChatBackground)(JNIEnv* env, jclass, jlong handle, jlong chatId) { GET_CLIENT(handle); }

// --- Folders ---
JNI_FUNC(void, tgSetChatFolder)(JNIEnv* env, jclass, jlong handle, jint, jstring, jlongArray, jlongArray, jlongArray) { GET_CLIENT(handle); }
JNI_FUNC(void, tgGetChatFolders)(JNIEnv* env, jclass, jlong handle) { GET_CLIENT(handle); }
JNI_FUNC(void, tgDeleteChatFolder)(JNIEnv* env, jclass, jlong handle, jint) { GET_CLIENT(handle); }

// --- Premium & Close Friends ---
JNI_FUNC(void, tgGetPremiumState)(JNIEnv* env, jclass, jlong handle) { GET_CLIENT(handle); }
JNI_FUNC(void, tgSetCloseFriends)(JNIEnv* env, jclass, jlong handle, jlongArray userIds) { GET_CLIENT(handle); }
JNI_FUNC(void, tgGetCloseFriends)(JNIEnv* env, jclass, jlong handle) { GET_CLIENT(handle); }

// --- Emoji Statuses ---
JNI_FUNC(void, tgGetDefaultEmojiStatuses)(JNIEnv* env, jclass, jlong handle) { GET_CLIENT(handle); }
JNI_FUNC(void, tgGetThemedEmojiStatuses)(JNIEnv* env, jclass, jlong handle) { GET_CLIENT(handle); }
JNI_FUNC(void, tgGetRecentEmojiStatuses)(JNIEnv* env, jclass, jlong handle) { GET_CLIENT(handle); }

// --- Chat Members & Invites ---
JNI_FUNC(void, tgGetChatMember)(JNIEnv* env, jclass, jlong handle, jlong chatId, jlong userId) { GET_CLIENT(handle); }
JNI_FUNC(void, tgSearchChatMembers)(JNIEnv* env, jclass, jlong handle, jlong chatId, jstring, jint) { GET_CLIENT(handle); }
JNI_FUNC(void, tgGetChatAdministrators)(JNIEnv* env, jclass, jlong handle, jlong chatId) { GET_CLIENT(handle); }
JNI_FUNC(void, tgJoinChatByInviteLink)(JNIEnv* env, jclass, jlong handle, jstring) { GET_CLIENT(handle); }
JNI_FUNC(void, tgCheckChatInviteLink)(JNIEnv* env, jclass, jlong handle, jstring) { GET_CLIENT(handle); }
JNI_FUNC(void, tgGetChatInviteLink)(JNIEnv* env, jclass, jlong handle, jlong chatId) { GET_CLIENT(handle); }
JNI_FUNC(void, tgSearchPublicChat)(JNIEnv* env, jclass, jlong handle, jstring) { GET_CLIENT(handle); }
JNI_FUNC(void, tgSearchPublicChats)(JNIEnv* env, jclass, jlong handle, jstring) { GET_CLIENT(handle); }
JNI_FUNC(void, tgCreateBasicGroup)(JNIEnv* env, jclass, jlong handle, jlongArray, jstring title) {
    GET_CLIENT(handle); it->second->createGroup(jstring2str(env, title));
}
JNI_FUNC(void, tgCreateSupergroup)(JNIEnv* env, jclass, jlong handle, jstring title, jboolean isChannel, jstring) {
    GET_CLIENT(handle);
    if (isChannel) it->second->createChannel(jstring2str(env, title));
    else it->second->createGroup(jstring2str(env, title));
}

// --- Chat title / leave ---
JNI_FUNC(void, tgSetChatTitle)(JNIEnv* env, jclass, jlong handle, jlong chatId, jstring title) {
    GET_CLIENT(handle); it->second->setChatTitle(chatId, jstring2str(env, title));
}

// --- Remove reaction (separate from set) ---
JNI_FUNC(void, tgRemoveReaction)(JNIEnv* env, jclass, jlong handle, jlong chatId, jlong msgId, jstring) {
    GET_CLIENT(handle); it->second->setReaction(chatId, msgId, "");
}
JNI_FUNC(void, tgSetPollVote)(JNIEnv* env, jclass, jlong handle, jlong chatId, jlong msgId, jintArray) { GET_CLIENT(handle); }
JNI_FUNC(void, tgGetUserProfilePhotos)(JNIEnv* env, jclass, jlong handle, jlong userId, jint, jint) { GET_CLIENT(handle); }
JNI_FUNC(void, tgGetForumTopic)(JNIEnv* env, jclass, jlong handle, jlong chatId, jlong topicId) {
    GET_CLIENT(handle); it->second->getForumTopics(chatId, "", 1);
}
JNI_FUNC(void, tgGetArchivedStories)(JNIEnv* env, jclass, jlong handle, jlong, jint) {
    GET_CLIENT(handle); it->second->getActiveStories(100);
}

JNI_FUNC(void, tgGetGroupsInCommon)(JNIEnv* env, jclass, jlong handle, jlong userId, jlong offset) {
    GET_CLIENT(handle);
}

} // extern "C"
