#include <android_native_app_glue.h>
#include <android/log.h>
#include <android/native_window.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <jni.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <functional>

#define LOG_TAG "NativeActivity"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// ── Asset Manager wrapper ──
static AAssetManager* gAssetManager = nullptr;

extern "C" {

JNIEXPORT void JNICALL
Java_chat_pcp_app_MainActivity_nativeSetAssetManager(JNIEnv* env, jobject thiz, jobject assetManager) {
    gAssetManager = AAssetManager_fromJava(env, assetManager);
}

} // extern "C"

// ── Native Activity helpers ──

namespace native_activity {

struct ActivityState {
    bool windowReady = false;
    bool resumed = false;
    bool focused = false;
    int32_t width = 0;
    int32_t height = 0;
    float density = 1.0f;

    // Lifecycle callbacks
    std::vector<std::function<void()>> onResumeCallbacks;
    std::vector<std::function<void()>> onPauseCallbacks;
    std::vector<std::function<void(int32_t, int32_t)>> onResizeCallbacks;
    std::vector<std::function<void()>> onDestroyCallbacks;

    std::mutex mutex;
};

static ActivityState gState;

bool isWindowReady() {
    std::lock_guard<std::mutex> lock(gState.mutex);
    return gState.windowReady;
}

bool isResumed() {
    std::lock_guard<std::mutex> lock(gState.mutex);
    return gState.resumed;
}

int32_t getWidth() {
    std::lock_guard<std::mutex> lock(gState.mutex);
    return gState.width;
}

int32_t getHeight() {
    std::lock_guard<std::mutex> lock(gState.mutex);
    return gState.height;
}

float getDensity() {
    std::lock_guard<std::mutex> lock(gState.mutex);
    return gState.density;
}

void addResumeCallback(std::function<void()> cb) {
    std::lock_guard<std::mutex> lock(gState.mutex);
    gState.onResumeCallbacks.push_back(std::move(cb));
}

void addPauseCallback(std::function<void()> cb) {
    std::lock_guard<std::mutex> lock(gState.mutex);
    gState.onPauseCallbacks.push_back(std::move(cb));
}

void addResizeCallback(std::function<void(int32_t, int32_t)> cb) {
    std::lock_guard<std::mutex> lock(gState.mutex);
    gState.onResizeCallbacks.push_back(std::move(cb));
}

void addDestroyCallback(std::function<void()> cb) {
    std::lock_guard<std::mutex> lock(gState.mutex);
    gState.onDestroyCallbacks.push_back(std::move(cb));
}

void setWindowReady(bool ready, int32_t w, int32_t h) {
    std::lock_guard<std::mutex> lock(gState.mutex);
    gState.windowReady = ready;
    if (ready) {
        gState.width = w;
        gState.height = h;
        for (auto& cb : gState.onResizeCallbacks) cb(w, h);
    }
}

void setResumed(bool resumed) {
    std::lock_guard<std::mutex> lock(gState.mutex);
    gState.resumed = resumed;
    if (resumed) {
        for (auto& cb : gState.onResumeCallbacks) cb();
    } else {
        for (auto& cb : gState.onPauseCallbacks) cb();
    }
}

void onDestroy() {
    std::lock_guard<std::mutex> lock(gState.mutex);
    for (auto& cb : gState.onDestroyCallbacks) cb();
    gState.onDestroyCallbacks.clear();
}

void setDisplayMetrics(float dpi) {
    std::lock_guard<std::mutex> lock(gState.mutex);
    gState.density = dpi / 160.0f;
}

// ── Asset file reading ──
std::vector<uint8_t> readAsset(const std::string& path) {
    if (!gAssetManager) return {};

    AAsset* asset = AAssetManager_open(gAssetManager, path.c_str(), AASSET_MODE_BUFFER);
    if (!asset) {
        LOGE("Failed to open asset: %s", path.c_str());
        return {};
    }

    off_t length = AAsset_getLength(asset);
    std::vector<uint8_t> data(length);

    int bytesRead = AAsset_read(asset, data.data(), length);
    AAsset_close(asset);

    if (bytesRead != length) {
        LOGE("Failed to read asset: %s", path.c_str());
        return {};
    }

    return data;
}

std::string readAssetString(const std::string& path) {
    auto data = readAsset(path);
    if (data.empty()) return "";
    return std::string(data.begin(), data.end());
}

// ── JNI helper for calling back into Java ──
static JavaVM* gJvm = nullptr;
static jclass gMainActivityClass = nullptr;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void*) {
    gJvm = vm;
    return JNI_VERSION_1_6;
}

void setMainActivityClass(jclass cls) {
    gMainActivityClass = reinterpret_cast<jclass>(
        gJvm ? reinterpret_cast<void*>( // Use global ref via env
            []() -> jobject {
                JNIEnv* env;
                gJvm->GetEnv((void**)&env, JNI_VERSION_1_6);
                return nullptr;
            }()
        ) : nullptr
    );
}

JNIEnv* getJniEnv() {
    if (!gJvm) return nullptr;
    JNIEnv* env = nullptr;
    gJvm->GetEnv((void**)&env, JNI_VERSION_1_6);
    if (!env) {
        gJvm->AttachCurrentThread(&env, nullptr);
    }
    return env;
}

// ── Show Android toast (for debugging) ──
void showToast(const std::string& message) {
    JNIEnv* env = getJniEnv();
    if (!env || !gMainActivityClass) return;

    // This would call Toast.makeText but requires more JNI setup
    LOGI("Toast: %s", message.c_str());
}

// ── Open URL in browser ──
void openUrl(const std::string& url) {
    LOGI("Opening URL: %s", url.c_str());
}

// ── Share text ──
void shareText(const std::string& text, const std::string& title) {
    LOGI("Share: %s - %s", title.c_str(), text.c_str());
}

} // namespace native_activity
