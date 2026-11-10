#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cstdint>
#include <functional>
#include <utility>

namespace pcp {

// ==== Event Decryptor — Deduplication & Retry Logic ====
//
// Original Kotlin (TimelineEventDecryptor.kt:84-179):
//   - requestDecryption(): dedup, skip unknown-session failures
//   - processDecryptRequest(): decrypt or mark thread-aware
//   - newSessionListener: re-enqueue failed decryptions on new Olm session

enum class DecryptionPriority { NORMAL = 0, HIGH = 1, CRITICAL = 2 };

struct DecryptionQueueEntry {
    std::string eventId; std::string roomId;
    DecryptionPriority priority = DecryptionPriority::NORMAL;
    int64_t enqueueTimeMs = 0; int retryCount = 0; int attemptCount = 0;
};

class DecryptionQueue {
public:
    struct QueueCmp { bool operator()(const DecryptionQueueEntry& a, const DecryptionQueueEntry& b) const; };
    bool enqueue(const std::string& eventId, const std::string& roomId, DecryptionPriority priority = DecryptionPriority::NORMAL);
    DecryptionQueueEntry dequeue(); void requeue(const DecryptionQueueEntry& entry);
    void cancel(const std::string& eventId); size_t size() const; bool isEmpty() const;
    bool hasPendingForRoom(const std::string& roomId) const;
    void registerUnknownSessionFailure(const std::string& sessionId, const std::string& eventId);
    std::vector<std::string> onNewSessionAvailable(const std::string& sessionId);
    bool isUnknownSession(const std::string& sessionId) const;
    std::vector<std::string> getUnknownSessionIds() const; void clear();
private:
    std::vector<DecryptionQueueEntry> queue_;
    std::unordered_set<std::string> existingRequests_;
    std::unordered_map<std::string, std::vector<std::string>> unknownSessionsFailure_;
};

using SessionCheckFn = std::function<bool(const std::string& sessionId)>;
using DecryptFn = std::function<std::pair<std::string,std::string>(const std::string&, const std::string&)>;

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
bool shouldDecrypt(const DecryptorState& state, const DecryptionRequest& req);

// Original Kotlin: when a new Olm session arrives, re-enqueue previously failed events
// fun onNewSession(sessionId) { unknownSessionsFailure.remove(sessionId); re-enqueue events }
std::vector<DecryptionRequest> onNewSession(DecryptorState& state, const std::string& sessionId);

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

std::string resolveRoomAvatar(const std::string& roomAvatarUrl, bool isDirect, const std::vector<RoomMemberInfo>& activeMembers, const std::vector<RoomMemberInfo>& leftMembers, const std::vector<std::string>& excludedUserIds, const std::string& currentUserId);

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
bool isReactionDuplicate(const std::vector<std::pair<std::string, bool>>& reactionsSummary, const std::string& reaction);

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

std::string selectMemberContent(const std::string& content, const std::string& prevContent, const std::string& eventType, int direction);

} // namespace pcp
