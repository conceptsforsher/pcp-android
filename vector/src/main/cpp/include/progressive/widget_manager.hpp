#pragma once

#include <string>
#include <vector>
#include <map>
#include <set>
#include <cstdint>
#include "progressive/widget_utils.hpp"

namespace progressive {

// ================================================================
// Widget Manager — full Matrix widget lifecycle management
//
// Inspired by Element Web's WidgetManager (matrix-react-sdk/src/widgets/)
// and Matrix Widget spec (MSC1236, MSC2762, MSC2774).
//
// Covers:
//   1. Widget URL templating ($matrix_* variables)
//   2. Widget sandbox security policy (allowed/blocked URLs)
//   3. Widget capability negotiation (MSC2762)
//   4. Widget lifecycle (persist, remove, resize, setAlwaysOnScreen)
//   5. Widget-to-client postMessage bridge format
//   6. Sticker picker, Jitsi, Etherpad, custom widget support
// ================================================================

// ---- Widget Capability (MSC2762) ----

enum class WidgetCapability {
    SCREENSHARE = 0,      // m.capability.screenshare
    STICKER = 1,          // m.sticker
    CAMERA = 2,           // m.capability.camera
    MICROPHONE = 3,       // m.capability.microphone
    LOCATION = 4,         // m.capability.location
    ALWAYS_ON_SCREEN = 5, // m.always_on_screen
    WIDGET_PIN = 6,       // m.widget_pin
    CUSTOM = 7            // anything else
};

const char* capabilityToString(WidgetCapability cap);
WidgetCapability capabilityFromString(const std::string& s);

// ---- Widget Capability Request/Approval ----

struct WidgetCapabilityRequest {
    std::string widgetId;
    std::string widgetName;
    WidgetCapability capability;
    bool approved = false;
    bool denied = false;
    std::string reason;              // Why approved/denied
};

// Request capability from user. Returns formatted prompt string.
std::string formatCapabilityRequest(const WidgetCapabilityRequest& req);

// Auto-approve capabilities that don't need user consent.
bool isAutoApprovedCapability(WidgetCapability cap, const std::string& widgetType);

// ---- Widget Capabilities set (from widget_utils WidgetType) ----
using WidgetCapabilities = std::set<WidgetCapability>;

// ---- WidgetPermission enum (Original Kotlin: API-scoped permissions) ----
// Managed by IntegrationManagerService; controls widget access to server APIs.

enum class WidgetApiPermission {
    SCALAR_TOKEN = 0,      // can use the scalar token for authenticated API calls
    ROOM_VISIBILITY = 1,   // can access room visibility state
    MEMBERSHIP = 2,        // can access room membership data
    IDENTITY = 3,          // can access user identity / profile data
    SEND_EVENT = 4         // can send room events via widget API
};

const char* widgetApiPermissionToString(WidgetApiPermission p);
WidgetApiPermission widgetApiPermissionFromString(const std::string& s);

// ---- Integration Manager Models (Original Kotlin: IntegrationManagerConfig) ----

struct IntegrationManagerConfig {
    std::string apiUrl;        // Original Kotlin: restUrl
    std::string uiUrl;         // Original Kotlin: uiUrl
    std::string kind;          // Original Kotlin: Kind enum — "ACCOUNT", "HOMESERVER", "DEFAULT"
};

struct ScalarToken {
    std::string token;         // scalar token for widget API authentication
};

struct WidgetOpenIDToken {
    std::string accessToken;   // OpenID access token for widget→homeserver auth
    std::string tokenType;     // e.g. "Bearer"
    std::string matrixServerName;
    int64_t expiresIn = 0;     // seconds until token expiry
};

// ---- Integration Manager Functions ----

// Original Kotlin: serialise IntegrationManagerConfig to JSON.
std::string buildIntegrationManagerConfig(const IntegrationManagerConfig& config);

// Original Kotlin: parse IntegrationManagerConfig from JSON.
IntegrationManagerConfig parseIntegrationManagerConfig(const std::string& json);

// ---- Widget Action / Permission Functions ----

// Original Kotlin: build a widget action request (toWidget / fromWidget postMessage).
// action examples: "visibility", "capability_request", "room_membership"
std::string buildWidgetActionRequest(const std::string& widgetId,
                                     const std::string& action,
                                     const std::string& data);

// Original Kotlin: validate that a widget type is allowed a given API permission.
bool validateWidgetPermission(const std::string& widgetId,
                              WidgetApiPermission perm,
                              const std::string& widgetType);

// Build a complete m.widgets state event JSON (type + content).
// Uses WidgetEventContent from widget_utils.
std::string buildWidgetStateEvent(const std::string& widgetId,
                                  const WidgetEventContent& content);

// Parse all WidgetStateEventContent from room state JSON array.
std::vector<WidgetStateEventContent> parseWidgetStateEvents(const std::string& stateEventsJson);

// ---- URL Template Variables ----
//
// Widget URLs can contain $matrix_* variables that are replaced at runtime.
//   $matrix_user_id      → @alice:example.org
//   $matrix_room_id      → !room:example.org
//   $matrix_widget_id    → widget_123
//   $matrix_display_name → Alice
//   $matrix_avatar_url   → mxc://example.org/avatar
//   $matrix_client_id    → chat.progressive.app
//   $matrix_client_theme → light / dark
//   $org.matrix.msc2873.client_id → chat.progressive.app (legacy)
//   $org.matrix.msc2873.client_theme → light / dark (legacy)

struct WidgetUrlTemplate {
    std::string userId;          // $matrix_user_id
    std::string roomId;          // $matrix_room_id
    std::string widgetId;        // $matrix_widget_id
    std::string displayName;     // $matrix_display_name
    std::string avatarUrl;       // $matrix_avatar_url
    std::string clientId;        // $matrix_client_id
    std::string clientTheme;     // $matrix_client_theme
    std::string clientLanguage;  // $matrix_client_language (optional)
};

// Replace all $matrix_* template variables in a widget URL.
std::string applyWidgetUrlTemplate(const std::string& url, const WidgetUrlTemplate& tpl);

// ---- Widget Sandbox Security Policy ----
//
// Inspired by Element Web's StopGapWidgetDriver:
//   - Blocked URL patterns (by domain or scheme)
//   - Allowed protocols (https only by default)
//   - Same-origin enforcement
//   - Content security policy headers

struct WidgetSecurityPolicy {
    std::set<std::string> blockedDomains;    // e.g. "evil.example.com"
    std::set<std::string> allowedSchemes;    // default: {"https"}
    bool enforceSameOrigin = true;           // block cross-origin widgets
    bool allowDataUrls = false;              // block data: URL widgets
    bool allowBlobUrls = false;              // block blob: URL widgets
    bool allowCustomProtocols = false;       // block custom:// protocol widgets
    int maxUrlLength = 2048;                 // maximum widget URL length
    std::string contentSecurityPolicy;       // CSP header value
};

// Default security policy (Element Web-compatible).
WidgetSecurityPolicy defaultWidgetSecurityPolicy();

// Check if a widget URL passes the security policy.
// Returns true if the URL is allowed; false if blocked.
// Sets `reason` to explain why it was blocked (if blocked).
bool validateWidgetSecurity(const std::string& url, const WidgetSecurityPolicy& policy,
                            std::string& reason);

// Get a sanitized CSP header value for embedding the widget.
std::string buildWidgetContentSecurityPolicy(const WidgetSecurityPolicy& policy,
                                              const std::string& widgetUrl);

// ---- Widget Type Classification ----
// Forward to widget_utils WidgetType; kept for backward compat.

WidgetType classifyWidgetType(const std::string& type);

// ---- Widget Info (extended) ----

struct WidgetEntry {
    std::string widgetId;
    std::string name;
    std::string type;              // "m.custom", "m.jitsi", etc.
    std::string url;
    std::string creatorUserId;
    std::string roomId;
    std::string avatarUrl;
    bool waitForIframeLoad = false;
    int width = 0;
    int height = 0;

    // Extended fields (Widget Manager additions)
    bool isPinned = false;         // Always on screen (not scrollable)
    bool isMinimized = false;      // Shown as PiP
    bool isMaximized = false;      // Fullscreen
    int displayOrder = 0;          // Sort order in room
    std::vector<WidgetCapability> approvedCapabilities;
    std::string sanitizedUrl;      // After template expansion + sanitization
    int64_t createdAt = 0;         // Epoch millis
    std::string etag;              // For cache invalidation
};

// ---- Widget Manager ----

class WidgetManager {
public:
    // Create a widget manager for a specific room/user combination.
    WidgetManager(const std::string& roomId, const std::string& userId,
                  const std::string& displayName, const std::string& avatarUrl);

    // Set the security policy (defaults to defaultWidgetSecurityPolicy()).
    void setSecurityPolicy(const WidgetSecurityPolicy& policy);

    // Load all widgets from room state events JSON.
    std::vector<WidgetEntry> loadWidgets(const std::string& stateEventsJson);

    // Create a new widget. Returns the widget JSON to send as state event.
    // Validates URL, applies template, checks security policy.
    // Sets `error` on failure.
    std::string createWidget(const std::string& widgetId, const std::string& type,
                             const std::string& url, const std::string& name,
                             bool waitForIframeLoad, std::string& error);

    // Remove a widget. Returns the state key to delete.
    std::string removeWidget(const std::string& widgetId);

    // Pin/unpin a widget (setAlwaysOnScreen).
    // Pinned widgets stay visible while scrolling.
    std::string setWidgetPinned(const std::string& widgetId, bool pinned, std::string& error);

    // Resize a widget.
    std::string resizeWidget(const std::string& widgetId, int width, int height, std::string& error);

    // Minimize/maximize/restore a widget.
    std::string setWidgetMinimized(const std::string& widgetId, bool minimized);
    std::string setWidgetMaximized(const std::string& widgetId, bool maximized);

    // Request a capability for a widget. Returns the formatted request.
    // The caller must get user consent before approving.
    std::string requestCapability(const std::string& widgetId, WidgetCapability cap,
                                  std::string& error);

    // Approve a capability request.
    std::string approveCapability(const std::string& widgetId, WidgetCapability cap);

    // Deny a capability request.
    std::string denyCapability(const std::string& widgetId, WidgetCapability cap);

    // Get the expanded widget URL (with template variables replaced).
    std::string getWidgetUrl(const std::string& widgetId, std::string& error);

    // Format a postMessage for widget→client communication.
    // Request types: "action", "content_loaded", "capability_request"
    std::string buildWidgetPostMessage(const std::string& widgetId,
                                       const std::string& action,
                                       const std::string& data);

    // Parse a postMessage from widget to client.
    // Returns the parsed action type and data payload.
    std::string parseWidgetPostMessage(const std::string& message,
                                       std::string& action, std::string& widgetId,
                                       std::string& data);

    // Check if a widget type supports PiP (Picture-in-Picture).
    bool supportsPiP(const std::string& widgetId) const;

    // Get all widgets of a specific type.
    std::vector<WidgetEntry> getWidgetsByType(const std::string& type) const;

    // Get a specific widget.
    bool getWidget(const std::string& widgetId, WidgetEntry& out) const;

    // Count total widgets.
    int widgetCount() const { return static_cast<int>(widgets_.size()); }

    // Serialize all managed widgets to JSON array.
    std::string widgetsToJson() const;

    // Get the sanitized CSP header for all widgets.
    std::string buildGlobalCsp() const;

private:
    std::string roomId_;
    std::string userId_;
    std::string displayName_;
    std::string avatarUrl_;
    WidgetSecurityPolicy policy_;
    std::vector<WidgetEntry> widgets_;

    // Quick lookup by widget ID
    WidgetEntry* findWidget(const std::string& widgetId);
    const WidgetEntry* findWidget(const std::string& widgetId) const;

    // Apply templating + security to a widget URL.
    std::string sanitizeWidgetUrl(const std::string& url, std::string& error);

    // Build JSON for an m.widget state event.
    std::string buildWidgetStateJson(const WidgetEntry& widget) const;

    // Build template variables from manager state.
    WidgetUrlTemplate buildTemplate() const;
};

} // namespace progressive
