#pragma once

#include <string>
#include <cstdint>

namespace progressive {

// Lightweight Settings Storage — fast, non-database storage for SDK
// preferences. Uses Android SharedPreferences under the hood; this C++
// module provides the data model and serialization.
//
// Original Kotlin (LightweightSettingsStorage.kt:19-22):
//   interface LightweightSettingsStorage {
//       fun setThreadMessagesEnabled(enabled: Boolean)
//       fun areThreadMessagesEnabled(): Boolean
//   }
//
// Original Kotlin (DefaultLightweightSettingsStorage.kt:35-39):
//   internal fun setSyncPresenceStatus(presence: SyncPresence)
//   internal fun getSyncPresenceStatus(): SyncPresence

// Presence status sent during sync when app is in foreground.
//
// Original Kotlin (SyncPresence.kt):
//   enum class SyncPresence(val value: String) {
//       Online("online"), Unavailable("unavailable"), Offline("offline")
//   }
enum class SyncPresence {
    ONLINE = 0,
    UNAVAILABLE = 1,
    OFFLINE = 2
};

const char* syncPresenceToString(SyncPresence p);
SyncPresence syncPresenceFromString(const std::string& s);

// All SDK lightweight settings in one struct for serialization.
struct LightweightSettings {
    bool threadMessagesEnabled = true;   // Default from MatrixConfiguration
    SyncPresence foregroundPresence = SyncPresence::ONLINE;

    // Original Kotlin key constants:
    //   MATRIX_SDK_SETTINGS_THREAD_MESSAGES_ENABLED
    //   MATRIX_SDK_SETTINGS_FOREGROUND_PRESENCE_STATUS
};

// Serialize all settings to a JSON string for storage.
std::string lightweightSettingsToJson(const LightweightSettings& settings);

// Parse settings from a JSON string.
LightweightSettings lightweightSettingsFromJson(const std::string& json);

// Get/set individual settings by key (matches original Kotlin constants).
bool getSettingBool(const std::string& settingsJson, const std::string& key, bool defaultVal);
std::string setSettingBool(const std::string& settingsJson, const std::string& key, bool val);
std::string getSettingString(const std::string& settingsJson, const std::string& key, const std::string& defaultVal);
std::string setSettingString(const std::string& settingsJson, const std::string& key, const std::string& val);

} // namespace progressive
