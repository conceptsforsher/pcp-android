#include "progressive/thread_manager.hpp"
#include <sstream>
#include <algorithm>
#include <ctime>

namespace progressive {

// ====== JSON helpers ======

std::string ThreadManager::extractStr(const std::string& json, const std::string& key) {
    auto pp = json.find("\"" + key + "\"");
    if (pp == std::string::npos) return "";
    pp = json.find('"', pp + key.size() + 2);
    if (pp == std::string::npos) return "";
    pp++;
    size_t e = pp;
    while (e < json.size() && json[e] != '"') e++;
    return json.substr(pp, e - pp);
}

int64_t ThreadManager::extractInt(const std::string& json, const std::string& key) {
    auto pp = json.find("\"" + key + "\"");
    if (pp == std::string::npos) return 0;
    pp = json.find(':', pp);
    if (pp == std::string::npos) return 0;
    pp++;
    while (pp < json.size() && (json[pp] == ' ' || json[pp] == '\t')) pp++;
    int64_t v = 0;
    bool neg = false;
    if (pp < json.size() && json[pp] == '-') { neg = true; pp++; }
    while (pp < json.size() && json[pp] >= '0' && json[pp] <= '9') { v = v * 10 + (json[pp] - '0'); pp++; }
    return neg ? -v : v;
}

// Extract a JSON object substring
// Original Kotlin: Moshi deserialization helpers
std::string ThreadManager::extractObj(const std::string& json, const std::string& key) {
    auto pp = json.find("\"" + key + "\"");
    if (pp == std::string::npos) return "";
    pp = json.find(':', pp);
    if (pp == std::string::npos) return "";
    pp++;
    while (pp < json.size() && (json[pp] == ' ' || json[pp] == '\t' || json[pp] == '\n' || json[pp] == '\r')) pp++;
    if (pp >= json.size() || json[pp] != '{') return "";
    int depth = 1;
    size_t start = pp;
    pp++;
    while (pp < json.size() && depth > 0) {
        if (json[pp] == '{') depth++;
        else if (json[pp] == '}') depth--;
        pp++;
    }
    return json.substr(start, pp - start);
}

// Extract a JSON array substring
std::string ThreadManager::extractArray(const std::string& json, const std::string& key) {
    auto pp = json.find("\"" + key + "\"");
    if (pp == std::string::npos) return "";
    pp = json.find(':', pp);
    if (pp == std::string::npos) return "";
    pp++;
    while (pp < json.size() && (json[pp] == ' ' || json[pp] == '\t' || json[pp] == '\n' || json[pp] == '\r')) pp++;
    if (pp >= json.size() || json[pp] != '[') return "";
    int depth = 1;
    size_t start = pp;
    pp++;
    while (pp < json.size() && depth > 0) {
        if (json[pp] == '[') depth++;
        else if (json[pp] == ']') depth--;
        pp++;
    }
    return json.substr(start, pp - start);
}

// ====== Constructor ======

ThreadManager::ThreadManager() {}

// ====== Thread Root Detection ======

bool ThreadManager::isThreadRoot(const std::string& eventContentJson, const std::string& eventId) {
    // Original Kotlin: Check for m.relates_to with rel_type = "m.thread" and event_id = this event
    auto relType = extractStr(eventContentJson, "rel_type");
    if (relType != "m.thread") return false;

    auto threadRoot = extractStr(eventContentJson, "event_id");
    if (threadRoot == eventId) return true;

    // Also check nested m.relates_to structure
    size_t pos = eventContentJson.find("\"m.relates_to\"");
    if (pos == std::string::npos) return false;

    // Extract the relates_to object
    pos = eventContentJson.find('{', pos);
    if (pos == std::string::npos) return false;
    int depth = 1;
    size_t start = pos;
    pos++;
    while (pos < eventContentJson.size() && depth > 0) {
        if (eventContentJson[pos] == '{') depth++;
        else if (eventContentJson[pos] == '}') depth--;
        pos++;
    }
    std::string relatesTo = eventContentJson.substr(start, pos - start);

    auto relType2 = extractStr(relatesTo, "rel_type");
    if (relType2 != "m.thread") return false;

    auto eventId2 = extractStr(relatesTo, "event_id");
    return eventId2 == eventId;
}

std::string ThreadManager::getThreadRootId(const std::string& eventContentJson) {
    // Original Kotlin: Check m.relates_to for thread relation
    auto threadRoot = extractThreadRoot(eventContentJson);
    if (!threadRoot.empty() && threadRoot != extractStr(eventContentJson, "event_id")) {
        return threadRoot; // This is a reply, not the root
    }

    return ""; // Not a thread reply
}

std::string ThreadManager::extractThreadRoot(const std::string& eventContentJson) {
    // Extract thread root from m.relates_to
    // Look for "rel_type":"m.thread" pattern
    size_t pos = eventContentJson.find("\"rel_type\":\"m.thread\"");
    if (pos == std::string::npos) {
        pos = eventContentJson.find("\"rel_type\": \"m.thread\"");
    }
    if (pos == std::string::npos) return "";

    // Find the enclosing object
    auto objStart = eventContentJson.rfind('{', pos);
    if (objStart == std::string::npos) return "";

    int depth = 0;
    auto objEnd = objStart;
    while (objEnd < eventContentJson.size()) {
        if (eventContentJson[objEnd] == '{') depth++;
        else if (eventContentJson[objEnd] == '}') depth--;
        if (depth == 0 && objEnd > objStart) break;
        objEnd++;
    }

    std::string relatesToObj = eventContentJson.substr(objStart, objEnd - objStart + 1);
    auto eventId = extractStr(relatesToObj, "event_id");

    return eventId;
}

// ====== Thread Management ======

void ThreadManager::upsertThread(const ThreadInfoFull& thread) {
    auto it = threads_.find(thread.threadId);
    if (it != threads_.end()) {
        // Update existing
        it->second.rootSenderName = thread.rootSenderName;
        it->second.rootBody = thread.rootBody;
        if (!thread.rootEventType.empty()) it->second.rootEventType = thread.rootEventType;
        if (thread.rootTimestampMs > 0) it->second.rootTimestampMs = thread.rootTimestampMs;
        if (!thread.lastReplySenderId.empty()) it->second.lastReplySenderId = thread.lastReplySenderId;
        if (thread.lastReplyTimestampMs > 0) it->second.lastReplyTimestampMs = thread.lastReplyTimestampMs;
    } else {
        threads_[thread.threadId] = thread;
    }

    // Initialize unread state if new
    if (unread_.find(thread.threadId) == unread_.end()) {
        ThreadUnreadState us;
        us.threadId = thread.threadId;
        unread_[thread.threadId] = us;
    }
}

void ThreadManager::addReply(const std::string& threadId, const std::string& senderId,
                              const std::string& senderName, const std::string& body,
                              int64_t timestampMs) {
    auto it = threads_.find(threadId);
    if (it == threads_.end()) return;

    it->second.replyCount++;
    it->second.lastReplyTimestampMs = timestampMs;
    it->second.lastReplySenderId = senderId;
    it->second.lastReplySenderName = senderName;
    it->second.lastReplyBody = body;

    // Add participant
    if (!senderId.empty() && senderId != it->second.rootSenderId) {
        it->second.participantIds.insert(senderId);
    }

    // Increment unread count
    auto uit = unread_.find(threadId);
    if (uit != unread_.end()) {
        uit->second.unreadCount++;
    }
}

void ThreadManager::removeThread(const std::string& threadId) {
    threads_.erase(threadId);
    unread_.erase(threadId);
    readReceipts_.erase(threadId);
    summaries_.erase(threadId);
}

void ThreadManager::clearRoom(const std::string& roomId) {
    for (auto it = threads_.begin(); it != threads_.end(); ) {
        if (it->second.roomId == roomId) {
            unread_.erase(it->first);
            readReceipts_.erase(it->first);
            summaries_.erase(it->first);
            it = threads_.erase(it);
        } else {
            ++it;
        }
    }
}

// ====== Thread Queries ======

bool ThreadManager::getThread(const std::string& threadId, ThreadInfoFull& out) const {
    auto it = threads_.find(threadId);
    if (it == threads_.end()) return false;
    out = it->second;
    return true;
}

std::vector<ThreadInfoFull> ThreadManager::getRoomThreads(const std::string& roomId) const {
    std::vector<ThreadInfoFull> result;
    for (const auto& [id, th] : threads_) {
        if (th.roomId == roomId) result.push_back(th);
    }
    // Add unread state
    for (auto& t : result) {
        auto it = unread_.find(t.threadId);
        if (it != unread_.end()) {
            t.isUnread = it->second.unreadCount > 0;
            t.hasHighlight = it->second.highlighted;
            t.unreadCount = it->second.unreadCount;
        }
    }
    sortThreads(result);
    return result;
}

ThreadList ThreadManager::getThreadList(int limit, int offset) const {
    ThreadList list;
    std::vector<ThreadInfoFull> all;
    for (const auto& [id, th] : threads_) {
        auto t = th;
        auto it = unread_.find(id);
        if (it != unread_.end()) {
            t.isUnread = it->second.unreadCount > 0;
            t.hasHighlight = it->second.highlighted;
            t.unreadCount = it->second.unreadCount;
        }
        all.push_back(t);
    }

    sortThreads(all);

    list.totalCount = static_cast<int>(all.size());

    // Count unread/highlighted
    for (const auto& t : all) {
        if (t.isUnread) list.unreadCount++;
        if (t.hasHighlight) list.highlightedCount++;
    }

    // Apply pagination
    if (offset > 0 && offset < static_cast<int>(all.size())) {
        all.erase(all.begin(), all.begin() + offset);
    }
    if (limit > 0 && limit < static_cast<int>(all.size())) {
        all.resize(limit);
        list.hasMore = true;
    }

    list.threads = all;
    return list;
}

int ThreadManager::getRoomThreadCount(const std::string& roomId) const {
    int count = 0;
    for (const auto& [id, th] : threads_) {
        if (th.roomId == roomId) count++;
    }
    return count;
}

// ====== Unread State ======

void ThreadManager::setThreadUnread(const std::string& threadId, int unreadCount, bool highlighted) {
    auto it = unread_.find(threadId);
    if (it == unread_.end()) {
        ThreadUnreadState us;
        us.threadId = threadId;
        us.unreadCount = unreadCount;
        us.highlighted = highlighted;
        unread_[threadId] = us;
    } else {
        it->second.unreadCount = unreadCount;
        it->second.highlighted = highlighted;
    }
}

void ThreadManager::markThreadRead(const std::string& threadId, int64_t readPos) {
    readReceipts_[threadId] = readPos;
    auto it = unread_.find(threadId);
    if (it != unread_.end()) {
        it->second.unreadCount = 0;
        it->second.highlighted = false;
        it->second.readReceiptPosition = readPos;
    }
}

ThreadUnreadState ThreadManager::getUnreadState(const std::string& threadId) const {
    auto it = unread_.find(threadId);
    if (it != unread_.end()) return it->second;
    ThreadUnreadState empty;
    empty.threadId = threadId;
    return empty;
}

int ThreadManager::getTotalUnreadCount() const {
    int total = 0;
    for (const auto& [id, us] : unread_) {
        total += us.unreadCount;
    }
    return total;
}

// ====== Notifications ======

std::vector<ThreadNotification> ThreadManager::getNotifications() const {
    std::vector<ThreadNotification> notifs;
    for (const auto& [id, us] : unread_) {
        if (us.unreadCount == 0 && !us.highlighted) continue;

        auto it = threads_.find(id);
        if (it == threads_.end()) continue;

        ThreadNotification tn;
        tn.threadId = id;
        tn.roomId = it->second.roomId;
        tn.unreadCount = us.unreadCount;
        tn.highlighted = us.highlighted;
        tn.timestampMs = it->second.rootTimestampMs;

        // Title: "Sender: first line of body"
        std::string sender = it->second.rootSenderName.empty() ? it->second.rootSenderId : it->second.rootSenderName;
        tn.title = sender + ": " + it->second.rootBody.substr(0, 50);

        notifs.push_back(tn);
    }
    return notifs;
}

std::string ThreadManager::formatThreadNotificationCount(int count) {
    if (count <= 0) return "";
    if (count <= 99) return std::to_string(count);
    return "99+";
}

// ====== Sorting ======

void ThreadManager::sortThreads(std::vector<ThreadInfoFull>& threads) const {
    std::sort(threads.begin(), threads.end(), [](const ThreadInfoFull& a, const ThreadInfoFull& b) {
        // Unread first
        if (a.isUnread != b.isUnread) return a.isUnread;
        // Highlighted next
        if (a.hasHighlight != b.hasHighlight) return a.hasHighlight;
        // Then by latest activity (descending)
        int64_t aLast = std::max(a.rootTimestampMs, a.lastReplyTimestampMs);
        int64_t bLast = std::max(b.rootTimestampMs, b.lastReplyTimestampMs);
        return aLast > bLast;
    });
}

// ====== Serialization ======

std::string ThreadManager::threadToJson(const ThreadInfoFull& thread) const {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    std::ostringstream os;
    os << R"({"thread_id":")" << esc(thread.threadId)
       << R"(","room_id":")" << esc(thread.roomId)
       << R"(","root_sender_id":")" << esc(thread.rootSenderId)
       << R"(","root_sender_name":")" << esc(thread.rootSenderName)
       << R"(","root_body":")" << esc(thread.rootBody.substr(0, 100))
       << R"(","reply_count":)" << thread.replyCount
       << R"(,"last_reply_ts":)" << thread.lastReplyTimestampMs
       << R"(,"last_reply_sender":")" << esc(thread.lastReplySenderName)
       << R"(,"is_unread":)" << (thread.isUnread ? "true" : "false")
       << R"(,"has_highlight":)" << (thread.hasHighlight ? "true" : "false")
       << R"(,"unread_count":)" << thread.unreadCount
       << R"(,"participants":)" << static_cast<int>(thread.participantIds.size())
       << R"(,"root_encrypted":)" << (thread.rootIsEncrypted ? "true" : "false")
       << "}";
    return os.str();
}

std::string ThreadManager::threadListToJson(const ThreadList& list) const {
    std::ostringstream os;
    os << R"({"threads":[)";
    for (size_t i = 0; i < list.threads.size(); i++) {
        if (i > 0) os << ",";
        os << threadToJson(list.threads[i]);
    }
    os << R"(],"total_count":)" << list.totalCount
       << R"(,"unread_count":)" << list.unreadCount
       << R"(,"highlighted_count":)" << list.highlightedCount
       << R"(,"has_more":)" << (list.hasMore ? "true" : "false")
       << "}";
    return os.str();
}

std::string ThreadManager::unreadStateToJson(const ThreadUnreadState& state) const {
    std::ostringstream os;
    os << R"({"thread_id":")" << state.threadId
       << R"(","unread_count":)" << state.unreadCount
       << R"(,"highlighted":)" << (state.highlighted ? "true" : "false")
       << R"(,"read_receipt_pos":)" << state.readReceiptPosition
       << "}";
    return os.str();
}

std::string ThreadManager::notificationToJson(const ThreadNotification& notif) const {
    std::ostringstream os;
    os << R"({"thread_id":")" << notif.threadId
       << R"(","room_id":")" << notif.roomId
       << R"(","title":")" << notif.title
       << R"(","unread_count":)" << notif.unreadCount
       << R"(,"highlighted":)" << (notif.highlighted ? "true" : "false")
       << R"(,"timestamp":)" << notif.timestampMs
       << "}";
    return os.str();
}

// ========================================================================
// NEW: Thread Summary Support
// Original Kotlin: ThreadSummaryHelper.kt
// ========================================================================

ThreadSummary ThreadManager::computeAndStoreSummary(const std::string& roomId,
                                                     const std::string& rootEventJson,
                                                     const std::string& latestEventJson,
                                                     bool isUserParticipating,
                                                     const std::string& userId) {
    ThreadSummary summary = computeThreadSummary(roomId, rootEventJson, latestEventJson,
                                                  isUserParticipating, userId);

    // Store in internal map
    if (!summary.rootEventId.empty()) {
        summaries_[summary.rootEventId] = summary;

        // Also sync to legacy ThreadInfoFull storage for backward compat
        ThreadInfoFull info = threadSummaryToInfoFull(summary);
        upsertThread(info);
    }

    return summary;
}

void ThreadManager::updateStoredSummary(const std::string& threadId,
                                         const std::string& eventJson,
                                         ThreadSummaryUpdateType type,
                                         const std::string& userId) {
    // Original Kotlin: ThreadSummaryHelper.createOrUpdate with ADD mode
    auto it = summaries_.find(threadId);

    if (it != summaries_.end() && type == ThreadSummaryUpdateType::ADD) {
        // Original Kotlin: ThreadSummary ADD exists case
        // Update latest event and increment count
        std::string latestEventId = extractStr(eventJson, "event_id");
        if (!latestEventId.empty()) {
            it->second.latestEventId = latestEventId;
            it->second.latestEventJson = eventJson;
        }
        it->second.numberOfThreads++;

        std::string senderId = extractStr(eventJson, "sender");
        if (senderId == userId) {
            it->second.isUserParticipating = true;
        }

        // Update latest sender info
        it->second.latestThreadSenderInfo.senderId = senderId;
        it->second.latestThreadSenderInfo.senderName = extractStr(eventJson, "displayname");

        // Update timestamp
        int64_t ts = extractInt(eventJson, "origin_server_ts");
        if (ts > 0) it->second.latestTimestampMs = ts;

        // Update body
        std::string body = extractStr(eventJson, "body");
        if (!body.empty()) it->second.latestBody = body;
    } else if (it != summaries_.end() && type == ThreadSummaryUpdateType::REPLACE) {
        // Full replacement — recompute from root event
        // Original Kotlin: ThreadSummaryHelper.createOrUpdate REPLACE mode
        updateThreadSummary(it->second, eventJson, type, userId);
    } else {
        // Create new summary (ADD mode on non-existent thread)
        std::string rootEventId = extractThreadRootId(eventJson);
        if (rootEventId.empty()) {
            rootEventId = threadId; // Fallback to provided threadId
        }

        ThreadSummary newSummary;
        newSummary.roomId = extractStr(eventJson, "room_id");
        newSummary.rootEventId = rootEventId;
        newSummary.latestEventId = extractStr(eventJson, "event_id");
        newSummary.latestEventJson = eventJson;
        newSummary.isUserParticipating = (extractStr(eventJson, "sender") == userId);
        newSummary.numberOfThreads = 1;
        newSummary.updateType = ThreadSummaryUpdateType::ADD;
        newSummary.valid = true;

        int64_t ts = extractInt(eventJson, "origin_server_ts");
        if (ts > 0) newSummary.latestTimestampMs = ts;

        std::string body = extractStr(eventJson, "body");
        if (!body.empty()) newSummary.latestBody = body;

        summaries_[rootEventId] = newSummary;
    }
}

std::vector<ThreadSummary> ThreadManager::getAllSummaries() const {
    std::vector<ThreadSummary> result;
    for (const auto& [id, s] : summaries_) {
        result.push_back(s);
    }
    return result;
}

std::vector<ThreadSummary> ThreadManager::getRoomSummaries(const std::string& roomId) const {
    // Original Kotlin: ThreadSummaryEntity.findAllThreadsForRoomId()
    // Sorted by latest event timestamp descending
    std::vector<ThreadSummary> result;
    for (const auto& [id, s] : summaries_) {
        if (s.roomId == roomId) result.push_back(s);
    }
    std::sort(result.begin(), result.end(), [](const ThreadSummary& a, const ThreadSummary& b) {
        return a.latestTimestampMs > b.latestTimestampMs;
    });
    return result;
}

std::vector<ThreadSummary> ThreadManager::getNotificationSummaries() const {
    // Original Kotlin: TimelineEventEntity.findAllLocalThreadNotificationsForRoomId()
    std::vector<ThreadSummary> result;
    for (const auto& [id, s] : summaries_) {
        if (s.notificationState == ThreadNotificationState::NEW_MESSAGE ||
            s.notificationState == ThreadNotificationState::NEW_HIGHLIGHTED_MESSAGE) {
            result.push_back(s);
        }
    }
    return result;
}

bool ThreadManager::getSummary(const std::string& threadId, ThreadSummary& out) const {
    auto it = summaries_.find(threadId);
    if (it == summaries_.end()) return false;
    out = it->second;
    return true;
}

// ========================================================================
// NEW: Thread Notification State
// Original Kotlin: ThreadEventsHelper.updateThreadNotifications()
// ========================================================================

void ThreadManager::updateThreadNotifications(const std::string& roomId,
                                               const std::string& threadId,
                                               const std::string& currentUserId) {
    // Original Kotlin: updateThreadNotifications()
    auto summaryIt = summaries_.find(threadId);
    if (summaryIt == summaries_.end()) return;

    auto& summary = summaryIt->second;

    // Check participation
    if (summary.isUserParticipating) {
        summary.notificationState = ThreadNotificationState::NEW_MESSAGE;
    }

    // Check for highlights (simplified: if highlightCount > 0)
    if (summary.highlightCount > 0) {
        summary.notificationState = ThreadNotificationState::NEW_HIGHLIGHTED_MESSAGE;
    }

    // Also update the legacy unread store
    auto uit = unread_.find(threadId);
    if (uit != unread_.end()) {
        if (summary.isUserParticipating) {
            uit->second.highlighted = (summary.highlightCount > 0);
        }
    }
}

ThreadNotificationBadgeState ThreadManager::getNotificationBadgeState(const std::string& userId) const {
    // Original Kotlin: ThreadNotificationBadgeState computation
    ThreadNotificationBadgeState state;
    bool mentioned = false;
    for (const auto& [id, s] : summaries_) {
        if (s.notificationState == ThreadNotificationState::NEW_MESSAGE ||
            s.notificationState == ThreadNotificationState::NEW_HIGHLIGHTED_MESSAGE) {
            state.numberOfLocalUnreadThreads++;
        }
        if (s.notificationState == ThreadNotificationState::NEW_HIGHLIGHTED_MESSAGE) {
            mentioned = true;
        }
    }
    state.isUserMentioned = mentioned;
    return state;
}

// ========================================================================
// NEW: Static Event Inspection Utilities
// Original Kotlin: ThreadsAwarenessHandler, ThreadEventsHelper
// ========================================================================

bool ThreadManager::hasThreadRelation(const std::string& eventContentJson) {
    // Original Kotlin: ThreadsAwarenessHandler.isThreadEvent()
    // Check if event content has m.thread relation type
    return eventContentJson.find("\"rel_type\":\"m.thread\"") != std::string::npos ||
           eventContentJson.find("\"rel_type\": \"m.thread\"") != std::string::npos;
}

bool ThreadManager::checkIsThreadRoot(const std::string& eventContentJson,
                                       const std::string& eventId) {
    // Original Kotlin: EventEntity.isRootThread()
    // An event is a root if:
    // 1. It has m.relates_to with rel_type="m.thread" and event_id == eventId
    // 2. OR it doesn't have a thread relation (meaning it IS the root)
    if (eventContentJson.empty() || eventId.empty()) return false;

    // If this event has a thread relation pointing to another event, it's a reply
    auto rootId = extractThreadRoot(eventContentJson);
    if (!rootId.empty() && rootId != eventId) return false;

    // If it has a thread relation pointing to itself, it's a root
    if (rootId == eventId) return true;

    // Check nested m.relates_to for self-referencing
    auto relType = extractStr(eventContentJson, "rel_type");
    return relType == "m.thread";
}

std::string ThreadManager::extractThreadRootId(const std::string& eventContentJson) {
    // Original Kotlin: getRootThreadEventId()
    // Extract the root event ID from the thread relation
    return extractThreadRoot(eventContentJson);
}

std::string ThreadManager::formatPreview(const std::string& senderName,
                                          const std::string& body,
                                          int maxLen) {
    // Original Kotlin: ThreadList preview formatting
    std::string displayName = senderName.empty() ? "Unknown" : senderName;
    std::string previewText = body;

    // Truncate body to first line
    size_t nl = previewText.find('\n');
    if (nl != std::string::npos) {
        previewText = previewText.substr(0, nl);
    }

    if (static_cast<int>(previewText.size()) > maxLen) {
        previewText = previewText.substr(0, maxLen - 3) + "...";
    }

    return displayName + ": " + previewText;
}

// ====== Stats ======

int ThreadManager::totalRoomsWithThreads() const {
    std::unordered_set<std::string> rooms;
    for (const auto& [id, th] : threads_) rooms.insert(th.roomId);
    return static_cast<int>(rooms.size());
}

// ========================================================================
// FREE FUNCTIONS — Thread Models & Computation
// ========================================================================

// ---- Thread Summary Computation ----
// Original Kotlin: ThreadSummaryHelper.kt

ThreadSummary computeThreadSummary(const std::string& roomId,
                                    const std::string& rootEventJson,
                                    const std::string& latestEventJson,
                                    bool isUserParticipating,
                                    const std::string& userId) {
    // Original Kotlin: ThreadSummaryHelper.createOrUpdate REPLACE mode
    ThreadSummary summary;
    summary.roomId = roomId;
    summary.valid = true;

    // Parse root event
    std::string rootEventId = ThreadManager::extractStr(rootEventJson, "event_id");
    summary.rootEventId = rootEventId;
    summary.rootEventJson = rootEventJson;

    // Root sender info
    std::string rootSender = ThreadManager::extractStr(rootEventJson, "sender");
    summary.rootThreadSenderInfo.senderId = rootSender;
    summary.rootThreadSenderInfo.senderName = ThreadManager::extractStr(rootEventJson, "displayname");
    summary.rootThreadSenderInfo.avatarUrl = ThreadManager::extractStr(rootEventJson, "avatar_url");

    // Root timestamp
    summary.rootTimestampMs = ThreadManager::extractInt(rootEventJson, "origin_server_ts");

    // Root body
    std::string contentJson = ThreadManager::extractObj(rootEventJson, "content");
    if (!contentJson.empty()) {
        summary.rootBody = ThreadManager::extractStr(contentJson, "body");
    } else {
        summary.rootBody = ThreadManager::extractStr(rootEventJson, "body");
    }

    // Extract numberOfThreads from unsignedData.relations.latestThread.count
    // Original Kotlin: rootThreadEvent.unsignedData?.relations?.latestThread?.count
    int64_t count = 0;
    std::string unsignedData = ThreadManager::extractObj(rootEventJson, "unsigned");
    if (!unsignedData.empty()) {
        std::string relations = ThreadManager::extractObj(unsignedData, "relations");
        if (!relations.empty()) {
            std::string latestThread = ThreadManager::extractObj(relations, "m.latest_thread");
            if (!latestThread.empty()) {
                count = ThreadManager::extractInt(latestThread, "count");
                // Original Kotlin: isUserParticipating
                auto participating = ThreadManager::extractStr(latestThread, "participated");
                if (participating == "true") summary.isUserParticipating = true;
                // Get latest event from latestThread
                summary.latestEventJson = ThreadManager::extractObj(latestThread, "latest_event");
            }
        }
    }
    summary.numberOfThreads = static_cast<int>(count > 0 ? count : 0);

    // Override participation if user was explicitly passed
    if (isUserParticipating) {
        summary.isUserParticipating = true;
    } else if (rootSender == userId) {
        summary.isUserParticipating = true;
    }

    // Parse latest event if not already parsed from unsignedData
    if (summary.latestEventJson.empty() && !latestEventJson.empty()) {
        summary.latestEventJson = latestEventJson;
    }

    if (!summary.latestEventJson.empty()) {
        summary.latestEventId = ThreadManager::extractStr(summary.latestEventJson, "event_id");
        summary.latestTimestampMs = ThreadManager::extractInt(summary.latestEventJson, "origin_server_ts");

        std::string latestSender = ThreadManager::extractStr(summary.latestEventJson, "sender");
        summary.latestThreadSenderInfo.senderId = latestSender;
        summary.latestThreadSenderInfo.senderName = ThreadManager::extractStr(summary.latestEventJson, "displayname");
        summary.latestThreadSenderInfo.avatarUrl = ThreadManager::extractStr(summary.latestEventJson, "avatar_url");

        std::string latestContent = ThreadManager::extractObj(summary.latestEventJson, "content");
        if (!latestContent.empty()) {
            summary.latestBody = ThreadManager::extractStr(latestContent, "body");
        } else {
            summary.latestBody = ThreadManager::extractStr(summary.latestEventJson, "body");
        }
    }

    summary.updateType = ThreadSummaryUpdateType::REPLACE;

    return summary;
}

void updateThreadSummary(ThreadSummary& summary,
                         const std::string& eventJson,
                         ThreadSummaryUpdateType type,
                         const std::string& userId) {
    // Original Kotlin: ThreadSummaryHelper.createOrUpdate()
    if (type == ThreadSummaryUpdateType::REPLACE) {
        // Full replacement — recompute summary from root event
        std::string rootEventId = ThreadManager::extractStr(eventJson, "event_id");
        std::string senderId = ThreadManager::extractStr(eventJson, "sender");
        if (rootEventId.empty() || senderId.empty()) return;

        // Extract numberOfThreads from unsigned data
        std::string unsignedData = ThreadManager::extractObj(eventJson, "unsigned");
        if (!unsignedData.empty()) {
            std::string relations = ThreadManager::extractObj(unsignedData, "relations");
            if (!relations.empty()) {
                std::string latestThread = ThreadManager::extractObj(relations, "m.latest_thread");
                if (!latestThread.empty()) {
                    int64_t count = ThreadManager::extractInt(latestThread, "count");
                    if (count <= 0) return; // Original Kotlin: return null if count <= 0
                    summary.numberOfThreads = static_cast<int>(count);

                    auto participating = ThreadManager::extractStr(latestThread, "participated");
                    summary.isUserParticipating = (participating == "true" || senderId == userId);

                    // Get latest event
                    summary.latestEventJson = ThreadManager::extractObj(latestThread, "latest_event");
                }
            }
        }

        summary.rootEventId = rootEventId;
        summary.rootEventJson = eventJson;
        summary.rootTimestampMs = ThreadManager::extractInt(eventJson, "origin_server_ts");
        summary.rootThreadSenderInfo.senderId = senderId;
        summary.rootThreadSenderInfo.senderName = ThreadManager::extractStr(eventJson, "displayname");

        std::string contentJson = ThreadManager::extractObj(eventJson, "content");
        if (!contentJson.empty()) {
            summary.rootBody = ThreadManager::extractStr(contentJson, "body");
        }

        summary.updateType = ThreadSummaryUpdateType::REPLACE;

    } else {
        // ADD mode — incremental update
        // Original Kotlin: ThreadSummary ADD case
        std::string eventId = ThreadManager::extractStr(eventJson, "event_id");
        std::string senderId = ThreadManager::extractStr(eventJson, "sender");

        if (!eventId.empty()) {
            summary.latestEventId = eventId;
            summary.latestEventJson = eventJson;
        }
        summary.numberOfThreads++;

        if (senderId == userId) {
            summary.isUserParticipating = true;
        }

        summary.latestThreadSenderInfo.senderId = senderId;
        summary.latestThreadSenderInfo.senderName = ThreadManager::extractStr(eventJson, "displayname");

        int64_t ts = ThreadManager::extractInt(eventJson, "origin_server_ts");
        if (ts > 0) summary.latestTimestampMs = ts;

        std::string body = ThreadManager::extractStr(eventJson, "body");
        if (!body.empty()) summary.latestBody = body;

        summary.updateType = ThreadSummaryUpdateType::ADD;
    }

    summary.valid = true;
}

std::string formatThreadPreview(const ThreadSummary& summary) {
    // Original Kotlin: Formatting logic from ThreadListViewModel
    // Prefer latest reply text, fallback to root body
    std::string senderName = summary.latestThreadSenderInfo.senderName;
    if (senderName.empty()) senderName = summary.latestThreadSenderInfo.senderId;

    std::string body = summary.latestBody;
    if (body.empty()) body = summary.rootBody;

    return ThreadManager::formatPreview(senderName, body, 60);
}

bool isThreadEvent(const std::string& eventContentJson) {
    // Original Kotlin: ThreadsAwarenessHandler.isThreadEvent()
    return ThreadManager::hasThreadRelation(eventContentJson);
}

// ---- Thread Event Helpers ----
// Original Kotlin: ThreadEventsHelper.kt

std::string findRootThreadEventId(const std::string& eventContentJson) {
    // Original Kotlin: EventEntity.findRootThreadEvent()
    // Extract the rootThreadEventId from m.relates_to with rel_type="m.thread"
    return ThreadManager::extractThreadRootId(eventContentJson);
}

std::string getThreadEventRelation(const std::string& eventContentJson) {
    // Original Kotlin: getRootThreadRelationContent()
    // Find the m.relates_to object with rel_type="m.thread"
    size_t pos = eventContentJson.find("\"rel_type\":\"m.thread\"");
    if (pos == std::string::npos) {
        pos = eventContentJson.find("\"rel_type\": \"m.thread\"");
    }
    if (pos == std::string::npos) return "{}";

    // Find the enclosing object
    auto objStart = eventContentJson.rfind('{', pos);
    if (objStart == std::string::npos) return "{}";

    int depth = 0;
    auto objEnd = objStart;
    while (objEnd < eventContentJson.size()) {
        if (eventContentJson[objEnd] == '{') depth++;
        else if (eventContentJson[objEnd] == '}') depth--;
        if (depth == 0 && objEnd > objStart) break;
        objEnd++;
    }

    return eventContentJson.substr(objStart, objEnd - objStart + 1);
}

std::string makeEventThreadAware(const std::string& roomId,
                                  const std::string& eventContentJson,
                                  const std::string& parentEventJson) {
    // Original Kotlin: ThreadsAwarenessHandler.makeEventThreadAware()
    // This function makes a thread event displayable as a reply:
    // 1. Check if thread messages are enabled and event is not already a reply
    // 2. If event is a thread event:
    //    a. Get the event body
    //    b. Get the thread relation
    //    c. Get the parent event (previous in thread or root)
    //    d. Inject parent event as reply quote, or inject fallback indicator

    if (eventContentJson.empty() || !isThreadEvent(eventContentJson)) {
        return eventContentJson; // Not a thread event, return unchanged
    }

    // Extract event body
    std::string eventBody = ThreadManager::extractStr(eventContentJson, "body");
    if (eventBody.empty()) {
        // Try nested content
        std::string contentJson = ThreadManager::extractObj(eventContentJson, "content");
        if (!contentJson.empty()) {
            eventBody = ThreadManager::extractStr(contentJson, "body");
        }
    }
    if (eventBody.empty()) return eventContentJson;

    // Get the parent event ID (the event this thread message is replying to in the chain)
    // Original Kotlin: getPreviousEventOrRoot()
    std::string relatesTo = getThreadEventRelation(eventContentJson);
    std::string inReplyToEventId = ThreadManager::extractStr(relatesTo, "m.in_reply_to");
    std::string rootThreadEventId = ThreadManager::extractStr(relatesTo, "event_id");

    // Get thread relation for injection
    std::string threadRelation = getThreadEventRelation(eventContentJson);

    // Build the "in reply to" formatted body
    // Original Kotlin: injectEvent() / injectFallbackIndicator()
    if (!parentEventJson.empty()) {
        // We have the parent event — inject it as reply
        std::string parentBody = ThreadManager::extractStr(parentEventJson, "body");
        std::string parentContent = ThreadManager::extractObj(parentEventJson, "content");
        if (parentBody.empty() && !parentContent.empty()) {
            parentBody = ThreadManager::extractStr(parentContent, "body");
        }
        std::string parentSender = ThreadManager::extractStr(parentEventJson, "sender");
        std::string parentEventId = ThreadManager::extractStr(parentEventJson, "event_id");

        if (!parentBody.empty() && !parentEventId.empty()) {
            // Build reply-formatted body
            // Original Kotlin: REPLY_PATTERN with permalink
            std::string replyFormatted =
                "<mx-reply><blockquote>"
                "<a href=\"https://matrix.to/#/" + roomId + "/" + parentEventId + "\">In reply to</a>"
                "<a href=\"https://matrix.to/#/" + parentSender + "\">" + parentSender + "</a>"
                "<br/>" + parentBody +
                "</blockquote></mx-reply>" + eventBody;

            // Reconstruct the event JSON with modified body
            // This is a simplified version — full implementation would modify content in-place
            std::ostringstream os;
            os << "{\"body\":\"" << eventBody << "\"";
            os << ",\"format\":\"org.matrix.custom.html\"";
            os << ",\"formatted_body\":\"" << replyFormatted << "\"";
            os << ",\"m.relates_to\":" << threadRelation;
            os << "}";
            return os.str();
        }
    }

    // No parent event available — inject fallback indicator
    // Original Kotlin: injectFallbackIndicator()
    std::string fallbackBody =
        "> In reply to a thread\n" + eventBody;

    std::ostringstream os;
    os << "{\"body\":\"" << fallbackBody << "\"";
    os << ",\"m.relates_to\":" << threadRelation;
    os << "}";
    return os.str();
}

bool isEventInThread(const std::string& eventContentJson,
                     const std::string& threadRootId) {
    // Original Kotlin: check if event.rootThreadEventId == threadRootId
    if (threadRootId.empty()) return false;

    // Check if this event IS the root
    std::string eventId = ThreadManager::extractStr(eventContentJson, "event_id");
    if (eventId == threadRootId) return true;

    // Check if this event references the root
    std::string rootId = ThreadManager::extractThreadRootId(eventContentJson);
    return rootId == threadRootId;
}

// ---- Thread Pagination ----
// Original Kotlin: FetchThreadTimelineTask.kt, FetchThreadSummariesTask.kt

std::string buildThreadRelationsUrl(const std::string& roomId,
                                     const std::string& rootEventId,
                                     const std::string& from,
                                     int limit) {
    // Original Kotlin: roomAPI.getRelations()
    // GET /_matrix/client/v3/rooms/{roomId}/relations/{eventId}/m.thread?from={from}&limit={limit}
    std::ostringstream url;
    url << "/_matrix/client/v3/rooms/" << roomId
        << "/relations/" << rootEventId
        << "/m.thread?limit=" << limit;
    if (!from.empty()) {
        url << "&from=" << from;
    }
    return url.str();
}

std::string buildThreadSummariesUrl(const std::string& roomId,
                                     const std::string& from,
                                     int limit,
                                     const std::string& filter) {
    // Original Kotlin: roomAPI.getThreadsList()
    // GET /_matrix/client/v3/rooms/{roomId}/messages?filter={"types":["m.room.message"],...}
    std::ostringstream url;
    url << "/_matrix/client/v3/rooms/" << roomId
        << "/messages?limit=" << limit;
    if (!from.empty()) {
        url << "&from=" << from;
    }
    url << "&filter={\"types\":[\"m.room.message\"],\"related_by_rel_types\":[\"m.thread\"]";
    if (filter == "participated") {
        url << ",\"related_by_senders\":[\"" << filter << "\"]";
    }
    url << "}";
    return url.str();
}

std::string buildThreadTimelineRequest(const std::string& roomId,
                                        const std::string& rootEventId,
                                        const std::string& from,
                                        int limit) {
    // Build the full request JSON body (for APIs that need POST body)
    // Most Matrix APIs use GET for /relations, but provide JSON body too
    return buildThreadRelationsUrl(roomId, rootEventId, from, limit);
}

ThreadPaginationResponse parseThreadTimelineResponse(const std::string& responseJson) {
    // Original Kotlin: ThreadSummariesResponse + RelationsResponse parsing
    ThreadPaginationResponse resp;

    // Extract chunk array
    std::string chunkJson = ThreadManager::extractArray(responseJson, "chunk");
    if (chunkJson.empty()) {
        // Relations API uses "chunks" key instead of "chunk"
        chunkJson = ThreadManager::extractArray(responseJson, "chunks");
    }

    if (!chunkJson.empty()) {
        // Parse individual events from the array
        // Events are JSON objects separated by commas
        size_t pos = 1; // Skip opening '['
        int depth = 0;
        size_t start = pos;
        while (pos < chunkJson.size()) {
            if (chunkJson[pos] == '{') {
                if (depth == 0) start = pos;
                depth++;
            } else if (chunkJson[pos] == '}') {
                depth--;
                if (depth == 0) {
                    resp.events.push_back(chunkJson.substr(start, pos - start + 1));
                }
            } else if (chunkJson[pos] == '[') {
                depth++;
            } else if (chunkJson[pos] == ']') {
                depth--;
            }
            pos++;
        }
    }

    // Extract pagination tokens
    resp.nextBatch = ThreadManager::extractStr(responseJson, "next_batch");
    resp.prevBatch = ThreadManager::extractStr(responseJson, "prev_batch");

    resp.hasMore = !resp.nextBatch.empty();
    resp.valid = true;

    return resp;
}

// ---- Thread Notification ----
// Original Kotlin: ThreadEventsHelper.updateThreadNotifications()

ThreadNotificationState computeThreadNotifications(int unreadCount,
                                                     bool isUserMentioned,
                                                     bool isUserParticipating) {
    // Original Kotlin: ThreadNotificationState determination
    if (unreadCount <= 0 && !isUserMentioned) {
        return ThreadNotificationState::NO_NEW_MESSAGE;
    }

    if (isUserMentioned) {
        return ThreadNotificationState::NEW_HIGHLIGHTED_MESSAGE;
    }

    if (isUserParticipating && unreadCount > 0) {
        return ThreadNotificationState::NEW_MESSAGE;
    }

    return ThreadNotificationState::NO_NEW_MESSAGE;
}

ThreadNotificationBadgeState computeThreadBadgeState(
    const std::vector<ThreadSummary>& summaries,
    bool isUserMentioned) {
    // Original Kotlin: ThreadNotificationBadgeState
    ThreadNotificationBadgeState state;
    for (const auto& s : summaries) {
        if (s.notificationState == ThreadNotificationState::NEW_MESSAGE ||
            s.notificationState == ThreadNotificationState::NEW_HIGHLIGHTED_MESSAGE) {
            state.numberOfLocalUnreadThreads++;
            if (s.notificationState == ThreadNotificationState::NEW_HIGHLIGHTED_MESSAGE) {
                state.isUserMentioned = true;
            }
        }
    }
    if (isUserMentioned) {
        state.isUserMentioned = true;
    }
    return state;
}

// ---- Thread Root Detection ----
// Original Kotlin: isRootThread check

std::string extract_thread_root_id(const std::string& eventContentJson) {
    // Original Kotlin: getRootThreadEventId()
    return ThreadManager::extractThreadRootId(eventContentJson);
}

std::string buildThreadRootRelation(const std::string& rootEventId,
                                     bool isFallingBack) {
    // Original Kotlin: RelationDefaultContent for m.thread
    // Build JSON: {"rel_type":"m.thread","event_id":"<rootEventId>"}
    // If isFallingBack, add "is_falling_back":true
    std::ostringstream os;
    os << "{\"rel_type\":\"m.thread\""
       << ",\"event_id\":\"" << rootEventId << "\"";
    if (isFallingBack) {
        os << ",\"is_falling_back\":true";
    }
    // Include m.in_reply_to if needed
    os << ",\"m.in_reply_to\":{\"event_id\":\"" << rootEventId << "\"}";
    os << "}";
    return os.str();
}

bool isThreadRootEvent(const std::string& eventContentJson,
                        const std::string& eventId) {
    // Original Kotlin: EventEntity.isRootThread()
    return ThreadManager::checkIsThreadRoot(eventContentJson, eventId);
}

// ---- Type Conversion ----

ThreadInfoFull threadSummaryToInfoFull(const ThreadSummary& summary) {
    // Original Kotlin: Convert ThreadSummary → ThreadInfoFull mapping
    ThreadInfoFull info;
    info.threadId = summary.rootEventId;
    info.roomId = summary.roomId;
    info.rootSenderId = summary.rootThreadSenderInfo.senderId;
    info.rootSenderName = summary.rootThreadSenderInfo.senderName;
    info.rootBody = summary.rootBody;
    info.rootTimestampMs = summary.rootTimestampMs;
    info.replyCount = summary.numberOfThreads;
    info.lastReplyTimestampMs = summary.latestTimestampMs;
    info.lastReplySenderId = summary.latestThreadSenderInfo.senderId;
    info.lastReplySenderName = summary.latestThreadSenderInfo.senderName;
    info.lastReplyBody = summary.latestBody;
    info.rootRelationType = "m.thread";
    info.latestThreadEdition = summary.threadEditions.latestThreadEdition;
    info.valid = summary.valid;
    return info;
}

ThreadSummary infoFullToThreadSummary(const ThreadInfoFull& info) {
    // Original Kotlin: Convert ThreadInfoFull → ThreadSummary mapping
    ThreadSummary summary;
    summary.roomId = info.roomId;
    summary.rootEventId = info.threadId;
    summary.rootThreadSenderInfo.senderId = info.rootSenderId;
    summary.rootThreadSenderInfo.senderName = info.rootSenderName;
    summary.rootBody = info.rootBody;
    summary.rootTimestampMs = info.rootTimestampMs;
    summary.numberOfThreads = info.replyCount;
    summary.latestTimestampMs = info.lastReplyTimestampMs;
    summary.latestThreadSenderInfo.senderId = info.lastReplySenderId;
    summary.latestThreadSenderInfo.senderName = info.lastReplySenderName;
    summary.latestBody = info.lastReplyBody;
    summary.threadEditions.latestThreadEdition = info.latestThreadEdition;
    summary.valid = info.valid;
    return summary;
}

// ---- Serialization ----

std::string threadSummaryToJson(const ThreadSummary& summary) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) {
            if (c == '"') out += "\\\"";
            else if (c == '\\') out += "\\\\";
            else if (c == '\n') out += "\\n";
            else if (c == '\r') out += "\\r";
            else if (c == '\t') out += "\\t";
            else out += c;
        }
        return out;
    };

    std::ostringstream os;
    os << "{"
       << R"("room_id":")" << esc(summary.roomId) << "\""
       << R"(,"root_event_id":")" << esc(summary.rootEventId) << "\""
       << R"(,"latest_event_id":")" << esc(summary.latestEventId) << "\""
       << R"(,"root_sender_name":")" << esc(summary.rootThreadSenderInfo.senderName) << "\""
       << R"(,"root_sender_id":")" << esc(summary.rootThreadSenderInfo.senderId) << "\""
       << R"(,"latest_sender_name":")" << esc(summary.latestThreadSenderInfo.senderName) << "\""
       << R"(,"latest_sender_id":")" << esc(summary.latestThreadSenderInfo.senderId) << "\""
       << R"(,"is_participating":)" << (summary.isUserParticipating ? "true" : "false")
       << R"(,"number_of_threads":)" << summary.numberOfThreads
       << R"(,"highlight_count":)" << summary.highlightCount
       << R"(,"notification_count":)" << summary.notificationCount
       << R"(,"root_timestamp_ms":)" << summary.rootTimestampMs
       << R"(,"latest_timestamp_ms":)" << summary.latestTimestampMs
       << R"(,"latest_body":")" << esc(summary.latestBody) << "\""
       << R"(,"root_body":")" << esc(summary.rootBody) << "\""
       << R"(,"update_type":")" << (summary.updateType == ThreadSummaryUpdateType::REPLACE ? "REPLACE" : "ADD") << "\""
       << R"(,"notification_state":")";
    switch (summary.notificationState) {
        case ThreadNotificationState::NO_NEW_MESSAGE: os << "NO_NEW_MESSAGE\""; break;
        case ThreadNotificationState::NEW_MESSAGE: os << "NEW_MESSAGE\""; break;
        case ThreadNotificationState::NEW_HIGHLIGHTED_MESSAGE: os << "NEW_HIGHLIGHTED_MESSAGE\""; break;
    }
    if (!summary.threadEditions.rootThreadEdition.empty()) {
        os << R"(,"root_edition":")" << esc(summary.threadEditions.rootThreadEdition) << "\"";
    }
    if (!summary.threadEditions.latestThreadEdition.empty()) {
        os << R"(,"latest_edition":")" << esc(summary.threadEditions.latestThreadEdition) << "\"";
    }
    os << R"(,"valid":)" << (summary.valid ? "true" : "false")
       << "}";
    return os.str();
}

std::string threadTimelineEventToJson(const ThreadTimelineEvent& event) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) {
            if (c == '"') out += "\\\"";
            else if (c == '\\') out += "\\\\";
            else if (c == '\n') out += "\\n";
            else out += c;
        }
        return out;
    };

    std::ostringstream os;
    os << "{"
       << R"("event_id":")" << esc(event.eventId) << "\""
       << R"(,"event_type":")" << esc(event.eventType) << "\""
       << R"(,"sender_id":")" << esc(event.senderId) << "\""
       << R"(,"sender_name":")" << esc(event.senderName) << "\""
       << R"(,"body":")" << esc(event.body) << "\""
       << R"(,"timestamp_ms":)" << event.timestampMs
       << R"(,"root_thread_event_id":")" << esc(event.rootThreadEventId) << "\""
       << R"(,"is_thread_root":)" << (event.isThreadRoot ? "true" : "false")
       << R"(,"is_participating":)" << (event.isParticipating ? "true" : "false")
       << R"(,"is_encrypted":)" << (event.isEncrypted ? "true" : "false")
       << R"(,"valid":)" << (event.valid ? "true" : "false")
       << "}";
    return os.str();
}

std::string threadSummaryListToJson(const std::vector<ThreadSummary>& list) {
    std::ostringstream os;
    os << "[";
    for (size_t i = 0; i < list.size(); i++) {
        if (i > 0) os << ",";
        os << threadSummaryToJson(list[i]);
    }
    os << "]";
    return os.str();
}

} // namespace progressive
