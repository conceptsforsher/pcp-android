#include "progressive/widget_utils.hpp"
#include "progressive/json_parser.hpp"
#include <sstream>
#include <regex>
#include <algorithm>

namespace progressive {

WidgetInfo parseWidgetStateContent(const std::string& stateContentJson, const std::string& widgetId, const std::string& roomId) {
    WidgetInfo w;
    w.widgetId = widgetId;
    w.roomId = roomId;
    w.name = parseJsonStringValue(stateContentJson, "name");
    w.type = parseJsonStringValue(stateContentJson, "type");
    w.url  = parseJsonStringValue(stateContentJson, "url");
    w.creatorUserId = parseJsonStringValue(stateContentJson, "creatorUserId");
    w.avatarUrl = parseJsonStringValue(stateContentJson, "avatar_url");

    auto data = parseJsonStringValue(stateContentJson, "data");
    if (!data.empty()) {
        w.name = parseJsonStringValue("{" + data + "}", "title");
    }

    return w;
}

std::string buildWidgetContent(const WidgetInfo& widget) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };
    std::ostringstream json;
    json << "{";
    json << R"("id": ")" << esc(widget.widgetId) << R"(",)";
    json << R"("type": ")" << esc(widget.type) << R"(",)";
    json << R"("url": ")" << esc(widget.url) << R"(",)";
    json << R"("name": ")" << esc(widget.name) << R"(",)";
    json << R"("creatorUserId": ")" << esc(widget.creatorUserId) << R"(",)";
    json << R"("data": {)";
    json << R"("title": ")" << esc(widget.name) << R"(")";
    json << "}}";
    return json.str();
}

bool isValidWidgetUrl(const std::string& url) {
    return url.rfind("https://", 0) == 0 && url.size() > 8;
}

bool isJitsiWidget(const std::string& type) {
    return type == "m.jitsi" || type == "jitsi";
}

bool isEtherpadWidget(const std::string& type) {
    return type == "m.etherpad" || type == "etherpad";
}

std::string getWidgetTypeName(const std::string& type) {
    if (isJitsiWidget(type)) return "Video Conference";
    if (isEtherpadWidget(type)) return "Collaborative Document";
    if (type == "m.custom") return "Custom Widget";
    if (type == "m.stickerpicker") return "Sticker Picker";
    if (type == "m.calculator") return "Calculator";
    return type;
}

std::vector<WidgetInfo> listRoomWidgets(const std::string& stateEventsJson) {
    std::vector<WidgetInfo> widgets;
    // Each widget is stored as a state event with type "im.vector.modular.widgets"
    // The state key is the widget ID, the content has widget data

    size_t pos = 0;
    while (true) {
        pos = stateEventsJson.find("\"im.vector.modular.widgets\"", pos);
        if (pos == std::string::npos) break;

        auto objStart = stateEventsJson.rfind('{', pos);
        if (objStart == std::string::npos) break;

        int depth = 0;
        auto objEnd = objStart;
        while (objEnd < stateEventsJson.size()) {
            if (stateEventsJson[objEnd] == '{') ++depth;
            else if (stateEventsJson[objEnd] == '}') --depth;
            if (depth == 0) break;
            ++objEnd;
        }
        if (objEnd >= stateEventsJson.size()) break;

        std::string obj = stateEventsJson.substr(objStart, objEnd - objStart + 1);

        auto content = parseJsonStringValue(obj, "content");
        auto stateKey = parseJsonStringValue(obj, "state_key");

        if (!content.empty() && !stateKey.empty()) {
            WidgetInfo w;
            w.widgetId = stateKey;
            w.type = parseJsonStringValue("{" + content + "}", "type");
            w.url  = parseJsonStringValue("{" + content + "}", "url");
            w.name = parseJsonStringValue("{" + content + "}", "name");
            if (w.name.empty()) w.name = w.widgetId;
            widgets.push_back(w);
        }

        pos = objEnd + 1;
    }

    return widgets;
}

std::string widgetToJson(const WidgetInfo& widget) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"widgetId": ")" << esc(widget.widgetId) << R"(")";
    json << R"(,"name": ")" << esc(widget.name) << R"(")";
    json << R"(,"type": ")" << esc(widget.type) << R"(")";
    json << R"(,"typeName": ")" << esc(getWidgetTypeName(widget.type)) << R"(")";
    json << R"(,"url": ")" << esc(widget.url) << R"(")";
    json << "}";
    return json.str();
}

bool isReasonablePermission(const std::string& permission, const std::string& widgetType) {
    if (isJitsiWidget(widgetType)) {
        return permission == "camera" || permission == "microphone";
    }
    return false;
}

std::string formatPermissionRequest(const std::string& widgetName, const std::string& permission) {
    std::ostringstream out;
    out << "\"" << widgetName << "\" is requesting \"" << permission << "\" access.";
    return out.str();
}

} // namespace progressive
