#include "progressive/svg_draw.hpp"
#include <sstream>
#include <regex>
#include <cmath>
#include <algorithm>

namespace progressive {

// ---- SVG Parser ----

static std::string extractAttr(const std::string& tag, const std::string& attr) {
    auto search = attr + "=\"";
    auto pos = tag.find(search);
    if (pos == std::string::npos) {
        search = attr + "='";
        pos = tag.find(search);
    }
    if (pos == std::string::npos) return {};
    pos += search.size();
    auto end = tag.find('"', pos);
    if (end == std::string::npos) end = tag.find('\'', pos);
    if (end == std::string::npos) return {};
    return tag.substr(pos, end - pos);
}

static double parsePx(const std::string& val) {
    if (val.empty()) return 0.0;
    std::string num = val;
    if (num.back() == '%') return 0.0; // percentages not supported yet
    auto px = num.find("px");
    if (px != std::string::npos) num = num.substr(0, px);
    return std::stod(num);
}

SvgDocument parseSvg(const std::string& svgData) {
    SvgDocument doc;

    // Extract viewBox
    std::regex viewBoxRe(R"(viewBox=["']([0-9.\s]+)["'])");
    std::smatch vbm;
    if (std::regex_search(svgData, vbm, viewBoxRe)) {
        std::istringstream vb(vbm[1]);
        double x, y, w, h;
        vb >> x >> y >> w >> h;
        if (w > 0) doc.width = w;
        if (h > 0) doc.height = h;
    }

    // Parse rect elements
    std::regex rectRe(R"(<rect\s+[^>]*/?>)");
    auto rectsBegin = std::sregex_iterator(svgData.begin(), svgData.end(), rectRe);
    for (auto it = rectsBegin; it != std::sregex_iterator(); ++it) {
        SvgElement el;
        el.type = SvgElement::Rect;
        std::string tag = it->str();
        el.rect.x = parsePx(extractAttr(tag, "x"));
        el.rect.y = parsePx(extractAttr(tag, "y"));
        el.rect.w = parsePx(extractAttr(tag, "width"));
        el.rect.h = parsePx(extractAttr(tag, "height"));
        el.fill = extractAttr(tag, "fill");
        if (el.fill.empty()) el.fill = "black";
        el.stroke = extractAttr(tag, "stroke");
        el.opacity = extractAttr(tag, "opacity").empty() ? 1.0 : std::stod(extractAttr(tag, "opacity"));
        doc.elements.push_back(el);
    }

    // Parse circle elements
    std::regex circleRe(R"(<circle\s+[^>]*/?>)");
    auto circlesBegin = std::sregex_iterator(svgData.begin(), svgData.end(), circleRe);
    for (auto it = circlesBegin; it != std::sregex_iterator(); ++it) {
        SvgElement el;
        el.type = SvgElement::Circle;
        std::string tag = it->str();
        el.circle.cx = parsePx(extractAttr(tag, "cx"));
        el.circle.cy = parsePx(extractAttr(tag, "cy"));
        el.circle.r = parsePx(extractAttr(tag, "r"));
        el.fill = extractAttr(tag, "fill");
        if (el.fill.empty()) el.fill = "black";
        doc.elements.push_back(el);
    }

    // Parse path elements
    std::regex pathRe(R"(<path\s+[^>]*/?>)");
    auto pathsBegin = std::sregex_iterator(svgData.begin(), svgData.end(), pathRe);
    for (auto it = pathsBegin; it != std::sregex_iterator(); ++it) {
        SvgElement el;
        el.type = SvgElement::Path;
        std::string tag = it->str();
        el.path.d = extractAttr(tag, "d");
        el.fill = extractAttr(tag, "fill");
        el.stroke = extractAttr(tag, "stroke");
        auto sw = extractAttr(tag, "stroke-width");
        if (!sw.empty()) el.strokeWidth = parsePx(sw);
        doc.elements.push_back(el);
    }

    return doc;
}

std::string renderSvgToDrawCommands(const SvgDocument& doc) {
    std::ostringstream json;
    json << R"({"width": )" << doc.width << R"(,"height": )" << doc.height << R"(,"commands": [)";

    for (size_t i = 0; i < doc.elements.size(); ++i) {
        if (i > 0) json << ",";
        const auto& el = doc.elements[i];

        switch (el.type) {
            case SvgElement::Rect:
                json << R"({"type":"rect","x":)" << el.rect.x << R"(,"y":)" << el.rect.y
                     << R"(,"w":)" << el.rect.w << R"(,"h":)" << el.rect.h
                     << R"(,"fill":")" << el.fill << R"("})";
                break;
            case SvgElement::Circle:
                json << R"({"type":"circle","cx":)" << el.circle.cx << R"(,"cy":)" << el.circle.cy
                     << R"(,"r":)" << el.circle.r << R"(,"fill":")" << el.fill << R"("})";
                break;
            case SvgElement::Path:
                json << R"({"type":"path","d":")" << el.path.d << R"(")";
                if (!el.fill.empty()) json << R"(,"fill":")" << el.fill << R"(")";
                if (!el.stroke.empty()) json << R"(,"stroke":")" << el.stroke << R"(")";
                if (el.strokeWidth > 0) json << R"(,"strokeWidth":)" << el.strokeWidth;
                json << "}";
                break;
            default: break;
        }
    }

    json << "]}";
    return json.str();
}

bool isValidSvg(const std::string& data) {
    return data.find("<svg") != std::string::npos;
}

// ---- Drawing Canvas ----

void DrawingCanvas::moveTo(double x, double y) {
    currentX_ = x; currentY_ = y;
    DrawCommand cmd;
    cmd.type = DrawCommand::MoveTo;
    cmd.x1 = x; cmd.y1 = y;
    cmd.argb = currentColor_;
    cmd.width = currentWidth_;
    cmd.alpha = currentAlpha_;
    commands_.push_back(cmd);
}

void DrawingCanvas::lineTo(double x, double y) {
    DrawCommand cmd;
    cmd.type = DrawCommand::LineTo;
    cmd.x1 = currentX_; cmd.y1 = currentY_;
    cmd.x2 = x; cmd.y2 = y;
    cmd.argb = currentColor_;
    cmd.width = currentWidth_;
    cmd.alpha = currentAlpha_;
    commands_.push_back(cmd);
    currentX_ = x; currentY_ = y;
}

void DrawingCanvas::quadTo(double cx, double cy, double x, double y) {
    DrawCommand cmd;
    cmd.type = DrawCommand::QuadTo;
    cmd.x1 = cx; cmd.y1 = cy;
    cmd.x2 = x; cmd.y2 = y;
    cmd.argb = currentColor_;
    cmd.width = currentWidth_;
    cmd.alpha = currentAlpha_;
    commands_.push_back(cmd);
    currentX_ = x; currentY_ = y;
}

void DrawingCanvas::setColor(int argb) {
    currentColor_ = argb;
}

void DrawingCanvas::setWidth(double w) {
    currentWidth_ = w;
}

void DrawingCanvas::setAlpha(double a) {
    currentAlpha_ = a;
}

void DrawingCanvas::clear() {
    commands_.clear();
    currentX_ = 0; currentY_ = 0;
}

std::string DrawingCanvas::exportCommandsJson() const {
    std::ostringstream json;
    json << "[";
    for (size_t i = 0; i < commands_.size(); ++i) {
        if (i > 0) json << ",";
        const auto& c = commands_[i];
        json << "{";
        json << R"("type": )" << static_cast<int>(c.type) << ",";
        json << R"("x1": )" << c.x1 << ",";
        json << R"("y1": )" << c.y1;
        if (c.type == DrawCommand::LineTo || c.type == DrawCommand::QuadTo) {
            json << R"(,"x2": )" << c.x2 << R"(,"y2": )" << c.y2;
        }
        if (c.type == DrawCommand::QuadTo) {
            json << R"(,"x3": )" << c.x3 << R"(,"y3": )" << c.y3;
        }
        json << R"(,"color": )" << static_cast<int64_t>(c.argb);
        json << R"(,"width": )" << c.width;
        json << R"(,"alpha": )" << c.alpha;
        json << "}";
    }
    json << "]";
    return json.str();
}

std::string DrawingCanvas::toSvgPath() const {
    std::ostringstream d;
    for (const auto& c : commands_) {
        switch (c.type) {
            case DrawCommand::MoveTo:
                d << "M" << c.x1 << " " << c.y1 << " ";
                break;
            case DrawCommand::LineTo:
                d << "L" << c.x2 << " " << c.y2 << " ";
                break;
            case DrawCommand::QuadTo:
                d << "Q" << c.x1 << " " << c.y1 << " " << c.x2 << " " << c.y2 << " ";
                break;
            default: break;
        }
    }
    return d.str();
}

} // namespace progressive
