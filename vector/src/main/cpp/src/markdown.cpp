#include "progressive/markdown.hpp"
#include <sstream>
#include <regex>
#include <vector>
#include <algorithm>
#include <cctype>

namespace progressive {

// ============================================================================
// Helper utilities
// ============================================================================

static std::string escapeHtml(const std::string& s) {
    std::string out;
    out.reserve(s.size());
    for (char c : s) {
        switch (c) {
            case '&': out += "&amp;"; break;
            case '<': out += "&lt;"; break;
            case '>': out += "&gt;"; break;
            case '"': out += "&quot;"; break;
            default:  out += c;
        }
    }
    return out;
}

static std::string unescapeHtml(const std::string& s) {
    std::string out = s;
    // Simple replacements for common entities
    for (size_t pos = 0; (pos = out.find("&amp;", pos)) != std::string::npos; )
        out.replace(pos, 5, "&"), pos += 1;
    for (size_t pos = 0; (pos = out.find("&lt;", pos)) != std::string::npos; )
        out.replace(pos, 4, "<"), pos += 1;
    for (size_t pos = 0; (pos = out.find("&gt;", pos)) != std::string::npos; )
        out.replace(pos, 4, ">"), pos += 1;
    for (size_t pos = 0; (pos = out.find("&quot;", pos)) != std::string::npos; )
        out.replace(pos, 6, "\""), pos += 1;
    for (size_t pos = 0; (pos = out.find("&#39;", pos)) != std::string::npos; )
        out.replace(pos, 5, "'"), pos += 1;
    for (size_t pos = 0; (pos = out.find("&apos;", pos)) != std::string::npos; )
        out.replace(pos, 6, "'"), pos += 1;
    return out;
}

static void ltrim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

static void rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

static void trim(std::string& s) {
    ltrim(s);
    rtrim(s);
}

// ============================================================================
// Legacy inline processor (regex-based, kept for backward compat)
// ============================================================================

static std::string processInline(const std::string& line, const MdConfig& config) {
    std::string result = line;

    // Code spans: `code`
    {
        std::regex codeRe(R"(`([^`]+)`)");
        result = std::regex_replace(result, codeRe, "<code>$1</code>");
    }

    // Bold: **text** or __text__
    {
        std::regex boldRe(R"(\*\*([^*]+)\*\*)");
        result = std::regex_replace(result, boldRe, "<b>$1</b>");
    }
    {
        std::regex boldRe2(R"(__([^_]+)__)");
        result = std::regex_replace(result, boldRe2, "<b>$1</b>");
    }

    // Italic: *text* or _text_
    {
        std::regex italicRe(R"(\*([^*]+)\*)");
        result = std::regex_replace(result, italicRe, "<i>$1</i>");
    }
    {
        std::regex italicRe2(R"(_([^_]+)_)");
        result = std::regex_replace(result, italicRe2, "<i>$1</i>");
    }

    // Strikethrough: ~~text~~
    {
        std::regex strikeRe(R"(~~([^~]+)~~)");
        result = std::regex_replace(result, strikeRe, "<s>$1</s>");
    }

    // Links: [text](url)
    if (config.enableLinks) {
        std::regex linkRe(R"(\[([^\]]+)\]\(([^)]+)\))");
        result = std::regex_replace(result, linkRe, R"(<a href="$2">$1</a>)");
    }

    return result;
}

// ============================================================================
// Table parsing (legacy)
// ============================================================================

static std::string parseTable(const std::string& tableBlock, bool withScroll) {
    std::istringstream stream(tableBlock);
    std::string line;
    std::vector<std::string> headers;
    std::vector<std::vector<std::string>> rows;
    bool inHeader = true;
    bool inSeparator = false;

    while (std::getline(stream, line)) {
        if (line.empty()) continue;
        if (line[0] != '|') break;

        std::vector<std::string> cells;
        size_t start = 1;
        while (start < line.size()) {
            size_t end = line.find('|', start);
            if (end == std::string::npos) {
                std::string cell = line.substr(start);
                while (!cell.empty() && cell.back() == ' ') cell.pop_back();
                while (!cell.empty() && cell.front() == ' ') cell.erase(0, 1);
                cells.push_back(cell);
                break;
            }
            std::string cell = line.substr(start, end - start);
            while (!cell.empty() && cell.back() == ' ') cell.pop_back();
            while (!cell.empty() && cell.front() == ' ') cell.erase(0, 1);
            cells.push_back(cell);
            start = end + 1;
        }

        if (inHeader) {
            headers = std::move(cells);
            inHeader = false;
            inSeparator = true;
        } else if (inSeparator) {
            bool isSep = true;
            for (const auto& cell : cells) {
                for (char c : cell) {
                    if (c != '-' && c != ':' && c != ' ') { isSep = false; break; }
                }
                if (!isSep) break;
            }
            if (isSep) {
                inSeparator = false;
            } else {
                rows.push_back(std::move(cells));
                inSeparator = false;
            }
        } else {
            rows.push_back(std::move(cells));
        }
    }

    if (headers.empty()) return {};

    std::ostringstream html;

    if (withScroll) {
        html << "<div style=\"overflow-x:auto; display:block; white-space:nowrap;\">";
    }

    html << "<table style=\"border-collapse:collapse; width:100%; margin:8px 0;\">";
    html << "<thead><tr>";
    for (const auto& h : headers) {
        html << "<th style=\"border:1px solid #ccc; padding:6px 12px; background:#f5f5f5; font-weight:600;\">"
             << escapeHtml(h) << "</th>";
    }
    html << "</tr></thead>";

    html << "<tbody>";
    for (const auto& row : rows) {
        html << "<tr>";
        size_t col = 0;
        for (const auto& cell : row) {
            html << "<td style=\"border:1px solid #ccc; padding:6px 12px;\">"
                 << processInline(cell, MdConfig{}) << "</td>";
            ++col;
        }
        while (col < headers.size()) {
            html << "<td></td>";
            ++col;
        }
        html << "</tr>";
    }
    html << "</tbody>";
    html << "</table>";

    if (withScroll) {
        html << "</div>";
    }

    return html.str();
}

// ============================================================================
// Legacy markdownToHtml (kept for backward compatibility)
// ============================================================================

std::string markdownToHtml(const std::string& markdown, const MdConfig& config) {
    std::istringstream stream(markdown);
    std::ostringstream html;
    std::string line;
    bool inCodeBlock = false;
    std::string codeBlockContent;
    std::string codeBlockLang;
    bool inBlockquote = false;
    bool inList = false;
    bool inOrderedList = false;
    bool inTable = false;
    std::string tableBlock;
    std::string prevLine;

    auto flushParagraph = [&]() {
        if (inList) { html << "</ul>\n"; inList = false; }
        if (inOrderedList) { html << "</ol>\n"; inOrderedList = false; }
    };

    while (std::getline(stream, line) || !codeBlockContent.empty()) {
        // Code block handling
        if (config.enableCodeBlocks && (line.rfind("```", 0) == 0)) {
            if (!inCodeBlock) {
                flushParagraph();
                codeBlockLang = line.substr(3);
                while (!codeBlockLang.empty() && codeBlockLang[0] == ' ') codeBlockLang.erase(0, 1);
                inCodeBlock = true;
                codeBlockContent.clear();
                continue;
            } else {
                html << "<pre><code";
                if (!codeBlockLang.empty()) html << " class=\"language-" << escapeHtml(codeBlockLang) << "\"";
                html << ">";
                html << escapeHtml(codeBlockContent);
                html << "</code></pre>\n";
                inCodeBlock = false;
                codeBlockContent.clear();
                codeBlockLang.clear();
                continue;
            }
        }

        if (inCodeBlock) {
            if (!codeBlockContent.empty()) codeBlockContent += "\n";
            codeBlockContent += line;
            continue;
        }

        // Table detection
        if (config.enableTables && !line.empty() && line[0] == '|' && line.find('|', 1) != std::string::npos) {
            if (!inTable) {
                flushParagraph();
                inTable = true;
                tableBlock.clear();
            }
            tableBlock += line + "\n";
            auto nextPos = stream.tellg();
            std::string nextLine;
            bool hasNext = (bool)std::getline(stream, nextLine);
            if (hasNext) stream.seekg(nextPos);
            if (!hasNext || nextLine.empty() || nextLine[0] != '|') {
                if (inTable) {
                    html << parseTable(tableBlock, config.enableHorizontalScroll) << "\n";
                    inTable = false;
                    tableBlock.clear();
                }
            }
            continue;
        }

        // Heading: # ## ### etc.
        if (line.rfind("#", 0) == 0) {
            flushParagraph();
            int level = 1;
            while (level < (int)line.size() && line[level] == '#') ++level;
            if (level <= 6 && level < (int)line.size() && line[level] == ' ') {
                auto heading = line.substr(level + 1);
                html << "<h" << level << ">" << processInline(heading, config) << "</h" << level << ">\n";
                continue;
            }
        }

        // Blockquote
        if (line.rfind("> ", 0) == 0) {
            if (!inBlockquote) {
                flushParagraph();
                html << "<blockquote>";
                inBlockquote = true;
            }
            html << "<p>" << processInline(line.substr(2), config) << "</p>\n";
            continue;
        } else if (inBlockquote) {
            html << "</blockquote>\n";
            inBlockquote = false;
        }

        // Unordered list
        if (line.rfind("- ", 0) == 0 || line.rfind("* ", 0) == 0) {
            if (inOrderedList) { html << "</ol>\n"; inOrderedList = false; }
            if (!inList) { html << "<ul>\n"; inList = true; }
            html << "<li>" << processInline(line.substr(2), config) << "</li>\n";
            continue;
        }

        // Ordered list
        std::regex orderedRe(R"(^(\d+)\.\s)");
        std::smatch omatch;
        if (std::regex_search(line, omatch, orderedRe)) {
            if (inList) { html << "</ul>\n"; inList = false; }
            if (!inOrderedList) { html << "<ol>\n"; inOrderedList = true; }
            auto content = line.substr(omatch.length());
            html << "<li>" << processInline(content, config) << "</li>\n";
            continue;
        }

        // Horizontal rule
        if (line == "---" || line == "***" || line == "___") {
            flushParagraph();
            html << "<hr>\n";
            continue;
        }

        // Empty line: paragraph break
        if (line.empty()) {
            flushParagraph();
            if (!prevLine.empty() && !inBlockquote) {
                html << "<br>\n";
            }
            prevLine = line;
            continue;
        }

        // Regular paragraph text
        if (!inList && !inOrderedList) {
            html << "<p>" << processInline(line, config) << "</p>\n";
        }

        prevLine = line;
    }

    flushParagraph();
    if (inBlockquote) html << "</blockquote>\n";

    return html.str();
}

std::string parseMarkdownTable(const std::string& tableBlock, bool withScroll) {
    return parseTable(tableBlock, withScroll);
}

// ============================================================================
// AST-based inline parser
// ============================================================================

// Original Kotlin: MarkdownParser — parse inline formatting elements
// Returns flat list of inline nodes from text.
// Order: code spans first, then links/images, then bold, then italic.
static std::vector<std::unique_ptr<MarkdownNode>> parseInlineNodes(const std::string& text) {
    std::vector<std::unique_ptr<MarkdownNode>> result;
    if (text.empty()) return result;

    std::string remaining = text;
    size_t pos = 0;

    while (pos < remaining.size()) {
        // Code span: `text`
        {
            std::regex codeRe(R"(`([^`]+)`)");
            std::smatch m;
            std::string sub = remaining.substr(pos);
            if (std::regex_search(sub, m, codeRe) && m.position() == 0) {
                // Before code: plain text
                // (nothing before since m.position() == 0)
                auto node = std::make_unique<MarkdownNode>(MarkdownNodeType::CODE);
                node->content = m[1].str();
                result.push_back(std::move(node));
                pos += m.length();
                continue;
            }
        }

        // Link: [text](url)
        {
            std::regex linkRe(R"(\[([^\]]+)\]\(([^)]+)\))");
            std::smatch m;
            std::string sub = remaining.substr(pos);
            if (std::regex_search(sub, m, linkRe) && m.position() == 0) {
                auto node = std::make_unique<MarkdownNode>(MarkdownNodeType::LINK);
                node->content = m[1].str();
                node->url = m[2].str();
                result.push_back(std::move(node));
                pos += m.length();
                continue;
            }
        }

        // Image: ![alt](url)
        {
            std::regex imgRe(R"(!\[([^\]]*)\]\(([^)]+)\))");
            std::smatch m;
            std::string sub = remaining.substr(pos);
            if (std::regex_search(sub, m, imgRe) && m.position() == 0) {
                auto node = std::make_unique<MarkdownNode>(MarkdownNodeType::IMAGE);
                node->content = m[1].str();
                node->url = m[2].str();
                result.push_back(std::move(node));
                pos += m.length();
                continue;
            }
        }

        // Bold: **text** or __text__
        {
            std::regex boldRe(R"(\*\*([^*]+)\*\*)");
            std::smatch m;
            std::string sub = remaining.substr(pos);
            if (std::regex_search(sub, m, boldRe) && m.position() == 0) {
                auto node = std::make_unique<MarkdownNode>(MarkdownNodeType::BOLD);
                node->content = m[1].str();
                result.push_back(std::move(node));
                pos += m.length();
                continue;
            }
        }
        {
            std::regex boldRe2(R"(__([^_]+)__)");
            std::smatch m;
            std::string sub = remaining.substr(pos);
            if (std::regex_search(sub, m, boldRe2) && m.position() == 0) {
                auto node = std::make_unique<MarkdownNode>(MarkdownNodeType::BOLD);
                node->content = m[1].str();
                result.push_back(std::move(node));
                pos += m.length();
                continue;
            }
        }

        // Italic: *text* or _text_
        {
            std::regex italicRe(R"(\*([^*]+)\*)");
            std::smatch m;
            std::string sub = remaining.substr(pos);
            if (std::regex_search(sub, m, italicRe) && m.position() == 0) {
                auto node = std::make_unique<MarkdownNode>(MarkdownNodeType::ITALIC);
                node->content = m[1].str();
                result.push_back(std::move(node));
                pos += m.length();
                continue;
            }
        }
        {
            std::regex italicRe2(R"(_([^_]+)_)");
            std::smatch m;
            std::string sub = remaining.substr(pos);
            if (std::regex_search(sub, m, italicRe2) && m.position() == 0) {
                auto node = std::make_unique<MarkdownNode>(MarkdownNodeType::ITALIC);
                node->content = m[1].str();
                result.push_back(std::move(node));
                pos += m.length();
                continue;
            }
        }

        // Plain text character
        {
            auto node = std::make_unique<MarkdownNode>(MarkdownNodeType::PLAINTEXT);
            node->content = remaining.substr(pos, 1);
            result.push_back(std::move(node));
            ++pos;
        }
    }

    return result;
}

// ============================================================================
// AST-based Markdown parser
// ============================================================================

// Original Kotlin: MarkdownParser.parse() — parse markdown text into AST
MarkdownAST parseMarkdown(const std::string& markdown) {
    MarkdownAST ast;
    if (markdown.empty()) return ast;

    std::istringstream stream(markdown);
    std::string line;
    bool inCodeBlock = false;
    std::string codeBlockContent;
    std::string codeBlockLang;
    bool inBlockquote = false;
    auto currentQuote = std::make_unique<MarkdownNode>(MarkdownNodeType::QUOTE);
    bool inUnorderedList = false;
    bool inOrderedList = false;
    int orderedListIdx = 0;

    auto flushQuote = [&]() {
        if (inBlockquote && !currentQuote->children.empty()) {
            ast.children.push_back(std::move(currentQuote));
            currentQuote = std::make_unique<MarkdownNode>(MarkdownNodeType::QUOTE);
            inBlockquote = false;
        }
    };

    auto flushUnorderedList = [&]() { inUnorderedList = false; };
    auto flushOrderedList = [&]() { inOrderedList = false; };

    while (std::getline(stream, line)) {
        // Code block fences
        if (line.rfind("```", 0) == 0) {
            if (!inCodeBlock) {
                flushQuote();
                flushUnorderedList();
                flushOrderedList();
                codeBlockLang = line.substr(3);
                while (!codeBlockLang.empty() && codeBlockLang[0] == ' ')
                    codeBlockLang.erase(0, 1);
                inCodeBlock = true;
                codeBlockContent.clear();
                continue;
            } else {
                auto node = std::make_unique<MarkdownNode>(MarkdownNodeType::CODE_BLOCK);
                node->content = codeBlockContent;
                node->codeLanguage = codeBlockLang;
                ast.children.push_back(std::move(node));
                inCodeBlock = false;
                codeBlockContent.clear();
                codeBlockLang.clear();
                continue;
            }
        }

        if (inCodeBlock) {
            if (!codeBlockContent.empty()) codeBlockContent += "\n";
            codeBlockContent += line;
            continue;
        }

        // Heading
        if (line.rfind("#", 0) == 0) {
            flushQuote();
            flushUnorderedList();
            flushOrderedList();
            int level = 1;
            while (level < (int)line.size() && line[level] == '#') ++level;
            if (level <= 6 && level < (int)line.size() && line[level] == ' ') {
                auto node = std::make_unique<MarkdownNode>(MarkdownNodeType::HEADING);
                node->headingLevel = level;
                auto headingText = line.substr(level + 1);
                trim(headingText);
                node->children = parseInlineNodes(headingText);
                ast.children.push_back(std::move(node));
                continue;
            }
        }

        // Blockquote
        if (line.rfind("> ", 0) == 0) {
            flushUnorderedList();
            flushOrderedList();
            if (!inBlockquote) {
                inBlockquote = true;
            }
            auto pNode = std::make_unique<MarkdownNode>(MarkdownNodeType::PARAGRAPH);
            pNode->children = parseInlineNodes(line.substr(2));
            currentQuote->children.push_back(std::move(pNode));
            continue;
        } else if (inBlockquote && !line.empty()) {
            // Continuation of blockquote without > prefix (treated as same quote)
            auto pNode = std::make_unique<MarkdownNode>(MarkdownNodeType::PARAGRAPH);
            pNode->children = parseInlineNodes(line);
            currentQuote->children.push_back(std::move(pNode));
            continue;
        } else if (inBlockquote) {
            flushQuote();
            continue;
        }

        // Unordered list: -, *, +
        if ((line.rfind("- ", 0) == 0) || (line.rfind("* ", 0) == 0) || (line.rfind("+ ", 0) == 0)) {
            flushQuote();
            flushOrderedList();
            auto node = std::make_unique<MarkdownNode>(MarkdownNodeType::LIST_ITEM);
            node->content = inUnorderedList ? "unordered" : "unordered_start";
            auto itemText = line.substr(2);
            trim(itemText);
            node->children = parseInlineNodes(itemText);
            ast.children.push_back(std::move(node));
            inUnorderedList = true;
            continue;
        }

        // Ordered list: 1. 2. etc.
        {
            std::regex orderedRe(R"(^(\d+)\.\s)");
            std::smatch omatch;
            if (std::regex_search(line, omatch, orderedRe)) {
                flushQuote();
                flushUnorderedList();
                auto node = std::make_unique<MarkdownNode>(MarkdownNodeType::LIST_ITEM);
                node->content = inOrderedList ? "ordered" : "ordered_start";
                auto itemText = line.substr(omatch.length());
                trim(itemText);
                node->children = parseInlineNodes(itemText);
                ast.children.push_back(std::move(node));
                inOrderedList = true;
                continue;
            }
        }

        // Horizontal rule
        if (line == "---" || line == "***" || line == "___") {
            flushQuote();
            flushUnorderedList();
            flushOrderedList();
            auto node = std::make_unique<MarkdownNode>(MarkdownNodeType::PARAGRAPH);
            node->content = "<hr>";
            ast.children.push_back(std::move(node));
            continue;
        }

        // Empty line
        if (line.empty()) {
            flushQuote();
            flushUnorderedList();
            flushOrderedList();
            continue;
        }

        // Paragraph
        flushQuote();
        flushUnorderedList();
        flushOrderedList();
        {
            auto node = std::make_unique<MarkdownNode>(MarkdownNodeType::PARAGRAPH);
            node->children = parseInlineNodes(line);
            ast.children.push_back(std::move(node));
        }
    }

    // Flush any remaining state
    if (inCodeBlock) {
        auto node = std::make_unique<MarkdownNode>(MarkdownNodeType::CODE_BLOCK);
        node->content = codeBlockContent;
        node->codeLanguage = codeBlockLang;
        ast.children.push_back(std::move(node));
    }
    flushQuote();

    return ast;
}

// ============================================================================
// AST → HTML renderer
// ============================================================================

static void renderNodeToHtml(const MarkdownNode& node, std::ostringstream& out, const MdConfig& config);

static void renderChildrenToHtml(const std::vector<std::unique_ptr<MarkdownNode>>& children,
                                  std::ostringstream& out, const MdConfig& config) {
    for (const auto& child : children) {
        renderNodeToHtml(*child, out, config);
    }
}

static void renderNodeToHtml(const MarkdownNode& node, std::ostringstream& out, const MdConfig& config) {
    switch (node.type) {
        case MarkdownNodeType::PARAGRAPH:
            if (node.content == "<hr>") {
                out << "<hr>\n";
            } else {
                out << "<p>";
                renderChildrenToHtml(node.children, out, config);
                out << "</p>\n";
            }
            break;
        case MarkdownNodeType::HEADING: {
            int lvl = node.headingLevel;
            if (lvl < 1) lvl = 1;
            if (lvl > 6) lvl = 6;
            out << "<h" << lvl << ">";
            renderChildrenToHtml(node.children, out, config);
            out << "</h" << lvl << ">\n";
            break;
        }
        case MarkdownNodeType::CODE_BLOCK:
            out << "<pre><code";
            if (!node.codeLanguage.empty())
                out << " class=\"language-" << escapeHtml(node.codeLanguage) << "\"";
            out << ">" << escapeHtml(node.content) << "</code></pre>\n";
            break;
        case MarkdownNodeType::QUOTE:
            out << "<blockquote>\n";
            renderChildrenToHtml(node.children, out, config);
            out << "</blockquote>\n";
            break;
        case MarkdownNodeType::LIST_ITEM:
            // List wrapping is handled by the parent container logic
            out << "<li>";
            renderChildrenToHtml(node.children, out, config);
            out << "</li>\n";
            break;
        case MarkdownNodeType::LINK:
            if (config.enableLinks) {
                out << "<a href=\"" << escapeHtml(node.url) << "\">"
                    << escapeHtml(node.content) << "</a>";
            } else {
                out << escapeHtml(node.content);
            }
            break;
        case MarkdownNodeType::IMAGE:
            if (config.enableImages) {
                out << "<img src=\"" << escapeHtml(node.url)
                    << "\" alt=\"" << escapeHtml(node.content) << "\">";
            } else {
                out << escapeHtml(node.content);
            }
            break;
        case MarkdownNodeType::BOLD:
            out << "<strong>" << escapeHtml(node.content) << "</strong>";
            break;
        case MarkdownNodeType::ITALIC:
            out << "<em>" << escapeHtml(node.content) << "</em>";
            break;
        case MarkdownNodeType::CODE:
            out << "<code>" << escapeHtml(node.content) << "</code>";
            break;
        case MarkdownNodeType::PLAINTEXT:
            out << escapeHtml(node.content);
            break;
    }
}

// Original Kotlin: EventHtmlRenderer.render() — AST→HTML
std::string renderMarkdownToHtml(const MarkdownAST& ast, const MdConfig& config) {
    std::ostringstream html;
    bool inUnorderedList = false;
    bool inOrderedList = false;

    for (size_t i = 0; i < ast.children.size(); ++i) {
        const auto& node = ast.children[i];

        // Handle list grouping
        if (node->type == MarkdownNodeType::LIST_ITEM) {
            bool isOrdered = (node->content.find("ordered") == 0);

            // Start new list if needed
            if (isOrdered && !inOrderedList) {
                if (inUnorderedList) { html << "</ul>\n"; inUnorderedList = false; }
                html << "<ol>\n";
                inOrderedList = true;
            } else if (!isOrdered && !inUnorderedList) {
                if (inOrderedList) { html << "</ol>\n"; inOrderedList = false; }
                html << "<ul>\n";
                inUnorderedList = true;
            }

            renderNodeToHtml(*node, html, config);

            // Peek ahead: close list if next node isn't same list type
            bool nextSameList = false;
            if (i + 1 < ast.children.size()) {
                const auto& next = ast.children[i + 1];
                if (next->type == MarkdownNodeType::LIST_ITEM) {
                    bool nextOrdered = (next->content.find("ordered") == 0);
                    nextSameList = (nextOrdered == isOrdered);
                }
            }
            if (!nextSameList) {
                if (inOrderedList) { html << "</ol>\n"; inOrderedList = false; }
                if (inUnorderedList) { html << "</ul>\n"; inUnorderedList = false; }
            }
        } else {
            if (inOrderedList) { html << "</ol>\n"; inOrderedList = false; }
            if (inUnorderedList) { html << "</ul>\n"; inUnorderedList = false; }
            renderNodeToHtml(*node, html, config);
        }
    }

    if (inOrderedList) html << "</ol>\n";
    if (inUnorderedList) html << "</ul>\n";

    return html.str();
}

// ============================================================================
// AST → Plain Text renderer (strip formatting)
// ============================================================================

static void renderNodeToPlainText(const MarkdownNode& node, std::ostringstream& out) {
    switch (node.type) {
        case MarkdownNodeType::PLAINTEXT:
        case MarkdownNodeType::BOLD:
        case MarkdownNodeType::ITALIC:
        case MarkdownNodeType::CODE:
        case MarkdownNodeType::LINK:
            out << node.content;
            break;
        case MarkdownNodeType::IMAGE:
            if (!node.content.empty())
                out << "[" << node.content << "]";
            break;
        case MarkdownNodeType::PARAGRAPH:
            renderChildrenToPlainText(node.children, out);
            out << "\n";
            break;
        case MarkdownNodeType::HEADING:
            renderChildrenToPlainText(node.children, out);
            out << "\n";
            break;
        case MarkdownNodeType::CODE_BLOCK:
            out << node.content << "\n";
            break;
        case MarkdownNodeType::QUOTE:
            renderChildrenToPlainText(node.children, out);
            break;
        case MarkdownNodeType::LIST_ITEM:
            out << "- ";
            renderChildrenToPlainText(node.children, out);
            out << "\n";
            break;
    }
}

static void renderChildrenToPlainText(const std::vector<std::unique_ptr<MarkdownNode>>& children,
                                       std::ostringstream& out) {
    for (const auto& child : children) {
        renderNodeToPlainText(*child, out);
    }
}

std::string renderMarkdownToPlainText(const MarkdownAST& ast) {
    std::ostringstream out;
    for (const auto& node : ast.children) {
        renderNodeToPlainText(*node, out);
    }
    auto result = out.str();
    // Trim trailing newline
    while (!result.empty() && result.back() == '\n') result.pop_back();
    return result;
}

// ============================================================================
// HTML validation (isFormattedBodyValid)
// ============================================================================

// Original Kotlin: MarkdownParser.isFormattedTextPertinent() — check if HTML is valid/formatted
[[nodiscard]] bool isFormattedBodyValid(const std::string& html) {
    if (html.empty()) return false;

    // Quick check: must contain at least one HTML tag
    if (html.find('<') == std::string::npos || html.find('>') == std::string::npos)
        return false;

    // Check balanced angle brackets
    int balance = 0;
    for (char c : html) {
        if (c == '<') ++balance;
        if (c == '>') --balance;
        if (balance < 0) return false; // unopened >
    }
    if (balance != 0) return false; // unclosed <

    // Must have real content (not just whitespace between tags)
    bool hasContent = false;
    bool inTag = false;
    for (char c : html) {
        if (c == '<') { inTag = true; continue; }
        if (c == '>') { inTag = false; continue; }
        if (!inTag && !std::isspace(static_cast<unsigned char>(c))) {
            hasContent = true;
            break;
        }
    }
    if (!hasContent) return false;

    // Check for common dangerous content
    std::string lower;
    lower.reserve(html.size());
    for (char c : html) lower += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));

    if (lower.find("<script") != std::string::npos) return false;
    if (lower.find("javascript:") != std::string::npos) return false;
    if (lower.find("onerror=") != std::string::npos) return false;
    if (lower.find("onload=") != std::string::npos) return false;

    return true;
}

// ============================================================================
// HTML sanitizer
// ============================================================================

// Original Kotlin: EventHtmlRenderer — strip dangerous HTML tags and attributes
std::string sanitizeHtml(const std::string& html) {
    std::string result;
    result.reserve(html.size());

    // Safe inline tags that are allowed through
    static const std::unordered_set<std::string> safeTags = {
        "p", "br", "b", "i", "em", "strong", "u", "s", "del", "strike",
        "code", "pre", "blockquote", "q", "cite",
        "a", "img", "font", "span", "div",
        "h1", "h2", "h3", "h4", "h5", "h6",
        "ul", "ol", "li", "dl", "dt", "dd",
        "table", "thead", "tbody", "tfoot", "tr", "th", "td",
        "hr", "br", "sub", "sup",
        "mx-reply"
    };

    size_t pos = 0;
    while (pos < html.size()) {
        if (html[pos] == '<') {
            size_t end = html.find('>', pos);
            if (end == std::string::npos) {
                result += html.substr(pos);
                break;
            }

            std::string tag = html.substr(pos, end - pos + 1);

            // Parse tag name (lowercase for comparison)
            std::string tagName;
            bool isClosing = false;
            size_t nameStart = 1;
            if (tag.size() > 1 && tag[1] == '/') {
                isClosing = true;
                nameStart = 2;
            }
            for (size_t i = nameStart; i < tag.size(); ++i) {
                char c = tag[i];
                if (c == ' ' || c == '>' || c == '/' || c == '\n') break;
                tagName += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
            }

            if (safeTags.find(tagName) != safeTags.end()) {
                // Strip event handler attributes
                std::string cleanTag;
                cleanTag.reserve(tag.size());
                bool inAttr = false;
                bool skipAttr = false;
                std::string attrName;
                for (size_t i = 0; i < tag.size(); ++i) {
                    char c = tag[i];
                    if (!inAttr && c == ' ') {
                        inAttr = true;
                        skipAttr = false;
                        attrName.clear();
                        cleanTag += c;
                        continue;
                    }
                    if (inAttr) {
                        if (c == '=' || c == ' ' || c == '>' || c == '/') {
                            // Check if attr is dangerous
                            std::string lowerAttr;
                            for (char ac : attrName)
                                lowerAttr += static_cast<char>(std::tolower(static_cast<unsigned char>(ac)));
                            if (lowerAttr.rfind("on", 0) == 0) {
                                // Event handler: skip the whole attribute
                                if (!skipAttr) {
                                    // Remove the attr name from output
                                    while (!cleanTag.empty() && cleanTag.back() == ' ')
                                        cleanTag.pop_back();
                                }
                                skipAttr = true;
                            }
                            if (c == '=' && !skipAttr) cleanTag += c;
                            if (c == ' ' && !skipAttr) cleanTag += c;
                            if (c == '>' || c == '/') {
                                cleanTag += c;
                                inAttr = false;
                                if (c == '>') break;
                            }
                            attrName.clear();
                            if (c == '>') break;
                        } else {
                            attrName += c;
                            cleanTag += c;
                        }
                        continue;
                    }
                    cleanTag += c;
                }
                result += cleanTag;
            }
            // If tag not safe, skip it entirely (strip)

            pos = end + 1;
        } else {
            result += html[pos];
            ++pos;
        }
    }

    return result;
}

// ============================================================================
// HTML compactor (HtmlCompactor.kt)
// ============================================================================

// Original Kotlin: HtmlCompactor.kt — compact HTML by removing redundant tags
std::string compactHtml(const std::string& html) {
    std::string result = html;

    // Remove empty tags like <p></p>, <b></b>, <i></i>, etc.
    const std::regex emptyTagRe(R"(<(p|b|i|em|strong|u|s|del|span|div|font)\s*>\s*</\1>)", std::regex::icase);
    result = std::regex_replace(result, emptyTagRe, "");

    // Remove redundant wrapping <p> around whole content
    {
        auto trimmed = result;
        ltrim(trimmed);
        rtrim(trimmed);
        if (trimmed.size() > 7 && trimmed.rfind("<p>", 0) == 0) {
            std::regex fullWrap(R"(^<p>(.*)</p>\n?$)");
            std::smatch m;
            if (std::regex_search(trimmed, m, fullWrap)) {
                // Only unwrap if there's no other <p> inside
                auto inner = m[1].str();
                if (inner.find("<p>") == std::string::npos && inner.find("<p ") == std::string::npos) {
                    result = inner;
                }
            }
        }
    }

    // Collapse multiple consecutive <br> tags
    {
        std::regex multiBr(R"((<br\s*/?>\s*){2,})", std::regex::icase);
        result = std::regex_replace(result, multiBr, "<br>");
    }

    // Trim leading/trailing whitespace
    trim(result);

    return result;
}

// ============================================================================
// HTML → Plain Text
// ============================================================================

// Original Kotlin: EventHtmlRenderer — extract plain text from HTML
std::string extractPlainTextFromHtml(const std::string& html) {
    std::string result;
    result.reserve(html.size());
    bool inTag = false;
    bool prevWasSpace = false;
    bool inPreBlock = false;

    std::string lower;
    for (size_t i = 0; i < html.size(); ++i) {
        char c = html[i];

        if (c == '<') {
            inTag = true;
            // Check if it's a </pre> or </code> to exit pre block
            continue;
        }

        if (c == '>') {
            inTag = false;
            continue;
        }

        if (inTag) continue;

        // Handle <br> tags already stripped; this handles regular content
        if (std::isspace(static_cast<unsigned char>(c))) {
            if (!prevWasSpace) {
                result += ' ';
                prevWasSpace = true;
            }
        } else {
            result += c;
            prevWasSpace = false;
        }
    }

    // Normalize whitespace
    trim(result);

    return result;
}

// ============================================================================
// Matrix HTML ↔ Standard HTML
// ============================================================================

// Original Kotlin: EventHtmlRenderer — convert Matrix HTML to standard HTML
std::string matrixToHtml(const std::string& matrixHtml) {
    std::string result = matrixHtml;

    // Convert <mx-reply> to nested <blockquote> structure
    {
        std::regex mxReplyOpen(R"(<mx-reply\s*>)", std::regex::icase);
        result = std::regex_replace(result, mxReplyOpen, "<blockquote class=\"mx-reply\">");
    }
    {
        std::regex mxReplyClose(R"(</mx-reply\s*>)", std::regex::icase);
        result = std::regex_replace(result, mxReplyClose, "</blockquote>");
    }

    return result;
}

// Original Kotlin: EventHtmlRenderer — convert standard HTML to Matrix format
std::string htmlToMatrix(const std::string& html) {
    std::string result = html;

    // Convert <blockquote class="mx-reply"> to <mx-reply>
    {
        std::regex bqRe(R"(<blockquote class="mx-reply"\s*>)", std::regex::icase);
        result = std::regex_replace(result, bqRe, "<mx-reply>");
    }
    {
        std::regex bqClose(R"(</blockquote\s*>)", std::regex::icase);
        // Only replace if nested inside an mx-reply context — simple approach:
        // We replace all </blockquote> that follow <mx-reply>
        std::string out;
        int depth = 0;
        size_t pos = 0;
        while (pos < result.size()) {
            size_t openPos = result.find("<mx-reply>", pos);
            size_t closePos = result.find("</blockquote>", pos);

            if (openPos != std::string::npos && (closePos == std::string::npos || openPos < closePos)) {
                out += result.substr(pos, openPos - pos);
                out += "<mx-reply>";
                pos = openPos + 10;
                ++depth;
            } else if (closePos != std::string::npos) {
                out += result.substr(pos, closePos - pos);
                if (depth > 0) {
                    out += "</mx-reply>";
                    --depth;
                } else {
                    out += "</blockquote>";
                }
                pos = closePos + 13;
            } else {
                out += result.substr(pos);
                break;
            }
        }
        result = out;
    }

    return result;
}

// ============================================================================
// mx-reply builder
// ============================================================================

// Original Kotlin: EventHtmlRenderer.MxReplyTagHandler — build mx-reply structure
std::string wrapInMatrixReply(
    const std::string& replyHtml,
    const std::string& eventId,
    const std::string& userId) {
    std::ostringstream out;

    // Build mx-reply structure per Matrix spec
    out << "<mx-reply>";
    out << "<blockquote>";
    out << "<a href=\"https://matrix.to/#/" << escapeHtml(eventId) << "\">";
    out << "In reply to</a>";
    out << "<a href=\"https://matrix.to/#/" << escapeHtml(userId) << "\">";
    out << escapeHtml(userId);
    out << "</a><br>";
    out << replyHtml;
    out << "</blockquote>";
    out << "</mx-reply>";

    return out.str();
}

} // namespace progressive
