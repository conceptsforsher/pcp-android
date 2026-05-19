#include "progressive/message_location.hpp"
#include <sstream>
#include <cmath>
#include <chrono>
#include <ctime>
#include <algorithm>
#include <cstdlib>

namespace progressive {

// =============================================================================
// Internal JSON extraction helpers
// =============================================================================

namespace {

// Original Kotlin: Moshi-based JSON extraction, ported as manual string search

// Extract a string value for a given JSON key.
// e.g. extractStr("{\"foo\":\"bar\"}", "foo") -> "bar"
std::string extractStr(const std::string& json, const std::string& key) {
    auto pp = json.find("\"" + key + "\"");
    if (pp == std::string::npos) return "";
    pp = json.find(':', pp + key.size() + 2);
    if (pp == std::string::npos) return "";
    pp++;
    while (pp < json.size() && (json[pp] == ' ' || json[pp] == '\t' || json[pp] == '\n')) pp++;
    if (pp >= json.size() || json[pp] != '"') return "";
    pp++;
    std::string result;
    while (pp < json.size()) {
        if (json[pp] == '\\' && pp + 1 < json.size()) {
            pp++;
            char c = json[pp];
            if (c == '"') result += '"';
            else if (c == '\\') result += '\\';
            else if (c == 'n') result += '\n';
            else if (c == 't') result += '\t';
            else { result += '\\'; result += c; }
        } else if (json[pp] == '"') {
            break;
        } else {
            result += json[pp];
        }
        pp++;
    }
    return result;
}

// Extract an int64_t value for a given JSON key.
int64_t extractInt(const std::string& json, const std::string& key) {
    auto pp = json.find("\"" + key + "\"");
    if (pp == std::string::npos) return 0;
    pp = json.find(':', pp + key.size() + 2);
    if (pp == std::string::npos) return 0;
    pp++;
    while (pp < json.size() && (json[pp] == ' ' || json[pp] == '\t' || json[pp] == '\n')) pp++;
    bool neg = false;
    if (pp < json.size() && json[pp] == '-') { neg = true; pp++; }
    int64_t v = 0;
    while (pp < json.size() && json[pp] >= '0' && json[pp] <= '9') {
        v = v * 10 + (json[pp] - '0');
        pp++;
    }
    return neg ? -v : v;
}

// Extract a bool value for a given JSON key.
// Looks for "key":true or "key":false
bool extractBool(const std::string& json, const std::string& key) {
    auto pp = json.find("\"" + key + "\"");
    if (pp == std::string::npos) return false;
    pp = json.find(':', pp + key.size() + 2);
    if (pp == std::string::npos) return false;
    pp++;
    while (pp < json.size() && (json[pp] == ' ' || json[pp] == '\t' || json[pp] == '\n')) pp++;
    if (pp + 3 < json.size() && json.compare(pp, 4, "true") == 0) return true;
    return false;
}

// Extract a nested JSON object for a given key as a raw string.
// e.g. extractObject("{\"foo\":{\"a\":1}}", "foo") -> "{\"a\":1}"
std::string extractObject(const std::string& json, const std::string& key) {
    auto pp = json.find("\"" + key + "\"");
    if (pp == std::string::npos) return "";
    pp = json.find(':', pp + key.size() + 2);
    if (pp == std::string::npos) return "";
    pp++;
    while (pp < json.size() && (json[pp] == ' ' || json[pp] == '\t' || json[pp] == '\n')) pp++;
    if (pp >= json.size() || json[pp] != '{') return "";
    size_t start = pp;
    int depth = 0;
    while (pp < json.size()) {
        if (json[pp] == '{') depth++;
        else if (json[pp] == '}') { depth--; if (depth == 0) { pp++; break; } }
        pp++;
    }
    return json.substr(start, pp - start);
}

// Parse a LocationInfo from a nested JSON object string.
LocationInfo parseLocationInfo(const std::string& json) {
    LocationInfo info;
    if (json.empty()) return info;
    info.geoUri = extractStr(json, "uri");
    info.description = extractStr(json, "description");
    return info;
}

// Parse a LocationAsset from a nested JSON object string.
LocationAsset parseLocationAsset(const std::string& json) {
    LocationAsset asset;
    if (json.empty()) return asset;
    asset.type = extractStr(json, "type");
    return asset;
}

// Parse a ThumbnailInfo from a nested JSON object string.
ThumbnailInfo parseThumbnailInfo(const std::string& json) {
    ThumbnailInfo info;
    if (json.empty()) return info;
    info.width = static_cast<int>(extractInt(json, "w"));
    info.height = static_cast<int>(extractInt(json, "h"));
    info.size = extractInt(json, "size");
    info.mimeType = extractStr(json, "mimetype");
    return info;
}

// Escape a string for JSON (basic escaping).
std::string jsonEscape(const std::string& s) {
    std::string out;
    for (char c : s) {
        if (c == '"') out += "\\\"";
        else if (c == '\\') out += "\\\\";
        else if (c == '\n') out += "\\n";
        else if (c == '\t') out += "\\t";
        else out += c;
    }
    return out;
}

} // anonymous namespace

// =============================================================================
// MessageLocationContent member functions
// =============================================================================

// Original Kotlin: fun getBestLocationInfo() = locationInfo ?: unstableLocationInfo
const LocationInfo& MessageLocationContent::getBestLocationInfo() const {
    if (!locationInfo.geoUri.empty() || !locationInfo.description.empty()) {
        return locationInfo;
    }
    return unstableLocationInfo;
}

// Original Kotlin: fun getBestTimestampMillis() = timestampMillis ?: unstableTimestampMillis
int64_t MessageLocationContent::getBestTimestampMillis() const {
    if (timestampMillis > 0) return timestampMillis;
    return unstableTimestampMillis;
}

// Original Kotlin: fun getBestText() = text ?: unstableText
std::string MessageLocationContent::getBestText() const {
    if (!text.empty()) return text;
    return unstableText;
}

// Original Kotlin: fun getBestLocationAsset() = locationAsset ?: unstableLocationAsset
const LocationAsset& MessageLocationContent::getBestLocationAsset() const {
    if (!locationAsset.type.empty()) return locationAsset;
    return unstableLocationAsset;
}

// Original Kotlin: fun getBestGeoUri() = getBestLocationInfo()?.geoUri ?: geoUri
std::string MessageLocationContent::getBestGeoUri() const {
    const auto& bestInfo = getBestLocationInfo();
    if (!bestInfo.geoUri.empty()) return bestInfo.geoUri;
    return geoUri;
}

// Original Kotlin:
//   fun isSelfLocation(): Boolean {
//       val locationAsset = getBestLocationAsset()
//       return locationAsset?.type == null || locationAsset.type == LocationAssetType.SELF
//   }
bool MessageLocationContent::isSelfLocation() const {
    const auto& asset = getBestLocationAsset();
    if (asset.type.empty()) return true;    // null asset -> treat as self
    return asset.type == LocationAssetType::SELF;
}

// =============================================================================
// MessageBeaconInfoContent member functions
// =============================================================================

// Original Kotlin: fun getBestTimestampMillis() = timestampMillis ?: unstableTimestampMillis
int64_t MessageBeaconInfoContent::getBestTimestampMillis() const {
    if (timestampMillis > 0) return timestampMillis;
    return unstableTimestampMillis;
}

// Original Kotlin: fun getBestLocationAsset() = locationAsset ?: unstableLocationAsset
const LocationAsset& MessageBeaconInfoContent::getBestLocationAsset() const {
    if (!locationAsset.type.empty()) return locationAsset;
    return unstableLocationAsset;
}

// =============================================================================
// MessageBeaconLocationDataContent member functions
// =============================================================================

// Original Kotlin: fun getBestLocationInfo() = locationInfo ?: unstableLocationInfo
const LocationInfo& MessageBeaconLocationDataContent::getBestLocationInfo() const {
    if (!locationInfo.geoUri.empty() || !locationInfo.description.empty()) {
        return locationInfo;
    }
    return unstableLocationInfo;
}

// Original Kotlin: fun getBestTimestampMillis() = timestampMillis ?: unstableTimestampMillis
int64_t MessageBeaconLocationDataContent::getBestTimestampMillis() const {
    if (timestampMillis > 0) return timestampMillis;
    return unstableTimestampMillis;
}

// =============================================================================
// buildLocationContent
// Original Kotlin: MessageLocationContent.toContent()
//
// Builds JSON for m.location message event.
// Format:
//   {"msgtype":"m.location","body":"...","geo_uri":"geo:...",
//    "m.location":{"uri":"...","description":"..."},
//    "m.ts":...,"m.asset":{"type":"..."}}
// =============================================================================
std::string buildLocationContent(const MessageLocationContent& content) {
    std::ostringstream os;
    os << R"({"msgtype":")" << jsonEscape(content.msgType) << R"(")";
    os << R"(,"body":")" << jsonEscape(content.body) << R"(")";
    os << R"(,"geo_uri":")" << jsonEscape(content.geoUri) << R"(")";

    // Original Kotlin: relatesTo serialization
    if (!content.relatesToRaw.empty()) {
        os << R"(,"m.relates_to":)" << content.relatesToRaw;
    }
    // Original Kotlin: newContent serialization
    if (!content.newContentRaw.empty()) {
        os << R"(,"m.new_content":)" << content.newContentRaw;
    }

    // Original Kotlin: @Json(name="m.location") val locationInfo: LocationInfo? = null
    const auto& locInfo = content.locationInfo;
    if (!locInfo.geoUri.empty() || !locInfo.description.empty()) {
        os << R"(,"m.location":{)";
        bool firstLoc = true;
        if (!locInfo.geoUri.empty()) {
            os << R"("uri":")" << jsonEscape(locInfo.geoUri) << R"(")";
            firstLoc = false;
        }
        if (!locInfo.description.empty()) {
            if (!firstLoc) os << ",";
            os << R"("description":")" << jsonEscape(locInfo.description) << R"(")";
            firstLoc = false;
        }
        os << "}";
    }

    // Original Kotlin: @Json(name="org.matrix.msc3488.location") val unstableLocationInfo
    const auto& unstableLoc = content.unstableLocationInfo;
    if (!unstableLoc.geoUri.empty() || !unstableLoc.description.empty()) {
        os << R"(,"org.matrix.msc3488.location":{)";
        bool first = true;
        if (!unstableLoc.geoUri.empty()) {
            os << R"("uri":")" << jsonEscape(unstableLoc.geoUri) << R"(")";
            first = false;
        }
        if (!unstableLoc.description.empty()) {
            if (!first) os << ",";
            os << R"("description":")" << jsonEscape(unstableLoc.description) << R"(")";
            first = false;
        }
        os << "}";
    }

    // Original Kotlin: @Json(name="m.ts") val timestampMillis: Long? = null
    if (content.timestampMillis > 0) {
        os << R"(,"m.ts":)" << content.timestampMillis;
    }
    // Original Kotlin: @Json(name="org.matrix.msc3488.ts") val unstableTimestampMillis
    if (content.unstableTimestampMillis > 0) {
        os << R"(,"org.matrix.msc3488.ts":)" << content.unstableTimestampMillis;
    }

    // Original Kotlin: @Json(name="m.text") val text: String? = null
    if (!content.text.empty()) {
        os << R"(,"m.text":")" << jsonEscape(content.text) << R"(")";
    }
    // Original Kotlin: @Json(name="org.matrix.msc1767.text") val unstableText
    if (!content.unstableText.empty()) {
        os << R"(,"org.matrix.msc1767.text":")" << jsonEscape(content.unstableText) << R"(")";
    }

    // Original Kotlin: @Json(name="m.asset") val locationAsset: LocationAsset? = null
    if (!content.locationAsset.type.empty()) {
        os << R"(,"m.asset":{"type":")" << jsonEscape(content.locationAsset.type) << R"("})";
    }
    // Original Kotlin: @Json(name="org.matrix.msc3488.asset") val unstableLocationAsset
    if (!content.unstableLocationAsset.type.empty()) {
        os << R"(,"org.matrix.msc3488.asset":{"type":")" << jsonEscape(content.unstableLocationAsset.type) << R"("})";
    }

    os << "}";
    return os.str();
}

// =============================================================================
// parseLocationContent
// Original Kotlin: toModel<MessageLocationContent>()
// =============================================================================
MessageLocationContent parseLocationContent(const std::string& contentJson) {
    MessageLocationContent content;

    // Original Kotlin: @Json(name=MSG_TYPE_JSON_KEY) msgType
    content.msgType = extractStr(contentJson, "msgtype");

    // Original Kotlin: @Json(name="body") body
    content.body = extractStr(contentJson, "body");

    // Original Kotlin: @Json(name="geo_uri") geoUri
    content.geoUri = extractStr(contentJson, "geo_uri");

    // Original Kotlin: @Json(name="m.relates_to") relatesTo
    content.relatesToRaw = extractObject(contentJson, "m.relates_to");

    // Original Kotlin: @Json(name="m.new_content") newContent
    content.newContentRaw = extractObject(contentJson, "m.new_content");

    // Original Kotlin: @Json(name="m.location") locationInfo
    {
        auto objJson = extractObject(contentJson, "m.location");
        if (!objJson.empty()) {
            content.locationInfo = parseLocationInfo(objJson);
            content.locationInfo.thumbnailUrl = extractStr(objJson, "thumbnail_url");
            auto thumbObj = extractObject(objJson, "thumbnail_info");
            if (!thumbObj.empty()) {
                content.locationInfo.thumbnailInfo = parseThumbnailInfo(thumbObj);
            }
        }
    }

    // Original Kotlin: @Json(name="org.matrix.msc3488.location") unstableLocationInfo
    {
        auto objJson = extractObject(contentJson, "org.matrix.msc3488.location");
        if (!objJson.empty()) {
            content.unstableLocationInfo = parseLocationInfo(objJson);
            content.unstableLocationInfo.thumbnailUrl = extractStr(objJson, "thumbnail_url");
            auto thumbObj = extractObject(objJson, "thumbnail_info");
            if (!thumbObj.empty()) {
                content.unstableLocationInfo.thumbnailInfo = parseThumbnailInfo(thumbObj);
            }
        }
    }

    // Original Kotlin: @Json(name="m.ts") timestampMillis
    content.timestampMillis = extractInt(contentJson, "m.ts");
    // Original Kotlin: @Json(name="org.matrix.msc3488.ts") unstableTimestampMillis
    content.unstableTimestampMillis = extractInt(contentJson, "org.matrix.msc3488.ts");

    // Original Kotlin: @Json(name="m.text") text
    content.text = extractStr(contentJson, "m.text");
    // Original Kotlin: @Json(name="org.matrix.msc1767.text") unstableText
    content.unstableText = extractStr(contentJson, "org.matrix.msc1767.text");

    // Original Kotlin: @Json(name="m.asset") locationAsset
    {
        auto objJson = extractObject(contentJson, "m.asset");
        if (!objJson.empty()) {
            content.locationAsset = parseLocationAsset(objJson);
        }
    }

    // Original Kotlin: @Json(name="org.matrix.msc3488.asset") unstableLocationAsset
    {
        auto objJson = extractObject(contentJson, "org.matrix.msc3488.asset");
        if (!objJson.empty()) {
            content.unstableLocationAsset = parseLocationAsset(objJson);
        }
    }

    return content;
}

// =============================================================================
// buildBeaconInfoContent
// Original Kotlin: MessageBeaconInfoContent(...).toContent()
//
// Builds JSON for m.beacon_info state event content.
// Format:
//   {"body":"...","description":"...","timeout":300000,"live":true,
//    "org.matrix.msc3488.ts":...,"m.ts":...,
//    "org.matrix.msc3488.asset":{"type":"m.self"},"m.asset":{"type":"..."}}
// =============================================================================
std::string buildBeaconInfoContent(const BeaconInfo& info) {
    std::ostringstream os;
    os << R"({"body":")" << jsonEscape(info.description) << R"(")";
    os << R"(,"description":")" << jsonEscape(info.description) << R"(")";
    os << R"(,"timeout":)" << info.timeout;
    os << R"(,"live":)" << (info.isLive ? "true" : "false");

    // Original Kotlin: @Json(name="org.matrix.msc3488.ts") unstableTimestampMillis
    if (info.timestamp > 0) {
        os << R"(,"org.matrix.msc3488.ts":)" << info.timestamp;
    }

    // Original Kotlin: @Json(name="org.matrix.msc3488.asset") unstableLocationAsset
    if (!info.assetType.empty()) {
        os << R"(,"org.matrix.msc3488.asset":{"type":")" << jsonEscape(info.assetType) << R"("})";
    }

    os << "}";
    return os.str();
}

// =============================================================================
// buildBeaconLocationDataContent
// Original Kotlin: Builds beacon location data event content (m.beacon).
//
// Format:
//   {"body":"","org.matrix.msc3488.location":{"uri":"geo:...","description":"..."},
//    "m.location":{"uri":"...","description":"..."},
//    "org.matrix.msc3488.ts":...,"m.ts":...}
// =============================================================================
std::string buildBeaconLocationDataContent(const BeaconLocationData& data) {
    std::ostringstream os;
    os << R"({"body":")" << jsonEscape(data.description) << R"(")";

    // Original Kotlin: @Json(name="org.matrix.msc3488.location") unstableLocationInfo
    os << R"(,"org.matrix.msc3488.location":{)";
    os << R"("uri":")" << jsonEscape(data.geoUri) << R"(")";
    if (!data.description.empty()) {
        os << R"(,"description":")" << jsonEscape(data.description) << R"(")";
    }
    os << "}";

    // Original Kotlin: @Json(name="org.matrix.msc3488.ts") unstableTimestampMillis
    if (data.timestamp > 0) {
        os << R"(,"org.matrix.msc3488.ts":)" << data.timestamp;
    }

    os << "}";
    return os.str();
}

// =============================================================================
// parseBeaconInfo
// Original Kotlin: toModel<MessageBeaconInfoContent>()
//
// Parses m.beacon_info content JSON into BeaconInfo.
// =============================================================================
BeaconInfo parseBeaconInfo(const std::string& contentJson) {
    BeaconInfo info;

    // Original Kotlin: @Json(name="description") val description: String? = null
    info.description = extractStr(contentJson, "description");

    // Original Kotlin: @Json(name="timeout") val timeout: Long? = null
    info.timeout = extractInt(contentJson, "timeout");

    // Original Kotlin: @Json(name="live") val isLive: Boolean? = null
    info.isLive = extractBool(contentJson, "live");

    // Original Kotlin: fun getBestTimestampMillis() = timestampMillis ?: unstableTimestampMillis
    info.timestamp = extractInt(contentJson, "m.ts");
    if (info.timestamp == 0) {
        info.timestamp = extractInt(contentJson, "org.matrix.msc3488.ts");
    }

    // Original Kotlin: fun getBestLocationAsset() = locationAsset ?: unstableLocationAsset
    auto stableAsset = extractObject(contentJson, "m.asset");
    if (!stableAsset.empty()) {
        info.assetType = parseLocationAsset(stableAsset).type;
    }
    if (info.assetType.empty()) {
        auto unstableAsset = extractObject(contentJson, "org.matrix.msc3488.asset");
        if (!unstableAsset.empty()) {
            info.assetType = parseLocationAsset(unstableAsset).type;
        }
    }
    if (info.assetType.empty()) {
        // Original Kotlin: default is SELF
        info.assetType = LocationAssetType::SELF;
    }

    return info;
}

// =============================================================================
// parseBeaconLocationData
// Original Kotlin: toModel<MessageBeaconLocationDataContent>()
//
// Parses beacon location data event content JSON.
// =============================================================================
BeaconLocationData parseBeaconLocationData(const std::string& contentJson) {
    BeaconLocationData data;

    // Original Kotlin: getBestLocationInfo()?.geoUri
    auto stableLoc = extractObject(contentJson, "m.location");
    if (!stableLoc.empty()) {
        auto locInfo = parseLocationInfo(stableLoc);
        data.geoUri = locInfo.geoUri;
        data.description = locInfo.description;
    }

    if (data.geoUri.empty()) {
        auto unstableLoc = extractObject(contentJson, "org.matrix.msc3488.location");
        if (!unstableLoc.empty()) {
            auto locInfo = parseLocationInfo(unstableLoc);
            data.geoUri = locInfo.geoUri;
            data.description = locInfo.description;
        }
    }

    // If still empty, try direct geo_uri or uri field
    if (data.geoUri.empty()) {
        data.geoUri = extractStr(contentJson, "geo_uri");
    }
    if (data.geoUri.empty()) {
        data.geoUri = extractStr(contentJson, "uri");
    }

    // Original Kotlin: getBestTimestampMillis()
    data.timestamp = extractInt(contentJson, "m.ts");
    if (data.timestamp == 0) {
        data.timestamp = extractInt(contentJson, "org.matrix.msc3488.ts");
    }

    // Original Kotlin: getBestLocationInfo()?.description (for fallback)
    if (data.description.empty()) {
        data.description = extractStr(contentJson, "description");
    }

    return data;
}

// =============================================================================
// isBeaconExpired
// Original Kotlin:
//   Beacon should be considered as inactive after this timeout as milliseconds.
//   In practice: (currentTimeMs - beaconInfo.getBestTimestampMillis()) >= timeout
// =============================================================================
bool isBeaconExpired(const BeaconInfo& info, int64_t currentTimeMs) {
    if (info.timeout <= 0) return false;

    // Original Kotlin: Get current time if not provided
    if (currentTimeMs <= 0) {
        currentTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
    }

    // Original Kotlin: Check if timeout has elapsed since beacon creation
    int64_t elapsed = currentTimeMs - info.timestamp;
    return elapsed >= info.timeout || info.timestamp <= 0;
}

// =============================================================================
// formatBeaconTimestamp
// Original Kotlin: Format beacon creation/timestamp in human-readable form.
//
// Uses C++ chrono time_t to format. Returns something like "2025-01-15T14:30:00Z".
// =============================================================================
std::string formatBeaconTimestamp(int64_t timestampMs) {
    if (timestampMs <= 0) return "";

    // Original Kotlin: Convert milliseconds to time_t
    auto timeSec = static_cast<std::time_t>(timestampMs / 1000);
    auto tm = *std::gmtime(&timeSec);

    char buf[64];
    std::strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%SZ", &tm);
    return std::string(buf);
}

// =============================================================================
// computeLiveLocationState
// Original Kotlin: Derived from LiveLocationShareAggregatedSummary.isActive,
//   DefaultLocationSharingService, and beacon timeout logic.
//
// Determines if a live location share is ACTIVE, PAUSED, EXPIRED, or STOPPED.
// =============================================================================
LiveLocationShareState computeLiveLocationState(const BeaconInfo& info, int64_t currentTimeMs) {
    if (currentTimeMs <= 0) {
        currentTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
    }

    // Original Kotlin: If isLive is explicitly false, it's been stopped
    if (!info.isLive) {
        return LiveLocationShareState::STOPPED;
    }

    // Original Kotlin: Check timeout expiration
    // From DefaultLocationSharingService: if !isActive && endOfLiveTimestamp < now, it's expired
    if (info.timeout > 0 && info.timestamp > 0) {
        int64_t elapsed = currentTimeMs - info.timestamp;
        if (elapsed >= info.timeout) {
            return LiveLocationShareState::EXPIRED;
        }
    }

    // Original Kotlin: Default: still active (isLive=true, not yet expired)
    return LiveLocationShareState::ACTIVE;
}

// =============================================================================
// isLiveLocationActive
// Original Kotlin: CheckIfExistingActiveLiveTask
//   getActiveBeaconInfoForUserTask.execute(params)
//     ?.getClearContent()?.toModel<MessageBeaconInfoContent>()?.isLive.orFalse()
// =============================================================================
bool isLiveLocationActive(const BeaconInfo& info, int64_t currentTimeMs) {
    if (!info.isLive) return false;

    // Original Kotlin: Also check if beacon has expired
    if (isBeaconExpired(info, currentTimeMs)) return false;

    return true;
}

// =============================================================================
// parseGeoUri
// Original Kotlin: RFC 5870 geo URI parsing used throughout location sharing.
//
// Parses "geo:lat,lon;u=uncertainty" format.
// Returns false if parsing fails.
// =============================================================================
bool parseGeoUri(const std::string& uri, double& lat, double& lon, double& uncertainty) {
    // Must start with "geo:"
    if (uri.compare(0, 4, "geo:") != 0) return false;

    // Extract lat,lon before semicolon
    auto semicolon = uri.find(';', 4);
    std::string coords = (semicolon == std::string::npos) ? uri.substr(4) : uri.substr(4, semicolon - 4);

    auto comma = coords.find(',');
    if (comma == std::string::npos) return false;

    lat = std::stod(coords.substr(0, comma));
    lon = std::stod(coords.substr(comma + 1));

    // Validate coordinate ranges
    if (lat < -90.0 || lat > 90.0) return false;
    if (lon < -180.0 || lon > 180.0) return false;

    // Parse uncertainty parameter: ;u=N
    uncertainty = 0.0;
    if (semicolon != std::string::npos) {
        auto uPos = uri.find(";u=", semicolon);
        if (uPos != std::string::npos) {
            auto uStart = uPos + 3;
            auto uEnd = uri.find(';', uStart);
            std::string uStr = (uEnd == std::string::npos) ? uri.substr(uStart) : uri.substr(uStart, uEnd - uStart);
            uncertainty = std::stod(uStr);
        }
    }

    return true;
}

// =============================================================================
// formatGeoUri
// Original Kotlin: Build RFC 5870 geo URI.
//
// Format: "geo:lat,lon" or "geo:lat,lon;u=uncertainty" if uncertainty > 0.
// =============================================================================
std::string formatGeoUri(double latitude, double longitude, double uncertainty) {
    std::ostringstream os;
    os << "geo:" << latitude << "," << longitude;
    if (uncertainty > 0.0) {
        os << ";u=" << uncertainty;
    }
    return os.str();
}

// =============================================================================
// computeDistance
// Original Kotlin: Haversine formula for distance between two geographic points.
//
// Computes the great-circle distance in meters between (lat1,lon1) and (lat2,lon2).
// Uses the Haversine formula with Earth radius = 6,371,000 meters.
// =============================================================================
double computeDistance(double lat1, double lon1, double lat2, double lon2) {
    const double R = 6371000.0; // Earth radius in meters

    double lat1Rad = lat1 * M_PI / 180.0;
    double lat2Rad = lat2 * M_PI / 180.0;
    double dLat = (lat2 - lat1) * M_PI / 180.0;
    double dLon = (lon2 - lon1) * M_PI / 180.0;

    double sinDLat = std::sin(dLat / 2.0);
    double sinDLon = std::sin(dLon / 2.0);

    double a = sinDLat * sinDLat +
               std::cos(lat1Rad) * std::cos(lat2Rad) * sinDLon * sinDLon;
    double c = 2.0 * std::atan2(std::sqrt(a), std::sqrt(1.0 - a));

    return R * c;
}

} // namespace progressive
