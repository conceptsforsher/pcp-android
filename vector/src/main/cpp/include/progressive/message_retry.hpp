#ifndef PROGRESSIVE_MESSAGE_RETRY_HPP
#define PROGRESSIVE_MESSAGE_RETRY_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// ---- Message Retry Queue ----
// Ported from: org.matrix.android.sdk.internal.session.room.send.QueueMediator.kt
//              org.matrix.android.sdk.internal.session.room.send.RetryDecider.kt
//              im.vector.app.features.home.room.detail.timeline.factory.MessageItemFactory.kt

enum class MessageSendState {
    Pending,         // queued, not yet sent
    Sending,         // currently being sent
    Sent,            // successfully sent
    Failed,          // failed (permanent error)
    Retrying,        // retrying after temporary failure
    Cancelled        // user cancelled
};

struct PendingMessage {
    std::string localId;          // local event ID (txnId)
    std::string roomId;
    std::string body;             // message content
    std::string msgType;          // "m.text", "m.image", "m.file", etc.
    int64_t queuedAtMs = 0;       // when was it queued
    int64_t lastAttemptMs = 0;    // last send attempt timestamp
    int retryCount = 0;           // number of retries so far
    MessageSendState state = MessageSendState::Pending;
    std::string error;            // last error message
    int errorCode = 0;            // HTTP status code (0 if unknown)
};

struct RetryDecision {
    bool shouldRetry = false;
    int64_t delayMs = 0;          // how long to wait before retrying
    std::string reason;           // why this decision was made
};

// Compute the retry delay using exponential backoff.
// Base delay 1s, doubles each retry, capped at 5 minutes.
// Original Kotlin (RetryDecider.kt):
//   fun computeRetryDelay(retryCount: Int, maxDelayMs: Long): Long
int64_t computeRetryDelay(int retryCount, int64_t maxDelayMs = 300000);

// Decide whether to retry sending a message based on error code.
// 429 Rate Limited → retry after Retry-After header
// 5xx Server Error → retry with backoff
// 4xx Client Error → don't retry (except 429)
// Network error → retry with backoff
RetryDecision decideRetry(const PendingMessage& msg, int errorCode, const std::string& retryAfterHeader = "");

// Update the message state after a send attempt.
PendingMessage afterAttempt(PendingMessage msg, bool success, int errorCode, const std::string& error, int64_t nowMs);

// Check if a message has been pending too long (stale).
bool isStaleMessage(const PendingMessage& msg, int64_t nowMs, int64_t maxAgeMs = 600000);

// Filter the retry queue: remove cancelled, mark stale as failed.
std::vector<PendingMessage> cleanQueue(const std::vector<PendingMessage>& queue, int64_t nowMs);

// Sort queue: oldest first, then by retry count (fewer retries first).
std::vector<PendingMessage> sortQueue(std::vector<PendingMessage> queue);

// Get the next message that should be sent (state == Pending or Retrying).
// Returns the one with the oldest queuedAtMs that has waited long enough.
PendingMessage getNextToSend(const std::vector<PendingMessage>& queue, int64_t nowMs);

// Format message state as JSON for the Kotlin UI layer.
std::string pendingMessageToJson(const PendingMessage& msg);

// Format the full queue as JSON array.
std::string queueToJson(const std::vector<PendingMessage>& queue);

// Get a human-readable status for a message.
std::string formatMessageStatus(MessageSendState state);

// Get a display text for the retry count badge.
std::string formatRetryBadge(int retryCount);

// ---- Pending Message Editing ----
// Allow editing messages that are still pending/sending.
// Original Element behavior: edit only after server confirms send.
// Progressive Chat: edit immediately, apply to pending queue.
// The edit is sent as an m.replace relation pointing to the pending event.

struct EditResult {
    bool success = false;
    std::string error;
    bool wasPending = false;       // message was still pending when edited
    bool wasSending = false;       // message was being sent when edited
};

// Edit a message that may still be pending/sending.
// Updates the body in the queue. If the message hasn't been sent yet,
// the edited body will be sent when the queue retries.
// If it's in progress, the edit event will be sent AFTER the original.
PendingMessage editPendingMessage(std::vector<PendingMessage>& queue, const std::string& localId, const std::string& newBody, int64_t nowMs);

// Check if a pending message CAN be edited (not failed/cancelled).
bool canEditPendingMessage(const PendingMessage& msg);

} // namespace progressive

#endif // PROGRESSIVE_MESSAGE_RETRY_HPP
