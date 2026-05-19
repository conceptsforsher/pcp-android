#ifndef PROGRESSIVE_LOCATION_SHARING_HPP
#define PROGRESSIVE_LOCATION_SHARING_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

struct GeoCoord {
    double latitude = 0.0;
    double longitude = 0.0;
    double accuracy = 0.0;    // meters
    double altitude = 0.0;
    int64_t timestampMs = 0;
    bool valid = false;
};

struct LocationSession {
    std::string sessionId;
    std::string roomId;         // target room
    std::string userId;         // the sharer
    std::vector<std::string> participants;  // additional sharers
    int intervalSeconds = 60;   // how often to send
    int64_t startedAtMs = 0;
    int64_t lastSentMs = 0;
    bool active = false;
    bool autoStop = false;      // stop after N minutes
    int autoStopMinutes = 60;
    GeoCoord lastCoord;
};

class LocationSharingManager {
public:
    // Start a new location sharing session.
    std::string startSession(const LocationSession& session);

    // Stop a session.
    void stopSession(const std::string& sessionId);

    // Get all active sessions for a user.
    std::vector<LocationSession> getActiveSessions(const std::string& userId) const;

    // Check if a session is due for an update.
    bool isDue(const std::string& sessionId) const;

    // Update the last sent timestamp.
    void markSent(const std::string& sessionId, const GeoCoord& coord);

    // Check if auto-stop has been reached.
    std::vector<std::string> checkAutoStop();

    // Get session by ID.
    const LocationSession* getSession(const std::string& sessionId) const;

    void clear();
    size_t sessionCount() const { return sessions_.size(); }

    // ---- Coordinate Utilities ----

    // Validate coordinates.
    static bool isValidCoord(const GeoCoord& coord);

    // Format coordinates as a Matrix message body (geo: URI + text).
    static std::string formatLocationMessage(const GeoCoord& coord, const std::string& label = "");

    // Format as geo: URI for map apps.
    static std::string formatGeoUri(const GeoCoord& coord);

    // Format coordinates as GeoJSON for rich rendering.
    static std::string formatGeoJson(const GeoCoord& coord);

    // Parse coordinates from a Matrix message body.
    static GeoCoord parseFromMessage(const std::string& body);

    // Compute distance between two coordinates (Haversine, in meters).
    static double distanceMeters(const GeoCoord& a, const GeoCoord& b);

    // Compute bearing from coordinate a to b (degrees).
    static double bearingDegrees(const GeoCoord& a, const GeoCoord& b);

    // Check if a coordinate has moved significantly (>meters threshold).
    static bool hasMoved(const GeoCoord& old, const GeoCoord& next, double thresholdMeters = 10.0);

    // Export active sessions as JSON.
    std::string exportJson() const;

private:
    std::vector<LocationSession> sessions_;
    static int nextId_;
    std::string generateId();
};

} // namespace progressive

#endif // PROGRESSIVE_LOCATION_SHARING_HPP
