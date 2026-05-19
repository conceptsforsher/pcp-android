#include "progressive/lightweight_call.hpp"
#include <sstream>
#include <fstream>

namespace progressive {

// ---- Memory Assessment ----

int64_t LightweightCallManager::getAvailableRamKb() {
    std::ifstream mem("/proc/meminfo");
    if (!mem.is_open()) return 0;

    std::string line;
    while (std::getline(mem, line)) {
        if (line.rfind("MemAvailable:", 0) == 0) {
            auto pos = line.find(':');
            if (pos != std::string::npos) {
                auto val = line.substr(pos + 1);
                while (!val.empty() && (val.front() == ' ' || val.front() == '\t')) val.erase(0, 1);
                auto kbPos = val.find("kB");
                if (kbPos != std::string::npos) val = val.substr(0, kbPos);
                while (!val.empty() && val.back() == ' ') val.pop_back();
                try { return std::stoll(val); }
                catch (...) { return 0; }
            }
        }
    }
    return 0;
}

int64_t LightweightCallManager::getProcessRssKb() {
    std::ifstream status("/proc/self/status");
    if (!status.is_open()) return 0;

    std::string line;
    while (std::getline(status, line)) {
        if (line.rfind("VmRSS:", 0) == 0) {
            auto pos = line.find(':');
            if (pos != std::string::npos) {
                auto val = line.substr(pos + 1);
                while (!val.empty() && (val.front() == ' ' || val.front() == '\t')) val.erase(0, 1);
                auto kbPos = val.find("kB");
                if (kbPos != std::string::npos) val = val.substr(0, kbPos);
                while (!val.empty() && val.back() == ' ') val.pop_back();
                try { return std::stoll(val); }
                catch (...) { return 0; }
            }
        }
    }
    return 0;
}

MemoryState LightweightCallManager::assessMemory() {
    MemoryState state;
    state.totalKb = getAvailableRamKb() + getProcessRssKb(); // rough estimate
    state.availableKb = getAvailableRamKb();
    state.usedKb = getProcessRssKb();

    // Low memory: < 256MB available
    state.isLowMemory = (state.availableKb < 256 * 1024);

    // Need lightweight mode if:
    // - Available RAM < 512MB AND process uses > 200MB
    state.needsLightweightMode = shouldUseLightweightMode(state);

    return state;
}

bool LightweightCallManager::shouldUseLightweightMode(const MemoryState& state) {
    // Use lightweight call if:
    // 1. Total device RAM < 2GB
    // 2. OR process RSS > 200MB
    // 3. OR available RAM < 512MB
    if (state.totalKb < 2 * 1024 * 1024) return true; // < 2GB total
    if (state.usedKb > 200 * 1024) return true;       // process > 200MB
    if (state.availableKb < 512 * 1024) return true;   // < 512MB free
    return false;
}

int64_t LightweightCallManager::estimateFreedBytes(CleanupAction action) {
    switch (action) {
        case CleanupAction::ClearImageCache:       return 50 * 1024 * 1024;  // ~50MB
        case CleanupAction::ClearTimelineViews:    return 30 * 1024 * 1024;  // ~30MB
        case CleanupAction::ClearComposerState:    return 10 * 1024 * 1024;  // ~10MB
        case CleanupAction::SuspendBackgroundSync: return 15 * 1024 * 1024;  // ~15MB (incoming events)
        case CleanupAction::ClearAvatarCache:      return 8 * 1024 * 1024;   // ~8MB
        case CleanupAction::ClearReactionCache:    return 3 * 1024 * 1024;   // ~3MB
        case CleanupAction::TrimMemoryAggressive:  return 40 * 1024 * 1024;  // ~40MB (GC)
        case CleanupAction::ClearWebViewCache:     return 25 * 1024 * 1024;  // ~25MB
        case CleanupAction::ClearMediaPlayer:      return 5 * 1024 * 1024;   // ~5MB
        case CleanupAction::DetachNonVisibleFrags: return 15 * 1024 * 1024;  // ~15MB
        default: return 0;
    }
}

CleanupPlan LightweightCallManager::computeCleanupPlan(const MemoryState& state) {
    CleanupPlan plan;

    // Priority-ordered cleanup based on memory pressure
    if (state.needsLightweightMode) {
        plan.actions = {
            CleanupAction::ClearImageCache,
            CleanupAction::ClearTimelineViews,
            CleanupAction::ClearComposerState,
            CleanupAction::SuspendBackgroundSync,
            CleanupAction::ClearAvatarCache,
            CleanupAction::ClearReactionCache,
            CleanupAction::TrimMemoryAggressive,
            CleanupAction::ClearWebViewCache,
            CleanupAction::ClearMediaPlayer,
            CleanupAction::DetachNonVisibleFrags,
        };
        plan.priority = 1;
    } else if (state.isLowMemory) {
        plan.actions = {
            CleanupAction::ClearImageCache,
            CleanupAction::ClearTimelineViews,
            CleanupAction::TrimMemoryAggressive,
            CleanupAction::ClearAvatarCache,
            CleanupAction::SuspendBackgroundSync,
        };
        plan.priority = 2;
    } else {
        // Minimal cleanup even on normal devices
        plan.actions = {
            CleanupAction::ClearImageCache,
            CleanupAction::TrimMemoryAggressive,
        };
        plan.priority = 3;
    }

    // Compute estimated freed bytes
    plan.estimatedBytesFreed = 0;
    for (auto action : plan.actions) {
        plan.estimatedBytesFreed += estimateFreedBytes(action);
    }

    return plan;
}

CleanupPlan LightweightCallManager::enterCallMode() {
    auto state = assessMemory();
    preCallState_ = state;
    isInCallMode_ = true;
    return computeCleanupPlan(state);
}

CleanupPlan LightweightCallManager::exitCallMode() {
    isInCallMode_ = false;
    // Return minimal restore plan
    CleanupPlan plan;
    plan.actions = {
        CleanupAction::SuspendBackgroundSync, // this one needs restoration
    };
    plan.priority = 0;
    plan.estimatedBytesFreed = 0;
    return plan;
}

std::string LightweightCallManager::planToJson(const CleanupPlan& plan) {
    std::ostringstream json;
    json << "{";
    json << R"("actions": [)";
    for (size_t i = 0; i < plan.actions.size(); ++i) {
        if (i > 0) json << ",";
        json << R"(")" << static_cast<int>(plan.actions[i]) << R"(")";
    }
    json << "],";
    json << R"("estimatedBytesFreed": )" << plan.estimatedBytesFreed << ",";
    json << R"("priority": )" << plan.priority;
    json << "}";
    return json.str();
}

std::string LightweightCallManager::memoryStateToJson(const MemoryState& state) {
    std::ostringstream json;
    json << "{";
    json << R"("availableKb": )" << state.availableKb << ",";
    json << R"("totalKb": )" << state.totalKb << ",";
    json << R"("usedKb": )" << state.usedKb << ",";
    json << R"("isLowMemory": )" << (state.isLowMemory ? "true" : "false") << ",";
    json << R"("needsLightweightMode": )" << (state.needsLightweightMode ? "true" : "false");
    json << "}";
    return json.str();
}

} // namespace progressive
