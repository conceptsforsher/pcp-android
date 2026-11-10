#include "rich_span.hpp"
#include <regex>
#include <sstream>
#include <algorithm>

namespace pcp::span {

RichSpanBuilder::RichSpanBuilder() = default;
RichSpanBuilder::~RichSpanBuilder() = default;

void RichSpanBuilder::addSpan(SpanType type, int start, int end, const std::string& data) {
    m_segments.push_back({type, start, end, data});
}

RichSpanBuilder& RichSpanBuilder::bold(int start, int end) {
    addSpan(SpanType::BOLD, start, end); return *this;
}
RichSpanBuilder& RichSpanBuilder::italic(int start, int end) {
    addSpan(SpanType::ITALIC, start, end); return *this;
}
RichSpanBuilder& RichSpanBuilder::underline(int start, int end) {
    addSpan(SpanType::UNDERLINE, start, end); return *this;
}
RichSpanBuilder& RichSpanBuilder::strikethrough(int start, int end) {
    addSpan(SpanType::STRIKETHROUGH, start, end); return *this;
}
RichSpanBuilder& RichSpanBuilder::code(int start, int end) {
    addSpan(SpanType::CODE, start, end); return *this;
}
RichSpanBuilder& RichSpanBuilder::codeBlock(int start, int end) {
    addSpan(SpanType::CODE_BLOCK, start, end); return *this;
}
RichSpanBuilder& RichSpanBuilder::link(int start, int end, const std::string& url) {
    addSpan(SpanType::LINK, start, end, url); return *this;
}
RichSpanBuilder& RichSpanBuilder::mention(int start, int end, const std::string& userId) {
    addSpan(SpanType::MENTION, start, end, userId); return *this;
}
RichSpanBuilder& RichSpanBuilder::roomMention(int start, int end, const std::string& roomId) {
    addSpan(SpanType::ROOM_MENTION, start, end, roomId); return *this;
}
RichSpanBuilder& RichSpanBuilder::color(int start, int end, uint32_t fg) {
    m_segments.push_back({SpanType::COLOR, start, end, "", fg}); return *this;
}
RichSpanBuilder& RichSpanBuilder::background(int start, int end, uint32_t bg) {
    m_segments.push_back({SpanType::BACKGROUND, start, end, "", bg}); return *this;
}
RichSpanBuilder& RichSpanBuilder::typeface(int start, int end, const std::string& fontFamily) {
    addSpan(SpanType::CUSTOM_TYPEFACE, start, end, fontFamily); return *this;
}
RichSpanBuilder& RichSpanBuilder::fontSize(int start, int end, float size) {
    m_segments.push_back({SpanType::CUSTOM_TYPEFACE, start, end, "", 0, size}); return *this;
}
RichSpanBuilder& RichSpanBuilder::lineSpacing(int start, int end, float spacing) {
    m_segments.push_back({SpanType::CUSTOM_TYPEFACE, start, end, "", 0, 0.0f, spacing}); return *this;
}
RichSpanBuilder& RichSpanBuilder::superscript(int start, int end) {
    addSpan(SpanType::SUPERSCRIPT, start, end); return *this;
}
RichSpanBuilder& RichSpanBuilder::subscript(int start, int end) {
    addSpan(SpanType::SUBSCRIPT, start, end); return *this;
}
RichSpanBuilder& RichSpanBuilder::spoiler(int start, int end) {
    addSpan(SpanType::SPOILER, start, end); return *this;
}
RichSpanBuilder& RichSpanBuilder::quote(int start, int end) {
    addSpan(SpanType::QUOTE, start, end); return *this;
}

// ── RichTextParser ──

RichTextParser::RichTextParser() = default;
RichTextParser::~RichTextParser() = default;

std::vector<RichTextNode> RichTextParser::parse(const std::string& text) {
    std::vector<RichTextNode> nodes;
    RichTextNode node;
    node.text = text;

    RichSpanBuilder builder;
    if (m_markdown) {
        std::vector<std::pair<int, int>> codeSpans;
        parseMarkdown(text, builder, codeSpans);
    }
    if (m_html) {
        parseHtml(text, builder);
    }

    parseLinks(text, builder);
    parseMentions(text, builder);

    node.spans = builder.build();
    nodes.push_back(node);
    return nodes;
}

void RichTextParser::parseMarkdown(const std::string& text, RichSpanBuilder& builder,
                                     std::vector<std::pair<int, int>>& codeSpans) {
    // **bold**
    std::regex boldRe("\\*\\*(.+?)\\*\\*");
    auto words_begin = std::sregex_iterator(text.begin(), text.end(), boldRe);
    for (auto it = words_begin; it != std::sregex_iterator(); ++it) {
        int start = it->position();
        int end = start + it->length();
        builder.bold(start + 2, end - 2);
    }

    // *italic*
    std::regex italicRe("(?<!\\*)\\*(?!\\*)(.+?)(?<!\\*)\\*(?!\\*)");
    for (auto it = std::sregex_iterator(text.begin(), text.end(), italicRe);
         it != std::sregex_iterator(); ++it) {
        int start = it->position();
        int end = start + it->length();
        builder.italic(start + 1, end - 1);
    }

    // `code`
    std::regex codeRe("`(.+?)`");
    for (auto it = std::sregex_iterator(text.begin(), text.end(), codeRe);
         it != std::sregex_iterator(); ++it) {
        int start = it->position();
        int end = start + it->length();
        builder.code(start + 1, end - 1);
        codeSpans.push_back({start, end});
    }
}

void RichTextParser::parseLinks(const std::string& text, RichSpanBuilder& builder) {
    std::regex linkRe(R"(https?://[^\s<>"{}|\\^`]+)");
    for (auto it = std::sregex_iterator(text.begin(), text.end(), linkRe);
         it != std::sregex_iterator(); ++it) {
        std::string url = it->str();
        builder.link(it->position(), it->position() + url.length(), url);
    }
}

void RichTextParser::parseMentions(const std::string& text, RichSpanBuilder& builder) {
    // @user:domain.com
    std::regex mentionRe(R"(@[a-zA-Z0-9._=-]+:[a-zA-Z0-9.-]+)");
    for (auto it = std::sregex_iterator(text.begin(), text.end(), mentionRe);
         it != std::sregex_iterator(); ++it) {
        std::string mxid = it->str().substr(1);
        builder.mention(it->position(), it->position() + it->length(), mxid);
    }
}

void RichTextParser::parseHtml(const std::string& text, RichSpanBuilder& builder) {
    // <b>, <i>, <u>, <s>, <code>, <a href="...">
    // Stub - full HTML parsing would use a proper HTML parser
}

std::string RichTextParser::toHtml(const std::vector<RichTextNode>& nodes) {
    std::ostringstream os;
    for (auto& node : nodes) {
        os << escapeHtml(node.text);
    }
    return os.str();
}

std::string RichTextParser::toPlainText(const std::vector<RichTextNode>& nodes) {
    std::ostringstream os;
    for (auto& node : nodes) os << node.text;
    return os.str();
}

std::string RichTextParser::escapeHtml(const std::string& text) const {
    std::string result;
    for (char c : text) {
        switch (c) {
            case '<': result += "&lt;"; break;
            case '>': result += "&gt;"; break;
            case '&': result += "&amp;"; break;
            case '"': result += "&quot;"; break;
            default: result += c;
        }
    }
    return result;
}

} // namespace pcp::span
