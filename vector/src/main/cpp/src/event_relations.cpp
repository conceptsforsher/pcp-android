#include "progressive/event_relations.hpp"
#include "progressive/json_parser.hpp"
#include <sstream>
#include <unordered_set>
#include <unordered_map>

namespace progressive {

EventRelationInfo parseEventRelation(const std::string& contentJson) {
    EventRelationInfo info;

    // Extract m.relates_to block
    auto relatesTo = parseJsonStringValue(contentJson, "m.relates_to");
    if (relatesTo.empty()) {
        relatesTo = parseJsonStringValue(contentJson, "relates_to");
    }
    if (relatesTo.empty()) return info;

    std::string wrapped = "{" + relatesTo + "}";
    info.relType = parseJsonStringValue(wrapped, "rel_type");
    info.eventId = parseJsonStringValue(wrapped, "event_id");
    info.key     = parseJsonStringValue(wrapped, "key");

    // For m.replace
    info.fallback = parseJsonStringValue(contentJson, "body");
    auto newContent = parseJsonStringValue(contentJson, "m.new_content");
    if (!newContent.empty()) {
        std::string nw = "{" + newContent + "}";
        info.fallback = parseJsonStringValue(nw, "body");
    }

    return info;
}

bool isThreadRoot(const std::string& contentJson) {
    return contentJson.find("\"rel_type\":\"m.thread\"") != std::string::npos;
}

bool isReply(const std::string& contentJson) {
    auto relatesTo = parseJsonStringValue(contentJson, "m.relates_to");
    if (relatesTo.empty()) return false;
    return relatesTo.find("\"rel_type\":\"m.reference\"") != std::string::npos ||
           relatesTo.find("\"rel_type\": \"m.reference\"") != std::string::npos;
}

bool isEdit(const std::string& contentJson) {
    auto relatesTo = parseJsonStringValue(contentJson, "m.relates_to");
    if (relatesTo.empty()) return false;
    return relatesTo.find("\"rel_type\":\"m.replace\"") != std::string::npos ||
           relatesTo.find("\"rel_type\": \"m.replace\"") != std::string::npos;
}

bool isReaction(const std::string& contentJson) {
    auto relatesTo = parseJsonStringValue(contentJson, "m.relates_to");
    if (relatesTo.empty()) return false;
    return relatesTo.find("\"rel_type\":\"m.annotation\"") != std::string::npos ||
           relatesTo.find("\"rel_type\": \"m.annotation\"") != std::string::npos;
}

std::string extractThreadRoot(const std::string& contentJson) {
    auto rel = parseEventRelation(contentJson);
    if (rel.relType == "m.thread") return rel.eventId;
    return {};
}

std::string extractReplySource(const std::string& contentJson) {
    auto rel = parseEventRelation(contentJson);
    if (rel.relType == "m.reference") return rel.eventId;
    return {};
}

std::string extractEditSource(const std::string& contentJson) {
    auto rel = parseEventRelation(contentJson);
    if (rel.relType == "m.replace") return rel.eventId;
    return {};
}

std::string buildReplyRelationWithThread(const std::string& eventId, const std::string& threadRoot) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"m.relates_to": {"event_id": ")" << esc(eventId)
         << R"(", "rel_type": "m.reference")";
    if (!threadRoot.empty())
        json << R"(,"m.in_reply_to": {"event_id": ")" << esc(threadRoot) << R"("})";
    json << "}}";
    return json.str();
}

std::string buildThreadRelation(const std::string& threadRoot) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    return R"({"m.relates_to": {"event_id": ")" + esc(threadRoot) +
           R"(", "rel_type": "m.thread"}})";
}

std::string buildEditRelation(const std::string& eventId) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    return R"({"m.relates_to": {"event_id": ")" + esc(eventId) +
           R"(", "rel_type": "m.replace"}})";
}

std::string formatRelationDescription(const EventRelationInfo& relation) {
    if (relation.relType == "m.annotation") return "Reaction: " + relation.key;
    if (relation.relType == "m.reference") return "Reply to message";
    if (relation.relType == "m.replace") return "Edited message";
    if (relation.relType == "m.thread") return "Thread reply";
    return "Unknown relation: " + relation.relType;
}

ThreadSummary computeThreadSummary(
    const std::string& rootEventId,
    const std::string& rootBody,
    const std::string& rootSender,
    const std::vector<std::string>& replyBodies,
    const std::vector<std::string>& replySenders,
    const std::vector<int64_t>& replyTimestamps,
    bool hasUnread
) {
    ThreadSummary summary;
    summary.rootEventId = rootEventId;
    summary.rootMessageBody = rootBody;
    summary.rootSender = rootSender;
    summary.replyCount = static_cast<int>(replyBodies.size());
    summary.unread = hasUnread;

    if (!replyBodies.empty()) {
        summary.lastReplyBody = replyBodies.back();
        summary.lastReplySender = replySenders.back();
        summary.lastReplyTs = replyTimestamps.back();
    }

    // Count unique participants
    std::unordered_set<std::string> participants;
    participants.insert(rootSender);
    for (const auto& s : replySenders) participants.insert(s);
    summary.participantCount = static_cast<int>(participants.size());

    return summary;
}

std::string threadSummaryToJson(const ThreadSummary& summary) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"rootEventId": ")" << esc(summary.rootEventId) << R"(")";
    json << R"(,"rootMessage": ")" << esc(summary.rootMessageBody) << R"(")";
    json << R"(,"rootSender": ")" << esc(summary.rootSender) << R"(")";
    json << R"(,"replyCount": )" << summary.replyCount << ",";
    json << R"(,"participantCount": )" << summary.participantCount << ",";
    json << R"(,"unread": )" << (summary.unread ? "true" : "false");
    if (!summary.lastReplyBody.empty()) {
        json << R"(,"lastReply": ")" << esc(summary.lastReplyBody) << R"(")";
        json << R"(,"lastReplySender": ")" << esc(summary.lastReplySender) << R"(")";
    }
    json << "}";
    return json.str();
}

// ==== Build Thread List from Events JSON ====

std::string buildThreadListJson(const std::string& eventsJson) {
    struct ThreadData {
        std::string rootId;
        std::string latestEventId;
        int64_t latestTs = 0;
        int replyCount = 0;
    };
    std::unordered_map<std::string, ThreadData> threads;
    size_t pos = 0;
    while (pos < eventsJson.size()) {
        pos = eventsJson.find("\"m.thread\"", pos);
        if (pos == std::string::npos) break;

        // Find the root event_id in this relation
        auto evPos = eventsJson.find("\"event_id\"", pos);
        if (evPos != std::string::npos) {
            evPos = eventsJson.find(':', evPos);
            if (evPos != std::string::npos) {
                evPos++;
                while (evPos < eventsJson.size() && eventsJson[evPos] != '"') evPos++;
                evPos++;
                size_t end = evPos;
                while (end < eventsJson.size() && eventsJson[end] != '"') end++;
                std::string rootId = eventsJson.substr(evPos, end - evPos);

                if (!rootId.empty() && rootId[0] == '$') {
                    auto& td = threads[rootId];
                    td.rootId = rootId;
                    td.replyCount++;
                }
            }
        }
        pos++;
    }

    // Build JSON output
    std::ostringstream os; os << "[";
    bool first = true;
    for (auto& kv : threads) {
        if (!first) os << ","; first = false;
        os << R"({"root_event_id":")" << kv.second.rootId
           << R"(","reply_count":)" << kv.second.replyCount << "}";
    }
    os << "]";
    return os.str();
}

// ==== Thread Unread Counter ====

ThreadUnreadCount computeThreadUnreadCount(
    const std::vector<std::string>& eventIds,
    const std::string& readReceiptEventId,
    const std::vector<std::string>& highlightIds)
{
    ThreadUnreadCount result;
    result.totalReplies = static_cast<int>(eventIds.size());

    if (readReceiptEventId.empty()) {
        // Nothing read — all are unread
        result.unreadReplies = result.totalReplies;
        result.hasUnread = result.totalReplies > 0;
        for (const auto& id : eventIds) {
            if (std::find(highlightIds.begin(), highlightIds.end(), id) != highlightIds.end())
                result.highlightReplies++;
        }
        return result;
    }

    // Find read receipt position
    int readPos = -1;
    for (int i = 0; i < result.totalReplies; i++) {
        if (eventIds[i] == readReceiptEventId) { readPos = i; break; }
    }

    // Count unread events after read position
    for (int i = readPos + 1; i < result.totalReplies; i++) {
        result.unreadReplies++;
        if (std::find(highlightIds.begin(), highlightIds.end(), eventIds[i]) != highlightIds.end())
            result.highlightReplies++;
    }

    result.hasUnread = result.unreadReplies > 0;
    return result;
}

} // namespace progressive
