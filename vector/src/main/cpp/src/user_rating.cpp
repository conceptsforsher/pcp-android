#include "progressive/user_rating.hpp"
#include <sstream>
#include <algorithm>
#include <chrono>
#include <set>

namespace progressive {

double normalizeValue(int value, int maxValue) {
    if (maxValue <= 0) return 0.0;
    return std::min(1.0, static_cast<double>(value) / maxValue);
}

double computeUserScore(const UserActivity& activity, int maxMessages, int maxReactions) {
    // Weighted composite: messages 40%, reactions 20%, days active 25%, media 15%
    double msgScore = normalizeValue(activity.messageCount, maxMessages) * 40.0;
    double reactScore = normalizeValue(activity.reactionReceived, maxReactions) * 20.0;
    double daysScore = normalizeValue(activity.daysActive, 365) * 25.0;
    double mediaScore = normalizeValue(activity.mediaShared, 100) * 15.0;

    return msgScore + reactScore + daysScore + mediaScore;
}

std::string assignTier(double score) {
    if (score >= 75.0) return "Gold";
    if (score >= 50.0) return "Silver";
    if (score >= 25.0) return "Bronze";
    return "Newcomer";
}

std::string assignBadge(const std::string& tier) {
    if (tier == "Gold") return "\xF0\x9F\xA5\x87";     // 🥇
    if (tier == "Silver") return "\xF0\x9F\xA5\x88";   // 🥈
    if (tier == "Bronze") return "\xF0\x9F\xA5\x89";   // 🥉
    return "\xF0\x9F\x8C\xB1";                          // 🌱
}

RoomLeaderboard computeLeaderboard(
    const std::string& roomId,
    const std::vector<UserActivity>& activities
) {
    RoomLeaderboard board;
    board.roomId = roomId;
    board.totalUsers = static_cast<int>(activities.size());
    board.generatedAtMs = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();

    if (activities.empty()) return board;

    // Find max values for normalization
    int maxMsgs = 0, maxReact = 0;
    for (const auto& a : activities) {
        if (a.messageCount > maxMsgs) maxMsgs = a.messageCount;
        if (a.reactionReceived > maxReact) maxReact = a.reactionReceived;
    }

    // Compute scores
    for (const auto& a : activities) {
        UserRating rating;
        rating.userId = a.userId;
        rating.displayName = a.displayName;
        rating.score = computeUserScore(a, maxMsgs, maxReact);
        rating.tier = assignTier(rating.score);
        rating.badge = assignBadge(rating.tier);
        board.ratings.push_back(rating);
    }

    // Sort by score descending
    std::sort(board.ratings.begin(), board.ratings.end(),
        [](const UserRating& a, const UserRating& b) { return a.score > b.score; });

    // Assign ranks
    for (size_t i = 0; i < board.ratings.size(); ++i) {
        board.ratings[i].rank = static_cast<int>(i) + 1;
    }

    return board;
}

StreakInfo computeStreak(const std::vector<int64_t>& activeTimestampsMs) {
    StreakInfo streak;
    if (activeTimestampsMs.empty()) return streak;

    // Convert to days (UTC)
    std::set<int> activeDays;
    for (int64_t ts : activeTimestampsMs) {
        if (ts <= 0) continue;
        // Convert ms to days since epoch
        int day = static_cast<int>(ts / (1000 * 86400));
        activeDays.insert(day);
    }

    streak.totalActiveDays = static_cast<int>(activeDays.size());
    if (activeDays.empty()) return streak;

    // Check if active today
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    int today = static_cast<int>(now / (1000 * 86400));
    streak.isActiveToday = activeDays.find(today) != activeDays.end();

    // Compute streaks
    int current = 0, longest = 0;
    int prevDay = -1;

    for (int day : activeDays) {
        streak.lastActiveMs = static_cast<int64_t>(day) * 1000 * 86400;

        if (prevDay == -1 || day == prevDay + 1) {
            current++;
        } else {
            current = 1;
        }
        if (current > longest) longest = current;
        prevDay = day;
    }

    streak.currentStreak = (today - prevDay <= 1) ? current : 0;
    streak.longestStreak = longest;

    return streak;
}

std::string formatStreakText(const StreakInfo& streak) {
    std::ostringstream out;
    out << "Current streak: " << streak.currentStreak << " days\n";
    out << "Longest streak: " << streak.longestStreak << " days\n";
    out << "Total active days: " << streak.totalActiveDays << "\n";
    if (streak.isActiveToday) out << "Active today? Yes";
    return out.str();
}

std::string leaderboardToJson(const RoomLeaderboard& board) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << "{";
    json << R"("roomId": ")" << esc(board.roomId) << R"(",)";
    json << R"("totalUsers": )" << board.totalUsers << ",";
    json << R"("ratings": [)";
    for (size_t i = 0; i < board.ratings.size(); ++i) {
        if (i > 0) json << ",";
        const auto& r = board.ratings[i];
        json << R"({"userId": ")" << esc(r.userId) << R"(")";
        json << R"(,"displayName": ")" << esc(r.displayName) << R"(")";
        json << R"(,"score": )" << r.score;
        json << R"(,"rank": )" << r.rank;
        json << R"(,"tier": ")" << r.tier << R"(")";
        json << R"(,"badge": ")" << esc(r.badge) << R"(")" << "}";
    }
    json << "]}";
    return json.str();
}

std::string leaderboardToText(const RoomLeaderboard& board, int topN) {
    std::ostringstream out;
    out << "Room Leaderboard\n===============\n";
    int count = std::min(topN, static_cast<int>(board.ratings.size()));
    for (int i = 0; i < count; ++i) {
        const auto& r = board.ratings[i];
        out << r.badge << " #" << r.rank << " " << r.displayName
            << " — " << static_cast<int>(r.score) << " pts (" << r.tier << ")\n";
    }
    return out.str();
}

} // namespace progressive
