#include "progressive/client_info.hpp"
#include <sstream>
#include <regex>

namespace progressive {

std::string buildUserAgent(const ClientVersionInfo& info) {
    std::ostringstream ua;
    ua << info.appName << "/" << info.appVersion;
    ua << " (Linux; " << info.platform << "; " << info.deviceModel;
    ua << "; " << info.architecture << ")";
    ua << " MatrixSDK/" << info.sdkVersion;
    if (!info.olmVersion.empty()) {
        ua << " Olm/" << info.olmVersion;
    }
    return ua.str();
}

std::string buildClientVersionQuery(const ClientVersionInfo& info) {
    std::ostringstream query;
    query << "?client_name=" << info.appName
          << "&client_version=" << info.appVersion;
    if (!info.sdkVersion.empty()) query << "&sdk_version=" << info.sdkVersion;
    return query.str();
}

std::string parseServerVersion(const std::string& apiResponseJson) {
    // Parse from {"versions": ["v1.1", "v1.2"], "unstable_features": {...}}
    auto serverInfo = apiResponseJson.find("\"server\"");
    if (serverInfo == std::string::npos) return "unknown";

    auto vStart = apiResponseJson.find("\"version\"", serverInfo);
    if (vStart == std::string::npos) return "unknown";

    vStart = apiResponseJson.find('"', vStart + 9);
    if (vStart == std::string::npos) return "unknown";
    ++vStart;
    auto vEnd = apiResponseJson.find('"', vStart);
    if (vEnd == std::string::npos) return "unknown";

    return apiResponseJson.substr(vStart, vEnd - vStart);
}

bool isServerCompatible(const std::string& serverVersion, const std::string& minRequiredVersion) {
    return satisfiesMinVersion(serverVersion, minRequiredVersion);
}

std::string formatVersionInfo(const ClientVersionInfo& info) {
    std::ostringstream out;
    out << info.appName << " " << info.appVersion << "\n";
    out << "SDK: " << info.sdkVersion << "\n";
    if (!info.olmVersion.empty()) out << "Olm: " << info.olmVersion << "\n";
    out << "Platform: " << info.platform << "\n";
    out << "Device: " << info.deviceModel << "\n";
    out << "Arch: " << info.architecture << "\n";
    if (!info.gitHash.empty()) out << "Build: " << info.gitHash;
    return out.str();
}

int compareSemver(const std::string& a, const std::string& b) {
    if (a == b) return 0;

    auto split = [](const std::string& s) -> std::vector<int> {
        std::vector<int> parts;
        std::istringstream stream(s);
        std::string part;
        while (std::getline(stream, part, '.')) {
            // Strip non-numeric suffix
            auto end = part.find_first_not_of("0123456789");
            if (end != std::string::npos) part = part.substr(0, end);
            if (part.empty()) parts.push_back(0);
            else parts.push_back(std::stoi(part));
        }
        return parts;
    };

    auto pa = split(a);
    auto pb = split(b);

    size_t maxLen = std::max(pa.size(), pb.size());
    pa.resize(maxLen, 0);
    pb.resize(maxLen, 0);

    for (size_t i = 0; i < maxLen; ++i) {
        if (pa[i] < pb[i]) return -1;
        if (pa[i] > pb[i]) return 1;
    }

    return 0;
}

bool satisfiesMinVersion(const std::string& current, const std::string& minimum) {
    return compareSemver(current, minimum) >= 0;
}

bool isDevelopmentBuild(const BuildInfo& info) {
    return info.isDebug || info.isNightly;
}

std::string formatBuildInfo(const BuildInfo& info) {
    std::ostringstream out;
    out << info.applicationId << " " << info.versionName << " (" << info.versionCode << ")\n";
    out << "Flavor: " << info.flavor << "\n";
    out << "Type: " << (info.isDebug ? "Debug" : "Release");
    if (info.isNightly) out << " (Nightly)";
    return out.str();
}

std::string getAppDisplayName(const BuildInfo& info) {
    if (info.isDebug) return "Progressive Chat (Debug)";
    if (info.isNightly) return "Progressive Chat (Nightly)";
    return "Progressive Chat";
}

} // namespace progressive
