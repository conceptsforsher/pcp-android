#pragma once
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <map>
#include <android/log.h>

#define MP_LOG_TAG "MultiPicker"
#define MP_LOGI(...) __android_log_print(ANDROID_LOG_INFO, MP_LOG_TAG, __VA_ARGS__)

namespace pcp::multipicker {

enum class PickerType { IMAGE, VIDEO, AUDIO, FILE, CAMERA, CAMERA_VIDEO, CONTACT };
enum class PickerMode { SINGLE, MULTIPLE };

struct PickerItem {
    std::string uri;
    std::string displayName;
    std::string mimeType;
    int64_t size = 0;
    int width = 0, height = 0;
    int durationMs = 0;
    PickerType type = PickerType::FILE;
};

struct PickerFilter {
    std::vector<std::string> allowedMimeTypes;
    int64_t maxFileSize = 100 * 1024 * 1024;
};

struct PickerConfig {
    PickerType type = PickerType::FILE;
    PickerMode mode = PickerMode::SINGLE;
    PickerFilter filter;
    int maxSelection = 10;
    bool showPreview = true;
    bool allowMultipleTypes = false;
};

class MultiPicker {
public:
    explicit MultiPicker(const PickerConfig& config);
    ~MultiPicker();

    void open();
    void cancel();
    bool isOpen() const { return m_open; }

    const std::vector<PickerItem>& selectedItems() const { return m_selected; }
    int selectedCount() const { return static_cast<int>(m_selected.size()); }

    using PickCallback = std::function<void(const std::vector<PickerItem>&)>;
    void onPicked(PickCallback cb) { m_pickCallback = std::move(cb); }

    using CancelCallback = std::function<void()>;
    void onCancelled(CancelCallback cb) { m_cancelCallback = std::move(cb); }

    using ErrorCallback = std::function<void(const std::string&)>;
    void onError(ErrorCallback cb) { m_errorCallback = std::move(cb); }

private:
    PickerConfig m_config;
    std::vector<PickerItem> m_selected;
    bool m_open = false;

    PickCallback m_pickCallback;
    CancelCallback m_cancelCallback;
    ErrorCallback m_errorCallback;

    void scanFiles();
    bool matchFilter(const PickerItem& item) const;
    std::string getMimeTypeFromExtension(const std::string& path) const;
};

// ── MIME type utilities ──

class MimeTypeUtil {
public:
    static std::string fromExtension(const std::string& extension);
    static std::string fromPath(const std::string& path);
    static bool isImage(const std::string& mimeType);
    static bool isVideo(const std::string& mimeType);
    static bool isAudio(const std::string& mimeType);
    static std::string getExtension(const std::string& mimeType);
    static std::string getCategory(const std::string& mimeType);
private:
    static std::map<std::string, std::string> s_mimeMap;
};

// ── Image utilities ──

class ImageUtils {
public:
    struct ImageInfo { int width = 0, height = 0; std::string mimeType; int orientation = 0; };
    static ImageInfo getImageInfo(const std::string& path);
    static bool generateThumbnail(const std::string& path, int maxW, int maxH,
                                   std::vector<uint8_t>& output);
    static bool resize(const std::string& input, const std::string& output,
                       int targetW, int targetH, int quality = 85);
};

// ── File utilities ──

class FileUtils {
public:
    static std::string getDisplayName(const std::string& path);
    static int64_t getFileSize(const std::string& path);
    static std::string getExtension(const std::string& path);
    static std::string formatFileSize(int64_t bytes);
    static bool isReadable(const std::string& path, int64_t maxSize);
};

} // namespace pcp::multipicker
