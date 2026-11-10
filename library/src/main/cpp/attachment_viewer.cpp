/**
 * attachment_viewer.cpp - Media/attachment viewer component implementation.
 *
 * Implements zoom/pan/pinch/rotation gesture handling, slideshow with
 * prefetching, media caching, and transform management.
 */

#include "attachment_viewer.hpp"

#include <android/log.h>

#include <algorithm>
#include <cmath>
#include <sstream>
#include <unordered_set>

#define LOG_TAG "PcpViewer"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

namespace pcp {
namespace library {

// ============================================================================
// Construction / Destruction
// ============================================================================

AttachmentViewer::AttachmentViewer() {
    LOGD("AttachmentViewer created");
}

AttachmentViewer::~AttachmentViewer() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (slideshowActive_) {
        slideshowStop();
    }
    state_ = ViewerState::Disposed;
    mediaCache_.clear();
    LOGD("AttachmentViewer destroyed");
}

// ============================================================================
// Lifecycle
// ============================================================================

bool AttachmentViewer::init(const ViewerConfig& config) {
    std::lock_guard<std::mutex> lock(mutex_);

    if (state_ == ViewerState::Disposed) {
        LOGE("init: viewer already disposed");
        return false;
    }

    config_ = config;
    backgroundColor_ = config.backgroundColor;
    transform_.reset();
    state_ = ViewerState::Hidden;

    LOGD("AttachmentViewer initialized: minZoom=%.2f maxZoom=%.2f prefetch=%d",
         config_.minZoom, config_.maxZoom, config_.prefetchCount);
    return true;
}

bool AttachmentViewer::show(const std::vector<MediaSource>& mediaList, int startIndex) {
    std::lock_guard<std::mutex> lock(mutex_);

    if (state_ == ViewerState::Disposed) {
        LOGE("show: viewer disposed");
        return false;
    }

    mediaList_ = mediaList;
    currentIndex_ = std::max(0, std::min(startIndex, static_cast<int>(mediaList_.size()) - 1));
    transform_.reset();
    overlayVisible_ = config_.showOverlay;
    state_ = ViewerState::Loading;

    if (!mediaList_.empty()) {
        loadMedia(mediaList_[currentIndex_], nullptr);
        if (config_.prefetchCount > 0) {
            prefetch(config_.prefetchCount);
        }
    } else {
        state_ = ViewerState::Ready; // Empty list, nothing to show
    }

    LOGD("AttachmentViewer shown: %zu items, starting at %d", mediaList_.size(), currentIndex_);
    return true;
}

void AttachmentViewer::hide(bool releaseResources) {
    std::lock_guard<std::mutex> lock(mutex_);

    if (slideshowActive_) {
        slideshowStop();
    }

    state_ = ViewerState::Hidden;

    if (releaseResources) {
        mediaList_.clear();
        currentIndex_ = -1;
        mediaCache_.clear();
    }

    LOGD("AttachmentViewer hidden (releaseResources=%d)", releaseResources);
}

// ============================================================================
// Media loading
// ============================================================================

void AttachmentViewer::loadMedia(const MediaSource& source, MediaLoadCallback callback) {
    std::lock_guard<std::mutex> lock(mutex_);

    if (state_ == ViewerState::Disposed) {
        if (callback) callback(false, "Viewer disposed");
        return;
    }

    loadCallback_ = callback;
    state_ = ViewerState::Loading;

    // In a real implementation, this would:
    // 1. Check cache (isCacheHit)
    // 2. Decode image/video on a background thread
    // 3. Call back with result
    //
    // Simplified: assume success for cached items, else mark ready (placeholder)

    bool cached = isCacheHit(source.uri);
    if (cached) {
        state_ = ViewerState::Ready;
        if (loadCallback_) {
            loadCallback_(true, "");
            loadCallback_ = nullptr;
        }
        LOGD("loadMedia: cache hit for %s", source.uri.c_str());
    } else {
        // Simulate async load — mark ready with placeholder
        state_ = ViewerState::Ready;
        if (loadCallback_) {
            loadCallback_(true, "");
            loadCallback_ = nullptr;
        }
        LOGD("loadMedia: loaded %s (type=%d, %dx%d)",
             source.uri.c_str(), static_cast<int>(source.type),
             source.width, source.height);
    }
}

void AttachmentViewer::setMediaSource(const std::vector<MediaSource>& mediaList, int startIndex) {
    std::lock_guard<std::mutex> lock(mutex_);

    if (state_ == ViewerState::Disposed) return;

    // Stop any running slideshow
    if (slideshowActive_) {
        slideshowStop();
    }

    mediaList_ = mediaList;
    currentIndex_ = std::max(0, std::min(startIndex, static_cast<int>(mediaList_.size()) - 1));
    transform_.reset();

    if (!mediaList_.empty()) {
        state_ = ViewerState::Loading;
        loadMedia(mediaList_[currentIndex_], nullptr);
    }
}

MediaInfo AttachmentViewer::getCurrentMediaInfo() const {
    std::lock_guard<std::mutex> lock(mutex_);

    MediaInfo info;
    if (currentIndex_ >= 0 && currentIndex_ < static_cast<int>(mediaList_.size())) {
        const auto& src = mediaList_[currentIndex_];
        info.uri = src.uri;
        info.mimeType = src.mimeType;
        info.type = src.type;
        info.width = src.width;
        info.height = src.height;
        info.fileSize = src.fileSize;
        info.durationMs = src.durationMs;
        info.caption = src.caption;
        info.indexInList = currentIndex_;
        info.totalCount = static_cast<int>(mediaList_.size());
    }
    return info;
}

// ============================================================================
// Zoom
// ============================================================================

bool AttachmentViewer::zoomIn() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (state_ != ViewerState::Ready && state_ != ViewerState::Playing) return false;

    float newScale = transform_.scale + config_.zoomStep;
    return zoomTo(newScale);
}

bool AttachmentViewer::zoomOut() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (state_ != ViewerState::Ready && state_ != ViewerState::Playing) return false;

    float newScale = transform_.scale - config_.zoomStep;
    return zoomTo(newScale);
}

bool AttachmentViewer::zoomTo(float level) {
    // Internal: mutex_ must be held by caller (zoomIn/zoomOut do)
    float clamped = std::max(config_.minZoom, std::min(config_.maxZoom, level));
    if (std::abs(clamped - transform_.scale) < 0.001f) return false;

    transform_.scale = clamped;
    clampTransform();

    if (transformCallback_) {
        transformCallback_(transform_);
    }

    LOGD("Zoom: %.2f", clamped);
    return true;
}

// ============================================================================
// Navigation
// ============================================================================

bool AttachmentViewer::navigateTo(int index) {
    std::lock_guard<std::mutex> lock(mutex_);

    if (mediaList_.empty()) return false;
    if (index < 0 || index >= static_cast<int>(mediaList_.size())) return false;

    currentIndex_ = index;
    transform_.reset();
    state_ = ViewerState::Loading;
    loadMedia(mediaList_[currentIndex_], nullptr);

    if (config_.prefetchCount > 0) {
        prefetch(config_.prefetchCount);
    }

    LOGD("Navigate to %d", index);
    return true;
}

bool AttachmentViewer::navigateNext() {
    std::lock_guard<std::mutex> lock(mutex_);

    if (mediaList_.empty()) return false;

    int next = currentIndex_ + 1;
    if (next >= static_cast<int>(mediaList_.size())) {
        if (slideshowActive_ && config_.loopSlideshow) {
            next = 0;
        } else {
            return false;
        }
    }

    return navigateTo(next);
}

bool AttachmentViewer::navigatePrev() {
    std::lock_guard<std::mutex> lock(mutex_);

    if (mediaList_.empty()) return false;

    int prev = currentIndex_ - 1;
    if (prev < 0) {
        if (slideshowActive_ && config_.loopSlideshow) {
            prev = static_cast<int>(mediaList_.size()) - 1;
        } else {
            return false;
        }
    }

    return navigateTo(prev);
}

void AttachmentViewer::resetView() {
    std::lock_guard<std::mutex> lock(mutex_);
    transform_.reset();

    if (transformCallback_) {
        transformCallback_(transform_);
    }
    LOGD("View reset");
}

// ============================================================================
// Rotation
// ============================================================================

void AttachmentViewer::rotate(float degrees) {
    std::lock_guard<std::mutex> lock(mutex_);

    if (!config_.enableRotation) return;
    if (state_ != ViewerState::Ready && state_ != ViewerState::Playing) return;

    transform_.rotation = std::fmod(transform_.rotation + degrees, 360.0f);
    // Normalize to [0, 360)
    if (transform_.rotation < 0.0f) transform_.rotation += 360.0f;

    if (transformCallback_) {
        transformCallback_(transform_);
    }
    LOGD("Rotated by %.1f (total: %.1f)", degrees, transform_.rotation);
}

// ============================================================================
// Slideshow
// ============================================================================

bool AttachmentViewer::slideshowStart(SlideshowCallback callback) {
    std::lock_guard<std::mutex> lock(mutex_);

    if (mediaList_.empty()) return false;
    if (slideshowActive_) return false;
    if (state_ == ViewerState::Disposed || state_ == ViewerState::Hidden) return false;

    slideshowActive_ = true;
    slideshowCallback_ = callback;
    state_ = ViewerState::Playing;

    LOGD("Slideshow started (interval=%dms, loop=%d)",
         config_.slideshowIntervalMs, config_.loopSlideshow);
    return true;
}

void AttachmentViewer::slideshowStop() {
    std::lock_guard<std::mutex> lock(mutex_);

    slideshowActive_ = false;
    slideshowCallback_ = nullptr;

    if (state_ == ViewerState::Playing) {
        state_ = ViewerState::Ready;
    }

    LOGD("Slideshow stopped");
}

bool AttachmentViewer::slideshowNext() {
    std::lock_guard<std::mutex> lock(mutex_);

    if (!slideshowActive_) return false;

    bool advanced = navigateNext();
    if (advanced && slideshowCallback_) {
        slideshowCallback_(currentIndex_, mediaList_[currentIndex_]);
    }
    if (advanced && slideshowTickCallback_) {
        slideshowTickCallback_(currentIndex_, mediaList_[currentIndex_]);
    }
    return advanced;
}

// ============================================================================
// Prefetch / cache
// ============================================================================

void AttachmentViewer::prefetch(int count) {
    std::lock_guard<std::mutex> lock(mutex_);

    if (mediaList_.empty()) return;

    int toPrefetch = (count < 0) ? config_.prefetchCount : count;

    // Prefetch items after current
    for (int i = 1; i <= toPrefetch; i++) {
        int idx = currentIndex_ + i;
        if (idx >= static_cast<int>(mediaList_.size())) {
            idx -= static_cast<int>(mediaList_.size());
        }
        if (idx == currentIndex_) break; // Wrapped around
        if (idx >= 0 && idx < static_cast<int>(mediaList_.size())) {
            const auto& src = mediaList_[idx];
            if (!isCacheHit(src.uri)) {
                // In production: fire off background decode task
                // For now: mark intent
                LOGD("Prefetch scheduled: %s", src.uri.c_str());
            }
        }
    }

    // Prefetch items before current
    for (int i = 1; i <= toPrefetch; i++) {
        int idx = currentIndex_ - i;
        if (idx < 0) {
            idx += static_cast<int>(mediaList_.size());
        }
        if (idx == currentIndex_) break;
        if (idx >= 0 && idx < static_cast<int>(mediaList_.size())) {
            const auto& src = mediaList_[idx];
            if (!isCacheHit(src.uri)) {
                LOGD("Prefetch scheduled: %s", src.uri.c_str());
            }
        }
    }
}

void AttachmentViewer::clearCache() {
    std::lock_guard<std::mutex> lock(mutex_);
    mediaCache_.clear();
    LOGD("Media cache cleared");
}

// ============================================================================
// Touch / gesture handling
// ============================================================================

bool AttachmentViewer::onTouchEvent(const TouchEvent& event) {
    std::lock_guard<std::mutex> lock(mutex_);

    if (state_ != ViewerState::Ready && state_ != ViewerState::Playing) {
        return false;
    }

    switch (event.action) {
    case TouchEvent::Action::Down: {
        isPanning_ = true;
        isPinching_ = false;
        panStartX_ = event.x;
        panStartY_ = event.y;
        panOriginX_ = transform_.panX;
        panOriginY_ = transform_.panY;
        break;
    }

    case TouchEvent::Action::Move: {
        if (event.isMultiTouch && config_.enablePinch) {
            // Pinch gesture
            if (!isPinching_) {
                isPinching_ = true;
                isPanning_ = false;
                pinchBaseScale_ = transform_.scale;
                pinchStartDist_ = std::sqrt(
                    (event.x - event.x2) * (event.x - event.x2) +
                    (event.y - event.y2) * (event.y - event.y2));
            }

            float currentDist = std::sqrt(
                (event.x - event.x2) * (event.x - event.x2) +
                (event.y - event.y2) * (event.y - event.y2));

            if (pinchStartDist_ > 0.0f) {
                float scaleFactor = currentDist / pinchStartDist_;
                float newScale = pinchBaseScale_ * scaleFactor;
                newScale = std::max(config_.minZoom, std::min(config_.maxZoom, newScale));

                transform_.scale = newScale;
                transform_.pivotX = event.pinchCenterX;
                transform_.pivotY = event.pinchCenterY;
                clampTransform();

                if (transformCallback_) {
                    transformCallback_(transform_);
                }
            }
        } else if (isPanning_ && config_.enablePan) {
            // Pan gesture
            float dx = event.x - panStartX_;
            float dy = event.y - panStartY_;

            // Scale pan by inverse of zoom (faster panning at low zoom)
            float panScale = 1.0f / std::max(0.1f, transform_.scale);
            transform_.panX = panOriginX_ + dx * panScale;
            transform_.panY = panOriginY_ + dy * panScale;
            clampTransform();

            if (transformCallback_) {
                transformCallback_(transform_);
            }
        }
        break;
    }

    case TouchEvent::Action::Up:
    case TouchEvent::Action::Cancel: {
        if (isPinching_) {
            isPinching_ = false;
        }
        if (isPanning_) {
            isPanning_ = false;

            // Check swipe-to-dismiss
            if (config_.swipeToDismiss) {
                float totalPan = std::abs(transform_.panX - panOriginX_) +
                                 std::abs(transform_.panY - panOriginY_);
                // If panned significantly, navigate
                if (totalPan > 200.0f && transform_.scale <= 1.05f) {
                    if (transform_.panX - panOriginX_ > 100.0f) {
                        navigatePrev();
                    } else if (panOriginX_ - transform_.panX > 100.0f) {
                        navigateNext();
                    }
                }
            }
        }
        break;
    }

    case TouchEvent::Action::DoubleTap: {
        // Toggle between 1x and doubleTapZoom
        if (std::abs(transform_.scale - 1.0f) < 0.05f) {
            zoomTo(config_.doubleTapZoom);
            transform_.pivotX = event.x;
            transform_.pivotY = event.y;
        } else if (config_.tripleTapToReset) {
            resetView();
        } else {
            zoomTo(1.0f);
        }
        break;
    }

    case TouchEvent::Action::LongPress: {
        // Long press: could trigger context menu, save, share, etc.
        if (tapCallback_) {
            tapCallback_(event.x, event.y);
        }
        break;
    }
    }

    // Update overlay visibility timer
    updateOverlayTimer();
    return true;
}

// ============================================================================
// Transform
// ============================================================================

MediaTransform AttachmentViewer::getTransform() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return transform_;
}

void AttachmentViewer::setTransform(const MediaTransform& transform, bool animate) {
    std::lock_guard<std::mutex> lock(mutex_);

    transform_ = transform;
    // Clamp scale
    transform_.scale = std::max(config_.minZoom, std::min(config_.maxZoom, transform_.scale));
    // Normalize rotation
    transform_.rotation = std::fmod(transform_.rotation, 360.0f);
    if (transform_.rotation < 0.0f) transform_.rotation += 360.0f;
    clampTransform();

    if (transformCallback_) {
        transformCallback_(transform_);
    }

    LOGD("Transform set: scale=%.2f rot=%.1f pan=(%.1f,%.1f) animate=%d",
         transform_.scale, transform_.rotation, transform_.panX, transform_.panY, animate);
}

// ============================================================================
// Appearance
// ============================================================================

void AttachmentViewer::setBackgroundColor(const Color& color) {
    std::lock_guard<std::mutex> lock(mutex_);
    backgroundColor_ = color;
    LOGD("Background color set: #%02x%02x%02x%02x", color.r, color.g, color.b, color.a);
}

void AttachmentViewer::setOverlayVisible(bool visible) {
    std::lock_guard<std::mutex> lock(mutex_);
    overlayVisible_ = visible;
    LOGD("Overlay %s", visible ? "shown" : "hidden");
}

// ============================================================================
// Callbacks
// ============================================================================

void AttachmentViewer::setOnTap(TapCallback callback) {
    std::lock_guard<std::mutex> lock(mutex_);
    tapCallback_ = std::move(callback);
}

void AttachmentViewer::setOnTransformChanged(TransformCallback callback) {
    std::lock_guard<std::mutex> lock(mutex_);
    transformCallback_ = std::move(callback);
}

void AttachmentViewer::setOnSlideshowTick(SlideshowCallback callback) {
    std::lock_guard<std::mutex> lock(mutex_);
    slideshowTickCallback_ = std::move(callback);
}

// ============================================================================
// State queries
// ============================================================================

ViewerState AttachmentViewer::getState() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return state_;
}

bool AttachmentViewer::isVisible() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return state_ == ViewerState::Loading || state_ == ViewerState::Ready
        || state_ == ViewerState::Playing;
}

int AttachmentViewer::getItemCount() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return static_cast<int>(mediaList_.size());
}

int AttachmentViewer::getCurrentIndex() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return currentIndex_;
}

// ============================================================================
// Private helpers
// ============================================================================

void AttachmentViewer::applyTransform() {
    // Called internally to push transform to the render layer.
    // In a real implementation, this would update a GPU transform matrix
    // on the rendering thread.
}

void AttachmentViewer::clampTransform() {
    // Clamp pan values so the media doesn't go completely off-screen
    // This is a simplified version; real impl would use viewport size
    float maxPan = 2000.0f * transform_.scale;
    transform_.panX = std::max(-maxPan, std::min(maxPan, transform_.panX));
    transform_.panY = std::max(-maxPan, std::min(maxPan, transform_.panY));
}

void AttachmentViewer::updateOverlayTimer() {
    if (!config_.showOverlay) return;
    // Reset auto-hide timer for overlay visibility.
    // Real impl would schedule a timer on the UI thread.
}

bool AttachmentViewer::isCacheHit(const std::string& uri) const {
    return mediaCache_.find(uri) != mediaCache_.end();
}

void AttachmentViewer::addToCache(const std::string& uri, const std::vector<uint8_t>& data) {
    // Evict oldest entry if at capacity
    if (mediaCache_.size() >= kMaxCacheEntries) {
        auto oldest = mediaCache_.begin();
        for (auto it = mediaCache_.begin(); it != mediaCache_.end(); ++it) {
            if (it->second.lastAccess < oldest->second.lastAccess) {
                oldest = it;
            }
        }
        mediaCache_.erase(oldest);
    }

    CacheEntry entry;
    entry.data = data;
    entry.lastAccess = std::chrono::steady_clock::now();
    mediaCache_[uri] = std::move(entry);
}

MediaType AttachmentViewer::detectType(const std::string& mimeType) const {
    if (mimeType.find("image/svg") != std::string::npos) return MediaType::SVG;
    if (mimeType.find("image/gif") != std::string::npos) return MediaType::GIF;
    if (mimeType.find("image/") != std::string::npos)   return MediaType::Image;
    if (mimeType.find("video/") != std::string::npos)   return MediaType::Video;
    if (mimeType.find("application/pdf") != std::string::npos) return MediaType::PDF_Page;
    return MediaType::Unknown;
}

FitMode AttachmentViewer::effectiveFitMode() const {
    return config_.fitMode;
}

} // namespace library
} // namespace pcp
