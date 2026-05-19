#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <cstdint>

namespace progressive {

// ================================================================
// Native Profiler — measure C++ function execution time
//
// Inspired by Element Android's internal tracing (Timber, Sentry spans)
// and Android Profiler (CPU trace).
//
// Usage:
//   Profiler::start("my_function");
//   // ... code ...
//   Profiler::stop("my_function");
//   // Get report: Profiler::instance().report();
//
// Slash commands:
//   /profile start — begin profiling
//   /profile stop  — stop profiling and print report
//   /profile reset — clear all data
//   /profile status — show current stats
// ================================================================

// ---- Profile Entry (single measurement) ----

struct ProfilerEntry {
    std::string name;                // Function/module name
    int64_t startTimeNs = 0;         // Start timestamp (nanoseconds)
    int64_t endTimeNs = 0;           // End timestamp
    int64_t durationNs = 0;          // Duration
    int callCount = 0;               // How many times called
    bool active = false;             // Currently measuring
};

// ---- Profile Summary (aggregated) ----

struct ProfileSummary {
    std::string name;
    int callCount = 0;
    int64_t totalTimeNs = 0;         // Total accumulated time
    int64_t minTimeNs = INT64_MAX;
    int64_t maxTimeNs = 0;
    int64_t avgTimeNs = 0;
    double percentTotal = 0.0;       // % of total profiling time
};

// ---- Profile Report ----

struct ProfileReport {
    std::vector<ProfileSummary> entries;
    int64_t totalTimeNs = 0;         // Total time of all entries
    int64_t reportTimeNs = 0;        // When report was generated
    bool isProfiling = false;        // Currently active
    int activeEntries = 0;           // Currently open entries
    std::string format;              // "text" or "json"
    int64_t overheadNs = 0;          // Profiler's own overhead
};

// ---- Memory Snapshot ----

struct MemorySnapshot {
    int64_t timestampNs = 0;
    int64_t allocatedBytes = 0;      // Approximate allocated memory
    int allocateCount = 0;
    int deallocateCount = 0;
    std::string label;
};

// ---- User Action Measurement ----
// Measures real-world UX: "back_to_menu", "open_room", "send_message", etc.
// Answers: "How long did the user wait after pressing Back?"

struct UserActionMeasurement {
    std::string actionName;          // "back_to_menu", "open_room", "send_message"
    int64_t startNs = 0;
    int64_t endNs = 0;
    int64_t durationNs = 0;          // Total wait time for user
    bool completed = false;
    bool withinBudget = true;        // Within threshold
    int64_t budgetNs = 200000000LL;   // Default: 200ms
    std::string context;             // "room_id:!abc:org" or extra info
    bool isCold = false;             // Cold start (first time)
};

// ---- Action Stats (percentiles over last N runs) ----

struct ActionStats {
    std::string actionName;
    int runCount = 0;
    int64_t budgetNs = 200000000LL;   // 200ms default
    int overBudgetCount = 0;         // How many times exceeded budget
    int64_t totalNs = 0;
    int64_t minNs = INT64_MAX;
    int64_t maxNs = 0;
    int64_t avgNs = 0;
    int64_t p50Ns = 0;               // Median (50th percentile)
    int64_t p90Ns = 0;               // 90th percentile
    int64_t p95Ns = 0;
    int64_t p99Ns = 0;
    std::vector<int64_t> history;    // Last N measurements (max 100)
    bool hasColdMeasurement = false;
    int64_t coldDurationNs = 0;      // First-run duration
};

// ---- Frame Timing (for animation smoothness) ----

struct FrameTiming {
    int frameNumber = 0;
    int64_t frameStartNs = 0;
    int64_t frameEndNs = 0;
    int64_t frameDurationNs = 0;     // Should be < 16.67ms for 60fps
    bool dropped = false;            // > 16.67ms = dropped frame
    int consecutiveDrops = 0;        // Consecutive dropped frames
};

// ---- Action Budget Config ----

struct ActionBudget {
    std::string actionPattern;       // "back_to_*", "*_menu", exact name, or regex-like
    int64_t budgetNs = 200000000LL;  // Default: 200ms
    bool reportOnViolation = true;   // Auto-log when exceeded
    int sampleSize = 100;            // Max history entries
};

// ---- Profiler ----

class Profiler {
public:
    // Singleton access.
    static Profiler& instance();

    // ====== Lifecycle ======
    void startProfiling();
    void stopProfiling();
    void reset();
    bool isProfiling() const { return profiling_; }

    // ====== Measurement ======
    int start(const std::string& name);
    int64_t stop(const std::string& name);
    int64_t stop(int entryIndex);

    // ====== Scoped Timer (RAII) ======
    class ScopeTimer {
    public:
        ScopeTimer(const std::string& name);
        ~ScopeTimer();
        int64_t elapsedNs() const;
    private:
        std::string name_;
        int64_t startNs_;
        bool stopped_ = false;
    };
    static ScopeTimer scope(const std::string& name) { return ScopeTimer(name); }

    // ====== User Action Timing (UX measurement) ======

    // Start measuring a user action (e.g. "back_to_menu").
    // Returns action index for pairing with stopAction().
    int startAction(const std::string& actionName, const std::string& context = "",
                     bool isCold = false);

    // Stop measuring. Returns duration in nanoseconds.
    // Checks budget (default: 200ms) and records violation if exceeded.
    int64_t stopAction(int actionIndex);

    // Convenience: start+stop in one. Returns duration.
    int64_t measureAction(const std::string& actionName, int64_t startNs);

    // Set a budget for a specific action pattern.
    void setActionBudget(const std::string& actionPattern, int64_t budgetNs);

    // Check if last measurement was within budget.
    bool lastActionWithinBudget(const std::string& actionName) const;

    // Get detailed stats for an action (with percentiles).
    ActionStats getActionStats(const std::string& actionName) const;

    // Get all action stats.
    std::vector<ActionStats> getAllActionStats() const;

    // ====== Frame Timing ======

    // Start a new frame measurement.
    int startFrame();

    // End frame measurement. Returns duration in nanoseconds.
    int64_t endFrame(int frameIndex);

    // Get frame timing stats (last N frames).
    std::vector<FrameTiming> getFrameTimings(int lastN = 120) const;

    // Check if current frame rate is smooth.
    double getCurrentFps() const;

    // ====== Memory Tracking ======
    void recordAlloc(int64_t bytes);
    void recordDealloc(int64_t bytes);
    MemorySnapshot takeMemorySnapshot(const std::string& label = "");
    int64_t currentTrackedMemory() const { return trackedMemory_; }

    // ====== Reporting ======
    ProfileReport generateReport() const;
    std::string reportToJson() const;
    std::string reportToText() const;
    std::string summaryToJson(const ProfileSummary& s) const;
    ProfileSummary getSummary(const std::string& name) const;

    // ====== Action Reporting ======
    std::string actionReportToJson() const;
    std::string actionReportToText() const;

    // ====== Real-Time Overlay (HUD data provider) ======

    // Compact JSON snapshot for real-time overlay rendering (~10fps poll).
    // Kotlin reads this and renders a translucent HUD.
    std::string realTimeSnapshotJson() const;

    // Real-time snapshot as compact text (for overlay).
    std::string realTimeSnapshotText() const;

    // Get hot actions (top 3 slowest in last 30 seconds).
    std::vector<ActionStats> hotActions() const;

    // Get any budget violations in the last N seconds.
    std::vector<std::string> recentViolations(int windowSec = 30) const;

    // Color code for a duration relative to budget.
    // Returns "green", "yellow", or "red" hex.
    static std::string colorForDuration(int64_t durationNs, int64_t budgetNs = 200000000LL);

    // Color string for FPS value.
    static std::string colorForFps(double fps);

    // ====== Stats ======
    int totalEntries() const { return static_cast<int>(entries_.size()); }
    int totalSummaries() const { return static_cast<int>(summaries_.size()); }
    int64_t totalProfiledTime() const;
    int getActiveCount() const;

private:
    Profiler();
    ~Profiler() = default;
    Profiler(const Profiler&) = delete;
    Profiler& operator=(const Profiler&) = delete;

    bool profiling_ = false;
    std::vector<ProfilerEntry> entries_;
    std::unordered_map<std::string, ProfileSummary> summaries_;
    std::vector<MemorySnapshot> memorySnapshots_;
    int64_t trackedMemory_ = 0;
    int allocCount_ = 0;
    int deallocCount_ = 0;

    // User action measurements
    std::vector<UserActionMeasurement> actions_;
    std::unordered_map<std::string, ActionStats> actionStats_;
    std::vector<ActionBudget> budgets_;
    int actionSeq_ = 0;

    // Frame timing
    std::vector<FrameTiming> frames_;
    int frameCount_ = 0;

    // Helpers
    static int64_t nowNs();
    int64_t measureOverhead();
    int64_t overheadNs_ = 0;

    // Compute percentiles from sorted history.
    static void computePercentiles(ActionStats& stats);

    // Format helpers
    static std::string formatNs(int64_t ns);
    static std::string formatBytes(int64_t bytes);
};

} // namespace progressive
