#ifndef PROGRESSIVE_NOTIF_ANALYZER_HPP
#define PROGRESSIVE_NOTIF_ANALYZER_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

struct NotifEvent {
    std::string eventId;
    std::string roomId;
    std::string roomName;
    std::string senderName;
    std::string body;
    std::string type;         // "message", "mention", "invite", "call", "reaction"
    int64_t timestampMs = 0;
    bool isNoisy = false;     // made sound
    bool isHighlight = false; // highlighted notification
};

struct NotifAnalytics {
    int totalNotifications = 0;
    int messages = 0;
    int mentions = 0;
    int invites = 0;
    int calls = 0;
    int reactions = 0;
    int noisyCount = 0;
    int highlightCount = 0;

    // Time-based
    std::vector<int> byHour;    // notifications per hour (24 slots)
    std::vector<int> byDay;     // notifications per day of week (7 slots)
    int peakHour = 0;
    int peakDay = 0;

    // Top sources
    std::vector<std::pair<std::string, int>> topRooms;   // room → count
    std::vector<std::pair<std::string, int>> topSenders; // sender → count

    // Rates
    double avgPerHour = 0.0;
    double avgPerDay = 0.0;
    int64_t firstSeenMs = 0;
    int64_t lastSeenMs = 0;
};

// Analyze notification history for patterns.
NotifAnalytics analyzeNotifications(const std::vector<NotifEvent>& events);

// Classify a notification event type from content and room state.
std::string classifyNotifEvent(const std::string& body, bool isMention,
    bool isInvite, bool isCall, bool isReaction);

// Format notification analytics as JSON.
std::string notifAnalyticsToJson(const NotifAnalytics& analytics);

// Format notification analytics as human-readable text.
std::string notifAnalyticsToText(const NotifAnalytics& analytics);

// Get notification quiet hours suggestion based on history.
std::pair<int, int> suggestQuietHours(const NotifAnalytics& analytics);

// Get the busiest notification day name.
std::string getBusiestDayName(const NotifAnalytics& analytics);

} // namespace progressive

#endif // PROGRESSIVE_NOTIF_ANALYZER_HPP
