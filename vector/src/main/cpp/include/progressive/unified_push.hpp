#pragma once

#include <string>
#include <vector>

namespace progressive {

// ==== UnifiedPush Connector ====
//
// UnifiedPush is a standard for push notifications that works without
// Google Play Services. It uses a local push distributor app (e.g. ntfy,
// NextPush, UP-FCM distributor) to receive notifications.
//
// Ref: https://unifiedpush.org/developers/spec/
//
// This module handles:
//   1. Discovery of available UnifiedPush distributors
//   2. Registration with a distributor (obtaining endpoint URL)
//   3. Configuring the Matrix pusher with the obtained endpoint
//   4. Receiving and decrypting push notifications

// ==== Distributor Discovery ====

struct UnifiedPushDistributor {
    std::string packageName;     // "io.heckel.ntfy"
    std::string displayName;     // "ntfy"
    std::string endpointUrl;     // URL for receiving push messages
    bool isAvailable = false;    // App is installed and responding
};

// Discover available UnifiedPush distributors on the device.
// On Android, this uses intents to query installed apps that support
// the org.unifiedpush.android.distributor action.
//
// Returns: list of available distributors (empty if none found).
std::vector<UnifiedPushDistributor> discoverUnifiedPushDistributors();

// ==== Registration ====

// Register with a UnifiedPush distributor to obtain an endpoint URL.
// The distributor returns a unique endpoint that the Matrix homeserver
// will POST push events to.
//
// Parameters:
//   connectorToken: opaque token identifying this app instance to the distributor
//
// Returns: endpoint URL (e.g. "https://ntfy.sh/abc123") or empty on failure.
std::string registerUnifiedPushEndpoint(
    const std::string& distributorPackage,
    const std::string& connectorToken
);

// Unregister from the distributor (called when user disables push).
bool unregisterUnifiedPushEndpoint(const std::string& endpointUrl);

// ==== Matrix Pusher Configuration ====

// Build the Matrix pusher JSON body for the HTTP Push API.
//
// POST /_matrix/client/r0/pushers/set
// Body:
//   {
//     "pushkey": "...",      // The UnifiedPush endpoint URL
//     "kind": "http",
//     "app_id": "chat.progressive.app",
//     "app_display_name": "Progressive Chat",
//     "device_display_name": "Android Device",
//     "lang": "en",
//     "data": {
//       "url": "...",         // Same as pushkey (legacy)
//       "format": "event_id_only"
//     },
//     "append": false
//   }

struct UnifiedPushPusherConfig {
    std::string pushKey;              // UnifiedPush endpoint URL
    std::string appId;                // "chat.progressive.app"
    std::string appDisplayName;       // "Progressive Chat"
    std::string deviceDisplayName;    // Device name for session list
    std::string lang;                 // "en", "ru", etc.
    std::string profileTag;           // Optional profile tag
    bool enabled = true;
};

std::string buildPusherJson(const UnifiedPushPusherConfig& config);

// Set the Matrix pusher (register with homeserver).
// Returns true if successfully registered.
bool setUnifiedPushPusher(const UnifiedPushPusherConfig& config,
                          const std::string& homeserverUrl,
                          const std::string& accessToken);

// ==== Message Decryption ====

// Parse a UnifiedPush message (received by the distributor and forwarded to
// the app via local broadcast). Extract the Matrix event data.
//
// UnifiedPush message format:
//   {
//     "message": {
//       "notification": {
//         "event_id": "$...",
//         "room_id": "!...",
//         "sender": "@...",
//         "content": {...},
//         "counts": {"unread": 3, "missed_calls": 0}
//       }
//     }
//   }

struct UnifiedPushMessage {
    std::string eventId;
    std::string roomId;
    std::string sender;
    std::string contentJson;           // Raw event content
    int unreadCount = 0;
    int missedCalls = 0;
    bool valid = false;
};

// Parse the raw UnifiedPush message JSON.
UnifiedPushMessage parseUnifiedPushMessage(const std::string& json);

// Check if a UnifiedPush message is a Matrix notification.
bool isMatrixNotification(const UnifiedPushMessage& msg);

// ==== Endpoint Management ====

// Store the current UnifiedPush endpoint locally (for recovery).
std::string serializePushState(const std::string& endpointUrl,
                               const std::string& connectorToken);
bool parsePushState(const std::string& state,
                    std::string& endpointUrl,
                    std::string& connectorToken);

} // namespace progressive
