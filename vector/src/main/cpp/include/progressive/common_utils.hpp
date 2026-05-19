#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>
#include <sstream>
#include <ctime>

namespace progressive {

// ==== Key-Value Store ====
//
// Simple in-memory key-value store with JSON serialization.
// Replaces Android SharedPreferences for C++ layer configuration.
// Can be backed by a file for persistence.

class KeyValueStore {
public:
    // Set a string value.
    void putString(const std::string& key, const std::string& value) { data_[key] = value; }
    // Get a string value.
    std::string getString(const std::string& key, const std::string& def = "") const {
        auto it = data_.find(key); return it != data_.end() ? it->second : def;
    }
    // Set a boolean value.
    void putBool(const std::string& key, bool value) { data_[key] = value ? "true" : "false"; }
    // Get a boolean value.
    bool getBool(const std::string& key, bool def = false) const {
        auto it = data_.find(key); return it != data_.end() ? it->second == "true" : def;
    }
    // Set an integer value.
    void putInt(const std::string& key, int value) { data_[key] = std::to_string(value); }
    // Get an integer value.
    int getInt(const std::string& key, int def = 0) const {
        auto it = data_.find(key); return it != data_.end() ? std::stoi(it->second) : def;
    }
    // Set a long value.
    void putLong(const std::string& key, int64_t value) { data_[key] = std::to_string(value); }
    // Get a long value.
    int64_t getLong(const std::string& key, int64_t def = 0) const {
        auto it = data_.find(key); return it != data_.end() ? std::stoll(it->second) : def;
    }
    // Check if a key exists.
    bool contains(const std::string& key) const { return data_.count(key); }
    // Remove a key.
    void remove(const std::string& key) { data_.erase(key); }
    // Clear all data.
    void clear() { data_.clear(); }

    // Serialize to JSON for persistence.
    std::string toJson() const {
        std::ostringstream os; os << "{"; bool first = true;
        for (const auto& [k, v] : data_) {
            if (!first) os << ","; first = false;
            os << "\"" << k << "\":\"" << v << "\"";
        }
        os << "}"; return os.str();
    }

    // Deserialize from JSON.
    static KeyValueStore fromJson(const std::string& json) {
        KeyValueStore kv;
        size_t pos = 1;
        while (pos < json.size()) {
            while (pos < json.size() && (json[pos]==' '||json[pos]==','||json[pos]=='\n')) pos++;
            if (pos >= json.size() || json[pos] == '}') break;
            if (json[pos] == '"') {
                pos++; size_t ke=pos; while (ke<json.size()&&json[ke]!='"')ke++;
                std::string key = json.substr(pos, ke-pos); pos = ke+1;
                while (pos<json.size()&&json[pos]!=':')pos++; pos++;
                while (pos<json.size()&&(json[pos]==' '||json[pos]=='\t'||json[pos]=='"'))pos++;
                size_t ve=pos; while (ve<json.size()&&json[ve]!='"'){if(json[ve]=='\\')ve++;ve++;}
                kv.data_[key] = json.substr(pos, ve-pos); pos = ve+1;
            }
        }
        return kv;
    }

private:
    std::unordered_map<std::string, std::string> data_;
};

// ==== Logging Utility ====

enum class LogLevel { DEBUG = 0, INFO = 1, WARN = 2, ERROR = 3, NONE = 4 };

class Logger {
public:
    static Logger& instance() { static Logger l; return l; }

    void setLevel(LogLevel level) { level_ = level; }
    void setCallback(void (*cb)(LogLevel, const std::string&)) { callback_ = cb; }

    void debug(const std::string& msg) { log(LogLevel::DEBUG, msg); }
    void info(const std::string& msg) { log(LogLevel::INFO, msg); }
    void warn(const std::string& msg) { log(LogLevel::WARN, msg); }
    void error(const std::string& msg) { log(LogLevel::ERROR, msg); }

private:
    LogLevel level_ = LogLevel::INFO;
    void (*callback_)(LogLevel, const std::string&) = nullptr;

    void log(LogLevel lvl, const std::string& msg) {
        if (lvl < level_) return;
        if (callback_) { callback_(lvl, msg); return; }
        // Default: stderr output
        const char* prefix = "";
        switch (lvl) {
            case LogLevel::DEBUG: prefix = "[D] "; break;
            case LogLevel::INFO: prefix = "[I] "; break;
            case LogLevel::WARN: prefix = "[W] "; break;
            case LogLevel::ERROR: prefix = "[E] "; break;
            default: break;
        }
        fprintf(stderr, "%s%s\n", prefix, msg.c_str());
    }
};

// ==== CSV Builder ====

// Build a CSV row from string values.
inline std::string buildCsvRow(const std::vector<std::string>& values) {
    std::ostringstream os; bool first = true;
    for (const auto& v : values) {
        if (!first) os << ","; first = false;
        if (v.find(',') != std::string::npos || v.find('"') != std::string::npos) {
            os << "\""; for (char c : v) { if (c == '"') os << "\"\""; else os << c; } os << "\"";
        } else { os << v; }
    }
    return os.str();
}

// ==== Timestamp Utilities ====

// Get current time in milliseconds since epoch.
inline int64_t currentTimeMillis() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
}

// Format a timestamp as ISO 8601 date (YYYY-MM-DD).
inline std::string formatIsoDate(int64_t timestampMs) {
    time_t t = timestampMs / 1000;
    struct tm* tm = gmtime(&t);
    char buf[16];
    snprintf(buf, sizeof(buf), "%04d-%02d-%02d", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday);
    return buf;
}

// Format a timestamp as ISO 8601 datetime.
inline std::string formatIsoDateTime(int64_t timestampMs) {
    time_t t = timestampMs / 1000;
    struct tm* tm = gmtime(&t);
    char buf[32];
    snprintf(buf, sizeof(buf), "%04d-%02d-%02dT%02d:%02d:%02dZ",
        tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
        tm->tm_hour, tm->tm_min, tm->tm_sec);
    return buf;
}

// Parse a JSON array of strings: ["a","b","c"] → vector of strings.
inline std::vector<std::string> parseJsonStringArray(const std::string& json) {
    std::vector<std::string> result;
    size_t pos = 0;
    while (pos < json.size() && json[pos] != '[') pos++;
    if (pos >= json.size()) return result;
    pos++;
    while (pos < json.size()) {
        while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == ',' || json[pos] == '\n')) pos++;
        if (pos >= json.size() || json[pos] == ']') break;
        if (json[pos] == '"') {
            pos++;
            size_t end = pos;
            while (end < json.size() && json[end] != '"') {
                if (json[end] == '\\') end++;
                end++;
            }
            result.push_back(json.substr(pos, end - pos));
            pos = end + 1;
        } else {
            pos++;
        }
    }
    return result;
}

} // namespace progressive
