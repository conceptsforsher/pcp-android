#ifndef PROGRESSIVE_SYNC_UTILS_HPP
#define PROGRESSIVE_SYNC_UTILS_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// Initial sync reason — from InitialSyncRequestReason.kt (27L)
enum class InitialSyncReason { IgnoredUsersListChange, Unknown };

// ---- Sync Filter Builder ----

struct SyncFilter {
    std::vector<std::string> notTypes;      // exclude event types
    std::vector<std::string> notSenders;    // exclude senders
    std::vector<std::string> notRooms;      // exclude rooms
    std::vector<std::string> rooms;          // include rooms (empty = all)
    bool includeThreads = true;
    bool includePresence = false;
    int timelineLimit = 20;                  // events per room
    bool lazyLoadMembers = true;
};

// Build Matrix sync filter JSON.
std::string buildSyncFilter(const SyncFilter& filter);

// Build a default sync filter with reasonable settings.
SyncFilter getDefaultSyncFilter();

// Build a minimal sync filter (for background sync).
SyncFilter getBackgroundSyncFilter();

// Add a room to the sync filter's room list.
void addRoomToFilter(SyncFilter& filter, const std::string& roomId);

// Check if a sync filter includes a specific room.
bool filterIncludesRoom(const SyncFilter& filter, const std::string& roomId);

// ---- Sync Token Management ----

struct SyncToken {
    std::string token;
    int64_t savedAtMs = 0;
    bool isInitialSync = true;    // first sync after login
    int syncCount = 0;            // how many syncs have been done
    bool valid = false;
};

// Validate a sync token.
bool isValidSyncToken(const SyncToken& token);

// Check if sync token has expired (usually 30 minutes max).
bool isSyncTokenExpired(const SyncToken& token, int maxAgeMinutes = 30);

// Update sync token after successful sync.
SyncToken updateSyncToken(const std::string& nextBatchToken);

// Format sync token state for debug.
std::string formatSyncToken(const SyncToken& token);

// ---- Sync Presets ----

enum class SyncPreset {
    Full,           // all events
    Limited,        // recent events only
    Online,         // no history on reconnect
    Offline,        // cached only, no server
};

// Get sync timeout for a preset (milliseconds).
int getSyncTimeoutMs(SyncPreset preset);

// Get sync filter preset for common scenarios.
SyncFilter getFilterForPreset(SyncPreset preset);

} // namespace progressive

#endif // PROGRESSIVE_SYNC_UTILS_HPP
