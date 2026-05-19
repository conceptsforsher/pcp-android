#ifndef PROGRESSIVE_AVATAR_HISTORY_HPP
#define PROGRESSIVE_AVATAR_HISTORY_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

struct AvatarEntry {
    std::string mxcUrl;
    std::string eventId;
    int64_t setAtMs = 0;       // when this avatar was set
    int64_t removedAtMs = 0;   // 0 if still active
    bool isCurrent = false;
    std::string setDate;       // formatted: "May 13, 2026"
    std::string removedDate;   // formatted or "" if active
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

} // namespace progressive

#endif // PROGRESSIVE_AVATAR_HISTORY_HPP
