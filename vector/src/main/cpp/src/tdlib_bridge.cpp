#include <jni.h>
#include <string>
#include <android/log.h>

#ifdef PCP_HAS_TDLIB
#include "pcp/tdlib_bridge.hpp"
#endif

#define JNI_FUNC(ret, name) \
    JNIEXPORT ret JNICALL Java_chat_pcp_app_telegram_TdLibNative_##name

#define TAG "TdLibBridge"

#ifdef PCP_HAS_TDLIB

static void* get_client_ptr(JNIEnv* env, jlong ptr) {
    return reinterpret_cast<void*>(static_cast<intptr_t>(ptr));
}

static jlong ptr_to_jlong(void* ptr) {
    return static_cast<jlong>(reinterpret_cast<intptr_t>(ptr));
}

extern "C" {

JNI_FUNC(jlong, nativeTdCreateClient)(JNIEnv* env, jclass clazz) {
    void* client = td_json_client_create();
    __android_log_print(ANDROID_LOG_INFO, TAG, "TDLib client created: %p", client);
    return ptr_to_jlong(client);
}

JNI_FUNC(void, nativeTdSend)(JNIEnv* env, jclass clazz, jlong clientPtr, jstring request) {
    void* client = get_client_ptr(env, clientPtr);
    if (!client) return;

    const char* req = env->GetStringUTFChars(request, nullptr);
    td_json_client_send(client, req);
    env->ReleaseStringUTFChars(request, req);
}

JNI_FUNC(jstring, nativeTdReceive)(JNIEnv* env, jclass clazz, jlong clientPtr, jdouble timeout) {
    void* client = get_client_ptr(env, clientPtr);
    if (!client) return nullptr;

    const char* result = td_json_client_receive(client, timeout);
    if (!result || result[0] == '\0') return nullptr;

    return env->NewStringUTF(result);
}

JNI_FUNC(jstring, nativeTdExecute)(JNIEnv* env, jclass clazz, jlong clientPtr, jstring request) {
    void* client = get_client_ptr(env, clientPtr);
    if (!client) return nullptr;

    const char* req = env->GetStringUTFChars(request, nullptr);
    const char* result = td_json_client_execute(client, req);
    env->ReleaseStringUTFChars(request, req);

    if (!result || result[0] == '\0') return nullptr;
    return env->NewStringUTF(result);
}

JNI_FUNC(void, nativeTdDestroyClient)(JNIEnv* env, jclass clazz, jlong clientPtr) {
    void* client = get_client_ptr(env, clientPtr);
    if (!client) return;

    td_json_client_destroy(client);
    __android_log_print(ANDROID_LOG_INFO, TAG, "TDLib client destroyed: %p", client);
}

} // extern "C"

#else // !PCP_HAS_TDLIB — stubs

extern "C" {

JNI_FUNC(jlong, nativeTdCreateClient)(JNIEnv* env, jclass clazz) {
    __android_log_print(ANDROID_LOG_ERROR, TAG, "TDLib not compiled in. Set PCP_HAS_TDLIB=1");
    return 0;
}

JNI_FUNC(void, nativeTdSend)(JNIEnv* env, jclass clazz, jlong clientPtr, jstring request) {}
JNI_FUNC(jstring, nativeTdReceive)(JNIEnv* env, jclass clazz, jlong clientPtr, jdouble timeout) {
    return nullptr;
}
JNI_FUNC(jstring, nativeTdExecute)(JNIEnv* env, jclass clazz, jlong clientPtr, jstring request) {
    return nullptr;
}
JNI_FUNC(void, nativeTdDestroyClient)(JNIEnv* env, jclass clazz, jlong clientPtr) {}

} // extern "C"

#endif // PCP_HAS_TDLIB
