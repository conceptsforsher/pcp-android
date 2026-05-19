#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_set>

namespace progressive {

// Original Kotlin: MarkdownParser.kt — markdown node types
enum class MarkdownNodeType {
    PARAGRAPH,
    HEADING,
    CODE_BLOCK,
    QUOTE,
    LIST_ITEM,
    LINK,
    IMAGE,
    BOLD,
    ITALIC,
    CODE,
    PLAINTEXT
};

// Original Kotlin: MarkdownParser.kt — AST node
struct MarkdownNode {
    MarkdownNodeType type = MarkdownNodeType::PLAINTEXT;
    std::string content;
    std::string url;          // for links, images
    int headingLevel = 1;     // for headings (1-6)
    std::string codeLanguage; // for code blocks
    std::vector<std::unique_ptr<MarkdownNode>> children;

    MarkdownNode() = default;
    explicit MarkdownNode(MarkdownNodeType t) : type(t) {}
};

// Original Kotlin: MarkdownParser.kt — parsed document as tree
struct MarkdownAST {
    std::vector<std::unique_ptr<MarkdownNode>> children;

    [[nodiscard]] bool empty() const { return children.empty(); }
};

struct MdConfig {
    bool enableTables = true;
    bool enableLinks = true;
    bool enableCodeBlocks = true;
    bool enableImages = false; // matrix-specific: images handled separately
    bool enableHtmlPassthrough = true; // pass through <del>, <u>, <font> (matrix extensions)
    bool enableHorizontalScroll = true; // wrap tables in scrollable container
};

// Original Kotlin: MarkdownParser.parse() — basic markdown→AST parser
// Supports: headings (# ## ###), bold (**text**), italic (*text*), code (`text`),
// code blocks (```...```), block quotes (> text), unordered lists (- * +),
// ordered lists (1. 2.), links [text](url), images ![alt](url), line breaks, paragraphs.
MarkdownAST parseMarkdown(const std::string& markdown);

// Original Kotlin: EventHtmlRenderer.render() — AST→HTML renderer
std::string renderMarkdownToHtml(const MarkdownAST& ast, const MdConfig& config = MdConfig{});

// AST→plain text renderer (strip formatting)
std::string renderMarkdownToPlainText(const MarkdownAST& ast);

// Original Kotlin: MarkdownParser.isFormattedTextPertinent() — check if formatted body is valid HTML
[[nodiscard]] bool isFormattedBodyValid(const std::string& html);

// Original Kotlin: EventHtmlRenderer — strip dangerous HTML tags/attributes (script, iframe, onclick, etc.)
std::string sanitizeHtml(const std::string& html);

// Original Kotlin: HtmlCompactor.kt — remove redundant/wrapping tags
std::string compactHtml(const std::string& html);

// Original Kotlin: EventHtmlRenderer — extract plain text from HTML (strip all tags)
std::string extractPlainTextFromHtml(const std::string& html);

// Original Kotlin: EventHtmlRenderer.MxReplyTagHandler — convert Matrix HTML to standard HTML
// Handles mx-reply, blockquote nesting, etc.
std::string matrixToHtml(const std::string& matrixHtml);

// Convert standard HTML back to Matrix format
std::string htmlToMatrix(const std::string& html);

// Existing: markdown→HTML direct renderer (kept for backward compatibility)
std::string markdownToHtml(const std::string& markdown, const MdConfig& config = MdConfig{});
std::string parseMarkdownTable(const std::string& tableBlock, bool withScroll);

// Original Kotlin: EventHtmlRenderer.MxReplyTagHandler — Matrix HTML tag constants
namespace MatrixHtmlTag {
    constexpr const char* MX_REPLY   = "mx-reply";
    constexpr const char* BLOCKQUOTE = "blockquote";
    constexpr const char* FONT       = "font";
    constexpr const char* SPAN       = "span";
    constexpr const char* A          = "a";
    constexpr const char* CODE       = "code";
    constexpr const char* PRE        = "pre";
    constexpr const char* P          = "p";
    constexpr const char* BR         = "br";
    constexpr const char* UL         = "ul";
    constexpr const char* OL         = "ol";
    constexpr const char* LI         = "li";
    constexpr const char* STRONG     = "strong";
    constexpr const char* EM         = "em";
    constexpr const char* DEL        = "del";
    constexpr const char* U          = "u";
    constexpr const char* H1         = "h1";
    constexpr const char* H2         = "h2";
    constexpr const char* H3         = "h3";
    constexpr const char* H4         = "h4";
    constexpr const char* H5         = "h5";
    constexpr const char* H6         = "h6";
    constexpr const char* IMG        = "img";
    constexpr const char* HR         = "hr";
    constexpr const char* TABLE      = "table";
    constexpr const char* THEAD      = "thead";
    constexpr const char* TBODY      = "tbody";
    constexpr const char* TR         = "tr";
    constexpr const char* TH         = "th";
    constexpr const char* TD         = "td";
}

// Original Kotlin: EventHtmlRenderer.MxReplyTagHandler — build mx-reply structure
// Returns HTML with mx-reply blockquote wrapper and fallback body.
std::string wrapInMatrixReply(
    const std::string& replyHtml,
    const std::string& eventId,
    const std::string& userId);

} // namespace progressive
