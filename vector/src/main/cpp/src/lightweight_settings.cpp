#include "progressive/lightweight_settings.hpp"

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

} // namespace progressive
