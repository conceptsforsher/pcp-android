# C++ Desync Detector (`desync_detector.cpp`)

## Problem Statement

When a Matrix user joins the same room from multiple homeservers (a scenario common in federated communities), the two views of the room can diverge. A message delivered successfully to homeserver A might never reach homeserver B due to federation lag, network splits, or server bugs. The user sees a different conversation depending on which account they're logged into.

**The desync detector is a safety net.** It continuously tracks which events exist on each homeserver and warns the user when divergence is detected.

This is a novel feature — neither Element Web nor Element Android has anything comparable. It was designed specifically for Progressive Chat's multi-account architecture where a user might be logged into `@user:matrix.org` and `@user:server.com` simultaneously in the same room.

## How It Works

The detector maintains two data structures in memory:

### Forward Index: `serverEvents_`
```
"matrix.org"  → {"$ev1", "$ev2", "$ev3", "$ev5"}
"server.com"  → {"$ev1", "$ev2", "$ev4"}
```

This tracks every event ID seen on each server. It's built incrementally — each time an event arrives in the timeline, `trackEvent()` records it.

### Event Timestamps: `eventTimestamps_`
```
"$ev1" → 1715600000000
"$ev2" → 1715600005000
...
```

Stores the origin server timestamp for each event, used for sorting and display.

### Detection Algorithm

When the user triggers a check (or when the periodic timer fires), `checkDesync()` runs:

1. Get all known servers from `serverEvents_` keys
2. Skip if only one server (no comparison possible)
3. For each pair of servers:
   - `missingOnCurrent` = events on other server but not on current
   - `missingOnOther` = events on current server but not on other
4. If any missing events found → `hasDesync = true`

### Example

```
Server matrix.org has: $ev1, $ev2, $ev3, $ev5
Server server.com has: $ev1, $ev2, $ev4

Checking from matrix.org:
  missingOnCurrent = {$ev4}  (exists on server.com but not on matrix.org)
  missingOnOther   = {$ev3, $ev5} (exist on matrix.org but not on server.com)

Result: hasDesync = true, 3 total events out of sync
```

## User Workflow

1. User is logged into `@user:matrix.org` and `@user:server.com`
2. Both accounts are in room `!room:matrix.org`
3. User enables "Federation desync detector" in Labs
4. Sets check interval to 30 minutes
5. When a desync is detected:
   - Notification: "3 event(s) missing on current server (present on matrix.org). Check room federation."
   - User can view the specific missing event IDs
   - User can check federation status or contact server admin

## API Reference

### `trackEvent(eventId, serverName, timestamp)`

Called from the Kotlin timeline controller every time an event is loaded into the timeline. The server name is extracted from the event's `sender` MXID domain or from the session's homeserver URL.

### `checkDesync(roomId, currentServer)` → DesyncReport

Performs the cross-server comparison and returns a report. The report includes:
- `hasDesync` — whether any divergence was found
- `missingOnCurrent` — count of events missing from the current server
- `missingServer` — which other server has the missing events
- `missingEventIds` — the actual event IDs (for detailed display)
- `warning` — human-readable message

### `shouldCheck(lastCheckMs, intervalMinutes)` → bool

Helper to determine if the user-configured interval has elapsed since the last check.

## JNI API

```kotlin
// Kotlin timeline controller calls this for every event
ProgressiveNative.nativeDesyncTrackEvent(eventId, "matrix.org", timestampMs)

// Periodic check (WorkManager / AlarmManager)
val report = ProgressiveNative.nativeDesyncCheck(roomId, "server.com")
val json = JSONObject(report)
if (json.getBoolean("hasDesync")) {
    showNotification(json.getString("warning"))
}
```

## Performance

- `trackEvent()` — O(1) hash set insertion, ~50ns per event
- `checkDesync()` — O(n) where n = total events across all servers
- Memory — ~200 bytes per event (event ID string + hash table overhead)

For a room with 100,000 events on 2 servers, a check takes ~10ms and uses ~20MB of memory. This is acceptable because checks run periodically (every 30 minutes), not on every event.

## Limitations

- **Not persisted.** The detector state is in-memory only. After app restart, it must be repopulated from cached events.
- **No cryptographic proof.** The detector can only compare event IDs — it cannot verify content integrity between servers.
- **Single-room scope.** Each room requires independent tracking. The current implementation uses a single detector for simplicity; multi-room support would require per-room instances.

## Future Work

- **Content comparison.** Beyond event ID presence, compare event content hashes to detect tampering.
- **Automatic recovery.** Offer to fetch missing events from the other server.
- **Federation health dashboard.** Aggregate desync stats across all joined rooms to build a server health score.
