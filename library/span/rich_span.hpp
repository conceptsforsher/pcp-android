#pragma once
#include <string>
#include <vector>
#include <functional>
#include <memory>

namespace pcp::span {

enum class SpanType {
    BOLD, ITALIC, UNDERLINE, STRIKETHROUGH,
    CODE, CODE_BLOCK, LINK, MENTION, ROOM_MENTION,
    COLOR, BACKGROUND, CUSTOM_TYPEFACE,
    SUPERSCRIPT, SUBSCRIPT, SPOILER, QUOTE
};

struct SpanSegment {
    SpanType type;
    int start = 0;
    int end = 0;
    std::string data; // URL for links, user ID for mentions, etc.
    uint32_t color = 0xFFFFFFFF;
    float fontSize = 0.0f;
    float lineSpacing = 0.0f;
};

class RichSpanBuilder {
public:
    RichSpanBuilder();
    ~RichSpanBuilder();

    // Bold
    RichSpanBuilder& bold(int start, int end);
    // Italic
    RichSpanBuilder& italic(int start, int end);
    // Underline
    RichSpanBuilder& underline(int start, int end);
    // Strikethrough
    RichSpanBuilder& strikethrough(int start, int end);
    // Inline code
    RichSpanBuilder& code(int start, int end);
    // Code block
    RichSpanBuilder& codeBlock(int start, int end);
    // Hyperlink
    RichSpanBuilder& link(int start, int end, const std::string& url);
    // User mention
    RichSpanBuilder& mention(int start, int end, const std::string& userId);
    // Room mention
    RichSpanBuilder& roomMention(int start, int end, const std::string& roomId);
    // Foreground/background color
    RichSpanBuilder& color(int start, int end, uint32_t fg);
    RichSpanBuilder& background(int start, int end, uint32_t bg);
    // Custom typeface
    RichSpanBuilder& typeface(int start, int end, const std::string& fontFamily);
    // Font size
    RichSpanBuilder& fontSize(int start, int end, float size);
    // Line spacing
    RichSpanBuilder& lineSpacing(int start, int end, float spacing);
    // Super/subscript
    RichSpanBuilder& superscript(int start, int end);
    RichSpanBuilder& subscript(int start, int end);
    // Spoiler
    RichSpanBuilder& spoiler(int start, int end);
    // Quote
    RichSpanBuilder& quote(int start, int end);

    std::vector<SpanSegment> build() const { return m_segments; }
    void clear() { m_segments.clear(); }

private:
    std::vector<SpanSegment> m_segments;
    void addSpan(SpanType type, int start, int end, const std::string& data = "");
};

// ── Markdown-style rich text parser ──

struct RichTextNode {
    std::string text;
    std::vector<SpanSegment> spans;
    bool isBlock = false;
};

class RichTextParser {
public:
    RichTextParser();
    ~RichTextParser();

    std::vector<RichTextNode> parse(const std::string& text);
    std::string toHtml(const std::vector<RichTextNode>& nodes);
    std::string toPlainText(const std::vector<RichTextNode>& nodes);

    void enableMarkdown(bool enable) { m_markdown = enable; }
    void enableHtml(bool enable) { m_html = enable; }

private:
    bool m_markdown = true;
    bool m_html = true;

    void parseMarkdown(const std::string& text, RichSpanBuilder& builder,
                       std::vector<std::pair<int, int>>& codeSpans);
    void parseHtml(const std::string& text, RichSpanBuilder& builder);
    void parseLinks(const std::string& text, RichSpanBuilder& builder);
    void parseMentions(const std::string& text, RichSpanBuilder& builder);
    std::string escapeHtml(const std::string& text) const;
};

} // namespace pcp::span
