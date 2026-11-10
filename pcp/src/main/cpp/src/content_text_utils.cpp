#include "pcp/content_text_utils.hpp"
#include <regex>
#include <sstream>
#include <algorithm>

namespace pcp {

static const char* MX_REPLY_START = "<mx-reply>";
static const char* MX_REPLY_END = "</mx-reply>";
static const char* SPOILER_SPAN_START = "<span data-mx-spoiler";
static const char* SPOILER_SPAN_END = "</span>";
static const char* SPOILER_CHAR = "\xE2\x96\x88"; // █ U+2588 UTF-8

std::string extractUsefulTextFromReply(const std::string& repliedBody) {
    // Original Kotlin:
    //   val lines = repliedBody.lines()
    //   var wellFormed = repliedBody.startsWith(">")
    //   var endOfPreviousFound = false
    //   val usefulLines = ArrayList<String>()
    //   lines.forEach {
    //       if (it == "") { endOfPreviousFound = true; return@forEach }
    //       if (!endOfPreviousFound) { wellFormed = wellFormed && it.startsWith(">") }
    //       else { usefulLines.add(it) }
    //   }
    //   return usefulLines.joinToString("\n").takeIf { wellFormed } ?: repliedBody

    if (repliedBody.empty()) return repliedBody;
    
    bool wellFormed = (repliedBody[0] == '>');
    bool endOfPreviousFound = false;
    std::vector<std::string> usefulLines;
    
    std::istringstream stream(repliedBody);
    std::string line;
    while (std::getline(stream, line)) {
        // Handle \r at end of line
        if (!line.empty() && line.back() == '\r') line.pop_back();
        
        if (line.empty()) {
            endOfPreviousFound = true;
            continue;
        }
        if (!endOfPreviousFound) {
            if (!line.empty() && line[0] != '>') wellFormed = false;
        } else {
            usefulLines.push_back(line);
        }
    }
    
    if (!wellFormed) return repliedBody;
    
    std::ostringstream result;
    for (size_t i = 0; i < usefulLines.size(); i++) {
        if (i > 0) result << '\n';
        result << usefulLines[i];
    }
    return result.str();
}

std::string extractUsefulTextFromHtmlReply(const std::string& repliedBody) {
    // Original Kotlin:
    //   if (repliedBody.startsWith(MX_REPLY_START_TAG)) {
    //       val closingTagIndex = repliedBody.lastIndexOf(MX_REPLY_END_TAG)
    //       if (closingTagIndex != -1) {
    //           return repliedBody.substring(closingTagIndex + MX_REPLY_END_TAG.length).trim()
    //       }
    //   }
    //   return repliedBody

    if (repliedBody.rfind(MX_REPLY_START, 0) != 0) return repliedBody;
    
    std::string endTag = MX_REPLY_END;
    auto closingPos = repliedBody.rfind(endTag);
    if (closingPos == std::string::npos) return repliedBody;
    
    std::string result = repliedBody.substr(closingPos + endTag.length());
    
    // Trim whitespace
    auto start = result.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) return "";
    auto end = result.find_last_not_of(" \t\n\r");
    return result.substr(start, end - start + 1);
}

std::string ensureCorrectFormattedBody(const std::string& newBody,
                                        const std::string& newFormattedBody,
                                        const std::string& originalFormattedBody) {
    // Original Kotlin:
    //   when {
    //       messageTextContent.formattedBody != null &&
    //           !messageTextContent.formattedBody.contains(MX_REPLY_END_TAG) &&
    //           originalFormattedBody.contains(MX_REPLY_END_TAG) -> {
    //           val newFormattedBody = originalFormattedBody.replaceAfterLast(MX_REPLY_END_TAG, messageTextContent.body)
    //           messageTextContent.copy(formattedBody = newFormattedBody, format = FORMAT_MATRIX_HTML)
    //       }
    //       else -> messageTextContent
    //   }

    if (!newFormattedBody.empty() && 
        newFormattedBody.find(MX_REPLY_END) == std::string::npos &&
        originalFormattedBody.find(MX_REPLY_END) != std::string::npos) {
        
        auto pos = originalFormattedBody.rfind(MX_REPLY_END);
        if (pos != std::string::npos) {
            return originalFormattedBody.substr(0, pos + std::string(MX_REPLY_END).length()) + newBody;
        }
    }
    
    return newFormattedBody;
}

std::string formatSpoilerTextFromHtml(const std::string& formattedBody) {
    // Original Kotlin:
    //   formattedBody
    //       .replace("(?<=<span data-mx-spoiler)=\\\".+?\\\">".toRegex(), ">")
    //       .replace("(?<=<span data-mx-spoiler>).+?(?=</span>)".toRegex()) { SPOILER_CHAR.repeat(it.value.length) }
    //       .unescapeHtml()

    std::string result = formattedBody;
    
    // Step 1: Replace <span data-mx-spoiler="reason"> → <span data-mx-spoiler>
    std::regex spoiler_reason(R"(<span data-mx-spoiler="[^"]*">)");
    result = std::regex_replace(result, spoiler_reason, "<span data-mx-spoiler>");
    
    // Step 2: Replace content inside <span data-mx-spoiler>...</span> with █ chars
    std::string spoiler_tag = "<span data-mx-spoiler>";
    size_t pos = 0;
    while ((pos = result.find(spoiler_tag, pos)) != std::string::npos) {
        auto contentStart = pos + spoiler_tag.length();
        auto endPos = result.find(SPOILER_SPAN_END, contentStart);
        if (endPos == std::string::npos) break;
        
        // Replace content between tags with spoiler chars
        size_t contentLen = endPos - contentStart;
        std::string replacement;
        for (size_t i = 0; i < contentLen; i++) {
            replacement += SPOILER_CHAR;
        }
        result.replace(contentStart, contentLen, replacement);
        pos = endPos + std::string(SPOILER_SPAN_END).length();
    }
    
    // Step 3: Unescape HTML entities (basic)
    // &amp; → &
    std::string unescaped;
    for (size_t i = 0; i < result.length(); i++) {
        if (result.substr(i, 5) == "&amp;") { unescaped += '&'; i += 4; }
        else if (result.substr(i, 4) == "&lt;") { unescaped += '<'; i += 3; }
        else if (result.substr(i, 4) == "&gt;") { unescaped += '>'; i += 3; }
        else if (result.substr(i, 6) == "&quot;") { unescaped += '"'; i += 5; }
        else if (result.substr(i, 6) == "&apos;") { unescaped += '\''; i += 5; }
        else { unescaped += result[i]; }
    }
    
    return unescaped;
}

bool isReplyText(const std::string& text) {
    if (text.empty()) return false;
    return text[0] == '>' || text.rfind(MX_REPLY_START, 0) == 0;
}

bool isHtmlReply(const std::string& text) {
    return text.rfind(MX_REPLY_START, 0) == 0;
}

} // namespace pcp
