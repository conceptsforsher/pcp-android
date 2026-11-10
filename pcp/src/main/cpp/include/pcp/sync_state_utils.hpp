#pragma once
#include <string>
#include <cstdint>
namespace pcp {
struct SyncState { bool isSyncing=false; int64_t lastSyncMs=0; int pendingRooms=0; std::string nextBatch; };
SyncState parseSyncState(const std::string& json);
std::string formatSyncState(const SyncState& s);
bool isInitialSync(const SyncState& s);
}
