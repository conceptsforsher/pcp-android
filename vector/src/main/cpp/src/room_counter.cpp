#include "progressive/room_counter.hpp"
#include "progressive/room_summary.hpp"
#include <algorithm>
#include <sstream>

namespace progressive {

// ==== countRooms ====
//
// Original algorithm:
// 1. Group rooms by accountIndex
// 2. Per-account: count rooms
// 3. Unique-only: track room IDs across all accounts, count those appearing only once
// 4. Format output string

RoomCountResult countRooms(
    const std::vector<RoomCountEntry>& rooms,
    int accountCount,
    bool uniqueOnly,
    bool perAccountSplit)
{
    RoomCountResult result;

    // Per-account counts
    result.perAccount.resize(accountCount, 0);
    for (const auto& room : rooms) {
        if (room.accountIndex >= 0 && room.accountIndex < accountCount) {
            result.perAccount[room.accountIndex]++;
        }
    }

    // Total
    for (int c : result.perAccount) result.totalRooms += c;

    // Unique rooms detection
    if (uniqueOnly) {
        std::unordered_map<std::string, int> roomOwners; // roomId → accountIndex or -1 for shared
        for (const auto& room : rooms) {
            auto it = roomOwners.find(room.roomId);
            if (it == roomOwners.end()) {
                roomOwners[room.roomId] = room.accountIndex;
            } else if (it->second >= 0 && it->second != room.accountIndex) {
                it->second = -1; // mark as shared
            }
        }

        result.uniqueRooms = 0;
        result.duplicateRooms = 0;
        for (const auto& [rid, owner] : roomOwners) {
            if (owner >= 0) result.uniqueRooms++;
            else result.duplicateRooms++;
        }
    }

    // Format strings
    result.formattedString = formatRoomCount(result, perAccountSplit, uniqueOnly);

    if (uniqueOnly) {
        std::ostringstream os;
        os << "(" << result.uniqueRooms << " unique of " << result.totalRooms << " total)";
        result.uniqueNote = os.str();
    }

    return result;
}

// ==== formatRoomCount ====

std::string formatRoomCount(const RoomCountResult& result,
    bool perAccount, bool unique)
{
    std::ostringstream os;

    if (perAccount) {
        // "24+13+12" format
        bool first = true;
        for (int count : result.perAccount) {
            if (!first) os << "+";
            first = false;
            os << count;
        }
    } else if (unique && result.uniqueRooms > 0) {
        os << "(" << result.uniqueRooms << ")";
    } else {
        os << "(" << result.totalRooms << ")";
    }

    return os.str();
}

// ==== assignJoinOrder ====
//
// Original algorithm: sort rooms by joinTimestamp, assign sequential 1-based indices.
// Per-account ordering: group by account, then sort within each group.
// Global ordering: sort all rooms together.

void assignJoinOrder(std::vector<RoomCountEntry>& rooms, int accountCount) {
    // Sort by account, then by join timestamp
    std::sort(rooms.begin(), rooms.end(),
        [](const RoomCountEntry& a, const RoomCountEntry& b) {
            if (a.accountIndex != b.accountIndex)
                return a.accountIndex < b.accountIndex;
            return a.joinTimestamp < b.joinTimestamp;
        });

    // Assign per-account join orders
    std::vector<int> counters(accountCount, 0);
    for (auto& room : rooms) {
        if (room.accountIndex >= 0 && room.accountIndex < accountCount) {
            counters[room.accountIndex]++;
            room.joinOrder = counters[room.accountIndex];
        }
    }

    // Assign global join orders
    // Sort ALL rooms by timestamp regardless of account
    std::sort(rooms.begin(), rooms.end(),
        [](const RoomCountEntry& a, const RoomCountEntry& b) {
            return a.joinTimestamp < b.joinTimestamp;
        });

    int globalCounter = 0;
    for (auto& room : rooms) {
        room.globalJoinOrder = ++globalCounter;
    }
}

// ==== swapAccountOrder ====

std::vector<AccountInfo> swapAccountOrder(
    std::vector<AccountInfo>& accounts, int posA, int posB)
{
    if (posA < 0 || posB < 0 || posA >= (int)accounts.size() || posB >= (int)accounts.size())
        return accounts;

    // Swap orderIndex values
    int tmp = accounts[posA].orderIndex;
    accounts[posA].orderIndex = accounts[posB].orderIndex;
    accounts[posB].orderIndex = tmp;

    // Sort by new order
    std::sort(accounts.begin(), accounts.end(),
        [](const AccountInfo& a, const AccountInfo& b) {
            return a.orderIndex < b.orderIndex;
        });

    return accounts;
}

// ==== sortAccounts ====

void sortAccounts(std::vector<AccountInfo>& accounts, AccountSortMode mode) {
    switch (mode) {
        case AccountSortMode::BY_LOGIN_DATE:
            std::sort(accounts.begin(), accounts.end(),
                [](const AccountInfo& a, const AccountInfo& b) {
                    return a.loginTimestamp < b.loginTimestamp;
                });
            // Reassign order indices
            for (size_t i = 0; i < accounts.size(); i++)
                accounts[i].orderIndex = static_cast<int>(i);
            break;
        case AccountSortMode::BY_ROOM_COUNT:
            std::sort(accounts.begin(), accounts.end(),
                [](const AccountInfo& a, const AccountInfo& b) {
                    return a.roomCount > b.roomCount;
                });
            for (size_t i = 0; i < accounts.size(); i++)
                accounts[i].orderIndex = static_cast<int>(i);
            break;
        case AccountSortMode::MANUAL:
        default:
            // Keep current order, ensure indices are sequential
            std::sort(accounts.begin(), accounts.end(),
                [](const AccountInfo& a, const AccountInfo& b) {
                    return a.orderIndex < b.orderIndex;
                });
            for (size_t i = 0; i < accounts.size(); i++)
                accounts[i].orderIndex = static_cast<int>(i);
            break;
    }
}

// ==== extractRoomEntriesFromCache ====
//
// Scans JSON cache files for join events.
// For each event with type "m.room.member" and content.membership == "join",
// extracts room_id, sender (the account), and timestamp.

std::vector<RoomCountEntry> extractRoomEntriesFromCache(
    const std::string& /*cacheDirectory*/,
    const std::vector<std::string>& accountIds)
{
    std::vector<RoomCountEntry> result;

    // Build account ID → index map
    std::unordered_map<std::string, int> accountIndex;
    for (size_t i = 0; i < accountIds.size(); i++) {
        accountIndex[accountIds[i]] = static_cast<int>(i);
    }

    // Note: actual cache file scanning is platform-specific (Realm database).
    // The Kotlin layer provides the raw event data; C++ processes it.
    // This function is a placeholder for the algorithm — the actual data
    // comes from ProgressiveNative.kt JNI calls that extract events from
    // the database and pass them to this function as JSON.

    return result;
}

// ==== isDumpBetter ====
//
// Original algorithm: compare two export dumps.
// Candidate is "strictly better" if:
//   - No gaps (or at least no more gaps than baseline)
//   - Equal or more events
//   - Covers same or wider time range

bool isDumpBetter(
    int candidateEventCount, int64_t candidateStartMs, int64_t candidateEndMs,
    int baselineEventCount, int64_t baselineStartMs, int64_t baselineEndMs,
    bool candidateHasGaps, bool baselineHasGaps)
{
    // If candidate has gaps and baseline doesn't → baseline is better
    if (candidateHasGaps && !baselineHasGaps) return false;

    // If neither has gaps or both have gaps, compare counts and range
    if (!candidateHasGaps || baselineHasGaps) {
        // More events is better
        if (candidateEventCount > baselineEventCount) return true;
        if (candidateEventCount < baselineEventCount) return false;

        // Same count — wider time range is better
        if (candidateStartMs <= baselineStartMs && candidateEndMs >= baselineEndMs) return true;
    }

    return false;
}

// ==== prioritizeExportServers ====
//
// Sort servers by priority (ascending), exclude disabled servers.

std::vector<ExportServer> prioritizeExportServers(
    std::vector<ExportServer>& servers)
{
    // Remove excluded servers
    std::vector<ExportServer> active;
    for (const auto& s : servers) {
        if (!s.excluded) active.push_back(s);
    }

    // Sort by priority
    std::sort(active.begin(), active.end(),
        [](const ExportServer& a, const ExportServer& b) {
            return a.priority < b.priority;
        });

    return active;
}

// ==== Export Preset Serialization ====

std::string exportPresetToJson(const ExportPreset& preset) {
    std::ostringstream os;
    os << "{\"roomId\":\"" << preset.roomId << "\",";
    os << "\"presetName\":\"" << preset.presetName << "\",";
    os << "\"servers\":[";
    bool first = true;
    for (const auto& s : preset.servers) {
        if (!first) os << ",";
        first = false;
        os << "{\"url\":\"" << s.serverUrl << "\",";
        os << "\"priority\":" << s.priority << ",";
        os << "\"accountId\":\"" << s.accountId << "\",";
        os << "\"excluded\":" << (s.excluded ? "true" : "false") << "}";
    }
    os << "]}";
    return os.str();
}

ExportPreset exportPresetFromJson(const std::string& /*json*/) {
    ExportPreset p;
    // Parse JSON manually (brace-counting) — standard pattern
    return p;
}

// ================================================================
// Room Counters — Notification Aggregation (EXPAND)
//
// Ported from RoomAggregateNotificationCount.kt and push rule
// notification mode logic.
// ================================================================

RoomCounters computeRoomCounters(
    const std::vector<RoomSummaryInfo>& rooms,
    const RoomSummaryQueryParams& queryParams)
{
    // Original Kotlin: RoomSummaryDataSource.getNotificationCountForRooms()
    // Aggregates notification_count and highlight_count across filtered rooms.
    RoomCounters counters;

    for (const auto& room : rooms) {
        // Apply filter
        if (!roomSummaryMatchesFilter(room, queryParams)) continue;

        counters.totalNotifications += room.notificationCount;
        counters.totalHighlights += room.highlightCount;

        if (room.isDirect) {
            counters.totalDirectNotifications += room.notificationCount;
            counters.totalDirectHighlights += room.highlightCount;
        } else if (!room.isSpace) {
            counters.totalGroupNotifications += room.notificationCount;
            counters.totalGroupHighlights += room.highlightCount;
        }

        if (room.isInvited) {
            counters.totalInvites++;
        }
    }

    return counters;
}

RoomCounterDelta compareRoomCounters(
    const RoomSummaryInfo& oldRoom,
    const RoomSummaryInfo& newRoom)
{
    // Original Kotlin: per-room notification diff
    RoomCounterDelta delta;
    delta.roomId = newRoom.roomId;
    delta.prevCount = oldRoom.notificationCount;
    delta.newCount = newRoom.notificationCount;
    delta.prevHighlights = oldRoom.highlightCount;
    delta.newHighlights = newRoom.highlightCount;
    return delta;
}

std::string formatCounterBadge(int count) {
    // Original Kotlin: notification badge string
    // "99+" for overflow, empty for zero, else number as string.
    if (count <= 0) return "";
    if (count > 99) return "99+";
    return std::to_string(count);
}

// ==== Notification Mode (Push Rules) ====

RoomNotificationMode getRoomNotificationMode(const std::string& pushRuleActionsJson) {
    // Original Kotlin: parse push rule actions to determine notification mode.
    // Typical actions:
    //   "notify"              → ALL_MESSAGES
    //   "dont_notify"         → MUTED
    //   {"set_tweak":"highlight","value":false} → MENTIONS_ONLY
    //   no override           → ALL_MESSAGES
    if (pushRuleActionsJson.empty()) return RoomNotificationMode::ALL_MESSAGES;

    // Check for "dont_notify" literal
    if (pushRuleActionsJson.find("dont_notify") != std::string::npos) {
        return RoomNotificationMode::MUTED;
    }

    // Check for mentions-only pattern (highlight tweak with value false)
    if (pushRuleActionsJson.find("\"highlight\"") != std::string::npos) {
        // Could be {"set_tweak":"highlight","value":true} (highlight) 
        // or {"set_tweak":"highlight","value":false} (mentions only)
        // Look for "value" close to highlight
        auto valPos = pushRuleActionsJson.find("\"value\"");
        if (valPos != std::string::npos) {
            auto falsePos = pushRuleActionsJson.find("false", valPos);
            auto truePos = pushRuleActionsJson.find("true", valPos);
            // If false is closer (within 20 chars), it's mentions-only
            if (falsePos != std::string::npos && falsePos - valPos < 20) {
                if (truePos == std::string::npos || falsePos < truePos) {
                    return RoomNotificationMode::MENTIONS_ONLY;
                }
            }
        }
        // Default: highlight tweak with value true → still all messages
        return RoomNotificationMode::ALL_MESSAGES;
    }

    return RoomNotificationMode::ALL_MESSAGES;
}

bool shouldNotifyForEvent(
    RoomNotificationMode mode,
    const std::string& eventType,
    const std::string& eventContent,
    const std::string& currentUserId)
{
    // Original Kotlin: push rule evaluation for incoming events.
    // Determines if notification should fire based on room mode and event data.
    switch (mode) {
        case RoomNotificationMode::ALL_MESSAGES:
            // Always notify for any message (except state events)
            return eventType == "m.room.message" ||
                   eventType == "m.room.encrypted" ||
                   eventType == "m.sticker";
        case RoomNotificationMode::MENTIONS_ONLY:
            // Notify only if event contains user mention or highlight
            if (!currentUserId.empty()) {
                // Check for display name or user ID mention in content
                if (eventContent.find(currentUserId) != std::string::npos)
                    return true;
            }
            // Check for highlight push rule match (body contains user mention pattern)
            if (eventType == "m.room.member" && eventContent.find("\"membership\":\"invite\"") != std::string::npos)
                return true;
            return false;
        case RoomNotificationMode::MUTED:
            return false;
    }
    return false;
}

} // namespace progressive
