#include "progressive/sync_utils.hpp"
#include <sstream>
#include <chrono>

namespace progressive {

std::string buildSyncFilter(const SyncFilter& filter) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };

    std::ostringstream json;
    json << "{";

    // Room filter
    json << R"("room": {)";
    if (!filter.rooms.empty()) {
        json << R"("rooms": [)";
        for (size_t i = 0; i < filter.rooms.size(); ++i) {
            if (i > 0) json << ",";
            json << R"(")" << esc(filter.rooms[i]) << R"(")";
        }
        json << "],";
    }
    if (!filter.notRooms.empty()) {
        json << R"("not_rooms": [)";
        for (size_t i = 0; i < filter.notRooms.size(); ++i) {
            if (i > 0) json << ",";
            json << R"(")" << esc(filter.notRooms[i]) << R"(")";
        }
        json << "],";
    }
    json << R"("timeline": {"limit": )" << filter.timelineLimit << "},";
    json << R"("lazy_load_members": )" << (filter.lazyLoadMembers ? "true" : "false");
    json << "},";

    // Thread filter
    json << R"("threads": {"include": )" << (filter.includeThreads ? "true" : "false") << "},";

    // Presence filter
    json << R"("presence": {"include": )" << (filter.includePresence ? "true" : "false") << "},";

    // Event filter
    json << R"("event_format": "client")";

    json << "}";

    // Clean trailing commas
    std::string result = json.str();
    // Simple cleanup of trailing commas before }
    size_t pos = 0;
    while ((pos = result.find(",}", pos)) != std::string::npos) {
        result.erase(pos, 1);
    }

    return result;
}

SyncFilter getDefaultSyncFilter() {
    SyncFilter filter;
    filter.timelineLimit = 20;
    filter.includeThreads = true;
    filter.lazyLoadMembers = true;
    return filter;
}

SyncFilter getBackgroundSyncFilter() {
    SyncFilter filter;
    filter.timelineLimit = 1;
    filter.includeThreads = false;
    filter.includePresence = false;
    filter.lazyLoadMembers = false;
    return filter;
}

void addRoomToFilter(SyncFilter& filter, const std::string& roomId) {
    // Check not already in list
    for (const auto& r : filter.rooms) {
        if (r == roomId) return;
    }
    filter.rooms.push_back(roomId);
}

bool filterIncludesRoom(const SyncFilter& filter, const std::string& roomId) {
    for (const auto& r : filter.rooms) {
        if (r == roomId) return true;
    }
    return filter.rooms.empty(); // empty = all rooms
}

bool isValidSyncToken(const SyncToken& token) {
    return !token.token.empty();
}

bool isSyncTokenExpired(const SyncToken& token, int maxAgeMinutes) {
    if (!token.valid) return true;
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    return (now - token.savedAtMs) > maxAgeMinutes * 60 * 1000LL;
}

SyncToken updateSyncToken(const std::string& nextBatchToken) {
    SyncToken token;
    token.token = nextBatchToken;
    token.savedAtMs = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    token.isInitialSync = false;
    token.valid = true;
    return token;
}

std::string formatSyncToken(const SyncToken& token) {
    std::ostringstream out;
    out << "Sync token: " << (token.valid ? token.token.substr(0, 20) + "..." : "none") << "\n";
    out << "Initial sync: " << (token.isInitialSync ? "Yes" : "No") << "\n";
    out << "Sync count: " << token.syncCount;
    return out.str();
}

int getSyncTimeoutMs(SyncPreset preset) {
    switch (preset) {
        case SyncPreset::Full:    return 30000;  // 30 seconds
        case SyncPreset::Limited: return 15000;  // 15 seconds
        case SyncPreset::Online:  return 5000;   // 5 seconds
        case SyncPreset::Offline: return 5000;
        default:                  return 30000;
    }
}

SyncFilter getFilterForPreset(SyncPreset preset) {
    switch (preset) {
        case SyncPreset::Full:
            return getDefaultSyncFilter();
        case SyncPreset::Limited: {
            auto f = getDefaultSyncFilter();
            f.timelineLimit = 10;
            return f;
        }
        case SyncPreset::Online: {
            auto f = getDefaultSyncFilter();
            f.timelineLimit = 5;
            f.lazyLoadMembers = false;
            return f;
        }
        case SyncPreset::Offline:
        default:
            return getBackgroundSyncFilter();
    }
}

} // namespace progressive
