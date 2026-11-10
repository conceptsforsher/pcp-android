#include "pcp/message_location_formatter.hpp"
#include <sstream>
#include <chrono>

namespace pcp {

static std::string extractField(const std::string& json, const std::string& key) {
    auto p = json.find("\"" + key + "\":\"");
    if (p == std::string::npos) return "";
    p += key.size() + 4;
    auto e = json.find('"', p);
    return e != std::string::npos ? json.substr(p, e - p) : "";
}

LocationMessageDisplay parseLocationForDisplay(const std::string& json, const std::string& provider) {
    LocationMessageDisplay l;
    l.geoUri = extractField(json, "geo_uri");
    l.description = extractField(json, "body");
    
    if (!l.geoUri.empty() && l.geoUri.compare(0, 4, "geo:") == 0) {
        auto comma = l.geoUri.find(',', 4);
        if (comma != std::string::npos) {
            try {
                l.latitude = std::stod(l.geoUri.substr(4, comma - 4));
                l.longitude = std::stod(l.geoUri.substr(comma + 1));
            } catch(...) {}
        }
    }
    
    if (l.latitude != 0.0 || l.longitude != 0.0)
        l.mapUrl = buildStaticMapTileUrl(l.latitude, l.longitude, 15, 400, 250);
    
    l.isLive = json.find("\"org.matrix.msc3488.beacon_info\"") != std::string::npos;
    return l;
}

std::string formatLocationText(const LocationMessageDisplay& l) {
    if (!l.description.empty()) return "📍 " + l.description;
    if (l.latitude != 0.0) {
        std::ostringstream os;
        os << "📍 " << l.latitude << ", " << l.longitude;
        if (l.isLive) os << " (live)";
        return os.str();
    }
    return "📍 Location shared";
}

std::string buildStaticMapTileUrl(double lat, double lon, int zoom, int w, int h) {
    std::ostringstream os;
    os << "https://staticmap.openstreetmap.de/staticmap.php?"
       << "center=" << lat << "," << lon << "&zoom=" << zoom
       << "&size=" << w << "x" << h << "&maptype=mapnik";
    return os.str();
}

bool isLocationStale(int64_t ts) {
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    return (now - ts) > 3600000;
}

} // namespace pcp
