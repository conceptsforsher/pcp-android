# C++ Location Sharing (`location_sharing.cpp`)

## Overview

Manages continuous location sharing sessions for Progressive Chat. Tracks who is sharing, to which room, at what interval, with auto-stop support. Includes Haversine distance calculation and GeoJSON formatting.

## Architecture

```
User Starts Sharing (Kotlin UI)
    │
    ▼
nativeLocationStartSession(roomId, userId, intervalSec, autoStop, autoStopMin)
    │
    ▼
LocationSharingManager::startSession()
    │  stores: sessionId, roomId, userId, intervalSeconds, autoStop config
    ▼
Periodic Timer (Kotlin AlarmManager / WorkManager)
    │
    ▼
nativeLocationIsDue(sessionId) → true/false
    │
    ▼
GPS Coordinates (Android LocationManager)
    │
    ▼
nativeLocationFormatMessage(lat, lon, acc, label) → "geo:51.5,-0.12"
    │
    ▼
Send to Matrix Room (Kotlin SDK)
    │
    ▼
nativeLocationFormatGeoJson(lat, lon, acc) → GeoJSON for rich rendering
```

## API Reference

### Session Management

| Function | Description |
|----------|------------|
| `startSession(session)` | Begin sharing, returns sessionId |
| `stopSession(id)` | Stop sharing by session ID |
| `getActiveSessions(userId)` | List active sessions for user |
| `isDue(sessionId)` | Check if it's time to send next update |
| `markSent(id, coord)` | Record that coordinates were sent |
| `checkAutoStop()` | Return sessions that hit auto-stop limit |

### Coordinate Utilities

| Function | Description |
|----------|------------|
| `isValidCoord(coord)` | Validate lat/lon range (-90..90, -180..180) |
| `formatLocationMessage(coord, label)` | Build `geo:` URI string |
| `formatGeoUri(coord)` | `geo:51.5,-0.12;u=10` |
| `formatGeoJson(coord)` | `{"type":"Point","coordinates":[-0.12,51.5]}` |
| `parseFromMessage(body)` | Extract coordinates from `geo:` message |
| `distanceMeters(a, b)` | Haversine great-circle distance |
| `bearingDegrees(a, b)` | Compass bearing from a to b |
| `hasMoved(old, next, threshold)` | Check if movement exceeded threshold |

## Haversine Formula

```cpp
double distanceMeters(const GeoCoord& a, const GeoCoord& b) {
    const double R = 6371000.0; // Earth radius
    double dLat = (b.lat - a.lat) * M_PI / 180.0;
    double dLon = (b.lon - a.lon) * M_PI / 180.0;
    double a = sin(dLat/2)*sin(dLat/2) +
               cos(a.lat*PI/180)*cos(b.lat*PI/180)*sin(dLon/2)*sin(dLon/2);
    return 2 * R * atan2(sqrt(a), sqrt(1-a));
}
```

## Session Configuration

```cpp
struct LocationSession {
    string sessionId;
    string roomId;          // target Matrix room
    string userId;          // the sharer
    int intervalSeconds;    // 60 = every minute
    bool autoStop;          // stop after N minutes
    int autoStopMinutes;    // e.g. 60 = one hour max
    GeoCoord lastCoord;     // most recent position
};
```

## JNI API

```kotlin
// Start sharing
val sessionId = ProgressiveNative.nativeLocationStartSession(
    roomId, userId, 60, true, 60  // every minute, auto-stop after 1h
)

// Check if due
if (ProgressiveNative.nativeLocationIsDue(sessionId)) {
    // Get GPS position
    val msg = ProgressiveNative.nativeLocationFormatMessage(lat, lon, acc, "")
    // Send to room
    room.sendService().sendTextMessage(msg)
    ProgressiveNative.nativeLocationMarkSent(sessionId, lat, lon, acc)
}
```

## Limitations

- **Single device sharing** — no multi-device sync of location state
- **No live location rendering** — GeoJSON is generated, UI rendering is in Kotlin
- **No encryption layer** — coordinates sent as plain text Matrix messages
