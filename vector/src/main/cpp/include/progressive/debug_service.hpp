#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// Debug Service — provides debug information about the SDK state.
//
// Original Kotlin (DebugService.kt / DefaultDebugService.kt:28-44):
//   interface DebugService {
//       fun getAllRealmConfigurations(): List<RealmConfiguration>
//       fun getDbUsageInfo(): String
//   }

struct DebugDbInfo {
    std::string dbName;
    int64_t fileSizeBytes = 0;       // Size of the database file
    int objectCount = 0;             // Total Realm objects
    int tableCount = 0;              // Number of tables/classes
    int version = 0;                 // Schema version
    std::string encryptionStatus;    // "none", "encrypted", "unknown"
};

struct DebugServiceInfo {
    std::vector<DebugDbInfo> databases;    // Info for each Realm database
    int sessionCount = 0;                  // Number of active sessions
    std::string sdkVersion;                // SDK version string
    std::string deviceInfo;                // Device model, OS version
    int64_t totalDbSize = 0;               // Sum of all DB sizes

    std::string toSummary() const;
};

// Original Kotlin (DefaultDebugService.kt:34-44):
//   fun getDbUsageInfo() = buildString {
//       append(RealmDebugTools(realmConfigurationAuth).getInfo("Auth"))
//       append(RealmDebugTools(realmConfigurationGlobal).getInfo("Global"))
//       append(sessionManager.getLastSession()?.getDbUsageInfo())
//   }

// Serialize debug info to a human-readable string.
std::string debugServiceInfoToJson(const DebugServiceInfo& info);

// Parse individual DB usage info from a Realm debug report string.
DebugDbInfo parseRealmDbInfo(const std::string& reportJson);

} // namespace progressive
