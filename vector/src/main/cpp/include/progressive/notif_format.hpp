#ifndef PROGRESSIVE_NOTIF_FORMAT_HPP
#define PROGRESSIVE_NOTIF_FORMAT_HPP

#include <string>

namespace progressive {

// ---- Notification Count Formatter ----
// Ported from original Kotlin:
//   im.vector.app.features.home.room.list.RoomSummaryFormatter.kt (25 lines)
//
// Original Kotlin:
//   fun formatUnreadMessagesCounter(count: Int): String {
//       return if (count > 999) "${count / 1000}.${count % 1000 / 100}k"
//       else count.toString()
//   }

// Format unread count for display in room list badge.
// 1 → "1", 42 → "42", 999 → "999", 1234 → "1.2k", 5432 → "5.4k"
std::string formatUnreadCounter(int count);

// Format notification count with optional highlight indicator.
// "5" — 5 unread messages
// "5!" — 5 unread with 1+ highlights
std::string formatNotificationCount(int count, int highlightCount = 0);

// Format thread notification count.
std::string formatThreadNotificationCount(int threadCount, int threadHighlightCount = 0);

// Format a combined notification badge (room + threads).
// Room: 5 unread, Threads: 3 unread → "5 (3)"
std::string formatCombinedNotificationCount(int roomCount, int threadCount);

// Get total unread count (room + threads).
// Returns a single number for the app icon badge.
int getTotalUnreadCount(int roomCount, int threadCount);

// Get the notification badge text (compact form).
// 0 → ""
// 1-99 → "1" ... "99"
// 100-999 → "100" ... "999" (shortened but still readable)
// 1000+ → "1k"
std::string formatBadgeText(int totalCount);

} // namespace progressive

#endif // PROGRESSIVE_NOTIF_FORMAT_HPP
