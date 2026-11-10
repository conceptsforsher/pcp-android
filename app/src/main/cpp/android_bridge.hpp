#pragma once

#include <jni.h>
#include <string>
#include <functional>
#include <memory>
#include <vector>
#include <unordered_map>

// ── Android JNI Bridge ──
// Provides C++ ↔ Java interop for PCP
// All Android-specific platform APIs are accessed through this bridge.

class AndroidBridge {
public:
    static AndroidBridge& instance();

    void initialize(JavaVM* vm, jobject mainActivity);

    // ── Notifications ──
    void showNotification(const std::string& channelId, int32_t id,
                          const std::string& title, const std::string& body,
                          const std::string& iconPath = "");
    void cancelNotification(int32_t id);
    void createNotificationChannel(const std::string& id, const std::string& name,
                                    const std::string& description, int importance = 3);

    // ── Clipboard ──
    std::string getClipboardText();
    void setClipboardText(const std::string& text);

    // ── File picker ──
    void pickImage(std::function<void(const std::string& path)> callback);
    void pickFile(const std::string& mimeType,
                  std::function<void(const std::string& path)> callback);

    // ── Media ──
    void playNotificationSound(const std::string& uri);
    void vibrate(int64_t milliseconds);

    // ── Permissions ──
    bool hasPermission(const std::string& permission);
    void requestPermission(const std::string& permission,
                           std::function<void(bool granted)> callback);

    // ── Network ──
    bool isNetworkAvailable();
    std::string getNetworkType(); // "wifi", "mobile", "ethernet", "none"

    // ── Device info ──
    std::string getDeviceId();
    std::string getDeviceModel();
    std::string getOSVersion();
    std::string getAppVersion();

    // ── Battery ──
    float getBatteryLevel();
    bool isCharging();

    // ── Storage ──
    std::string getInternalStoragePath();
    std::string getExternalStoragePath();
    int64_t getAvailableStorageBytes();

    // ── Camera ──
    void openCamera(std::function<void(const std::string& path)> callback);

    // ── Share sheet ──
    void shareText(const std::string& text, const std::string& title = "");
    void shareFile(const std::string& filePath, const std::string& mimeType);

    // ── UI helpers ──
    void setStatusBarColor(uint32_t argb);
    void setNavigationBarColor(uint32_t argb);
    void keepScreenOn(bool on);
    void showToast(const std::string& message);

    // ── Callbacks from Java → C++ ──
    using ActivityResultCallback = std::function<void(int32_t requestCode, int32_t resultCode, const std::string& data)>;
    void setActivityResultCallback(ActivityResultCallback cb);

    using PermissionResultCallback = std::function<void(const std::string& permission, bool granted)>;
    void setPermissionResultCallback(PermissionResultCallback cb);

private:
    AndroidBridge() = default;
    ~AndroidBridge() = default;

    JavaVM* m_jvm = nullptr;
    jobject m_mainActivity = nullptr;

    ActivityResultCallback m_activityResultCallback;
    PermissionResultCallback m_permissionResultCallback;

    JNIEnv* getEnv();
    void callVoidMethod(const char* methodName, const char* sig, ...);
    bool callBoolMethod(const char* methodName, const char* sig, ...);
    std::string callStringMethod(const char* methodName, const char* sig, ...);
    int32_t callIntMethod(const char* methodName, const char* sig, ...);
    int64_t callLongMethod(const char* methodName, const char* sig, ...);
    float callFloatMethod(const char* methodName, const char* sig, ...);
};
