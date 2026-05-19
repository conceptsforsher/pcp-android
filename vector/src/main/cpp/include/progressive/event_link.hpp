#ifndef PROGRESSIVE_EVENT_LINK_HPP
#define PROGRESSIVE_EVENT_LINK_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>

namespace progressive {

// ---- Event Link Resolution ----

struct EventLink {
    size_t startPos;           // position in the original message text
    size_t endPos;             // end position
    std::string eventId;       // extracted event ID
    std::string originalText;  // the link text (e.g. "$abc123" or full URL)
    std::string resolvedBody;  // the resolved message body
    std::string resolvedSender;// who sent the resolved message
    bool resolved = false;
};

struct ResolveState {
    bool isExpanded = false;      // links expanded or collapsed
    int maxConcurrent = 5;        // max simultaneous fetches
    int pendingCount = 0;
    int resolvedCount = 0;
    int totalLinks = 0;
};

// Extract event links from message body.
// Matches: $eventId, $eventId:server, https://matrix.to/#/!room/$event
std::vector<EventLink> extractEventLinks(const std::string& body);

// Check if a string looks like an event ID: $xxx or $xxx:server
// bool isEventId(const std::string& text);  // now in matrix_patterns.hpp

// Parse a Matrix.to URL to extract eventId and roomId.
// e.g. "https://matrix.to/#/!room:server/$event:server" → {eventId, roomId}
struct MatrixToLink { std::string eventId; std::string roomId; };
MatrixToLink parseMatrixToUrl(const std::string& url);

// Build the resolved display text: "> senderName: message body"
std::string formatResolvedEventText(const std::string& senderName, const std::string& body);

// Check if text has been expanded (has resolved markers).
bool isExpandedText(const std::string& text);

// ---- Full Timestamps with Seconds ----

// Format epoch ms to "HH:MM:SS" or "HH:MM" based on config.
std::string formatTimestamp(int64_t epochMs, bool includeSeconds);

// Format epoch ms to full date+time with seconds: "May 13, 2026 14:30:05"
std::string formatFullTimestamp(int64_t epochMs);

} // namespace progressive

#endif // PROGRESSIVE_EVENT_LINK_HPP
