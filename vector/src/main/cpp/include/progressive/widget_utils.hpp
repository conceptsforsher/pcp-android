#ifndef PROGRESSIVE_WIDGET_UTILS_HPP
#define PROGRESSIVE_WIDGET_UTILS_HPP

#include <string>
#include <vector>

namespace progressive {

// ---- Matrix Widget Utilities ----

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

} // namespace progressive

#endif // PROGRESSIVE_WIDGET_UTILS_HPP
