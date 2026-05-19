#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>

namespace progressive {

// Original Kotlin: AvatarChangeReason
enum class AvatarChangeReason {
    USER_CHANGE,
    PROFILE_SYNC,
    ROOM_CREATE,
    UNKNOWN
};

const char* avatarChangeReasonToString(AvatarChangeReason r);
AvatarChangeReason avatarChangeReasonFromString(const std::string& s);

struct AvatarEntry {
    std::string mxcUrl;
    std::string eventId;
    int64_t setAtMs = 0;       // when this avatar was set
    int64_t removedAtMs = 0;   // 0 if still active
    bool isCurrent = false;
    std::string setDate;       // formatted: "May 13, 2026"
    std::string removedDate;   // formatted or "" if active
    std::string url;           // avatar URL (alias for mxcUrl)
    std::string setBy;         // userId who set this avatar
    std::string setAt;         // formatted timestamp string
};

class AvatarHistory {
public:
    // Add an avatar change event.
    void addChange(const std::string& mxcUrl, const std::string& eventId, int64_t timestamp);

    // Get all avatars sorted by date (newest first).
    std::vector<AvatarEntry> getHistory() const;

    // Get current active avatar (latest that wasn't removed).
    const AvatarEntry* getCurrent() const;

    // Remove all entries.
    void clear();

    size_t count() const { return entries_.size(); }

    // Format a UTC epoch to readable date.
    static std::string formatDate(int64_t epochMs);

    // Export as JSON.
    std::string exportJson() const;

    // Track an avatar change with full context and reason.
    // Original Kotlin: trackAvatarChange(url, setBy, timestamp, reason)
    void trackAvatarChange(const std::string& url, const std::string& setBy,
                           int64_t timestamp, AvatarChangeReason reason);

    // Get avatar history as-is (unsorted).
    // Original Kotlin: getAvatarHistory()
    std::vector<AvatarEntry> getAvatarHistory() const;

    // Format avatar history as human-readable text.
    // Original Kotlin: formatAvatarHistory()
    std::string formatAvatarHistory() const;

    // Get the previous avatar URL (before current).
    // Original Kotlin: getPreviousAvatar()
    std::string getPreviousAvatar() const;

    // Check if avatar has changed from initial/default state.
    // Original Kotlin: isAvatarChanged()
    bool isAvatarChanged() const;

    // Get the current avatar URL as a string.
    // Original Kotlin: currentUrl
    std::string currentUrl() const;

private:
    std::vector<AvatarEntry> entries_;
};

// ---- Extended /jumptodate time parsing ----

struct JumpToDateTarget {
    bool valid = false;
    int year = 2024;
    int month = 1;
    int day = 1;
    int hour = 0;
    int minute = 0;
    bool hasTime = false;  // user specified hours:minutes
    int64_t timestampMs = 0;
    std::string error;
};

// Parse a /jumptodate argument that may include time.
// Formats: "YYYY-MM-DD", "YYYY-MM-DD HH:MM", "YYYY-MM-DD HH:MM:SS"
JumpToDateTarget parseJumpToDate(const std::string& input);

// Format jump target as ISO 8601.
std::string formatJumpTarget(const JumpToDateTarget& target);

// ---- /jumptoroom matching ----

struct RoomMatch {
    std::string roomId;
    std::string roomName;
    std::string canonicalAlias;
    double score = 0.0;  // match score (higher = better)
};

// Score and rank rooms by how well they match the query.
// Query can be: room ID (!abc:server), alias (#room:server), or partial name.
std::vector<RoomMatch> matchRooms(
    const std::string& query,
    const std::vector<RoomMatch>& rooms
);

// Check if a string is a valid room ID: !xxx:server

// Compute a fuzzy match score between query and candidate string.
double fuzzyScore(const std::string& query, const std::string& candidate);

// ---- Avatar event builders & parsers ----

// Build m.room.avatar event content JSON: {"url":"mxc://..."}
// Original Kotlin: buildAvatarEvent()
std::string buildAvatarEvent(const std::string& url);

// Parse m.room.avatar event content to extract the avatar URL.
// Original Kotlin: parseAvatarEventContent()
std::string parseAvatarEventContent(const std::string& contentJson);

// Get default avatar URL for a user or room.
// Original Kotlin: getDefaultAvatarUrl(type)
std::string getDefaultAvatarUrl(const std::string& kind);

} // namespace progressive
