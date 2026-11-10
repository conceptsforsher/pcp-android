#include "android_bridge.hpp"
#include <android/log.h>
#include <cstdarg>
#include <cstring>
#include <mutex>

#define LOG_TAG "AndroidBridge"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)

static std::mutex gBridgeMutex;

AndroidBridge& AndroidBridge::instance() {
    static AndroidBridge bridge;
    return bridge;
}

void AndroidBridge::initialize(JavaVM* vm, jobject mainActivity) {
    std::lock_guard<std::mutex> lock(gBridgeMutex);
    m_jvm = vm;
    if (mainActivity && vm) {
        JNIEnv* env = getEnv();
        if (env) {
            m_mainActivity = env->NewGlobalRef(mainActivity);
        }
    }
    LOGI("AndroidBridge initialized");
}

JNIEnv* AndroidBridge::getEnv() {
    if (!m_jvm) return nullptr;
    JNIEnv* env = nullptr;
    jint result = m_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);
    if (result == JNI_EDETACHED) {
        result = m_jvm->AttachCurrentThread(&env, nullptr);
    }
    if (result != JNI_OK) {
        LOGE("Failed to get JNIEnv: %d", result);
        return nullptr;
    }
    return env;
}

// ── JNI helper: find our bridge class ──
static jclass findBridgeClass(JNIEnv* env) {
    if (!env) return nullptr;
    jclass cls = env->FindClass("chat/pcp/app/AndroidBridgeHelper");
    if (!cls) {
        LOGE("AndroidBridgeHelper class not found!");
    }
    return cls;
}

// ── Notifications ──

void AndroidBridge::showNotification(const std::string& channelId, int32_t id,
                                      const std::string& title, const std::string& body,
                                      const std::string& iconPath) {
    JNIEnv* env = getEnv();
    if (!env) return;

    jclass cls = findBridgeClass(env);
    if (!cls) return;

    jmethodID method = env->GetStaticMethodID(cls, "showNotification",
        "(Ljava/lang/String;ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
    if (!method) return;

    jstring jChannelId = env->NewStringUTF(channelId.c_str());
    jstring jTitle = env->NewStringUTF(title.c_str());
    jstring jBody = env->NewStringUTF(body.c_str());
    jstring jIcon = env->NewStringUTF(iconPath.c_str());

    env->CallStaticVoidMethod(cls, method, jChannelId, id, jTitle, jBody, jIcon);

    env->DeleteLocalRef(jChannelId);
    env->DeleteLocalRef(jTitle);
    env->DeleteLocalRef(jBody);
    env->DeleteLocalRef(jIcon);
    env->DeleteLocalRef(cls);
}

void AndroidBridge::cancelNotification(int32_t id) {
    JNIEnv* env = getEnv();
    if (!env) return;
    jclass cls = findBridgeClass(env);
    if (!cls) return;
    jmethodID method = env->GetStaticMethodID(cls, "cancelNotification", "(I)V");
    if (method) {
        env->CallStaticVoidMethod(cls, method, id);
    }
    env->DeleteLocalRef(cls);
}

void AndroidBridge::createNotificationChannel(const std::string& id, const std::string& name,
                                               const std::string& description, int importance) {
    JNIEnv* env = getEnv();
    if (!env) return;
    jclass cls = findBridgeClass(env);
    if (!cls) return;
    jmethodID method = env->GetStaticMethodID(cls, "createNotificationChannel",
        "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)V");
    if (!method) return;

    jstring jId = env->NewStringUTF(id.c_str());
    jstring jName = env->NewStringUTF(name.c_str());
    jstring jDesc = env->NewStringUTF(description.c_str());

    env->CallStaticVoidMethod(cls, method, jId, jName, jDesc, importance);

    env->DeleteLocalRef(jId);
    env->DeleteLocalRef(jName);
    env->DeleteLocalRef(jDesc);
    env->DeleteLocalRef(cls);
}

// ── Clipboard ──

std::string AndroidBridge::getClipboardText() {
    JNIEnv* env = getEnv();
    if (!env) return "";
    jclass cls = findBridgeClass(env);
    if (!cls) return "";
    jmethodID method = env->GetStaticMethodID(cls, "getClipboardText", "()Ljava/lang/String;");
    if (!method) { env->DeleteLocalRef(cls); return ""; }

    jstring result = (jstring)env->CallStaticObjectMethod(cls, method);
    std::string text;
    if (result) {
        const char* chars = env->GetStringUTFChars(result, nullptr);
        text = chars ? chars : "";
        env->ReleaseStringUTFChars(result, chars);
        env->DeleteLocalRef(result);
    }
    env->DeleteLocalRef(cls);
    return text;
}

void AndroidBridge::setClipboardText(const std::string& text) {
    JNIEnv* env = getEnv();
    if (!env) return;
    jclass cls = findBridgeClass(env);
    if (!cls) return;
    jmethodID method = env->GetStaticMethodID(cls, "setClipboardText", "(Ljava/lang/String;)V");
    if (!method) { env->DeleteLocalRef(cls); return; }

    jstring jText = env->NewStringUTF(text.c_str());
    env->CallStaticVoidMethod(cls, method, jText);
    env->DeleteLocalRef(jText);
    env->DeleteLocalRef(cls);
}

// ── Permissions ──

bool AndroidBridge::hasPermission(const std::string& permission) {
    JNIEnv* env = getEnv();
    if (!env) return false;
    jclass cls = findBridgeClass(env);
    if (!cls) return false;
    jmethodID method = env->GetStaticMethodID(cls, "hasPermission", "(Ljava/lang/String;)Z");
    if (!method) { env->DeleteLocalRef(cls); return false; }

    jstring jPerm = env->NewStringUTF(permission.c_str());
    bool result = env->CallStaticBooleanMethod(cls, method, jPerm);
    env->DeleteLocalRef(jPerm);
    env->DeleteLocalRef(cls);
    return result;
}

void AndroidBridge::requestPermission(const std::string& permission,
                                       std::function<void(bool)> callback) {
    // Store callback and request via Java
    m_permissionResultCallback = [this, permission, callback](const std::string& perm, bool granted) {
        if (perm == permission) {
            callback(granted);
        }
    };

    JNIEnv* env = getEnv();
    if (!env) return;
    jclass cls = findBridgeClass(env);
    if (!cls) return;
    jmethodID method = env->GetStaticMethodID(cls, "requestPermission", "(Ljava/lang/String;)V");
    if (!method) { env->DeleteLocalRef(cls); return; }

    jstring jPerm = env->NewStringUTF(permission.c_str());
    env->CallStaticVoidMethod(cls, method, jPerm);
    env->DeleteLocalRef(jPerm);
    env->DeleteLocalRef(cls);
}

// ── Network ──

bool AndroidBridge::isNetworkAvailable() {
    return callBoolMethod("isNetworkAvailable", "()Z");
}

std::string AndroidBridge::getNetworkType() {
    return callStringMethod("getNetworkType", "()Ljava/lang/String;");
}

// ── Device info ──

std::string AndroidBridge::getDeviceId() {
    return callStringMethod("getDeviceId", "()Ljava/lang/String;");
}

std::string AndroidBridge::getDeviceModel() {
    return callStringMethod("getDeviceModel", "()Ljava/lang/String;");
}

std::string AndroidBridge::getOSVersion() {
    return callStringMethod("getOSVersion", "()Ljava/lang/String;");
}

std::string AndroidBridge::getAppVersion() {
    return callStringMethod("getAppVersion", "()Ljava/lang/String;");
}

// ── Battery ──

float AndroidBridge::getBatteryLevel() {
    return callFloatMethod("getBatteryLevel", "()F");
}

bool AndroidBridge::isCharging() {
    return callBoolMethod("isCharging", "()Z");
}

// ── Storage ──

std::string AndroidBridge::getInternalStoragePath() {
    return callStringMethod("getInternalStoragePath", "()Ljava/lang/String;");
}

std::string AndroidBridge::getExternalStoragePath() {
    return callStringMethod("getExternalStoragePath", "()Ljava/lang/String;");
}

int64_t AndroidBridge::getAvailableStorageBytes() {
    return callLongMethod("getAvailableStorageBytes", "()J");
}

// ── UI Helpers ──

void AndroidBridge::setStatusBarColor(uint32_t argb) {
    callVoidMethod("setStatusBarColor", "(I)V", argb);
}

void AndroidBridge::setNavigationBarColor(uint32_t argb) {
    callVoidMethod("setNavigationBarColor", "(I)V", argb);
}

void AndroidBridge::keepScreenOn(bool on) {
    callVoidMethod("keepScreenOn", "(Z)V", on);
}

void AndroidBridge::showToast(const std::string& message) {
    JNIEnv* env = getEnv();
    if (!env) return;
    jclass cls = findBridgeClass(env);
    if (!cls) return;
    jmethodID method = env->GetStaticMethodID(cls, "showToast", "(Ljava/lang/String;)V");
    if (!method) { env->DeleteLocalRef(cls); return; }

    jstring jMsg = env->NewStringUTF(message.c_str());
    env->CallStaticVoidMethod(cls, method, jMsg);
    env->DeleteLocalRef(jMsg);
    env->DeleteLocalRef(cls);
}

void AndroidBridge::shareText(const std::string& text, const std::string& title) {
    JNIEnv* env = getEnv();
    if (!env) return;
    jclass cls = findBridgeClass(env);
    if (!cls) return;
    jmethodID method = env->GetStaticMethodID(cls, "shareText",
        "(Ljava/lang/String;Ljava/lang/String;)V");
    if (!method) { env->DeleteLocalRef(cls); return; }

    jstring jText = env->NewStringUTF(text.c_str());
    jstring jTitle = env->NewStringUTF(title.c_str());
    env->CallStaticVoidMethod(cls, method, jText, jTitle);
    env->DeleteLocalRef(jText);
    env->DeleteLocalRef(jTitle);
    env->DeleteLocalRef(cls);
}

void AndroidBridge::shareFile(const std::string& filePath, const std::string& mimeType) {
    JNIEnv* env = getEnv();
    if (!env) return;
    jclass cls = findBridgeClass(env);
    if (!cls) return;
    jmethodID method = env->GetStaticMethodID(cls, "shareFile",
        "(Ljava/lang/String;Ljava/lang/String;)V");
    if (!method) { env->DeleteLocalRef(cls); return; }

    jstring jPath = env->NewStringUTF(filePath.c_str());
    jstring jMime = env->NewStringUTF(mimeType.c_str());
    env->CallStaticVoidMethod(cls, method, jPath, jMime);
    env->DeleteLocalRef(jPath);
    env->DeleteLocalRef(jMime);
    env->DeleteLocalRef(cls);
}

void AndroidBridge::vibrate(int64_t milliseconds) {
    callVoidMethod("vibrate", "(J)V", milliseconds);
}

void AndroidBridge::playNotificationSound(const std::string& uri) {
    callVoidMethod("playNotificationSound", "(Ljava/lang/String;)V", uri.c_str());
}

// ── Callback setters ──

void AndroidBridge::setActivityResultCallback(ActivityResultCallback cb) {
    m_activityResultCallback = std::move(cb);
}

void AndroidBridge::setPermissionResultCallback(PermissionResultCallback cb) {
    m_permissionResultCallback = std::move(cb);
}

// ── Private JNI helpers ──

void AndroidBridge::callVoidMethod(const char* methodName, const char* sig, ...) {
    JNIEnv* env = getEnv();
    if (!env) return;
    jclass cls = findBridgeClass(env);
    if (!cls) return;
    jmethodID method = env->GetStaticMethodID(cls, methodName, sig);
    if (!method) { env->DeleteLocalRef(cls); return; }

    va_list args;
    va_start(args, sig);
    env->CallStaticVoidMethodV(cls, method, args);
    va_end(args);
    env->DeleteLocalRef(cls);
}

bool AndroidBridge::callBoolMethod(const char* methodName, const char* sig, ...) {
    JNIEnv* env = getEnv();
    if (!env) return false;
    jclass cls = findBridgeClass(env);
    if (!cls) return false;
    jmethodID method = env->GetStaticMethodID(cls, methodName, sig);
    if (!method) { env->DeleteLocalRef(cls); return false; }

    va_list args;
    va_start(args, sig);
    bool result = env->CallStaticBooleanMethodV(cls, method, args);
    va_end(args);
    env->DeleteLocalRef(cls);
    return result;
}

std::string AndroidBridge::callStringMethod(const char* methodName, const char* sig, ...) {
    JNIEnv* env = getEnv();
    if (!env) return "";
    jclass cls = findBridgeClass(env);
    if (!cls) return "";
    jmethodID method = env->GetStaticMethodID(cls, methodName, sig);
    if (!method) { env->DeleteLocalRef(cls); return ""; }

    va_list args;
    va_start(args, sig);
    jstring result = (jstring)env->CallStaticObjectMethodV(cls, method, args);
    va_end(args);

    std::string text;
    if (result) {
        const char* chars = env->GetStringUTFChars(result, nullptr);
        text = chars ? chars : "";
        env->ReleaseStringUTFChars(result, chars);
        env->DeleteLocalRef(result);
    }
    env->DeleteLocalRef(cls);
    return text;
}

int32_t AndroidBridge::callIntMethod(const char* methodName, const char* sig, ...) {
    JNIEnv* env = getEnv();
    if (!env) return 0;
    jclass cls = findBridgeClass(env);
    if (!cls) return 0;
    jmethodID method = env->GetStaticMethodID(cls, methodName, sig);
    if (!method) { env->DeleteLocalRef(cls); return 0; }

    va_list args;
    va_start(args, sig);
    int32_t result = env->CallStaticIntMethodV(cls, method, args);
    va_end(args);
    env->DeleteLocalRef(cls);
    return result;
}

int64_t AndroidBridge::callLongMethod(const char* methodName, const char* sig, ...) {
    JNIEnv* env = getEnv();
    if (!env) return 0;
    jclass cls = findBridgeClass(env);
    if (!cls) return 0;
    jmethodID method = env->GetStaticMethodID(cls, methodName, sig);
    if (!method) { env->DeleteLocalRef(cls); return 0; }

    va_list args;
    va_start(args, sig);
    int64_t result = env->CallStaticLongMethodV(cls, method, args);
    va_end(args);
    env->DeleteLocalRef(cls);
    return result;
}

float AndroidBridge::callFloatMethod(const char* methodName, const char* sig, ...) {
    JNIEnv* env = getEnv();
    if (!env) return 0.0f;
    jclass cls = findBridgeClass(env);
    if (!cls) return 0.0f;
    jmethodID method = env->GetStaticMethodID(cls, methodName, sig);
    if (!method) { env->DeleteLocalRef(cls); return 0.0f; }

    va_list args;
    va_start(args, sig);
    float result = env->CallStaticFloatMethodV(cls, method, args);
    va_end(args);
    env->DeleteLocalRef(cls);
    return result;
}
