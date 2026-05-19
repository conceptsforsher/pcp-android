#include "progressive/debug_service.hpp"
#include <sstream>

namespace progressive {

// ==== Debug Service Info Summary ====
//
// Original Kotlin (DefaultDebugService.kt:34-44):
//   override fun getDbUsageInfo() = buildString {
//       append(RealmDebugTools(realmConfigurationAuth).getInfo("Auth"))
//       append(RealmDebugTools(realmConfigurationGlobal).getInfo("Global"))
//       append(sessionManager.getLastSession()?.getDbUsageInfo())
//   }

std::string DebugServiceInfo::toSummary() const {
    std::ostringstream os;
    os << "SDK Version: " << sdkVersion << "\n";
    os << "Device: " << deviceInfo << "\n";
    os << "Sessions: " << sessionCount << "\n";
    os << "Total DB Size: " << totalDbSize << " bytes\n";
    os << "--- Database Details ---\n";
    for (const auto& db : databases) {
        os << "  [" << db.dbName << "] v" << db.version
           << " tables=" << db.tableCount
           << " objects=" << db.objectCount
           << " size=" << db.fileSizeBytes << "B"
           << " enc=" << db.encryptionStatus
           << "\n";
    }
    return os.str();
}

// ==== JSON Serialization ====

std::string debugServiceInfoToJson(const DebugServiceInfo& info) {
    std::string json = "{";
    json += "\"sdkVersion\":\"" + info.sdkVersion + "\",";
    json += "\"deviceInfo\":\"" + info.deviceInfo + "\",";
    json += "\"sessionCount\":" + std::to_string(info.sessionCount) + ",";
    json += "\"totalDbSize\":" + std::to_string(info.totalDbSize) + ",";
    json += "\"databases\":[";
    bool first = true;
    for (const auto& db : info.databases) {
        if (!first) json += ",";
        first = false;
        json += "{\"name\":\"" + db.dbName + "\",";
        json += "\"size\":" + std::to_string(db.fileSizeBytes) + ",";
        json += "\"objects\":" + std::to_string(db.objectCount) + ",";
        json += "\"tables\":" + std::to_string(db.tableCount) + ",";
        json += "\"version\":" + std::to_string(db.version) + ",";
        json += "\"encryption\":\"" + db.encryptionStatus + "\"}";
    }
    json += "]}";
    return json;
}

// ==== Realm DB Info Parsing ====
//
// Original Kotlin: RealmDebugTools.getInfo(dbName) returns
// a string report about the Realm database.

DebugDbInfo parseRealmDbInfo(const std::string& reportJson) {
    DebugDbInfo info;

    auto pos = reportJson.find("\"name\"");
    if (pos != std::string::npos) {
        pos = reportJson.find(':', pos);
        if (pos != std::string::npos) {
            pos++;
            while (pos < reportJson.size() && (reportJson[pos] == ' ' || reportJson[pos] == '\t' || reportJson[pos] == '"')) pos++;
            size_t end = pos;
            while (end < reportJson.size() && reportJson[end] != '"') end++;
            info.dbName = reportJson.substr(pos, end - pos);
        }
    }

    // Extract numeric fields
    auto extractInt = [&](const std::string& key) -> int64_t {
        auto p = reportJson.find("\"" + key + "\"");
        if (p == std::string::npos) return 0;
        p = reportJson.find(':', p);
        if (p == std::string::npos) return 0;
        p++;
        while (p < reportJson.size() && (reportJson[p] == ' ' || reportJson[p] == '\t')) p++;
        int64_t v = 0;
        while (p < reportJson.size() && reportJson[p] >= '0' && reportJson[p] <= '9') {
            v = v * 10 + (reportJson[p] - '0');
            p++;
        }
        return v;
    };

    info.fileSizeBytes = extractInt("size");
    info.objectCount = static_cast<int>(extractInt("objects"));
    info.tableCount = static_cast<int>(extractInt("tables"));
    info.version = static_cast<int>(extractInt("version"));

    auto ep = reportJson.find("\"encryption\"");
    if (ep != std::string::npos) {
        ep = reportJson.find(':', ep);
        if (ep != std::string::npos) {
            ep++;
            while (ep < reportJson.size() && (reportJson[ep] == ' ' || reportJson[ep] == '\t' || reportJson[ep] == '"')) ep++;
            size_t end = ep;
            while (end < reportJson.size() && reportJson[end] != '"') end++;
            info.encryptionStatus = reportJson.substr(ep, end - ep);
        }
    }

    return info;
}

} // namespace progressive
