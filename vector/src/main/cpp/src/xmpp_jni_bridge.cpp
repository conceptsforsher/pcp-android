#include <jni.h>
#include <string>
#include <android/log.h>
#include <sstream>
#include <unordered_map>
#include <mutex>

#define LOG_TAG "XmppNative"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#define JNI_XMPP(ret, name) \
    JNIEXPORT ret JNICALL Java_chat_pcp_app_xmpp_XmppNative_##name

#ifdef PCP_HAS_XMPP

#include "pcp/xmpp_client.hpp"
#include "pcp/json_parser.hpp"
#include "pcp/exporter.hpp"
#include <memory>

static JavaVM* g_xmppJvm = nullptr;
static jclass g_xmppCallbackClass = nullptr;
static std::mutex g_xmppMutex;
static std::unordered_map<int64_t, std::unique_ptr<pcp::XmppClient>> g_xmppClients;
static int64_t g_xmppNextHandle = 1;

static std::string jStr(JNIEnv* env, jstring js) {
    if (!js) return "";
    const char* chars = env->GetStringUTFChars(js, nullptr);
    std::string result(chars);
    env->ReleaseStringUTFChars(js, chars);
    return result;
}

static void xmppPostToKotlin(const std::string& methodName, const std::string& json) {
    if (!g_xmppJvm) return;

    JNIEnv* env = nullptr;
    jint attachResult = g_xmppJvm->GetEnv((void**)&env, JNI_VERSION_1_6);
    bool needDetach = false;
    if (attachResult == JNI_EDETACHED) {
        if (g_xmppJvm->AttachCurrentThread(&env, nullptr) != JNI_OK) {
            LOGE("Failed to attach thread to JVM");
            return;
        }
        needDetach = true;
    } else if (attachResult != JNI_OK) {
        LOGE("Failed to get JNI env");
        return;
    }

    jclass clazz = env->FindClass("chat/pcp/app/xmpp/XmppNative");
    if (!clazz) {
        LOGE("Failed to find XmppNative class");
        if (needDetach) g_xmppJvm->DetachCurrentThread();
        return;
    }

    jmethodID mid = env->GetStaticMethodID(clazz, methodName.c_str(), "(Ljava/lang/String;)V");
    if (!mid) {
        LOGE("Failed to find method %s", methodName.c_str());
        env->DeleteLocalRef(clazz);
        if (needDetach) g_xmppJvm->DetachCurrentThread();
        return;
    }

    jstring jjson = env->NewStringUTF(json.c_str());
    env->CallStaticVoidMethod(clazz, mid, jjson);
    env->DeleteLocalRef(jjson);
    env->DeleteLocalRef(clazz);

    if (needDetach) {
        g_xmppJvm->DetachCurrentThread();
    }
}

static std::string rosterToJson(const std::vector<pcp::XmppUser>& roster) {
    std::ostringstream os;
    os << "[";
    for (size_t i = 0; i < roster.size(); i++) {
        if (i > 0) os << ",";
        const auto& u = roster[i];
        os << R"({"jid":")" << pcp::escapeJson(u.jid)
           << R"(","nickname":")" << pcp::escapeJson(u.nickname)
           << R"(","status":")" << pcp::escapeJson(u.status)
           << R"(","avatarHash":")" << pcp::escapeJson(u.avatarHash)
           << R"(","isOnline":)" << (u.isOnline ? "true" : "false") << "}";
    }
    os << "]";
    return os.str();
}

static std::string chatsToJson(const std::vector<pcp::XmppChat>& chats) {
    std::ostringstream os;
    os << "[";
    for (size_t i = 0; i < chats.size(); i++) {
        if (i > 0) os << ",";
        const auto& c = chats[i];
        os << R"({"jid":")" << pcp::escapeJson(c.jid)
           << R"(","name":")" << pcp::escapeJson(c.name)
           << R"(","lastMessage":")" << pcp::escapeJson(c.lastMessage)
           << R"(,"lastMessageTime":)" << c.lastMessageTime
           << R"(,"unreadCount":)" << c.unreadCount
           << R"(,"isGroupChat":)" << (c.isGroupChat ? "true" : "false") << "}";
    }
    os << "]";
    return os.str();
}

// ====== JNI Functions ======

JNI_XMPP(jlong, nativeXmppInit)(JNIEnv* env, jclass) {
    env->GetJavaVM(&g_xmppJvm);
    LOGD("XMPP JNI initialized, JVM=%p", g_xmppJvm);
    return 0;
}

JNI_XMPP(jlong, nativeXmppConnect)(JNIEnv* env, jclass, jstring jAccountJson) {
    if (!g_xmppJvm) env->GetJavaVM(&g_xmppJvm);

    pcp::XmppAccount account;
    std::string json = jStr(env, jAccountJson);

    account.jid = pcp::parseJsonStringValue(json, "jid");
    account.password = pcp::parseJsonStringValue(json, "password");
    account.server = pcp::parseJsonStringValue(json, "server");
    account.resource = pcp::parseJsonStringValue(json, "resource");
    account.port = static_cast<int>(pcp::parseJsonInt64Value(json, "port", 5222));
    account.useTls = pcp::parseJsonBoolValue(json, "useTls", true);

    if (account.resource.empty()) account.resource = "PCP";

    auto client = std::make_unique<pcp::XmppClient>();
    client->setEventCallback([](pcp::XmppEventType type, const std::string& data) {
        switch (type) {
            case pcp::XmppEventType::CONNECTION_CHANGED:
                xmppPostToKotlin("xmppOnConnection", data);
                break;
            case pcp::XmppEventType::AUTH_CHANGED:
                xmppPostToKotlin("xmppOnConnection", data);
                break;
            case pcp::XmppEventType::MESSAGE_RECEIVED:
                xmppPostToKotlin("xmppOnMessage", data);
                break;
            case pcp::XmppEventType::MAM_MESSAGE:
                xmppPostToKotlin("xmppOnMamMessage", data);
                break;
            case pcp::XmppEventType::MAM_COMPLETE:
                xmppPostToKotlin("xmppOnMamComplete", data);
                break;
            case pcp::XmppEventType::PRESENCE_CHANGED:
                xmppPostToKotlin("xmppOnPresence", data);
                break;
            case pcp::XmppEventType::ROSTER_UPDATED:
                xmppPostToKotlin("xmppOnRoster", data);
                break;
            case pcp::XmppEventType::ERROR:
                xmppPostToKotlin("xmppOnError", data);
                break;
        }
    });

    if (!client->connect(account)) {
        LOGE("XMPP connect returned false");
        return 0;
    }

    std::lock_guard<std::mutex> lock(g_xmppMutex);
    int64_t handle = g_xmppNextHandle++;
    g_xmppClients[handle] = std::move(client);
    LOGD("XMPP client created, handle=%lld", (long long)handle);
    return handle;
}

JNI_XMPP(void, nativeXmppDisconnect)(JNIEnv*, jclass, jlong jHandle) {
    std::lock_guard<std::mutex> lock(g_xmppMutex);
    auto it = g_xmppClients.find(jHandle);
    if (it != g_xmppClients.end()) {
        it->second->disconnect();
        g_xmppClients.erase(it);
    }
}

JNI_XMPP(jboolean, nativeXmppIsConnected)(JNIEnv*, jclass, jlong jHandle) {
    std::lock_guard<std::mutex> lock(g_xmppMutex);
    auto it = g_xmppClients.find(jHandle);
    if (it != g_xmppClients.end()) {
        return it->second->isConnected();
    }
    return false;
}

JNI_XMPP(jboolean, nativeXmppIsAuthenticated)(JNIEnv*, jclass, jlong jHandle) {
    std::lock_guard<std::mutex> lock(g_xmppMutex);
    auto it = g_xmppClients.find(jHandle);
    if (it != g_xmppClients.end()) {
        return it->second->isAuthenticated();
    }
    return false;
}

JNI_XMPP(void, nativeXmppSend)(JNIEnv* env, jclass, jlong jHandle, jstring jTo, jstring jBody) {
    std::lock_guard<std::mutex> lock(g_xmppMutex);
    auto it = g_xmppClients.find(jHandle);
    if (it != g_xmppClients.end()) {
        it->second->sendMessage(jStr(env, jTo), jStr(env, jBody));
    }
}

JNI_XMPP(void, nativeXmppJoinRoom)(JNIEnv* env, jclass, jlong jHandle, jstring jRoomJid) {
    std::lock_guard<std::mutex> lock(g_xmppMutex);
    auto it = g_xmppClients.find(jHandle);
    if (it != g_xmppClients.end()) {
        it->second->joinRoom(jStr(env, jRoomJid));
    }
}

JNI_XMPP(void, nativeXmppLeaveRoom)(JNIEnv* env, jclass, jlong jHandle, jstring jRoomJid) {
    std::lock_guard<std::mutex> lock(g_xmppMutex);
    auto it = g_xmppClients.find(jHandle);
    if (it != g_xmppClients.end()) {
        it->second->leaveRoom(jStr(env, jRoomJid));
    }
}

JNI_XMPP(jstring, nativeXmppGetRoster)(JNIEnv* env, jclass, jlong jHandle) {
    std::lock_guard<std::mutex> lock(g_xmppMutex);
    auto it = g_xmppClients.find(jHandle);
    if (it != g_xmppClients.end()) {
        return env->NewStringUTF(rosterToJson(it->second->getRoster()).c_str());
    }
    return env->NewStringUTF("[]");
}

JNI_XMPP(jstring, nativeXmppGetChats)(JNIEnv* env, jclass, jlong jHandle) {
    std::lock_guard<std::mutex> lock(g_xmppMutex);
    auto it = g_xmppClients.find(jHandle);
    if (it != g_xmppClients.end()) {
        return env->NewStringUTF(chatsToJson(it->second->getChats()).c_str());
    }
    return env->NewStringUTF("[]");
}

JNI_XMPP(void, nativeXmppSendPresence)(JNIEnv* env, jclass, jlong jHandle, jstring jStatus, jstring jShow) {
    std::lock_guard<std::mutex> lock(g_xmppMutex);
    auto it = g_xmppClients.find(jHandle);
    if (it != g_xmppClients.end()) {
        it->second->sendPresence(jStr(env, jStatus), jStr(env, jShow));
    }
}

JNI_XMPP(jstring, nativeXmppGetOwnJid)(JNIEnv* env, jclass, jlong jHandle) {
    std::lock_guard<std::mutex> lock(g_xmppMutex);
    auto it = g_xmppClients.find(jHandle);
    if (it != g_xmppClients.end()) {
        return env->NewStringUTF(it->second->getOwnJid().c_str());
    }
    return env->NewStringUTF("");
}

JNI_XMPP(void, nativeXmppRunOnce)(JNIEnv*, jclass, jlong jHandle, jint jTimeoutMs) {
    std::lock_guard<std::mutex> lock(g_xmppMutex);
    auto it = g_xmppClients.find(jHandle);
    if (it != g_xmppClients.end()) {
        it->second->runOnce(jTimeoutMs);
    }
}

JNI_XMPP(void, nativeXmppSendTyping)(JNIEnv* env, jclass, jlong jHandle, jstring jTo, jboolean jIsTyping) {
    std::lock_guard<std::mutex> lock(g_xmppMutex);
    auto it = g_xmppClients.find(jHandle);
    if (it != g_xmppClients.end()) {
        it->second->sendTypingNotification(jStr(env, jTo), jIsTyping);
    }
}

JNI_XMPP(void, nativeXmppSetNickname)(JNIEnv* env, jclass, jlong jHandle, jstring jRoomJid, jstring jNick) {
    std::lock_guard<std::mutex> lock(g_xmppMutex);
    auto it = g_xmppClients.find(jHandle);
    if (it != g_xmppClients.end()) {
        it->second->setNickname(jStr(env, jRoomJid), jStr(env, jNick));
    }
}

JNI_XMPP(void, nativeXmppRequestMam)(JNIEnv* env, jclass, jlong jHandle, jstring jChatJid, jstring jBeforeId, jint jLimit) {
    std::lock_guard<std::mutex> lock(g_xmppMutex);
    auto it = g_xmppClients.find(jHandle);
    if (it != g_xmppClients.end()) {
        it->second->requestMam(jStr(env, jChatJid), jStr(env, jBeforeId), jLimit);
    }
}

JNI_XMPP(void, nativeXmppRequestMamPage)(JNIEnv* env, jclass, jlong jHandle, jstring jChatJid, jstring jAfterId, jint jLimit) {
    std::lock_guard<std::mutex> lock(g_xmppMutex);
    auto it = g_xmppClients.find(jHandle);
    if (it != g_xmppClients.end()) {
        it->second->requestMamPage(jStr(env, jChatJid), jStr(env, jAfterId), jLimit);
    }
}

JNI_XMPP(jstring, nativeXmppGetRooms)(JNIEnv* env, jclass, jlong jHandle) {
    std::lock_guard<std::mutex> lock(g_xmppMutex);
    auto it = g_xmppClients.find(jHandle);
    if (it != g_xmppClients.end()) {
        auto rooms = it->second->getRooms();
        std::ostringstream os;
        os << "[";
        for (size_t i = 0; i < rooms.size(); i++) {
            if (i > 0) os << ",";
            const auto& r = rooms[i];
            os << R"({"jid":")" << pcp::escapeJson(r.jid)
               << R"(","name":")" << pcp::escapeJson(r.name)
               << R"(","subject":")" << pcp::escapeJson(r.subject)
               << R"(","ownNickname":")" << pcp::escapeJson(r.ownNickname)
               << R"(","isJoined":)" << (r.isJoined ? "true" : "false")
               << R"(,"participantCount":)" << r.participants.size() << "}";
        }
        os << "]";
        return env->NewStringUTF(os.str().c_str());
    }
    return env->NewStringUTF("[]");
}

JNI_XMPP(void, nativeXmppRequestRoomInfo)(JNIEnv* env, jclass, jlong jHandle, jstring jRoomJid) {
    std::lock_guard<std::mutex> lock(g_xmppMutex);
    auto it = g_xmppClients.find(jHandle);
    if (it != g_xmppClients.end()) {
        it->second->requestRoomInfo(jStr(env, jRoomJid));
    }
}

JNI_XMPP(void, nativeXmppRequestVCard)(JNIEnv* env, jclass, jlong jHandle, jstring jJid) {
    std::lock_guard<std::mutex> lock(g_xmppMutex);
    auto it = g_xmppClients.find(jHandle);
    if (it != g_xmppClients.end()) {
        it->second->requestVCard(jStr(env, jJid));
    }
}

JNI_XMPP(jstring, nativeXmppGetAvatarHash)(JNIEnv* env, jclass, jlong jHandle, jstring jJid) {
    std::lock_guard<std::mutex> lock(g_xmppMutex);
    auto it = g_xmppClients.find(jHandle);
    if (it != g_xmppClients.end()) {
        return env->NewStringUTF(it->second->getAvatarHash(jStr(env, jJid)).c_str());
    }
    return env->NewStringUTF("");
}

JNI_XMPP(jstring, nativeXmppRequestUploadSlot)(JNIEnv* env, jclass, jlong jHandle, jstring jServiceJid,
    jstring jFilename, jlong jFileSize, jstring jContentType) {
    std::lock_guard<std::mutex> lock(g_xmppMutex);
    auto it = g_xmppClients.find(jHandle);
    if (it != g_xmppClients.end()) {
        it->second->requestUploadSlot(jStr(env, jServiceJid), jStr(env, jFilename),
                                       jFileSize, jStr(env, jContentType));
    }
    return env->NewStringUTF("{}");
}

#else // PCP_HAS_XMPP not defined — stubs

JNI_XMPP(jlong, nativeXmppInit)(JNIEnv*, jclass) {
    LOGE("XMPP support not compiled (PCP_HAS_XMPP not defined)");
    return 0;
}

JNI_XMPP(jlong, nativeXmppConnect)(JNIEnv*, jclass, jstring) {
    LOGE("XMPP support not compiled");
    return 0;
}

JNI_XMPP(void, nativeXmppDisconnect)(JNIEnv*, jclass, jlong) {
    LOGE("XMPP support not compiled");
}

JNI_XMPP(jboolean, nativeXmppIsConnected)(JNIEnv*, jclass, jlong) {
    return false;
}

JNI_XMPP(jboolean, nativeXmppIsAuthenticated)(JNIEnv*, jclass, jlong) {
    return false;
}

JNI_XMPP(void, nativeXmppSend)(JNIEnv*, jclass, jlong, jstring, jstring) {}

JNI_XMPP(void, nativeXmppJoinRoom)(JNIEnv*, jclass, jlong, jstring) {}

JNI_XMPP(void, nativeXmppLeaveRoom)(JNIEnv*, jclass, jlong, jstring) {}

JNI_XMPP(jstring, nativeXmppGetRoster)(JNIEnv* env, jclass, jlong) {
    return env->NewStringUTF("[]");
}

JNI_XMPP(jstring, nativeXmppGetChats)(JNIEnv* env, jclass, jlong) {
    return env->NewStringUTF("[]");
}

JNI_XMPP(void, nativeXmppSendPresence)(JNIEnv*, jclass, jlong, jstring, jstring) {}

JNI_XMPP(jstring, nativeXmppGetOwnJid)(JNIEnv* env, jclass, jlong) {
    return env->NewStringUTF("");
}

JNI_XMPP(void, nativeXmppRunOnce)(JNIEnv*, jclass, jlong, jint) {}

JNI_XMPP(void, nativeXmppSendTyping)(JNIEnv*, jclass, jlong, jstring, jboolean) {}

JNI_XMPP(void, nativeXmppSetNickname)(JNIEnv*, jclass, jlong, jstring, jstring) {}

JNI_XMPP(void, nativeXmppRequestMam)(JNIEnv*, jclass, jlong, jstring, jstring, jint) {}

JNI_XMPP(void, nativeXmppRequestMamPage)(JNIEnv*, jclass, jlong, jstring, jstring, jint) {}

JNI_XMPP(jstring, nativeXmppGetRooms)(JNIEnv* env, jclass, jlong) {
    return env->NewStringUTF("[]");
}

JNI_XMPP(void, nativeXmppRequestRoomInfo)(JNIEnv*, jclass, jlong, jstring) {}

JNI_XMPP(void, nativeXmppRequestVCard)(JNIEnv*, jclass, jlong, jstring) {}

JNI_XMPP(jstring, nativeXmppGetAvatarHash)(JNIEnv* env, jclass, jlong, jstring) {
    return env->NewStringUTF("");
}

JNI_XMPP(jstring, nativeXmppRequestUploadSlot)(JNIEnv* env, jclass, jlong, jstring,
    jstring, jlong, jstring) {
    return env->NewStringUTF("{}");
}

#endif // PCP_HAS_XMPP
