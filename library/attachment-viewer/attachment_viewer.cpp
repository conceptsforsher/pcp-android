#include "attachment_viewer.hpp"
#include <fstream>
#include <algorithm>
#include <cmath>

namespace pcp::attachment_viewer {

AttachmentViewer::AttachmentViewer(const AttachmentViewerConfig& config) : m_config(config) {}
AttachmentViewer::~AttachmentViewer() = default;

void AttachmentViewer::show(const std::vector<AttachmentItem>& items, int startIndex) {
    m_items = items;
    m_currentIndex = std::max(0, std::min(startIndex, static_cast<int>(items.size()) - 1));
    m_visible = true;
    m_state = ViewerState::LOADING;
    AV_LOGI("Showing attachment viewer: %zu items, start=%d", items.size(), m_currentIndex);
    loadItem(m_currentIndex);
}

void AttachmentViewer::dismiss() {
    m_visible = false;
    m_state = ViewerState::DISMISSED;
    if (m_dismissCallback) m_dismissCallback();
}

void AttachmentViewer::next() {
    if (m_currentIndex < static_cast<int>(m_items.size()) - 1) {
        m_currentIndex++;
        loadItem(m_currentIndex);
        if (m_indexChangeCallback) m_indexChangeCallback(m_currentIndex);
    }
}

void AttachmentViewer::previous() {
    if (m_currentIndex > 0) {
        m_currentIndex--;
        loadItem(m_currentIndex);
        if (m_indexChangeCallback) m_indexChangeCallback(m_currentIndex);
    }
}

void AttachmentViewer::zoomTo(float scale, float cx, float cy) {
    m_zoom = std::max(m_config.minZoom, std::min(m_config.maxZoom, scale));
    m_panX = cx; m_panY = cy;
}

void AttachmentViewer::resetZoom() {
    m_zoom = 1.0f; m_panX = 0.0f; m_panY = 0.0f;
}

const AttachmentItem* AttachmentViewer::currentItem() const {
    if (m_currentIndex >= 0 && m_currentIndex < static_cast<int>(m_items.size()))
        return &m_items[m_currentIndex];
    return nullptr;
}

void AttachmentViewer::loadItem(int index) {
    if (index < 0 || index >= static_cast<int>(m_items.size())) {
        m_state = ViewerState::ERROR;
        return;
    }
    m_state = ViewerState::LOADING;
    // In real implementation: load image/video data
    // Decode and prepare for display
    m_state = ViewerState::READY;
}

void AttachmentViewer::onSwipeDetected(SwipeDirection direction) {
    m_swipeDirection = direction;
    switch (direction) {
        case SwipeDirection::LEFT: next(); break;
        case SwipeDirection::RIGHT: previous(); break;
        case SwipeDirection::UP: dismiss(); break;
        default: break;
    }
}

// ── ImageLoader ──

ImageLoader::ImageLoader() = default;
ImageLoader::~ImageLoader() = default;

bool ImageLoader::loadFromUrl(const std::string& url, std::vector<uint8_t>& output) {
    // Check cache first
    std::string cachePath = getCachePath(url);
    if (!cachePath.empty()) {
        std::ifstream f(cachePath, std::ios::binary);
        if (f.good()) {
            f.seekg(0, std::ios::end);
            output.resize(f.tellg());
            f.seekg(0);
            f.read(reinterpret_cast<char*>(output.data()), output.size());
            return true;
        }
    }
    // Fetch from URL (stub - would use HTTP client)
    return false;
}

bool ImageLoader::loadFromFile(const std::string& path, std::vector<uint8_t>& output) {
    std::ifstream f(path, std::ios::binary);
    if (!f) return false;
    f.seekg(0, std::ios::end);
    output.resize(f.tellg());
    f.seekg(0);
    f.read(reinterpret_cast<char*>(output.data()), output.size());
    return true;
}

bool ImageLoader::loadThumbnail(const std::string& url, int maxWidth, int maxHeight,
                                  std::vector<uint8_t>& output) {
    // Generate/retrieve scaled thumbnail
    return false;
}

void ImageLoader::clearCache() {
    // Delete cached files
}

std::string ImageLoader::getCachePath(const std::string& url) const {
    if (m_cacheDir.empty()) return "";
    // Hash URL to create cache filename
    return m_cacheDir + "/img_" + std::to_string(std::hash<std::string>{}(url));
}

// ── MediaPlayer ──

MediaPlayer::MediaPlayer() = default;
MediaPlayer::~MediaPlayer() { stop(); }

bool MediaPlayer::play(const std::string& url) { m_playing = true; m_paused = false; return true; }
bool MediaPlayer::playLocal(const std::string& path) { m_playing = true; m_paused = false; return true; }
void MediaPlayer::pause() { m_paused = true; m_playing = false; }
void MediaPlayer::resume() { m_paused = false; m_playing = true; }
void MediaPlayer::stop() { m_playing = false; m_paused = false; m_position = 0; }
void MediaPlayer::seek(int positionMs) { m_position = std::max(0, std::min(positionMs, m_duration)); }

} // namespace pcp::attachment_viewer
