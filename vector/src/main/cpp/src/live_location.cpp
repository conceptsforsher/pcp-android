#include "progressive/live_location.hpp"
#include <sstream>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <sstream>

namespace progressive {

// ====== Utility helpers ======

static std::string extractStr(const std::string& json, const std::string& key) {
    auto pp = json.find("\"" + key + "\"");
    if (pp == std::string::npos) return "";
    pp = json.find('"', pp + key.size() + 2);
    if (pp == std::string::npos) return "";
    pp++;
    size_t e = pp;
    while (e < json.size() && json[e] != '"') e++;
    return json.substr(pp, e - pp);
}

static int64_t extractInt(const std::string& json, const std::string& key) {
    auto pp = json.find("\"" + key + "\"");
    if (pp == std::string::npos) return 0;
    pp = json.find(':', pp);
    if (pp == std::string::npos) return 0;
    pp++;
    while (pp < json.size() && (json[pp] == ' ' || json[pp] == '\t')) pp++;
    int64_t v = 0;
    while (pp < json.size() && json[pp] >= '0' && json[pp] <= '9') { v=v*10+(json[pp]-'0'); pp++; }
    return v;
}

static bool extractBool(const std::string& json, const std::string& key) {
    return json.find("\"" + key + "\":true") != std::string::npos;
}

static double toRadians(double deg) { return deg * M_PI / 180.0; }
static double toDegrees(double rad) { return rad * 180.0 / M_PI; }

// Haversine distance between two coordinates (meters)
static double haversineMeters(double lat1, double lon1, double lat2, double lon2) {
    double dlat = toRadians(lat2 - lat1);
    double dlon = toRadians(lon2 - lon1);
    double a = std::sin(dlat / 2) * std::sin(dlat / 2) +
               std::cos(toRadians(lat1)) * std::cos(toRadians(lat2)) *
               std::sin(dlon / 2) * std::sin(dlon / 2);
    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));
    return 6371000.0 * c; // Earth radius in meters
}

static double bearingDegrees(double lat1, double lon1, double lat2, double lon2) {
    double dlon = toRadians(lon2 - lon1);
    double y = std::sin(dlon) * std::cos(toRadians(lat2));
    double x = std::cos(toRadians(lat1)) * std::sin(toRadians(lat2)) -
               std::sin(toRadians(lat1)) * std::cos(toRadians(lat2)) * std::cos(dlon);
    double bearing = toDegrees(std::atan2(y, x));
    return bearing < 0 ? bearing + 360.0 : bearing;
}

// ====== Geo URI Parsing (RFC 5870) ======

GeoUri parseGeoUri(const std::string& uri) {
    GeoUri result;

    // Must start with "geo:"
    if (uri.compare(0, 4, "geo:") != 0) return result;

    // Extract lat,lon
    auto coordEnd = uri.find(';', 4);
    std::string coords = (coordEnd == std::string::npos) ? uri.substr(4) : uri.substr(4, coordEnd - 4);

    auto comma = coords.find(',');
    if (comma == std::string::npos) return result;

    result.latitude = std::stod(coords.substr(0, comma));
    result.longitude = std::stod(coords.substr(comma + 1));

    // Validate ranges
    if (result.latitude < -90.0 || result.latitude > 90.0) return result;
    if (result.longitude < -180.0 || result.longitude > 180.0) return result;

    result.crs = "wgs84"; // default
    result.valid = true;

    // Parse parameters: ;crs=...;u=...;alt=...
    if (coordEnd != std::string::npos) {
        std::string params = uri.substr(coordEnd + 1);
        size_t pos = 0;
        while (pos < params.size()) {
            auto eq = params.find('=', pos);
            auto semi = params.find(';', pos);
            if (semi == std::string::npos) semi = params.size();

            if (eq != std::string::npos && eq < semi) {
                std::string key = params.substr(pos, eq - pos);
                std::string val = params.substr(eq + 1, semi - eq - 1);

                if (key == "crs") result.crs = val;
                else if (key == "u") result.uncertainty = std::stod(val);
            } else {
                // Label parameter (no value)
                result.label = params.substr(pos, semi - pos);
            }
            pos = semi + 1;
        }
    }

    return result;
}

std::string formatGeoUri(const GeoCoordinate& coord) {
    std::ostringstream os;
    os << "geo:" << coord.latitude << "," << coord.longitude;
    if (coord.accuracy > 0) os << ";u=" << coord.accuracy;
    if (!coord.label.empty()) os << ";" << coord.label;
    return os.str();
}

// ====== Beacon Info Parsing ======

BeaconInfoContent parseBeaconInfo(const std::string& contentJson) {
    BeaconInfoContent info;
    info.description = extractStr(contentJson, "description");
    info.timeoutSec = static_cast<int>(extractInt(contentJson, "timeout"));
    info.live = extractBool(contentJson, "live");
    info.timestampMs = extractInt(contentJson, "org.matrix.msc3488.ts");
    if (info.timestampMs == 0) info.timestampMs = extractInt(contentJson, "m.ts");
    info.assetType = extractStr(contentJson, "org.matrix.msc3488.asset");
    if (info.assetType.empty()) info.assetType = extractStr(contentJson, "m.asset_type");
    info.valid = info.timeoutSec > 0;
    return info;
}

std::string buildBeaconInfoContent(const BeaconInfoContent& info) {
    std::ostringstream os;
    os << R"({"description":")" << info.description << R"(")";
    os << R"(,"timeout":)" << info.timeoutSec;
    os << R"(,"live":)" << (info.live ? "true" : "false");
    if (info.timestampMs > 0) os << R"(,"org.matrix.msc3488.ts":)" << info.timestampMs;
    if (!info.assetType.empty()) os << R"(,"org.matrix.msc3488.asset":")" << info.assetType << R"(")";
    os << "}";
    return os.str();
}

// ====== Beacon Location Parsing ======

BeaconLocationContent parseBeaconLocation(const std::string& contentJson) {
    BeaconLocationContent loc;
    auto uriStr = extractStr(contentJson, "org.matrix.msc3488.location");
    if (uriStr.empty()) uriStr = extractStr(contentJson, "m.location");
    if (uriStr.empty()) {
        // Try parsing direct geo: URI from the full JSON
        auto geoPos = contentJson.find("geo:");
        if (geoPos != std::string::npos) {
            auto end = contentJson.find('"', geoPos);
            if (end != std::string::npos) uriStr = contentJson.substr(geoPos, end - geoPos);
        }
    }
    loc.geoUri = uriStr;
    loc.timestampMs = extractInt(contentJson, "org.matrix.msc3488.ts");
    if (loc.timestampMs == 0) loc.timestampMs = extractInt(contentJson, "m.ts");
    loc.unstable = contentJson.find("org.matrix.msc3488") != std::string::npos;
    loc.valid = !uriStr.empty();

    // Parse the geo URI to get coordinates
    if (loc.valid) {
        auto geo = parseGeoUri(uriStr);
        loc.valid = geo.valid;
    }

    return loc;
}

std::string buildBeaconLocationContent(const GeoCoordinate& coord, bool unstable) {
    std::ostringstream os;
    std::string prefix = unstable ? "org.matrix.msc3488" : "m";
    os << R"({")";
    os << R"(")" << prefix << R"(.location":")" << formatGeoUri(coord) << R"(")";
    if (coord.timestampMs > 0) os << R"(,")" << prefix << R"(.ts":)" << coord.timestampMs;
    os << "}";
    return os.str();
}

// ====== Static Map Tiles ======

std::string buildStaticMapUrl(const GeoCoordinate& coord, const MapTileConfig& config) {
    std::ostringstream url;

    switch (config.provider) {
        case MapProvider::OPENSTREETMAP:
            url << "https://staticmap.openstreetmap.de/staticmap.php?"
                << "center=" << coord.latitude << "," << coord.longitude
                << "&zoom=" << config.zoom
                << "&size=" << config.width << "x" << config.height
                << "&maptype=mapnik";
            if (config.showMarker) {
                url << "&markers=" << coord.latitude << "," << coord.longitude
                    << ",red-pushpin";
            }
            break;

        case MapProvider::GOOGLE_MAPS:
            url << "https://maps.googleapis.com/maps/api/staticmap?"
                << "center=" << coord.latitude << "," << coord.longitude
                << "&zoom=" << config.zoom
                << "&size=" << config.width << "x" << config.height
                << "&maptype=" << (config.style == "satellite" ? "satellite" : "roadmap");
            if (config.showMarker) {
                url << "&markers=color:" << config.markerColor
                    << "%7C" << coord.latitude << "," << coord.longitude;
            }
            if (!config.apiKey.empty()) url << "&key=" << config.apiKey;
            break;

        case MapProvider::APPLE_MAPS:
            url << "https://maps.apple.com/mapssnapshot?"
                << "center=" << coord.latitude << "," << coord.longitude
                << "&size=" << config.width << "x" << config.height;
            break;
    }

    return url.str();
}

std::string buildBoundingBoxMapUrl(const std::vector<GeoCoordinate>& coords, const MapTileConfig& config) {
    if (coords.empty()) return "";

    // Compute bounding box
    double minLat = 90, maxLat = -90, minLon = 180, maxLon = -180;
    for (const auto& c : coords) {
        minLat = std::min(minLat, c.latitude);
        maxLat = std::max(maxLat, c.latitude);
        minLon = std::min(minLon, c.longitude);
        maxLon = std::max(maxLon, c.longitude);
    }

    // Center is the midpoint
    GeoCoordinate center;
    center.latitude = (minLat + maxLat) / 2.0;
    center.longitude = (minLon + maxLon) / 2.0;

    std::ostringstream url;
    switch (config.provider) {
        case MapProvider::OPENSTREETMAP:
            url << "https://staticmap.openstreetmap.de/staticmap.php?"
                << "center=" << center.latitude << "," << center.longitude
                << "&zoom=" << config.zoom
                << "&size=" << config.width << "x" << config.height
                << "&maptype=mapnik";
            for (size_t i = 0; i < coords.size() && i < 10; i++) {
                url << "&markers=" << coords[i].latitude << "," << coords[i].longitude << ",red-pushpin";
            }
            break;

        case MapProvider::GOOGLE_MAPS:
            url << "https://maps.googleapis.com/maps/api/staticmap?"
                << "center=" << center.latitude << "," << center.longitude
                << "&zoom=" << config.zoom
                << "&size=" << config.width << "x" << config.height;
            for (size_t i = 0; i < coords.size() && i < 10; i++) {
                url << "&markers=color:" << config.markerColor
                    << "%7C" << coords[i].latitude << "," << coords[i].longitude;
            }
            if (!config.apiKey.empty()) url << "&key=" << config.apiKey;
            break;

        default:
            break;
    }

    return url.str();
}

// ====== Proximity & Geofencing ======

bool isWithinGeofence(const GeoCoordinate& coord, const GeofenceRegion& region) {
    double dist = haversineMeters(coord.latitude, coord.longitude,
                                   region.centerLat, region.centerLon);
    return dist <= region.radiusMeters;
}

std::string nearestGeofence(const GeoCoordinate& coord,
                             const std::vector<GeofenceRegion>& regions,
                             double maxDistanceMeters) {
    std::string nearest;
    double minDist = maxDistanceMeters;

    for (const auto& r : regions) {
        double dist = haversineMeters(coord.latitude, coord.longitude,
                                       r.centerLat, r.centerLon);
        if (dist < minDist) {
            minDist = dist;
            nearest = r.name;
        }
    }

    return nearest;
}

// ====== Location Formatting ======

std::string formatLocationMessage(const GeoCoordinate& coord, const std::string& label) {
    std::ostringstream os;
    os << "Location: " << coord.latitude << "," << coord.longitude;
    if (!label.empty()) os << " (" << label << ")";
    if (coord.accuracy > 0) os << " (accuracy: " << static_cast<int>(coord.accuracy) << "m)";
    return os.str();
}

std::string formatLocationHtml(const GeoCoordinate& coord, const std::string& label,
                                const MapTileConfig& mapConfig) {
    std::ostringstream os;
    os << "<div style='location-container'>";
    if (mapConfig.width > 0) {
        os << "<img src='" << buildStaticMapUrl(coord, mapConfig)
           << "' width='" << mapConfig.width << "' height='" << mapConfig.height
           << "' style='border-radius:8px' /><br/>";
    }
    os << "<a href='" << formatGeoUri(coord) << "'>";
    os << (label.empty() ? "Open in maps" : label);
    os << "</a> — " << coord.latitude << ", " << coord.longitude;
    if (coord.accuracy > 0) os << " (±" << static_cast<int>(coord.accuracy) << "m)";
    os << "</div>";
    return os.str();
}

std::string formatLocationDescription(const GeoCoordinate& coord) {
    std::ostringstream os;
    os << coord.latitude;
    os << (coord.latitude >= 0 ? " N, " : " S, ");
    os << coord.longitude;
    os << (coord.longitude >= 0 ? " E" : " W");
    if (coord.altitude != 0.0) os << " (altitude: " << static_cast<int>(coord.altitude) << "m)";
    if (coord.accuracy > 0) os << " (accuracy: ±" << static_cast<int>(coord.accuracy) << "m)";
    return os.str();
}

// ====== Location Actions ======

LocationAction parseLocationAction(const std::string& actionJson) {
    LocationAction action;
    auto typeStr = extractStr(actionJson, "action");
    if (typeStr == "pin_drop") action.type = LocationActionType::PIN_DROP;
    else if (typeStr == "live_start") action.type = LocationActionType::LIVE_START;
    else if (typeStr == "live_stop") action.type = LocationActionType::LIVE_STOP;
    else if (typeStr == "view_on_map") action.type = LocationActionType::VIEW_ON_MAP;
    else if (typeStr == "refresh") action.type = LocationActionType::REFRESH;

    action.coord.latitude = static_cast<double>(extractInt(actionJson, "lat")) * 0.000001; // microdegrees
    if (action.coord.latitude == 0.0) {
        action.coord.latitude = std::stod(extractStr(actionJson, "latitude"));
    }
    action.coord.longitude = std::stod(extractStr(actionJson, "longitude"));
    action.sessionId = extractStr(actionJson, "session_id");
    action.roomId = extractStr(actionJson, "room_id");
    action.timestampMs = extractInt(actionJson, "timestamp");
    action.isValid = action.coord.latitude != 0.0 || !action.sessionId.empty();
    return action;
}

std::string locationActionToJson(const LocationAction& action) {
    std::ostringstream os;
    os << R"({"action":")";
    switch (action.type) {
        case LocationActionType::PIN_DROP: os << "pin_drop"; break;
        case LocationActionType::LIVE_START: os << "live_start"; break;
        case LocationActionType::LIVE_STOP: os << "live_stop"; break;
        case LocationActionType::VIEW_ON_MAP: os << "view_on_map"; break;
        case LocationActionType::REFRESH: os << "refresh"; break;
    }
    os << R"(","latitude":)" << action.coord.latitude
       << R"(,"longitude":)" << action.coord.longitude
       << R"(,"session_id":")" << action.sessionId
       << R"(","room_id":")" << action.roomId
       << R"(","timestamp":)" << action.timestampMs
       << R"(,"map_url":")" << formatGeoUri(action.coord) << R"(")";
    os << "}";
    return os.str();
}

// ====== Live Location Manager ======

LiveLocationManager::LiveLocationManager() {}

std::string LiveLocationManager::generateSessionId() const {
    auto now = static_cast<int64_t>(std::time(nullptr));
    std::ostringstream id;
    id << "live_" << now << "_" << (sessions_.size() + 1);
    return id.str();
}

bool LiveLocationManager::isValidTimeout(int timeoutSec) const {
    return timeoutSec >= 30 && timeoutSec <= 86400; // 30s to 24h
}

bool LiveLocationManager::isValidInterval(int intervalSec) const {
    return intervalSec >= 5 && intervalSec <= 3600; // 5s to 1h
}

LiveLocationSession* LiveLocationManager::findSession(const std::string& sessionId) {
    for (auto& s : sessions_) {
        if (s.sessionId == sessionId) return &s;
    }
    return nullptr;
}

const LiveLocationSession* LiveLocationManager::findSession(const std::string& sessionId) const {
    for (const auto& s : sessions_) {
        if (s.sessionId == sessionId) return &s;
    }
    return nullptr;
}

// ====== Session Management ======

std::string LiveLocationManager::startLiveSession(
    const std::string& roomId, const std::string& userId,
    const std::string& description, int timeoutSec,
    int intervalSec, bool autoStop, int autoStopMin,
    std::string& error) {

    if (!isValidTimeout(timeoutSec)) {
        error = "Invalid timeout: " + std::to_string(timeoutSec) + "s (valid: 30-86400)";
        return "";
    }
    if (!isValidInterval(intervalSec)) {
        error = "Invalid interval: " + std::to_string(intervalSec) + "s (valid: 5-3600)";
        return "";
    }

    LiveLocationSession sess;
    sess.sessionId = generateSessionId();
    sess.roomId = roomId;
    sess.userId = userId;
    sess.description = description.empty() ? (userId + "'s location") : description;
    sess.timeoutSec = timeoutSec;
    sess.intervalSec = intervalSec;
    sess.startedAtMs = static_cast<int64_t>(std::time(nullptr)) * 1000;
    sess.expiresAtMs = sess.startedAtMs + (autoStopMin * 60000LL);
    sess.autoStop = autoStop;
    sess.autoStopMin = autoStopMin;
    sess.active = true;
    sess.isLive = true;

    sessions_.push_back(sess);

    // Build beacon info content
    BeaconInfoContent beacon;
    beacon.description = sess.description;
    beacon.timeoutSec = timeoutSec;
    beacon.live = true;
    beacon.timestampMs = sess.startedAtMs;
    beacon.assetType = "m.self";
    beacon.valid = true;

    return buildBeaconInfoContent(beacon);
}

std::string LiveLocationManager::stopLiveSession(const std::string& sessionId) {
    auto* s = findSession(sessionId);
    if (!s) return "";

    s->active = false;
    s->isLive = false;

    // Return empty beacon info to mark as stopped
    return R"({"live":false,"timeout":0})";
}

bool LiveLocationManager::isSessionExpired(const std::string& sessionId) const {
    auto* s = findSession(sessionId);
    if (!s || !s->active) return true;

    auto now = static_cast<int64_t>(std::time(nullptr)) * 1000;
    // Auto-stop check
    if (s->autoStop && now >= s->expiresAtMs) return true;

    // Timeout check: no update in timeout * 2 seconds
    int64_t lastUpdate = s->lastUpdatedMs > 0 ? s->lastUpdatedMs : s->startedAtMs;
    if (now - lastUpdate > s->timeoutSec * 2000LL) return true;

    return false;
}

bool LiveLocationManager::isUpdateDue(const std::string& sessionId) const {
    auto* s = findSession(sessionId);
    if (!s || !s->active) return false;

    auto now = static_cast<int64_t>(std::time(nullptr)) * 1000;
    int64_t lastUpdate = s->lastUpdatedMs > 0 ? s->lastUpdatedMs : s->startedAtMs;
    return (now - lastUpdate) >= s->intervalSec * 1000LL;
}

std::string LiveLocationManager::updateLocation(const std::string& sessionId,
                                                 const GeoCoordinate& coord,
                                                 std::string& error) {
    auto* s = findSession(sessionId);
    if (!s) {
        error = "Session not found: " + sessionId;
        return "";
    }
    if (!s->active) {
        error = "Session is not active: " + sessionId;
        return "";
    }

    if (coord.latitude < -90 || coord.latitude > 90 ||
        coord.longitude < -180 || coord.longitude > 180) {
        error = "Invalid coordinates: " + std::to_string(coord.latitude) + "," + std::to_string(coord.longitude);
        return "";
    }

    s->lastCoord = coord;
    s->lastUpdatedMs = static_cast<int64_t>(std::time(nullptr)) * 1000;
    s->updateCount++;
    totalUpdates_++;

    // Store in history
    locationHistory_[sessionId].push_back(coord);

    // Build beacon location content
    return buildBeaconLocationContent(coord, true);
}

std::vector<LiveLocationSession> LiveLocationManager::getActiveSessions(const std::string& userId) const {
    std::vector<LiveLocationSession> result;
    auto now = static_cast<int64_t>(std::time(nullptr)) * 1000;
    for (const auto& s : sessions_) {
        if (s.userId == userId && s.active && s.expiresAtMs > now) {
            result.push_back(s);
        }
    }
    return result;
}

std::vector<LiveLocationSession> LiveLocationManager::getRoomSessions(const std::string& roomId) const {
    std::vector<LiveLocationSession> result;
    for (const auto& s : sessions_) {
        if (s.roomId == roomId && s.active) {
            result.push_back(s);
        }
    }
    return result;
}

bool LiveLocationManager::getSession(const std::string& sessionId, LiveLocationSession& out) const {
    auto* s = findSession(sessionId);
    if (!s) return false;
    out = *s;
    return true;
}

// ====== Location Tracking ======

void LiveLocationManager::addBeaconUpdate(const std::string& sessionId, const GeoCoordinate& coord) {
    std::string error;
    updateLocation(sessionId, coord, error);
}

GeoCoordinate LiveLocationManager::getLatestLocation(const std::string& sessionId) const {
    auto* s = findSession(sessionId);
    if (!s) return {};
    return s->lastCoord;
}

std::vector<GeoCoordinate> LiveLocationManager::getLocationHistory(const std::string& sessionId) const {
    auto it = locationHistory_.find(sessionId);
    if (it != locationHistory_.end()) return it->second;
    return {};
}

// ====== Clustering ======

std::vector<LocationCluster> LiveLocationManager::clusterLocations(
    const std::vector<GeoCoordinate>& coords, double clusterRadiusMeters) const {

    std::vector<LocationCluster> clusters;
    if (coords.empty()) return clusters;

    std::vector<bool> clustered(coords.size(), false);

    for (size_t i = 0; i < coords.size(); i++) {
        if (clustered[i]) continue;

        LocationCluster cluster;
        cluster.members.push_back(coords[i]);
        clustered[i] = true;

        for (size_t j = i + 1; j < coords.size(); j++) {
            if (clustered[j]) continue;
            double dist = haversineMeters(coords[i].latitude, coords[i].longitude,
                                           coords[j].latitude, coords[j].longitude);
            if (dist <= clusterRadiusMeters) {
                cluster.members.push_back(coords[j]);
                clustered[j] = true;
            }
        }

        cluster.memberCount = static_cast<int>(cluster.members.size());

        // Compute center
        double sumLat = 0, sumLon = 0;
        for (const auto& m : cluster.members) {
            sumLat += m.latitude;
            sumLon += m.longitude;
        }
        cluster.centerLat = sumLat / cluster.memberCount;
        cluster.centerLon = sumLon / cluster.memberCount;

        // Compute radius
        double maxDist = 0;
        for (const auto& m : cluster.members) {
            double dist = haversineMeters(cluster.centerLat, cluster.centerLon,
                                           m.latitude, m.longitude);
            maxDist = std::max(maxDist, dist);
        }
        cluster.radiusMeters = maxDist;

        if (cluster.memberCount > 1) {
            cluster.label = std::to_string(cluster.memberCount) + " participants";
        }

        clusters.push_back(cluster);
    }

    return clusters;
}

// ====== Proximity ======

bool LiveLocationManager::areSessionsNearby(const std::string& sessionA,
                                              const std::string& sessionB,
                                              double maxDistanceMeters) const {
    auto* sa = findSession(sessionA);
    auto* sb = findSession(sessionB);
    if (!sa || !sb || !sa->active || !sb->active) return false;

    double dist = haversineMeters(sa->lastCoord.latitude, sa->lastCoord.longitude,
                                   sb->lastCoord.latitude, sb->lastCoord.longitude);
    return dist <= maxDistanceMeters;
}

std::string LiveLocationManager::findNearestSession(const GeoCoordinate& coord,
                                                      double maxDistanceMeters) const {
    std::string nearest;
    double minDist = maxDistanceMeters;

    for (const auto& s : sessions_) {
        if (!s.active) continue;
        double dist = haversineMeters(coord.latitude, coord.longitude,
                                       s.lastCoord.latitude, s.lastCoord.longitude);
        if (dist < minDist) {
            minDist = dist;
            nearest = s.sessionId;
        }
    }

    return nearest;
}

// ====== Formatting ======

std::string LiveLocationManager::sessionsToJson() const {
    std::ostringstream os;
    os << "[";
    bool first = true;
    for (const auto& s : sessions_) {
        if (!first) os << ","; first = false;
        os << R"({"session_id":")" << s.sessionId
           << R"(","room_id":")" << s.roomId
           << R"(","user_id":")" << s.userId
           << R"(","description":")" << s.description
           << R"(","active":)" << (s.active ? "true" : "false")
           << R"(,"is_live":)" << (s.isLive ? "true" : "false")
           << R"(,"latitude":)" << s.lastCoord.latitude
           << R"(,"longitude":)" << s.lastCoord.longitude
           << R"(,"updates":)" << s.updateCount
           << R"(,"expires_at":)" << s.expiresAtMs
           << R"(,"geo_uri":")" << formatGeoUri(s.lastCoord) << R"(")";
        os << "}";
    }
    os << "]";
    return os.str();
}

std::string LiveLocationManager::historyToJson(const std::string& sessionId) const {
    auto it = locationHistory_.find(sessionId);
    std::ostringstream os;
    os << "[";
    if (it != locationHistory_.end()) {
        bool first = true;
        for (const auto& coord : it->second) {
            if (!first) os << ","; first = false;
            os << R"({"lat":)" << coord.latitude
               << R"(,"lon":)" << coord.longitude
               << R"(,"ts":)" << coord.timestampMs
               << R"(,"acc":)" << coord.accuracy
               << "}";
        }
    }
    os << "]";
    return os.str();
}

std::string LiveLocationManager::buildRoomMapUrl(const std::string& roomId,
                                                   const MapTileConfig& config) const {
    std::vector<GeoCoordinate> coords;
    for (const auto& s : sessions_) {
        if (s.roomId == roomId && s.active) {
            coords.push_back(s.lastCoord);
        }
    }
    if (coords.empty()) return "";

    if (coords.size() == 1) {
        return buildStaticMapUrl(coords[0], config);
    }

    return buildBoundingBoxMapUrl(coords, config);
}

int LiveLocationManager::activeSessionCount() const {
    int count = 0;
    for (const auto& s : sessions_) if (s.active) count++;
    return count;
}

int LiveLocationManager::totalUpdateCount() const {
    return totalUpdates_;
}

// ================================================================
// Live Location Share Functions (standalone, global state)
// ================================================================

namespace {

std::vector<LiveLocationShare> g_liveLocationShares;

LiveLocationShare* findShare(const std::string& shareId) {
    for (auto& s : g_liveLocationShares) {
        if (s.shareId == shareId) return &s;
    }
    return nullptr;
}

const LiveLocationShare* findShare(const std::string& shareId, bool) {
    for (const auto& s : g_liveLocationShares) {
        if (s.shareId == shareId) return &s;
    }
    return nullptr;
}

} // anonymous namespace

std::string startLiveLocationShare(const std::string& roomId, const std::string& userId,
                                    const std::string& description, int timeoutMs,
                                    std::string& error) {
    // Original Kotlin: stateService.liveLocation().startLiveLocationShare()
    if (timeoutMs < 30000 || timeoutMs > 86400000) {
        error = "Invalid timeout: " + std::to_string(timeoutMs) + "ms (valid: 30000-86400000)";
        return "";
    }

    LiveLocationShare share;
    auto now = static_cast<int64_t>(std::time(nullptr)) * 1000;
    share.shareId = "lls_" + std::to_string(now) + "_" + std::to_string(g_liveLocationShares.size() + 1);
    share.roomId = roomId;
    share.userId = userId;
    share.description = description.empty() ? (userId + "'s location") : description;
    share.timeoutMs = timeoutMs;
    share.startedAt = now;
    share.expiresAt = computeLiveLocationShareExpiry(now, timeoutMs);
    share.state = LiveLocationShareState::ACTIVE;
    share.lastUpdatedAt = now;

    g_liveLocationShares.push_back(share);

    return buildLiveLocationShareStartEvent(share);
}

std::string stopLiveLocationShare(const std::string& shareId) {
    // Original Kotlin: stateService.liveLocation().stopLiveLocationShare()
    auto* s = findShare(shareId);
    if (!s) return "";
    s->state = LiveLocationShareState::STOPPED;
    return buildLiveLocationShareStopEvent(shareId);
}

bool pauseLiveLocationShare(const std::string& shareId) {
    auto* s = findShare(shareId);
    if (!s || s->state != LiveLocationShareState::ACTIVE) return false;
    s->state = LiveLocationShareState::PAUSED;
    return true;
}

bool resumeLiveLocationShare(const std::string& shareId) {
    auto* s = findShare(shareId);
    if (!s || s->state != LiveLocationShareState::PAUSED) return false;
    s->state = LiveLocationShareState::ACTIVE;
    return true;
}

std::vector<LiveLocationShare> getActiveLiveLocationShares() {
    std::vector<LiveLocationShare> result;
    auto now = static_cast<int64_t>(std::time(nullptr)) * 1000;
    for (const auto& s : g_liveLocationShares) {
        if (s.state == LiveLocationShareState::ACTIVE && s.expiresAt > now) {
            result.push_back(s);
        }
    }
    return result;
}

bool getLiveLocationShareById(const std::string& shareId, LiveLocationShare& out) {
    const auto* s = findShare(shareId, true);
    if (!s) return false;
    out = *s;
    return true;
}

std::vector<LiveLocationShare> getLiveLocationSharesForRoom(const std::string& roomId) {
    std::vector<LiveLocationShare> result;
    for (const auto& s : g_liveLocationShares) {
        if (s.roomId == roomId) {
            result.push_back(s);
        }
    }
    return result;
}

std::vector<LiveLocationShare> getActiveLiveLocationSharesForUser(const std::string& userId) {
    std::vector<LiveLocationShare> result;
    auto now = static_cast<int64_t>(std::time(nullptr)) * 1000;
    for (const auto& s : g_liveLocationShares) {
        if (s.userId == userId && s.state == LiveLocationShareState::ACTIVE && s.expiresAt > now) {
            result.push_back(s);
        }
    }
    return result;
}

bool isLiveLocationShareExpired(const LiveLocationShare& share) {
    // Original Kotlin: checks if the share has passed its expiry time
    if (share.state == LiveLocationShareState::STOPPED ||
        share.state == LiveLocationShareState::DELETED ||
        share.state == LiveLocationShareState::EXPIRED) {
        return true;
    }
    auto now = static_cast<int64_t>(std::time(nullptr)) * 1000;
    return now >= share.expiresAt;
}

int64_t computeLiveLocationShareExpiry(int64_t startedAt, int timeoutMs) {
    // Original Kotlin: expiry = startupTimestamp + timeout
    return startedAt + static_cast<int64_t>(timeoutMs);
}

std::string buildLiveLocationShareStartEvent(const LiveLocationShare& share) {
    // Original Kotlin: builds a m.beacon_info start event
    // {"description":"...","timeout":...,"live":true,"org.matrix.msc3488.ts":...}
    std::ostringstream os;
    os << R"({"description":")" << share.description << R"(")";
    os << R"(,"timeout":)" << (share.timeoutMs / 1000);
    os << R"(,"live":true)";
    os << R"(,"org.matrix.msc3488.ts":)" << share.startedAt;
    os << R"(,"org.matrix.msc3488.asset":"m.self")";
    os << "}";
    return os.str();
}

std::string buildLiveLocationShareStopEvent(const std::string& shareId) {
    // Original Kotlin: builds an empty beacon info to mark as stopped
    std::ostringstream os;
    os << R"({"live":false,"timeout":0,"share_id":")" << shareId << R"("})";
    return os.str();
}

LiveLocationShare parseLiveLocationShareEvent(const std::string& eventJson) {
    // Original Kotlin: parses a m.beacon_info event into LiveLocationShare
    LiveLocationShare share;
    share.description = extractStr(eventJson, "description");
    share.timeoutMs = static_cast<int>(extractInt(eventJson, "timeout")) * 1000;
    share.startedAt = extractInt(eventJson, "org.matrix.msc3488.ts");
    if (share.startedAt == 0) share.startedAt = extractInt(eventJson, "m.ts");
    share.beaconInfoId = extractStr(eventJson, "event_id");
    share.roomId = extractStr(eventJson, "room_id");
    share.userId = extractStr(eventJson, "sender");
    share.deviceId = extractStr(eventJson, "device_id");
    share.shareId = extractStr(eventJson, "share_id");
    bool live = extractBool(eventJson, "live");
    bool stopped = eventJson.find("\"live\":false") != std::string::npos && extractInt(eventJson, "timeout") == 0;
    share.state = stopped ? LiveLocationShareState::STOPPED :
                   live ? LiveLocationShareState::ACTIVE : LiveLocationShareState::EXPIRED;
    auto now = static_cast<int64_t>(std::time(nullptr)) * 1000;
    share.expiresAt = share.startedAt + share.timeoutMs;
    share.lastUpdatedAt = now;
    return share;
}

LiveLocationAggregationResult aggregateLiveLocationData(const std::string& shareId,
                                                         const std::vector<LiveLocationBeaconData>& beacons) {
    // Original Kotlin: aggregates beacon data for a share
    LiveLocationAggregationResult result;
    result.shareId = shareId;
    result.beacons = beacons;
    result.beaconCount = static_cast<int>(beacons.size());

    if (!beacons.empty()) {
        result.firstTimestamp = beacons.front().timestamp;
        result.lastTimestamp = beacons.back().timestamp;

        // Find latest active beacon
        for (auto it = beacons.rbegin(); it != beacons.rend(); ++it) {
            if (it->isLive) {
                auto geo = parseGeoUri(it->geoUri);
                if (geo.valid) {
                    result.latestCoord.latitude = geo.latitude;
                    result.latestCoord.longitude = geo.longitude;
                    result.latestCoord.accuracy = it->accuracy;
                    result.isActive = true;
                    break;
                }
            }
        }
    }

    return result;
}

std::string formatLiveLocationShareDuration(int64_t startedAt, int64_t nowMs) {
    // Original Kotlin: formats "Live for 5m" or "Live for 2h 30m"
    if (startedAt <= 0 || nowMs <= 0) return "";

    int64_t diffMs = nowMs - startedAt;
    if (diffMs < 0) return "just started";

    int64_t seconds = diffMs / 1000;
    int64_t minutes = seconds / 60;
    int64_t hours = minutes / 60;

    std::ostringstream out;
    if (hours > 0) {
        out << hours << "h ";
    }
    out << (minutes % 60) << "m";
    return out.str();
}

} // namespace progressive
