#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace pcp {

// ---- Notification Mode ----

enum class NightMode {
    NORMAL = 0,
    NIGHT = 1       // Only alarms, keyword pings, no sound for regular messages
};

struct NightNotifConfig {
    NightMode mode = NightMode::NORMAL;
    std::vector<std::string> nightKeywords;  // words that still ping in night mode
    std::vector<std::string> nightMxids;     // users that still ping in night mode
    bool nightPingRooms = false;             // @room still pings in night mode
};

// ---- Notification Mode Manager ----

class NightModeManager {
public:
    NightModeManager();

    void setMode(NightMode m);
    NightMode getMode() const;

    void setNightKeywords(const std::vector<std::string>& keywords);
    void addNightKeyword(const std::string& kw);
    void removeNightKeyword(const std::string& kw);

    // Check if a notification should be suppressed in night mode.
    // Returns true if the notification SHOULD ring (not suppressed).
    bool shouldNotify(const std::string& body, const std::string& senderMxid,
                      bool isRoomPing, bool isAlarm) const;

    // Serialize to/from JSON for persistence.
    std::string toJson() const;
    void fromJson(const std::string& json);

    const NightNotifConfig& config() const { return cfg_; }

private:
    NightNotifConfig cfg_;
    bool matchKeyword(const std::string& body) const;
};

} // namespace pcp
