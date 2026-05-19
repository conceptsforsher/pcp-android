#ifndef PROGRESSIVE_CHAT_TOOLS_HPP
#define PROGRESSIVE_CHAT_TOOLS_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// ---- User Hide Timer ----

struct UserHideEntry {
    std::string userId;
    std::string displayName;
    int64_t hiddenUntilMs = 0;  // epoch ms when unhide happens
    int remainingSeconds = 0;   // for display
};

class UserHideManager {
public:
    // Hide a user for N minutes.
    void hideFor(const std::string& userId, const std::string& displayName, int minutes);

    // Check if user is currently hidden.
    bool isHidden(const std::string& userId) const;

    // Get remaining seconds for a hidden user (0 = not hidden).
    int getRemainingSeconds(const std::string& userId) const;

    // Get all currently hidden users with remaining time.
    std::vector<UserHideEntry> getActiveHides() const;

    // Clean expired hides.
    void cleanExpired();

    // Export as JSON.
    std::string exportJson() const;

    void clear();

private:
    std::vector<UserHideEntry> entries_;
};

// ---- Message Queue with Ordering ----

struct QueuedMessage {
    std::string msgId;           // unique ID
    std::string roomId;
    std::string body;
    std::string formattedBody;
    int order = 0;               // user-set send order (lower = first)
    int retries = 0;
    int maxRetries = 5;
    int64_t queuedAtMs = 0;
    bool failed = false;
    std::string lastError;
};

class MessageQueue {
public:
    // Enqueue a message.
    void enqueue(const QueuedMessage& msg);

    // Set the send order for a message.
    void setOrder(const std::string& msgId, int order);

    // Mark as failed (will retry).
    void markFailed(const std::string& msgId, const std::string& error);

    // Mark as sent (remove from queue).
    void markSent(const std::string& msgId);

    // Get next message to send (lowest order, not failed).
    const QueuedMessage* getNext() const;

    // Get all messages sorted by order.
    std::vector<QueuedMessage> getAll() const;

    // Get pending count.
    int pendingCount() const;

    void clear();

    std::string exportJson() const;

private:
    std::vector<QueuedMessage> queue_;
};

// ---- Auto-Scroll Math ----

struct AutoScrollConfig {
    bool enabled = false;
    int durationMinutes = 5;
    bool smoothScroll = true;      // true = smooth, false = pages
    int scrollSpeedPxPerSec = 50;  // pixels per second (smooth mode)
    int pageSizeLines = 20;        // lines per jump (page mode)
};

// Compute scroll parameters: total lines, lines per minute, etc.
struct ScrollPlan {
    int totalLines = 0;
    int linesPerMinute = 0;
    int scrollIntervalMs = 1000;  // how often to scroll
    int scrollPxPerTick = 10;     // how much to scroll each tick
    int estimatedFullScrollMin = 0;
};

ScrollPlan computeScrollPlan(const AutoScrollConfig& config, int totalLines, int lineHeightPx);

// ---- Image Crop Helper ----

struct CropInfo {
    std::string sourcePath;
    std::string outputPath;      // cropped PNG in zip
    int cropX = 0, cropY = 0;
    int cropW = 0, cropH = 0;
    bool lossless = true;
};

// Validate crop parameters against image dimensions.
bool isValidCrop(int imgW, int imgH, int cropX, int cropY, int cropW, int cropH);

} // namespace progressive

#endif // PROGRESSIVE_CHAT_TOOLS_HPP
