#pragma once

#include <string>
#include <cstdint>
#include <vector>

namespace progressive {

// =============================================================================
// Location Asset Type
// Original Kotlin: LocationAssetType object
// =============================================================================

namespace LocationAssetType {
    // Original Kotlin: const val SELF = "m.self"
    constexpr const char* SELF = "m.self";
    // Original Kotlin: const val PIN = "m.pin"
    constexpr const char* PIN = "m.pin";
    // Custom (not in spec, used for third-party asset types)
    constexpr const char* CUSTOM = "custom";
}

// =============================================================================
// ThumbnailInfo (for location thumbnails)
// Original Kotlin: ThumbnailInfo data class
// =============================================================================
struct ThumbnailInfo {
    // Original Kotlin: @Json(name="w") val width: Int = 0
    int width = 0;
    // Original Kotlin: @Json(name="h") val height: Int = 0
    int height = 0;
    // Original Kotlin: @Json(name="size") val size: Long = 0
    int64_t size = 0;
    // Original Kotlin: @Json(name="mimetype") val mimeType: String?
    std::string mimeType;
};

// =============================================================================
// LocationInfo
// Original Kotlin: LocationInfo data class
// =============================================================================
struct LocationInfo {
    // Original Kotlin: @Json(name="uri") val geoUri: String? = null
    std::string geoUri;
    // Original Kotlin: @Json(name="description") val description: String? = null
    std::string description;
    // Thumbnail URL (not directly in Kotlin LocationInfo, but used in extended spec)
    std::string thumbnailUrl;
    // Thumbnail metadata
    ThumbnailInfo thumbnailInfo;
};

// =============================================================================
// LocationAsset
// Original Kotlin: LocationAsset data class
// =============================================================================
struct LocationAsset {
    // Original Kotlin: @Json(name="type") val type: String? = null
    std::string type;
};

// =============================================================================
// MessageLocationContent
// Original Kotlin: MessageLocationContent data class (implements MessageContent)
// =============================================================================
struct MessageLocationContent {
    // Original Kotlin: @Json(name=MSG_TYPE_JSON_KEY) override val msgType: String = MSGTYPE_LOCATION
    std::string msgType;    // "m.location"
    // Original Kotlin: @Json(name="body") override val body: String
    std::string body;
    // Original Kotlin: @Json(name="geo_uri") val geoUri: String
    std::string geoUri;
    // Original Kotlin: @Json(name="m.relates_to") override val relatesTo: RelationDefaultContent? = null
    std::string relatesToRaw;
    // Original Kotlin: @Json(name="m.new_content") override val newContent: Content? = null
    std::string newContentRaw;
    // Original Kotlin: @Json(name="m.location") val locationInfo: LocationInfo? = null
    LocationInfo locationInfo;
    // Original Kotlin: @Json(name="org.matrix.msc3488.location") val unstableLocationInfo: LocationInfo? = null
    LocationInfo unstableLocationInfo;
    // Original Kotlin: @Json(name="m.ts") val timestampMillis: Long? = null
    int64_t timestampMillis = 0;
    // Original Kotlin: @Json(name="org.matrix.msc3488.ts") val unstableTimestampMillis: Long? = null
    int64_t unstableTimestampMillis = 0;
    // Original Kotlin: @Json(name="m.text") val text: String? = null
    std::string text;
    // Original Kotlin: @Json(name="org.matrix.msc1767.text") val unstableText: String? = null
    std::string unstableText;
    // Original Kotlin: @Json(name="m.asset") val locationAsset: LocationAsset? = null
    LocationAsset locationAsset;
    // Original Kotlin: @Json(name="org.matrix.msc3488.asset") val unstableLocationAsset: LocationAsset? = null
    LocationAsset unstableLocationAsset;

    // Original Kotlin: fun getBestLocationInfo() = locationInfo ?: unstableLocationInfo
    const LocationInfo& getBestLocationInfo() const;
    // Original Kotlin: fun getBestTimestampMillis() = timestampMillis ?: unstableTimestampMillis
    int64_t getBestTimestampMillis() const;
    // Original Kotlin: fun getBestText() = text ?: unstableText
    std::string getBestText() const;
    // Original Kotlin: fun getBestLocationAsset() = locationAsset ?: unstableLocationAsset
    const LocationAsset& getBestLocationAsset() const;
    // Original Kotlin: fun getBestGeoUri() = getBestLocationInfo()?.geoUri ?: geoUri
    std::string getBestGeoUri() const;
    // Original Kotlin: fun isSelfLocation(): Boolean
    bool isSelfLocation() const;
};

// =============================================================================
// BeaconInfo
// Original Kotlin: derived from MessageBeaconInfoContent fields
// =============================================================================
struct BeaconInfo {
    // Original Kotlin: @Json(name="description") val description: String? = null
    std::string description;
    // Original Kotlin: @Json(name="timeout") val timeout: Long? = null
    int64_t timeout = 0;
    // Original Kotlin: @Json(name="live") val isLive: Boolean? = null
    bool isLive = false;
    // Original Kotlin: derived from getBestLocationAsset()
    std::string assetType;
    // Original Kotlin: derived from getBestTimestampMillis()
    int64_t timestamp = 0;
};

// =============================================================================
// BeaconLocationData
// Original Kotlin: derived from MessageBeaconLocationDataContent fields
// =============================================================================
struct BeaconLocationData {
    // Original Kotlin: from getBestLocationInfo()?.geoUri
    std::string geoUri;
    // Original Kotlin: from getBestTimestampMillis()
    int64_t timestamp = 0;
    // Original Kotlin: getBestLocationInfo()?.description
    std::string description;
};

// =============================================================================
// MessageBeaconInfoContent
// Original Kotlin: MessageBeaconInfoContent data class (implements MessageContent)
// =============================================================================
struct MessageBeaconInfoContent {
    // Original Kotlin: @Transient override val msgType: String = MSGTYPE_BEACON_INFO
    std::string msgType;    // "org.matrix.android.sdk.beacon.info"
    // Original Kotlin: @Json(name="body") override val body: String = ""
    std::string body;
    // Original Kotlin: @Json(name="m.relates_to") override val relatesTo: RelationDefaultContent? = null
    std::string relatesToRaw;
    // Original Kotlin: @Json(name="m.new_content") override val newContent: Content? = null
    std::string newContentRaw;
    // Original Kotlin: @Json(name="description") val description: String? = null
    std::string description;
    // Original Kotlin: @Json(name="timeout") val timeout: Long? = null
    int64_t timeout = 0;
    // Original Kotlin: @Json(name="live") val isLive: Boolean? = null
    bool isLive = false;
    // Original Kotlin: @Json(name="m.ts") val timestampMillis: Long? = null
    int64_t timestampMillis = 0;
    // Original Kotlin: @Json(name="org.matrix.msc3488.ts") val unstableTimestampMillis: Long? = null
    int64_t unstableTimestampMillis = 0;
    // Original Kotlin: @Json(name="m.asset") val locationAsset: LocationAsset? = null
    LocationAsset locationAsset;
    // Original Kotlin: @Json(name="org.matrix.msc3488.asset") val unstableLocationAsset: LocationAsset = LocationAsset(LocationAssetType.SELF)
    LocationAsset unstableLocationAsset;
    // Default unstable asset type
    bool usesDefaultAsset = true;

    // Original Kotlin: fun getBestTimestampMillis() = timestampMillis ?: unstableTimestampMillis
    int64_t getBestTimestampMillis() const;
    // Original Kotlin: fun getBestLocationAsset() = locationAsset ?: unstableLocationAsset
    const LocationAsset& getBestLocationAsset() const;
};

// =============================================================================
// MessageBeaconLocationDataContent
// Original Kotlin: MessageBeaconLocationDataContent data class (implements MessageContent)
// =============================================================================
struct MessageBeaconLocationDataContent {
    // Original Kotlin: @Transient override val msgType: String = MSGTYPE_BEACON_LOCATION_DATA
    std::string msgType;    // "org.matrix.android.sdk.beacon.location.data"
    // Original Kotlin: @Json(name="body") override val body: String = ""
    std::string body;
    // Original Kotlin: @Json(name="m.relates_to") override val relatesTo: RelationDefaultContent? = null
    std::string relatesToRaw;
    // Original Kotlin: @Json(name="m.new_content") override val newContent: Content? = null
    std::string newContentRaw;
    // Original Kotlin: @Json(name="m.location") val locationInfo: LocationInfo? = null
    LocationInfo locationInfo;
    // Original Kotlin: @Json(name="org.matrix.msc3488.location") val unstableLocationInfo: LocationInfo? = null
    LocationInfo unstableLocationInfo;
    // Original Kotlin: @Json(name="m.ts") val timestampMillis: Long? = null
    int64_t timestampMillis = 0;
    // Original Kotlin: @Json(name="org.matrix.msc3488.ts") val unstableTimestampMillis: Long? = null
    int64_t unstableTimestampMillis = 0;

    // Original Kotlin: fun getBestLocationInfo() = locationInfo ?: unstableLocationInfo
    const LocationInfo& getBestLocationInfo() const;
    // Original Kotlin: fun getBestTimestampMillis() = timestampMillis ?: unstableTimestampMillis
    int64_t getBestTimestampMillis() const;
};

// =============================================================================
// Live Location Share State
// Original Kotlin: derived from LiveLocationShareAggregatedSummary.isActive
// =============================================================================
enum class LiveLocationShareState {
    ACTIVE,    // Live sharing is running
    PAUSED,    // Live sharing is paused (beacon exists but isLive=false or expired)
    EXPIRED,   // Timeout has elapsed
    STOPPED    // Explicitly stopped by user
};

// =============================================================================
// LiveLocationShareSummary
// Original Kotlin: LiveLocationShareAggregatedSummary data class
// =============================================================================
struct LiveLocationShareSummary {
    // Original Kotlin: val roomId: String?
    std::string roomId;
    // Original Kotlin: val userId: String?
    std::string userId;
    // Original Kotlin: val isActive: Boolean?
    bool isActive = false;
    // Original Kotlin: val endOfLiveTimestampMillis: Long?
    int64_t endOfLiveTimestampMillis = 0;
    // Original Kotlin: val lastLocationDataContent: MessageBeaconLocationDataContent?
    MessageBeaconLocationDataContent lastLocationData;
};

// =============================================================================
// JSON Builders
// =============================================================================

// Original Kotlin: MessageLocationContent.toContent()
std::string buildLocationContent(const MessageLocationContent& content);

// Original Kotlin: MessageBeaconInfoContent(...).toContent()
std::string buildBeaconInfoContent(const BeaconInfo& info);

// Original Kotlin: Build beacon location data event content
std::string buildBeaconLocationDataContent(const BeaconLocationData& data);

// =============================================================================
// JSON Parsers
// =============================================================================

// Original Kotlin: toModel<MessageLocationContent>()
MessageLocationContent parseLocationContent(const std::string& contentJson);

// Original Kotlin: toModel<MessageBeaconInfoContent>()
BeaconInfo parseBeaconInfo(const std::string& contentJson);

// Original Kotlin: toModel<MessageBeaconLocationDataContent>()
BeaconLocationData parseBeaconLocationData(const std::string& contentJson);

// =============================================================================
// Beacon Logic
// =============================================================================

// Original Kotlin: Beacon should be considered as inactive after timeout
// Checks if beacon has expired based on creation timestamp + timeout
bool isBeaconExpired(const BeaconInfo& info, int64_t currentTimeMs);

// Original Kotlin: Format beacon timestamp for human-readable display
std::string formatBeaconTimestamp(int64_t timestampMs);

// =============================================================================
// Live Location State Computations
// =============================================================================

// Original Kotlin: Derived logic from DefaultLocationSharingService + aggregation
// Compute the current state of a live location share from its beacon info.
LiveLocationShareState computeLiveLocationState(const BeaconInfo& info, int64_t currentTimeMs);

// Original Kotlin: CheckIfExistingActiveLiveTask — is live location active?
bool isLiveLocationActive(const BeaconInfo& info, int64_t currentTimeMs);

// =============================================================================
// Geolocation Utilities
// =============================================================================

// Original Kotlin: Geo URI parsing used throughout location sharing
//
// Parse "geo:lat,lon;u=uncertainty" format (RFC 5870).
// Returns false if parsing fails.
// On success, fills lat, lon, uncertainty.
bool parseGeoUri(const std::string& uri, double& lat, double& lon, double& uncertainty);

// Original Kotlin: Geo URI building
//
// Build a "geo:lat,lon;u=uncertainty" formatted string.
// If uncertainty is <= 0, the ";u=..." parameter is omitted.
std::string formatGeoUri(double latitude, double longitude, double uncertainty);

// Original Kotlin: Distance computation via Haversine formula
//
// Compute the great-circle distance between two points on Earth
// using the Haversine formula. Returns distance in meters.
double computeDistance(double lat1, double lon1, double lat2, double lon2);

} // namespace progressive
