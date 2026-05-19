#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cstdint>

namespace progressive {

// ==== Event Decryptor — Deduplication & Retry Logic ====
//
// Original Kotlin (TimelineEventDecryptor.kt:84-179):
//   - requestDecryption(): dedup, skip unknown-session failures
//   - processDecryptRequest(): decrypt or mark thread-aware
//   - newSessionListener: re-enqueue failed decryptions on new Olm session

struct DecryptionRequest {
    std::string eventId;
    std::string roomId;
    std::string contentJson;            // encrypted content JSON
    std::string timelineId;
    bool isEncrypted = false;
};

struct DecryptionResult {
    bool success = false;
    std::string clearPayloadJson;       // {"type":"...","content":{...}}
    std::string senderKey;
    std::string errorCode;              // "M_UNKNOWN_SESSION", "M_BAD_MAC", etc.
    std::string errorReason;
    std::string sessionId;              // for retry-on-new-session tracking
};

// Check if we already have a pending request for this event.
// Returns true if the event should be decrypted (not a duplicate).
//
// Original Kotlin (TimelineEventDecryptor.kt:84-94):
//   if (existingRequests.contains(eventId)) return
//   if (unknownSessionsFailure.contains(sessionId)) return
//   existingRequests.add(eventId)
//   executor.submit { processDecryptRequest(...) }

struct DecryptorState {
    std::unordered_set<std::string> existingRequests;      // currently pending
    std::unordered_set<std::string> unknownSessionsFailure; // session IDs that failed
    std::unordered_map<std::string, std::vector<std::string>> sessionToEvents; // sessionId → eventIds
};

// Original Kotlin: deduplication check before enqueueing
inline bool shouldDecrypt(const DecryptorState& state, const DecryptionRequest& req) {
    if (state.existingRequests.count(req.eventId)) return false;
    // Check if this session is already known to fail
    // (sessionId extracted from contentJson's EncryptedEventContent)
    return true;
}

// Original Kotlin: when a new Olm session arrives, re-enqueue previously failed events
// fun onNewSession(sessionId) { unknownSessionsFailure.remove(sessionId); re-enqueue events }
inline std::vector<DecryptionRequest> onNewSession(DecryptorState& state, const std::string& sessionId) {
    state.unknownSessionsFailure.erase(sessionId);
    std::vector<DecryptionRequest> retry;
    auto it = state.sessionToEvents.find(sessionId);
    if (it != state.sessionToEvents.end()) {
        for (const auto& eventId : it->second) {
            state.existingRequests.insert(eventId);
            // In real impl: retry.push_back(buildRequest(eventId))
        }
        state.sessionToEvents.erase(it);
    }
    return retry;
}

// ==== Room Avatar Resolver ====
//
// Original Kotlin (RoomAvatarResolver.kt:48-84):
//   fun resolve(): String?
//   1. Try m.room.avatar state event → return avatarUrl if non-empty
//   2. For direct rooms: get active members excluding certain users
//      - If exactly 1 active member: return their avatar
//      - If exactly 2 active members: return the OTHER member's avatar
//   3. Return null

struct RoomMemberInfo {
    std::string userId;
    std::string displayName;
    std::string avatarUrl;
    bool isActive = true;               // membership = JOIN or INVITE
    bool isLeft = false;               // membership = LEAVE, BAN, KNOCK
};

// Resolve room avatar from state event + member list.
// currentUserId is excluded (we want the OTHER person's avatar for DMs).
//
// Original Kotlin (RoomAvatarResolver.kt:48-84):
//   val content = roomStateDataSource.getStateEvent(roomId, STATE_ROOM_AVATAR)
//   if (content?.avatarUrl != null) return avatarUrl
//   if (!roomSummary.isDirect) return null
//   val excludedUserIds = fallbackProvider(roomId)
//   val activeMembers = roomMembersDataSource.getRoomMembers(roomId, activeMemberships)
//   val activeOthers = activeMembers.filter { it.userId !in excludedUserIds }
//   if (activeOthers.size == 1) {
//       return leftMembers.firstOrNull()?.avatarUrl ?: activeOthers.first().avatarUrl
//   }
//   if (activeOthers.size == 2) {
//       return activeOthers.first { it.userId != currentUserId }.avatarUrl
//   }
//   return null

inline std::string resolveRoomAvatar(
    const std::string& roomAvatarUrl,         // from m.room.avatar state event
    bool isDirect,
    const std::vector<RoomMemberInfo>& activeMembers,
    const std::vector<RoomMemberInfo>& leftMembers,
    const std::vector<std::string>& excludedUserIds,
    const std::string& currentUserId)
{
    // Original Kotlin: try state event avatar first
    if (!roomAvatarUrl.empty()) return roomAvatarUrl;

    // Original Kotlin: only for direct rooms
    if (!isDirect) return "";

    // Filter: exclude specific users + current user
    std::vector<RoomMemberInfo> activeOthers;
    for (const auto& m : activeMembers) {
        bool excluded = false;
        for (const auto& id : excludedUserIds) {
            if (m.userId == id) { excluded = true; break; }
        }
        if (!excluded && m.userId != currentUserId) {
            activeOthers.push_back(m);
        }
    }

    // Original Kotlin: exactly 1 other active member
    if (activeOthers.size() == 1) {
        // Try left members first, then active
        for (const auto& m : leftMembers) {
            if (!m.avatarUrl.empty()) return m.avatarUrl;
        }
        return activeOthers[0].avatarUrl;
    }

    // Original Kotlin: exactly 2 — return the other one's avatar
    if (activeOthers.size() == 2) {
        for (const auto& m : activeOthers) {
            if (m.userId != currentUserId) return m.avatarUrl;
        }
    }

    return "";
}

// ==== Reaction Dedup Check ====
//
// Original Kotlin (DefaultRelationService.kt:59-73):
//   val targetTimelineEvent = timelineEventDataSource.getTimelineEvent(roomId, targetEventId)
//   if (targetTimelineEvent?.annotations?.reactionsSummary
//           .orEmpty().none { it.addedByMe && it.key == reaction }) {
//       // send reaction
//   } else {
//       // already added → NoOpCancellable
//   }

// Check if a reaction was already added by the current user.
// Returns true if the reaction IS a duplicate (should NOT be sent).
//
// reactionsSummary: list of {key, count, addedByMe} entries
// reaction: the emoji/key to check
inline bool isReactionDuplicate(
    const std::vector<std::pair<std::string, bool>>& reactionsSummary, // {key, addedByMe}
    const std::string& reaction)
{
    // Original Kotlin: .none { it.addedByMe && it.key == reaction }
    for (const auto& [key, addedByMe] : reactionsSummary) {
        if (addedByMe && key == reaction) return true;
    }
    return false;
}

// ==== TokenChunkEventPersistor — Direction-Aware Member Content ====
//
// Original Kotlin (TokenChunkEventPersistor.kt:153-185):
//   For backwards pagination: use prevContent for membership tracking
//   For forwards pagination: use content for membership tracking
//
// Direction-aware member content selection for room state events.
// When paginating BACKWARDS, we should use prev_content (state BEFORE the event).
// When paginating FORWARDS, we should use content (state AFTER the event).

enum class PaginationDirection { BACKWARDS = 0, FORWARDS = 1 };

inline std::string selectMemberContent(
    const std::string& content,           // "content" key from event
    const std::string& prevContent,        // "prev_content" key from event
    const std::string& eventType,
    PaginationDirection direction)
{
    // Original Kotlin: only for m.room.member events
    if (eventType != "m.room.member") return content;

    // Original Kotlin: for backwards, prefer prevContent
    if (direction == PaginationDirection::BACKWARDS && !prevContent.empty())
        return prevContent;

    return content;
}

} // namespace progressive
