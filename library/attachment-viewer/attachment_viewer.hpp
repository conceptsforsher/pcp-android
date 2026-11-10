#pragma once
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <android/log.h>

#define AV_LOG_TAG "AttachmentViewer"
#define AV_LOGI(...) __android_log_print(ANDROID_LOG_INFO, AV_LOG_TAG, __VA_ARGS__)
#define AV_LOGE(...) __android_log_print(ANDROID_LOG_ERROR, AV_LOG_TAG, __VA_ARGS__)

namespace pcp::attachment_viewer {

enum class AttachmentType { IMAGE, VIDEO, AUDIO, FILE, STICKER, LOCATION, UNKNOWN };
enum class SwipeDirection { LEFT, RIGHT, UP, DOWN, NONE };
enum class ViewerState { LOADING, READY, ERROR, DISMISSED };

struct AttachmentItem {
    std::string id;
    std::string url;
    std::string thumbnailUrl;
    std::string localPath;
    std::string mimeType;
    std::string title;
    std::string description;
    AttachmentType type = AttachmentType::UNKNOWN;
    int64_t size = 0;
    int width = 0, height = 0;
    int durationMs = 0;
    bool isEncrypted = false;
};

struct AttachmentViewerConfig {
    bool showTitle = true;
    bool showDescription = false;
    bool allowSwipe = true;
    bool allowPinchZoom = true;
    bool allowShare = true;
    bool allowDownload = true;
    bool loopVideo = false;
    bool autoplayVideo = true;
    float maxZoom = 5.0f;
    float minZoom = 0.5f;
};

class AttachmentViewer {
public:
    explicit AttachmentViewer(const AttachmentViewerConfig& config = {});
    ~AttachmentViewer();

    void show(const std::vector<AttachmentItem>& items, int startIndex = 0);
    void dismiss();
    bool isVisible() const { return m_visible; }

    void next();
    void previous();
    int currentIndex() const { return m_currentIndex; }
    int itemCount() const { return static_cast<int>(m_items.size()); }

    void zoomTo(float scale, float cx = 0.5f, float cy = 0.5f);
    void resetZoom();

    ViewerState state() const { return m_state; }
    const AttachmentItem* currentItem() const;

    using DismissCallback = std::function<void()>;
    void onDismiss(DismissCallback cb) { m_dismissCallback = std::move(cb); }

    using IndexChangeCallback = std::function<void(int index)>;
    void onIndexChanged(IndexChangeCallback cb) { m_indexChangeCallback = std::move(cb); }

    using ShareCallback = std::function<void(const AttachmentItem&)>;
    void onShare(ShareCallback cb) { m_shareCallback = std::move(cb); }

    using DownloadCallback = std::function<void(const AttachmentItem&)>;
    void onDownload(DownloadCallback cb) { m_downloadCallback = std::move(cb); }

private:
    AttachmentViewerConfig m_config;
    std::vector<AttachmentItem> m_items;
    int m_currentIndex = 0;
    bool m_visible = false;
    ViewerState m_state = ViewerState::LOADING;

    float m_zoom = 1.0f;
    float m_panX = 0.0f, m_panY = 0.0f;
    SwipeDirection m_swipeDirection = SwipeDirection::NONE;

    DismissCallback m_dismissCallback;
    IndexChangeCallback m_indexChangeCallback;
    ShareCallback m_shareCallback;
    DownloadCallback m_downloadCallback;

    void loadItem(int index);
    void onSwipeDetected(SwipeDirection direction);
};

// ── Image Loader ──
class ImageLoader {
public:
    ImageLoader();
    ~ImageLoader();

    bool loadFromUrl(const std::string& url, std::vector<uint8_t>& output);
    bool loadFromFile(const std::string& path, std::vector<uint8_t>& output);
    bool loadThumbnail(const std::string& url, int maxWidth, int maxHeight,
                       std::vector<uint8_t>& output);

    void setCacheDir(const std::string& dir) { m_cacheDir = dir; }
    void clearCache();

private:
    std::string m_cacheDir;
    std::string getCachePath(const std::string& url) const;
};

// ── Media Player ──
class MediaPlayer {
public:
    MediaPlayer();
    ~MediaPlayer();

    bool play(const std::string& url);
    bool playLocal(const std::string& path);
    void pause();
    void resume();
    void stop();
    void seek(int positionMs);
    int duration() const { return m_duration; }
    int position() const { return m_position; }
    bool isPlaying() const { return m_playing; }
    bool isPaused() const { return m_paused; }
    float volume() const { return m_volume; }
    void setVolume(float v) { m_volume = v; }
    void setLooping(bool loop) { m_loop = loop; }

    using StateCallback = std::function<void(bool playing, int pos, int dur)>;
    void onStateChanged(StateCallback cb) { m_stateCallback = std::move(cb); }

    using ErrorCallback = std::function<void(const std::string&)>;
    void onError(ErrorCallback cb) { m_errorCallback = std::move(cb); }

private:
    bool m_playing = false;
    bool m_paused = false;
    int m_position = 0;
    int m_duration = 0;
    float m_volume = 1.0f;
    bool m_loop = false;
    StateCallback m_stateCallback;
    ErrorCallback m_errorCallback;
};

} // namespace pcp::attachment_viewer
