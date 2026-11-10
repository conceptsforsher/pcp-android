#pragma once
#include <string>
#include <cstdint>

namespace pcp {

struct LocationMessageDisplay {
    std::string geoUri;
    double latitude = 0.0;
    double longitude = 0.0;
    std::string description;
    std::string mapUrl;         // static map image URL
    std::string formattedText;  // "📍 Location" or "📍 description"
    bool isLive = false;        // live location sharing
    int64_t liveTimeoutMs = 0;
};

// Parse location message for display
LocationMessageDisplay parseLocationForDisplay(const std::string& json,
                                                 const std::string& mapProvider = "osm");

// Format location message text
std::string formatLocationText(const LocationMessageDisplay& loc);

// Build static map URL
std::string buildStaticMapTileUrl(double lat, double lon, int zoom, int width, int height);

// Check if location is stale (> 1 hour)
bool isLocationStale(int64_t timestampMs);

} // namespace pcp
