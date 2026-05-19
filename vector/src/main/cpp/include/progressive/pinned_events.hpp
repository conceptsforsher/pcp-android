#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// Original Kotlin: PinnedEventInfo
struct PinnedEventInfo {
    std::string eventId;
    std::string pinnedBy;
    int64_t pinnedAt = 0;
    bool isPinned = true;
};

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

// ================================================================
// Extended Pinned Events API
// ================================================================

// Original Kotlin: PinnedEventsContent
struct PinnedEventsContent {
    std::vector<std::string> pinned;
};

// Original Kotlin: PinnedEventChange
enum class PinnedEventChange {
    PINNED,
    UNPINNED
};

// Original Kotlin: PinnedEventsChangeInfo
struct PinnedEventsChangeInfo {
    PinnedEventChange change = PinnedEventChange::PINNED;
    std::string eventId;
    std::string changedBy;
    int64_t changedAt = 0;
};

// Parse full pinned events content from state JSON.
// Original Kotlin: parsePinnedEventsContent()
PinnedEventsContent parsePinnedEventsContent(const std::string& stateContentJson);

// Check if an event is in the pinned list.
// Original Kotlin: isEventPinned()
bool isEventPinned(const std::string& eventId, const std::vector<std::string>& pinnedIds);

// Add an event to the pinned list (returns new list).
// Original Kotlin: pinEvent()
std::vector<std::string> pinEvent(const std::vector<std::string>& currentPins, const std::string& eventId);

// Remove an event from the pinned list (returns new list).
// Original Kotlin: unpinEvent()
std::vector<std::string> unpinEvent(const std::vector<std::string>& currentPins, const std::string& eventId);

// Get pinned event info list from state content JSON.
// Original Kotlin: getPinnedEvents()
std::vector<PinnedEventInfo> getPinnedEvents(const std::string& stateContentJson);

// Format pinned events as a human-readable message.
// Original Kotlin: formatPinnedEventsMessage()
std::string formatPinnedEventsMessage(const std::vector<PinnedEvent>& events);

} // namespace progressive
