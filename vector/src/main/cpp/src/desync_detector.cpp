#include "progressive/desync_detector.hpp"
#include <sstream>
#include <chrono>

namespace progressive {

void DesyncDetector::trackEvent(const std::string& eventId, const std::string& serverName, int64_t timestamp) {
    serverEvents_[serverName].insert(eventId);
    
    auto it = eventTimestamps_.find(eventId);
    if (it == eventTimestamps_.end() || timestamp > it->second) {
        eventTimestamps_[eventId] = timestamp;
    }
}

DesyncReport DesyncDetector::checkDesync(const std::string& roomId, const std::string& currentServer) {
    DesyncReport report;
    report.roomId = roomId;
    report.lastCheckMs = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();

    auto servers = getServers(roomId);

    // Need at least 2 servers to detect desync
    if (servers.size() < 2) return report;

    // Get the event set for current server
    auto currentIt = serverEvents_.find(currentServer);
    if (currentIt == serverEvents_.end()) return report;

    const auto& currentSet = currentIt->second;

    // Compare against every other server
    for (const auto& otherServer : servers) {
        if (otherServer == currentServer) continue;

        auto otherIt = serverEvents_.find(otherServer);
        if (otherIt == serverEvents_.end()) continue;
        const auto& otherSet = otherIt->second;

        // Check: events on other server that are missing on current
        std::vector<std::string> missingOnCurrent;
        for (const auto& eid : otherSet) {
            if (currentSet.find(eid) == currentSet.end()) {
                missingOnCurrent.push_back(eid);
            }
        }

        // Check: events on current server that are missing on other
        std::vector<std::string> missingOnOther;
        for (const auto& eid : currentSet) {
            if (otherSet.find(eid) == otherSet.end()) {
                missingOnOther.push_back(eid);
            }
        }

        if (!missingOnCurrent.empty() || !missingOnOther.empty()) {
            report.hasDesync = true;
            report.totalEventsCompared = static_cast<int>(currentSet.size() + otherSet.size());
            report.missingOnCurrent += static_cast<int>(missingOnCurrent.size());
            report.missingOnOther += static_cast<int>(missingOnOther.size());
            report.missingServer = otherServer;

            for (const auto& eid : missingOnCurrent) {
                report.missingEventIds.push_back(eid);
            }
        }
    }

    return report;
}

std::vector<std::string> DesyncDetector::getServers(const std::string& roomId) const {
    std::vector<std::string> result;
    for (const auto& p : serverEvents_) {
        if (!p.second.empty()) result.push_back(p.first);
    }
    return result;
}

void DesyncDetector::removeRoom(const std::string& roomId) {
    // We don't store by roomId currently, so this is a no-op.
    // Future: could prefix eventIds with roomId for per-room tracking.
}

bool DesyncDetector::hasEventOnServer(const std::string& eventId, const std::string& serverName) const {
    auto it = serverEvents_.find(serverName);
    if (it == serverEvents_.end()) return false;
    return it->second.find(eventId) != it->second.end();
}

bool DesyncDetector::shouldCheck(int64_t lastCheckMs, int intervalMinutes) const {
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    return (now - lastCheckMs) >= (static_cast<int64_t>(intervalMinutes) * 60 * 1000LL);
}

std::string DesyncDetector::formatDesyncWarning(const DesyncReport& report) {
    if (!report.hasDesync) return "";

    std::ostringstream out;
    out << report.missingOnCurrent << " event(s) missing on current server";
    if (!report.missingServer.empty()) {
        out << " (present on " << report.missingServer << ")";
    }
    out << ". Check room federation.";
    return out.str();
}

std::string DesyncDetector::reportToJson(const DesyncReport& report) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    std::ostringstream json;
    json << "{";
    json << R"("hasDesync": )" << (report.hasDesync ? "true" : "false") << ",";
    json << R"("roomId": ")" << esc(report.roomId) << R"(",)";
    json << R"("totalEventsCompared": )" << report.totalEventsCompared << ",";
    json << R"("missingOnCurrent": )" << report.missingOnCurrent << ",";
    json << R"("missingOnOther": )" << report.missingOnOther << ",";
    json << R"("missingServer": ")" << esc(report.missingServer) << R"(",)";
    json << R"("warning": ")" << esc(formatDesyncWarning(report)) << R"(",)";
    json << R"("lastCheckMs": )" << report.lastCheckMs;
    if (!report.missingEventIds.empty()) {
        json << R"(,"missingEventIds": [)";
        for (size_t i = 0; i < report.missingEventIds.size(); ++i) {
            if (i > 0) json << ",";
            json << R"(")" << esc(report.missingEventIds[i]) << R"(")";
        }
        json << "]";
    }
    json << "}";
    return json.str();
}

void DesyncDetector::clear() {
    serverEvents_.clear();
    eventTimestamps_.clear();
}

// ---- DesyncCheckResult ----

DesyncCheckResult checkSyncDesynchronization(
    const std::string& roomId,
    int64_t lastSyncMs,
    const std::vector<std::string>& localEventIds,
    const std::vector<std::string>& serverEventIds,
    const std::string& localReadMarker,
    const std::string& serverReadMarker,
    const std::string& localMembership,
    const std::string& serverMembership
) {
    DesyncCheckResult result;
    (void)roomId; // roomId reserved for future logging / metric keying

    // Factor 1: Check sync freshness
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    result.lastSyncAgeMs = now - lastSyncMs;

    // Factor 2: Missing events in timeline
    std::unordered_set<std::string> localSet(localEventIds.begin(), localEventIds.end());
    for (const auto& eid : serverEventIds) {
        if (localSet.find(eid) == localSet.end()) {
            ++result.missingEvents;
        }
    }

    // Factor 3: Extra events locally that are not on server
    std::unordered_set<std::string> serverSet(serverEventIds.begin(), serverEventIds.end());
    for (const auto& eid : localEventIds) {
        if (serverSet.find(eid) == serverSet.end()) {
            ++result.extraEvents;
        }
    }

    // Factor 4: Read marker conflicts
    bool readMarkerConflict = !localReadMarker.empty() && !serverReadMarker.empty() &&
                               localReadMarker != serverReadMarker;

    // Factor 5: Membership inconsistencies
    bool membershipConflict = !localMembership.empty() && !serverMembership.empty() &&
                               localMembership != serverMembership;

    // Aggregate result
    result.isDesynchronized = (result.missingEvents > 0) || (result.extraEvents > 0) ||
                               readMarkerConflict || membershipConflict;

    if (result.isDesynchronized) {
        std::ostringstream reason;
        if (result.missingEvents > 0) {
            reason << result.missingEvents << " event(s) missing from local timeline; ";
        }
        if (result.extraEvents > 0) {
            reason << result.extraEvents << " event(s) extra in local timeline; ";
        }
        if (readMarkerConflict) {
            reason << "read marker conflict; ";
        }
        if (membershipConflict) {
            reason << "membership inconsistency; ";
        }
        result.reason = reason.str();
    } else {
        result.reason = "No desync detected";
    }

    return result;
}

// ---- DesyncCause ----

DesyncCause diagnoseDesyncCause(const DesyncCheckResult& result) {
    if (!result.isDesynchronized) return DesyncCause::UNKNOWN;

    // Cache corruption: extra events locally that server doesn't know about
    if (result.extraEvents > 0 && result.missingEvents == 0) {
        return DesyncCause::CACHE_CORRUPTION;
    }

    // Network gap: many missing events
    if (result.missingEvents > 10) {
        return DesyncCause::NETWORK_GAP;
    }

    // Race condition: few missing events, recently synced
    if (result.missingEvents <= 3 && result.lastSyncAgeMs < 60000) {
        return DesyncCause::RACE_CONDITION;
    }

    // Server bug / federation issue: events missing but no network gap
    if (result.missingEvents > 0 && result.lastSyncAgeMs < 300000) {
        return DesyncCause::SERVER_BUG;
    }

    return DesyncCause::UNKNOWN;
}

std::string getDesyncRecoveryAction(DesyncCause cause) {
    switch (cause) {
        case DesyncCause::NETWORK_GAP:     return "re-sync";
        case DesyncCause::CACHE_CORRUPTION: return "clear_cache";
        case DesyncCause::SERVER_BUG:       return "re-sync";
        case DesyncCause::RACE_CONDITION:   return "re-sync";
        default:                            return "none";
    }
}

std::string getDesyncRecoveryAction(const DesyncCheckResult& result) {
    if (!result.isDesynchronized) return "none";
    return getDesyncRecoveryAction(diagnoseDesyncCause(result));
}

} // namespace progressive
