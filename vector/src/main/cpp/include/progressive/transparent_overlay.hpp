#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// ================================================================
// Transparent Overlay — touch state machine for background interaction
//
// Core engine: processes touch events and timers, decides when to:
//   - Route touches to background activity
//   - Bring background to foreground temporarily
//   - Return to foreground activity
//
// All timing and gesture thresholds configurable via Labs.
//
// Kotlin integration:
//   touchDown/touchMove/touchUp → forwarded from Activity.onTouchEvent()
//   timerTick() → called from Kotlin coroutine every ~16ms
//   getOverlayState() → Kotlin reads state to render UI
// ================================================================

// ---- Touch Action (what Kotlin should do) ----

enum class TouchAction {
    NONE = 0,                    // Nothing to do
    ROUTE_TO_BACKGROUND = 1,     // Route this touch to background activity
    BRING_BACKGROUND_FOREGROUND = 2, // Show background as foreground
    RETURN_TO_FOREGROUND = 3,    // Hide background, show foreground
    EXTEND_TIMER = 4,            // Extend the foreground timer
};

// ---- Gesture Type (what the user is doing) ----

enum class GestureType {
    IDLE = 0,
    ONE_FINGER_STILL = 1,       // One finger held still (arming pass-through)
    ONE_FINGER_ARMED = 2,       // One finger still long enough — second finger can interact
    TWO_FINGER_HOLD = 3,        // Two fingers held (waiting to switch)
    FOREGROUND_TRANSITION = 4,  // Swapping foreground/background
    RETURN_TRANSITION = 5,      // Returning to foreground
};

// ---- Overlay Config (all adjustable via Labs) ----

// ---- Safety Mode (what user can do with background) ----

enum class OverlaySafetyMode {
    FULL = 0,               // All interactions allowed
    READ_ONLY = 1,          // View only, no touches pass through
    SCROLL_ONLY = 2,        // Only scroll/swipe gestures
    TAP_ONLY = 3,           // Only single taps
    CUSTOM = 4,             // User-defined permission set
};

struct OverlaySafetyPermissions {
    bool allowTap = false;           // Single taps
    bool allowScroll = false;        // Swipe/scroll gestures
    bool allowLongPress = false;     // Long press
    bool allowDoubleTap = false;     // Double tap
    bool allowTextInput = false;     // Keyboard input
    bool allowNavigation = false;    // Back button, link clicks
    bool allowMediaControl = false;  // Play/pause audio
    bool showSensitiveContent = false; // Show message content vs blur
};

struct TransparentOverlayConfig {
    // One-finger pass-through (arm/disarm)
    int oneFingerHoldMs = 200;
    double oneFingerMovePx = 20.0;

    // Two-finger hold to switch foreground
    int twoFingerHoldMs = 1000;

    // Foreground temporary display
    int foregroundDurationMs = 2000;
    int foregroundExtendedMs = 3000;

    // Return timing
    int returnDurationMs = 500;
    int quickReturnHoldMs = 500;

    // Feature toggles
    bool enableOneFingerPassThrough = true;
    bool enableTwoFingerSwitch = true;
    bool enableBackButton = true;
    bool enableSwipeToReturn = true;

    // Swipe detection
    double swipeVelocityPxPerSec = 500.0;
    double swipeDistancePx = 100.0;

    // Safety mode
    OverlaySafetyMode safetyMode = OverlaySafetyMode::READ_ONLY;
    OverlaySafetyPermissions safetyPermissions;
};

// ---- Touch Point ----

struct TouchPoint {
    double x = 0.0;
    double y = 0.0;
    int64_t timeNs = 0;
    int pointerCount = 0;          // Number of pointers currently down
    int pointerId = 0;             // Which pointer (0, 1, ...)
};

// ---- Overlay State (for Kotlin rendering) ----

struct OverlayState {
    GestureType gesture = GestureType::IDLE;
    bool backgroundIsForeground = false;   // Background is currently shown as foreground
    bool oneFingerArmed = false;           // One-finger hold is armed
    bool twoFingerWaiting = false;         // Two fingers held, waiting timer
    int64_t foregroundUntilNs = 0;         // When foreground expires (0 = not active)
    int64_t oneFingerStillSinceNs = 0;     // When one finger became still
    int64_t twoFingerHoldSinceNs = 0;      // When two-finger hold started
    double firstFingerX = 0.0;
    double firstFingerY = 0.0;
    int currentPointerCount = 0;
};

// ---- Transparent Overlay State Machine ----

class TransparentOverlayEngine {
public:
    TransparentOverlayEngine();

    // ====== Configuration ======

    void setConfig(const TransparentOverlayConfig& config);
    TransparentOverlayConfig getConfig() const;

    // ====== Touch Events ======
    // Called from Kotlin Activity.onTouchEvent()

    // A finger went down.
    TouchAction touchDown(double x, double y, int pointerId, int64_t timeNs);

    // A finger moved.
    TouchAction touchMove(double x, double y, int pointerId, int64_t timeNs);

    // A finger was lifted.
    TouchAction touchUp(int pointerId, int64_t timeNs);

    // Back button pressed.
    TouchAction backPressed(int64_t timeNs);

    // ====== Timer Tick ======
    // Called ~16ms by Kotlin coroutine.
    // Checks all running timers and may trigger state transitions.

    TouchAction timerTick(int64_t timeNs);

    // ====== Safety Mode ======

    // Check if a specific touch action is allowed by safety mode.
    bool isTouchAllowed(TouchAction action) const;

    // Get human-readable description of current safety mode.
    std::string getSafetyModeLabel() const;

    // Get a list of what IS allowed (for UI display).
    std::vector<std::string> getAllowedActions() const;

    // Set safety mode.
    void setSafetyMode(OverlaySafetyMode mode);
    void setSafetyPermissions(const OverlaySafetyPermissions& perms);

    // ====== State ======

    OverlayState getState() const;

    // Determine what action should happen for a given touch.
    TouchAction getTouchAction(double x, double y, int pointerCount, int64_t timeNs) const;

    // ====== Serialization ======

    std::string stateToJson() const;
    std::string configToJson() const;
    std::string safetyToJson() const;

private:
    TransparentOverlayConfig config_;
    OverlayState state_;

    // Last known primary pointer position
    double primaryX_ = 0.0;
    double primaryY_ = 0.0;
    double primaryStartX_ = 0.0;
    double primaryStartY_ = 0.0;
    int64_t primaryDownTimeNs_ = 0;
    bool primaryIsDown_ = false;

    // Second pointer
    double secondaryX_ = 0.0;
    double secondaryY_ = 0.0;
    int64_t secondaryDownTimeNs_ = 0;
    bool secondaryIsDown_ = false;

    // Swipe tracking
    double swipeTotalDx_ = 0.0;
    double swipeTotalDy_ = 0.0;
    int64_t swipeStartTimeNs_ = 0;

    // Helpers
    bool isFingerStill(double startX, double startY, double currentX, double currentY) const;
    bool isTwoFingerHoldElapsed(int64_t nowNs) const;
    bool isForegroundExpired(int64_t nowNs) const;
};

} // namespace progressive
