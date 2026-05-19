#ifndef PROGRESSIVE_NOTIFICATION_HPP
#define PROGRESSIVE_NOTIFICATION_HPP

#include <string>
#include <vector>

namespace progressive {

// ---- Notification Keywords ----

struct NotificationKeyword {
    std::string keyword;        // the word/phrase that triggers notification
    bool enabled = true;
    bool caseSensitive = false;
};

class NotificationKeywords {
public:
    void addKeyword(const std::string& keyword, bool caseSensitive = false);
    void removeKeyword(const std::string& keyword);
    void setEnabled(const std::string& keyword, bool enabled);

    // Check if message body triggers any notification keyword.
    // Returns the matching keyword or empty string.
    std::string check(const std::string& body) const;

    // Get all keywords as JSON.
    std::string exportJson() const;

    // Load from JSON.
    void importJson(const std::string& json);

    void clear();
    size_t count() const { return keywords_.size(); }

private:
    std::vector<NotificationKeyword> keywords_;
    static std::string toLower(const std::string& s);
};

// ---- Reaction Preview Formatting ----

struct ReactionPreview {
    std::string reactorName;      // who reacted
    std::string reactionEmoji;    // 🚀
    std::string sourceBody;       // beginning of the source message
    std::string sourceSenderName; // who wrote the source message
};

// Format a reaction preview: "User reacted: 🚀 to 'beginning of message...'"
std::string formatReactionPreview(const ReactionPreview& reaction);

// Truncate a message body for reaction preview (first N chars)
std::string truncateForReactionPreview(const std::string& body, int maxLen = 40);

} // namespace progressive

#endif // PROGRESSIVE_NOTIFICATION_HPP
