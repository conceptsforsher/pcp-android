#include "progressive/chat_tools.hpp"
#include <sstream>
#include <algorithm>
#include <chrono>

namespace progressive {

// ---- UserHideManager ----

void UserHideManager::hideFor(const std::string& userId, const std::string& displayName, int minutes) {
    // Remove existing hide for this user
    entries_.erase(std::remove_if(entries_.begin(), entries_.end(),
        [&](const UserHideEntry& e) { return e.userId == userId; }
    ), entries_.end());

    UserHideEntry entry;
    entry.userId = userId;
    entry.displayName = displayName;
    entry.remainingSeconds = minutes * 60;
    entry.hiddenUntilMs = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count() + (minutes * 60 * 1000LL);
    entries_.push_back(entry);
}

bool UserHideManager::isHidden(const std::string& userId) const {
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    for (const auto& e : entries_) {
        if (e.userId == userId && e.hiddenUntilMs > now) return true;
    }
    return false;
}

int UserHideManager::getRemainingSeconds(const std::string& userId) const {
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    for (const auto& e : entries_) {
        if (e.userId == userId && e.hiddenUntilMs > now) {
            return static_cast<int>((e.hiddenUntilMs - now) / 1000);
        }
    }
    return 0;
}

std::vector<UserHideEntry> UserHideManager::getActiveHides() const {
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    std::vector<UserHideEntry> result;
    for (const auto& e : entries_) {
        if (e.hiddenUntilMs > now) {
            auto copy = e;
            copy.remainingSeconds = static_cast<int>((e.hiddenUntilMs - now) / 1000);
            result.push_back(copy);
        }
    }
    return result;
}

void UserHideManager::cleanExpired() {
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    entries_.erase(std::remove_if(entries_.begin(), entries_.end(),
        [&](const UserHideEntry& e) { return e.hiddenUntilMs <= now; }
    ), entries_.end());
}

std::string UserHideManager::exportJson() const {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    auto active = getActiveHides();
    std::ostringstream json;
    json << "[";
    for (size_t i = 0; i < active.size(); ++i) {
        if (i > 0) json << ",";
        json << R"({"userId": ")" << esc(active[i].userId) << R"(")";
        json << R"(,"displayName": ")" << esc(active[i].displayName) << R"(")";
        json << R"(,"remainingSeconds": )" << active[i].remainingSeconds << "}";
    }
    json << "]";
    return json.str();
}

void UserHideManager::clear() {
    entries_.clear();
}

// ---- MessageQueue ----

void MessageQueue::enqueue(const QueuedMessage& msg) {
    queue_.push_back(msg);
}

void MessageQueue::setOrder(const std::string& msgId, int order) {
    for (auto& m : queue_) {
        if (m.msgId == msgId) {
            m.order = order;
            return;
        }
    }
}

void MessageQueue::markFailed(const std::string& msgId, const std::string& error) {
    for (auto& m : queue_) {
        if (m.msgId == msgId) {
            m.failed = true;
            m.lastError = error;
            m.retries++;
            return;
        }
    }
}

void MessageQueue::markSent(const std::string& msgId) {
    queue_.erase(std::remove_if(queue_.begin(), queue_.end(),
        [&](const QueuedMessage& m) { return m.msgId == msgId; }
    ), queue_.end());
}

const QueuedMessage* MessageQueue::getNext() const {
    // Find the non-failed message with lowest order, or first pending
    for (const auto& m : queue_) {
        if (!m.failed) return &m;
    }
    // Retry failed ones that haven't exceeded max retries
    for (const auto& m : queue_) {
        if (m.failed && m.retries < m.maxRetries) return &m;
    }
    return nullptr;
}

std::vector<QueuedMessage> MessageQueue::getAll() const {
    auto result = queue_;
    std::sort(result.begin(), result.end(), [](const QueuedMessage& a, const QueuedMessage& b) {
        return a.order < b.order;
    });
    return result;
}

int MessageQueue::pendingCount() const {
    return static_cast<int>(queue_.size());
}

void MessageQueue::clear() {
    queue_.clear();
}

std::string MessageQueue::exportJson() const {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    auto sorted = getAll();
    std::ostringstream json;
    json << "[";
    for (size_t i = 0; i < sorted.size(); ++i) {
        if (i > 0) json << ",";
        json << R"({"msgId": ")" << esc(sorted[i].msgId) << R"(")";
        json << R"(,"body": ")" << esc(sorted[i].body) << R"(")";
        json << R"(,"order": )" << sorted[i].order;
        json << R"(,"failed": )" << (sorted[i].failed ? "true" : "false");
        json << R"(,"retries": )" << sorted[i].retries << "}";
    }
    json << "]";
    return json.str();
}

// ---- Auto-Scroll ----

ScrollPlan computeScrollPlan(const AutoScrollConfig& config, int totalLines, int lineHeightPx) {
    ScrollPlan plan;
    plan.totalLines = totalLines;
    int totalPx = totalLines * lineHeightPx;

    if (totalLines <= 0 || config.durationMinutes <= 0) return plan;

    plan.linesPerMinute = totalLines / config.durationMinutes;
    plan.estimatedFullScrollMin = config.durationMinutes;

    if (config.smoothScroll) {
        // Smooth: calculate pixels per tick
        double totalMs = config.durationMinutes * 60.0 * 1000.0;
        plan.scrollPxPerTick = static_cast<int>((totalPx / totalMs) * 1000.0);
        if (plan.scrollPxPerTick < 1) plan.scrollPxPerTick = 1;
    } else {
        // Pages: calculate lines per jump
        int jumpsPerMinute = 60 / 2; // jump every 2 seconds
        plan.linesPerMinute = totalLines / config.durationMinutes;
    }

    return plan;
}

// ---- Image Crop ----

bool isValidCrop(int imgW, int imgH, int cropX, int cropY, int cropW, int cropH) {
    if (imgW <= 0 || imgH <= 0) return false;
    if (cropX < 0 || cropY < 0) return false;
    if (cropW <= 0 || cropH <= 0) return false;
    if (cropX + cropW > imgW) return false;
    if (cropY + cropH > imgH) return false;
    return true;
}

} // namespace progressive
