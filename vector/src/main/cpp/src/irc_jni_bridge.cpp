#include <jni.h>
#include <string>
#include <memory>
#include <mutex>
#include <map>
#include "progressive/irc_client.hpp"
#include "progressive/irc_parser.hpp"

using namespace progressive::irc;

static std::map<jlong, std::unique_ptr<IrcClient>> clients;
static std::mutex clientMutex;
static jlong nextClientId = 1;

static IrcClient* getClient(jlong clientId) {
    std::lock_guard<std::mutex> lock(clientMutex);
    auto it = clients.find(clientId);
    return it != clients.end() ? it->second.get() : nullptr;
}

static std::string jstringToString(JNIEnv* env, jstring str) {
    if (!str) return "";
    const char* utf = env->GetStringUTFChars(str, nullptr);
    std::string result(utf);
    env->ReleaseStringUTFChars(str, utf);
    return result;
}

static std::mutex jniCallbackMutex;
static JavaVM* gJvm = nullptr;
static jobject gCallbackObj = nullptr;
static jmethodID gOnMessageMethod = nullptr;
static jmethodID gOnStateMethod = nullptr;
static jmethodID gOnErrorMethod = nullptr;

extern "C" JNIEXPORT jlong JNICALL
Java_chat_progressive_app_irc_IrcNative_nativeIrcCreateClient(JNIEnv* env, jclass) {
    auto client = std::make_unique<IrcClient>();

    jlong id = nextClientId++;

    client->setMessageCallback([id](const IrcMessage& msg) {
        JNIEnv* env = nullptr;
        bool attached = false;
        if (gJvm) {
            jint getEnvResult = gJvm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);
            if (getEnvResult == JNI_EDETACHED) {
                gJvm->AttachCurrentThread(&env, nullptr);
                attached = true;
            }
            if (env && gCallbackObj && gOnMessageMethod) {
                jstring json = env->NewStringUTF(msg.toJson().c_str());
                env->CallVoidMethod(gCallbackObj, gOnMessageMethod, id, json);
                env->DeleteLocalRef(json);
            }
            if (attached) gJvm->DetachCurrentThread();
        }
    });

    client->setStateCallback([id](IrcConnectionState st) {
        JNIEnv* env = nullptr;
        bool attached = false;
        if (gJvm) {
            jint getEnvResult = gJvm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);
            if (getEnvResult == JNI_EDETACHED) {
                gJvm->AttachCurrentThread(&env, nullptr);
                attached = true;
            }
            if (env && gCallbackObj && gOnStateMethod) {
                env->CallVoidMethod(gCallbackObj, gOnStateMethod, id, static_cast<jint>(st));
            }
            if (attached) gJvm->DetachCurrentThread();
        }
    });

    client->setErrorCallback([id](int code, const std::string& msg) {
        JNIEnv* env = nullptr;
        bool attached = false;
        if (gJvm) {
            jint getEnvResult = gJvm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);
            if (getEnvResult == JNI_EDETACHED) {
                gJvm->AttachCurrentThread(&env, nullptr);
                attached = true;
            }
            if (env && gCallbackObj && gOnErrorMethod) {
                jstring jmsg = env->NewStringUTF(msg.c_str());
                env->CallVoidMethod(gCallbackObj, gOnErrorMethod, id, code, jmsg);
                env->DeleteLocalRef(jmsg);
            }
            if (attached) gJvm->DetachCurrentThread();
        }
    });

    std::lock_guard<std::mutex> lock(clientMutex);
    clients[id] = std::move(client);
    return id;
}

extern "C" JNIEXPORT void JNICALL
Java_chat_progressive_app_irc_IrcNative_nativeIrcConnect(JNIEnv* env, jclass, jlong clientId,
                                                         jstring host, jint port, jboolean tls,
                                                         jstring nick, jstring user, jstring realName,
                                                         jstring password) {
    auto* client = getClient(clientId);
    if (!client) return;

    IrcServerConfig cfg;
    cfg.host = jstringToString(env, host);
    cfg.port = port;
    cfg.tls = tls;
    cfg.nick = jstringToString(env, nick);
    cfg.user = jstringToString(env, user);
    cfg.realName = jstringToString(env, realName);
    cfg.password = jstringToString(env, password);
    client->connect(cfg);
}

extern "C" JNIEXPORT void JNICALL
Java_chat_progressive_app_irc_IrcNative_nativeIrcDisconnect(JNIEnv*, jclass, jlong clientId) {
    auto* client = getClient(clientId);
    if (client) client->disconnect();
}

extern "C" JNIEXPORT void JNICALL
Java_chat_progressive_app_irc_IrcNative_nativeIrcSendRaw(JNIEnv* env, jclass, jlong clientId, jstring line) {
    auto* client = getClient(clientId);
    if (client) client->sendRaw(jstringToString(env, line));
}

extern "C" JNIEXPORT void JNICALL
Java_chat_progressive_app_irc_IrcNative_nativeIrcJoin(JNIEnv* env, jclass, jlong clientId, jstring channel) {
    auto* client = getClient(clientId);
    if (client) client->join(jstringToString(env, channel));
}

extern "C" JNIEXPORT void JNICALL
Java_chat_progressive_app_irc_IrcNative_nativeIrcPart(JNIEnv* env, jclass, jlong clientId, jstring channel, jstring reason) {
    auto* client = getClient(clientId);
    if (client) client->part(jstringToString(env, channel), jstringToString(env, reason));
}

extern "C" JNIEXPORT void JNICALL
Java_chat_progressive_app_irc_IrcNative_nativeIrcPrivmsg(JNIEnv* env, jclass, jlong clientId, jstring target, jstring text) {
    auto* client = getClient(clientId);
    if (client) client->privmsg(jstringToString(env, target), jstringToString(env, text));
}

extern "C" JNIEXPORT void JNICALL
Java_chat_progressive_app_irc_IrcNative_nativeIrcNotice(JNIEnv* env, jclass, jlong clientId, jstring target, jstring text) {
    auto* client = getClient(clientId);
    if (client) client->notice(jstringToString(env, target), jstringToString(env, text));
}

extern "C" JNIEXPORT void JNICALL
Java_chat_progressive_app_irc_IrcNative_nativeIrcNick(JNIEnv* env, jclass, jlong clientId, jstring newNick) {
    auto* client = getClient(clientId);
    if (client) client->nick(jstringToString(env, newNick));
}

extern "C" JNIEXPORT void JNICALL
Java_chat_progressive_app_irc_IrcNative_nativeIrcTopic(JNIEnv* env, jclass, jlong clientId, jstring channel, jstring newTopic) {
    auto* client = getClient(clientId);
    if (client) client->topic(jstringToString(env, channel), jstringToString(env, newTopic));
}

extern "C" JNIEXPORT void JNICALL
Java_chat_progressive_app_irc_IrcNative_nativeIrcKick(JNIEnv* env, jclass, jlong clientId, jstring channel, jstring nick, jstring reason) {
    auto* client = getClient(clientId);
    if (client) client->kick(jstringToString(env, channel), jstringToString(env, nick), jstringToString(env, reason));
}

extern "C" JNIEXPORT void JNICALL
Java_chat_progressive_app_irc_IrcNative_nativeIrcDestroyClient(JNIEnv*, jclass, jlong clientId) {
    std::lock_guard<std::mutex> lock(clientMutex);
    auto it = clients.find(clientId);
    if (it != clients.end()) {
        it->second->disconnect();
        clients.erase(it);
    }
}

extern "C" JNIEXPORT void JNICALL
Java_chat_progressive_app_irc_IrcNative_nativeIrcSetCallbackObject(JNIEnv* env, jclass, jobject callback) {
    std::lock_guard<std::mutex> lock(jniCallbackMutex);

    if (gCallbackObj) env->DeleteGlobalRef(gCallbackObj);
    gCallbackObj = env->NewGlobalRef(callback);

    jclass clazz = env->GetObjectClass(gCallbackObj);
    gOnMessageMethod = env->GetMethodID(clazz, "onIrcMessage", "(JLjava/lang/String;)V");
    gOnStateMethod = env->GetMethodID(clazz, "onIrcState", "(JI)V");
    gOnErrorMethod = env->GetMethodID(clazz, "onIrcError", "(JILjava/lang/String;)V");

    if (gJvm == nullptr) {
        env->GetJavaVM(&gJvm);
    }
}
