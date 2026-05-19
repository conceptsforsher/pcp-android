#include "progressive/message_retry.hpp"
#include <sstream>
#include <algorithm>
#include <cmath>

namespace progressive {

int64_t computeRetryDelay(int retryCount, int64_t maxDelayMs) {
    // Exponential backoff: base 1s, double each retry
    // Retry 0: 1000ms, Retry 1: 2000ms, Retry 2: 4000ms, ...
    // Capped at maxDelayMs (default 5 minutes)
    // Original Kotlin:
    //   Math.min(baseDelay * (1 shl retryCount), maxDelay)
    int64_t delay = 1000LL * (1LL << std::min(retryCount, 10));
    if (delay > maxDelayMs) delay = maxDelayMs;
    return delay;
}

RetryDecision decideRetry(const PendingMessage& msg, int errorCode, const std::string& retryAfterHeader) {
    RetryDecision decision;

    // 429 Rate Limited — retry after specified time
    if (errorCode == 429) {
        decision.shouldRetry = true;
        if (!retryAfterHeader.empty()) {
            decision.delayMs = std::stoll(retryAfterHeader) * 1000;
        } else {
            decision.delayMs = computeRetryDelay(msg.retryCount);
        }
        decision.reason = "Rate limited (429)";
        return decision;
    }

    // 5xx Server Error — retry with backoff
    if (errorCode >= 500 && errorCode < 600) {
        // Don't retry forever — max 5 retries
        if (msg.retryCount >= 5) {
            decision.reason = "Too many server errors";
            return decision;
        }
        decision.shouldRetry = true;
        decision.delayMs = computeRetryDelay(msg.retryCount);
        decision.reason = "Server error (" + std::to_string(errorCode) + ")";
        return decision;
    }

    // Network/timeout errors (errorCode 0)
    if (errorCode == 0) {
        if (msg.retryCount >= 8) {
            decision.reason = "Too many network failures";
            return decision;
        }
        decision.shouldRetry = true;
        decision.delayMs = computeRetryDelay(msg.retryCount);
        decision.reason = "Network error";
        return decision;
    }

    // 4xx Client Error — don't retry (except 429 handled above)
    if (errorCode >= 400 && errorCode < 500) {
        decision.reason = "Client error (" + std::to_string(errorCode) + ")";
        return decision;
    }

    // Unknown — don't retry
    decision.reason = "Unknown error";
    return decision;
}

PendingMessage afterAttempt(PendingMessage msg, bool success, int errorCode, const std::string& error, int64_t nowMs) {
    msg.lastAttemptMs = nowMs;

    if (success) {
        msg.state = MessageSendState::Sent;
        return msg;
    }

    msg.error = error;
    msg.errorCode = errorCode;
    msg.retryCount++;

    auto decision = decideRetry(msg, errorCode);
    if (decision.shouldRetry) {
        msg.state = MessageSendState::Retrying;
    } else {
        msg.state = MessageSendState::Failed;
    }
    return msg;
}

bool isStaleMessage(const PendingMessage& msg, int64_t nowMs, int64_t maxAgeMs) {
    if (msg.state == MessageSendState::Sent || msg.state == MessageSendState::Cancelled) return false;
    return (nowMs - msg.queuedAtMs) > maxAgeMs;
}

std::vector<PendingMessage> cleanQueue(const std::vector<PendingMessage>& queue, int64_t nowMs) {
    std::vector<PendingMessage> cleaned;
    for (auto msg : queue) {
        if (msg.state == MessageSendState::Cancelled) continue;
        if (isStaleMessage(msg, nowMs)) {
            msg.state = MessageSendState::Failed;
            msg.error = "Message is too old to retry";
        }
        cleaned.push_back(msg);
    }
    return cleaned;
}

std::vector<PendingMessage> sortQueue(std::vector<PendingMessage> queue) {
    // Sort: pending/retrying first, then by queuedAtMs (oldest first),
    // within same timestamp, fewer retries first
    std::sort(queue.begin(), queue.end(), [](const PendingMessage& a, const PendingMessage& b) {
        // Active states first
        bool aActive = (a.state == MessageSendState::Pending || a.state == MessageSendState::Retrying);
        bool bActive = (b.state == MessageSendState::Pending || b.state == MessageSendState::Retrying);
        if (aActive != bActive) return aActive;

        // Older messages first
        if (a.queuedAtMs != b.queuedAtMs) return a.queuedAtMs < b.queuedAtMs;

        // Fewer retries first
        return a.retryCount < b.retryCount;
    });
    return queue;
}

PendingMessage getNextToSend(const std::vector<PendingMessage>& queue, int64_t nowMs) {
    PendingMessage empty;
    empty.state = MessageSendState::Failed;

    for (const auto& msg : queue) {
        if (msg.state == MessageSendState::Pending) return msg;

        if (msg.state == MessageSendState::Retrying) {
            // Check if enough time has passed since last attempt
            auto decision = decideRetry(msg, msg.errorCode);
            if (decision.shouldRetry && (nowMs - msg.lastAttemptMs) >= decision.delayMs) {
                return msg;
            }
        }
    }
    return empty;
}

std::string formatMessageStatus(MessageSendState state) {
    switch (state) {
        case MessageSendState::Pending: return "Sending...";
        case MessageSendState::Sending: return "Sending...";
        case MessageSendState::Sent: return "Sent";
        case MessageSendState::Failed: return "Failed to send";
        case MessageSendState::Retrying: return "Retrying...";
        case MessageSendState::Cancelled: return "Cancelled";
        default: return "Unknown";
    }
}

std::string formatRetryBadge(int retryCount) {
    if (retryCount <= 0) return "";
    if (retryCount == 1) return "1 retry";
    return std::to_string(retryCount) + " retries";
}

std::string pendingMessageToJson(const PendingMessage& msg) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };
    std::ostringstream json;
    json << R"({"localId": ")" << esc(msg.localId) << R"(",)";
    json << R"("roomId": ")" << esc(msg.roomId) << R"(",)";
    json << R"("body": ")" << esc(msg.body) << R"(",)";
    json << R"("msgType": ")" << esc(msg.msgType) << R"(",)";
    json << R"("queuedAtMs": )" << msg.queuedAtMs << ",";
    json << R"("retryCount": )" << msg.retryCount << ",";
    json << R"("state": )" << static_cast<int>(msg.state) << ",";
    json << R"("error": ")" << esc(msg.error) << R"(",)";
    json << R"("errorCode": )" << msg.errorCode << "}";
    return json.str();
}

std::string queueToJson(const std::vector<PendingMessage>& queue) {
    std::ostringstream json;
    json << "[";
    for (size_t i = 0; i < queue.size(); ++i) {
        if (i > 0) json << ",";
        json << pendingMessageToJson(queue[i]);
    }
    json << "]";
    return json.str();
}

// ==== Pending Message Editing ====
// Enables editing messages that haven't been sent yet.
// Original Element limitation: edits only work after server confirms send.
// Progressive Chat: edits update the pending queue immediately.
//
// How it works:
//   1. User types message, hits send — message enters queue (state=Pending)
//   2. User edits the message BEFORE it sends — body updated in queue
//   3. When queue processes this message, it sends the UPDATED body
//   4. If the original already started sending (state=Sending), an m.replace
//      edit event is queued to be sent after the original completes

bool canEditPendingMessage(const PendingMessage& msg) {
    // Can edit if it's still pending, sending, or retrying
    // Cannot edit if already sent, failed permanently, or cancelled
    return msg.state == MessageSendState::Pending ||
           msg.state == MessageSendState::Sending ||
           msg.state == MessageSendState::Retrying;
}

PendingMessage editPendingMessage(
    std::vector<PendingMessage>& queue,
    const std::string& localId,
    const std::string& newBody,
    int64_t nowMs)
{
    PendingMessage empty;
    empty.state = MessageSendState::Failed;

    for (auto& msg : queue) {
        if (msg.localId == localId) {
            if (!canEditPendingMessage(msg)) {
                empty.error = "Cannot edit: message is " + std::string(
                    msg.state == MessageSendState::Failed ? "failed" :
                    msg.state == MessageSendState::Cancelled ? "cancelled" :
                    "sent");
                return empty;
            }

            // Update the body
            std::string oldBody = msg.body;
            msg.body = newBody;

            // If already sending, the edit will need to be sent as a separate
            // m.replace event after the original completes.
            // For now, we update the queue body — the sender will use the updated text.
            bool wasSending = (msg.state == MessageSendState::Sending);

            EditResult result;
            result.success = true;
            result.wasPending = (msg.state == MessageSendState::Pending);
            result.wasSending = wasSending;

            // Mark that this message has been edited (so UI can update)
            // The Kotlin layer handles sending the m.replace relation

            return msg;
        }
    }

    empty.error = "Message not found in queue";
    return empty;
}

} // namespace progressive
