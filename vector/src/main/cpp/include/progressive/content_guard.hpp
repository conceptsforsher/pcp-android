#ifndef PROGRESSIVE_CONTENT_GUARD_HPP
#define PROGRESSIVE_CONTENT_GUARD_HPP

#include <string>
#include <vector>

namespace progressive {

// ---- Content Guard: Emoji Attack + Media Collapse Protection ----
// Protects against client slowdown/crash from malicious or accidental
// content flooding.

// ==== Emoji Attack Protection ====
// Detects when a message contains an excessive number of emoji that
// could cause rendering performance issues or crashes.

struct EmojiCheck {
    int totalEmojis = 0;         // total emoji count in the message
    int uniqueEmojis = 0;        // how many different emoji types
    bool isAttack = false;       // exceeds threshold
    int limitExceeded = 0;       // max configured limit
    std::string label;           // "(emoji attack)" or empty
};

// Count emoji characters in a string.
// Detects Unicode emoji ranges: flags, symbols, emoticons, etc.
int countEmojis(const std::string& text);

// Count unique emoji types in a string.
int countUniqueEmojis(const std::string& text);

// Check if a message constitutes an emoji attack.
// @param text  The message body
// @param maxEmojis  Max allowed emoji before flagging (0 = disabled)
// @param maxUniqueEmojis  Max unique emoji types (0 = disabled)
EmojiCheck checkEmojiAttack(const std::string& text, int maxEmojis = 50, int maxUniqueEmojis = 20);

// Get the emoji attack warning label.
inline std::string getEmojiAttackLabel() { return "(emoji attack)"; }

// ==== Media Collage / Grouping ====
// Collapses consecutive images into a clickable "N media omitted" label
// when the count exceeds the configured threshold.

struct MediaGroup {
    int imageCount = 0;          // total consecutive images
    int videoCount = 0;          // total consecutive videos
    int fileCount = 0;           // total consecutive files
    int totalMedia = 0;          // total media items
    bool shouldCollapse = false; // exceeds threshold
    int threshold = 0;           // configured threshold
    std::string label;           // "5 media omitted" or empty
};

// Check if consecutive media items should be collapsed.
// @param mediaTypes  Ordered list of media types ("image", "video", "file")
// @param threshold  Collapse if >= this many consecutive items (0 = never)
MediaGroup checkMediaCollapse(
    const std::vector<std::string>& mediaTypes,
    int threshold = 10
);

// Group consecutive media types and return collapse instructions.
// Returns a list where consecutive media of the same type beyond threshold
// are replaced with a collapse marker.
struct ContentMediaItem {
    std::string mediaType;       // "image", "video", "file", "text"
    bool isCollapseMarker = false; // this is a placeholder
    int collapseCount = 0;       // how many items are collapsed
};

std::vector<ContentMediaItem> groupMedia(
    const std::vector<std::string>& mediaTypes,
    int threshold = 10
);

// Format the media collapse label.
std::string formatMediaCollapseLabel(int count);

// Check if a Unicode codepoint is an emoji.
bool isEmojiCodePoint(int codepoint);

} // namespace progressive

#endif // PROGRESSIVE_CONTENT_GUARD_HPP
