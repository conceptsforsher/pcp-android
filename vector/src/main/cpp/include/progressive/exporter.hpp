#ifndef PROGRESSIVE_EXPORTER_HPP
#define PROGRESSIVE_EXPORTER_HPP

#include <string>
#include <vector>
#include <functional>
#include <cstdint>

namespace progressive {

enum class ExportFormat { Html, PlainText, Json };
enum class ExportRange { Timeline, Beginning, LastNMessages };

struct ExportOptions {
    ExportFormat format = ExportFormat::Html;
    ExportRange range = ExportRange::Timeline;
    int64_t numberOfMessages = 100;
    bool includeAttachments = false;
    int64_t maxSizeBytes = 100 * 1024 * 1024; // 100 MB
};

struct ExportEvent {
    std::string eventId;
    std::string senderId;
    std::string senderName;
    std::string timestamp;     // ISO 8601
    std::string body;          // plain text body
    std::string formattedBody; // HTML body if available
    std::string eventType;     // "m.room.message", "m.reaction", etc.
    std::string msgType;       // "m.text", "m.image", etc.
    std::string relationType;  // "m.annotation", "m.reference", "" if none
    std::string sourceEventId; // if this is a reaction/reply
    std::string mediaUrl;      // MXC URL if attachment
    std::string mediaMimeType;
    std::string fileName;
    int64_t mediaSize = 0;
    bool isEncrypted = false;
};

struct ExportProgress {
    int totalEvents = 0;
    int processedEvents = 0;
    int64_t bytesWritten = 0;
    bool done = false;
    std::string error;
};

// Format one event as HTML
std::string formatEventHtml(const ExportEvent& event, bool isContinuation);

// Format one event as plain text
std::string formatEventPlainText(const ExportEvent& event);

// Format one event as JSON line
std::string formatEventJson(const ExportEvent& event);

// Build a complete HTML document from events
std::string buildHtmlDocument(
    const std::string& roomName,
    const std::string& roomTopic,
    const std::string& exportDate,
    const std::vector<ExportEvent>& events
);

// Build a complete plain text document
std::string buildPlainTextDocument(
    const std::string& roomName,
    const std::string& exportDate,
    const std::vector<ExportEvent>& events
);

// Build a complete JSON document
std::string buildJsonDocument(
    const std::string& roomName,
    const std::string& roomTopic,
    const std::string& exportDate,
    const std::string& roomCreator,
    const std::vector<ExportEvent>& events
);

// Escape HTML entities
std::string escapeHtml(const std::string& input);

// Escape JSON string
std::string escapeJson(const std::string& input);

} // namespace progressive

#endif // PROGRESSIVE_EXPORTER_HPP
