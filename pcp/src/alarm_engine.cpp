#include "pcp/alarm_engine.hpp"
#include <sstream>
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <ctime>

namespace pcp {

static std::string escapeJsonStr(const std::string& s) {
    std::string out;
    for (char c : s) {
        switch (c) {
            case '"': out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\n': out += "\\n"; break;
            default: out += c;
        }
    }
    return out;
}

AlarmManager::AlarmManager() {}

std::string AlarmManager::generateId() const {
    int64_t t = nowMs();
    std::string id = "alarm_" + std::to_string(t);
    // Add random suffix
    id += "_" + std::to_string(rand());
    return id;
}

int64_t AlarmManager::nowMs() const {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
}

int AlarmManager::parseMinutes(const std::string& s) const {
    try { return std::stoi(s); }
    catch (...) { return 0; }
}

int AlarmManager::parseHours(const std::string& s) const {
    try { return std::stoi(s); }
    catch (...) { return 0; }
}

int AlarmManager::parseTime(const std::string& s, int& hour, int& minute) const {
    auto colon = s.find(':');
    if (colon == std::string::npos) return -1;
    try {
        hour = std::stoi(s.substr(0, colon));
        minute = std::stoi(s.substr(colon + 1));
        return 0;
    } catch (...) { return -1; }
}

Alarm AlarmManager::parseAgentText(const std::string& text) const {
    Alarm alarm;
    alarm.id = generateId();
    alarm.createdAtMs = nowMs();
    alarm.note = text;

    std::string lower;
    std::transform(text.begin(), text.end(), std::back_inserter(lower), ::tolower);

    // Detect time patterns
    std::vector<std::string> words;
    std::istringstream iss(text);
    std::string w;
    while (iss >> w) words.push_back(w);

    // Pattern: "N часов" or "N часа" or "N hour(s)"
    for (size_t i = 0; i + 1 < words.size(); i++) {
        if (words[i + 1] == "часов" || words[i + 1] == "часа" || words[i + 1] == "час" ||
            words[i] == "hours" || words[i] == "hour") {
            int n = parseMinutes(words[i]);
            if (n > 0 || (i > 0 && parseMinutes(words[i - 1]) > 0)) {
                if (n == 0) n = parseMinutes(words[i - 1]);
                alarm.triggerAtMs = nowMs() + n * 3600LL * 1000;
                return alarm;
            }
        }
        if (i + 3 < words.size() && words[i + 1] == "часов" && words[i + 2] == "и") {
            // "N часов и M минут"
            int hrs = parseHours(words[i]);
            int mins = parseMinutes(words[i + 3]);
            if (hrs > 0 || mins > 0) {
                alarm.triggerAtMs = nowMs() + hrs * 3600LL * 1000 + mins * 60000LL;
                return alarm;
            }
        }
    }

    // Pattern: "N минут" or "N minutes"
    for (size_t i = 0; i + 1 < words.size(); i++) {
        if (words[i + 1] == "минут" || words[i + 1] == "минуты" || words[i + 1] == "минуту" ||
            words[i + 1] == "min" || words[i + 1] == "mins" || words[i + 1] == "minutes" ||
            words[i + 1] == "minute") {
            int n = parseMinutes(words[i]);
            if (n > 0) {
                alarm.triggerAtMs = nowMs() + n * 60000LL;
                return alarm;
            }
        }
    }

    // Pattern: "в HH:MM" or "at HH:MM"
    for (size_t i = 0; i + 1 < words.size(); i++) {
        int h = 0, m = 0;
        if (parseTime(words[i + 1], h, m) == 0 &&
            (words[i] == "в" || words[i] == "на" || words[i] == "at")) {
            // Calculate next occurrence at that time
            time_t now = time(nullptr);
            struct tm tm_now;
            localtime_r(&now, &tm_now);
            struct tm tm_alarm = tm_now;
            tm_alarm.tm_hour = h;
            tm_alarm.tm_min = m;
            tm_alarm.tm_sec = 0;

            time_t alarm_time = mktime(&tm_alarm);
            if (alarm_time <= now) alarm_time += 86400; // next day
            alarm.triggerAtMs = alarm_time * 1000LL;
            return alarm;
        }
    }

    // Pattern: "будильник на HH:MM"
    for (size_t i = 0; i < words.size(); i++) {
        int h = 0, m = 0;
        if (parseTime(words[i], h, m) == 0) {
            if (i > 0 && (words[i - 1] == "на" || words[i - 1] == "в")) {
                time_t now = time(nullptr);
                struct tm tm_now;
                localtime_r(&now, &tm_now);
                struct tm tm_alarm = tm_now;
                tm_alarm.tm_hour = h;
                tm_alarm.tm_min = m;
                tm_alarm.tm_sec = 0;
                time_t alarm_time = mktime(&tm_alarm);
                if (alarm_time <= now) alarm_time += 86400;
                alarm.triggerAtMs = alarm_time * 1000LL;
                return alarm;
            }
        }
    }

    // Detect pre-action: "погода <city>" or "weather <city>"
    for (size_t i = 0; i + 1 < words.size(); i++) {
        if (words[i] == "погода" || words[i] == "weather") {
            alarm.preAction = AlarmAction::WEATHER;
            if (i + 1 < words.size()) alarm.preActionParam = words[i + 1];
        }
        if (words[i] == "новости" || words[i] == "news") {
            alarm.preAction = AlarmAction::NEWS;
        }
        if (words[i] == "напомни" && i + 1 < words.size()) {
            alarm.preAction = AlarmAction::REMINDER;
            alarm.preActionParam = words[i + 1];
        }
    }

    // Default: alarm in 0 seconds (now) if no time pattern detected
    if (alarm.triggerAtMs == 0) {
        alarm.triggerAtMs = nowMs();
    }

    return alarm;
}

std::string AlarmManager::createAlarm(const std::string& agentText) {
    Alarm alarm = parseAgentText(agentText);
    alarm.enabled = true;
    alarms_.push_back(alarm);
    return alarm.id;
}

Alarm AlarmManager::getNextAlarm() const {
    Alarm next;
    int64_t soonest = INT64_MAX;
    for (const auto& a : alarms_) {
        int64_t trigger = a.snoozed ? a.snoozeUntilMs : a.triggerAtMs;
        if (a.enabled && trigger > nowMs() && trigger < soonest) {
            soonest = trigger;
            next = a;
        }
    }
    return next;
}

std::vector<Alarm> AlarmManager::getActiveAlarms() const {
    std::vector<Alarm> result;
    for (const auto& a : alarms_) {
        if (a.enabled) result.push_back(a);
    }
    return result;
}

void AlarmManager::snoozeAlarm(const std::string& id, int minutes) {
    for (auto& a : alarms_) {
        if (a.id == id) {
            a.snoozed = true;
            a.snoozeUntilMs = nowMs() + minutes * 60000LL;
            break;
        }
    }
}

void AlarmManager::setRingtone(const std::string& id, const std::string& uri) {
    for (auto& a : alarms_) {
        if (a.id == id) { a.ringtoneUri = uri; break; }
    }
}

void AlarmManager::dismissAlarm(const std::string& id) {
    for (auto& a : alarms_) {
        if (a.id == id) {
            a.enabled = false;
            a.snoozed = false;
            break;
        }
    }
}

void AlarmManager::deleteAlarm(const std::string& id) {
    alarms_.erase(std::remove_if(alarms_.begin(), alarms_.end(),
        [&](const Alarm& a) { return a.id == id; }), alarms_.end());
}

std::string AlarmManager::alarmsToJson() const {
    std::ostringstream os;
    os << "[";
    for (size_t i = 0; i < alarms_.size(); i++) {
        if (i > 0) os << ",";
        const auto& a = alarms_[i];
        os << "{";
        os << "\"id\":\"" << a.id << "\"";
        os << ",\"note\":\"" << escapeJsonStr(a.note) << "\"";
        os << ",\"triggerAtMs\":" << a.triggerAtMs;
        os << ",\"createdAtMs\":" << a.createdAtMs;
        os << ",\"repeat\":" << static_cast<int>(a.repeat);
        os << ",\"preAction\":" << static_cast<int>(a.preAction);
        os << ",\"preActionParam\":\"" << escapeJsonStr(a.preActionParam) << "\"";
        os << ",\"ttsEnabled\":" << (a.ttsEnabled ? "true" : "false");
        os << ",\"ttsDelaySeconds\":" << a.ttsDelaySeconds;
        os << ",\"enabled\":" << (a.enabled ? "true" : "false");
        os << ",\"snoozed\":" << (a.snoozed ? "true" : "false");
        os << ",\"snoozeUntilMs\":" << a.snoozeUntilMs;
        os << ",\"ringtoneUri\":\"" << escapeJsonStr(a.ringtoneUri) << "\"";
        os << "}";
    }
    os << "]";
    return os.str();
}

void AlarmManager::loadAlarmsFromJson(const std::string& json) {
    alarms_.clear();
    if (json.empty() || json == "[]") return;

    size_t pos = 0;
    while (true) {
        pos = json.find("\"id\":\"", pos);
        if (pos == std::string::npos) break;

        Alarm a;
        // id
        auto start = pos + 6;
        auto end = json.find('"', start);
        if (end == std::string::npos) break;
        a.id = json.substr(start, end - start);

        // Parse other fields
        auto extractStr = [&](const std::string& key) -> std::string {
            auto p = json.find("\"" + key + "\":\"", pos);
            if (p == std::string::npos) return "";
            auto s = p + key.size() + 4;
            auto e = json.find('"', s);
            if (e == std::string::npos) return "";
            return json.substr(s, e - s);
        };
        auto extractInt = [&](const std::string& key) -> int64_t {
            auto p = json.find("\"" + key + "\":", pos);
            if (p == std::string::npos) return 0;
            auto s = p + key.size() + 2;
            while (s < json.size() && json[s] == ' ') s++;
            try { return std::stoll(json.substr(s)); } catch (...) { return 0; }
        };
        auto extractBool = [&](const std::string& key) -> bool {
            auto p = json.find("\"" + key + "\":", pos);
            if (p == std::string::npos) return false;
            auto s = p + key.size() + 2;
            while (s < json.size() && json[s] == ' ') s++;
            if (json.substr(s, 4) == "true") return true;
            return false;
        };

        a.note = extractStr("note");
        a.triggerAtMs = extractInt("triggerAtMs");
        a.createdAtMs = extractInt("createdAtMs");
        a.repeat = static_cast<AlarmRepeat>(extractInt("repeat"));
        a.preAction = static_cast<AlarmAction>(extractInt("preAction"));
        a.preActionParam = extractStr("preActionParam");
        a.ttsEnabled = extractBool("ttsEnabled");
        a.ttsDelaySeconds = static_cast<int>(extractInt("ttsDelaySeconds"));
        a.enabled = extractBool("enabled");
        a.ringtoneUri = extractStr("ringtoneUri");

        alarms_.push_back(a);
        pos = json.find('}', end) + 1;
    }
}

} // namespace pcp
