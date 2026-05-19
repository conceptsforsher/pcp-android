#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>
#include <unordered_set>

namespace progressive {

// ==== Room Counter — Advanced Room Counting & Numbering ====
//
// Provides room count aggregation (total, per-account, unique),
// room join-order numbering, and account ordering for multi-account setups.

// A single room entry for counting purposes.
struct RoomCountEntry {
    std::string roomId;
    std::string roomName;
    int accountIndex = 0;            // which account (0-based order)
    int64_t joinTimestamp = 0;       // when user joined (epoch ms)
    int joinOrder = 0;               // computed: 1-based join order per account
    int globalJoinOrder = 0;         // computed: 1-based order across all accounts
};

// Result of room counting.
struct RoomCountResult {
    int totalRooms = 0;              // total across all accounts
    std::vector<int> perAccount;     // count per account: [24, 13, 12]
    int uniqueRooms = 0;             // rooms not shared across accounts
    int duplicateRooms = 0;          // rooms shared by 2+ accounts
    std::string formattedString;     // "24+13+12" or "(49)"
    std::string uniqueNote;          // "(24 unique of 49 total)" if unique-only mode
};

// Account info for ordering and display.
struct AccountInfo {
    int orderIndex = 0;              // 0-based display order (user-configurable)
    std::string accountId;           // "@user:server.org"
    std::string displayName;         // user-visible name
    int64_t loginTimestamp = 0;      // when user first logged in with this account
    int roomCount = 0;               // cached count of rooms for this account
};

// ==== Core Counting Functions ====

// Count rooms from a list of entries across multiple accounts.
// Original algorithm: iterate all rooms, group by account,
// detect unique vs shared rooms.
RoomCountResult countRooms(
    const std::vector<RoomCountEntry>& rooms,
    int accountCount,
    bool uniqueOnly = false,
    bool perAccountSplit = false
);

// Format the room count as "(N)" or per-account "A+B+C"
std::string formatRoomCount(const RoomCountResult& result,
    bool perAccount = false, bool unique = false);

// ==== Join Order Numbering ====

// Assign join order numbers (1-based) to rooms.
// Sorts by joinTimestamp, assigns sequential order numbers.
// Supports per-account and global numbering.
void assignJoinOrder(
    std::vector<RoomCountEntry>& rooms,
    int accountCount
);

// ==== Account Ordering ====

// Reorder accounts by swapping two positions.
// Returns updated account list with new orderIndex values.
std::vector<AccountInfo> swapAccountOrder(
    std::vector<AccountInfo>& accounts,
    int posA, int posB
);

// Sort accounts by a criterion: login date, room count, or manual order.
enum class AccountSortMode { MANUAL = 0, BY_LOGIN_DATE = 1, BY_ROOM_COUNT = 2 };

void sortAccounts(std::vector<AccountInfo>& accounts, AccountSortMode mode);

// ==== Cache/Export-Based Counting ====

// Scan JSON event cache for join (m.room.member with membership=join) events.
// Extracts room_id, sender (account), and timestamp.
// Returns list of RoomCountEntry suitable for countRooms().
std::vector<RoomCountEntry> extractRoomEntriesFromCache(
    const std::string& cacheDirectory,
    const std::vector<std::string>& accountIds
);

// ==== Export: Multi-Server Dump Priority ====

// Information about a server for multi-server export.
struct ExportServer {
    std::string serverUrl;           // "https://matrix.example.org"
    int priority = 0;                // lower = higher priority
    std::string accountId;           // which account to use for this server
    int64_t lastSuccessMs = 0;       // last successful export time
    bool excluded = false;           // user-disabled this server
};

// Compare two export dumps: returns true if 'candidate' is strictly better than 'baseline'.
// "Better" means: no missing events, same or larger event count,
// earlier start timestamp, later end timestamp.
// Original algorithm: compare dump manifests and select the best one.
bool isDumpBetter(
    int candidateEventCount, int64_t candidateStartMs, int64_t candidateEndMs,
    int baselineEventCount, int64_t baselineStartMs, int64_t baselineEndMs,
    bool candidateHasGaps, bool baselineHasGaps
);

// Pick the best server for export based on priority and recent success.
std::vector<ExportServer> prioritizeExportServers(
    std::vector<ExportServer>& servers
);

// Serialize/deserialize export preset (per-room server config).
struct ExportPreset {
    std::string roomId;
    std::vector<ExportServer> servers;
    std::string presetName;
};

std::string exportPresetToJson(const ExportPreset& preset);
ExportPreset exportPresetFromJson(const std::string& json);

} // namespace progressive
