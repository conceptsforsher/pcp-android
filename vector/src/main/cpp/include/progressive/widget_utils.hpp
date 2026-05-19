#pragma once

#include <string>
#include <vector>

namespace progressive {

// ================================================================
// WidgetType enum (Original Kotlin: WidgetType sealed class)
// Ref: https://github.com/matrix-org/matrix-doc/issues/1236
//
// Each widget type has a preferred Matrix type string and an optional
// legacy name.  Values 0-10 match the original widget_manager WidgetType
// to keep existing tests and JNI bridge working.
// ================================================================

enum class WidgetType {
    UNKNOWN = 0,
    JITSI = 1,               // Original Kotlin: Jitsi  — preferred="m.jitsi",     legacy="jitsi"
    ETHERPAD = 2,            // Original Kotlin: Etherpad — preferred="m.etherpad",  legacy="etherpad"
    CUSTOM = 3,              // Original Kotlin: Custom  — preferred="m.custom"
    STICKERPICKER = 4,       // Original Kotlin: StickerPicker — preferred="m.stickerpicker"
    CALCULATOR = 5,          // "m.calculator"
    YOUTUBE = 6,             // "m.youtube"
    SPOTIFY = 7,             // Original Kotlin: Spotify — preferred="m.spotify"
    WHITEBOARD = 8,          // "m.whiteboard"
    DIAGRAM = 9,             // "m.diagram"
    GOOGLE_DOCS = 10,        // Original Kotlin: GoogleDoc — preferred="m.googledoc"
    TRADINGVIEW = 11,        // Original Kotlin: TradingView — preferred="m.tradingview"
    VIDEO = 12,              // Original Kotlin: Video — preferred="m.video"
    GOOGLE_CALENDAR = 13,    // Original Kotlin: GoogleCalendar — preferred="m.googlecalendar"
    GRAFANA = 14,            // Original Kotlin: Grafana — preferred="m.grafana"
    INTEGRATION_MANAGER = 15,// Original Kotlin: IntegrationManager — preferred="m.integration_manager"
    ELEMENT_CALL = 16,       // Original Kotlin: ElementCall — preferred="io.element.call"
    FALLBACK = 99            // Original Kotlin: Fallback(preferred)
};

// Original Kotlin: WidgetType.preferred
const char* widgetTypePreferred(WidgetType wt);

// Original Kotlin: WidgetType.legacy
const char* widgetTypeLegacy(WidgetType wt);

// Original Kotlin: WidgetType.fromString()
WidgetType widgetTypeFromString(const std::string& s);

// Original Kotlin: WidgetType.matches()
bool widgetTypeMatches(WidgetType wt, const std::string& type);

// ---- Original WidgetInfo model (backward compat) ----

struct WidgetInfo {
    std::string widgetId;
    std::string name;
    std::string type;          // "m.custom", "m.jitsi", "m.ethan", etc.
    std::string url;
    std::string creatorUserId;
    std::string roomId;
    std::string avatarUrl;
    bool waitForIframeLoad = false;
    int width = 0;
    int height = 0;
};

// Parse widget data from m.widget state event content.
WidgetInfo parseWidgetStateContent(const std::string& stateContentJson, const std::string& widgetId, const std::string& roomId);

// Build m.widget state content JSON for creating/updating a widget.
std::string buildWidgetContent(const WidgetInfo& widget);

// Validate a widget URL (must be https).
bool isValidWidgetUrl(const std::string& url);

// Check if a widget type is a Jitsi conference.
bool isJitsiWidget(const std::string& type);

// Check if a widget type is an Etherpad.
bool isEtherpadWidget(const std::string& type);

// Get a human-readable widget type name.
std::string getWidgetTypeName(const std::string& type);

// List all widgets for a room from state events.
std::vector<WidgetInfo> listRoomWidgets(const std::string& stateEventsJson);

// Format widget info as JSON.
std::string widgetToJson(const WidgetInfo& widget);

// ---- Widget Permission Model ----

struct WidgetPermission {
    std::string roomId;
    std::string widgetId;
    std::string permission;    // "camera", "microphone", "screen_sharing"
    bool granted = false;
};

// Check if a widget permission request is reasonable.
bool isReasonablePermission(const std::string& permission, const std::string& widgetType);

// Format permission request for display.
std::string formatPermissionRequest(const std::string& widgetName, const std::string& permission);

// ================================================================
// New expanded widget models (ported from Kotlin SDK)
// ================================================================

// ---- Widget struct (Original Kotlin: Widget data class) ----
// Combines state event metadata with WidgetContent fields.

struct Widget {
    std::string widgetId;          // Original Kotlin: widgetId
    std::string type;              // Original Kotlin: widgetContent.type
    WidgetType widgetTypeEnum;     // Original Kotlin: type  (WidgetType)
    std::string name;              // Original Kotlin: widgetContent.getHumanName()
    std::string url;               // Original Kotlin: widgetContent.url  (templated)
    std::string rawUrl;            // original URL before template expansion
    std::string creatorUserId;     // Original Kotlin: widgetContent.creatorUserId
    std::string avatarUrl;
    std::string roomId;
    std::string eventId;           // Original Kotlin: event.eventId
    std::string data;              // Original Kotlin: widgetContent.data  (JSON dict)
    bool waitForIframeLoad = false;// Original Kotlin: widgetContent.waitForIframeLoad
    bool isActive = false;         // Original Kotlin: widget.isActive
};

// ---- WidgetEventContent (Original Kotlin: WidgetContent) ----
// Ref: https://github.com/matrix-org/matrix-doc/issues/1236

struct WidgetEventContent {
    std::string type;              // Original Kotlin: @Json(name = "type")
    std::string name;              // Original Kotlin: @Json(name = "name")
    std::string url;               // Original Kotlin: @Json(name = "url")
    std::string data;              // Original Kotlin: data: JsonDict = emptyMap()
    std::string creatorUserId;     // Original Kotlin: @Json(name = "creatorUserId")
    std::string id;                // Original Kotlin: @Json(name = "id")
    bool waitForIframeLoad = false;// Original Kotlin: @Json(name = "waitForIframeLoad")
};

// Original Kotlin: WidgetContent.isActive()  — type && url both non-null
bool isActiveWidget(const WidgetEventContent& content);

// Original Kotlin: WidgetContent.getHumanName()
std::string getWidgetHumanName(const WidgetEventContent& content);

// ---- WidgetStateEventContent ----
// Full m.widgets state event wrapper.

struct WidgetStateEventContent {
    std::string eventId;
    std::string type;              // "im.vector.modular.widgets"
    std::string stateKey;          // widget ID  (state key)
    WidgetEventContent content;    // parsed content
    std::string roomId;
    int64_t originServerTs = 0;
};

// ---- JSON builders and parsers ----

// Original Kotlin: serialise WidgetContent to JSON for state event content.
std::string buildWidgetEventContent(const WidgetEventContent& content);

// Original Kotlin: parse a full state event into a Widget.
Widget parseWidget(const std::string& stateEventJson);

// Parse WidgetEventContent from content JSON string.
WidgetEventContent parseWidgetEventContent(const std::string& contentJson);

// Original Kotlin: detect WidgetType from URL pattern or type string.
// Matches common widget URLs against known providers.
WidgetType getWidgetTypeFromUrl(const std::string& url, const std::string& data = "");

} // namespace progressive
