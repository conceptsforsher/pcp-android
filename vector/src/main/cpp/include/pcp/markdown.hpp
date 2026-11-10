#ifndef PCP_MARKDOWN_HPP
#define PCP_MARKDOWN_HPP

#include <string>

namespace pcp {

struct MdConfig {
    bool enableTables = true;
    bool enableLinks = true;
    bool enableCodeBlocks = true;
    bool enableImages = false; // matrix-specific: images handled separately
    bool enableHtmlPassthrough = true; // pass through <del>, <u>, <font> (matrix extensions)
    bool enableHorizontalScroll = true; // wrap tables in scrollable container
};

// Render markdown to HTML suitable for Android TextView rendering.
// Supports: headings, bold, italic, strikethrough, code, links, blockquotes,
//           ordered/unordered lists, and tables (with horizontal scroll).
std::string markdownToHtml(const std::string& markdown, const MdConfig& config = MdConfig{});

// Parse a markdown table and return HTML with horizontal scroll wrapper.
// Table format:
// | Header 1 | Header 2 |
// |----------|----------|
// | Cell 1   | Cell 2   |
std::string parseMarkdownTable(const std::string& tableBlock, bool withScroll);

} // namespace pcp

#endif // PCP_MARKDOWN_HPP
