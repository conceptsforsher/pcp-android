#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// ================================================================
// Spoiler Manager — content spoiler wrapping & detection
//
// Matrix spec spoiler format:
//   Formatted body: <span data-mx-spoiler>hidden content</span>
//   Plain text: unchanged (the content itself is still in body)
//   Receiving clients render click-to-reveal overlay
//
// Usage with images:
//   User checks "send as spoiler" → image content gets spoiler wrapper
//   Recipient sees: [SPOILER: photo.jpg] (click to reveal)
//
// Covers:
//   1. Build spoiler-wrapped content (plain + formatted)
//   2. Detect spoilers in received content
//   3. Strip spoilers for notifications
//   4. Labs flag for spoiler checkbox visibility
// ================================================================

// ---- Spoiler Config (Labs settings) ----

struct SpoilerConfig {
    bool showSpoilerCheckbox = true;       // Show checkbox when sending images
    bool autoSpoilerNsfw = false;          // Auto-spoiler images marked NSFW
    bool revealByDefault = false;          // Auto-reveal spoilers (override)
    std::string spoilerLabel = "SPOILER";  // Label shown on blurred content
    std::string spoilerReason = "";        // Why it's a spoiler (empty = none)
};

// ---- Spoiler Content ----//

struct SpoilerContent {
    std::string plainBody;           // Original plain text body
    std::string formattedBody;       // Formatted body with spoiler wrapper
    std::string spoilerType;         // "image", "video", "text", "file"
    bool hasSpoiler = false;
    std::string reason;              // Optional spoiler reason ("NSFW", "Episode 5")
};

// ---- Spoiler Manager ----

class SpoilerManager {
public:
    SpoilerManager();

    // ====== Config ======

    void setConfig(const SpoilerConfig& config);
    SpoilerConfig getConfig() const;

    // ====== Build Spoiler Content ======
    // Wrap content in spoiler HTML tags.

    // Wrap an image in spoiler.
    // plainBody: "photo.jpg" or "image"
    // mxcUrl: "mxc://server/mediaId"
    // Returns (plainBody, formattedBody) as strings.
    SpoilerContent buildImageSpoiler(const std::string& plainBody,
                                      const std::string& mxcUrl,
                                      const std::string& mimeType,
                                      int width, int height,
                                      int64_t sizeBytes,
                                      const std::string& reason = "");

    // Wrap text in spoiler (e.g. "||spoiler text||" format).
    SpoilerContent buildTextSpoiler(const std::string& plainBody,
                                     const std::string& reason = "");

    // Wrap video in spoiler.
    SpoilerContent buildVideoSpoiler(const std::string& plainBody,
                                      const std::string& mxcUrl,
                                      const std::string& mimeType,
                                      int durationMs,
                                      const std::string& reason = "");

    // Wrap file in spoiler.
    SpoilerContent buildFileSpoiler(const std::string& plainBody,
                                     const std::string& mxcUrl,
                                     const std::string& mimeType,
                                     int64_t sizeBytes,
                                     const std::string& reason = "");

    // ====== Detection ======

    // Check if a received message contains a spoiler.
    bool hasSpoiler(const std::string& formattedBody) const;

    // Extract spoiler type from formatted body.
    std::string detectSpoilerType(const std::string& formattedBody) const;

    // Extract spoiler reason from formatted body.
    std::string extractSpoilerReason(const std::string& formattedBody) const;

    // ====== Display Formatting ======

    // Format spoiler notification text (for push notifications).
    // "Alice sent a spoiler: image"
    std::string formatSpoilerNotification(const std::string& senderName,
                                           const std::string& spoilerType) const;

    // Format spoiler preview for room list.
    std::string formatSpoilerPreview(const std::string& spoilerType) const;

    // ====== Build Content (for sending) ======

    // Build the full m.room.message content with spoiler wrapper.
    // Returns complete JSON for the send event.
    std::string buildSpoilerMessageContent(const SpoilerContent& spoiler,
                                            const std::string& mxcUrl,
                                            const std::string& msgType) const;

    // ====== Serialization ======

    std::string spoilerToJson(const SpoilerContent& spoiler) const;
    std::string configToJson() const;

private:
    SpoilerConfig config_;

    std::string buildSpoilerSpan(const std::string& innerContent, const std::string& reason) const;
    std::string buildImageTag(const std::string& mxcUrl, const std::string& mimeType,
                               int w, int h, int64_t size, const std::string& plainBody) const;
    std::string buildVideoTag(const std::string& mxcUrl, const std::string& mimeType,
                               int durationMs, const std::string& plainBody) const;
    std::string buildFileTag(const std::string& mxcUrl, const std::string& mimeType,
                              int64_t size, const std::string& plainBody) const;
};

} // namespace progressive
