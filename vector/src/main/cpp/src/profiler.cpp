#include "progressive/profiler.hpp"
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <thread>
#include <cmath>
#include <numeric>

namespace progressive {

// ====== High-resolution timer ======

int64_t Profiler::nowNs() {
    auto now = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
}

// ====== Singleton ======

Profiler& Profiler::instance() {
    static Profiler inst;
    return inst;
}

Profiler::Profiler() {
    overheadNs_ = measureOverhead();
}

int64_t Profiler::measureOverhead() {
    // Measure the overhead of start/stop pair
    auto before = nowNs();
    auto after = nowNs();
    return after - before;
}

// ====== Lifecycle ======

void Profiler::startProfiling() {
    profiling_ = true;
}

void Profiler::stopProfiling() {
    profiling_ = false;
}

void Profiler::reset() {
    entries_.clear();
    summaries_.clear();
    memorySnapshots_.clear();
    trackedMemory_ = 0;
    allocCount_ = 0;
    deallocCount_ = 0;
    profiling_ = false;
}

// ====== Measurement ======

int Profiler::start(const std::string& name) {
    return start(name, "", "");
}

int Profiler::start(const std::string& name, const std::string& parentName,
    const std::string& metadata)
{
    ProfilerEntry entry;
    entry.name = name;
    entry.startTimeNs = nowNs();
    entry.active = true;
    entry.callCount = 1;
    entry.parentName = parentName;
    entry.threadId = static_cast<int>(
        std::hash<std::thread::id>{}(std::this_thread::get_id()));
    entry.metadata = metadata;

    // Update summary
    auto& summ = summaries_[name];
    summ.name = name;
    summ.callCount++;

    entries_.push_back(entry);
    return static_cast<int>(entries_.size()) - 1;
}

int64_t Profiler::stop(const std::string& name) {
    // Find the most recent active entry with this name
    for (auto it = entries_.rbegin(); it != entries_.rend(); ++it) {
        if (it->active && it->name == name) {
            it->endTimeNs = nowNs();
            it->active = false;
            it->durationNs = it->endTimeNs - it->startTimeNs - overheadNs_;

            // Update summary
            auto& summ = summaries_[name];
            summ.totalTimeNs += it->durationNs;
            if (it->durationNs < summ.minTimeNs) summ.minTimeNs = it->durationNs;
            if (it->durationNs > summ.maxTimeNs) summ.maxTimeNs = it->durationNs;
            summ.avgTimeNs = summ.totalTimeNs / summ.callCount;

            return it->durationNs;
        }
    }
    return 0;
}

int64_t Profiler::stop(int entryIndex) {
    if (entryIndex < 0 || entryIndex >= static_cast<int>(entries_.size())) return 0;
    auto& entry = entries_[entryIndex];
    if (!entry.active) return 0;

    entry.endTimeNs = nowNs();
    entry.active = false;
    entry.durationNs = entry.endTimeNs - entry.startTimeNs - overheadNs_;

    auto& summ = summaries_[entry.name];
    summ.totalTimeNs += entry.durationNs;
    if (entry.durationNs < summ.minTimeNs) summ.minTimeNs = entry.durationNs;
    if (entry.durationNs > summ.maxTimeNs) summ.maxTimeNs = entry.durationNs;
    summ.avgTimeNs = summ.totalTimeNs / summ.callCount;

    return entry.durationNs;
}

// ====== Scoped Timer ======

Profiler::ScopeTimer::ScopeTimer(const std::string& name) : name_(name) {
    startNs_ = nowNs();
}

Profiler::ScopeTimer::~ScopeTimer() {
    if (!stopped_) {
        auto duration = nowNs() - startNs_;
        auto& summ = Profiler::instance().summaries_[name_];
        summ.name = name_;
        summ.callCount++;
        summ.totalTimeNs += duration;
        if (duration < summ.minTimeNs) summ.minTimeNs = duration;
        if (duration > summ.maxTimeNs) summ.maxTimeNs = duration;
        summ.avgTimeNs = summ.totalTimeNs / summ.callCount;
    }
}

int64_t Profiler::ScopeTimer::elapsedNs() const {
    return nowNs() - startNs_;
}

// ====== Memory Tracking ======

void Profiler::recordAlloc(int64_t bytes) {
    trackedMemory_ += bytes;
    allocCount_++;
}

void Profiler::recordDealloc(int64_t bytes) {
    trackedMemory_ -= bytes;
    if (trackedMemory_ < 0) trackedMemory_ = 0;
    deallocCount_++;
}

MemorySnapshot Profiler::takeMemorySnapshot(const std::string& label) {
    MemorySnapshot snap;
    snap.timestampNs = nowNs();
    snap.heapSize = trackedMemory_ * 2;  // rough estimate: heap ~ 2x tracked
    snap.allocatedBytes = trackedMemory_;
    snap.deallocatedBytes = 0;           // set after comparison
    snap.allocateCount = allocCount_;
    snap.deallocateCount = deallocCount_;
    snap.liveObjects = allocCount_ - deallocCount_;
    if (allocCount_ > 0) {
        snap.fragments = deallocCount_;
        snap.fragmentationPercent =
            static_cast<double>(deallocCount_) / allocCount_ * 100.0;
    }
    snap.label = label;
    memorySnapshots_.push_back(snap);
    return snap;
}

// ====== Reporting ======

ProfileReport Profiler::generateReport() const {
    ProfileReport report;
    report.isProfiling = profiling_;
    report.reportTimeNs = nowNs();
    report.overheadNs = overheadNs_;

    int64_t total = 0;
    for (const auto& [name, summ] : summaries_) {
        total += summ.totalTimeNs;
        report.entries.push_back(summ);
        if (summ.callCount > 0 && !summaries_.empty()) {
            // Check if entry is still active
        }
    }

    report.totalTimeNs = total;
    report.activeEntries = getActiveCount();

    // Calculate percentages
    for (auto& entry : report.entries) {
        if (total > 0) {
            entry.percentTotal = (static_cast<double>(entry.totalTimeNs) / total) * 100.0;
        }
    }

    // Sort by total time (descending)
    std::sort(report.entries.begin(), report.entries.end(),
        [](const ProfileSummary& a, const ProfileSummary& b) {
            return a.totalTimeNs > b.totalTimeNs;
        });

    return report;
}

// ====== Formatting ======

std::string Profiler::formatNs(int64_t ns) {
    if (ns < 1000) return std::to_string(ns) + "ns";
    double us = ns / 1000.0;
    if (us < 1000) {
        std::ostringstream os; os << std::fixed << std::setprecision(1) << us; return os.str() + "μs";
    }
    double ms = us / 1000.0;
    if (ms < 1000) {
        std::ostringstream os; os << std::fixed << std::setprecision(2) << ms; return os.str() + "ms";
    }
    double sec = ms / 1000.0;
    std::ostringstream os; os << std::fixed << std::setprecision(3) << sec; return os.str() + "s";
}

std::string Profiler::formatBytes(int64_t bytes) {
    if (bytes < 1024) return std::to_string(bytes) + " B";
    double kb = bytes / 1024.0;
    if (kb < 1024) { std::ostringstream os; os << std::fixed << std::setprecision(1) << kb; return os.str() + " KB"; }
    double mb = kb / 1024.0;
    if (mb < 1024) { std::ostringstream os; os << std::fixed << std::setprecision(2) << mb; return os.str() + " MB"; }
    double gb = mb / 1024.0;
    std::ostringstream os; os << std::fixed << std::setprecision(2) << gb; return os.str() + " GB";
}

std::string Profiler::reportToJson() const {
    auto report = generateReport();
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    std::ostringstream os;
    os << R"({"total_time_ns":)" << report.totalTimeNs
       << R"(,"total_time_fmt":")" << formatNs(report.totalTimeNs)
       << R"(","overhead_ns":)" << report.overheadNs
       << R"(,"is_profiling":)" << (report.isProfiling ? "true" : "false")
       << R"(,"active_entries":)" << report.activeEntries
       << R"(,"tracked_memory":)" << trackedMemory_
       << R"(,"memory_fmt":")" << formatBytes(trackedMemory_)
       << R"(","alloc_count":)" << allocCount_
       << R"(,"dealloc_count":)" << deallocCount_
       << R"(,"entries":[)";

    bool first = true;
    for (const auto& e : report.entries) {
        if (!first) os << ","; first = false;
        os << R"({"name":")" << esc(e.name)
           << R"(","calls":)" << e.callCount
           << R"(,"total_ns":)" << e.totalTimeNs
           << R"(,"total_fmt":")" << formatNs(e.totalTimeNs)
           << R"(","avg_ns":)" << e.avgTimeNs
           << R"(,"min_ns":)" << e.minTimeNs
           << R"(,"max_ns":)" << e.maxTimeNs
           << R"(,"percent":)" << e.percentTotal
           << "}";
    }
    os << R"(],"snapshots":[)";

    first = true;
    for (const auto& s : memorySnapshots_) {
        if (!first) os << ","; first = false;
        os << R"({"ts":)" << s.timestampNs
           << R"(,"bytes":)" << s.allocatedBytes
           << R"(,"label":")" << esc(s.label) << R"(")";
        os << "}";
    }
    os << "]}";
    return os.str();
}

std::string Profiler::reportToText() const {
    auto report = generateReport();
    std::ostringstream os;

    os << "=== Progressive Chat Native Profiler ===\n";
    os << "Profiling: " << (report.isProfiling ? "ACTIVE" : "stopped") << "\n";
    os << "Total time: " << formatNs(report.totalTimeNs) << "\n";
    os << "Overhead: " << formatNs(report.overheadNs) << "\n";
    os << "Memory: " << formatBytes(trackedMemory_) << " (" << allocCount_ << " allocs, " << deallocCount_ << " deallocs)\n";
    os << "\n";

    if (report.entries.empty()) {
        os << "No profiled entries.\n";
    } else {
        os << "Top entries by total time:\n";
        os << "  NAME                          CALLS    TOTAL       AVG        MIN        MAX        %\n";
        os << "  ----                          -----    -----       ---        ---        ---        -\n";

        int shown = 0;
        for (const auto& e : report.entries) {
            if (shown >= 20) break; // Top 20
            shown++;

            // Align name to 30 chars
            std::string name = e.name;
            if (name.size() > 30) name = name.substr(0, 27) + "...";
            while (name.size() < 30) name += ' ';

            os << "  " << name
               << std::setw(8) << e.callCount << " "
               << std::setw(10) << formatNs(e.totalTimeNs) << " "
               << std::setw(10) << formatNs(e.avgTimeNs) << " "
               << std::setw(10) << formatNs(e.minTimeNs) << " "
               << std::setw(10) << formatNs(e.maxTimeNs) << " "
               << std::fixed << std::setprecision(1) << std::setw(6) << e.percentTotal << "%\n";
        }
    }

    return os.str();
}

std::string Profiler::summaryToJson(const ProfileSummary& s) const {
    std::ostringstream os;
    os << R"({"name":")" << s.name
       << R"(","calls":)" << s.callCount
       << R"(,"total_ns":)" << s.totalTimeNs
       << R"(,"avg_ns":)" << s.avgTimeNs
       << R"(,"min_ns":)" << s.minTimeNs
       << R"(,"max_ns":)" << s.maxTimeNs;
    if (s.percentTotal > 0) os << R"(,"percent":)" << s.percentTotal;
    os << "}";
    return os.str();
}

ProfileSummary Profiler::getSummary(const std::string& name) const {
    auto it = summaries_.find(name);
    if (it != summaries_.end()) return it->second;
    ProfileSummary empty;
    empty.name = name;
    return empty;
}

int64_t Profiler::totalProfiledTime() const {
    int64_t total = 0;
    for (const auto& [name, s] : summaries_) total += s.totalTimeNs;
    return total;
}

int Profiler::getActiveCount() const {
    int count = 0;
    for (const auto& e : entries_) if (e.active) count++;
    return count;
}

// ====== User Action Timing ======

int Profiler::startAction(const std::string& actionName, const std::string& context, bool isCold) {
    UserActionMeasurement m;
    m.actionName = actionName;
    m.startNs = nowNs();
    m.context = context;
    m.isCold = isCold;
    m.withinBudget = true;

    // Find matching budget
    for (const auto& b : budgets_) {
        if (actionName == b.actionPattern ||
            (b.actionPattern.back() == '*' && actionName.rfind(b.actionPattern.substr(0, b.actionPattern.size()-1), 0) == 0)) {
            m.budgetNs = b.budgetNs;
            break;
        }
    }

    actions_.push_back(m);
    return static_cast<int>(actions_.size()) - 1;
}

int64_t Profiler::stopAction(int actionIndex) {
    if (actionIndex < 0 || actionIndex >= static_cast<int>(actions_.size())) return 0;

    auto& m = actions_[actionIndex];
    if (m.completed) return m.durationNs;

    m.endNs = nowNs();
    m.durationNs = m.endNs - m.startNs - overheadNs_;
    m.completed = true;
    m.withinBudget = (m.durationNs <= m.budgetNs);

    // Update action stats
    auto& stats = actionStats_[m.actionName];
    stats.actionName = m.actionName;
    stats.budgetNs = m.budgetNs;
    stats.runCount++;
    stats.totalNs += m.durationNs;
    if (m.durationNs < stats.minNs) stats.minNs = m.durationNs;
    if (m.durationNs > stats.maxNs) stats.maxNs = m.durationNs;
    stats.avgNs = stats.totalNs / stats.runCount;
    if (!m.withinBudget) stats.overBudgetCount++;

    // Store in history (up to 100 entries)
    stats.history.push_back(m.durationNs);
    if (static_cast<int>(stats.history.size()) > 100) {
        stats.history.erase(stats.history.begin());
    }

    // Cold measurement
    if (m.isCold) {
        stats.hasColdMeasurement = true;
        stats.coldDurationNs = m.durationNs;
    }

    // Compute percentiles
    computePercentiles(stats);

    return m.durationNs;
}

int64_t Profiler::measureAction(const std::string& actionName, int64_t startNs) {
    int idx = startAction(actionName);
    // Override start time
    if (idx >= 0 && idx < static_cast<int>(actions_.size())) {
        actions_[idx].startNs = startNs;
    }
    return stopAction(idx);
}

void Profiler::setActionBudget(const std::string& actionPattern, int64_t budgetNs) {
    ActionBudget b;
    b.actionPattern = actionPattern;
    b.budgetNs = budgetNs;
    budgets_.push_back(b);
}

bool Profiler::lastActionWithinBudget(const std::string& actionName) const {
    for (auto it = actions_.rbegin(); it != actions_.rend(); ++it) {
        if (it->actionName == actionName) return it->withinBudget;
    }
    return true; // No measurement = no violation
}

void Profiler::computePercentiles(ActionStats& stats) {
    if (stats.history.empty()) return;

    auto sorted = stats.history;
    std::sort(sorted.begin(), sorted.end());
    size_t n = sorted.size();

    stats.p50Ns = sorted[n * 50 / 100];
    stats.p90Ns = sorted[n * 90 / 100];
    stats.p95Ns = sorted[n * 95 / 100];
    stats.p99Ns = sorted[n * 99 / 100];
}

ActionStats Profiler::getActionStats(const std::string& actionName) const {
    auto it = actionStats_.find(actionName);
    if (it != actionStats_.end()) return it->second;
    ActionStats empty;
    empty.actionName = actionName;
    return empty;
}

std::vector<ActionStats> Profiler::getAllActionStats() const {
    std::vector<ActionStats> result;
    for (const auto& [name, stats] : actionStats_) result.push_back(stats);
    std::sort(result.begin(), result.end(), [](const ActionStats& a, const ActionStats& b) {
        return a.avgNs > b.avgNs; // Slowest first
    });
    return result;
}

// ====== Frame Timing ======

int Profiler::startFrame() {
    FrameTiming f;
    f.frameNumber = ++frameCount_;
    f.frameStartNs = nowNs();
    frames_.push_back(f);
    if (frames_.size() > 600) frames_.erase(frames_.begin()); // Keep last 10 sec @ 60fps
    return static_cast<int>(frames_.size()) - 1;
}

int64_t Profiler::endFrame(int frameIndex) {
    if (frameIndex < 0 || frameIndex >= static_cast<int>(frames_.size())) return 0;
    auto& f = frames_[frameIndex];
    f.frameEndNs = nowNs();
    f.frameDurationNs = f.frameEndNs - f.frameStartNs;
    f.dropped = (f.frameDurationNs > 16666667LL); // > 16.67ms = 60fps threshold
    return f.frameDurationNs;
}

std::vector<FrameTiming> Profiler::getFrameTimings(int lastN) const {
    std::vector<FrameTiming> result;
    int start = std::max(0, static_cast<int>(frames_.size()) - lastN);
    for (int i = start; i < static_cast<int>(frames_.size()); i++) result.push_back(frames_[i]);
    return result;
}

double Profiler::getCurrentFps() const {
    if (frames_.size() < 2) return 0.0;
    // Use last 60 frames
    int count = std::min(120, static_cast<int>(frames_.size()));
    auto first = frames_[frames_.size() - count];
    auto last = frames_.back();
    if (last.frameEndNs <= first.frameStartNs) return 0.0;
    double elapsedSec = (last.frameEndNs - first.frameStartNs) / 1e9;
    return count / elapsedSec;
}

// ====== Action Reporting ======

std::string Profiler::actionReportToJson() const {
    auto stats = getAllActionStats();
    std::ostringstream os;
    os << R"({"actions":[)";
    for (size_t i = 0; i < stats.size(); i++) {
        if (i > 0) os << ",";
        os << R"({"name":")" << stats[i].actionName
           << R"(","runs":)" << stats[i].runCount
           << R"(,"avg_ms":)" << (stats[i].avgNs / 1000000.0)
           << R"(,"p50_ms":)" << (stats[i].p50Ns / 1000000.0)
           << R"(,"p90_ms":)" << (stats[i].p90Ns / 1000000.0)
           << R"(,"p99_ms":)" << (stats[i].p99Ns / 1000000.0)
           << R"(,"min_ms":)" << (stats[i].minNs / 1000000.0)
           << R"(,"max_ms":)" << (stats[i].maxNs / 1000000.0)
           << R"(,"over_budget":)" << stats[i].overBudgetCount
           << R"(,"budget_ms":)" << (stats[i].budgetNs / 1000000.0);
        if (stats[i].hasColdMeasurement)
            os << R"(,"cold_ms":)" << (stats[i].coldDurationNs / 1000000.0);
        os << "}";
    }
    os << R"(],"frame_fps":)" << getCurrentFps()
       << R"(,"frame_count":)" << static_cast<int>(frames_.size())
       << "}";
    return os.str();
}

std::string Profiler::actionReportToText() const {
    auto stats = getAllActionStats();
    std::ostringstream os;
    os << "=== User Action Performance ===\n\n";

    if (stats.empty()) {
        os << "No actions measured yet.\n";
        os << "Usage: Profiler::startAction(\"back_to_menu\"); ... Profiler::stopAction(idx);\n";
    } else {
        os << "ACTION                  RUNS   AVG      P50      P90      P99      BUDGET   VIOLATIONS\n";
        os << "------                  ----   ---      ---      ---      ---      ------   ----------\n";
        for (const auto& s : stats) {
            std::string name = s.actionName;
            if (name.size() > 22) name = name.substr(0, 19) + "...";
            while (name.size() < 22) name += ' ';

            os << name
               << std::setw(6) << s.runCount << " "
               << std::setw(8) << formatNs(s.avgNs) << " "
               << std::setw(8) << formatNs(s.p50Ns) << " "
               << std::setw(8) << formatNs(s.p90Ns) << " "
               << std::setw(8) << formatNs(s.p99Ns) << " "
               << std::setw(8) << formatNs(s.budgetNs) << " ";
            if (s.overBudgetCount > 0) {
                os << "\033[31m" << s.overBudgetCount << " !!\033[0m";
            } else {
                os << "OK";
            }
            os << "\n";
        }
    }

    // Frame stats
    double fps = getCurrentFps();
    os << "\nFrame rate: " << std::fixed << std::setprecision(1) << fps << " FPS";
    if (fps < 55.0) os << " \033[31m(LAG!)\033[0m";

    return os.str();
}

// ====== Real-Time Overlay (HUD) ======

std::string Profiler::realTimeSnapshotJson() const {
    auto hot = const_cast<Profiler*>(this)->getAllActionStats();
    auto violations = const_cast<Profiler*>(this)->recentViolations(30);
    double fps = getCurrentFps();

    std::ostringstream os;
    os << R"({"fps":)" << std::fixed << std::setprecision(1) << fps
       << R"(,"fps_color":")" << colorForFps(fps) << R"(")"
       << R"(,"memory_bytes":)" << trackedMemory_
       << R"(,"memory_mb":)" << std::fixed << std::setprecision(1) << (trackedMemory_ / 1048576.0)
       << R"(,"active_actions":)" << getActiveCount()
       << R"(,"violations":)" << static_cast<int>(violations.size())
       << R"(,"violation_list":[)";
    for (size_t i = 0; i < violations.size() && i < 5; i++) {
        if (i > 0) os << ","; os << "\"" << violations[i] << "\"";
    }
    os << R"(],"hot_actions":[)";
    int shown = 0;
    for (size_t i = 0; i < hot.size() && shown < 3; i++) {
        if (hot[i].runCount == 0) continue;
        if (shown > 0) os << ",";
        int64_t lastMs = hot[i].history.empty() ? 0 : hot[i].history.back() / 1000000;
        os << R"({"name":")" << hot[i].actionName
           << R"(","last_ms":)" << lastMs
           << R"(,"avg_ms":)" << (hot[i].avgNs / 1000000)
           << R"(,"color":")" << colorForDuration(hot[i].avgNs, hot[i].budgetNs) << R"(")"
           << R"(,"over_budget":)" << hot[i].overBudgetCount << "}";
        shown++;
    }
    os << R"(],"drops":)" << static_cast<int>(std::count_if(frames_.begin(), frames_.end(),
        [](const FrameTiming& f) { return f.dropped; }))
       << R"(,"total_frames":)" << static_cast<int>(frames_.size())
       << "}";
    return os.str();
}

std::string Profiler::realTimeSnapshotText() const {
    double fps = getCurrentFps();
    auto violations = const_cast<Profiler*>(this)->recentViolations(30);

    std::ostringstream os;
    os << std::fixed << std::setprecision(1) << fps << " FPS";
    if (!violations.empty()) os << " [" << violations.size() << "!]";
    os << " | " << (trackedMemory_ / 1048576.0) << " MB";
    return os.str();
}

std::vector<ActionStats> Profiler::hotActions() const {
    auto all = const_cast<Profiler*>(this)->getAllActionStats();
    if (all.size() > 3) all.resize(3);
    return all;
}

std::vector<std::string> Profiler::recentViolations(int windowSec) const {
    std::vector<std::string> result;
    int64_t cutoff = nowNs() - windowSec * 1000000000LL;
    for (auto it = actions_.rbegin(); it != actions_.rend(); ++it) {
        if (it->endNs < cutoff) break;
        if (!it->withinBudget && it->completed) {
            result.push_back(it->actionName);
        }
    }
    // Remove duplicates
    std::sort(result.begin(), result.end());
    result.erase(std::unique(result.begin(), result.end()), result.end());
    return result;
}

std::string Profiler::colorForDuration(int64_t durationNs, int64_t budgetNs) {
    if (budgetNs <= 0) return "#2196F3"; // No budget set

    double ratio = static_cast<double>(durationNs) / budgetNs;
    if (ratio <= 0.5) return "#4CAF50";   // Green: well under budget
    if (ratio <= 0.8) return "#FFC107";   // Yellow: approaching budget
    if (ratio <= 1.0) return "#FF9800";   // Orange: near budget
    return "#F44336";                      // Red: over budget
}

std::string Profiler::colorForFps(double fps) {
    if (fps >= 55.0) return "#4CAF50";   // Green: smooth
    if (fps >= 30.0) return "#FFC107";   // Yellow: acceptable
    if (fps >= 15.0) return "#FF9800";   // Orange: laggy
    return "#F44336";                      // Red: slideshow
}

// ================================================================
// ProfileScope — RAII
// ================================================================

ProfileScope::ProfileScope(const std::string& name,
    const std::string& parentName,
    const std::string& metadata)
    : name_(name)
    , startNs_(Profiler::nowNs())
{
    entryIndex_ = Profiler::instance().start(name, parentName, metadata);
}

ProfileScope::~ProfileScope() {
    if (!stopped_) {
        Profiler::instance().stop(entryIndex_);
        stopped_ = true;
    }
}

int64_t ProfileScope::elapsedNs() const {
    return Profiler::nowNs() - startNs_;
}

// ================================================================
// ProfilerSession
// ================================================================

void ProfilerSession::startProfiling() {
    profiling_ = true;
}

void ProfilerSession::stopProfiling() {
    profiling_ = false;
}

void ProfilerSession::reset() {
    entries_.clear();
    summaries_.clear();
    profiling_ = false;
}

int ProfilerSession::start(const std::string& name, const std::string& parentName) {
    ProfilerEntry entry;
    entry.name = name;
    entry.startTimeNs = Profiler::nowNs();
    entry.active = true;
    entry.callCount = 1;
    entry.parentName = parentName;
    entry.threadId = static_cast<int>(
        std::hash<std::thread::id>{}(std::this_thread::get_id()));

    auto& summ = summaries_[name];
    summ.name = name;
    summ.callCount++;

    entries_.push_back(entry);
    return static_cast<int>(entries_.size()) - 1;
}

int64_t ProfilerSession::stop(const std::string& name) {
    for (auto it = entries_.rbegin(); it != entries_.rend(); ++it) {
        if (it->active && it->name == name) {
            it->endTimeNs = Profiler::nowNs();
            it->active = false;
            it->durationNs = it->endTimeNs - it->startTimeNs;

            auto& summ = summaries_[name];
            summ.totalTimeNs += it->durationNs;
            if (it->durationNs < summ.minTimeNs) summ.minTimeNs = it->durationNs;
            if (it->durationNs > summ.maxTimeNs) summ.maxTimeNs = it->durationNs;
            summ.avgTimeNs = summ.totalTimeNs / summ.callCount;

            return it->durationNs;
        }
    }
    return 0;
}

std::vector<ProfilerEntry> ProfilerSession::getEntries() const {
    return entries_;
}

int64_t ProfilerSession::getTotalTime() const {
    int64_t total = 0;
    for (const auto& [name, s] : summaries_) total += s.totalTimeNs;
    return total;
}

int64_t ProfilerSession::getEntryTime(const std::string& name) const {
    auto it = summaries_.find(name);
    if (it != summaries_.end()) return it->second.totalTimeNs;
    return 0;
}

std::vector<ProfileSummary> ProfilerSession::getHotPaths() const {
    std::vector<ProfileSummary> result;
    for (const auto& [name, s] : summaries_) {
        result.push_back(s);
    }
    // Sort by total time descending
    std::sort(result.begin(), result.end(),
        [](const ProfileSummary& a, const ProfileSummary& b) {
            return a.totalTimeNs > b.totalTimeNs;
        });
    return result;
}

// ================================================================
// ProfilerStats
// ================================================================

ProfilerStats computeProfilerStats(const ProfileSummary& summary) {
    ProfilerStats s;
    s.name = summary.name;
    s.totalCalls = summary.callCount;
    s.totalTimeMs = summary.totalTimeNs / 1000000.0;
    s.avgTimeMs = summary.avgTimeNs / 1000000.0;
    s.minTimeMs = summary.minTimeNs / 1000000.0;
    s.maxTimeMs = summary.maxTimeNs / 1000000.0;
    return s;
}

// ================================================================
// Memory Snapshot Utilities
// ================================================================

MemorySnapshotDelta compareMemorySnapshots(
    const MemorySnapshot& older,
    const MemorySnapshot& newer)
{
    MemorySnapshotDelta delta;
    delta.elapsedNs = newer.timestampNs - older.timestampNs;
    delta.heapDelta = newer.heapSize - older.heapSize;
    delta.allocDelta = newer.allocatedBytes - older.allocatedBytes;
    delta.deallocDelta = newer.deallocatedBytes - older.deallocatedBytes;
    delta.liveObjectsDelta = newer.liveObjects - older.liveObjects;
    delta.fragmentationDelta = newer.fragmentationPercent - older.fragmentationPercent;
    return delta;
}

std::vector<MemoryLeakInfo> detectMemoryLeaks(
    const std::vector<MemorySnapshot>& snapshots,
    int64_t growthThresholdBytes)
{
    std::vector<MemoryLeakInfo> leaks;
    if (snapshots.size() < 2) return leaks;

    // Compare consecutive snapshots
    for (size_t i = 1; i < snapshots.size(); i++) {
        auto delta = compareMemorySnapshots(snapshots[i - 1], snapshots[i]);
        if (delta.heapDelta > growthThresholdBytes) {
            MemoryLeakInfo leak;
            leak.allocationSite = snapshots[i].label;
            leak.bytesLeaked = delta.heapDelta;
            leak.allocationsLeaked = delta.allocDelta > 0 ?
                static_cast<int>(delta.allocDelta) : 0;
            leak.firstSeen = snapshots[i].timestampNs;
            if (delta.elapsedNs > 0) {
                leak.growthRate = static_cast<double>(delta.heapDelta) /
                    (delta.elapsedNs / 1000000000.0);
            }
            leaks.push_back(leak);
        }
    }

    return leaks;
}

// ================================================================
// Performance Thresholds
// ================================================================

std::vector<PerformanceThreshold> checkPerformanceThresholds(
    const std::vector<PerformanceThreshold>& thresholds,
    const std::unordered_map<std::string, double>& currentValues)
{
    std::vector<PerformanceThreshold> results;
    for (auto t : thresholds) {
        auto it = currentValues.find(t.metric);
        if (it != currentValues.end()) {
            t.currentValue = it->second;
            t.exceeded = it->second > t.thresholdValue;
            if (t.exceeded) {
                std::ostringstream msg;
                msg << t.metric << " exceeded threshold: "
                    << it->second << " > " << t.thresholdValue
                    << " (" << t.severity << ")";
                t.message = msg.str();
            }
        }
        results.push_back(t);
    }
    return results;
}

// ================================================================
// Flame Graph Generation
// ================================================================

std::string generateProfilerFlameGraph(const Profiler& profiler) {
    auto report = profiler.generateReport();
    std::ostringstream os;
    // Flamegraph JSON format: array of stack frames
    // Each frame: {name, value, children[]}
    // We flatten profiler entries into this format.

    os << R"({"name":"root","value":)" << report.totalTimeNs
       << R"(,"children":[)";

    bool first = true;
    for (const auto& entry : report.entries) {
        if (entry.callCount == 0) continue;
        if (!first) os << ","; first = false;
        os << R"({"name":")" << entry.name
           << R"(","value":)" << entry.totalTimeNs
           << R"(,"children":[]})";
    }

    os << "]}";
    return os.str();
}

std::string generateProfilerFlameGraph(const ProfilerSession& session) {
    auto entries = session.getEntries();
    auto totalTime = session.getTotalTime();
    std::ostringstream os;

    os << R"({"name":"root","value":)" << totalTime
       << R"(,"children":[)";

    bool first = true;
    std::unordered_map<std::string, int64_t> aggTimes;
    for (const auto& e : entries) {
        aggTimes[e.name] += e.durationNs;
    }

    for (const auto& [name, total] : aggTimes) {
        if (!first) os << ","; first = false;
        os << R"({"name":")" << name
           << R"(","value":)" << total
           << R"(,"children":[]})";
    }

    os << "]}";
    return os.str();
}

} // namespace progressive
