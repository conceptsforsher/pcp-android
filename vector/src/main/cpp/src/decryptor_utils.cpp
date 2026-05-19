#include "progressive/decryptor_utils.hpp"
#include <algorithm>
#include <chrono>

namespace progressive {

// ============================================================================
// DecryptionQueue
// ============================================================================

// Original Kotlin (TimelineEventDecryptor.kt:59-108):
//   executor = Executors.newSingleThreadExecutor()
//   existingRequests tracks pending, unknownSessionsFailure tracks retryable failures

bool DecryptionQueue::QueueCmp::operator()(const DecryptionQueueEntry& a, const DecryptionQueueEntry& b) const {
    if (a.priority != b.priority)
        return static_cast<int>(a.priority) < static_cast<int>(b.priority);
    return a.enqueueTimeMs < b.enqueueTimeMs;
}

bool DecryptionQueue::enqueue(const std::string& eventId, const std::string& roomId,
                               DecryptionPriority priority) {
    // Original Kotlin: if already in unknownSessionsFailure, skip
    for (const auto& [sid, events] : unknownSessionsFailure_) {
        if (events.count(eventId)) return false;
    }
    // Original Kotlin: if already in existingRequests, skip
    if (existingRequests_.count(eventId)) return false;

    existingRequests_.insert(eventId);

    DecryptionQueueEntry entry;
    entry.eventId = eventId;
    entry.roomId = roomId;
    entry.priority = priority;
    entry.enqueueTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    entry.attemptCount = 0;

    queue_.push_back(entry);
    std::push_heap(queue_.begin(), queue_.end(), QueueCmp{});
    return true;
}

DecryptionQueueEntry DecryptionQueue::dequeue() {
    if (queue_.empty()) return {};

    std::pop_heap(queue_.begin(), queue_.end(), QueueCmp{});
    auto entry = queue_.back();
    queue_.pop_back();
    existingRequests_.erase(entry.eventId);
    return entry;
}

void DecryptionQueue::requeue(const DecryptionQueueEntry& entry) {
    DecryptionQueueEntry req = entry;
    req.attemptCount++;
    req.enqueueTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();

    existingRequests_.insert(req.eventId);
    queue_.push_back(req);
    std::push_heap(queue_.begin(), queue_.end(), QueueCmp{});
}

void DecryptionQueue::cancel(const std::string& eventId) {
    // Remove from existingRequests
    existingRequests_.erase(eventId);

    // Remove from queue
    auto it = std::find_if(queue_.begin(), queue_.end(), [&](const auto& e) {
        return e.eventId == eventId;
    });
    if (it != queue_.end()) {
        queue_.erase(it);
        std::make_heap(queue_.begin(), queue_.end(), QueueCmp{});
    }

    // Remove from unknownSessionsFailure
    for (auto& [sid, events] : unknownSessionsFailure_) {
        events.erase(eventId);
    }
}

size_t DecryptionQueue::size() const {
    return queue_.size();
}

bool DecryptionQueue::isEmpty() const {
    return queue_.empty();
}

bool DecryptionQueue::hasPendingForRoom(const std::string& roomId) const {
    for (const auto& e : queue_) {
        if (e.roomId == roomId) return true;
    }
    return false;
}

void DecryptionQueue::registerUnknownSessionFailure(const std::string& sessionId, const std::string& eventId) {
    // Original Kotlin (TimelineEventDecryptor.kt:160-164):
    //   unknownSessionsFailure.getOrPut(sessionId) { mutableSetOf() }.add(request)
    unknownSessionsFailure_[sessionId].insert(eventId);
}

std::vector<std::string> DecryptionQueue::onNewSessionAvailable(const std::string& sessionId) {
    // Original Kotlin (TimelineEventDecryptor.kt:44-57):
    //   unknownSessionsFailure[sessionId]?.toList()?.also { clear }?.forEach { requestDecryption }
    std::vector<std::string> retryEventIds;
    auto it = unknownSessionsFailure_.find(sessionId);
    if (it != unknownSessionsFailure_.end()) {
        retryEventIds.assign(it->second.begin(), it->second.end());
        unknownSessionsFailure_.erase(it);
    }
    return retryEventIds;
}

bool DecryptionQueue::isUnknownSession(const std::string& sessionId) const {
    return unknownSessionsFailure_.count(sessionId) > 0;
}

std::vector<std::string> DecryptionQueue::getUnknownSessionIds() const {
    std::vector<std::string> ids;
    for (const auto& [sid, _] : unknownSessionsFailure_) {
        ids.push_back(sid);
    }
    return ids;
}

void DecryptionQueue::clear() {
    queue_.clear();
    existingRequests_.clear();
    unknownSessionsFailure_.clear();
}

// ============================================================================
// Decryption Request Processing
// ============================================================================

// Original Kotlin (TimelineEventDecryptor.kt:122-174):
//   fun processDecryptRequest(request, realm):
//     if !isEncrypted: threadAware
//     try: cryptoService.decryptEvent
//     catch MXCryptoError: track unknown session

bool isRetryableDecryptionError(const std::string& errorCode) {
    // Original Kotlin (TimelineEventDecryptor.kt:148-165):
    //   if (e.errorType == UNKNOWN_INBOUND_SESSION_ID) { track for retry }
    // Also from RoomSummaryEventDecryptor.kt:119-124:
    //   if (failure.errorType == UNKNOWN_INBOUND_SESSION_ID ||
    //       failure.errorType == UNKNOWN_MESSAGE_INDEX)
    return errorCode == "M_UNKNOWN_SESSION"
        || errorCode == "UNKNOWN_INBOUND_SESSION_ID"
        || errorCode == "UNKNOWN_MESSAGE_INDEX";
}

int processDecryptionRequests(DecryptionQueue& queue,
                              SessionCheckFn sessionCheck,
                              DecryptFn decrypt,
                              int maxAttempts,
                              int64_t backoffBaseMs) {
    int processed = 0;

    while (!queue.isEmpty()) {
        auto entry = queue.dequeue();

        // Original Kotlin: skip if too many attempts
        if (entry.attemptCount >= maxAttempts) {
            // Permanent failure — event stays failed
            processed++;
            continue;
        }

        // Calculate backoff delay: base * 2^attemptCount (exponential backoff)
        int64_t now = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
        int64_t backoffDelay = backoffBaseMs * (1LL << entry.attemptCount);
        int64_t nextRetryTime = entry.enqueueTimeMs + backoffDelay;
        if (now < nextRetryTime) {
            // Not ready yet, put back
            queue.requeue(entry);
            break; // stop processing — wait for backoff
        }

        // Attempt decryption
        auto [clearPayload, errorCode] = decrypt(entry.eventId, entry.eventId /* contentJson */);

        if (!clearPayload.empty()) {
            // Success
            processed++;
            continue;
        }

        // Failure — check if retryable
        if (isRetryableDecryptionError(errorCode)) {
            // Track unknown session and requeue
            // sessionId would come from the encrypted event content
            queue.requeue(entry);
        }
        // else: permanent failure — don't requeue

        processed++;
    }

    return processed;
}

// ============================================================================
// DecryptorState helpers
// ============================================================================

bool shouldDecrypt(const DecryptorState& state, const DecryptionRequest& req) {
    if (state.existingRequests.count(req.eventId)) return false;
    // Original Kotlin (TimelineEventDecryptor.kt:84-94):
    //   Check if session is known to fail (unknownSessionsFailure check)
    return true;
}

std::vector<DecryptionRequest> onNewSession(DecryptorState& state, const std::string& sessionId) {
    // Original Kotlin (TimelineEventDecryptor.kt:44-57):
    //   NewSessionListener.onNewSession(sessionId):
    //     unknownSessionsFailure[sessionId]?.toList()?.also { clear }?.forEach { requestDecryption }
    state.unknownSessionsFailure.erase(sessionId);
    std::vector<DecryptionRequest> retry;
    auto it = state.sessionToEvents.find(sessionId);
    if (it != state.sessionToEvents.end()) {
        for (const auto& eventId : it->second) {
            state.existingRequests.insert(eventId);
            DecryptionRequest req;
            req.eventId = eventId;
            retry.push_back(req);
        }
        state.sessionToEvents.erase(it);
    }
    return retry;
}

// ============================================================================
// Room Avatar Resolver
// ============================================================================

// Original Kotlin (RoomAvatarResolver.kt:48-84):
//   1. Try m.room.avatar state event → return avatarUrl if non-empty
//   2. For direct rooms: get active members excluding certain users
//   3. Return null
std::string resolveRoomAvatar(
    const std::string& roomAvatarUrl,
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

// ============================================================================
// Reaction Dedup Check
// ============================================================================

// Original Kotlin (DefaultRelationService.kt:59-73):
//   .none { it.addedByMe && it.key == reaction }
bool isReactionDuplicate(
    const std::vector<std::pair<std::string, bool>>& reactionsSummary,
    const std::string& reaction)
{
    for (const auto& [key, addedByMe] : reactionsSummary) {
        if (addedByMe && key == reaction) return true;
    }
    return false;
}

// ============================================================================
// Direction-Aware Member Content
// ============================================================================

// Original Kotlin (TokenChunkEventPersistor.kt:153-185):
//   For backwards pagination: use prevContent for membership tracking
//   For forwards pagination: use content for membership tracking
std::string selectMemberContent(
    const std::string& content,
    const std::string& prevContent,
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
