#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cstdint>
#include <functional>

namespace progressive {

// ==== Decryption Priority Enum ====
//
// Original Kotlin: implicit in executor ordering; explicit priority
// allows HIGH (new session events) before NORMAL (timeline) before LOW (room summary).

enum class DecryptionPriority : int {
    HIGH = 0,
    NORMAL = 1,
    LOW = 2
};

// ==== Decryption Queue Entry ====
//
// Original Kotlin (TimelineEventDecryptor.kt:176-179):
//   data class DecryptionRequest(event: Event, timelineId: String)

struct DecryptionQueueEntry {
    std::string eventId;
    std::string roomId;
    int64_t enqueueTimeMs = 0;         // epoch millis when enqueued
    int attemptCount = 0;               // number of decryption attempts
    DecryptionPriority priority = DecryptionPriority::NORMAL;
};

// ==== Decryption Queue ====
//
// Original Kotlin (TimelineEventDecryptor.kt:59-108):
//   - executor = Executors.newSingleThreadExecutor()
//   - existingRequests tracks pending decrypts
//   - unknownSessionsFailure tracks sessionId -> eventIds for retry
//
// Single-threaded queue for decryption requests with priority ordering
// and deduplication. Prevents duplicate decrypts and tracks unknown-session
// failures for automatic retry when new Olm sessions arrive.

class DecryptionQueue {
public:
    // Add an event to the decryption queue.
    // Returns false if the event is already queued or is in unknown-session failures.
    //
    // Original Kotlin (TimelineEventDecryptor.kt:84-108):
    //   fun requestDecryption(request): check existingRequests + unknownSessionsFailure
    bool enqueue(const std::string& eventId, const std::string& roomId,
                 DecryptionPriority priority = DecryptionPriority::NORMAL);

    // Get the next event to process, and optionally its priority.
    // Returns empty DecryptionQueueEntry on empty queue.
    DecryptionQueueEntry dequeue();

    // Put back an event (e.g., session not ready).
    // Increments attemptCount for backoff.
    //
    // Original Kotlin (TimelineEventDecryptor.kt:148-165):
    //   On MXCryptoError, add to unknownSessionsFailure for retry
    void requeue(const DecryptionQueueEntry& entry);

    // Cancel a specific event.
    void cancel(const std::string& eventId);

    // Number of queued events.
    size_t size() const;

    // True if queue is empty.
    bool isEmpty() const;

    // Check if there are pending events for a specific room.
    bool hasPendingForRoom(const std::string& roomId) const;

    // ==== Unknown Session Tracking ====
    //
    // Original Kotlin (TimelineEventDecryptor.kt:65):
    //   private val unknownSessionsFailure: MutableMap<String, MutableSet<DecryptionRequest>>

    // Register that a session failed with unknown session error.
    void registerUnknownSessionFailure(const std::string& sessionId, const std::string& eventId);

    // Called when a new Olm session becomes available.
    // Returns eventIds that should be retried.
    //
    // Original Kotlin (TimelineEventDecryptor.kt:44-57):
    //   fun onNewSession(sessionId) { re-enqueue failed events }
    std::vector<std::string> onNewSessionAvailable(const std::string& sessionId);

    // Check if a session ID is in the unknown failures list.
    bool isUnknownSession(const std::string& sessionId) const;

    // Get all tracked unknown session IDs.
    std::vector<std::string> getUnknownSessionIds() const;

    // Clear all state.
    void clear();

private:
    // Priority queue: entries ordered by (priority, enqueueTimeMs)
    struct QueueCmp {
        bool operator()(const DecryptionQueueEntry& a, const DecryptionQueueEntry& b) const;
    };

    std::vector<DecryptionQueueEntry> queue_;
    std::unordered_set<std::string> existingRequests_;               // currently pending
    std::unordered_map<std::string, std::unordered_set<std::string>> unknownSessionsFailure_; // sessionId -> eventIds
};

// ==== Decryption Request Processing ====
//
// Original Kotlin (TimelineEventDecryptor.kt:122-174):
//   fun processDecryptRequest(request, realm)

// Check if an Olm session exists for the given sessionId.
// Callback returns true if the session is available.
using SessionCheckFn = std::function<bool(const std::string& sessionId)>;

// Decrypt a message using the available Olm/Megolm session.
// Callback returns the clear payload JSON, or empty on failure.
// Also returns an error code string on failure (M_UNKNOWN_SESSION, M_BAD_MAC, etc.).
using DecryptFn = std::function<std::pair<std::string, std::string>(
    const std::string& eventId, const std::string& contentJson)>;

// Process queued decryption requests.
// For each event in the queue:
//   1. Check if Olm/Megolm session exists
//   2. If not -> requeue with backoff (delay grows with attemptCount)
//   3. If yes -> decrypt
//   4. On failure -> check if retryable (unknown session -> track), else fail permanent
//
// Original Kotlin (TimelineEventDecryptor.kt:122-174):
//   private fun processDecryptRequest(request, realm) {
//       if (!event.isEncrypted()) { threadAwareNonEncryptedEvents(); return }
//       try { result = cryptoService.decryptEvent(event, timelineId) }
//       catch (e: MXCryptoError) { track unknown session; retry later }
//   }
//
// Returns the number of events processed.
int processDecryptionRequests(DecryptionQueue& queue,
                              SessionCheckFn sessionCheck,
                              DecryptFn decrypt,
                              int maxAttempts = 3,
                              int64_t backoffBaseMs = 1000);

// ==== Retryable Error Detection ====
//
// Original Kotlin (TimelineEventDecryptor.kt:148-165):
//   if (e.errorType == UNKNOWN_INBOUND_SESSION_ID || e.errorType == UNKNOWN_MESSAGE_INDEX)
//       -> retry (track session)
//   else -> permanent failure

// Check if a decryption error is retryable.
// M_UNKNOWN_SESSION, UNKNOWN_INBOUND_SESSION_ID, UNKNOWN_MESSAGE_INDEX -> true
// M_BAD_MAC, M_INVALID_CIPHERTEXT, etc. -> false
bool isRetryableDecryptionError(const std::string& errorCode);

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
    std::unordered_map<std::string, std::vector<std::string>> sessionToEvents; // sessionId -> eventIds
};

// Original Kotlin: deduplication check before enqueueing
// Declared in header, implemented in cpp.
bool shouldDecrypt(const DecryptorState& state, const DecryptionRequest& req);

// Original Kotlin: when a new Olm session arrives, re-enqueue previously failed events
// Declared in header, implemented in cpp.
std::vector<DecryptionRequest> onNewSession(DecryptorState& state, const std::string& sessionId);

// ==== Room Avatar Resolver ====
//
// Original Kotlin (RoomAvatarResolver.kt:48-84):
//   fun resolve(): String?

struct RoomMemberInfo {
    std::string userId;
    std::string displayName;
    std::string avatarUrl;
    bool isActive = true;               // membership = JOIN or INVITE
    bool isLeft = false;               // membership = LEAVE, BAN, KNOCK
};

// Resolve room avatar from state event + member list.
// Declared in header, implemented in cpp.
std::string resolveRoomAvatar(
    const std::string& roomAvatarUrl,
    bool isDirect,
    const std::vector<RoomMemberInfo>& activeMembers,
    const std::vector<RoomMemberInfo>& leftMembers,
    const std::vector<std::string>& excludedUserIds,
    const std::string& currentUserId);

// ==== Reaction Dedup Check ====
//
// Original Kotlin (DefaultRelationService.kt:59-73):
//   if (!annotations.reactionsSummary.none { it.addedByMe && it.key == reaction }) { send }

// Check if a reaction was already added by the current user.
// Returns true if the reaction IS a duplicate (should NOT be sent).
// Declared in header, implemented in cpp.
bool isReactionDuplicate(
    const std::vector<std::pair<std::string, bool>>& reactionsSummary,
    const std::string& reaction);

// ==== TokenChunkEventPersistor — Direction-Aware Member Content ====
//
// Original Kotlin (TokenChunkEventPersistor.kt:153-185):
//   For backwards pagination: use prevContent for membership tracking
//   For forwards pagination: use content for membership tracking

enum class PaginationDirection { BACKWARDS = 0, FORWARDS = 1 };

// Declared in header, implemented in cpp.
std::string selectMemberContent(
    const std::string& content,
    const std::string& prevContent,
    const std::string& eventType,
    PaginationDirection direction);

} // namespace progressive
