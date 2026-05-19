#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// ============================================================================
// NotifSound — notification sound policy
// ============================================================================
//
// Original Kotlin (NotificationUtils.kt):
//   enum class NotifSound { DEFAULT, SILENT, RING_ONCE, RING_REPEAT, CUSTOM }
//
// Android sound URIs:
//   DEFAULT: Settings.System.DEFAULT_NOTIFICATION_URI
//   SILENT: null / empty

enum class NotifSound {
    DEFAULT,
    SILENT,
    RING_ONCE,
    RING_REPEAT,
    CUSTOM
};

// Get the Android content:// URI for each sound level.
// Returns empty string for SILENT.
std::string notifSoundToUri(NotifSound sound);
NotifSound notifSoundFromString(const std::string& str);

// ============================================================================
// NotifPriority — Android notification priority levels
// ============================================================================
//
// Original Kotlin (NotificationUtils.kt / NotificationCompat.PRIORITY_*):
//   PRIORITY_DEFAULT = 0
//   PRIORITY_HIGH    = 1
//   PRIORITY_LOW     = -1
//   PRIORITY_MIN     = -2
//   PRIORITY_MAX     = 2

enum class NotifPriority {
    DEFAULT = 0,
    HIGH    = 1,
    LOW     = -1,
    MIN     = -2,
    MAX     = 2
};

// Convert between enum and Android integer values.
int notifPriorityToAndroidValue(NotifPriority priority);
NotifPriority notifPriorityFromAndroidValue(int value);
std::string notifPriorityToString(NotifPriority priority);
NotifPriority notifPriorityFromString(const std::string& str);

// ============================================================================
// NotifImportance — Android notification channel importance
// ============================================================================
//
// Original Kotlin (NotificationManager.IMPORTANCE_*):
//   IMPORTANCE_NONE    = 0
//   IMPORTANCE_MIN     = 1
//   IMPORTANCE_LOW     = 2
//   IMPORTANCE_DEFAULT = 3
//   IMPORTANCE_HIGH    = 4
//   IMPORTANCE_MAX     = 5
//
// These map to Android's NotificationManager.IMPORTANCE_* constants.

enum class NotifImportance {
    NONE    = 0,
    MIN     = 1,
    LOW     = 2,
    DEFAULT = 3,
    HIGH    = 4,
    MAX     = 5
};

// Get channel name string for each importance level (human-readable).
// Used for Android NotificationChannel creation.
std::string notifImportanceToChannelName(NotifImportance importance);
int notifImportanceToAndroidValue(NotifImportance importance);
NotifImportance notifImportanceFromAndroidValue(int value);
std::string notifImportanceToString(NotifImportance importance);
NotifImportance notifImportanceFromString(const std::string& str);

// ============================================================================
// NotifVisibility — lockscreen visibility
// ============================================================================
//
// Original Kotlin (NotificationCompat.VISIBILITY_*):
//   VISIBILITY_PUBLIC  = 1
//   VISIBILITY_PRIVATE = 0
//   VISIBILITY_SECRET  = -1

enum class NotifVisibility {
    PUBLIC  = 1,
    PRIVATE = 0,
    SECRET  = -1
};

int notifVisibilityToAndroidValue(NotifVisibility visibility);
NotifVisibility notifVisibilityFromAndroidValue(int value);
std::string notifVisibilityToString(NotifVisibility visibility);
NotifVisibility notifVisibilityFromString(const std::string& str);

// ============================================================================
// NotificationChannel — Android 8+ channel descriptor
// ============================================================================
//
// Original Kotlin (NotificationChannel.kt / ChannelManager.kt):
//   data class NotificationChannel(
//       val channelId: String,
//       val name: String,
//       val importance: Int,
//       val sound: Uri?,
//       val enableVibration: Boolean,
//       val description: String,
//       val lightColor: Int
//   )

struct NotificationChannel {
    std::string channelId;                       // Unique channel identifier
    std::string name;                            // User-visible channel name
    NotifImportance importance = NotifImportance::DEFAULT;
    NotifSound sound = NotifSound::DEFAULT;
    std::string customSoundUri;                  // Custom sound URI (e.g. content://)
    bool enableVibration = true;
    std::string description;                     // Channel description shown in settings
    int lightColor = 0;                          // ARGB color for notification LED
    NotifVisibility lockscreenVisibility = NotifVisibility::PRIVATE;
    bool showBadge = true;                       // Show badge for this channel
    bool bypassDnd = false;                      // Allow this channel to bypass Do Not Disturb
    int64_t vibrationPatternMs = 0;              // Custom vibration pattern (0 = default)

    // Get the effective sound URI string.
    std::string effectiveSoundUri() const;
};

// Build NotificationChannel JSON for JNI bridge.
std::string buildNotificationChannel(const NotificationChannel& channel);
std::string notificationChannelToJson(const NotificationChannel& channel);

// Parse NotificationChannel from JSON.
NotificationChannel parseNotificationChannel(const std::string& json);

// ============================================================================
// Predefined notification channels (matching Element Android defaults)
// ============================================================================
//
// Original Kotlin (NotificationUtils.kt):
//   NoisyNotificationChannel, SilentNotificationChannel, etc.

// Default noisy notification channel for direct messages.
NotificationChannel createNoisyNotificationChannel();

// Silent notification channel (no sound, no vibration).
NotificationChannel createSilentNotificationChannel();

// Call notification channel (high priority, ringtone).
NotificationChannel createCallNotificationChannel();

// Background sync channel (low importance, no sound).
NotificationChannel createBackgroundSyncChannel();

// ============================================================================
// Notification Count Formatting (legacy)
// ============================================================================
//
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

// ============================================================================
// Notification channel summary helpers
// ============================================================================

// Build a list of all notification channels as JSON array.
// Used for JNI to register all channels with Android on startup.
std::string buildAllNotificationChannelsJson();

// Build a single channel registration string for JNI.
// Returns creator parameters: channelId, name, importance, sound, vibration, desc, color
std::string buildChannelRegistrationInfo(const NotificationChannel& channel);

} // namespace progressive
