#ifndef PROGRESSIVE_CONTENT_FILTER_HPP
#define PROGRESSIVE_CONTENT_FILTER_HPP

#include <string>
#include <vector>
#include <unordered_set>

namespace progressive {

// ---- Forbidden Keywords Filter ----

class KeywordFilter {
public:
    // Load keywords from a comma- or newline-separated string.
    void loadKeywords(const std::string& raw);

    // Check if text contains any forbidden keyword (case-insensitive).
    // Returns the first matched keyword, or empty string if clean.
    std::string check(const std::string& text) const;

    // Add a single keyword.
    void addKeyword(const std::string& keyword);

    // Remove a single keyword.
    void removeKeyword(const std::string& keyword);

    // Export all keywords as a comma-separated string.
    std::string exportKeywords() const;

    // Clear all keywords.
    void clear();

    size_t count() const { return keywords_.size(); }

private:
    std::vector<std::string> keywords_;
    static std::string toLower(const std::string& s);
};

// ---- Image Security Policy ----

struct ImagePolicy {
    bool blockAllRemote = false;   // block ALL images from internet
    bool allowAvatars = true;      // exception: allow avatars even if blockAllRemote
    bool allowStickers = false;    // exception: allow stickers
    bool allowEmoji = true;        // exception: allow emoji

    // Check if an image URL should be blocked under this policy.
    // Returns true if the image should be blocked.
    bool shouldBlock(const std::string& mxcUrl, const std::string& imageType) const;
    // imageType: "avatar", "sticker", "emoji", "attachment", "preview"
};

// ---- Media Sending Policy ----

struct MediaSendPolicy {
    bool sendOriginalSize = true;      // don't compress
    bool skipPreviewGeneration = false; // don't generate thumbnail for RAM saving
};

} // namespace progressive

#endif // PROGRESSIVE_CONTENT_FILTER_HPP
