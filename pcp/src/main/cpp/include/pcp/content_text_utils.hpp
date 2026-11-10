#pragma once
#include <string>
#include <vector>

namespace pcp {

// ==== Content Text Utilities ====
// Ported from: org.matrix.android.sdk.api.util.ContentUtils.kt
// Matrix reply format: <mx-reply>quoted</mx-reply>new text

// Extract useful text from a plain-text reply.
// Strips quoted lines (starting with ">") and returns only the new text.
// If format is malformed, returns original body.
// Original Kotlin: ContentUtils.extractUsefulTextFromReply(repliedBody)
std::string extractUsefulTextFromReply(const std::string& repliedBody);

// Extract useful text from an HTML reply.
// Strips <mx-reply>...</mx-reply> block and returns the new text after it.
// Original Kotlin: ContentUtils.extractUsefulTextFromHtmlReply(repliedBody)
std::string extractUsefulTextFromHtmlReply(const std::string& repliedBody);

// Ensure formatted body contains mx-reply end tag in edited replies.
// If the new formatted body lacks </mx-reply> but original has it,
// append the original reply block to the new body.
// Original Kotlin: ContentUtils.ensureCorrectFormattedBodyInTextReply
std::string ensureCorrectFormattedBody(const std::string& newBody, 
                                        const std::string& newFormattedBody,
                                        const std::string& originalFormattedBody);

// Format spoiler text from HTML to plain text with █ characters.
// Removes <span data-mx-spoiler>reason</span> tags,
// replaces spoiler content with █ (Unicode U+2588) characters.
// Original Kotlin: ContentUtils.formatSpoilerTextFromHtml(formattedBody)
std::string formatSpoilerTextFromHtml(const std::string& formattedBody);

// Check if text is likely a Matrix reply (starts with ">" or <mx-reply>).
bool isReplyText(const std::string& text);

// Check if text is likely an HTML-formatted reply.
bool isHtmlReply(const std::string& text);

} // namespace pcp
