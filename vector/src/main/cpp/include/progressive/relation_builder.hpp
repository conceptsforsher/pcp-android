#pragma once

#include <string>

namespace progressive {

// ==== Event Relation Builder ====
//
// Builds the "m.relates_to" JSON content for Matrix events.
// Used when creating replies, edits, reactions, and thread messages.
// Original Kotlin: RelationDefaultContent, ReplyToContent, EventFactory

// Build a reply relation (m.in_reply_to).
// Used when replying to a specific event.
inline std::string buildReplyRelation(const std::string& eventId) {
    return R"({"m.in_reply_to":{"event_id":")" + eventId + R"("}})";
}

// Build a thread relation (m.thread).
// rootEventId: the thread root event
// latestEventId: the latest event in the thread (for fallback rendering)
inline std::string buildThreadRelation(
    const std::string& rootEventId,
    const std::string& latestEventId,
    bool isFallingBack = true)
{
    return R"({"rel_type":"m.thread","event_id":")" + rootEventId +
           R"(","m.in_reply_to":{"event_id":")" + latestEventId +
           R"("},"is_falling_back":)" + (isFallingBack ? "true" : "false") + "}";
}

// Build an edit (m.replace) relation.
// eventId: the event being edited
inline std::string buildReplaceRelation(const std::string& eventId) {
    return R"({"rel_type":"m.replace","event_id":")" + eventId + R"("})";
}

// Build a reaction (m.annotation) relation.
// eventId: the event being reacted to
// key: the reaction emoji/text
inline std::string buildReactionRelation(const std::string& eventId, const std::string& key) {
    return R"({"rel_type":"m.annotation","event_id":")" + eventId +
           R"(","key":")" + key + R"("})";
}

// Build a reference (m.reference) relation.
inline std::string buildReferenceRelation(const std::string& eventId) {
    return R"({"rel_type":"m.reference","event_id":")" + eventId + R"("})";
}

// Wrap content JSON with a relation.
// content: the message content JSON (e.g. {"msgtype":"m.text","body":"hello"})
// relationJson: the relation JSON from one of the builders above
// Returns: content with "m.relates_to" added
inline std::string wrapWithRelation(const std::string& contentJson, const std::string& relationJson) {
    if (contentJson.empty() || contentJson.size() < 2) return contentJson;
    std::string result = contentJson;
    result.pop_back(); // Remove closing }
    if (result.back() != '{') result += ",";
    result += R"("m.relates_to":)" + relationJson + "}";
    return result;
}

// ==== New Content (for edits) ====

// Build m.new_content for an edit event.
// The new_content field contains the replacement content.
inline std::string buildNewContent(const std::string& msgType, const std::string& body,
                                   const std::string& formattedBody = "",
                                   const std::string& format = "org.matrix.custom.html")
{
    std::string result = R"({"msgtype":")" + msgType + R"(","body":")" + body + R"(")";
    if (!formattedBody.empty()) {
        result += R"(,"format":")" + format + R"(","formatted_body":")" + formattedBody + R"(")";
    }
    result += "}";
    return result;
}

// Build a full edit event content (with m.relates_to and m.new_content).
inline std::string buildEditContent(
    const std::string& originalEventId,
    const std::string& msgType,
    const std::string& newBody,
    const std::string& newFormattedBody = "")
{
    std::string content = R"({"msgtype":")" + msgType + R"(","body":" * edited")" + R"(")";
    std::string relation = buildReplaceRelation(originalEventId);
    std::string newContent = buildNewContent(msgType, newBody, newFormattedBody);

    // Combine: content + m.relates_to + m.new_content
    std::string result = content;
    result.pop_back();
    result += R"(,"m.relates_to":)" + relation;
    result += R"(,"m.new_content":)" + newContent;
    result += "}";
    return result;
}

// ==== Matrix Reply Formatting ====

// Build the reply fallback body (plain text).
// Original Kotlin: > Author\n> Message\n\nMy reply
inline std::string buildReplyFallbackBody(
    const std::string& repliedAuthor,
    const std::string& repliedBody,
    const std::string& myReply)
{
    std::string result;
    // Quote each line of the original message
    result += "> <" + repliedAuthor + "> " + repliedBody + "\n\n";
    result += myReply;
    return result;
}

// Build the reply fallback HTML (formatted body).
inline std::string buildReplyFallbackHtml(
    const std::string& repliedAuthor,
    const std::string& repliedAuthorId,
    const std::string& repliedHtml,
    const std::string& myHtml)
{
    return R"(<mx-reply><blockquote><a href="https://matrix.to/#/)" + repliedAuthorId +
           R"(">)" + repliedAuthor + R"(</a><br />)" + repliedHtml +
           R"(</blockquote></mx-reply>)" + myHtml;
}

} // namespace progressive
