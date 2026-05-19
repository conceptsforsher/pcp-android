#include "progressive/sync_models.hpp"
#include <cstring>

namespace progressive {

// ==== JSON Helpers ====

static std::string extractJsonString(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return "";
    pos = json.find(':', pos);
    if (pos == std::string::npos) return "";
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    if (pos >= json.size() || json[pos] != '"') return "";
    pos++;
    size_t end = pos;
    while (end < json.size() && json[end] != '"') {
        if (json[end] == '\\') end++;
        end++;
    }
    return json.substr(pos, end - pos);
}

static int extractJsonInt(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return 0;
    pos = json.find(':', pos);
    if (pos == std::string::npos) return 0;
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    if (pos >= json.size()) return 0;
    int val = 0;
    while (pos < json.size() && json[pos] >= '0' && json[pos] <= '9') {
        val = val * 10 + (json[pos] - '0');
        pos++;
    }
    return val;
}

static int64_t extractJsonInt64(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return 0;
    pos = json.find(':', pos);
    if (pos == std::string::npos) return 0;
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    if (pos >= json.size()) return 0;
    int64_t val = 0;
    while (pos < json.size() && json[pos] >= '0' && json[pos] <= '9') {
        val = val * 10 + (json[pos] - '0');
        pos++;
    }
    return val;
}

static bool extractJsonBool(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return false;
    pos = json.find(':', pos);
    if (pos == std::string::npos) return false;
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    return json.compare(pos, 4, "true") == 0;
}

static std::string extractJsonObject(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return "";
    pos = json.find(':', pos);
    if (pos == std::string::npos) return "";
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    if (pos >= json.size() || json[pos] != '{') return "";
    int depth = 1;
    size_t start = pos;
    pos++;
    while (pos < json.size() && depth > 0) {
        if (json[pos] == '{') depth++;
        else if (json[pos] == '}') depth--;
        pos++;
    }
    return json.substr(start, pos - start);
}

static std::vector<std::string> extractJsonStringArray(const std::string& json, const std::string& key) {
    std::vector<std::string> result;
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return result;
    pos = json.find('[', pos);
    if (pos == std::string::npos) return result;
    pos++;
    while (pos < json.size()) {
        while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == ',' || json[pos] == '\n')) pos++;
        if (pos >= json.size() || json[pos] == ']') break;
        if (json[pos] == '"') {
            pos++;
            size_t end = pos;
            while (end < json.size() && json[end] != '"') end++;
            result.push_back(json.substr(pos, end - pos));
            pos = end + 1;
        } else {
            pos++;
        }
    }
    return result;
}

static std::vector<Event> extractJsonEventsArray(const std::string& json, const std::string& key) {
    std::vector<Event> result;
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return result;
    pos = json.find('[', pos);
    if (pos == std::string::npos) return result;
    pos++;
    while (pos < json.size()) {
        while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == ',' || json[pos] == '\n')) pos++;
        if (pos >= json.size() || json[pos] == ']') break;
        if (json[pos] == '{') {
            int depth = 1;
            size_t start = pos;
            pos++;
            while (pos < json.size() && depth > 0) {
                if (json[pos] == '{') depth++;
                else if (json[pos] == '}') depth--;
                pos++;
            }
            std::string eventJson = json.substr(start, pos - start);
            result.push_back(parseEvent(eventJson));
        } else {
            pos++;
        }
    }
    return result;
}

// Extract a top-level string value that may appear first or second (for stable/unstable keys)
static std::string extractJsonStringAnyOf(const std::string& json, const std::string& key1, const std::string& key2) {
    auto val = extractJsonString(json, key1);
    if (!val.empty()) return val;
    return extractJsonString(json, key2);
}

// Extract cached raw content substring from within a JSON object (by key), keeping it as-is
static std::string extractRawJsonValue(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return "";
    pos = json.find(':', pos);
    if (pos == std::string::npos) return "";
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n')) pos++;
    if (pos >= json.size()) return "";

    if (json[pos] == '"') {
        pos++;
        size_t end = pos;
        while (end < json.size() && json[end] != '"') {
            if (json[end] == '\\') end++;
            end++;
        }
        return json.substr(pos - 1, end - pos + 2); // include quotes for string values
    }
    if (json[pos] == '{' || json[pos] == '[') {
        char open = json[pos];
        char close = (open == '{') ? '}' : ']';
        int depth = 1;
        size_t start = pos;
        pos++;
        while (pos < json.size() && depth > 0) {
            if (json[pos] == open) depth++;
            else if (json[pos] == close) depth--;
            pos++;
        }
        return json.substr(start, pos - start);
    }
    if (json[pos] == 't' || json[pos] == 'f') {
        size_t end = pos;
        while (end < json.size() && json[end] != ',' && json[end] != '}' && json[end] != ']') end++;
        return json.substr(pos, end - pos);
    }
    if (json[pos] >= '0' && json[pos] <= '9' || json[pos] == '-') {
        size_t end = pos;
        while (end < json.size() && json[end] != ',' && json[end] != '}' && json[end] != ']') end++;
        return json.substr(pos, end - pos);
    }
    return "";
}

// ==== Parse SyncSection ====

SyncDeviceListResponse parseSyncDeviceList(const std::string& json) {
    SyncDeviceListResponse r;
    r.changed = extractJsonStringArray(json, "changed");
    r.left = extractJsonStringArray(json, "left");
    return r;
}

SyncRoomTimeline parseSyncTimeline(const std::string& json) {
    SyncRoomTimeline t;
    t.events = extractJsonEventsArray(json, "events");
    t.limited = extractJsonBool(json, "limited");
    t.prevToken = extractJsonString(json, "prev_batch");
    return t;
}

SyncUnreadNotifications parseSyncUnreadNotifications(const std::string& json) {
    SyncUnreadNotifications u;
    // Original Kotlin: unread_notifications may also carry an "events" sub-array
    u.events = extractJsonEventsArray(json, "events");
    u.notificationCount = extractJsonInt(json, "notification_count");
    u.highlightCount = extractJsonInt(json, "highlight_count");
    return u;
}

SyncRoomSummary parseSyncRoomSummary(const std::string& json) {
    SyncRoomSummary s;
    s.heroes = extractJsonStringArray(json, "m.heroes");
    s.joinedMembersCount = extractJsonInt(json, "m.joined_member_count");
    s.invitedMembersCount = extractJsonInt(json, "m.invited_member_count");
    return s;
}

// ==== Parse RoomSync ====
//
// Original Kotlin (RoomSync.kt:26-54):
//   data class RoomSync(state, timeline, ephemeral, accountData,
//       unreadNotifications, unreadThreadNotifications, summary)

SyncRoom parseSyncRoom(const std::string& json) {
    SyncRoom room;

    auto stateJson = extractJsonObject(json, "state");
    if (!stateJson.empty()) {
        room.state.events = extractJsonEventsArray(stateJson, "events");
    }

    auto timelineJson = extractJsonObject(json, "timeline");
    if (!timelineJson.empty()) {
        room.timeline = parseSyncTimeline(timelineJson);
    }

    // Ephemeral — store as lazy (raw JSON), parse on demand
    auto ephemeralJson = extractJsonObject(json, "ephemeral");
    if (!ephemeralJson.empty()) {
        room.ephemeral.storedJson = ephemeralJson;
        room.ephemeral.state = EphemeralState::STORED;
    }

    auto acctJson = extractJsonObject(json, "account_data");
    if (!acctJson.empty()) {
        room.accountData.events = extractJsonEventsArray(acctJson, "events");
    }

    auto unreadJson = extractJsonObject(json, "unread_notifications");
    if (!unreadJson.empty()) {
        room.unreadNotifications = parseSyncUnreadNotifications(unreadJson);
    }

    // Original Kotlin: unread_thread_notifications is a map of thread_id -> {notification_count, highlight_count}
    auto utnJson = extractJsonObject(json, "unread_thread_notifications");
    if (!utnJson.empty()) {
        size_t pos = 1;
        while (pos < utnJson.size()) {
            while (pos < utnJson.size() && (utnJson[pos] == ' ' || utnJson[pos] == ',' || utnJson[pos] == '\n')) pos++;
            if (pos >= utnJson.size() || utnJson[pos] == '}') break;
            if (utnJson[pos] == '"') {
                pos++;
                size_t keyEnd = pos;
                while (keyEnd < utnJson.size() && utnJson[keyEnd] != '"') keyEnd++;
                std::string threadId = utnJson.substr(pos, keyEnd - pos);
                pos = keyEnd + 1;
                while (pos < utnJson.size() && utnJson[pos] != ':') pos++;
                pos++;
                while (pos < utnJson.size() && (utnJson[pos] == ' ' || utnJson[pos] == '\n')) pos++;
                if (pos < utnJson.size() && utnJson[pos] == '{') {
                    int depth = 1;
                    size_t start = pos;
                    pos++;
                    while (pos < utnJson.size() && depth > 0) {
                        if (utnJson[pos] == '{') depth++;
                        else if (utnJson[pos] == '}') depth--;
                        pos++;
                    }
                    std::string threadJson = utnJson.substr(start, pos - start);
                    SyncUnreadThreadNotifications tn;
                    tn.notificationCount = extractJsonInt(threadJson, "notification_count");
                    tn.highlightCount = extractJsonInt(threadJson, "highlight_count");
                    room.unreadThreadNotifications[threadId] = tn;
                }
            }
        }
    }

    auto summaryJson = extractJsonObject(json, "summary");
    if (!summaryJson.empty()) {
        room.summary = parseSyncRoomSummary(summaryJson);
    }

    return room;
}

// ==== Parse RoomsSyncResponse ====
//
// Original Kotlin (RoomsSyncResponse.kt:25-39):
//   data class RoomsSyncResponse(join, invite, leave)
//
// JSON: {"join":{"!room:server":{...}},"invite":{...},"leave":{...}}

SyncRoomsResponse parseSyncRooms(const std::string& json) {
    SyncRoomsResponse rooms;

    // Parse "join" map
    auto joinJson = extractJsonObject(json, "join");
    if (!joinJson.empty()) {
        size_t pos = 1;
        while (pos < joinJson.size()) {
            while (pos < joinJson.size() && (joinJson[pos] == ' ' || joinJson[pos] == ',' || joinJson[pos] == '\n')) pos++;
            if (pos >= joinJson.size() || joinJson[pos] == '}') break;
            if (joinJson[pos] == '"') {
                pos++;
                size_t keyEnd = pos;
                while (keyEnd < joinJson.size() && joinJson[keyEnd] != '"') keyEnd++;
                std::string roomId = joinJson.substr(pos, keyEnd - pos);
                pos = keyEnd + 1;
                while (pos < joinJson.size() && joinJson[pos] != ':') pos++;
                pos++;
                while (pos < joinJson.size() && (joinJson[pos] == ' ' || joinJson[pos] == '\n')) pos++;
                if (pos < joinJson.size() && joinJson[pos] == '{') {
                    int depth = 1;
                    size_t start = pos;
                    pos++;
                    while (pos < joinJson.size() && depth > 0) {
                        if (joinJson[pos] == '{') depth++;
                        else if (joinJson[pos] == '}') depth--;
                        pos++;
                    }
                    rooms.join[roomId] = parseSyncRoom(joinJson.substr(start, pos - start));
                }
            }
        }
    }

    // Parse "invite" map
    auto inviteJson = extractJsonObject(json, "invite");
    if (!inviteJson.empty()) {
        size_t pos = 1;
        while (pos < inviteJson.size()) {
            while (pos < inviteJson.size() && (inviteJson[pos] == ' ' || inviteJson[pos] == ',' || inviteJson[pos] == '\n')) pos++;
            if (pos >= inviteJson.size() || inviteJson[pos] == '}') break;
            if (inviteJson[pos] == '"') {
                pos++;
                size_t keyEnd = pos;
                while (keyEnd < inviteJson.size() && inviteJson[keyEnd] != '"') keyEnd++;
                std::string roomId = inviteJson.substr(pos, keyEnd - pos);
                pos = keyEnd + 1;
                while (pos < inviteJson.size() && inviteJson[pos] != ':') pos++;
                pos++;
                while (pos < inviteJson.size() && (inviteJson[pos] == ' ' || inviteJson[pos] == '\n')) pos++;
                if (pos < inviteJson.size() && inviteJson[pos] == '{') {
                    int depth = 1;
                    size_t start = pos;
                    pos++;
                    while (pos < inviteJson.size() && depth > 0) {
                        if (inviteJson[pos] == '{') depth++;
                        else if (inviteJson[pos] == '}') depth--;
                        pos++;
                    }
                    std::string invJson = inviteJson.substr(start, pos - start);
                    SyncInvitedRoom inv;
                    auto isJson = extractJsonObject(invJson, "invite_state");
                    if (!isJson.empty()) {
                        inv.inviteState.events = extractJsonEventsArray(isJson, "events");
                    }
                    rooms.invite[roomId] = inv;
                }
            }
        }
    }

    // Parse "leave" map
    auto leaveJson = extractJsonObject(json, "leave");
    if (!leaveJson.empty()) {
        size_t pos = 1;
        while (pos < leaveJson.size()) {
            while (pos < leaveJson.size() && (leaveJson[pos] == ' ' || leaveJson[pos] == ',' || leaveJson[pos] == '\n')) pos++;
            if (pos >= leaveJson.size() || leaveJson[pos] == '}') break;
            if (leaveJson[pos] == '"') {
                pos++;
                size_t keyEnd = pos;
                while (keyEnd < leaveJson.size() && leaveJson[keyEnd] != '"') keyEnd++;
                std::string roomId = leaveJson.substr(pos, keyEnd - pos);
                pos = keyEnd + 1;
                while (pos < leaveJson.size() && leaveJson[pos] != ':') pos++;
                pos++;
                while (pos < leaveJson.size() && (leaveJson[pos] == ' ' || leaveJson[pos] == '\n')) pos++;
                if (pos < leaveJson.size() && leaveJson[pos] == '{') {
                    int depth = 1;
                    size_t start = pos;
                    pos++;
                    while (pos < leaveJson.size() && depth > 0) {
                        if (leaveJson[pos] == '{') depth++;
                        else if (leaveJson[pos] == '}') depth--;
                        pos++;
                    }
                    rooms.leave[roomId] = parseSyncRoom(leaveJson.substr(start, pos - start));
                }
            }
        }
    }

    return rooms;
}

// ==== Parse Top-Level SyncResponse ====
//
// Original Kotlin (SyncResponse.kt:25-64)

SyncResponse parseSyncResponse(const std::string& json) {
    SyncResponse response;

    // Original Kotlin: account_data
    auto acctJson = extractJsonObject(json, "account_data");
    if (!acctJson.empty()) {
        response.accountData.events = extractJsonEventsArray(acctJson, "events");
    }

    // Original Kotlin: next_batch
    response.nextBatch = extractJsonString(json, "next_batch");

    // Original Kotlin: presence
    auto presJson = extractJsonObject(json, "presence");
    if (!presJson.empty()) {
        response.presence.events = extractJsonEventsArray(presJson, "events");
    }

    // Original Kotlin: to_device
    auto tdJson = extractJsonObject(json, "to_device");
    if (!tdJson.empty()) {
        response.toDevice.events = extractJsonEventsArray(tdJson, "events");
    }

    // Original Kotlin: rooms
    auto roomsJson = extractJsonObject(json, "rooms");
    if (!roomsJson.empty()) {
        response.rooms = parseSyncRooms(roomsJson);
    }

    // Original Kotlin: device_lists
    auto dlJson = extractJsonObject(json, "device_lists");
    if (!dlJson.empty()) {
        response.deviceLists = parseSyncDeviceList(dlJson);
    }

    // Original Kotlin: device_one_time_keys_count
    auto otkJson = extractJsonObject(json, "device_one_time_keys_count");
    if (!otkJson.empty()) {
        response.deviceOneTimeKeysCount.signedCurve25519 = extractJsonInt(otkJson, "signed_curve25519");
    }

    // Original Kotlin: device_unused_fallback_key_types
    //   val deviceUnusedFallbackKeyTypes = stableDeviceUnusedFallbackKeyTypes ?: devDeviceUnusedFallbackKeyTypes
    response.deviceUnusedFallbackKeyTypes = extractJsonStringArray(json, "device_unused_fallback_key_types");
    if (response.deviceUnusedFallbackKeyTypes.empty()) {
        response.deviceUnusedFallbackKeyTypes = extractJsonStringArray(json, "org.matrix.msc2732.device_unused_fallback_key_types");
    }

    return response;
}

// ==== Parse To-Device Response ====
//
// Original Kotlin (ToDeviceSyncResponse.kt:24-30):
//   data class ToDeviceSyncResponse(events: List<Event>?)
//
// Parses a to-device block as a list of ToDeviceEvent (simplified model).
// JSON: {"events":[{"sender":"@alice:example.org","type":"m.room_key","content":{...}}, ...]}

ParsedToDeviceResponse parseToDeviceResponse(const std::string& json) {
    ParsedToDeviceResponse response;

    auto pos = json.find("\"events\"");
    if (pos == std::string::npos) return response;
    pos = json.find('[', pos);
    if (pos == std::string::npos) return response;
    pos++;

    while (pos < json.size()) {
        while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == ',' || json[pos] == '\n')) pos++;
        if (pos >= json.size() || json[pos] == ']') break;
        if (json[pos] == '{') {
            int depth = 1;
            size_t start = pos;
            pos++;
            while (pos < json.size() && depth > 0) {
                if (json[pos] == '{') depth++;
                else if (json[pos] == '}') depth--;
                pos++;
            }
            std::string eventJson = json.substr(start, pos - start);

            ToDeviceEvent ev;
            ev.type = extractJsonString(eventJson, "type");
            ev.senderId = extractJsonString(eventJson, "sender");
            ev.contentJson = extractRawJsonValue(eventJson, "content");
            ev.encrypted = (ev.type == "m.room.encrypted");
            response.events.push_back(std::move(ev));
        } else {
            pos++;
        }
    }

    return response;
}

// ==== Parse User Account Data ====
//
// Original Kotlin (UserAccountDataEvent.kt:27-31):
//   data class UserAccountDataEvent(type: String, content: Content)
//   Simplified Event with just type and content for account data events.
//
// Original Kotlin (UserAccountDataSync.kt:25-27):
//   data class UserAccountDataSync(list: List<UserAccountDataEvent>)

std::vector<UserAccountDataEvent> parseUserAccountData(const std::string& json) {
    std::vector<UserAccountDataEvent> result;

    // account_data blocks use "events" key (or "list" in some wire formats)
    std::string keyName = "events";
    auto pos = json.find("\"" + keyName + "\"");
    if (pos == std::string::npos) {
        keyName = "list";
        pos = json.find("\"" + keyName + "\"");
    }
    if (pos == std::string::npos) return result;
    pos = json.find('[', pos);
    if (pos == std::string::npos) return result;
    pos++;

    while (pos < json.size()) {
        while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == ',' || json[pos] == '\n')) pos++;
        if (pos >= json.size() || json[pos] == ']') break;
        if (json[pos] == '{') {
            int depth = 1;
            size_t start = pos;
            pos++;
            while (pos < json.size() && depth > 0) {
                if (json[pos] == '{') depth++;
                else if (json[pos] == '}') depth--;
                pos++;
            }
            std::string eventJson = json.substr(start, pos - start);

            UserAccountDataEvent ev;
            ev.type = extractJsonString(eventJson, "type");
            ev.contentJson = extractRawJsonValue(eventJson, "content");
            result.push_back(std::move(ev));
        } else {
            pos++;
        }
    }

    return result;
}

// ==== Parse Devices List Response ====
//
// Original Kotlin (DevicesListResponse.kt:22-23):
//   data class DevicesListResponse(devices: List<DeviceInfo>?)
//
// Original Kotlin (DeviceInfo.kt:25-55):
//   data class DeviceInfo(userId, deviceId, displayName, lastSeenTs, lastSeenIp)
//
// JSON: {"devices":[{"user_id":"@alice:example.org","device_id":"ABCDEF",
//          "display_name":"Alice's Phone","last_seen_ts":1234567890,
//          "last_seen_ip":"1.2.3.4"}, ...]}

ParsedDevicesListResponse parseDeviceListsResponse(const std::string& json) {
    ParsedDevicesListResponse response;

    auto pos = json.find("\"devices\"");
    if (pos == std::string::npos) return response;
    pos = json.find('[', pos);
    if (pos == std::string::npos) return response;
    pos++;

    while (pos < json.size()) {
        while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == ',' || json[pos] == '\n')) pos++;
        if (pos >= json.size() || json[pos] == ']') break;
        if (json[pos] == '{') {
            int depth = 1;
            size_t start = pos;
            pos++;
            while (pos < json.size() && depth > 0) {
                if (json[pos] == '{') depth++;
                else if (json[pos] == '}') depth--;
                pos++;
            }
            std::string devJson = json.substr(start, pos - start);

            DeviceInfoUpdate di;
            di.userId = extractJsonString(devJson, "user_id");
            di.deviceId = extractJsonString(devJson, "device_id");
            di.displayName = extractJsonString(devJson, "display_name");
            di.lastSeenTs = extractJsonInt64(devJson, "last_seen_ts");
            di.lastSeenIp = extractJsonString(devJson, "last_seen_ip");
            response.devices.push_back(std::move(di));
        } else {
            pos++;
        }
    }

    return response;
}

// ==== Parse Initial Sync Metadata ====
//
// Original Kotlin (InitialSyncResponseParser.kt:43-65):
//   fun parse(syncStrategy: InitialSyncStrategy.Optimized, workingFile: File): SyncResponse
//
// Extracts the initial-sync progress step and metadata from a cached sync file or
// partially-parsed JSON blob. This is used to report download/import progress.
//
// JSON (top-level): {"next_batch":"...", "presence":{...}, "account_data":{...}, ...}

InitialSyncStep parseInitialSyncMetadata(const std::string& json) {
    InitialSyncStep step;

    // Determine step type by checking which sections are present
    auto nextBatch = extractJsonString(json, "next_batch");
    auto roomsJson = extractJsonObject(json, "rooms");
    auto presJson = extractJsonObject(json, "presence");
    auto acctJson = extractJsonObject(json, "account_data");

    // If the file has rooms parsed, we're past the download phase
    if (!nextBatch.empty() && !roomsJson.empty()) {
        step.type = InitialSyncStepType::IMPORTING_ACCOUNT;
    } else if (!nextBatch.empty()) {
        step.type = InitialSyncStepType::DOWNLOADING;
    } else {
        // Deferred keys still pending — fallback to downloading
        step.type = InitialSyncStepType::DOWNLOADING;
    }

    // Estimate percent progress based on which sections exist
    if (step.type == InitialSyncStepType::DOWNLOADING) {
        // Check if we have partial data to estimate download progress
        step.percentProgress = nextBatch.empty() ? 10 : 50;
    } else if (step.type == InitialSyncStepType::IMPORTING_ACCOUNT) {
        // Account data import phase
        auto tdJson = extractJsonObject(json, "to_device");
        if (!acctJson.empty() && !presJson.empty()) {
            step.percentProgress = 70;
        } else {
            step.percentProgress = 30;
        }
    } else if (step.type == InitialSyncStepType::IMPORTING_DEFERRED_KEYS) {
        step.percentProgress = 85;
    }

    return step;
}

// ==== Serialize SyncResponse ====

std::string syncResponseToJson(const SyncResponse& response) {
    std::string json = "{";
    json += "\"next_batch\":\"" + response.nextBatch + "\"";
    // Minimal serialization — full serialization is complex
    // Used primarily for caching the next_batch token
    json += "}";
    return json;
}

} // namespace progressive
