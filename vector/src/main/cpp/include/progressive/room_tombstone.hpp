#pragma once

#include <string>

namespace progressive {

// Room Tombstone — handles room upgrade/replacement events.
// When a room is upgraded (e.g. from version 5 to 6), the server sends
// a "m.room.tombstone" state event pointing to the replacement room.
//
// Ref: https://matrix.org/docs/spec/client_server/latest#room-upgrades
//
// Original Kotlin (RoomTombstoneContent.kt:22-35):
//   data class RoomTombstoneContent(
//       @Json(name = "body") val body: String? = null,
//       @Json(name = "replacement_room") val replacementRoomId: String?
//   )
//
// JSON: {"body": "This room has been replaced", "replacement_room": "!newroom:example.org"}

struct RoomTombstoneContent {
    std::string body;                // Server-defined message explaining the tombstone
    std::string replacementRoomId;   // The new room the client should navigate to

    bool isUpgrade() const {
        // Original Kotlin: createRoomContent?.replacementRoomId == null → return
        return !replacementRoomId.empty();
    }
};

// Versioning state for a room after tombstone processing.
//
// Original Kotlin (RoomTombstoneEventProcessor.kt:37-43):
//   if (predecessorRoomSummary.versioningState == VersioningState.NONE) {
//       predecessorRoomSummary.versioningState =
//           VersioningState.UPGRADED_ROOM_NOT_JOINED
//   }
enum class VersioningState {
    NONE = 0,
    UPGRADED_ROOM_NOT_JOINED = 1,
    UPGRADED_ROOM_JOINED = 2,
    PREDECESSOR_ROOM = 3
};

const char* versioningStateToString(VersioningState state);

// Parse RoomTombstoneContent from a state event JSON.
//
// Original Kotlin (RoomTombstoneEventProcessor.kt:30-34):
//   event.roomId == null → return
//   val createRoomContent = event.getClearContent()
//       .toModel<RoomTombstoneContent>()
//   if (createRoomContent?.replacementRoomId == null) return
RoomTombstoneContent parseRoomTombstoneContent(const std::string& stateEventJson);

// Process a tombstone event for a room:
// - Check if replacement room exists
// - Update versioning state (UPGRADED_ROOM_NOT_JOINED)
//
// Original Kotlin (RoomTombstoneEventProcessor.kt:36-43):
//   val predecessorRoomSummary = RoomSummaryEntity.where(realm, event.roomId)
//   if (predecessorRoomSummary.versioningState == VersioningState.NONE)
//       predecessorRoomSummary.versioningState = UPGRADED_ROOM_NOT_JOINED
bool shouldProcessTombstoneEvent(const std::string& eventType);

// Serialize tombstone content to JSON
std::string tombstoneContentToJson(const RoomTombstoneContent& content);

// ==== Room Upgrade Handler ====

struct UpgradeInfo {
    std::string predecessorRoomId;    // the old room (tombstoned)
    std::string successorRoomId;      // the new room (replacement)
    bool isUpgrade = false;           // true if both rooms exist
    std::string noticeText;           // for timeline display
};

// Process a tombstone event to extract upgrade info.
UpgradeInfo processRoomUpgrade(const std::string& tombstoneEventJson);

// Format the upgrade notice for timeline display.
std::string formatUpgradeNotice(const UpgradeInfo& info);

} // namespace progressive
