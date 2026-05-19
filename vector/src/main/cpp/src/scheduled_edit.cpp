#include "progressive/scheduled_edit.hpp"
#include <sstream>
#include <algorithm>
#include <chrono>
#include <regex>

namespace progressive {

std::string ScheduledEditQueue::generateId() {
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    std::ostringstream id;
    id << "sched_" << now << "_" << (nextId_++);
    return id.str();
}

std::string ScheduledEditQueue::schedule(const ScheduledEdit& edit) {
    ScheduledEdit copy = edit;
    if (copy.editId.empty()) copy.editId = generateId();
    if (copy.createdAtMs == 0) {
        copy.createdAtMs = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
    }
    queue_.push_back(copy);
    return copy.editId;
}

void ScheduledEditQueue::cancel(const std::string& editId) {
    queue_.erase(std::remove_if(queue_.begin(), queue_.end(),
        [&](const ScheduledEdit& e) { return e.editId == editId; }
    ), queue_.end());
}

std::vector<ScheduledEdit> ScheduledEditQueue::getDueEdits() {
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();

    std::vector<ScheduledEdit> due;
    for (const auto& e : queue_) {
        if (!e.applied && !e.failed && e.scheduledAtMs <= now) {
            due.push_back(e);
        }
    }
    // Sort by scheduled time (oldest first)
    std::sort(due.begin(), due.end(), [](const ScheduledEdit& a, const ScheduledEdit& b) {
        return a.scheduledAtMs < b.scheduledAtMs;
    });
    return due;
}

std::vector<ScheduledEdit> ScheduledEditQueue::getPendingEdits() const {
    std::vector<ScheduledEdit> result;
    for (const auto& e : queue_) {
        if (!e.applied) result.push_back(e);
    }
    std::sort(result.begin(), result.end(), [](const ScheduledEdit& a, const ScheduledEdit& b) {
        return a.scheduledAtMs < b.scheduledAtMs;
    });
    return result;
}

std::vector<ScheduledEdit> ScheduledEditQueue::getByRoom(const std::string& roomId) const {
    std::vector<ScheduledEdit> result;
    for (const auto& e : queue_) {
        if (e.roomId == roomId) result.push_back(e);
    }
    std::sort(result.begin(), result.end(), [](const ScheduledEdit& a, const ScheduledEdit& b) {
        return a.scheduledAtMs < b.scheduledAtMs;
    });
    return result;
}

void ScheduledEditQueue::markApplied(const std::string& editId) {
    for (auto& e : queue_) {
        if (e.editId == editId) {
            e.applied = true;
            return;
        }
    }
}

void ScheduledEditQueue::markFailed(const std::string& editId, const std::string& error) {
    for (auto& e : queue_) {
        if (e.editId == editId) {
            e.failed = true;
            e.lastError = error;
            return;
        }
    }
}

void ScheduledEditQueue::retry(const std::string& editId, int delayMinutes) {
    for (auto& e : queue_) {
        if (e.editId == editId) {
            e.failed = false;
            e.lastError.clear();
            e.scheduledAtMs = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()).count() + (delayMinutes * 60 * 1000LL);
            return;
        }
    }
}

EditQueueStats ScheduledEditQueue::getStats() const {
    EditQueueStats stats;
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();

    stats.totalEdits = static_cast<int>(queue_.size());

    for (const auto& e : queue_) {
        if (e.applied) stats.appliedEdits++;
        else if (e.failed) stats.failedEdits++;
        else {
            stats.pendingEdits++;
            if (stats.nextEditAtMs == 0 || e.scheduledAtMs < stats.nextEditAtMs) {
                stats.nextEditAtMs = e.scheduledAtMs;
                stats.nextEditId = e.editId;
            }
        }
    }

    return stats;
}

bool ScheduledEditQueue::isValidContentUrl(const std::string& url) {
    return url.rfind("http://", 0) == 0 || url.rfind("https://", 0) == 0;
}

std::string ScheduledEditQueue::editToJson(const ScheduledEdit& edit) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    std::ostringstream json;
    json << "{";
    json << R"("editId": ")" << esc(edit.editId) << R"(",)";
    json << R"("roomId": ")" << esc(edit.roomId) << R"(",)";
    json << R"("targetEventId": ")" << esc(edit.targetEventId) << R"(",)";
    json << R"("newContent": ")" << esc(edit.newContent) << R"(",)";
    json << R"("contentUrl": ")" << esc(edit.contentUrl) << R"(",)";
    json << R"("scheduledAtMs": )" << edit.scheduledAtMs << ",";
    json << R"("applied": )" << (edit.applied ? "true" : "false") << ",";
    json << R"("failed": )" << (edit.failed ? "true" : "false") << ",";
    json << R"("isRecurring": )" << (edit.isRecurring ? "true" : "false");
    if (!edit.lastError.empty()) json << R"(,"lastError": ")" << esc(edit.lastError) << R"(")";
    json << "}";
    return json.str();
}

std::string ScheduledEditQueue::exportJson() const {
    auto pending = getPendingEdits();
    std::ostringstream json;
    json << "[";
    for (size_t i = 0; i < pending.size(); ++i) {
        if (i > 0) json << ",";
        json << editToJson(pending[i]);
    }
    json << "]";
    return json.str();
}

void ScheduledEditQueue::importJson(const std::string& json) {
    size_t pos = 0;
    while (true) {
        pos = json.find("editId", pos);
        if (pos == std::string::npos) break;

        auto objStart = json.rfind('{', pos);
        if (objStart == std::string::npos) break;

        int depth = 0;
        auto objEnd = objStart;
        while (objEnd < json.size()) {
            if (json[objEnd] == '{') ++depth;
            else if (json[objEnd] == '}') --depth;
            if (depth == 0) break;
            ++objEnd;
        }
        if (objEnd >= json.size()) break;

        std::string obj = json.substr(objStart, objEnd - objStart + 1);
        ScheduledEdit edit;

        auto extract = [&](const std::string& key) -> std::string {
            auto search = '"' + key + R"(": ")";
            auto p = obj.find(search);
            if (p == std::string::npos) {
                search = '"' + key + R"(":)";
                p = obj.find(search);
            }
            if (p == std::string::npos) return {};
            p += search.size();
            if (obj[p] == '"') {
                ++p;
                auto e = obj.find('"', p);
                if (e != std::string::npos) return obj.substr(p, e - p);
            }
            return {};
        };

        auto extractNum = [&](const std::string& key) -> int64_t {
            auto search = '"' + key + R"(": )";
            auto p = obj.find(search);
            if (p == std::string::npos) return 0;
            p += search.size();
            auto e = p;
            while (e < obj.size() && obj[e] >= '0' && obj[e] <= '9') ++e;
            if (e > p) return std::stoll(obj.substr(p, e - p));
            return 0;
        };

        edit.editId       = extract("editId");
        edit.roomId       = extract("roomId");
        edit.targetEventId = extract("targetEventId");
        edit.newContent   = extract("newContent");
        edit.contentUrl   = extract("contentUrl");
        edit.scheduledAtMs = extractNum("scheduledAtMs");
        edit.applied      = obj.find("\"applied\": true") != std::string::npos;
        edit.failed       = obj.find("\"failed\": true") != std::string::npos;
        edit.lastError    = extract("lastError");
        edit.isRecurring  = obj.find("\"isRecurring\": true") != std::string::npos;

        if (!edit.editId.empty() && !edit.targetEventId.empty()) {
            queue_.push_back(edit);
        }

        pos = objEnd + 1;
    }
}

void ScheduledEditQueue::cleanApplied(int olderThanDays) {
    auto cutoff = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count()
        - (static_cast<int64_t>(olderThanDays) * 24 * 60 * 60 * 1000LL);

    queue_.erase(std::remove_if(queue_.begin(), queue_.end(),
        [&](const ScheduledEdit& e) {
            return e.applied && e.scheduledAtMs < cutoff;
        }
    ), queue_.end());
}

void ScheduledEditQueue::clear() {
    queue_.clear();
}

} // namespace progressive
