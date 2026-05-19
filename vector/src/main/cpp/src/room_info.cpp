#include "progressive/room_info.hpp"
#include <sstream>
#include <ctime>

namespace progressive {

bool isLikelyFullHistory(int cachedCount, int estimatedTotal) {
    if (estimatedTotal <= 0) return false;
    return cachedCount >= static_cast<int>(estimatedTotal * 0.9);
}

std::string formatCreationDate(int64_t epochMs) {
    if (epochMs <= 0) return "";
    time_t ts = epochMs / 1000;
    struct tm result;
    gmtime_r(&ts, &result);
    char buf[64];
    strftime(buf, sizeof(buf), "%B %d, %Y at %H:%M UTC", &result);
    return std::string(buf);
}

std::string roomInfoToJson(const RoomInfo& info) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) {
            if (c == '"') out += "\\\"";
            else out += c;
        }
        return out;
    };

    std::ostringstream json;
    json << "{";
    json << R"("roomId": ")" << esc(info.roomId) << R"(",)";
    json << R"("roomName": ")" << esc(info.roomName) << R"(",)";
    json << R"("creationTs": )" << info.creationTs << ",";
    json << R"("creationDate": ")" << esc(info.creationDate) << R"(",)";
    json << R"("cachedEventCount": )" << info.cachedEventCount << ",";
    json << R"("estimatedTotalEvents": )" << info.estimatedTotalEvents << ",";
    json << R"("likelyFullHistory": )" << (info.likelyFullHistory ? "true" : "false");
    json << "}";
    return json.str();
}

int estimateTotalEvents(int cachedCount, bool hasMoreHistory) {
    if (!hasMoreHistory) return cachedCount;
    // If we have more history on server but don't know how much,
    // estimate a reasonable multiplier
    return 0; // unknown
}

} // namespace progressive
