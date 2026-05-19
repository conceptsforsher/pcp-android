#include "progressive/widget_utils.hpp"
#include "progressive/json_parser.hpp"
#include <sstream>
#include <regex>
#include <algorithm>
#include <cctype>

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

// ==================================================================
// New: WidgetType conversions and helpers (ported from Kotlin)
// ==================================================================

const char* widgetTypePreferred(WidgetType wt) {
    switch (wt) {
        case WidgetType::JITSI:                return "m.jitsi";
        case WidgetType::ETHERPAD:             return "m.etherpad";
        case WidgetType::CUSTOM:               return "m.custom";
        case WidgetType::STICKERPICKER:        return "m.stickerpicker";
        case WidgetType::CALCULATOR:           return "m.calculator";
        case WidgetType::YOUTUBE:              return "m.youtube";
        case WidgetType::SPOTIFY:              return "m.spotify";
        case WidgetType::WHITEBOARD:           return "m.whiteboard";
        case WidgetType::DIAGRAM:              return "m.diagram";
        case WidgetType::GOOGLE_DOCS:          return "m.googledoc";
        case WidgetType::TRADINGVIEW:          return "m.tradingview";
        case WidgetType::VIDEO:                return "m.video";
        case WidgetType::GOOGLE_CALENDAR:      return "m.googlecalendar";
        case WidgetType::GRAFANA:              return "m.grafana";
        case WidgetType::INTEGRATION_MANAGER:  return "m.integration_manager";
        case WidgetType::ELEMENT_CALL:         return "io.element.call";
        default: return "unknown";
    }
}

const char* widgetTypeLegacy(WidgetType wt) {
    // Original Kotlin: most types use preferred == legacy; few have distinct legacy names
    switch (wt) {
        case WidgetType::JITSI:                return "jitsi";
        case WidgetType::ETHERPAD:             return "etherpad";
        default: return widgetTypePreferred(wt);
    }
}

WidgetType widgetTypeFromString(const std::string& s) {
    // Original Kotlin: WidgetType.fromString() — check DEFINED_TYPES first, then Fallback
    if (s == "m.jitsi" || s == "jitsi")                   return WidgetType::JITSI;
    if (s == "m.etherpad" || s == "etherpad")             return WidgetType::ETHERPAD;
    if (s == "m.custom")                                   return WidgetType::CUSTOM;
    if (s == "m.stickerpicker")                            return WidgetType::STICKERPICKER;
    if (s == "m.calculator")                               return WidgetType::CALCULATOR;
    if (s == "m.youtube")                                  return WidgetType::YOUTUBE;
    if (s == "m.spotify")                                  return WidgetType::SPOTIFY;
    if (s == "m.whiteboard")                               return WidgetType::WHITEBOARD;
    if (s == "m.diagram")                                  return WidgetType::DIAGRAM;
    if (s == "m.googledoc")                                return WidgetType::GOOGLE_DOCS;
    if (s == "m.tradingview" || s == "tradingview")        return WidgetType::TRADINGVIEW;
    if (s == "m.video")                                    return WidgetType::VIDEO;
    if (s == "m.googlecalendar")                           return WidgetType::GOOGLE_CALENDAR;
    if (s == "m.grafana")                                  return WidgetType::GRAFANA;
    if (s == "m.integration_manager")                      return WidgetType::INTEGRATION_MANAGER;
    if (s == "io.element.call")                            return WidgetType::ELEMENT_CALL;
    // Original Kotlin: return Fallback(preferred) — some type was given but not recognised
    if (!s.empty()) return WidgetType::FALLBACK;
    return WidgetType::UNKNOWN;
}

bool widgetTypeMatches(WidgetType wt, const std::string& type) {
    // Original Kotlin: WidgetType.matches(type) — type == preferred || type == legacy
    return type == widgetTypePreferred(wt) || type == widgetTypeLegacy(wt);
}

// ==================================================================
// New: Widget, WidgetEventContent, WidgetStateEventContent helpers
// ==================================================================

bool isActiveWidget(const WidgetEventContent& content) {
    // Original Kotlin: WidgetContent.isActive() — type != null && url != null
    return !content.type.empty() && !content.url.empty();
}

std::string getWidgetHumanName(const WidgetEventContent& content) {
    // Original Kotlin: WidgetContent.getHumanName() — name ?: type ?: ""
    if (!content.name.empty()) {
        std::string n = content.name;
        if (!n.empty()) n[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(n[0])));
        return n;
    }
    if (!content.type.empty()) {
        std::string n = content.type;
        if (!n.empty()) n[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(n[0])));
        return n;
    }
    return "";
}

std::string buildWidgetEventContent(const WidgetEventContent& content) {
    // Original Kotlin: serialise WidgetContent fields to JSON for m.widgets state event
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };
    std::ostringstream json;
    json << "{";
    json << R"("type":")" << esc(content.type) << R"(")";
    if (!content.name.empty())
        json << R"(,"name":")" << esc(content.name) << R"(")";
    json << R"(,"url":")" << esc(content.url) << R"(")";
    if (!content.creatorUserId.empty())
        json << R"(,"creatorUserId":")" << esc(content.creatorUserId) << R"(")";
    if (!content.id.empty())
        json << R"(,"id":")" << esc(content.id) << R"(")";
    if (!content.data.empty())
        json << R"(,"data":)" << content.data;
    else
        json << R"(,"data":{})";
    json << R"(,"waitForIframeLoad":)" << (content.waitForIframeLoad ? "true" : "false");
    json << "}";
    return json.str();
}

Widget parseWidget(const std::string& stateEventJson) {
    // Original Kotlin: parse a full state event (Event + WidgetContent) into a Widget
    Widget w;

    // Extract JSON string value helper
    auto extractStr = [](const std::string& json, const std::string& key) -> std::string {
        auto pp = json.find("\"" + key + "\"");
        if (pp == std::string::npos) return "";
        pp = json.find('"', pp + key.size() + 2);
        if (pp == std::string::npos) return "";
        pp++;
        size_t e = pp;
        while (e < json.size() && json[e] != '"') e++;
        return json.substr(pp, e - pp);
    };

    // Extract integer value helper
    auto extractInt = [](const std::string& json, const std::string& key) -> int {
        auto pp = json.find("\"" + key + "\"");
        if (pp == std::string::npos) return 0;
        pp = json.find(':', pp);
        if (pp == std::string::npos) return 0;
        pp++;
        while (pp < json.size() && (json[pp] == ' ' || json[pp] == '\t')) pp++;
        int v = 0;
        while (pp < json.size() && json[pp] >= '0' && json[pp] <= '9') {
            v = v * 10 + (json[pp] - '0'); pp++;
        }
        return v;
    };

    w.eventId = extractStr(stateEventJson, "event_id");
    w.roomId  = extractStr(stateEventJson, "room_id");
    w.widgetId = extractStr(stateEventJson, "state_key");

    // Extract content as a JSON object substring
    auto extractObj = [](const std::string& json, const std::string& key) -> std::string {
        auto pp = json.find("\"" + key + "\"");
        if (pp == std::string::npos) return "";
        pp = json.find('{', pp);
        if (pp == std::string::npos) return "";
        int depth = 1;
        size_t start = pp;
        pp++;
        while (pp < json.size() && depth > 0) {
            if (json[pp] == '{') depth++;
            else if (json[pp] == '}') depth--;
            pp++;
        }
        return json.substr(start, pp - start);
    };

    std::string contentJson = extractObj(stateEventJson, "content");
    if (!contentJson.empty()) {
        w.type   = extractStr(contentJson, "type");
        w.url    = extractStr(contentJson, "url");
        w.rawUrl = w.url;                          // saved before template expansion
        w.name   = extractStr(contentJson, "name");
        w.creatorUserId = extractStr(contentJson, "creatorUserId");
        w.avatarUrl     = extractStr(contentJson, "avatar_url");
        w.data          = extractObj(contentJson, "data");
        w.waitForIframeLoad = contentJson.find("\"waitForIframeLoad\":true") != std::string::npos;
    }

    w.widgetTypeEnum = widgetTypeFromString(w.type);
    w.isActive = isActiveWidget({w.type, w.name, w.url, w.data, w.creatorUserId, w.widgetId, w.waitForIframeLoad});

    return w;
}

WidgetEventContent parseWidgetEventContent(const std::string& contentJson) {
    // Original Kotlin: parse WidgetContent from content JSON
    WidgetEventContent c;

    auto extractStr = [](const std::string& json, const std::string& key) -> std::string {
        auto pp = json.find("\"" + key + "\"");
        if (pp == std::string::npos) return "";
        pp = json.find('"', pp + key.size() + 2);
        if (pp == std::string::npos) return "";
        pp++;
        size_t e = pp;
        while (e < json.size() && json[e] != '"') e++;
        return json.substr(pp, e - pp);
    };

    auto extractObj = [](const std::string& json, const std::string& key) -> std::string {
        auto pp = json.find("\"" + key + "\"");
        if (pp == std::string::npos) return "";
        pp = json.find('{', pp);
        if (pp == std::string::npos) return "";
        int depth = 1;
        size_t start = pp;
        pp++;
        while (pp < json.size() && depth > 0) {
            if (json[pp] == '{') depth++;
            else if (json[pp] == '}') depth--;
            pp++;
        }
        return json.substr(start, pp - start);
    };

    c.type          = extractStr(contentJson, "type");
    c.name          = extractStr(contentJson, "name");
    c.url           = extractStr(contentJson, "url");
    c.creatorUserId = extractStr(contentJson, "creatorUserId");
    c.id            = extractStr(contentJson, "id");
    c.data          = extractObj(contentJson, "data");
    c.waitForIframeLoad = contentJson.find("\"waitForIframeLoad\":true") != std::string::npos;

    return c;
}

WidgetType getWidgetTypeFromUrl(const std::string& url, const std::string& data) {
    // Original Kotlin: detect WidgetType from URL pattern or type data string

    // If data contains a known type string, use that first
    if (!data.empty()) {
        auto wt = widgetTypeFromString(data);
        if (wt != WidgetType::UNKNOWN && wt != WidgetType::FALLBACK) return wt;
    }

    // URL pattern matching against known widget providers
    std::string lower = url;
    std::transform(lower.begin(), lower.end(), lower.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });

    if (lower.find("jitsi") != std::string::npos)              return WidgetType::JITSI;
    if (lower.find("etherpad") != std::string::npos)           return WidgetType::ETHERPAD;
    if (lower.find("docs.google.com") != std::string::npos)    return WidgetType::GOOGLE_DOCS;
    if (lower.find("calendar.google.com") != std::string::npos)return WidgetType::GOOGLE_CALENDAR;
    if (lower.find("spotify") != std::string::npos)            return WidgetType::SPOTIFY;
    if (lower.find("youtube") != std::string::npos)            return WidgetType::YOUTUBE;
    if (lower.find("grafana") != std::string::npos)            return WidgetType::GRAFANA;
    if (lower.find("tradingview") != std::string::npos)        return WidgetType::TRADINGVIEW;
    if (lower.find("whiteboard") != std::string::npos)         return WidgetType::WHITEBOARD;
    if (lower.find("scalar.vector.im") != std::string::npos)   return WidgetType::INTEGRATION_MANAGER;
    if (lower.find("scalar") != std::string::npos)             return WidgetType::INTEGRATION_MANAGER;
    if (lower.find("call.element.io") != std::string::npos)    return WidgetType::ELEMENT_CALL;

    return WidgetType::FALLBACK;
}

} // namespace progressive
