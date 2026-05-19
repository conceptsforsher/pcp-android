#ifndef PROGRESSIVE_MENTION_PARSER_HPP
#define PROGRESSIVE_MENTION_PARSER_HPP

#include <string>
#include <vector>

namespace progressive {

struct Mention {
    std::string userId;        // @user:server
    std::string displayName;   // "Alice"
    size_t startPos = 0;       // position in original text
    size_t endPos = 0;         // end position
    bool isRoom = false;       // @room mention
};

struct ParsedMentions {
    std::string processedText;     // text with mentions resolved
    std::vector<Mention> mentions; // extracted mentions
    bool hasEveryone = false;      // @room, @here, @channel
};

// Parse mentions from message body.
// Matrix format: <a href="https://matrix.to/#/@user:server">Display Name</a>
// Also handles plain text @user:server mentions.
ParsedMentions parseMentions(const std::string& body, const std::string& formattedBody = "");

// Extract @-mentions from plain text.
std::vector<Mention> extractAtMentions(const std::string& text);

// Check if text contains a mention of a specific user.
bool containsMention(const std::string& text, const std::string& userId);

// Build a Matrix pill link for a user mention.
std::string buildUserPill(const std::string& userId, const std::string& displayName);

// Build a Matrix pill link for a room mention.
std::string buildRoomPill(const std::string& roomId);

// Build a Matrix pill link for a message link.
std::string buildEventPill(const std::string& eventId, const std::string& roomId);

// Check if a string is a valid Matrix pill (HTML <a> with matrix.to href).
bool isMatrixPill(const std::string& html);

// Strip pills from text (for plain text notifications).
std::string stripPills(const std::string& html);

// Resolve a user mention to display name.
std::string resolveMentionDisplayName(const std::string& userId,
    const std::vector<std::pair<std::string, std::string>>& knownUsers);

} // namespace progressive

#endif // PROGRESSIVE_MENTION_PARSER_HPP
