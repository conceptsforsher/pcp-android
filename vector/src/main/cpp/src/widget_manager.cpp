#include "progressive/widget_manager.hpp"
#include "progressive/widget_utils.hpp"
#include <sstream>
#include <algorithm>
#include <regex>
#include <ctime>

namespace progressive {

// ---- Capability to/from string ----

const char* capabilityToString(WidgetCapability cap) {
    switch (cap) {
        case WidgetCapability::SCREENSHARE: return "m.capability.screenshare";
        case WidgetCapability::STICKER: return "m.sticker";
        case WidgetCapability::CAMERA: return "m.capability.camera";
        case WidgetCapability::MICROPHONE: return "m.capability.microphone";
        case WidgetCapability::LOCATION: return "m.capability.location";
        case WidgetCapability::ALWAYS_ON_SCREEN: return "m.always_on_screen";
        case WidgetCapability::WIDGET_PIN: return "m.widget_pin";
        case WidgetCapability::CUSTOM: return "m.capability.custom";
    }
    return "unknown";
}

WidgetCapability capabilityFromString(const std::string& s) {
    if (s == "m.capability.screenshare") return WidgetCapability::SCREENSHARE;
    if (s == "m.sticker") return WidgetCapability::STICKER;
    if (s == "m.capability.camera") return WidgetCapability::CAMERA;
    if (s == "m.capability.microphone") return WidgetCapability::MICROPHONE;
    if (s == "m.capability.location") return WidgetCapability::LOCATION;
    if (s == "m.always_on_screen") return WidgetCapability::ALWAYS_ON_SCREEN;
    if (s == "m.widget_pin") return WidgetCapability::WIDGET_PIN;
    return WidgetCapability::CUSTOM;
}

std::string formatCapabilityRequest(const WidgetCapabilityRequest& req) {
    std::ostringstream out;
    out << R"({"widgetId":")" << req.widgetId
        << R"(","widgetName":")" << req.widgetName
        << R"(","capability":")" << capabilityToString(req.capability)
        << R"(","approved":)" << (req.approved ? "true" : "false")
        << R"(,"denied":)" << (req.denied ? "true" : "false");
    if (!req.reason.empty()) out << R"(,"reason":")" << req.reason << "\"";
    out << "}";
    return out.str();
}

bool isAutoApprovedCapability(WidgetCapability cap, const std::string& widgetType) {
    // Sticker picker widgets can always use stickers
    if (widgetType == "m.stickerpicker" && cap == WidgetCapability::STICKER) return true;
    // Jitsi widgets auto-get camera/microphone/screenshare
    if ((widgetType == "m.jitsi" || widgetType == "jitsi") &&
        (cap == WidgetCapability::CAMERA || cap == WidgetCapability::MICROPHONE ||
         cap == WidgetCapability::SCREENSHARE)) return true;
    // Always on screen is auto-approved for all widgets
    if (cap == WidgetCapability::ALWAYS_ON_SCREEN) return true;
    return false;
}

// ---- URL Template Expansion ----

std::string applyWidgetUrlTemplate(const std::string& url, const WidgetUrlTemplate& tpl) {
    std::string result = url;

    // Matrix spec variables
    auto replace = [&](const std::string& var, const std::string& val) {
        size_t pos = 0;
        while ((pos = result.find(var, pos)) != std::string::npos) {
            result.replace(pos, var.size(), val);
            pos += val.size();
        }
    };

    replace("$matrix_user_id", tpl.userId);
    replace("$matrix_room_id", tpl.roomId);
    replace("$matrix_widget_id", tpl.widgetId);
    replace("$matrix_display_name", tpl.displayName);
    replace("$matrix_avatar_url", tpl.avatarUrl);
    replace("$matrix_client_id", tpl.clientId);
    replace("$matrix_client_theme", tpl.clientTheme);
    if (!tpl.clientLanguage.empty()) {
        replace("$matrix_client_language", tpl.clientLanguage);
    }

    // Legacy MSC2873 identifiers
    replace("$org.matrix.msc2873.client_id", tpl.clientId);
    replace("$org.matrix.msc2873.client_theme", tpl.clientTheme);

    return result;
}

// ---- Security Policy ----

WidgetSecurityPolicy defaultWidgetSecurityPolicy() {
    WidgetSecurityPolicy p;
    p.allowedSchemes = {"https"};
    p.enforceSameOrigin = true;
    p.allowDataUrls = false;
    p.allowBlobUrls = false;
    p.allowCustomProtocols = false;
    p.maxUrlLength = 2048;
    p.contentSecurityPolicy =
        "default-src 'none'; "
        "script-src 'self' 'unsafe-inline' 'unsafe-eval'; "
        "style-src 'self' 'unsafe-inline'; "
        "img-src https: data:; "
        "media-src https:; "
        "connect-src https:; "
        "frame-src https:; "
        "font-src 'self'; "
        "sandbox allow-forms allow-popups allow-scripts allow-same-origin;";
    return p;
}

static bool isHostBlocked(const std::string& host, const std::set<std::string>& blocked) {
    for (const auto& d : blocked) {
        if (host == d || (host.size() > d.size() && host.compare(host.size() - d.size(), d.size(), d) == 0 &&
                          (host.size() == d.size() || host[host.size() - d.size() - 1] == '.'))) {
            return true;
        }
    }
    return false;
}

bool validateWidgetSecurity(const std::string& url, const WidgetSecurityPolicy& policy,
                            std::string& reason) {
    if (url.empty()) {
        reason = "Empty widget URL";
        return false;
    }

    if (url.size() > static_cast<size_t>(policy.maxUrlLength)) {
        reason = "Widget URL exceeds maximum length of " + std::to_string(policy.maxUrlLength);
        return false;
    }

    // Check for data: URLs
    if (!policy.allowDataUrls && url.compare(0, 5, "data:") == 0) {
        reason = "data: URLs are not allowed for widgets";
        return false;
    }

    // Check for blob: URLs
    if (!policy.allowBlobUrls && url.compare(0, 5, "blob:") == 0) {
        reason = "blob: URLs are not allowed for widgets";
        return false;
    }

    // Extract scheme
    size_t schemeEnd = url.find("://");
    if (schemeEnd == std::string::npos) {
        if (!policy.allowCustomProtocols) {
            reason = "Widget URL must use https:// protocol";
            return false;
        }
        return true; // custom protocol, allowed
    }

    std::string scheme = url.substr(0, schemeEnd);
    if (policy.allowedSchemes.find(scheme) == policy.allowedSchemes.end()) {
        reason = "Widget URL scheme '" + scheme + "' is not allowed";
        return false;
    }

    // Extract host
    size_t hostStart = schemeEnd + 3;
    size_t hostEnd = url.find('/', hostStart);
    if (hostEnd == std::string::npos) hostEnd = url.size();
    std::string host = url.substr(hostStart, hostEnd - hostStart);

    // Remove port if present
    size_t portPos = host.find(':');
    if (portPos != std::string::npos) host = host.substr(0, portPos);

    // Check blocked domains
    if (isHostBlocked(host, policy.blockedDomains)) {
        reason = "Widget domain '" + host + "' is blocked";
        return false;
    }

    return true;
}

std::string buildWidgetContentSecurityPolicy(const WidgetSecurityPolicy& policy,
                                              const std::string& widgetUrl) {
    std::string csp = policy.contentSecurityPolicy;
    if (!widgetUrl.empty()) {
        // Extract origin from widget URL
        size_t schemeEnd = widgetUrl.find("://");
        if (schemeEnd != std::string::npos) {
            size_t hostStart = schemeEnd + 3;
            size_t hostEnd = widgetUrl.find('/', hostStart);
            if (hostEnd == std::string::npos) hostEnd = widgetUrl.size();
            std::string origin = widgetUrl.substr(0, hostEnd);
            // Add widget origin to frame-src
            size_t framePos = csp.find("frame-src");
            if (framePos != std::string::npos) {
                csp.insert(framePos + 9, " " + origin);
            }
        }
    }
    return csp;
}

// ---- Widget Type Classification ----

WidgetType classifyWidgetType(const std::string& type) {
    if (type == "m.jitsi" || type == "jitsi") return WidgetType::JITSI;
    if (type == "m.etherpad" || type == "etherpad") return WidgetType::ETHERPAD;
    if (type == "m.custom") return WidgetType::CUSTOM;
    if (type == "m.stickerpicker") return WidgetType::STICKERPICKER;
    if (type == "m.calculator") return WidgetType::CALCULATOR;
    if (type == "m.youtube") return WidgetType::YOUTUBE;
    if (type == "m.spotify") return WidgetType::SPOTIFY;
    if (type == "m.whiteboard") return WidgetType::WHITEBOARD;
    if (type == "m.diagram") return WidgetType::DIAGRAM;
    if (type == "m.google_docs") return WidgetType::GOOGLE_DOCS;
    return WidgetType::UNKNOWN;
}

// ---- Widget Manager Implementation ----

WidgetManager::WidgetManager(const std::string& roomId, const std::string& userId,
                             const std::string& displayName, const std::string& avatarUrl)
    : roomId_(roomId), userId_(userId), displayName_(displayName), avatarUrl_(avatarUrl),
      policy_(defaultWidgetSecurityPolicy()) {}

void WidgetManager::setSecurityPolicy(const WidgetSecurityPolicy& policy) {
    policy_ = policy;
}

WidgetEntry* WidgetManager::findWidget(const std::string& widgetId) {
    for (auto& w : widgets_) {
        if (w.widgetId == widgetId) return &w;
    }
    return nullptr;
}

const WidgetEntry* WidgetManager::findWidget(const std::string& widgetId) const {
    for (const auto& w : widgets_) {
        if (w.widgetId == widgetId) return &w;
    }
    return nullptr;
}

WidgetUrlTemplate WidgetManager::buildTemplate() const {
    WidgetUrlTemplate t;
    t.userId = userId_;
    t.roomId = roomId_;
    t.displayName = displayName_;
    t.avatarUrl = avatarUrl_;
    t.clientId = "chat.progressive.app";
    t.clientTheme = "light";
    return t;
}

std::string WidgetManager::sanitizeWidgetUrl(const std::string& url, std::string& error) {
    auto tpl = buildTemplate();
    std::string expanded = applyWidgetUrlTemplate(url, tpl);

    std::string reason;
    if (!validateWidgetSecurity(expanded, policy_, reason)) {
        error = "Security check failed: " + reason;
        return "";
    }

    return expanded;
}

std::string WidgetManager::buildWidgetStateJson(const WidgetEntry& widget) const {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    std::ostringstream json;
    json << "{";
    json << R"("id":")" << esc(widget.widgetId) << R"(",)";
    json << R"("type":")" << esc(widget.type) << R"(",)";
    json << R"("url":")" << esc(widget.sanitizedUrl.empty() ? widget.url : widget.sanitizedUrl) << R"(",)";
    json << R"("name":")" << esc(widget.name) << R"(",)";
    if (!widget.creatorUserId.empty()) json << R"("creatorUserId":")" << esc(widget.creatorUserId) << R"(",)";
    json << R"("waitForIframeLoad":)" << (widget.waitForIframeLoad ? "true" : "false");
    if (widget.isPinned) json << R"(,"alwaysOnScreen":true)";
    if (widget.width > 0) json << R"(,"width":)" << widget.width;
    if (widget.height > 0) json << R"(,"height":)" << widget.height;

    // Capabilities
    if (!widget.approvedCapabilities.empty()) {
        json << R"(,"capabilities":[)";
        bool first = true;
        for (auto cap : widget.approvedCapabilities) {
            if (!first) json << ","; first = false;
            json << "\"" << capabilityToString(cap) << "\"";
        }
        json << "]";
    }

    json << R"(,"data":{"title":")" << esc(widget.name) << R"("})";
    return json.str();
}

std::vector<WidgetEntry> WidgetManager::loadWidgets(const std::string& stateEventsJson) {
    widgets_.clear();

    // Parse state events looking for im.vector.modular.widgets
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

        auto stateKey = extractStr(obj, "state_key");
        auto content = extractStr(obj, "content");
        if (content.empty() || stateKey.empty()) {
            pos = objEnd + 1;
            continue;
        }

        // Content might be double-encoded — try to parse as JSON
        std::string contentJson = "{" + content + "}";

        WidgetEntry w;
        w.widgetId = stateKey;
        w.roomId = roomId_;
        w.type = extractStr(contentJson, "type");
        w.url = extractStr(contentJson, "url");
        w.name = extractStr(contentJson, "name");
        if (w.name.empty()) {
            // Try from data.title
            auto data = extractStr(contentJson, "data");
            if (!data.empty()) {
                w.name = extractStr("{" + data + "}", "title");
            }
        }
        if (w.name.empty()) w.name = stateKey;

        w.creatorUserId = extractStr(contentJson, "creatorUserId");
        w.avatarUrl = extractStr(contentJson, "avatar_url");

        // Boolean fields
        w.waitForIframeLoad = contentJson.find("\"waitForIframeLoad\":true") != std::string::npos;
        w.isPinned = contentJson.find("\"alwaysOnScreen\":true") != std::string::npos;

        // Size fields
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
        w.width = extractInt(contentJson, "width");
        w.height = extractInt(contentJson, "height");

        // Sanitize the URL
        std::string err;
        w.sanitizedUrl = sanitizeWidgetUrl(w.url, err);
        if (w.sanitizedUrl.empty() && !w.url.empty()) {
            w.sanitizedUrl = w.url; // fallback — URL may have already been templated
        }

        // Display order
        w.displayOrder = static_cast<int>(widgets_.size());

        widgets_.push_back(w);
        pos = objEnd + 1;
    }

    return widgets_;
}

std::string WidgetManager::createWidget(const std::string& widgetId, const std::string& type,
                                        const std::string& url, const std::string& name,
                                        bool waitForIframeLoad, std::string& error) {
    // Check for duplicate
    if (findWidget(widgetId)) {
        error = "Widget with ID '" + widgetId + "' already exists";
        return "";
    }

    // Validate type
    auto wtype = classifyWidgetType(type);
    if (wtype == WidgetType::UNKNOWN && type != "m.custom") {
        // Allow unknown types but warn
    }

    WidgetEntry w;
    w.widgetId = widgetId;
    w.type = type;
    w.url = url;
    w.name = name.empty() ? widgetId : name;
    w.roomId = roomId_;
    w.waitForIframeLoad = waitForIframeLoad;
    w.createdAt = static_cast<int64_t>(std::time(nullptr)) * 1000;

    // Sanitize URL
    w.sanitizedUrl = sanitizeWidgetUrl(url, error);
    if (w.sanitizedUrl.empty()) {
        return "";
    }

    w.displayOrder = static_cast<int>(widgets_.size());
    widgets_.push_back(w);

    return buildWidgetStateJson(w);
}

std::string WidgetManager::removeWidget(const std::string& widgetId) {
    for (auto it = widgets_.begin(); it != widgets_.end(); ++it) {
        if (it->widgetId == widgetId) {
            widgets_.erase(it);
            return R"({"state_key":")" + widgetId + R"(","type":"im.vector.modular.widgets","content":{}})";
        }
    }
    return R"({"state_key":")" + widgetId + R"(","type":"im.vector.modular.widgets","content":{}})";
}

std::string WidgetManager::setWidgetPinned(const std::string& widgetId, bool pinned, std::string& error) {
    auto* w = findWidget(widgetId);
    if (!w) {
        error = "Widget not found: " + widgetId;
        return "";
    }
    w->isPinned = pinned;
    return buildWidgetStateJson(*w);
}

std::string WidgetManager::resizeWidget(const std::string& widgetId, int width, int height, std::string& error) {
    auto* w = findWidget(widgetId);
    if (!w) {
        error = "Widget not found: " + widgetId;
        return "";
    }
    if (width < 0 || height < 0) {
        error = "Invalid dimensions: " + std::to_string(width) + "x" + std::to_string(height);
        return "";
    }
    w->width = width;
    w->height = height;
    return buildWidgetStateJson(*w);
}

std::string WidgetManager::setWidgetMinimized(const std::string& widgetId, bool minimized) {
    auto* w = findWidget(widgetId);
    if (!w) return "";
    w->isMinimized = minimized;
    if (minimized) w->isMaximized = false;
    return buildWidgetStateJson(*w);
}

std::string WidgetManager::setWidgetMaximized(const std::string& widgetId, bool maximized) {
    auto* w = findWidget(widgetId);
    if (!w) return "";
    w->isMaximized = maximized;
    if (maximized) w->isMinimized = false;
    return buildWidgetStateJson(*w);
}

std::string WidgetManager::requestCapability(const std::string& widgetId, WidgetCapability cap,
                                             std::string& error) {
    auto* w = findWidget(widgetId);
    if (!w) {
        error = "Widget not found: " + widgetId;
        return "";
    }

    WidgetCapabilityRequest req;
    req.widgetId = widgetId;
    req.widgetName = w->name;
    req.capability = cap;

    if (isAutoApprovedCapability(cap, w->type)) {
        req.approved = true;
        req.reason = "Auto-approved for " + w->type;
        w->approvedCapabilities.push_back(cap);
        return formatCapabilityRequest(req);
    }

    req.reason = "User consent required";
    return formatCapabilityRequest(req);
}

std::string WidgetManager::approveCapability(const std::string& widgetId, WidgetCapability cap) {
    auto* w = findWidget(widgetId);
    if (!w) return "";

    // Check if already approved
    for (auto c : w->approvedCapabilities) {
        if (c == cap) return R"({"approved":true,"already":true})";
    }

    w->approvedCapabilities.push_back(cap);
    return R"({"approved":true,"capability":")" + std::string(capabilityToString(cap)) + R"("})";
}

std::string WidgetManager::denyCapability(const std::string& widgetId, WidgetCapability cap) {
    return R"({"denied":true,"capability":")" + std::string(capabilityToString(cap)) + R"("})";
}

std::string WidgetManager::getWidgetUrl(const std::string& widgetId, std::string& error) {
    auto* w = findWidget(widgetId);
    if (!w) {
        error = "Widget not found: " + widgetId;
        return "";
    }
    if (!w->sanitizedUrl.empty()) return w->sanitizedUrl;

    // Apply template on the fly
    auto tpl = buildTemplate();
    tpl.widgetId = widgetId;
    return applyWidgetUrlTemplate(w->url, tpl);
}

std::string WidgetManager::buildWidgetPostMessage(const std::string& widgetId,
                                                   const std::string& action,
                                                   const std::string& data) {
    std::ostringstream msg;
    msg << R"({"api":"fromWidget",)"
        << R"("widgetId":")" << widgetId << R"(",)";
    msg << R"("action":")" << action << R"(",)";
    msg << R"("data":)" << (data.empty() ? "{}" : data) << ",";
    msg << R"("requestId":")" << widgetId << "_" << std::time(nullptr) << R"(")";
    msg << "}";
    return msg.str();
}

std::string WidgetManager::parseWidgetPostMessage(const std::string& message,
                                                   std::string& action,
                                                   std::string& widgetId,
                                                   std::string& data) {
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

    action = extractStr(message, "action");
    widgetId = extractStr(message, "widgetId");
    data = extractObj(message, "data");
    return extractStr(message, "api");
}

bool WidgetManager::supportsPiP(const std::string& widgetId) const {
    auto* w = findWidget(widgetId);
    if (!w) return false;
    // Jitsi supports PiP, others by capability
    if (w->type == "m.jitsi" || w->type == "jitsi") return true;
    for (auto cap : w->approvedCapabilities) {
        if (cap == WidgetCapability::ALWAYS_ON_SCREEN) return true;
    }
    return false;
}

std::vector<WidgetEntry> WidgetManager::getWidgetsByType(const std::string& type) const {
    std::vector<WidgetEntry> result;
    for (const auto& w : widgets_) {
        if (w.type == type) result.push_back(w);
    }
    // Sort by display order
    std::sort(result.begin(), result.end(),
              [](const WidgetEntry& a, const WidgetEntry& b) { return a.displayOrder < b.displayOrder; });
    return result;
}

bool WidgetManager::getWidget(const std::string& widgetId, WidgetEntry& out) const {
    auto* w = findWidget(widgetId);
    if (!w) return false;
    out = *w;
    return true;
}

std::string WidgetManager::widgetsToJson() const {
    std::ostringstream os;
    os << "[";
    bool first = true;
    for (const auto& w : widgets_) {
        if (!first) os << ","; first = false;
        auto esc = [](const std::string& s) -> std::string {
            std::string out;
            for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
            return out;
        };
        os << R"({"widgetId":")" << esc(w.widgetId)
           << R"(","name":")" << esc(w.name)
           << R"(","type":")" << esc(w.type)
           << R"(","typeName":")" << esc(getWidgetTypeName(w.type))
           << R"(","url":")" << esc(w.url)
           << R"(","sanitizedUrl":")" << esc(w.sanitizedUrl)
           << R"(","pinned":)" << (w.isPinned ? "true" : "false")
           << R"(,"minimized":)" << (w.isMinimized ? "true" : "false")
           << R"(,"maximized":)" << (w.isMaximized ? "true" : "false")
           << R"(,"supportsPiP":)" << (supportsPiP(w.widgetId) ? "true" : "false");
        if (w.width > 0) os << R"(,"width":)" << w.width;
        if (w.height > 0) os << R"(,"height":)" << w.height;
        if (!w.approvedCapabilities.empty()) {
            os << R"(,"capabilities":[)";
            bool cf = true;
            for (auto cap : w.approvedCapabilities) {
                if (!cf) os << ","; cf = false;
                os << "\"" << capabilityToString(cap) << "\"";
            }
            os << "]";
        }
        os << "}";
    }
    os << "]";
    return os.str();
}

std::string WidgetManager::buildGlobalCsp() const {
    return policy_.contentSecurityPolicy;
}

// Utility wrapper from widget_utils (kept for backward compat)

} // namespace progressive
