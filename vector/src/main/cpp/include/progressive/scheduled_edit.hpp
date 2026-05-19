#ifndef PROGRESSIVE_SCHEDULED_EDIT_HPP
#define PROGRESSIVE_SCHEDULED_EDIT_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// ---- Scheduled Message Edit ----

struct ScheduledEdit {
    std::string editId;          // unique ID
    std::string roomId;
    std::string targetEventId;   // which message to edit
    std::string newContent;      // new body (if static)
    std::string contentUrl;      // URL to fetch content from (optional)
    std::string formattedContent;// HTML formatted body
    std::string formattedUrl;    // URL for HTML content (optional)
    int64_t scheduledAtMs = 0;   // epoch ms when to apply
    int64_t createdAtMs = 0;
    bool applied = false;
    bool failed = false;
    std::string lastError;
    bool isRecurring = false;     // repeat?
    int64_t intervalMs = 0;       // recurring interval
    std::string cronExpression;   // or cron-like schedule (future)
};

struct EditQueueStats {
    int totalEdits = 0;
    int pendingEdits = 0;
    int appliedEdits = 0;
    int failedEdits = 0;
    int64_t nextEditAtMs = 0;
    std::string nextEditId;
};

class ScheduledEditQueue {
public:
    // Schedule a new edit.
    std::string schedule(const ScheduledEdit& edit);

    // Cancel a scheduled edit.
    void cancel(const std::string& editId);

    // Get all edits due NOW (scheduledAtMs <= current time, not yet applied).
    std::vector<ScheduledEdit> getDueEdits();

    // Get all pending edits (not yet applied, including future).
    std::vector<ScheduledEdit> getPendingEdits() const;

    // Get all edits for a room.
    std::vector<ScheduledEdit> getByRoom(const std::string& roomId) const;

    // Mark an edit as applied.
    void markApplied(const std::string& editId);

    // Mark an edit as failed.
    void markFailed(const std::string& editId, const std::string& error);

    // Reschedule a failed edit (retry in N minutes).
    void retry(const std::string& editId, int delayMinutes);

    // Get queue statistics.
    EditQueueStats getStats() const;

    // Check if content URL is valid.
    static bool isValidContentUrl(const std::string& url);

    // Format an edit as JSON.
    static std::string editToJson(const ScheduledEdit& edit);

    // Export all edits as JSON.
    std::string exportJson() const;

    // Import edits from JSON.
    void importJson(const std::string& json);

    // Clean applied edits older than N days.
    void cleanApplied(int olderThanDays);

    void clear();

private:
    std::vector<ScheduledEdit> queue_;
    int nextId_ = 1;
    std::string generateId();
};

} // namespace progressive

#endif // PROGRESSIVE_SCHEDULED_EDIT_HPP
