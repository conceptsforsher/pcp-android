#pragma once
#include <string>
#include <cstdint>
namespace pcp {
struct SyncSummary { int roomsJoined=0; int roomsInvited=0; int roomsLeft=0; int totalEvents=0; int64_t syncDurationMs=0; std::string nextBatch; };
SyncSummary parseSyncSummary(const std::string& syncJson);
std::string formatSyncSummary(const SyncSummary& s);
}
