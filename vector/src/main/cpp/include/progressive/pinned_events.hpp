#ifndef PROGRESSIVE_PINNED_EVENTS_HPP
#define PROGRESSIVE_PINNED_EVENTS_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

struct PinnedEvent {
    std::string eventId;
    std::string pinnedBy;       // who pinned it
    int64_t pinnedAtMs = 0;     // when pinned
    std::string body;           // message preview (from cache)
    std::string senderName;     // original message sender
};

// Parse Matrix m.room.pinned_events state content.
// Content format: {"pinned": ["$ev1", "$ev2", ...]}
std::vector<std::string> parsePinnedEventIds(const std::string& stateContentJson);

// Format pinned events list for display.
std::string formatPinnedEventsText(const std::vector<PinnedEvent>& events);

// Format as JSON for UI panel.
std::string pinnedEventsToJson(const std::vector<PinnedEvent>& events, const std::string& roomId);

// Build the m.room.pinned_events content JSON for sending to server.
// Takes the new list of event IDs to pin.
std::string buildPinnedEventsContent(const std::vector<std::string>& eventIds);

// Check if pinning is supported (requires appropriate power level).
// PL50 = moderator, PL100 = admin. Pin/unpin typically requires PL50.
bool canManagePins(int userPowerLevel, int requiredLevel = 50);

} // namespace progressive

#endif // PROGRESSIVE_PINNED_EVENTS_HPP
