#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>
#include "progressive/event_models.hpp"

namespace progressive {

// ==== Sync Models — Matrix /sync v2 API Response ====
//
// Ref: https://matrix.org/docs/spec/client_server/latest#get-matrix-client-r0-sync
//
// These structs mirror the Kotlin data classes in api/session/sync/model/

// Original Kotlin (DeviceListResponse.kt:25-30):
//   data class DeviceListResponse(changed: List<String>, left: List<String>)
struct SyncDeviceListResponse {
    std::vector<std::string> changed;  // user IDs with new crypto devices
    std::vector<std::string> left;     // user IDs no longer tracked
};

// Original Kotlin (DeviceOneTimeKeysCountSyncResponse.kt:25-27):
//   data class DeviceOneTimeKeysCountSyncResponse(signedCurve25519: Int?)
struct SyncDeviceOneTimeKeysCount {
    int signedCurve25519 = 0;
};

// Original Kotlin (PresenceSyncResponse.kt:26-32):
//   data class PresenceSyncResponse(events: List<Event>?)
struct SyncPresenceResponse {
    std::vector<Event> events;
};

// Original Kotlin (UserAccountDataSync.kt:25-27):
//   data class UserAccountDataSync(list: List<UserAccountDataEvent>)
struct SyncUserAccountData {
    std::vector<Event> events;  // account data events
};

// Original Kotlin (ToDeviceSyncResponse.kt:24-30):
//   data class ToDeviceSyncResponse(events: List<Event>?)
struct SyncToDeviceResponse {
    std::vector<Event> events;
};

// Original Kotlin (RoomSyncUnreadNotifications.kt:28-42):
//   data class RoomSyncUnreadNotifications(events, notificationCount, highlightCount)
struct SyncUnreadNotifications {
    std::vector<Event> events;
    int notificationCount = 0;
    int highlightCount = 0;
};

// Original Kotlin (RoomSyncUnreadThreadNotifications.kt:25-33):
//   data class RoomSyncUnreadThreadNotifications(notificationCount, highlightCount)
struct SyncUnreadThreadNotifications {
    int notificationCount = 0;
    int highlightCount = 0;
};

// Original Kotlin (RoomSyncSummary.kt:26-46):
//   data class RoomSyncSummary(heroes, joinedMembersCount, invitedMembersCount)
struct SyncRoomSummary {
    std::vector<std::string> heroes;       // "m.heroes" key
    int joinedMembersCount = 0;            // "m.joined_member_count"
    int invitedMembersCount = 0;           // "m.invited_member_count"
};

// Original Kotlin (RoomSyncTimeline.kt:25-37):
//   data class RoomSyncTimeline(events, limited, prevToken)
struct SyncRoomTimeline {
    std::vector<Event> events;
    bool limited = false;                  // "limited" key
    std::string prevToken;                 // "prev_batch" key — pagination token
};

// Original Kotlin (RoomSyncState.kt:25-31):
//   data class RoomSyncState(events: List<Event>?)
struct SyncRoomState {
    std::vector<Event> events;
};

// ==== Ephemeral Events (typed) ====
//
// Original Kotlin (EphemeralEvent.kt / TypingSyncEphemeralEvent.kt / ReceiptSyncEphemeralEvent.kt):
//   Ephemeral events are room-level events that are not stored in the room history.
//   They include typing notifications (m.typing) and read receipts (m.receipt).

// Original Kotlin (TypingSyncEphemeralEvent.kt:22-30):
//   data class TypingSyncEphemeralEvent(userIds: List<String>)
struct SyncTypingEvent {
    std::vector<std::string> userIds;      // users currently typing
};

// Original Kotlin (ReceiptSyncEphemeralEvent.kt:22-28):
//   data class ReceiptSyncEphemeralEvent(eventIdToReadReceipts: Map<String, ReadReceiptsData>)
//   Where ReadReceiptsData = Map<String, ReceiptInfo> (userId -> ReceiptInfo)
//   And ReceiptInfo(originalTs: Long?, threadId: String?)

struct SyncReceiptInfo {
    int64_t originalTs = 0;               // "ts" key — timestamp of the read event
    std::string threadId;                  // "thread_id" key — for threaded receipts (MSC3771)
};

// Map from userId -> ReceiptInfo for a given event
struct SyncReadReceiptsData {
    std::unordered_map<std::string, SyncReceiptInfo> receipts;
};

// Original Kotlin (ReceiptSyncEphemeralEvent.kt):
//   The ephemeral receipt data: event_id -> per-user receipt info
struct SyncReceiptEvent {
    std::unordered_map<std::string, SyncReadReceiptsData> eventIdToReceipts; // eventId -> {userId -> info}
};

// Discriminated ephemeral event: either typing or receipts (or other/unparsed)
enum class SyncEphemeralEventType {
    TYPING = 0,
    RECEIPT = 1,
    OTHER = 2
};

// Original Kotlin (LazyRoomSyncEphemeral.kt:22-25):
//   sealed class LazyRoomSyncEphemeral {
//       data class Parsed(val roomSyncEphemeral: RoomSyncEphemeral)
//       object Stored
//   }
enum class EphemeralState {
    STORED = 0,     // Not parsed yet, stored as raw JSON
    PARSED = 1      // Already parsed into SyncRoomEphemeral
};

// Original Kotlin (RoomSyncEphemeral.kt:25-31):
//   data class RoomSyncEphemeral(events: List<Event>?)
struct SyncRoomEphemeral {
    std::vector<Event> events;             // typing, receipts
};

struct SyncLazyEphemeral {
    EphemeralState state = EphemeralState::STORED;
    SyncRoomEphemeral parsed;              // Only valid if state == PARSED
    std::string storedJson;                // Raw JSON if state == STORED
};

// Original Kotlin (RoomSyncAccountData.kt:26-31):
//   data class RoomSyncAccountData(events: List<Event>?)
struct SyncRoomAccountData {
    std::vector<Event> events;             // tags, etc.
};

// Original Kotlin (RoomSync.kt:26-54):
//   data class RoomSync(state, timeline, ephemeral, accountData,
//       unreadNotifications, unreadThreadNotifications, summary)
struct SyncRoom {
    SyncRoomState state;                   // "state" key
    SyncRoomTimeline timeline;             // "timeline" key
    SyncLazyEphemeral ephemeral;           // "ephemeral" key (lazy)
    SyncRoomAccountData accountData;       // "account_data" key
    SyncUnreadNotifications unreadNotifications;     // "unread_notifications"
    std::unordered_map<std::string, SyncUnreadThreadNotifications> unreadThreadNotifications; // "unread_thread_notifications"
    SyncRoomSummary summary;               // "summary" key
};

// Original Kotlin (RoomInviteState.kt:25-30):
//   data class RoomInviteState(events: List<Event>)
struct SyncRoomInviteState {
    std::vector<Event> events;
};

// Original Kotlin (InvitedRoomSync.kt:25-35):
//   data class InvitedRoomSync(inviteState: RoomInviteState?)
struct SyncInvitedRoom {
    SyncRoomInviteState inviteState;       // "invite_state" key
};

// Original Kotlin (RoomsSyncResponse.kt:25-39):
//   data class RoomsSyncResponse(join, invite, leave)
struct SyncRoomsResponse {
    // key = room_id, value = room sync data
    std::unordered_map<std::string, SyncRoom> join;
    std::unordered_map<std::string, SyncInvitedRoom> invite;
    std::unordered_map<std::string, SyncRoom> leave;
};

// ==== Top-Level Sync Response ====
//
// Original Kotlin (SyncResponse.kt:25-64):
//   data class SyncResponse(
//       accountData, nextBatch, presence, toDevice,
//       rooms, deviceLists, deviceOneTimeKeysCount,
//       devDeviceUnusedFallbackKeyTypes, stableDeviceUnusedFallbackKeyTypes
//   )

struct SyncResponse {
    SyncUserAccountData accountData;        // "account_data" key
    std::string nextBatch;                  // "next_batch" key — pagination token
    SyncPresenceResponse presence;          // "presence" key
    SyncToDeviceResponse toDevice;          // "to_device" key
    SyncRoomsResponse rooms;                // "rooms" key
    SyncDeviceListResponse deviceLists;     // "device_lists" key
    SyncDeviceOneTimeKeysCount deviceOneTimeKeysCount; // "device_one_time_keys_count"
    std::vector<std::string> deviceUnusedFallbackKeyTypes; // "device_unused_fallback_key_types"

    bool empty() const { return nextBatch.empty() && rooms.join.empty()
        && rooms.invite.empty() && rooms.leave.empty(); }
};

// ==== User Account Data Event ====
//
// Original Kotlin (UserAccountDataEvent.kt:27-31):
//   data class UserAccountDataEvent(type: String, content: Content)
//   Simplified Event with just type and content for account data events.
//   Used types are defined in UserAccountDataTypes.

struct UserAccountDataEvent {
    std::string type;                       // "type" key — e.g. "m.direct", "m.ignored_user_list"
    std::string contentJson;                // "content" key — raw JSON content object
};

// ==== To-Device Event ====
//
// Original Kotlin (ToDeviceEvent.kt / ToDeviceSyncResponse.kt):
//   To-device events are sent directly between devices without passing through rooms.
//   Examples: m.room_key, m.room_key_request, m.new_device

struct ToDeviceEvent {
    std::string type;                       // "type" key — e.g. "m.room_key"
    std::string senderId;                   // "sender" key
    std::string contentJson;                // "content" key — raw JSON content object
    std::string encrypted;                  // true if this is an olm-encrypted to-device msg
};

// Parsed to-device response (separate from inline SyncToDeviceResponse parsing)
//
// Original Kotlin (ToDeviceSyncResponse.kt:24-30):
//   data class ToDeviceSyncResponse(events: List<Event>?)
struct ParsedToDeviceResponse {
    std::vector<ToDeviceEvent> events;
};

// ==== Device Info Update ====
//
// Original Kotlin (DeviceInfo.kt:25-55):
//   data class DeviceInfo(userId, deviceId, displayName, lastSeenTs, lastSeenIp)
//   This class describes the device information received from a device list update.

struct DeviceInfoUpdate {
    std::string userId;                     // "user_id" key
    std::string deviceId;                   // "device_id" key
    std::string displayName;                // "display_name" key
    int64_t lastSeenTs = 0;                 // "last_seen_ts" key
    std::string lastSeenIp;                 // "last_seen_ip" key
};

// Original Kotlin (DevicesListResponse.kt:22-23):
//   data class DevicesListResponse(devices: List<DeviceInfo>?)
//   Internal model for /devices endpoint response.
struct ParsedDevicesListResponse {
    std::vector<DeviceInfoUpdate> devices;
};

// ==== JSON Parsing ====

SyncResponse parseSyncResponse(const std::string& json);
SyncRoomsResponse parseSyncRooms(const std::string& json);
SyncRoom parseSyncRoom(const std::string& json);
SyncRoomTimeline parseSyncTimeline(const std::string& json);
SyncUnreadNotifications parseSyncUnreadNotifications(const std::string& json);
SyncRoomSummary parseSyncRoomSummary(const std::string& json);
SyncDeviceListResponse parseSyncDeviceList(const std::string& json);

// Parse to-device events from a /sync to_device block
//
// Original Kotlin (ToDeviceSyncResponse adapter / ToDeviceEventsHandler.kt):
//   parseToDeviceResponse(json) -> ParsedToDeviceResponse
ParsedToDeviceResponse parseToDeviceResponse(const std::string& json);

// Parse user account data events (simplified Event with type + content)
//
// Original Kotlin (UserAccountDataSync.kt adapter):
//   parseUserAccountData(json) -> vector<UserAccountDataEvent>
std::vector<UserAccountDataEvent> parseUserAccountData(const std::string& json);

// Parse device list response from /devices API
//
// Original Kotlin (DevicesListResponse.kt adapter):
//   parseDeviceListsResponse(json) -> ParsedDevicesListResponse
ParsedDevicesListResponse parseDeviceListsResponse(const std::string& json);

// Parse initial sync metadata from cached sync file
//
// Original Kotlin (InitialSyncResponseParser.kt:43-65):
//   fun parse(syncStrategy, workingFile): SyncResponse
//   The parser extracts next_batch, presence, account_data, etc. from a stored
//   initial sync file to allow incremental sync to resume.
InitialSyncStep parseInitialSyncMetadata(const std::string& json);

// Serialize top-level sync response (for caching)
std::string syncResponseToJson(const SyncResponse& response);

// ==== Sync State Machine ====
//
// Original Kotlin (SyncState.kt:24-27), (SyncRequestState.kt:20-44), (InitialSyncStrategy.kt:22-53)

enum class SyncStateType { IDLE = 0, RUNNING = 1, PAUSED = 2, KILLING = 3, KILLED = 4, NO_NETWORK = 5, INVALID_TOKEN = 6 };

struct SyncState {
    SyncStateType type = SyncStateType::IDLE;
    bool afterPause = false;             // for RUNNING state
};

enum class InitialSyncStepType { DOWNLOADING = 0, IMPORTING_ACCOUNT = 1, IMPORTING_DEFERRED_KEYS = 2 };

struct InitialSyncStep {
    InitialSyncStepType type = InitialSyncStepType::DOWNLOADING;
    int percentProgress = 0;
};

struct IncrementalSyncParsing {
    int rooms = 0;
    int toDevice = 0;
};

enum class SyncRequestStateType { IDLE = 0, INITIAL_PROGRESS = 1, INCREMENTAL_IDLE = 2, INCREMENTAL_PARSING = 3, INCREMENTAL_ERROR = 4, INCREMENTAL_DONE = 5 };

struct SyncRequestState {
    SyncRequestStateType type = SyncRequestStateType::IDLE;
    InitialSyncStep initialStep;         // for INITIAL_PROGRESS
    IncrementalSyncParsing parsing;       // for INCREMENTAL_PARSING
};

enum class InitialSyncStrategyType { LEGACY = 0, OPTIMIZED = 1 };

struct InitialSyncStrategy {
    InitialSyncStrategyType type = InitialSyncStrategyType::OPTIMIZED;
    int64_t minSizeToSplit = 1048576;    // 1 MB
    int64_t minSizeToStoreInFile = 1024; // 1 KB
    int maxRoomsToInsert = 100;
};

} // namespace progressive
