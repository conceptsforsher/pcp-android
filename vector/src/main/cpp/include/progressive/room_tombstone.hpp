#pragma once

#include <string>
#include <vector>
#include <cstdint>

#include "progressive/room_filter.hpp"  // for RoomUpgradeInfo (shared struct)

namespace progressive {

// ==== Room Tombstone Content ====
// Represents an m.room.tombstone state event.
//
// When a room is upgraded, the server sends a tombstone event pointing
// to the replacement room. Clients should navigate to that room.
//
// Ref: https://spec.matrix.org/latest/client-server-api/#room-upgrades
//
// Original Kotlin (RoomTombstoneContent.kt:24-35):
//   @JsonClass(generateAdapter = true)
//   data class RoomTombstoneContent(
//       @Json(name = "body") val body: String? = null,
//       @Json(name = "replacement_room") val replacementRoomId: String?
//   )

struct RoomTombstoneContent {
    std::string body;                // Required. Server-defined message.
    std::string replacementRoomId;   // Required. New room the client should visit.

    // Original Kotlin: replacementRoomId == null → not an upgrade
    bool isUpgrade() const {
        return !replacementRoomId.empty();
    }
};

// ==== Versioning State ====
// Describes the upgrade/versioning status of a room.
//
// Original Kotlin (VersioningState.kt:22-39):
//   enum class VersioningState {
//       NONE,
//       UPGRADED_ROOM_NOT_JOINED,
//       UPGRADED_ROOM_JOINED;
//       fun isUpgraded() = this != NONE
//   }

enum class VersioningState {
    NONE = 0,                     // Room is not versioned
    UPGRADING = 1,                // Upgrade in progress (tombstone sent, new room may not exist yet)
    UPGRADED_ROOM_JOINED = 2,     // Upgraded and the user has joined the new room
    UPGRADED_ROOM_NOT_JOINED = 3, // Upgraded but the user has not joined the new room
    PRE_UPGRADE_ROOM = 4          // This room is the predecessor (the one that was upgraded away from)
};

// Original Kotlin (VersioningState.kt:38):
//   fun isUpgraded() = this != NONE
inline bool isVersioningStateUpgraded(VersioningState s) {
    return s != VersioningState::NONE;
}

// --- Serialization ---

// Original Kotlin: enum name
const char* versioningStateToString(VersioningState state);

// Original Kotlin: enum valueOf / custom parsing
VersioningState versioningStateFromString(const std::string& s);

// ==== Compute versioning state ====
// Determine versioning state from tombstone content + membership.
//
// Original Kotlin (RoomTombstoneEventProcessor.kt:33-44):
//   if (event.roomId == null) return
//   val createRoomContent = event.getClearContent().toModel<RoomTombstoneContent>()
//   if (createRoomContent?.replacementRoomId == null) return
//   val predecessorRoomSummary = RoomSummaryEntity.where(realm, event.roomId).findFirst()
//       ?: RoomSummaryEntity(event.roomId)
//   if (predecessorRoomSummary.versioningState == VersioningState.NONE) {
//       predecessorRoomSummary.versioningState = VersioningState.UPGRADED_ROOM_NOT_JOINED
//   }
//
// @param tombstoneJson      JSON of the m.room.tombstone state event (or its content)
// @param currentMembership  current membership string ("join", "leave", etc.) or empty
// @param currentVersioningState  existing versioning state
VersioningState computeVersioningState(
    const std::string& tombstoneJson,
    const std::string& currentMembership,
    VersioningState currentVersioningState);

// ==== Room Predecessor ====
// A link to an old room in case of room versioning.
// Appears inside m.room.create content.
//
// JSON: {"room_id": "!old:example.org", "event_id": "$something:example.org"}
//
// Original Kotlin (Predecessor.kt:24-28):
//   @JsonClass(generateAdapter = true)
//   data class Predecessor(
//       @Json(name = "room_id") val roomId: String? = null,
//       @Json(name = "event_id") val eventId: String? = null
//   )

struct RoomPredecessor {
    std::string roomId;   // Room ID of the old room
    std::string eventId;  // Event ID of the last known event in the old room
};

// Build JSON for a predecessor object.
// Output: {"room_id":"!old:example.org","event_id":"$ev123"}
std::string buildPredecessor(const RoomPredecessor& p);

// Parse a predecessor from JSON.
RoomPredecessor parsePredecessor(const std::string& json);

// ==== Room Create Content ====
// Content of an m.room.create state event.
// Contains the room's creator, version, predecessor link, and type.
//
// JSON:
// {
//   "creator": "@alice:example.org",
//   "room_version": "9",
//   "predecessor": {"room_id": "!old:example.org", "event_id": "$ev:example.org"},
//   "type": "m.space",
//   "m.federate": true
// }
//
// Original Kotlin (RoomCreateContent.kt:28-37):
//   @JsonClass(generateAdapter = true)
//   data class RoomCreateContent(
//       @Json(name = "creator") val creator: String? = null,
//       @Json(name = "room_version") val roomVersion: String? = null,
//       @Json(name = "predecessor") val predecessor: Predecessor? = null,
//       @Json(name = "type") val type: String? = null,
//       @Json(name = "additional_creators") val additionalCreators: List<String>? = null,
//   )

struct RoomCreateContent {
    std::string creator;                   // User ID who created the room
    std::string roomVersion;               // Room version (e.g. "1", "9", "10")
    RoomPredecessor predecessor;           // Link to the old room this replaced
    std::string type;                      // Optional room type (e.g. "m.space")
    bool federate = true;                  // Whether the room can be federated
    bool mFederate = true;                 // Legacy federation flag (deprecated)
    std::vector<std::string> additionalCreators;  // Extra creators beside sender

    bool hasPredecessor() const {
        return !predecessor.roomId.empty();
    }
};

// Parse RoomCreateContent from an m.room.create state event JSON.
// Handles both the full event (with "content" wrapper) and bare content JSON.
//
// Original Kotlin (DefaultRoomVersionService.kt:44-51):
//   stateEventDataSource.getStateEvent(roomId, EventType.STATE_ROOM_CREATE, ...)
//       ?.content?.toModel<RoomCreateContent>()?.roomVersion ?: DEFAULT_ROOM_VERSION
RoomCreateContent parseRoomCreateContent(const std::string& json);

// Build RoomCreateContent as JSON string.
std::string buildRoomCreateContent(const RoomCreateContent& c);

// ==== Tombstone Content JSON ====

// Build tombstone content JSON for m.room.tombstone.
// Output: {"body":"...","replacement_room":"!new:example.org"}
//
// Original Kotlin: Moshi serialization of RoomTombstoneContent
std::string buildTombstoneContent(const RoomTombstoneContent& content);

// Backward-compatible alias for buildTombstoneContent.
std::string tombstoneContentToJson(const RoomTombstoneContent& content);

// Parse RoomTombstoneContent from a state event JSON.
// Handles both full event (with "content" wrapper) and bare content JSON.
RoomTombstoneContent parseRoomTombstoneContent(const std::string& stateEventJson);

// ==== Room Upgrade Models ====
//
// RoomUpgradeInfo is defined in room_filter.hpp (shared across modules).
// It contains: roomId, replacementRoomId, predecessorRoomId, isPredecessor.
//
// Original Kotlin (derived from process + RoomVersionUpgradeTask):
//   The server returns {"replacement_room": "!new:example.org"}
//   The tombstone event body contains the replacement_room.
//   The room create content predecessor links back.

// Backward-compatible alias for backwards compat with jni_bridge.cpp.
//
// Original Kotlin (processRoomUpgrade in existing C++ port):
//   info.predecessorRoomId = roomId from context
//   info.successorRoomId = content.replacementRoomId
//   info.isUpgrade = true
struct UpgradeInfo {
    std::string predecessorRoomId;    // the old room (tombstoned)
    std::string successorRoomId;      // the new room (replacement)
    bool isUpgrade = false;           // true if both rooms exist
    std::string noticeText;           // for timeline display
};

// ==== Room Upgrade Body ====
// POST body for the /rooms/{roomId}/upgrade API.
//
// Original Kotlin (RoomUpgradeBody.kt:22-26):
//   @JsonClass(generateAdapter = true)
//   internal data class RoomUpgradeBody(
//       @Json(name = "new_version") val newVersion: String
//   )
//
// Extended with optional fields for a richer upgrade request.
//
// JSON: {"new_version":"9","new_room_name":"New Room","invite_users":["@bob:host"]}

struct RoomUpgradeBody {
    std::string newVersion;                 // Target room version (e.g. "9")
    std::string newRoomName;                // Optional: name for the new room
    std::string newRoomTopic;               // Optional: topic for the new room
    std::vector<std::string> inviteUsers;   // Optional: users to invite to the new room
    bool enableEncryption = true;           // Whether to enable encryption in the new room
};

// Build the JSON body for a POST /rooms/{roomId}/upgrade request.
//
// Original Kotlin (RoomVersionUpgradeTask.kt:48-54):
//   executeRequest(globalErrorReceiver) {
//       roomAPI.upgradeRoom(roomId = params.roomId, body = RoomUpgradeBody(params.newVersion))
//   }.replacementRoomId
std::string buildRoomUpgradeBody(const RoomUpgradeBody& body);

// ==== Tombstone Analytics ====

// Check whether the room is currently tombstoned (has a valid replacement).
// Parses the tombstone event JSON and returns true if replacement_room is set.
//
// Original Kotlin (RoomTombstoneEventProcessor.kt:33-36):
//   val createRoomContent = event.getClearContent().toModel<RoomTombstoneContent>()
//   if (createRoomContent?.replacementRoomId == null) return
bool isRoomTombstoned(const std::string& tombstoneEventJson);

// Extract the replacement room ID from a tombstone event.
// Returns empty string if the room is not tombstoned.
//
// Original Kotlin (RoomTombstoneEventProcessor.kt:35-36):
//   val replacementRoomId = createRoomContent.replacementRoomId
std::string getReplacementRoomId(const std::string& tombstoneEventJson);

// ==== Tombstone Event Processing ====

// Process a tombstone event: given the current versioning state and membership,
// compute the new versioning state.
//
// Original Kotlin (RoomTombstoneEventProcessor.kt:33-44):
//   if (event.roomId == null) return
//   val createRoomContent = event.getClearContent().toModel<RoomTombstoneContent>()
//   if (createRoomContent?.replacementRoomId == null) return
//   val predecessorRoomSummary = RoomSummaryEntity.where(realm, event.roomId).findFirst()
//       ?: RoomSummaryEntity(event.roomId)
//   if (predecessorRoomSummary.versioningState == VersioningState.NONE) {
//       predecessorRoomSummary.versioningState = VersioningState.UPGRADED_ROOM_NOT_JOINED
//   }
//   realm.insertOrUpdate(predecessorRoomSummary)
//
// @param tombstoneEventJson   the m.room.tombstone event JSON
// @param currentMembership    current room membership ("join", "leave", etc.)
// @param currentState         existing versioning state for this room
// @return                     the new versioning state
VersioningState processTombstoneEvent(
    const std::string& tombstoneEventJson,
    const std::string& currentMembership,
    VersioningState currentState);

// Detect whether an event type should trigger tombstone processing.
//
// Original Kotlin (RoomTombstoneEventProcessor.kt:46-48):
//   override fun shouldProcess(eventId, eventType, insertType): Boolean {
//       return eventType == EventType.STATE_ROOM_TOMBSTONE
//   }
bool shouldProcessTombstoneEvent(const std::string& eventType);

// ==== Room Upgrade Handler (legacy, backward compat) ====

// Process a tombstone event to extract upgrade info.
UpgradeInfo processRoomUpgrade(const std::string& tombstoneEventJson);

// Format the upgrade notice for timeline display.
std::string formatUpgradeNotice(const UpgradeInfo& info);

} // namespace progressive
