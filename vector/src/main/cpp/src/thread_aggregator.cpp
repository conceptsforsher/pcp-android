#include "progressive/thread_aggregator.hpp"
#include <sstream>
#include <algorithm>

namespace progressive {

void ThreadAggregator::addThread(const ThreadInfo& thread) {
    threads_[thread.threadId] = thread;
}

void ThreadAggregator::removeRoom(const std::string& roomId) {
    for (auto it = threads_.begin(); it != threads_.end(); ) {
        if (it->second.roomId == roomId) {
            it = threads_.erase(it);
        } else {
            ++it;
        }
    }
}

void ThreadAggregator::removeAccount(const std::string& accountId) {
    for (auto it = threads_.begin(); it != threads_.end(); ) {
        if (it->second.accountId == accountId) {
            it = threads_.erase(it);
        } else {
            ++it;
        }
    }
}

std::vector<ThreadInfo> ThreadAggregator::getAllThreads() const {
    std::vector<ThreadInfo> result;
    for (const auto& p : threads_) {
        const auto& t = p.second;
        result.push_back(t);
    }
    // Sort by last timestamp, most recent first
    std::sort(result.begin(), result.end(), [](const ThreadInfo& a, const ThreadInfo& b) {
        return a.lastTimestamp > b.lastTimestamp;
    });
    return result;
}

void ThreadAggregator::clear() {
    threads_.clear();
}

std::string ThreadAggregator::exportJson() const {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) {
            if (c == '"') out += "\\\"";
            else if (c == '\\') out += "\\\\";
            else out += c;
        }
        return out;
    };

    auto threads = getAllThreads();
    std::ostringstream json;
    json << "[";
    for (size_t i = 0; i < threads.size(); ++i) {
        if (i > 0) json << ",";
        const auto& t = threads[i];
        json << "{";
        json << R"("threadId": ")" << esc(t.threadId) << R"(",)";
        json << R"("roomId": ")" << esc(t.roomId) << R"(",)";
        json << R"("roomName": ")" << esc(t.roomName) << R"(",)";
        json << R"("accountId": ")" << esc(t.accountId) << R"(",)";
        json << R"("accountIndex": ")" << esc(t.accountIndex) << R"(",)";
        json << R"("lastMessage": ")" << esc(t.lastMessage) << R"(",)";
        json << R"("lastSender": ")" << esc(t.lastSender) << R"(",)";
        json << R"("lastTimestamp": )" << t.lastTimestamp << ",";
        json << R"("replyCount": )" << t.replyCount << ",";
        json << R"("unread": )" << (t.unread ? "true" : "false");
        json << "}";
    }
    json << "]";
    return json.str();
}

// ---- Thread Meta (header display metadata) ----

std::string ThreadMeta::messageCountText() const {
    if (replyCount == 0) return "0 messages";
    if (replyCount == 1) return "1 message";
    return std::to_string(replyCount) + " messages";
}

std::string ThreadMeta::title() const {
    std::string t = rootBody;
    if (t.empty()) return "Thread";

    // Truncate long titles
    if (t.size() > 60) {
        t = t.substr(0, 57) + "...";
    }

    // Prefix with sender name if known
    if (!rootSenderName.empty()) {
        return rootSenderName + ": " + t;
    }
    return t;
}

ThreadMeta computeThreadMeta(
    const std::string& rootEventContent,
    const std::vector<std::string>& replySenders,
    const std::vector<std::string>& replyBodies,
    const std::vector<int64_t>& replyTimestamps)
{
    ThreadMeta meta;

    // Parse root event: extract body and sender from content JSON
    auto extract = [](const std::string& json, const std::string& key) -> std::string {
        auto search = "\"" + key + "\":\"";
        auto pos = json.find(search);
        if (pos == std::string::npos) {
            search = "\"" + key + "\": \"";
            pos = json.find(search);
        }
        if (pos == std::string::npos) return "";
        pos += search.size();
        auto end = json.find('"', pos);
        if (end == std::string::npos) return "";
        return json.substr(pos, end - pos);
    };

    meta.rootBody = extract(rootEventContent, "body");
    meta.rootSenderId = extract(rootEventContent, "sender");
    meta.rootEventId = extract(rootEventContent, "event_id");
    meta.valid = !meta.rootEventId.empty() && !meta.rootBody.empty();

    // Count replies and find last reply
    meta.replyCount = static_cast<int>(replyBodies.size());

    if (!replyBodies.empty()) {
        size_t lastIdx = replyBodies.size() - 1;
        meta.lastReplyBody = replyBodies[lastIdx];
        if (lastIdx < replySenders.size()) meta.lastReplySenderName = replySenders[lastIdx];
        if (lastIdx < replyTimestamps.size()) meta.lastReplyTs = replyTimestamps[lastIdx];
    }

    return meta;
}

std::string threadMetaToJson(const ThreadMeta& meta) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"valid": )" << (meta.valid ? "true" : "false") << ",";
    json << R"("rootEventId": ")" << esc(meta.rootEventId) << R"(",)";
    json << R"("rootBody": ")" << esc(meta.rootBody) << R"(",)";
    json << R"("rootSenderId": ")" << esc(meta.rootSenderId) << R"(",)";
    json << R"("rootSenderName": ")" << esc(meta.rootSenderName) << R"(",)";
    json << R"("rootSenderAvatar": ")" << esc(meta.rootSenderAvatar) << R"(",)";
    json << R"("title": ")" << esc(meta.title()) << R"(",)";
    json << R"("messageCountText": ")" << esc(meta.messageCountText()) << R"(",)";
    json << R"("replyCount": )" << meta.replyCount << ",";
    json << R"("isUnread": )" << (meta.isUnread ? "true" : "false") << ",";
    json << R"("lastReplyBody": ")" << esc(meta.lastReplyBody) << R"(",)";
    json << R"("lastReplySenderName": ")" << esc(meta.lastReplySenderName) << R"(")";
    json << "}";
    return json.str();
}

} // namespace progressive
