#ifndef PROGRESSIVE_USER_RATING_HPP
#define PROGRESSIVE_USER_RATING_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>

namespace progressive {

struct UserActivity {
    std::string userId;
    std::string displayName;
    int messageCount = 0;
    int reactionGiven = 0;
    int reactionReceived = 0;
    int daysActive = 0;
    int64_t firstSeenMs = 0;
    int64_t lastSeenMs = 0;
    int mediaShared = 0;
    int pollsCreated = 0;
};

struct UserRating {
    std::string userId;
    std::string displayName;
    double score = 0.0;        // 0-100 composite score
    int rank = 0;
    std::string tier;          // "Gold", "Silver", "Bronze", "Newcomer"
    std::string badge;         // "🥇", "🥈", "🥉", "🌱"
};

struct RoomLeaderboard {
    std::string roomId;
    std::vector<UserRating> ratings;  // sorted by score descending
    int totalUsers = 0;
    int64_t generatedAtMs = 0;
};

// Compute user ratings from activity data.
RoomLeaderboard computeLeaderboard(
    const std::string& roomId,
    const std::vector<UserActivity>& activities
);

// Compute a composite score (0-100) from activity metrics.
double computeUserScore(const UserActivity& activity, int maxMessages, int maxReactions);

// Assign a tier based on score.
std::string assignTier(double score);

// Assign a badge emoji based on tier.
std::string assignBadge(const std::string& tier);

// Normalize a value to 0-1 range.
double normalizeValue(int value, int maxValue);

// Format leaderboard as JSON.
std::string leaderboardToJson(const RoomLeaderboard& board);

// Format leaderboard as text.
std::string leaderboardToText(const RoomLeaderboard& board, int topN = 10);

// ---- Activity Streak Calculator ----

struct StreakInfo {
    std::string userId;
    int currentStreak = 0;     // consecutive days active
    int longestStreak = 0;
    int totalActiveDays = 0;
    int64_t lastActiveMs = 0;
    bool isActiveToday = false;
};

// Compute activity streaks from timestamps.
StreakInfo computeStreak(const std::vector<int64_t>& activeTimestampsMs);

// Format streak info as text.
std::string formatStreakText(const StreakInfo& streak);

} // namespace progressive

#endif // PROGRESSIVE_USER_RATING_HPP
