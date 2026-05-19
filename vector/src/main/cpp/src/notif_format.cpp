#include "progressive/notif_format.hpp"
#include <sstream>
#include <iomanip>
#include <cmath>

namespace progressive {

// ---- Unread Counter (from RoomSummaryFormatter.kt:18-24) ----
// Original Kotlin:
//   fun formatUnreadMessagesCounter(count: Int): String {
//       return if (count > 999) "${count / 1000}.${count % 1000 / 100}k"
//       else count.toString()
//   }

std::string formatUnreadCounter(int count) {
    if (count <= 0) return "0";
    if (count <= 999) return std::to_string(count);

    // 1000+ → "1.0k", "1.2k", "9.9k"
    int thousands = count / 1000;
    int hundreds = (count % 1000) / 100;

    std::ostringstream out;
    out << thousands << "." << hundreds << "k";
    return out.str();
}

// ---- Notification Count with Highlight ----
std::string formatNotificationCount(int count, int highlightCount) {
    if (count <= 0 && highlightCount <= 0) return "";

    std::ostringstream out;
    out << formatUnreadCounter(count);
    if (highlightCount > 0) out << "!";
    return out.str();
}

// ---- Thread Notification Count ----
std::string formatThreadNotificationCount(int threadCount, int threadHighlightCount) {
    if (threadCount <= 0) return "";
    return formatNotificationCount(threadCount, threadHighlightCount);
}

// ---- Combined Count ----
std::string formatCombinedNotificationCount(int roomCount, int threadCount) {
    if (roomCount <= 0 && threadCount <= 0) return "";

    std::ostringstream out;
    out << formatUnreadCounter(roomCount);
    if (threadCount > 0) {
        out << " (" << formatUnreadCounter(threadCount) << ")";
    }
    return out.str();
}

// ---- Total Unread ----
int getTotalUnreadCount(int roomCount, int threadCount) {
    return std::max(0, roomCount) + std::max(0, threadCount);
}

// ---- Badge Text ----
// Original Kotlin pattern (similar to Android NotificationCompat):
//   If > 999, show "999+" or compact form

std::string formatBadgeText(int totalCount) {
    if (totalCount <= 0) return "";
    if (totalCount <= 99) return std::to_string(totalCount);
    if (totalCount <= 999) return std::to_string(totalCount);  // still fits
    return "999+";  // capped for Android notification badge
}

} // namespace progressive
