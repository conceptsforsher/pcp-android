#pragma once

#include <string>
#include <vector>
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

// Original Kotlin: LightweightSettingType — value type of a setting
enum class LightweightSettingType {
    BOOLEAN,
    INTEGER,
    STRING,
    ENUM,
    FLOAT,
    JSON
};

// Original Kotlin: LightweightSettingCategory — grouping for settings UI
enum class LightweightSettingCategory {
    GENERAL,
    APPEARANCE,
    NOTIFICATIONS,
    SECURITY,
    LABS,
    ADVANCED,
    DEBUG
};

// Original Kotlin: LightweightSetting — single setting descriptor
struct LightweightSetting {
    std::string key;
    LightweightSettingType type = LightweightSettingType::BOOLEAN;
    std::string defaultValue;
    std::string currentValue;
    std::string description;
    LightweightSettingCategory category = LightweightSettingCategory::GENERAL;
    bool isExperimental = false;
    bool requiresRestart = false;
};

// Original Kotlin: SettingsMigration — versioned migration descriptor
struct SettingsMigration {
    int fromVersion = 0;
    int toVersion = 0;
    std::string migrations; // JSON-encoded migration operations
};

// Original Kotlin: getAllSettings — get all registered lightweight settings
std::vector<LightweightSetting> getAllSettings();

// Original Kotlin: getSettingsByCategory — filter settings by category
std::vector<LightweightSetting> getSettingsByCategory(LightweightSettingCategory category);

// Original Kotlin: setSetting — update a setting value by key
bool setSetting(const std::string& key, const std::string& value);

// Original Kotlin: getSetting — read a setting value by key
std::string getSetting(const std::string& key, const std::string& defaultVal = "");

// Original Kotlin: resetSetting — restore default value for key
bool resetSetting(const std::string& key);

// Original Kotlin: isSettingModified — check if setting differs from default
bool isSettingModified(const std::string& key);

// Original Kotlin: exportSettings — serialize all settings to JSON
std::string exportSettings();

// Original Kotlin: importSettings — load settings from JSON
bool importSettings(const std::string& json);

// Original Kotlin: migrateSettings — apply versioned migrations
bool migrateSettings(const std::vector<SettingsMigration>& migrations, int fromVersion, int toVersion);

// Original Kotlin: settingTypeToString — enum to string conversion
const char* settingTypeToString(LightweightSettingType type);

// Original Kotlin: settingTypeFromString — parse type from string
LightweightSettingType settingTypeFromString(const std::string& s);

// Original Kotlin: settingCategoryToString — enum to string conversion
const char* settingCategoryToString(LightweightSettingCategory category);

// Original Kotlin: settingCategoryFromString — parse category from string
LightweightSettingCategory settingCategoryFromString(const std::string& s);

// Original Kotlin: validateSettingValue — check value is valid for type
bool validateSettingValue(const std::string& value, LightweightSettingType type);

// Original Kotlin: registerSetting — add a new setting to the global registry
bool registerSetting(const LightweightSetting& setting);

// Original Kotlin: registerDefaultSettings — populate with standard Element settings
void registerDefaultSettings();

// Original Kotlin: getAllModifiedSettings — list settings that differ from defaults
std::vector<LightweightSetting> getAllModifiedSettings();

// Original Kotlin: resetAllSettings — restore all settings to defaults
void resetAllSettings();

// Original Kotlin: castSettingValue — convert value to target type
std::string castSettingValue(const std::string& value, LightweightSettingType type);

} // namespace progressive
