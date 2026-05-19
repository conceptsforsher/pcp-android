#include "progressive/location_sharing.hpp"
#include <sstream>
#include <cmath>
#include <chrono>

namespace progressive {

int LocationSharingManager::nextId_ = 1;

std::string LocationSharingManager::generateId() {
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    std::ostringstream id;
    id << "loc_" << now << "_" << (nextId_++);
    return id.str();
}

std::string LocationSharingManager::startSession(const LocationSession& session) {
    LocationSession copy = session;
    if (copy.sessionId.empty()) copy.sessionId = generateId();
    if (copy.startedAtMs == 0) {
        copy.startedAtMs = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
    }
    copy.active = true;
    sessions_.push_back(copy);
    return copy.sessionId;
}

void LocationSharingManager::stopSession(const std::string& sessionId) {
    for (auto& s : sessions_) {
        if (s.sessionId == sessionId) {
            s.active = false;
            return;
        }
    }
}

std::vector<LocationSession> LocationSharingManager::getActiveSessions(const std::string& userId) const {
    std::vector<LocationSession> result;
    for (const auto& s : sessions_) {
        if (s.active && s.userId == userId) result.push_back(s);
    }
    return result;
}

bool LocationSharingManager::isDue(const std::string& sessionId) const {
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();

    for (const auto& s : sessions_) {
        if (s.sessionId == sessionId && s.active) {
            return (now - s.lastSentMs) >= (s.intervalSeconds * 1000LL);
        }
    }
    return false;
}

void LocationSharingManager::markSent(const std::string& sessionId, const GeoCoord& coord) {
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();

    for (auto& s : sessions_) {
        if (s.sessionId == sessionId) {
            s.lastSentMs = now;
            s.lastCoord = coord;
            return;
        }
    }
}

std::vector<std::string> LocationSharingManager::checkAutoStop() {
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();

    std::vector<std::string> toStop;
    for (auto& s : sessions_) {
        if (!s.active || !s.autoStop) continue;
        auto elapsed = now - s.startedAtMs;
        if (elapsed >= s.autoStopMinutes * 60 * 1000LL) {
            toStop.push_back(s.sessionId);
        }
    }
    for (const auto& id : toStop) stopSession(id);
    return toStop;
}

const LocationSession* LocationSharingManager::getSession(const std::string& sessionId) const {
    for (const auto& s : sessions_) {
        if (s.sessionId == sessionId) return &s;
    }
    return nullptr;
}

void LocationSharingManager::clear() {
    sessions_.clear();
}

// ---- Coordinate Utilities ----

bool LocationSharingManager::isValidCoord(const GeoCoord& coord) {
    return coord.latitude >= -90.0 && coord.latitude <= 90.0 &&
           coord.longitude >= -180.0 && coord.longitude <= 180.0;
}

std::string LocationSharingManager::formatGeoUri(const GeoCoord& coord) {
    std::ostringstream uri;
    uri << "geo:" << coord.latitude << "," << coord.longitude;
    if (coord.accuracy > 0) uri << ";u=" << coord.accuracy;
    return uri.str();
}

std::string LocationSharingManager::formatLocationMessage(const GeoCoord& coord, const std::string& label) {
    std::ostringstream msg;
    if (!label.empty()) msg << label << ": ";
    msg << formatGeoUri(coord);
    if (coord.accuracy > 0) msg << " (±" << static_cast<int>(coord.accuracy) << "m)";
    return msg.str();
}

std::string LocationSharingManager::formatGeoJson(const GeoCoord& coord) {
    std::ostringstream json;
    json << R"({"type": "Point", "coordinates": [)"
         << coord.longitude << ", " << coord.latitude;
    if (coord.altitude != 0.0) json << ", " << coord.altitude;
    json << R"(], "properties": {)";
    json << R"("accuracy": )" << coord.accuracy;
    if (coord.timestampMs > 0) json << R"(, "timestamp": )" << coord.timestampMs;
    json << "}}";
    return json.str();
}

GeoCoord LocationSharingManager::parseFromMessage(const std::string& body) {
    GeoCoord coord;
    // Parse "geo:lat,lon" or "geo:lat,lon;u=acc"
    if (body.rfind("geo:", 0) != 0) return coord;

    auto rest = body.substr(4);
    auto semicolon = rest.find(';');
    auto coordStr = (semicolon != std::string::npos) ? rest.substr(0, semicolon) : rest;
    auto comma = coordStr.find(',');
    if (comma == std::string::npos) return coord;

    coord.latitude = std::stod(coordStr.substr(0, comma));
    coord.longitude = std::stod(coordStr.substr(comma + 1));
    coord.valid = isValidCoord(coord);

    // Parse accuracy if present
    auto u = rest.find("u=");
    if (u != std::string::npos) {
        auto accStr = rest.substr(u + 2);
        coord.accuracy = std::stod(accStr);
    }

    return coord;
}

double LocationSharingManager::distanceMeters(const GeoCoord& a, const GeoCoord& b) {
    // Haversine formula
    const double R = 6371000.0; // Earth radius in meters
    double lat1 = a.latitude * M_PI / 180.0;
    double lat2 = b.latitude * M_PI / 180.0;
    double dLat = (b.latitude - a.latitude) * M_PI / 180.0;
    double dLon = (b.longitude - a.longitude) * M_PI / 180.0;

    double sindLat = std::sin(dLat / 2.0);
    double sindLon = std::sin(dLon / 2.0);

    double a_hav = sindLat * sindLat + std::cos(lat1) * std::cos(lat2) * sindLon * sindLon;
    double c = 2.0 * std::atan2(std::sqrt(a_hav), std::sqrt(1.0 - a_hav));

    return R * c;
}

double LocationSharingManager::bearingDegrees(const GeoCoord& a, const GeoCoord& b) {
    double lat1 = a.latitude * M_PI / 180.0;
    double lat2 = b.latitude * M_PI / 180.0;
    double dLon = (b.longitude - a.longitude) * M_PI / 180.0;

    double y = std::sin(dLon) * std::cos(lat2);
    double x = std::cos(lat1) * std::sin(lat2) - std::sin(lat1) * std::cos(lat2) * std::cos(dLon);
    double bearing = std::atan2(y, x) * 180.0 / M_PI;
    if (bearing < 0) bearing += 360.0;

    return bearing;
}

bool LocationSharingManager::hasMoved(const GeoCoord& old, const GeoCoord& nxt, double thresholdMeters) {
    return distanceMeters(old, nxt) > thresholdMeters;
}

std::string LocationSharingManager::exportJson() const {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    std::ostringstream json;
    json << "[";
    for (size_t i = 0; i < sessions_.size(); ++i) {
        if (i > 0) json << ",";
        const auto& s = sessions_[i];
        json << R"({"sessionId": ")" << esc(s.sessionId) << R"(")";
        json << R"(,"roomId": ")" << esc(s.roomId) << R"(")";
        json << R"(,"userId": ")" << esc(s.userId) << R"(")";
        json << R"(,"active": )" << (s.active ? "true" : "false");
        json << R"(,"intervalSeconds": )" << s.intervalSeconds << "}";
    }
    json << "]";
    return json.str();
}

} // namespace progressive
