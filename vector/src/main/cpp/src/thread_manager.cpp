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
    while (pp < json.size() && json[pp] >= '0' && json[pp] <= '9') { v=v*10+(json[pp]-'0'); pp++; }
    return v;
}

// ====== Constructor ======

ThreadManager::ThreadManager() {}

// ====== Thread Root Detection ======

bool ThreadManager::isThreadRoot(const std::string& eventContentJson, const std::string& eventId) {
    // Check for m.relates_to with rel_type = "m.thread" and event_id = this event
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
    // Check for m.in_reply_to with thread root
    auto inReplyTo = extractStr(eventContentJson, "m.in_reply_to");
    if (!inReplyTo.empty()) {
        // Nested JSON — extract event_id from {"event_id":"..."}
        // If it's a JSON object string, parse it
        // Actually, check if the relates_to has rel_type = "m.thread"
    }

    // Check m.relates_to for thread relation
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
}

void ThreadManager::clearRoom(const std::string& roomId) {
    for (auto it = threads_.begin(); it != threads_.end(); ) {
        if (it->second.roomId == roomId) {
            unread_.erase(it->first);
            readReceipts_.erase(it->first);
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

std::string ThreadManager::formatThreadNotificationCount(int count) const {
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

int ThreadManager::totalRoomsWithThreads() const {
    std::unordered_set<std::string> rooms;
    for (const auto& [id, th] : threads_) rooms.insert(th.roomId);
    return static_cast<int>(rooms.size());
}

} // namespace progressive
