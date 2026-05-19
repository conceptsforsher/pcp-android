#ifndef PROGRESSIVE_SVG_DRAW_HPP
#define PROGRESSIVE_SVG_DRAW_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// ---- SVG Rendering ----

struct SvgRect {
    double x, y, w, h;
};

struct SvgCircle {
    double cx, cy, r;
};

struct SvgPath {
    std::string d; // SVG path data
    std::string fill;
    std::string stroke;
    double strokeWidth = 1.0;
};

struct SvgText {
    double x, y;
    std::string text;
    std::string fontFamily;
    double fontSize = 12.0;
    std::string fill;
};

struct SvgElement {
    enum Type { Rect, Circle, Path, Text, Ellipse, Line, Polygon, G };
    Type type = Rect;
    SvgRect rect;
    SvgCircle circle;
    SvgPath path;
    SvgText text;
    std::string fill = "black";
    std::string stroke = "none";
    double strokeWidth = 0.0;
    double opacity = 1.0;
    std::string transform; // e.g. "translate(10,20) rotate(45)"
};

struct SvgDocument {
    double width = 400.0;
    double height = 300.0;
    std::vector<SvgElement> elements;
};

// Parse an SVG string into a SvgDocument.
SvgDocument parseSvg(const std::string& svgData);

// Render SVG document to a flat list of draw commands (for Android Canvas).
// Returns JSON array of draw commands.
std::string renderSvgToDrawCommands(const SvgDocument& doc);

// Check if a string is valid SVG.
bool isValidSvg(const std::string& data);

// ---- Drawing Tool ----

struct DrawCommand {
    enum CmdType { MoveTo, LineTo, QuadTo, CubicTo, Arc, Close, SetColor, SetWidth, SetAlpha, BeginFill, EndFill };
    CmdType type = MoveTo;
    double x1 = 0, y1 = 0;
    double x2 = 0, y2 = 0;
    double x3 = 0, y3 = 0;
    double argb = 0xFF000000; // color
    double width = 2.0;
    double alpha = 1.0;
};

class DrawingCanvas {
public:
    void moveTo(double x, double y);
    void lineTo(double x, double y);
    void quadTo(double cx, double cy, double x, double y);
    void setColor(int argb);
    void setWidth(double w);
    void setAlpha(double a);
    void clear();

    // Export all commands as JSON for Android Canvas replay.
    std::string exportCommandsJson() const;

    // Estimate SVG path data from draw commands.
    std::string toSvgPath() const;

    int commandCount() const { return static_cast<int>(commands_.size()); }

private:
    std::vector<DrawCommand> commands_;
    double currentX_ = 0, currentY_ = 0;
    int currentColor_ = 0xFF000000;
    double currentWidth_ = 2.0;
    double currentAlpha_ = 1.0;
};

} // namespace progressive

#endif // PROGRESSIVE_SVG_DRAW_HPP
