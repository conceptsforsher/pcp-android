#pragma once

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <cstdint>

namespace progressive {

// ================================================================
// Live Location Sharing — full port of Element Android's feature
//
// Based on:
//   Element Android: im.vector.app.features.location.*
//   MSC3488: Extensible Events — Location events
//   MSC3489: Live location sharing via beacons
//   RFC 5870: geo: URI scheme
//
// Covers:
//   1. Beacon events (m.beacon_info, m.beacon, m.beacon_info.*)
//   2. Live session lifecycle (start/stop/auto-stop/is-due)
//   3. Geo URI full parser (RFC 5870: geo:lat,lon;crs=...;u=...)
//   4. Location aggregation and cluster analysis
//   5. Static map tile URL generation (OSM/Google)
//   6. Proximity detection and geofencing
//   7. Location sharing permissions and state management
// ================================================================

// ---- GeoCoordinate (extended) ----

struct GeoCoordinate {
    double latitude = 0.0;
    double longitude = 0.0;
    double altitude = 0.0;       // meters above sea level
    double accuracy = 0.0;       // meters
    double bearing = 0.0;        // degrees from true north
    double speed = 0.0;          // meters/second
    int64_t timestampMs = 0;
    std::string label;           // display name (e.g. "Big Ben")
    bool valid = false;
};

// ---- Geo URI (RFC 5870) ----

struct GeoUri {
    double latitude = 0.0;
    double longitude = 0.0;
    double altitude = 0.0;
    double uncertainty = 0.0;    // u= parameter
    std::string crs;             // coordinate reference system (default: wgs84)
    std::string label;           // human-readable label
    bool valid = false;
};

// Parse geo: URI per RFC 5870.
// "geo:48.858093,2.294694;crs=wgs84;u=10"
// "geo:48.858093,2.294694"
GeoUri parseGeoUri(const std::string& uri);

// Format coordinates as geo: URI.
std::string formatGeoUri(const GeoCoordinate& coord);

// ---- Beacon Event Types (MSC3488/MSC3489) ----

enum class BeaconEventType {
    BEACON_INFO = 0,         // m.beacon_info — session metadata (stable)
    BEACON = 1,              // m.beacon — actual location data (unstable)
    BEACON_INFO_STABLE = 2,  // m.beacon_info.* — versioned stable type
};

// ---- Beacon Info Content ----

struct BeaconInfoContent {
    BeaconEventType eventType = BeaconEventType::BEACON_INFO;
    std::string description;          // Human-readable label (e.g. "Alice's location")
    int timeoutSec = 0;               // Seconds after which beacon data expires
    bool live = false;                // true = live location sharing
    int64_t timestampMs = 0;          // When the beacon was created
    std::string assetType;            // "m.self" = user's own location
    bool valid = false;
};

// Parse m.beacon_info content.
BeaconInfoContent parseBeaconInfo(const std::string& contentJson);

// Build m.beacon_info content JSON.
std::string buildBeaconInfoContent(const BeaconInfoContent& info);

// ---- Beacon Location Content ----

struct BeaconLocationContent {
    std::string geoUri;               // "geo:48.858093,2.294694;u=10"
    int64_t timestampMs = 0;          // When the location was recorded
    bool unstable = true;             // true = uses unstable prefix (msc3488)
    bool valid = false;
};

// Parse m.beacon content (location update).
BeaconLocationContent parseBeaconLocation(const std::string& contentJson);

// Build m.beacon content JSON.
std::string buildBeaconLocationContent(const GeoCoordinate& coord, bool unstable);

// ---- Live Location Session ----

struct LiveLocationSession {
    std::string sessionId;             // Unique session ID
    std::string roomId;
    std::string userId;                // The sharer
    std::string description;           // "Alice's live location"
    int timeoutSec = 300;              // Beacon expiry in seconds (5 min default)
    int intervalSec = 30;              // How often to send updates
    int64_t startedAtMs = 0;
    int64_t expiresAtMs = 0;           // When the session auto-stops
    int64_t lastUpdatedMs = 0;
    GeoCoordinate lastCoord;
    bool active = false;
    bool isLive = false;               // true = live, false = static pin
    bool autoStop = false;
    int autoStopMin = 60;
    int updateCount = 0;               // Number of location updates sent
    std::string assetType = "m.self";  // "m.self" or custom
};

// ---- Location Cluster (for map display) ----

struct LocationCluster {
    double centerLat = 0.0;
    double centerLon = 0.0;
    int memberCount = 0;
    std::vector<GeoCoordinate> members;  // Individual locations in cluster
    double radiusMeters = 0.0;           // Cluster radius
    std::string label;                   // e.g. "3 participants"
};

// ---- Static Map Tile ----

enum class MapProvider {
    OPENSTREETMAP = 0,
    GOOGLE_MAPS = 1,
    APPLE_MAPS = 2,
};

struct MapTileConfig {
    MapProvider provider = MapProvider::OPENSTREETMAP;
    int width = 320;
    int height = 240;
    int zoom = 14;
    bool showMarker = true;
    std::string markerColor = "red";
    std::string apiKey;                 // For Google Maps
    std::string style;                  // Map style: "light", "dark", "satellite"
};

// Build a static map tile URL for a given coordinate.
std::string buildStaticMapUrl(const GeoCoordinate& coord, const MapTileConfig& config);

// Build a static map tile URL for a bounding box (multiple coordinates).
std::string buildBoundingBoxMapUrl(const std::vector<GeoCoordinate>& coords, const MapTileConfig& config);

// ---- Proximity & Geofencing ----

struct GeofenceRegion {
    double centerLat = 0.0;
    double centerLon = 0.0;
    double radiusMeters = 500.0;        // Default: 500m radius
    std::string name;                   // e.g. "Work", "Home"
};

// Check if a coordinate is within a geofence region.
bool isWithinGeofence(const GeoCoordinate& coord, const GeofenceRegion& region);

// Find the nearest geofence region (by name) for a coordinate.
// Returns empty string if no region is close enough.
std::string nearestGeofence(const GeoCoordinate& coord,
                             const std::vector<GeofenceRegion>& regions,
                             double maxDistanceMeters = 500.0);

// ---- Location Formatting ----

// Format as Matrix message body (plain text).
std::string formatLocationMessage(const GeoCoordinate& coord, const std::string& label = "");

// Format as HTML for rich Matrix messages.
std::string formatLocationHtml(const GeoCoordinate& coord, const std::string& label = "",
                                const MapTileConfig& mapConfig = {});

// Format as human-readable address-like string.
// "48.858093 N, 2.294694 E (altitude: 324m, accuracy: 10m)"
std::string formatLocationDescription(const GeoCoordinate& coord);

// ---- Location Actions ----

enum class LocationActionType {
    PIN_DROP = 0,            // Static pin drop
    LIVE_START = 1,          // Start live sharing
    LIVE_STOP = 2,           // Stop live sharing
    VIEW_ON_MAP = 3,         // Open in map app
    REFRESH = 4,             // Request fresh location
};

struct LocationAction {
    LocationActionType type = LocationActionType::PIN_DROP;
    GeoCoordinate coord;
    std::string sessionId;
    std::string roomId;
    int64_t timestampMs = 0;
    bool isValid = false;
};

// Parse a location action from a user command/message.
LocationAction parseLocationAction(const std::string& actionJson);

// Format a location action as JSON.
std::string locationActionToJson(const LocationAction& action);

// ---- Live Location Share State ----
// Original Kotlin: LiveLocationShareState enum

enum class LiveLocationShareState {
    ACTIVE = 0,
    PAUSED = 1,
    EXPIRED = 2,
    STOPPED = 3,
    DELETED = 4,
};

// ---- Live Location Share ----
// Original Kotlin: LiveLocationShare data class

struct LiveLocationShare {
    std::string shareId;
    std::string roomId;
    std::string userId;
    std::string deviceId;
    std::string beaconInfoId;
    LiveLocationShareState state = LiveLocationShareState::ACTIVE;
    int64_t startedAt = 0;
    int64_t expiresAt = 0;
    int64_t lastUpdatedAt = 0;
    std::string description;
    int timeoutMs = 0;
};

// ---- Live Location Participants ----
// Original Kotlin: LiveLocationParticipants data class

struct LiveLocationParticipants {
    std::string shareId;
    std::vector<std::string> participants;
    int totalCount = 0;
};

// ---- Live Location Beacon Data ----
// Original Kotlin: LiveLocationBeaconData data class

struct LiveLocationBeaconData {
    std::string shareId;
    std::string geoUri;
    int64_t timestamp = 0;
    bool isLive = false;
    double accuracy = 0.0;
};

// ---- Live Location Aggregation Result ----
// Original Kotlin: LiveLocationAggregationResult data class

struct LiveLocationAggregationResult {
    std::string shareId;
    std::vector<LiveLocationBeaconData> beacons;
    GeoCoordinate latestCoord;
    int beaconCount = 0;
    int64_t firstTimestamp = 0;
    int64_t lastTimestamp = 0;
    bool isActive = false;
};

// ---- Live Location Share Functions ----

// Start a live location share. Returns m.beacon_info event JSON.
// Original Kotlin: stateService.liveLocation().startLiveLocationShare()
std::string startLiveLocationShare(const std::string& roomId, const std::string& userId,
                                    const std::string& description, int timeoutMs,
                                    std::string& error);

// Stop a live location share.
// Original Kotlin: stateService.liveLocation().stopLiveLocationShare()
std::string stopLiveLocationShare(const std::string& shareId);

// Pause a live location share.
bool pauseLiveLocationShare(const std::string& shareId);

// Resume a paused live location share.
bool resumeLiveLocationShare(const std::string& shareId);

// Get all active live location shares.
std::vector<LiveLocationShare> getActiveLiveLocationShares();

// Get a live location share by ID.
bool getLiveLocationShareById(const std::string& shareId, LiveLocationShare& out);

// Get all live location shares for a room.
std::vector<LiveLocationShare> getLiveLocationSharesForRoom(const std::string& roomId);

// Get active live location shares for a specific user.
std::vector<LiveLocationShare> getActiveLiveLocationSharesForUser(const std::string& userId);

// Check if a live location share has expired.
bool isLiveLocationShareExpired(const LiveLocationShare& share);

// Compute the expiry timestamp for a live location share.
int64_t computeLiveLocationShareExpiry(int64_t startedAt, int timeoutMs);

// Build the start event JSON for a live location share (m.beacon_info).
std::string buildLiveLocationShareStartEvent(const LiveLocationShare& share);

// Build the stop event JSON for a live location share.
std::string buildLiveLocationShareStopEvent(const std::string& shareId);

// Parse a live location share event from JSON.
LiveLocationShare parseLiveLocationShareEvent(const std::string& eventJson);

// Aggregate live location beacon data.
LiveLocationAggregationResult aggregateLiveLocationData(const std::string& shareId,
                                                         const std::vector<LiveLocationBeaconData>& beacons);

// Format the duration of a live location share as a human-readable string.
std::string formatLiveLocationShareDuration(int64_t startedAt, int64_t nowMs);

// ---- Live Location Manager ----

class LiveLocationManager {
public:
    LiveLocationManager();

    // ====== Session Management ======

    // Start a new live location sharing session.
    // Returns the session JSON for the m.beacon_info event.
    std::string startLiveSession(const std::string& roomId, const std::string& userId,
                                  const std::string& description, int timeoutSec,
                                  int intervalSec, bool autoStop, int autoStopMin,
                                  std::string& error);

    // Stop a live session. Returns the empty beacon content (marking as stopped).
    std::string stopLiveSession(const std::string& sessionId);

    // Check if a session has expired and should be stopped.
    bool isSessionExpired(const std::string& sessionId) const;

    // Check if a session is due for a location update.
    bool isUpdateDue(const std::string& sessionId) const;

    // Record a location update for a session.
    std::string updateLocation(const std::string& sessionId, const GeoCoordinate& coord,
                                std::string& error);

    // Get all active sessions for a user.
    std::vector<LiveLocationSession> getActiveSessions(const std::string& userId) const;

    // Get all active sessions in a room.
    std::vector<LiveLocationSession> getRoomSessions(const std::string& roomId) const;

    // Get a session by ID.
    bool getSession(const std::string& sessionId, LiveLocationSession& out) const;

    // ====== Location Tracking ======

    // Add a raw location update from a beacon event.
    void addBeaconUpdate(const std::string& sessionId, const GeoCoordinate& coord);

    // Get the latest location for a session.
    GeoCoordinate getLatestLocation(const std::string& sessionId) const;

    // Get location history for a session (all recorded updates).
    std::vector<GeoCoordinate> getLocationHistory(const std::string& sessionId) const;

    // ====== Clustering ======

    // Cluster a set of coordinates into groups for map display.
    // Returns clusters centered around nearby groups.
    std::vector<LocationCluster> clusterLocations(const std::vector<GeoCoordinate>& coords,
                                                    double clusterRadiusMeters = 100.0) const;

    // ====== Proximity ======

    // Check if two sessions are near each other.
    bool areSessionsNearby(const std::string& sessionA, const std::string& sessionB,
                           double maxDistanceMeters = 200.0) const;

    // Find the nearest session to a coordinate.
    std::string findNearestSession(const GeoCoordinate& coord,
                                    double maxDistanceMeters = 500.0) const;

    // ====== Formatting ======

    // Format active sessions as JSON for the UI.
    std::string sessionsToJson() const;

    // Format location history as JSON.
    std::string historyToJson(const std::string& sessionId) const;

    // Build a static map URL for all live locations in a room.
    std::string buildRoomMapUrl(const std::string& roomId, const MapTileConfig& config) const;

    // ====== Stats ======

    int activeSessionCount() const;
    int totalUpdateCount() const;

private:
    std::vector<LiveLocationSession> sessions_;
    std::unordered_map<std::string, std::vector<GeoCoordinate>> locationHistory_; // sessionId → history
    int totalUpdates_ = 0;

    LiveLocationSession* findSession(const std::string& sessionId);
    const LiveLocationSession* findSession(const std::string& sessionId) const;

    std::string generateSessionId() const;
    bool isValidTimeout(int timeoutSec) const;
    bool isValidInterval(int intervalSec) const;
};

} // namespace progressive
