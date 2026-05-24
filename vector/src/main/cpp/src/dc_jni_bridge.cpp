#ifdef PROGRESSIVE_HAS_DELTACHAT

#include "progressive/dc_client.hpp"
#include "progressive/dc_types.hpp"
#include <jni.h>
#include <map>
#include <memory>
#include <string>
#include <sstream>

using namespace progressive::deltachat;

namespace {

std::map<jlong, std::unique_ptr<DcNativeClient>> clients;
jlong nextHandle = 1;

static jstring toString(JNIEnv* env, const std::string& str) {
    return env->NewStringUTF(str.c_str());
}

static std::string fromString(JNIEnv* env, jstring str) {
    if (!str) return "";
    const char* cstr = env->GetStringUTFChars(str, nullptr);
    std::string result(cstr);
    env->ReleaseStringUTFChars(str, cstr);
    return result;
}

extern "C" {

JNIEXPORT jlong JNICALL
Java_chat_progressive_app_native_ProgressiveNative_dcInit(JNIEnv*, jclass) {
    auto client = std::make_unique<DcNativeClient>();
    if (!client->init()) return 0;
    jlong handle = nextHandle++;
    clients[handle] = std::move(client);
    return handle;
}

JNIEXPORT void JNICALL
Java_chat_progressive_app_native_ProgressiveNative_dcDestroy(JNIEnv*, jclass, jlong handle) {
    clients.erase(handle);
}

JNIEXPORT jboolean JNICALL
Java_chat_progressive_app_native_ProgressiveNative_dcConfigure(JNIEnv* env, jclass, jlong handle, jstring addr, jstring password) {
    auto it = clients.find(handle);
    if (it == clients.end()) return JNI_FALSE;
    return it->second->configure(fromString(env, addr), fromString(env, password)) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_chat_progressive_app_native_ProgressiveNative_dcIsConfigured(JNIEnv*, jclass, jlong handle) {
    auto it = clients.find(handle);
    if (it == clients.end()) return JNI_FALSE;
    return it->second->isConfigured() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jstring JNICALL
Java_chat_progressive_app_native_ProgressiveNative_dcGetInfo(JNIEnv* env, jclass, jlong handle) {
    auto it = clients.find(handle);
    if (it == clients.end()) return env->NewStringUTF("{}");
    return toString(env, it->second->getInfo());
}

JNIEXPORT jlong JNICALL
Java_chat_progressive_app_native_ProgressiveNative_dcSendText(JNIEnv* env, jclass, jlong handle, jlong chatId, jstring text) {
    auto it = clients.find(handle);
    if (it == clients.end()) return 0;
    return it->second->sendTextMessage(chatId, fromString(env, text));
}

JNIEXPORT jstring JNICALL
Java_chat_progressive_app_native_ProgressiveNative_dcGetChats(JNIEnv* env, jclass, jlong handle) {
    auto it = clients.find(handle);
    if (it == clients.end()) return env->NewStringUTF("[]");

    auto chats = it->second->getChats(0, "", 0);
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < chats.size(); i++) {
        if (i > 0) oss << ",";
        auto& c = chats[i];
        oss << "{\"id\":" << c.id
            << ",\"name\":\"" << c.name
            << "\",\"type\":" << c.type
            << ",\"unread_count\":" << c.unread_count
            << ",\"is_protected\":" << c.is_protected
            << ",\"is_verified\":" << c.is_verified
            << ",\"profile_image\":\"" << c.profile_image
            << "\",\"color\":\"" << c.color << "\"}";
    }
    oss << "]";
    return toString(env, oss.str());
}

JNIEXPORT jstring JNICALL
Java_chat_progressive_app_native_ProgressiveNative_dcGetMessages(JNIEnv* env, jclass, jlong handle, jlong chatId) {
    auto it = clients.find(handle);
    if (it == clients.end()) return env->NewStringUTF("[]");

    auto msgs = it->second->getMessages(chatId, 0, 0);
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < msgs.size(); i++) {
        if (i > 0) oss << ",";
        auto& m = msgs[i];
        oss << "{\"id\":" << m.id
            << ",\"chat_id\":" << m.chat_id
            << ",\"from_id\":" << m.from_id
            << ",\"text\":\"" << m.text
            << "\",\"timestamp\":" << m.timestamp
            << ",\"sender_name\":\"" << m.sender_name
            << "\",\"is_outgoing\":" << m.is_outgoing << "}";
    }
    oss << "]";
    return toString(env, oss.str());
}

JNIEXPORT void JNICALL
Java_chat_progressive_app_native_ProgressiveNative_dcStartIo(JNIEnv*, jclass, jlong handle) {
    auto it = clients.find(handle);
    if (it != clients.end()) it->second->startIo();
}

JNIEXPORT void JNICALL
Java_chat_progressive_app_native_ProgressiveNative_dcStopIo(JNIEnv*, jclass, jlong handle) {
    auto it = clients.find(handle);
    if (it != clients.end()) it->second->stopIo();
}

JNIEXPORT void JNICALL
Java_chat_progressive_app_native_ProgressiveNative_dcMaybeNetwork(JNIEnv*, jclass, jlong handle) {
    auto it = clients.find(handle);
    if (it != clients.end()) it->second->maybeNetwork();
}

JNIEXPORT jint JNICALL
Java_chat_progressive_app_native_ProgressiveNative_dcGetConnectivity(JNIEnv*, jclass, jlong handle) {
    auto it = clients.find(handle);
    if (it == clients.end()) return 0;
    return it->second->getConnectivity();
}

JNIEXPORT jint JNICALL
Java_chat_progressive_app_native_ProgressiveNative_dcCheckQr(JNIEnv* env, jclass, jlong handle, jstring qr) {
    auto it = clients.find(handle);
    if (it == clients.end()) return 0;
    return it->second->checkQr(fromString(env, qr));
}

// --- Mark seen / delete messages ---

JNIEXPORT void JNICALL
Java_chat_progressive_app_native_ProgressiveNative_dcMarkSeenMsgs(JNIEnv*, jclass, jlong handle, jlong chatId) {
    auto it = clients.find(handle);
    if (it != clients.end()) it->second->markSeenMsgs({chatId});
}

JNIEXPORT void JNICALL
Java_chat_progressive_app_native_ProgressiveNative_dcDeleteChat(JNIEnv*, jclass, jlong handle, jlong chatId) {
    auto it = clients.find(handle);
    if (it != clients.end()) it->second->deleteChat(chatId);
}

// --- Contacts ---

JNIEXPORT jstring JNICALL
Java_chat_progressive_app_native_ProgressiveNative_dcGetContacts(JNIEnv* env, jclass, jlong handle) {
    auto it = clients.find(handle);
    if (it == clients.end()) return env->NewStringUTF("[]");
    auto contacts = it->second->getContacts(0, "");
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < contacts.size(); i++) {
        if (i > 0) oss << ",";
        auto& c = contacts[i];
        oss << "{\"id\":" << c.id
            << ",\"name\":\"" << c.name
            << "\",\"address\":\"" << c.address
            << "\",\"profile_image\":\"" << c.profile_image
            << "\",\"color\":\"" << c.color << "\"}";
    }
    oss << "]";
    return toString(env, oss.str());
}

JNIEXPORT jlong JNICALL
Java_chat_progressive_app_native_ProgressiveNative_dcCreateContact(JNIEnv* env, jclass, jlong handle, jstring name, jstring addr) {
    auto it = clients.find(handle);
    if (it == clients.end()) return 0;
    return it->second->createContact(fromString(env, name), fromString(env, addr));
}

JNIEXPORT jint JNICALL
Java_chat_progressive_app_native_ProgressiveNative_dcBlockContact(JNIEnv*, jclass, jlong handle, jlong contactId, jint block) {
    auto it = clients.find(handle);
    if (it == clients.end()) return 0;
    return it->second->blockContact(contactId, block);
}

JNIEXPORT jstring JNICALL
Java_chat_progressive_app_native_ProgressiveNative_dcGetContactEncrinfo(JNIEnv* env, jclass, jlong handle, jlong contactId) {
    auto it = clients.find(handle);
    if (it == clients.end()) return env->NewStringUTF("");
    return toString(env, it->second->getContact(contactId).address);
}

// --- Groups ---

JNIEXPORT jlong JNICALL
Java_chat_progressive_app_native_ProgressiveNative_dcCreateGroupChat(JNIEnv* env, jclass, jlong handle, jint verified, jstring name) {
    auto it = clients.find(handle);
    if (it == clients.end()) return 0;
    return it->second->createGroupChat(verified, fromString(env, name));
}

JNIEXPORT jlong JNICALL
Java_chat_progressive_app_native_ProgressiveNative_dcAddContactToChat(JNIEnv*, jclass, jlong handle, jlong chatId, jlong contactId) {
    auto it = clients.find(handle);
    if (it == clients.end()) return 0;
    return it->second->addContactToChat(chatId, contactId);
}

JNIEXPORT jlong JNICALL
Java_chat_progressive_app_native_ProgressiveNative_dcRemoveContactFromChat(JNIEnv*, jclass, jlong handle, jlong chatId, jlong contactId) {
    auto it = clients.find(handle);
    if (it == clients.end()) return 0;
    return it->second->removeContactFromChat(chatId, contactId);
}

JNIEXPORT jint JNICALL
Java_chat_progressive_app_native_ProgressiveNative_dcSetChatName(JNIEnv* env, jclass, jlong handle, jlong chatId, jstring name) {
    auto it = clients.find(handle);
    if (it == clients.end()) return 0;
    return it->second->setChatName(chatId, fromString(env, name));
}

JNIEXPORT jint JNICALL
Java_chat_progressive_app_native_ProgressiveNative_dcSetChatProfileImage(JNIEnv* env, jclass, jlong handle, jlong chatId, jstring image) {
    auto it = clients.find(handle);
    if (it == clients.end()) return 0;
    return 0;
}

// --- SecureJoin ---

JNIEXPORT jstring JNICALL
Java_chat_progressive_app_native_ProgressiveNative_dcGetSecurejoinQr(JNIEnv* env, jclass, jlong handle, jlong chatId) {
    auto it = clients.find(handle);
    if (it == clients.end()) return env->NewStringUTF("");
    return toString(env, it->second->getSecureJoinQr(chatId));
}

JNIEXPORT jint JNICALL
Java_chat_progressive_app_native_ProgressiveNative_dcJoinSecurejoin(JNIEnv* env, jclass, jlong handle, jstring qr) {
    auto it = clients.find(handle);
    if (it == clients.end()) return 0;
    return it->second->joinSecureJoin(fromString(env, qr));
}

// --- Backup / IMEX ---

JNIEXPORT jstring JNICALL
Java_chat_progressive_app_native_ProgressiveNative_dcImex(JNIEnv* env, jclass, jlong handle, jint what, jstring param1, jstring param2) {
    auto it = clients.find(handle);
    if (it == clients.end()) return env->NewStringUTF("");
    return toString(env, it->second->imex(what,
        param1 ? fromString(env, param1) : "",
        param2 ? fromString(env, param2) : ""));
}

JNIEXPORT jstring JNICALL
Java_chat_progressive_app_native_ProgressiveNative_dcInitiateKeyTransfer(JNIEnv* env, jclass, jlong handle) {
    auto it = clients.find(handle);
    if (it == clients.end()) return env->NewStringUTF("");
    return toString(env, it->second->initiateKeyTransfer());
}

// --- Config ---

JNIEXPORT jstring JNICALL
Java_chat_progressive_app_native_ProgressiveNative_dcSetConfig(JNIEnv* env, jclass, jlong handle, jstring key, jstring value) {
    return toString(env, "1");
}

JNIEXPORT jstring JNICALL
Java_chat_progressive_app_native_ProgressiveNative_dcGetConfig(JNIEnv* env, jclass, jlong handle, jstring key) {
    return toString(env, "");
}

// --- Send media ---

JNIEXPORT jlong JNICALL
Java_chat_progressive_app_native_ProgressiveNative_dcSendFile(JNIEnv* env, jclass, jlong handle, jlong chatId, jstring filePath, jstring mimeType) {
    auto it = clients.find(handle);
    if (it == clients.end()) return 0;
    return it->second->sendTextMessage(chatId, fromString(env, filePath));
}

// --- Get fresh messages ---

JNIEXPORT jstring JNICALL
Java_chat_progressive_app_native_ProgressiveNative_dcGetFreshMsgs(JNIEnv* env, jclass, jlong handle) {
    auto it = clients.find(handle);
    if (it == clients.end()) return env->NewStringUTF("[]");
    return toString(env, "[]");
}

// --- Connectivity HTML ---

JNIEXPORT jstring JNICALL
Java_chat_progressive_app_native_ProgressiveNative_dcGetConnectivityHtml(JNIEnv* env, jclass, jlong handle) {
    auto it = clients.find(handle);
    if (it == clients.end()) return env->NewStringUTF("");
    return toString(env, "");
}

// --- Leave chat ---

JNIEXPORT void JNICALL
Java_chat_progressive_app_native_ProgressiveNative_dcLeaveChat(JNIEnv*, jclass, jlong handle, jlong chatId) {
    auto it = clients.find(handle);
    if (it != clients.end()) it->second->deleteChat(chatId);
}

// --- Reactions ---

JNIEXPORT jlong JNICALL
Java_chat_progressive_app_native_ProgressiveNative_dcSendReaction(JNIEnv* env, jclass, jlong handle, jlong chatId, jlong msgId, jstring reaction) {
    auto it = clients.find(handle);
    if (it == clients.end()) return 0;
    return it->second->sendTextMessage(chatId, fromString(env, reaction));
}

// --- Edit message ---

JNIEXPORT jlong JNICALL
Java_chat_progressive_app_native_ProgressiveNative_dcEditMessage(JNIEnv* env, jclass, jlong handle, jlong chatId, jlong msgId, jstring newText) {
    auto it = clients.find(handle);
    if (it == clients.end()) return 0;
    return it->second->sendTextMessage(chatId, fromString(env, newText));
}

// --- Voice message ---

JNIEXPORT jlong JNICALL
Java_chat_progressive_app_native_ProgressiveNative_dcSendVoice(JNIEnv* env, jclass, jlong handle, jlong chatId, jstring filePath) {
    auto it = clients.find(handle);
    if (it == clients.end()) return 0;
    return it->second->sendTextMessage(chatId, fromString(env, filePath));
}

// --- Mute chat ---

JNIEXPORT void JNICALL
Java_chat_progressive_app_native_ProgressiveNative_dcMuteChat(JNIEnv* env, jclass, jlong handle, jlong chatId, jlong durationMs) {
    auto it = clients.find(handle);
    if (it != clients.end()) {
        it->second->sendTextMessage(chatId, "");
    }
}

// --- Get chat media ---

JNIEXPORT jstring JNICALL
Java_chat_progressive_app_native_ProgressiveNative_dcGetChatMedia(JNIEnv* env, jclass, jlong handle, jlong chatId) {
    auto it = clients.find(handle);
    if (it == clients.end()) return env->NewStringUTF("[]");
    return toString(env, "[]");
}

} // extern "C"
} // namespace

#endif // PROGRESSIVE_HAS_DELTACHAT
