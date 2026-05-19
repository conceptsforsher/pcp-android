#ifndef PROGRESSIVE_LIGHTWEIGHT_CALL_HPP
#define PROGRESSIVE_LIGHTWEIGHT_CALL_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// ---- Lightweight Call Mode ----

enum class CleanupAction {
    ClearImageCache,       // Glide/Coil bitmap cache
    ClearTimelineViews,    // RecyclerView adapter data
    ClearComposerState,    // rich text editor state
    SuspendBackgroundSync,  // pause matrix sync
    ClearAvatarCache,      // avatar bitmap cache
    ClearReactionCache,    // emoji/reaction render cache
    TrimMemoryAggressive,  // force GC + trim to MODERATE
    ClearWebViewCache,     // widget/Jitsi webview cache
    ClearMediaPlayer,      // stop audio playback
    DetachNonVisibleFrags, // detach fragments not in foreground
};

struct CleanupPlan {
    std::vector<CleanupAction> actions;
    int64_t estimatedBytesFreed = 0;
    int priority = 0; // lower numbers first
};

struct MemoryState {
    int64_t availableKb = 0;
    int64_t totalKb = 0;
    int64_t usedKb = 0;
    bool isLowMemory = false;       // device is in low memory state
    bool needsLightweightMode = false; // recommended
};

class LightweightCallManager {
public:
    // Enter lightweight call mode. Returns a cleanup plan.
    CleanupPlan enterCallMode();

    // Exit call mode. Returns actions that should be restored.
    CleanupPlan exitCallMode();

    // Check if the device is low on memory and should use lightweight mode.
    MemoryState assessMemory();

    // Determine which cleanup actions are applicable.
    static CleanupPlan computeCleanupPlan(const MemoryState& state);

    // Estimate bytes freed by an action.
    static int64_t estimateFreedBytes(CleanupAction action);

    // Format cleanup plan as JSON for Kotlin execution.
    static std::string planToJson(const CleanupPlan& plan);

    // Format memory state as JSON.
    static std::string memoryStateToJson(const MemoryState& state);

    // Check if this device would benefit from lightweight call mode.
    static bool shouldUseLightweightMode(const MemoryState& state);

    // Get total available RAM in KB (from /proc/meminfo).
    static int64_t getAvailableRamKb();

    // Get process RSS in KB.
    static int64_t getProcessRssKb();

private:
    bool isInCallMode_ = false;
    MemoryState preCallState_;
};

} // namespace progressive

#endif // PROGRESSIVE_LIGHTWEIGHT_CALL_HPP
