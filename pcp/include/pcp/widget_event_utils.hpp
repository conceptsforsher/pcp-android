#include <nlohmann/json.hpp>
#pragma once
#include <string>
#include <vector>

namespace pcp {

struct WidgetEvent {
    std::string widgetId;
    std::string type;           // "m.custom", "jitsi", "etherpad"
    std::string name;
    std::string url;
    bool isActive = true;
};

// Parse widget from state event
WidgetEvent parseWidgetEvent(const std::string& stateKey, const std::string& json);

// Build widget API request for sending events
std::string buildWidgetApiRequest(const std::string& widgetId, const std::string& action,
                                    const std::string& data);

// Format widget name for room header
std::string formatWidgetBadge(const WidgetEvent& w);

// Check if widget type is supported
bool isWidgetTypeSupported(const std::string& type);

// Get widget icon from type
std::string getWidgetIcon(const std::string& type);

} // namespace pcp
