#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>

namespace progressive {

// ================================================================
// Pin Manager — manage pinned messages in Matrix rooms
//
// Ported from Element Android:
//   PinnedMessagesViewModel.kt, PinnedMessage.kt
//   PinUnpinAction.kt
//
// Uses m.room.pinned_events state event:
//   {"pinned": ["$eventId1", "$eventId2", ...]}
//   Max 100 pinned events per room.
//   Requires PL50 (moderator) to pin/unpin.
// ================================================================

// ---- Pinned Event Info ----

struct PinnedEventInfo {
    std::string eventId;             // Matrix event ID
    std::string roomId;
    std::string senderId;            // Original sender
    std::string senderName;          // Display name
    std::string body;                // Message body (truncated for preview)
    std::string eventType;           // "m.room.message", etc.
    std::string pinnedBy;            // Who pinned it
    int64_t pinnedAtMs = 0;          // When pinned
    int64_t originalTimestampMs = 0; // When the message was sent
    int displayOrder = 0;            // Position in pinned list
    bool isEncrypted = false;
    bool hasImage = false;           // Has image attachment
    bool hasFile = false;            // Has file attachment
    bool hasVideo = false;
    std::string previewText;         // Formatted preview text
};

// ---- Pinned Events List ----

struct PinnedEventsList {
    std::vector<PinnedEventInfo> events;
    std::string roomId;
    int totalCount = 0;
    int64_t lastUpdatedMs = 0;
    std::string rawStateContent;     // Original state JSON
};

// ---- Pin Manager ----

class PinManagerFull {
public:
    PinManagerFull();

    // ====== Pin/Unpin Lifecycle ======

    // Pin an event. Returns the new m.room.pinned_events content JSON.
    // Returns error if: already pinned, max limit reached (100), invalid PL.
    std::string pinEvent(const std::string& roomId, const std::string& eventId,
                          const std::string& pinnedBy, int userPowerLevel,
                          std::string& error);

    // Unpin an event. Returns the new state content JSON.
    std::string unpinEvent(const std::string& roomId, const std::string& eventId,
                            const std::string& removedBy, int userPowerLevel,
                            std::string& error);

    // Toggle pin (pin if not pinned, unpin if pinned).
    std::string togglePin(const std::string& roomId, const std::string& eventId,
                           const std::string& userId, int userPowerLevel,
                           std::string& error);

    // ====== State Loading ======

    // Load pinned events from room state JSON.
    // stateContentJson = content of m.room.pinned_events state event.
    void loadState(const std::string& roomId, const std::string& stateContentJson);

    // Set event metadata for a pinned event (called when event content is loaded).
    void setEventMetadata(const std::string& roomId, const std::string& eventId,
                           const std::string& senderId, const std::string& senderName,
                           const std::string& body, const std::string& eventType,
                           int64_t originalTimestampMs, bool isEncrypted);

    // ====== Queries ======

    // Get all pinned events for a room.
    std::vector<PinnedEventInfo> getPinnedEvents(const std::string& roomId) const;

    // Check if an event is already pinned.
    bool isEventPinned(const std::string& roomId, const std::string& eventId) const;

    // Get the number of pinned events in a room.
    int getPinnedCount(const std::string& roomId) const;

    // Check if user can pin/unpin.
    bool canManagePins(int userPowerLevel) const;

    // Get the maximum allowed pinned events.
    int maxPinnedEvents() const { return 100; }

    // ====== Formatting ======

    // Format pinned events as a list for display.
    std::string formatPinnedList(const std::string& roomId) const;

    // Format a single pinned event preview.
    std::string formatPinnedPreview(const PinnedEventInfo& event) const;

    // ====== Serialization ======

    // Export pinned events as JSON for UI.
    std::string pinnedEventsToJson(const std::string& roomId) const;

    // Build the m.room.pinned_events state content JSON.
    static std::string buildPinnedEventsContent(const std::vector<std::string>& eventIds);

    // Parse event IDs from m.room.pinned_events content.
    static std::vector<std::string> parsePinnedEventIds(const std::string& stateContentJson);

    // ====== Stats ======

    int totalRooms() const { return static_cast<int>(pinned_.size()); }
    int totalPinnedEvents() const;

private:
    // roomId → list of pinned event IDs (in order)
    std::unordered_map<std::string, std::vector<std::string>> pinned_;
    // roomId → (eventId → PinnedEventInfo)
    std::unordered_map<std::string, std::unordered_map<std::string, PinnedEventInfo>> metadata_;

    static const int MAX_PINNED = 100;
    static const int REQUIRED_POWER_LEVEL = 50;

    // Get current pinned event IDs for a room.
    std::vector<std::string>& getPinnedIds(const std::string& roomId);

    // Build preview text from body.
    std::string buildPreview(const std::string& body, const std::string& senderName,
                              const std::string& eventType, bool hasImage, bool hasFile, bool hasVideo) const;
};

} // namespace progressive

