#include "progressive/avatar_history.hpp"
#include <sstream>
#include <ctime>
#include <algorithm>
#include <regex>
#include <cctype>

namespace progressive {

// ---- AvatarHistory ----

void AvatarHistory::addChange(const std::string& mxcUrl, const std::string& eventId, int64_t timestamp) {
    AvatarEntry entry;
    entry.mxcUrl   = mxcUrl;
    entry.eventId  = eventId;
    entry.setAtMs  = timestamp;
    entry.setDate  = formatDate(timestamp);

    // Mark previous entry as removed (if any)
    if (!entries_.empty() && entries_[0].removedAtMs == 0) {
        entries_[0].removedAtMs = timestamp;
        entries_[0].removedDate = formatDate(timestamp);
        entries_[0].isCurrent = false;
    }

    entry.isCurrent = true;
    entries_.insert(entries_.begin(), entry);
}

std::vector<AvatarEntry> AvatarHistory::getHistory() const {
    auto result = entries_;
    std::sort(result.begin(), result.end(), [](const AvatarEntry& a, const AvatarEntry& b) {
        return a.setAtMs > b.setAtMs;
    });
    return result;
}

const AvatarEntry* AvatarHistory::getCurrent() const {
    for (const auto& e : entries_) {
        if (e.isCurrent) return &e;
    }
    return nullptr;
}

void AvatarHistory::clear() {
    entries_.clear();
}

std::string AvatarHistory::formatDate(int64_t epochMs) {
    if (epochMs <= 0) return "";
    time_t ts = epochMs / 1000;
    struct tm result;
    gmtime_r(&ts, &result);
    char buf[64];
    strftime(buf, sizeof(buf), "%B %d, %Y", &result);
    return std::string(buf);
}

std::string AvatarHistory::exportJson() const {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    auto history = getHistory();
    std::ostringstream json;
    json << "[";
    for (size_t i = 0; i < history.size(); ++i) {
        if (i > 0) json << ",";
        const auto& e = history[i];
        json << R"({"mxcUrl": ")" << esc(e.mxcUrl) << R"(")";
        json << R"(,"eventId": ")" << esc(e.eventId) << R"(")";
        json << R"(,"setAtMs": )" << e.setAtMs;
        json << R"(,"setDate": ")" << esc(e.setDate) << R"(")";
        json << R"(,"isCurrent": )" << (e.isCurrent ? "true" : "false");
        if (!e.isCurrent && !e.removedDate.empty()) {
            json << R"(,"removedAtMs": )" << e.removedAtMs;
            json << R"(,"removedDate": ")" << esc(e.removedDate) << R"(")";
        }
        json << "}";
    }
    json << "]";
    return json.str();
}

// ---- JumpToDate with Time ----

JumpToDateTarget parseJumpToDate(const std::string& input) {
    JumpToDateTarget target;

    // Try "YYYY-MM-DD HH:MM" or "YYYY-MM-DD HH:MM:SS" or "YYYY-MM-DD"
    std::regex dateTimeRe(R"(^(\d{4})-(\d{2})-(\d{2})(?:\s+(\d{1,2}):(\d{2})(?::(\d{2}))?)?$)");
    std::smatch match;
    if (!std::regex_match(input, match, dateTimeRe)) {
        target.error = "Invalid format. Use YYYY-MM-DD or YYYY-MM-DD HH:MM";
        return target;
    }

    target.year  = std::stoi(match[1]);
    target.month = std::stoi(match[2]);
    target.day   = std::stoi(match[3]);

    if (target.month < 1 || target.month > 12) {
        target.error = "Invalid month. Must be 01-12.";
        return target;
    }
    if (target.day < 1 || target.day > 31) {
        target.error = "Invalid day. Must be 01-31.";
        return target;
    }

    // Days per month validation
    static const int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    auto isLeap = [](int y) { return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0); };
    int maxDay = daysInMonth[target.month - 1];
    if (target.month == 2 && isLeap(target.year)) maxDay = 29;
    if (target.day > maxDay) {
        target.error = "Invalid day for given month.";
        return target;
    }

    // Time
    if (match[4].matched && match[5].matched) {
        target.hour   = std::stoi(match[4]);
        target.minute = std::stoi(match[5]);
        if (target.hour < 0 || target.hour > 23) {
            target.error = "Invalid hour. Must be 00-23.";
            return target;
        }
        if (target.minute < 0 || target.minute > 59) {
            target.error = "Invalid minute. Must be 00-59.";
            return target;
        }
        target.hasTime = true;
    }

    // Compute UTC timestamp
    struct tm timeinfo = {};
    timeinfo.tm_year = target.year - 1900;
    timeinfo.tm_mon  = target.month - 1;
    timeinfo.tm_mday = target.day;
    timeinfo.tm_hour = target.hour;
    timeinfo.tm_min  = target.minute;
    timeinfo.tm_sec  = 0;

    // timegm for UTC
    target.timestampMs = static_cast<int64_t>(timegm(&timeinfo)) * 1000;
    target.valid = true;
    return target;
}

std::string formatJumpTarget(const JumpToDateTarget& target) {
    if (!target.valid) return "";
    std::ostringstream out;
    out << target.year << "-"
        << (target.month < 10 ? "0" : "") << target.month << "-"
        << (target.day < 10 ? "0" : "") << target.day;
    if (target.hasTime) {
        out << " "
            << (target.hour < 10 ? "0" : "") << target.hour << ":"
            << (target.minute < 10 ? "0" : "") << target.minute;
    }
    return out.str();
}

// ---- Room Matching ----


double fuzzyScore(const std::string& query, const std::string& candidate) {
    if (query.empty()) return 0.0;
    if (candidate.empty()) return 0.0;

    auto lowerQuery = query;
    auto lowerCandidate = candidate;
    std::transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);
    std::transform(lowerCandidate.begin(), lowerCandidate.end(), lowerCandidate.begin(), ::tolower);

    // Exact match
    if (lowerQuery == lowerCandidate) return 1.0;

    // Starts with
    if (lowerCandidate.rfind(lowerQuery, 0) == 0) return 0.8;

    // Contains
    if (lowerCandidate.find(lowerQuery) != std::string::npos) return 0.5;

    // Character-by-character fuzzy match
    size_t qi = 0;
    int matches = 0;
    int consecutive = 0;
    int bestConsecutive = 0;

    for (size_t ci = 0; ci < lowerCandidate.size() && qi < lowerQuery.size(); ++ci) {
        if (lowerCandidate[ci] == lowerQuery[qi]) {
            matches++;
            consecutive++;
            if (consecutive > bestConsecutive) bestConsecutive = consecutive;
            qi++;
        } else {
            consecutive = 0;
        }
    }

    if (matches == 0) return 0.0;

    double matchRatio = static_cast<double>(matches) / static_cast<double>(lowerQuery.size());
    double consecutiveBonus = static_cast<double>(bestConsecutive) / static_cast<double>(lowerQuery.size()) * 0.3;

    return std::min(0.7, matchRatio * 0.5 + consecutiveBonus);
}

std::vector<RoomMatch> matchRooms(const std::string& query, const std::vector<RoomMatch>& rooms) {
    std::vector<std::pair<RoomMatch, double>> scored;

    for (const auto& room : rooms) {
        double bestScore = 0.0;

        // Check exact match on roomId or alias first
        if (query == room.roomId || query == room.canonicalAlias) {
            bestScore = 1.0;
        } else {
            bestScore = std::max({
                fuzzyScore(query, room.roomName),
                fuzzyScore(query, room.roomId),
                fuzzyScore(query, room.canonicalAlias)
            });
        }

        if (bestScore > 0.0) {
            auto copy = room;
            copy.score = bestScore;
            scored.push_back({copy, bestScore});
        }
    }

    // Sort by score descending
    std::sort(scored.begin(), scored.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    std::vector<RoomMatch> result;
    for (const auto& [room, _] : scored) {
        result.push_back(room);
    }
    return result;
}

} // namespace progressive
