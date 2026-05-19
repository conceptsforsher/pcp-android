#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>

namespace progressive {

// ================================================================
// Room History Visibility Manager
//
// Faithful port from Element Android original sources:
//   RSM_RoomHistoryVisibility.kt — WORLD_READABLE, SHARED, INVITED, JOINED
//     shouldShareHistory() → WORLD_READABLE | SHARED
//   RoomJoinRules.kt — PUBLIC, INVITE, KNOCK, etc.
//
// Matrix spec: m.room.history_visibility state event
// Controls who can see what in the room based on membership.
//
// Covers:
//   1. History visibility types (world_readable, shared, invited, joined)
//   2. Who can see events based on membership state
//   3. Share history check (for MSC3061 key sharing)
//   4. Join rule integration
//   5. Build state event content
// ================================================================

// ---- Room History Visibility ----
// Original: RSM_RoomHistoryVisibility.kt (WORLD_READABLE, SHARED, INVITED, JOINED)

enum class RSM_RoomHistoryVisibility {
    WORLD_READABLE = 0,  // Anyone can see all events
    SHARED = 1,          // Joined members see all events
    INVITED = 2,         // Members see from invite point
    JOINED = 3,          // Members see from join point
};


// ---- Room Join Rules ----

enum class RoomJoinRule {
    PUBLIC = 0,    // Anyone can join
    INVITE = 1,    // Only invited users
    KNOCK = 2,     // Can knock to request join
    PRIVATE = 3,   // Not joinable
};


// ---- Membership State (for visibility checks) ----

enum class MembershipState {
    JOIN = 0,       // Currently in the room
    INVITE = 1,     // Invited but not joined
    KNOCK = 2,      // Requested to join
    LEAVE = 3,      // Left the room
    BAN = 4,        // Banned
    NONE = 5,       // Never been in the room
};

// ---- Room State Summary ----

struct RoomStateSummary {
    std::string roomId;
    std::string roomName;
    RSM_RoomHistoryVisibility historyVisibility = RSM_RoomHistoryVisibility::SHARED;
    RoomJoinRule joinRule = RoomJoinRule::INVITE;
    bool isEncrypted = false;
    bool isPublicRoom = false;          // joinRule == PUBLIC
    bool isWorldReadable = false;       // historyVisibility == WORLD_READABLE
    bool canShareHistory = false;       // shouldShareHistory()
    int memberCount = 0;
    std::string creatorId;
    std::string roomVersion;
};

// ---- History Visibility Check ----
// Original: shouldShareHistory() = WORLD_READABLE || SHARED

// Check if history can be shared with new members.
// Per MSC3061: only WORLD_READABLE and SHARED allow sharing.
bool shouldShareHistory(RSM_RoomHistoryVisibility visibility);

// Check if a member can see an event based on history_visibility.
// Takes: history visibility, member's state when event was sent, member's current state.
bool canSeeEvent(RSM_RoomHistoryVisibility visibility, MembershipState memberStateAtEventTime,
                  MembershipState memberCurrentState);

// Check if a non-member can see events.
bool canNonMemberSeeEvents(RSM_RoomHistoryVisibility visibility);

// Get the effective visibility for a given membership.
std::string getVisibilityLabel(RSM_RoomHistoryVisibility visibility);

// Get a human-readable description of what the visibility means.
std::string getVisibilityDescription(RSM_RoomHistoryVisibility visibility);

// ---- Room State Builder ----
// Original: Build m.room.history_visibility state event content

// Build history_visibility state event content.
std::string buildHistoryVisibilityContent(RSM_RoomHistoryVisibility visibility);

// Build join_rules state event content.
std::string buildJoinRulesContent(RoomJoinRule rule);

// Parse history visibility from state event content.

// Parse join rules from state event content.

// ---- Room State Manager ----

class RoomStateManager {
public:
    RoomStateManager();

    // ====== Room State ======

    // Set room state from events.
    void setHistoryVisibility(const std::string& roomId, RSM_RoomHistoryVisibility visibility);
    void setJoinRule(const std::string& roomId, RoomJoinRule rule);
    void setRoomName(const std::string& roomId, const std::string& name);
    void setEncrypted(const std::string& roomId, bool encrypted);
    void setMemberCount(const std::string& roomId, int count);

    // Get room state summary.
    RoomStateSummary getRoomState(const std::string& roomId) const;

    // ====== Visibility Checks ======

    // Check if room history can be shared (for MSC3061 key sharing).
    bool canShareRoomHistory(const std::string& roomId) const;

    // Check if a room is publicly viewable.
    bool isPublicRoom(const std::string& roomId) const;

    // Check if a room is world-readable.
    bool isWorldReadable(const std::string& roomId) const;

    // Check if a room is invite-only.
    bool isInviteOnly(const std::string& roomId) const;

    // Check if guests are allowed.
    bool areGuestsAllowed(const std::string& roomId) const;

    // ====== Serialization ======

    // Export room state as JSON.
    std::string roomStateToJson(const RoomStateSummary& state) const;

    // Clear room state.
    void clear();

private:
    std::unordered_map<std::string, RoomStateSummary> rooms_; // roomId → state
};

} // namespace progressive
