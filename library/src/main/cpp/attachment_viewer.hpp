#pragma once

/**
 * attachment_viewer.hpp - Media/attachment viewer component for PCP.
 *
 * Provides a fully-featured media viewer supporting images, videos (stills),
 * zoom/pan with double-tap and pinch gestures, slideshow mode with
 * prefetching, rotation, and transform state tracking.
 *
 * Thread-safe: all public methods acquire the internal mutex.
 */

#include "core_utils.hpp"

#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <mutex>
#include <chrono>

namespace pcp {
namespace library {

// ============================================================================
// Enums and constants
// ============================================================================

/** Viewer visibility/state enum. */
enum class ViewerState {
    Hidden = 0,       // Not visible; no resources held
    Loading,          // Media is being decoded / loaded
    Ready,            // Media loaded and displayed
    Playing,          // Video playback or slideshow active
    Error,            // Load failed
    Disposed          // Viewer destroyed; no further operations allowed
};

/** Media type classification. */
enum class MediaType {
    Image = 0,
    Video,
    GIF,
    SVG,
    PDF_Page,
    Unknown
};

/** Fit mode for displaying media. */
enum class FitMode {
    Contain,          // Scale to fit within bounds, maintaining aspect ratio
    Cover,            // Scale to cover bounds, cropping if needed
    Fill,             // Stretch to fill bounds (ignore aspect ratio)
    None_             // No scaling; show at 1:1 pixels
};

// ============================================================================
// Structs
// ============================================================================

/** Describes a single media source. */
struct MediaSource {
    std::string uri;                  // File URI, content URI, or URL
    std::string localPath;            // Cached local path (may be empty)
    std::string mimeType = "image/*";
    MediaType type = MediaType::Image;
    int64_t fileSize = 0;
    int width = 0;
    int height = 0;
    int64_t durationMs = 0;           // For video / GIF
    std::string thumbnailUri;         // URI for a low-res thumbnail
    std::string caption;
    bool isEncrypted = false;
    std::string decryptionKey;

    bool operator==(const MediaSource& other) const {
        return uri == other.uri && localPath == other.localPath;
    }
    bool operator!=(const MediaSource& other) const { return !(*this == other); }
};

/** Configuration for the attachment viewer. */
struct ViewerConfig {
    FitMode fitMode = FitMode::Contain;
    float minZoom = 0.5f;
    float maxZoom = 8.0f;
    float zoomStep = 0.25f;
    float doubleTapZoom = 2.5f;       // Zoom level on double-tap
    int prefetchCount = 3;            // Number of adjacent items to prefetch
    int slideshowIntervalMs = 3000;
    bool loopSlideshow = true;
    bool enableRotation = true;
    bool enablePinch = true;
    bool enablePan = true;
    bool showOverlay = true;          // Show caption/controls overlay
    float overlayTimeoutMs = 3000.0f; // Auto-hide overlay after inactivity
    Color backgroundColor{0, 0, 0, 255};  // Default black background
    bool tripleTapToReset = true;
    bool swipeToDismiss = true;
    float swipeDismissThreshold = 0.3f;
};

/** Descriptive info about the currently displayed media. */
struct MediaInfo {
    std::string uri;
    std::string mimeType;
    MediaType type = MediaType::Image;
    int width = 0;
    int height = 0;
    int64_t fileSize = 0;
    int64_t durationMs = 0;
    std::string caption;
    int indexInList = -1;
    int totalCount = 0;
};

/** Current transform applied to the media (pan, zoom, rotation). */
struct MediaTransform {
    float scale = 1.0f;
    float rotation = 0.0f;      // Degrees
    float panX = 0.0f;
    float panY = 0.0f;
    float pivotX = 0.5f;        // Normalized [0,1] pivot for zoom/rotation
    float pivotY = 0.5f;

    bool isIdentity() const {
        return scale == 1.0f && rotation == 0.0f && panX == 0.0f && panY == 0.0f;
    }

    void reset() {
        scale = 1.0f;
        rotation = 0.0f;
        panX = 0.0f;
        panY = 0.0f;
        pivotX = 0.5f;
        pivotY = 0.5f;
    }
};

/** Touch event data passed for gesture handling. */
struct TouchEvent {
    enum class Action { Down, Move, Up, Cancel, DoubleTap, LongPress };

    Action action = Action::Down;
    float x = 0.0f;
    float y = 0.0f;

    // Multi-touch: second pointer
    float x2 = 0.0f;
    float y2 = 0.0f;
    bool isMultiTouch = false;

    // Pinch state
    float pinchScale = 1.0f;
    float pinchCenterX = 0.0f;
    float pinchCenterY = 0.0f;

    // Velocity (for fling)
    float velocityX = 0.0f;
    float velocityY = 0.0f;

    int64_t timestampMs = 0;
};

// ============================================================================
// Callback types
// ============================================================================

using MediaLoadCallback = std::function<void(bool success, const std::string& errorMsg)>;
using SlideshowCallback = std::function<void(int newIndex, const MediaSource& item)>;
using TapCallback = std::function<void(float x, float y)>;
using TransformCallback = std::function<void(const MediaTransform& transform)>;

// ============================================================================
// AttachmentViewer class
// ============================================================================

/**
 * Full-featured media viewer component.
 *
 * Supports images, videos, GIFs, and PDF pages. Handles zoom, pan, rotation,
 * slideshow mode with prefetching, and gesture-based interaction.
 *
 * All public methods are thread-safe.
 */
class AttachmentViewer {
public:
    AttachmentViewer();
    ~AttachmentViewer();

    // --- Lifecycle ---

    /** Initialize with configuration. Must be called before use. */
    bool init(const ViewerConfig& config = ViewerConfig());

    /** Show the viewer with the given media list starting at index. */
    bool show(const std::vector<MediaSource>& mediaList, int startIndex = 0);

    /** Hide the viewer and optionally release resources. */
    void hide(bool releaseResources = false);

    // --- Media loading ---

    /** Load a specific media source. */
    void loadMedia(const MediaSource& source, MediaLoadCallback callback = nullptr);

    /** Set the media source list (replaces current). */
    void setMediaSource(const std::vector<MediaSource>& mediaList, int startIndex = 0);

    /** Get info about the currently displayed media. */
    MediaInfo getCurrentMediaInfo() const;

    // --- Zoom ---

    /** Zoom in by one step. */
    bool zoomIn();

    /** Zoom out by one step. */
    bool zoomOut();

    /** Zoom to a specific level. Clamped to [minZoom, maxZoom]. */
    bool zoomTo(float level);

    // --- Navigation ---

    /** Navigate to a specific index in the media list. */
    bool navigateTo(int index);

    /** Go to next item. Wraps if loop is enabled in slideshow. */
    bool navigateNext();

    /** Go to previous item. */
    bool navigatePrev();

    /** Reset the view transform (zoom=1, pan=0, rotation=0). */
    void resetView();

    // --- Rotation ---

    /** Rotate the media by the given degrees (relative). */
    void rotate(float degrees);

    // --- Slideshow ---

    /** Start slideshow mode. */
    bool slideshowStart(SlideshowCallback callback = nullptr);

    /** Stop slideshow mode. */
    void slideshowStop();

    /** Advance to the next slideshow item. */
    bool slideshowNext();

    // --- Prefetch / cache ---

    /** Prefetch adjacent media items into cache. */
    void prefetch(int count = -1); // -1 = use config.prefetchCount

    /** Clear the media cache (thumbnail and full decodes). */
    void clearCache();

    // --- Touch / gesture handling ---

    /**
     * Process a touch event for pan/pinch/double-tap.
     * Returns true if the event was consumed.
     */
    bool onTouchEvent(const TouchEvent& event);

    // --- Transform ---

    /** Get the current media transform. */
    MediaTransform getTransform() const;

    /** Set the media transform (animate optional). */
    void setTransform(const MediaTransform& transform, bool animate = false);

    // --- Appearance ---

    /** Set the viewer background color. */
    void setBackgroundColor(const Color& color);

    /** Show or hide the overlay (caption, controls). */
    void setOverlayVisible(bool visible);

    // --- Callbacks ---

    /** Set callback for tap events. */
    void setOnTap(TapCallback callback);

    /** Set callback for transform changes. */
    void setOnTransformChanged(TransformCallback callback);

    /** Set callback for slideshow advancement. */
    void setOnSlideshowTick(SlideshowCallback callback);

    // --- State ---

    /** Get current viewer state. */
    ViewerState getState() const;

    /** Check if viewer is currently visible. */
    bool isVisible() const;

    /** Get total number of items in the media list. */
    int getItemCount() const;

    /** Get the current item index. */
    int getCurrentIndex() const;

private:
    // --- Internal helpers ---
    void applyTransform();
    void clampTransform();
    void updateOverlayTimer();
    bool isCacheHit(const std::string& uri) const;
    void addToCache(const std::string& uri, const std::vector<uint8_t>& data);
    MediaType detectType(const std::string& mimeType) const;
    FitMode effectiveFitMode() const;

    // --- Thread safety ---
    mutable std::mutex mutex_;

    // --- State ---
    ViewerState state_ = ViewerState::Hidden;
    ViewerConfig config_;
    std::vector<MediaSource> mediaList_;
    int currentIndex_ = -1;
    MediaTransform transform_;
    bool overlayVisible_ = true;
    bool slideshowActive_ = false;
    int slideshowTimerId_ = -1;
    Color backgroundColor_{0, 0, 0, 255};

    // --- Cache ---
    struct CacheEntry {
        std::vector<uint8_t> data;
        std::chrono::steady_clock::time_point lastAccess;
    };
    std::unordered_map<std::string, CacheEntry> mediaCache_;
    static constexpr size_t kMaxCacheEntries = 20;

    // --- Callbacks ---
    MediaLoadCallback loadCallback_;
    SlideshowCallback slideshowCallback_;
    SlideshowCallback slideshowTickCallback_;
    TapCallback tapCallback_;
    TransformCallback transformCallback_;

    // --- Touch state ---
    bool isPinching_ = false;
    float pinchBaseScale_ = 1.0f;
    float pinchStartDist_ = 0.0f;
    bool isPanning_ = false;
    float panStartX_ = 0.0f;
    float panStartY_ = 0.0f;
    float panOriginX_ = 0.0f;
    float panOriginY_ = 0.0f;
};

} // namespace library
} // namespace pcp
