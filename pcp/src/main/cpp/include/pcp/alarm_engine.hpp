#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace pcp {

// ---- Alarm Repeat Mode ----
enum class AlarmRepeat {
    ONCE = 0,
    DAILY = 1,
    WEEKLY = 2,
    WEEKDAYS = 3
};

// ---- Alarm Action ----
// Pre-ring action the agent performs
enum class AlarmAction {
    NONE = 0,
    WEATHER = 1,       // Fetch weather forecast
    NEWS = 2,           // Fetch news headlines
    REMINDER = 3        // Custom reminder text
};

// ---- Alarm Definition ----
struct Alarm {
    std::string id;                    // UUID
    std::string note;                  // Alarm note / label
    std::string ringtoneUri;          // Android content:// URI for ringtone
    int64_t triggerAtMs = 0;          // Unix millis when to ring
    int64_t createdAtMs = 0;          // When alarm was created
    AlarmRepeat repeat = AlarmRepeat::ONCE;
    std::vector<int> repeatDays;      // Days of week (0=Mon..6=Sun) for WEEKLY
    AlarmAction preAction = AlarmAction::NONE;
    std::string preActionParam;       // City for weather, URL for news, text for reminder
    int ttsDelaySeconds = 5;          // TTS starts after N seconds of ringing
    bool ttsEnabled = false;
    bool enabled = true;
    bool snoozed = false;
    int64_t snoozeUntilMs = 0;
};

// ---- Alarm Manager ----
class AlarmManager {
public:
    AlarmManager();

    // Create a new alarm from agent-parsed text.
    // Returns the alarm ID.
    std::string createAlarm(const std::string& agentText);

    // Get next active alarm (nearest trigger time, enabled + not snoozed past).
    Alarm getNextAlarm() const;

    // Get all active alarms.
    std::vector<Alarm> getActiveAlarms() const;

    // Snooze an alarm by N minutes.
    void snoozeAlarm(const std::string& id, int minutes);

    // Dismiss an alarm (disable it until re-enabled).
    void dismissAlarm(const std::string& id);

    // Set ringtone for an alarm.
    void setRingtone(const std::string& id, const std::string& uri);

    // Delete an alarm permanently.
    void deleteAlarm(const std::string& id);

    // Parse natural language text into alarm parameters.
    // "иду спать 5 часов" → alarm in 5 hours
    // "будильник на 7:30 с погодой Москва" → alarm at 7:30 with weather
    Alarm parseAgentText(const std::string& text) const;

    // Serialize all alarms to JSON.
    std::string alarmsToJson() const;

    // Load alarms from JSON.
    void loadAlarmsFromJson(const std::string& json);

private:
    std::vector<Alarm> alarms_;
    std::string generateId() const;
    int64_t nowMs() const;
    int parseMinutes(const std::string& s) const;
    int parseHours(const std::string& s) const;
    int parseTime(const std::string& s, int& hour, int& minute) const;
};

} // namespace pcp
