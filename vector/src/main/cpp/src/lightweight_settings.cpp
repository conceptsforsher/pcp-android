#include "progressive/lightweight_settings.hpp"
#include <unordered_map>

namespace progressive {

// ==== SyncPresence ====
//
// Original Kotlin (SyncPresence.kt):
//   enum class SyncPresence(val value: String) {
//       Online("online"), Unavailable("unavailable"), Offline("offline")
//   }

const char* syncPresenceToString(SyncPresence p) {
    switch (p) {
        case SyncPresence::ONLINE: return "online";
        case SyncPresence::UNAVAILABLE: return "unavailable";
        case SyncPresence::OFFLINE: return "offline";
    }
    return "online";
}

SyncPresence syncPresenceFromString(const std::string& s) {
    // Original Kotlin: SyncPresence.from(presenceString) ?: SyncPresence.Online
    if (s == "online") return SyncPresence::ONLINE;
    if (s == "unavailable") return SyncPresence::UNAVAILABLE;
    if (s == "offline") return SyncPresence::OFFLINE;
    return SyncPresence::ONLINE;
}

// ==== Serialization ====
//
// Original Kotlin (DefaultLightweightSettingsStorage.kt):
//   Stores individual keys in SharedPreferences.
//   C++ version bundles all settings into a single JSON for JNI efficiency.

std::string lightweightSettingsToJson(const LightweightSettings& settings) {
    std::string json = "{";
    // Original Kotlin: MATRIX_SDK_SETTINGS_THREAD_MESSAGES_ENABLED
    json += "\"MATRIX_SDK_SETTINGS_THREAD_MESSAGES_ENABLED\":";
    json += settings.threadMessagesEnabled ? "true" : "false";
    json += ",";
    // Original Kotlin: MATRIX_SDK_SETTINGS_FOREGROUND_PRESENCE_STATUS
    json += "\"MATRIX_SDK_SETTINGS_FOREGROUND_PRESENCE_STATUS\":\"";
    json += syncPresenceToString(settings.foregroundPresence);
    json += "\"";
    json += "}";
    return json;
}

LightweightSettings lightweightSettingsFromJson(const std::string& json) {
    LightweightSettings settings;

    // Original Kotlin: areThreadMessagesEnabled()
    // Default from MatrixConfiguration.threadMessagesEnabledDefault
    settings.threadMessagesEnabled = getSettingBool(json,
        "MATRIX_SDK_SETTINGS_THREAD_MESSAGES_ENABLED", true);

    // Original Kotlin: getSyncPresenceStatus()
    auto presence = getSettingString(json,
        "MATRIX_SDK_SETTINGS_FOREGROUND_PRESENCE_STATUS", "online");
    settings.foregroundPresence = syncPresenceFromString(presence);

    return settings;
}

// ==== Individual Key Access ====
//
// Allows incremental updates without re-serializing the entire JSON.

static std::string extractJsonBool(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return "";
    pos = json.find(':', pos);
    if (pos == std::string::npos) return "";
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    if (pos >= json.size()) return "";
    if (json.compare(pos, 4, "true") == 0) return "true";
    if (json.compare(pos, 5, "false") == 0) return "false";
    // Might be quoted
    if (json[pos] == '"') {
        pos++;
        size_t end = pos;
        while (end < json.size() && json[end] != '"') end++;
        return json.substr(pos, end - pos);
    }
    return "";
}

bool getSettingBool(const std::string& settingsJson, const std::string& key, bool defaultVal) {
    // Original Kotlin: sdkDefaultPrefs.getBoolean(key, default)
    auto val = extractJsonBool(settingsJson, key);
    if (val == "true") return true;
    if (val == "false") return false;
    return defaultVal;
}

std::string setSettingBool(const std::string& settingsJson, const std::string& key, bool val) {
    // Original Kotlin: sdkDefaultPrefs.edit { putBoolean(key, enabled) }
    std::string newJson = settingsJson;
    auto keyPos = newJson.find("\"" + key + "\"");
    if (keyPos == std::string::npos) {
        // Add key to end
        if (newJson.size() > 1 && newJson.back() == '}') {
            newJson.pop_back();
            if (newJson.back() != '{') newJson += ",";
        }
        newJson += "\"" + key + "\":" + std::string(val ? "true" : "false") + "}";
        return newJson;
    }
    // Find and replace the value
    auto colonPos = newJson.find(':', keyPos);
    if (colonPos == std::string::npos) return newJson;
    colonPos++;
    while (colonPos < newJson.size() && (newJson[colonPos] == ' ' || newJson[colonPos] == '\t')) colonPos++;
    size_t end = colonPos;
    while (end < newJson.size() && newJson[end] != ',' && newJson[end] != '}') end++;
    newJson.replace(colonPos, end - colonPos, val ? "true" : "false");
    return newJson;
}

std::string getSettingString(const std::string& settingsJson, const std::string& key, const std::string& defaultVal) {
    auto pos = settingsJson.find("\"" + key + "\"");
    if (pos == std::string::npos) return defaultVal;
    pos = settingsJson.find(':', pos);
    if (pos == std::string::npos) return defaultVal;
    pos++;
    while (pos < settingsJson.size() && (settingsJson[pos] == ' ' || settingsJson[pos] == '\t')) pos++;
    if (pos >= settingsJson.size() || settingsJson[pos] != '"') return defaultVal;
    pos++;
    size_t end = pos;
    while (end < settingsJson.size() && settingsJson[end] != '"') {
        if (settingsJson[end] == '\\') end++;
        end++;
    }
    return settingsJson.substr(pos, end - pos);
}

std::string setSettingString(const std::string& settingsJson, const std::string& key, const std::string& val) {
    std::string newJson = settingsJson;
    auto keyPos = newJson.find("\"" + key + "\"");
    if (keyPos == std::string::npos) {
        if (newJson.size() > 1 && newJson.back() == '}') {
            newJson.pop_back();
            if (newJson.back() != '{') newJson += ",";
        }
        newJson += "\"" + key + "\":\"" + val + "\"}";
        return newJson;
    }
    auto colonPos = newJson.find(':', keyPos);
    if (colonPos == std::string::npos) return newJson;
    colonPos++;
    while (colonPos < newJson.size() && (newJson[colonPos] == ' ' || newJson[colonPos] == '\t')) colonPos++;
    if (colonPos >= newJson.size() || newJson[colonPos] != '"') return newJson;
    colonPos++;
    size_t end = colonPos;
    while (end < newJson.size() && newJson[end] != '"') {
        if (newJson[end] == '\\') end++;
        end++;
    }
    newJson.replace(colonPos, end - colonPos, val);
    return newJson;
}

// Original Kotlin: global settings registry & state
static std::vector<LightweightSetting> g_settings;
static std::unordered_map<std::string, std::string> g_currentValues;

// Original Kotlin: getAllSettings — return all registered settings
std::vector<LightweightSetting> getAllSettings() {
    auto result = g_settings;
    for (auto& s : result) {
        auto it = g_currentValues.find(s.key);
        if (it != g_currentValues.end()) {
            s.currentValue = it->second;
        } else {
            s.currentValue = s.defaultValue;
        }
    }
    return result;
}

// Original Kotlin: getSettingsByCategory — filter settings by category
std::vector<LightweightSetting> getSettingsByCategory(LightweightSettingCategory category) {
    std::vector<LightweightSetting> result;
    for (const auto& s : g_settings) {
        if (s.category == category) {
            auto it = g_currentValues.find(s.key);
            auto copy = s;
            copy.currentValue = (it != g_currentValues.end()) ? it->second : s.defaultValue;
            result.push_back(copy);
        }
    }
    return result;
}

// Original Kotlin: setSetting — update a setting value by key
bool setSetting(const std::string& key, const std::string& value) {
    // Find existing setting to validate
    bool found = false;
    for (const auto& s : g_settings) {
        if (s.key == key) { found = true; break; }
    }
    if (!found) return false;
    g_currentValues[key] = value;
    return true;
}

// Original Kotlin: getSetting — read a setting value by key
std::string getSetting(const std::string& key, const std::string& defaultVal) {
    auto it = g_currentValues.find(key);
    if (it != g_currentValues.end()) return it->second;
    // Check defaults
    for (const auto& s : g_settings) {
        if (s.key == key) return s.defaultValue;
    }
    return defaultVal;
}

// Original Kotlin: resetSetting — restore default value for key
bool resetSetting(const std::string& key) {
    for (const auto& s : g_settings) {
        if (s.key == key) {
            g_currentValues.erase(key);
            return true;
        }
    }
    return false;
}

// Original Kotlin: isSettingModified — check if setting differs from default
bool isSettingModified(const std::string& key) {
    auto it = g_currentValues.find(key);
    if (it == g_currentValues.end()) return false;
    for (const auto& s : g_settings) {
        if (s.key == key) return it->second != s.defaultValue;
    }
    return false;
}

// Original Kotlin: exportSettings — serialize all current values to JSON
std::string exportSettings() {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) {
            if (c == '"') out += "\\\"";
            else out += c;
        }
        return out;
    };

    std::string json = "{";
    bool first = true;
    auto all = getAllSettings();
    for (const auto& s : all) {
        if (!first) json += ",";
        json += "\"" + esc(s.key) + "\":\"" + esc(s.currentValue) + "\"";
        first = false;
    }
    json += "}";
    return json;
}

// Original Kotlin: importSettings — load settings from JSON string
bool importSettings(const std::string& json) {
    // Simple JSON key-value parser for settings import
    size_t pos = 0;
    while (pos < json.size()) {
        // Skip to opening quote
        pos = json.find('"', pos);
        if (pos == std::string::npos) break;
        size_t keyStart = pos + 1;
        size_t keyEnd = json.find('"', keyStart);
        if (keyEnd == std::string::npos) break;
        std::string key = json.substr(keyStart, keyEnd - keyStart);

        // Find colon
        pos = json.find(':', keyEnd);
        if (pos == std::string::npos) break;
        pos++;

        // Find value (quoted string or bare)
        while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
        if (pos >= json.size()) break;

        std::string value;
        if (json[pos] == '"') {
            pos++;
            size_t valStart = pos;
            while (pos < json.size() && json[pos] != '"') {
                if (json[pos] == '\\') pos++;
                pos++;
            }
            value = json.substr(valStart, pos - valStart);
            pos++; // skip closing quote
        } else {
            size_t valStart = pos;
            while (pos < json.size() && json[pos] != ',' && json[pos] != '}' && json[pos] != ' ') pos++;
            value = json.substr(valStart, pos - valStart);
        }

        g_currentValues[key] = value;
        pos++;
    }
    return true;
}

// Original Kotlin: migrateSettings — apply versioned migration steps
bool migrateSettings(const std::vector<SettingsMigration>& migrations, int fromVersion, int toVersion) {
    if (fromVersion >= toVersion) return true;
    for (const auto& m : migrations) {
        if (m.fromVersion >= fromVersion && m.toVersion <= toVersion) {
            if (!m.migrations.empty() && m.migrations != "{}") {
                importSettings(m.migrations);
            }
        }
    }
    return true;
}

// Original Kotlin: settingTypeToString — enum to string conversion
const char* settingTypeToString(LightweightSettingType type) {
    switch (type) {
        case LightweightSettingType::BOOLEAN: return "boolean";
        case LightweightSettingType::INTEGER: return "integer";
        case LightweightSettingType::STRING:  return "string";
        case LightweightSettingType::ENUM:    return "enum";
        case LightweightSettingType::FLOAT:   return "float";
        case LightweightSettingType::JSON:    return "json";
    }
    return "string";
}

// Original Kotlin: settingTypeFromString — parse type from string
LightweightSettingType settingTypeFromString(const std::string& s) {
    if (s == "boolean") return LightweightSettingType::BOOLEAN;
    if (s == "integer") return LightweightSettingType::INTEGER;
    if (s == "string")  return LightweightSettingType::STRING;
    if (s == "enum")    return LightweightSettingType::ENUM;
    if (s == "float")   return LightweightSettingType::FLOAT;
    if (s == "json")    return LightweightSettingType::JSON;
    return LightweightSettingType::STRING;
}

// Original Kotlin: settingCategoryToString — enum to string conversion
const char* settingCategoryToString(LightweightSettingCategory category) {
    switch (category) {
        case LightweightSettingCategory::GENERAL:       return "general";
        case LightweightSettingCategory::APPEARANCE:    return "appearance";
        case LightweightSettingCategory::NOTIFICATIONS: return "notifications";
        case LightweightSettingCategory::SECURITY:      return "security";
        case LightweightSettingCategory::LABS:          return "labs";
        case LightweightSettingCategory::ADVANCED:      return "advanced";
        case LightweightSettingCategory::DEBUG:         return "debug";
    }
    return "general";
}

// Original Kotlin: settingCategoryFromString — parse category from string
LightweightSettingCategory settingCategoryFromString(const std::string& s) {
    if (s == "general")       return LightweightSettingCategory::GENERAL;
    if (s == "appearance")    return LightweightSettingCategory::APPEARANCE;
    if (s == "notifications") return LightweightSettingCategory::NOTIFICATIONS;
    if (s == "security")      return LightweightSettingCategory::SECURITY;
    if (s == "labs")          return LightweightSettingCategory::LABS;
    if (s == "advanced")      return LightweightSettingCategory::ADVANCED;
    if (s == "debug")         return LightweightSettingCategory::DEBUG;
    return LightweightSettingCategory::GENERAL;
}

// Original Kotlin: validateSettingValue — check value is valid for type
bool validateSettingValue(const std::string& value, LightweightSettingType type) {
    switch (type) {
        case LightweightSettingType::BOOLEAN:
            return value == "true" || value == "false" || value == "1" || value == "0";
        case LightweightSettingType::INTEGER:
            try { std::stoll(value); return true; } catch (...) { return false; }
        case LightweightSettingType::FLOAT:
            try { std::stod(value); return true; } catch (...) { return false; }
        case LightweightSettingType::STRING:
        case LightweightSettingType::ENUM:
        case LightweightSettingType::JSON:
            return true; // always valid for these types
    }
    return false;
}

// Original Kotlin: registerSetting — add a new setting to the global registry
bool registerSetting(const LightweightSetting& setting) {
    // Check for duplicate keys
    for (const auto& s : g_settings) {
        if (s.key == setting.key) return false;
    }
    g_settings.push_back(setting);
    // Initialize current value to default
    if (g_currentValues.find(setting.key) == g_currentValues.end()) {
        g_currentValues[setting.key] = setting.defaultValue;
    }
    return true;
}

// Original Kotlin: registerDefaultSettings — populate with standard Element settings
void registerDefaultSettings() {
    registerSetting({"threadMessagesEnabled", LightweightSettingType::BOOLEAN,
        "true", "true", "Enable threaded messages in rooms",
        LightweightSettingCategory::GENERAL, false, false});
    registerSetting({"foregroundPresenceStatus", LightweightSettingType::ENUM,
        "online", "online", "Presence status when app is in foreground",
        LightweightSettingCategory::GENERAL, false, false});
    registerSetting({"showReadReceipts", LightweightSettingType::BOOLEAN,
        "true", "true", "Show read receipts in rooms",
        LightweightSettingCategory::APPEARANCE, false, false});
    registerSetting({"showTypingIndicators", LightweightSettingType::BOOLEAN,
        "true", "true", "Show typing indicators in rooms",
        LightweightSettingCategory::APPEARANCE, false, false});
    registerSetting({"enablePushNotifications", LightweightSettingType::BOOLEAN,
        "true", "true", "Enable push notifications globally",
        LightweightSettingCategory::NOTIFICATIONS, false, false});
    registerSetting({"notificationSound", LightweightSettingType::STRING,
        "default", "default", "Notification sound identifier",
        LightweightSettingCategory::NOTIFICATIONS, false, false});
    registerSetting({"enableRinging", LightweightSettingType::BOOLEAN,
        "true", "true", "Enable ringing for incoming calls",
        LightweightSettingCategory::NOTIFICATIONS, false, false});
    registerSetting({"encryptToUnverifiedSessions", LightweightSettingType::BOOLEAN,
        "false", "false", "Encrypt messages to unverified sessions",
        LightweightSettingCategory::SECURITY, false, false});
    registerSetting({"crossSigningEnabled", LightweightSettingType::BOOLEAN,
        "true", "true", "Enable cross-signing for device verification",
        LightweightSettingCategory::SECURITY, false, false});
    registerSetting({"useNativeHttpClient", LightweightSettingType::BOOLEAN,
        "false", "false", "Use native C++ HTTP client instead of Retrofit",
        LightweightSettingCategory::LABS, true, true});
    registerSetting({"useJumboEmoji", LightweightSettingType::BOOLEAN,
        "true", "true", "Render single emoji messages in large size",
        LightweightSettingCategory::APPEARANCE, false, false});
    registerSetting({"debugLoggingEnabled", LightweightSettingType::BOOLEAN,
        "false", "false", "Enable debug-level logging",
        LightweightSettingCategory::DEBUG, false, false});
}

// Original Kotlin: getAllModifiedSettings — list settings that differ from defaults
std::vector<LightweightSetting> getAllModifiedSettings() {
    std::vector<LightweightSetting> result;
    for (const auto& s : g_settings) {
        if (isSettingModified(s.key)) {
            auto copy = s;
            copy.currentValue = getSetting(s.key, s.defaultValue);
            result.push_back(copy);
        }
    }
    return result;
}

// Original Kotlin: resetAllSettings — restore all settings to defaults
void resetAllSettings() {
    g_currentValues.clear();
}

// Original Kotlin: castSettingValue — convert value to target type
std::string castSettingValue(const std::string& value, LightweightSettingType type) {
    switch (type) {
        case LightweightSettingType::BOOLEAN:
            if (value == "1" || value == "true") return "true";
            return "false";
        case LightweightSettingType::INTEGER:
            try { return std::to_string(std::stoll(value)); } catch (...) { return "0"; }
        case LightweightSettingType::FLOAT:
            try { return std::to_string(static_cast<int64_t>(std::stod(value) * 1000)); } catch (...) { return "0.0"; }
        case LightweightSettingType::STRING:
        case LightweightSettingType::ENUM:
        case LightweightSettingType::JSON:
        default:
            return value;
    }
}

} // namespace progressive
