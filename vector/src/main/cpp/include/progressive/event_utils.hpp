#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>

namespace progressive {

// ==== Event Summary Generator ====
//
// Builds human-readable one-line summaries of Matrix events.
// Used for room list previews, notifications, and timeline summaries.
// Original Kotlin: Event.getDecryptedTextSummary(), TimelineEvent extensions

// Generate a human-readable summary of an event based on its type and content.
//
// Returns a short string suitable for room list previews:
//   "Alice: Hello!"           (text)
//   "Alice sent an image"     (media)
//   "Alice created a poll"    (poll)
//   "Alice is live"           (location/beacon)
//   "Alice joined the room"   (membership)

std::string formatEventSummary(
    const std::string& eventType,      // "m.room.message", "m.room.member", etc.
    const std::string& msgType,        // "m.text", "m.image", etc. (empty for non-message)
    const std::string& senderName,
    const std::string& body,           // Message body (may be empty)
    const std::string& membership = "", // "join", "invite", "leave", "ban" (for member events)
    const std::string& displayName = "",// New display name (for member events with displayname changes)
    bool isRedacted = false,
    bool isEncrypted = false
);

// ==== Read Marker Position Calculator ====
//
// Calculates the optimal position for the read marker in a timeline.
// The read marker separates "read" and "unread" messages.
// Original Kotlin: ReadMarkersTracker logic

struct ReadMarkerPosition {
    int eventIndex = -1;               // Index in the timeline where marker should go
    int unreadCount = 0;               // Number of unread events after the marker
    int highlightCount = 0;            // Number of highlighted unreads
    bool allRead = true;               // True if everything has been read
};

// Calculate read marker position from a list of event read states.
// events: ordered list of events in the timeline (oldest first)
// readEventId: the last event the user has read (null = nothing read)
// Returns the position where the "new messages" divider should appear.
ReadMarkerPosition calculateReadMarker(
    const std::vector<std::string>& eventIds,     // Ordered event IDs
    const std::string& readEventId,               // User's read receipt event ID (empty = none)
    const std::vector<std::string>& highlightIds  // Events that should be highlighted
);

// ==== Typing Notification Formatter ====
//
// Builds the "Alice is typing..." text for the typing indicator.
// Original Kotlin: TypingHelper, TypingUsersTracker

// Format the typing indicator text for a list of typing users.
// 1 user:   "Alice is typing..."
// 2 users:  "Alice and Bob are typing..."
// 3+ users: "Alice, Bob and Charlie are typing..."
// 4+ users: "Alice, Bob and 2 others are typing..."
std::string formatTypingIndicator(
    const std::vector<std::string>& typingUserNames,
    int maxNamesShown = 3
);

// ==== User Power Level Utilities ====
//
// Determine user capabilities based on power levels.

struct PowerLevelCapabilities {
    bool canSendMessages = true;         // events_default + specific event types
    bool canSendState = false;           // state_default
    bool canInvite = false;              // invite level
    bool canKick = false;                // kick level
    bool canBan = false;                 // ban level
    bool canRedact = false;              // redact level
    bool canRedactOthers = false;        // moderator (50+) can redact others
    bool canChangeRoomName = false;      // state_default for m.room.name
    bool canChangeRoomTopic = false;     // state_default for m.room.topic
    bool canChangeRoomAvatar = false;    // state_default for m.room.avatar
    bool canChangeJoinRules = false;     // state_default for m.room.join_rules
    bool canChangeHistoryVisibility = false;
    bool canChangeGuestAccess = false;
    bool canNotifyEveryone = false;      // notifications.room level
    bool isOwner = false;                // 100+ = admin
    bool isModerator = false;            // 50+ = moderator
};

// Calculate user capabilities from power levels.
PowerLevelCapabilities calculateCapabilities(
    int userPowerLevel,
    int eventsDefault = 0,
    int stateDefault = 50,
    int inviteLevel = 0,
    int kickLevel = 50,
    int banLevel = 50,
    int redactLevel = 50,
    int notifyRoomLevel = 50,
    const std::unordered_map<std::string, int>& eventTypeLevels = {}
);

// ==== Member Event Notice Formatter ====
//
// Formats membership change events for the timeline:
//   "Alice joined the room"
//   "Bob was invited by Charlie"
//   "You were kicked by Alice: inappropriate behavior"
//   "Alice changed their display name to Al"
// etc. Handles all Matrix membership transitions + profile changes.

std::string formatMemberNotice(
    const std::string& membership,         // new membership: "join", "invite", "leave", "ban", "knock"
    const std::string& prevMembership,     // previous membership (empty when first join)
    const std::string& senderId,           // who performed the action
    const std::string& senderName,         // display name of sender
    const std::string& targetUserId,       // who this happened to (state_key)
    const std::string& targetDisplayName,  // display name of target
    const std::string& reason,             // optional reason for kick/ban/invite
    bool isDirectMessage,                  // true for DM rooms
    bool sentByCurrentUser                 // true if the logged-in user performed the action
);

// ==== Call Event Notice Formatter ====

std::string formatCallNotice(
    const std::string& eventType,          // "m.call.invite", "m.call.answer", "m.call.hangup", "m.call.reject"
    bool isVideo,                          // true for video, false for voice
    const std::string& senderName,
    bool sentByCurrentUser
);

// ==== Redaction Notice ====

// Format a redaction notice: "Message removed" with optional reason.
std::string formatRedactionNotice(const std::string& reason, bool redactedBySameUser, bool isStateEvent);

// ==== Edit Annotation ====

std::string annotateEdited(const std::string& body, bool isEdited);

// ==== Room State Notice Formatters ====

// Room name change: "Alice changed the room name to General"
std::string formatRoomNameNotice(const std::string& senderName, const std::string& newName, bool sentByCurrentUser);

// Room topic change: "Alice changed the topic to Welcome!"
std::string formatRoomTopicNotice(const std::string& senderName, const std::string& newTopic, bool sentByCurrentUser);

// Room avatar change: "Alice changed the room avatar" / "Alice removed the room avatar"
std::string formatRoomAvatarNotice(const std::string& senderName, bool isRemoved, bool sentByCurrentUser);

// Room create: "You created the room" / "Alice created the room"
std::string formatRoomCreateNotice(const std::string& senderName, const std::string& predecessorRoomId, bool isDirect, bool sentByCurrentUser);

// Room tombstone: "This room has been replaced" / "Room upgraded to new_room"
std::string formatRoomTombstoneNotice(const std::string& senderName, const std::string& replacementRoom, bool sentByCurrentUser);

// Room encryption: "Encryption has been enabled by Alice"
std::string formatRoomEncryptionNotice(const std::string& senderName, bool isEnabled, bool sentByCurrentUser);

// Power level change: "Alice changed power levels"
std::string formatPowerLevelNotice(const std::string& senderName, bool sentByCurrentUser);

// Power level diff: "Alice changed power levels: Bob from User to Moderator"
// oldLevels / newLevels: map of userId → power level (only include changed users)
std::string formatPowerLevelDiff(const std::string& senderName,
    const std::unordered_map<std::string, int>& oldLevels,
    const std::unordered_map<std::string, int>& newLevels,
    const std::unordered_map<std::string, std::string>& userNames, // userId → displayName
    bool sentByCurrentUser);

// Join rules change: "Alice changed join rules to invite-only"
std::string formatJoinRulesNotice(const std::string& senderName, const std::string& newRule, bool sentByCurrentUser);

// History visibility change: "Alice changed history visibility"
std::string formatHistoryVisibilityNotice(const std::string& senderName, const std::string& newVisibility, bool sentByCurrentUser);

// Guest access change: "Alice changed guest access"
std::string formatGuestAccessNotice(const std::string& senderName, bool guestsAllowed, bool sentByCurrentUser);

} // namespace progressive
