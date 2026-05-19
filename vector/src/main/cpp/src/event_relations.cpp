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

// ==== Manual JSON Helpers (anonymous namespace) ====

namespace {

// Find a JSON key occurrence starting at or after pos
size_t findJsonKey(const std::string& json, const std::string& key, size_t start = 0) {
    std::string search = "\"" + key + "\"";
    size_t pos = json.find(search, start);
    if (pos == std::string::npos) return std::string::npos;
    // Verify it's not escaped
    if (pos > 0 && json[pos - 1] == '\\') return findJsonKey(json, key, pos + 1);
    return pos;
}

// Skip whitespace from pos
size_t skipWs(const std::string& s, size_t pos) {
    while (pos < s.size() && (s[pos] == ' ' || s[pos] == '\t' || s[pos] == '\n' || s[pos] == '\r')) pos++;
    return pos;
}

// Extract a quoted string value for a given key from a JSON object
std::string extractQuoted(const std::string& json, const std::string& key) {
    size_t pos = findJsonKey(json, key);
    if (pos == std::string::npos) return {};
    pos = json.find(':', pos);
    if (pos == std::string::npos) return {};
    pos = skipWs(json, pos + 1);
    if (pos >= json.size() || json[pos] != '"') return {};
    pos++; // skip opening quote
    size_t end = pos;
    while (end < json.size()) {
        if (json[end] == '\\' && end + 1 < json.size()) { end += 2; continue; }
        if (json[end] == '"') break;
        end++;
    }
    return json.substr(pos, end - pos);
}

// Extract an integer value for a given key from a JSON object
int64_t extractInt(const std::string& json, const std::string& key) {
    size_t pos = findJsonKey(json, key);
    if (pos == std::string::npos) return 0;
    pos = json.find(':', pos);
    if (pos == std::string::npos) return 0;
    pos = skipWs(json, pos + 1);
    size_t end = pos;
    while (end < json.size() && ((json[end] >= '0' && json[end] <= '9') || json[end] == '-')) end++;
    if (end == pos) return 0;
    return std::stoll(json.substr(pos, end - pos));
}

// Extract a boolean value for a given key from a JSON object
bool extractBool(const std::string& json, const std::string& key) {
    size_t pos = findJsonKey(json, key);
    if (pos == std::string::npos) return false;
    pos = json.find(':', pos);
    if (pos == std::string::npos) return false;
    pos = skipWs(json, pos + 1);
    return json.compare(pos, 4, "true") == 0;
}

// Extract a nested JSON object for a given key (returns raw {}-enclosed string)
std::string extractNested(const std::string& json, const std::string& key) {
    size_t pos = findJsonKey(json, key);
    if (pos == std::string::npos) return {};
    pos = json.find(':', pos);
    if (pos == std::string::npos) return {};
    pos = skipWs(json, pos + 1);
    if (pos >= json.size() || json[pos] != '{') return {};
    int depth = 0;
    size_t start = pos;
    while (pos < json.size()) {
        if (json[pos] == '{') depth++;
        else if (json[pos] == '}') {
            depth--;
            if (depth == 0) return json.substr(start, pos - start + 1);
        } else if (json[pos] == '"') {
            pos++;
            while (pos < json.size() && json[pos] != '"') {
                if (json[pos] == '\\' && pos + 1 < json.size()) pos++;
                pos++;
            }
        }
        pos++;
    }
    return {};
}

// Escape a string for JSON value output
std::string escapeJson(const std::string& s) {
    std::string out;
    out.reserve(s.size() + 8);
    for (char c : s) {
        switch (c) {
            case '"':  out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\n': out += "\\n";  break;
            case '\t': out += "\\t";  break;
            case '\r': out += "\\r";  break;
            default:   out += c;      break;
        }
    }
    return out;
}

// Unescape a JSON string (handle common escape sequences)
std::string unescapeJson(const std::string& s) {
    std::string out;
    out.reserve(s.size());
    for (size_t i = 0; i < s.size(); i++) {
        if (s[i] == '\\' && i + 1 < s.size()) {
            switch (s[i + 1]) {
                case '"':  out += '"';  i++; break;
                case '\\': out += '\\'; i++; break;
                case 'n':  out += '\n'; i++; break;
                case 't':  out += '\t'; i++; break;
                case 'r':  out += '\r'; i++; break;
                default:   out += s[i];       break;
            }
        } else {
            out += s[i];
        }
    }
    return out;
}

} // anonymous namespace

// ==== Relation Content Parsing / Building ====

// Original Kotlin: getRelationContent() -> RelationDefaultContent
RelationDefaultContent parseRelationContent(const std::string& contentJson) {
    RelationDefaultContent rc;

    // Original Kotlin: content.toModel<MessageContent>()?.relatesTo
    //                 ?: getClearContent()?.get("m.relates_to")
    // Extract the m.relates_to nested object from event content
    auto relatesTo = extractNested(contentJson, "m.relates_to");
    if (relatesTo.empty()) return rc;

    rc.type    = extractQuoted(relatesTo, "rel_type");
    rc.eventId = extractQuoted(relatesTo, "event_id");
    rc.key     = extractQuoted(relatesTo, "key");

    // Original Kotlin: m.in_reply_to as ReplyToContent
    auto inReplyTo = extractNested(relatesTo, "m.in_reply_to");
    if (!inReplyTo.empty()) {
        rc.inReplyToEventId = extractQuoted(inReplyTo, "event_id");
    }

    // Original Kotlin: option: Int?
    auto optPos = findJsonKey(relatesTo, "option");
    if (optPos != std::string::npos) {
        rc.option = static_cast<int>(extractInt(relatesTo, "option"));
    }

    // Original Kotlin: is_falling_back: Boolean?
    auto fbPos = findJsonKey(relatesTo, "is_falling_back");
    if (fbPos != std::string::npos) {
        rc.isFallingBack = extractBool(relatesTo, "is_falling_back");
    }

    return rc;
}

// Original Kotlin: builds m.relates_to in event content
std::string buildRelationContent(
    const std::string& relType,
    const std::string& eventId,
    const std::string& key,
    const std::string& inReplyToEventId,
    bool isFallingBack
) {
    std::ostringstream json;
    json << R"({"m.relates_to":{)";
    json << R"("rel_type":")" << escapeJson(relType) << R"(")";
    json << R"(,"event_id":")" << escapeJson(eventId) << R"(")";
    // Original Kotlin: key field for m.annotation
    if (!key.empty()) {
        json << R"(,"key":")" << escapeJson(key) << R"(")";
    }
    // Original Kotlin: m.in_reply_to for replies
    if (!inReplyToEventId.empty()) {
        json << R"(,"m.in_reply_to":{"event_id":")" << escapeJson(inReplyToEventId) << R"("})";
    }
    // Original Kotlin: is_falling_back flag
    if (isFallingBack) {
        json << R"(,"is_falling_back":true)";
    }
    json << "}}";
    return json.str();
}

// ==== Relation Aggregation Functions ====

// Original Kotlin: computeAggregatedAnnotations — reaction counts per key
std::unordered_map<std::string, AggregatedAnnotation> computeAggregatedAnnotations(
    const std::vector<MinimalEvent>& events
) {
    std::unordered_map<std::string, AggregatedAnnotation> result;

    for (const auto& ev : events) {
        auto rc = parseRelationContent(ev.contentJson);
        // Original Kotlin: rel_type must be m.annotation
        if (rc.type != "m.annotation" || rc.key.empty()) continue;

        auto& agg = result[rc.key];
        agg.key = rc.key;
        agg.count++;
        agg.sourceEvents.push_back(ev.eventId);
    }

    return result;
}

// Original Kotlin: computeAggregatedReplace — find latest edit in chain
std::optional<AggregatedReplace> computeAggregatedReplace(
    const std::vector<MinimalEvent>& events
) {
    // Original Kotlin: events are sorted by originServerTs then eventId
    // The latest is the one with highest (ts, eventId)
    AggregatedReplace best;
    bool found = false;

    for (const auto& ev : events) {
        auto rc = parseRelationContent(ev.contentJson);
        // Original Kotlin: rel_type must be m.replace
        if (rc.type != "m.replace" || rc.eventId.empty()) continue;

        if (!found) {
            best.eventId        = ev.eventId;
            best.originServerTs = ev.originServerTs;
            best.senderId       = ev.senderId;
            found = true;
            continue;
        }

        // Original Kotlin: compareBy<EditionOfEvent> { it.timestamp }.thenBy { it.eventId }
        // Higher timestamp = more recent; if equal, lexicographically larger eventId = more recent
        if (ev.originServerTs > best.originServerTs ||
            (ev.originServerTs == best.originServerTs && ev.eventId > best.eventId)) {
            best.eventId        = ev.eventId;
            best.originServerTs = ev.originServerTs;
            best.senderId       = ev.senderId;
        }
    }

    if (!found) return std::nullopt;
    return best;
}

// Original Kotlin: computeAggregatedReferences — collect m.reference events
std::vector<AggregatedReference> computeAggregatedReferences(
    const std::vector<MinimalEvent>& events
) {
    std::vector<AggregatedReference> result;

    for (const auto& ev : events) {
        auto rc = parseRelationContent(ev.contentJson);
        // Original Kotlin: rel_type must be m.reference
        if (rc.type != "m.reference") continue;

        AggregatedReference ref;
        ref.eventId        = ev.eventId;
        ref.originServerTs = ev.originServerTs;
        ref.senderId       = ev.senderId;
        ref.type           = ev.type;
        result.push_back(std::move(ref));
    }

    return result;
}

// Original Kotlin: aggregateRelations — compute all relation types from event list
AggregatedRelationsData aggregateRelations(
    const std::vector<MinimalEvent>& relatedEvents,
    const std::string& currentUserId
) {
    AggregatedRelationsData data;
    (void)currentUserId; // reserved for future use (e.g. adding addedByMe)

    data.annotations = computeAggregatedAnnotations(relatedEvents);
    data.replace     = computeAggregatedReplace(relatedEvents);
    data.references  = computeAggregatedReferences(relatedEvents);

    return data;
}

// ==== Event Edit Processing ====

// Original Kotlin: EditAggregatedSummary.kt — compute edit aggregation
EditAggregationInfo computeEditAggregation(
    const std::vector<MinimalEvent>& editEvents
) {
    EditAggregationInfo result;

    if (editEvents.empty()) return result;

    // Original Kotlin: editions sorted by compareBy { it.timestamp }.thenBy { it.eventId }
    // Find the latest (highest sort order = most recent)
    for (const auto& ev : editEvents) {
        EditionOfEvent ed;
        ed.eventId    = ev.eventId;
        ed.timestamp  = ev.originServerTs;
        ed.isLocalEcho = ev.isLocalEcho;

        result.editions.push_back(std::move(ed));
    }

    // Original Kotlin: latestEdition = sortedWith(...).lastOrNull()
    // Sort by timestamp then eventId
    std::sort(result.editions.begin(), result.editions.end(),
        [](const EditionOfEvent& a, const EditionOfEvent& b) {
            if (a.timestamp != b.timestamp) return a.timestamp < b.timestamp;
            return a.eventId < b.eventId;
        });

    const auto& latest = result.editions.back();
    result.latestEditEventId  = latest.eventId;
    result.lastEditTs         = latest.timestamp;
    result.editCount          = static_cast<int>(result.editions.size());

    // Find the original event's sender for latestEditSenderId
    for (const auto& ev : editEvents) {
        if (ev.eventId == latest.eventId) {
            result.latestEditSenderId = ev.senderId;
            break;
        }
    }

    return result;
}

// ==== Reaction Processing ====

// Original Kotlin: ReactionAggregatedSummary.kt — compute reaction aggregation
std::vector<ReactionAggregationInfo> computeReactionAggregation(
    const std::vector<MinimalEvent>& reactionEvents,
    const std::string& currentUserId
) {
    // Original Kotlin: handleReaction — group by key
    struct Accum {
        int count = 0;
        int64_t firstTs = INT64_MAX;
        bool addedByMe = false;
        std::vector<std::string> srcEvents;
        std::vector<std::string> echoEvents;
    };
    std::unordered_map<std::string, Accum> groups;

    for (const auto& ev : reactionEvents) {
        auto rc = parseRelationContent(ev.contentJson);
        // Original Kotlin: rel_type must be m.annotation
        if (rc.type != "m.annotation" || rc.key.empty()) continue;

        auto& acc = groups[rc.key];
        acc.count++;

        if (ev.originServerTs > 0 && ev.originServerTs < acc.firstTs) {
            acc.firstTs = ev.originServerTs;
        }

        // Original Kotlin: check if current user added this reaction
        if (!currentUserId.empty() && ev.senderId == currentUserId) {
            acc.addedByMe = true;
        }

        // Original Kotlin: track source events (remote) vs local echo
        if (ev.isLocalEcho) {
            acc.echoEvents.push_back(ev.eventId);
        } else {
            acc.srcEvents.push_back(ev.eventId);
        }
    }

    // Original Kotlin: convert to list of ReactionAggregatedSummary
    std::vector<ReactionAggregationInfo> result;
    result.reserve(groups.size());

    for (auto& [key, acc] : groups) {
        ReactionAggregationInfo info;
        info.key            = key;
        info.count          = acc.count;
        info.addedByMe      = acc.addedByMe;
        info.firstTimestamp = (acc.firstTs == INT64_MAX) ? 0 : acc.firstTs;
        info.sourceEvents   = std::move(acc.srcEvents);
        info.localEchoEvents = std::move(acc.echoEvents);
        result.push_back(std::move(info));
    }

    return result;
}

// ==== References Aggregation (MSC3912) ====

// Original Kotlin: parse ReferencesAggregatedContent from JSON
ReferencesAggregatedContent parseReferencesAggregatedContent(const std::string& contentJson) {
    ReferencesAggregatedContent result;

    // Original Kotlin: @Json(name = "verif_sum") val verificationState: VerificationState
    auto stateStr = extractQuoted(contentJson, "verif_sum");

    // Original Kotlin: VerificationState enum values
    if (stateStr == "WAITING")
        result.verificationState = VerificationState::WAITING;
    else if (stateStr == "CANCELED_BY_ME")
        result.verificationState = VerificationState::CANCELED_BY_ME;
    else if (stateStr == "CANCELED_BY_OTHER")
        result.verificationState = VerificationState::CANCELED_BY_OTHER;
    else if (stateStr == "DONE")
        result.verificationState = VerificationState::DONE;
    else
        result.verificationState = VerificationState::REQUEST;

    return result;
}

// ==== Event Annotations Summary ====

// Original Kotlin: buildEventAnnotationsSummary — construct from components
EventAnnotationsSummary buildEventAnnotationsSummary(
    const std::vector<ReactionAggregationInfo>& reactions,
    const std::optional<EditAggregationInfo>& edit,
    const std::optional<ReferencesAggregatedSummary>& refs
) {
    EventAnnotationsSummary summary;
    summary.reactionsSummary  = reactions;
    summary.editSummary       = edit;
    summary.referencesSummary = refs;
    return summary;
}

// Original Kotlin: serialize EventAnnotationsSummary to JSON
std::string eventAnnotationsSummaryToJson(const EventAnnotationsSummary& summary) {
    std::ostringstream json;
    json << "{";

    // Reactions summary
    json << R"("reactionsSummary":[)";
    for (size_t i = 0; i < summary.reactionsSummary.size(); i++) {
        if (i > 0) json << ",";
        const auto& r = summary.reactionsSummary[i];
        json << R"({"key":")" << escapeJson(r.key) << R"(")";
        json << R"(,"count":)" << r.count;
        json << R"(,"addedByMe":)" << (r.addedByMe ? "true" : "false");
        json << R"(,"firstTimestamp":)" << r.firstTimestamp;
        json << "}";
    }
    json << "]";

    // Edit summary
    if (summary.editSummary.has_value()) {
        json << R"(,"editSummary":{)";
        const auto& ed = *summary.editSummary;
        json << R"("latestEditEventId":")" << escapeJson(ed.latestEditEventId) << R"(")";
        json << R"(,"lastEditTs":)" << ed.lastEditTs;
        json << R"(,"editCount":)" << ed.editCount;
        json << "}";
    } else {
        json << R"(,"editSummary":null)";
    }

    // References summary
    if (summary.referencesSummary.has_value()) {
        json << R"(,"referencesSummary":{)";
        const auto& ref = *summary.referencesSummary;
        json << R"("content":)" << (ref.contentJson.empty() ? "null" : ref.contentJson);
        json << R"(,"sourceEvents":[)";
        for (size_t i = 0; i < ref.sourceEvents.size(); i++) {
            if (i > 0) json << ",";
            json << R"(")" << escapeJson(ref.sourceEvents[i]) << R"(")";
        }
        json << "]";
        json << R"(,"localEchoEventIds":[)";
        for (size_t i = 0; i < ref.localEchoEventIds.size(); i++) {
            if (i > 0) json << ",";
            json << R"(")" << escapeJson(ref.localEchoEventIds[i]) << R"(")";
        }
        json << "]";
        json << "}";
    } else {
        json << R"(,"referencesSummary":null)";
    }

    json << "}";
    return json.str();
}

// Original Kotlin: parse EventAnnotationsSummary from JSON
EventAnnotationsSummary parseEventAnnotationsSummary(const std::string& json) {
    EventAnnotationsSummary summary;

    // Parse reactions summary array
    auto reactionsArr = extractNested(json, "reactionsSummary");
    if (!reactionsArr.empty()) {
        size_t pos = 0;
        while (pos < reactionsArr.size()) {
            pos = reactionsArr.find('{', pos);
            if (pos == std::string::npos) break;
            // Manual extraction of current object by brace matching
            if (reactionsArr[pos] == '{') {
                int depth = 0;
                size_t start = pos;
                while (pos < reactionsArr.size()) {
                    if (reactionsArr[pos] == '{') depth++;
                    else if (reactionsArr[pos] == '}') {
                        depth--;
                        if (depth == 0) {
                            std::string obj = reactionsArr.substr(start, pos - start + 1);
                            ReactionAggregationInfo ri;
                            ri.key            = extractQuoted(obj, "key");
                            ri.count          = static_cast<int>(extractInt(obj, "count"));
                            ri.addedByMe      = extractBool(obj, "addedByMe");
                            ri.firstTimestamp = extractInt(obj, "firstTimestamp");
                            summary.reactionsSummary.push_back(std::move(ri));
                            pos++;
                            break;
                        }
                    }
                    pos++;
                }
            }
        }
    }

    // Parse edit summary
    auto editObj = extractNested(json, "editSummary");
    if (!editObj.empty()) {
        EditAggregationInfo ed;
        ed.latestEditEventId = extractQuoted(editObj, "latestEditEventId");
        ed.lastEditTs         = extractInt(editObj, "lastEditTs");
        ed.editCount          = static_cast<int>(extractInt(editObj, "editCount"));
        summary.editSummary   = std::move(ed);
    }

    // Parse references summary
    auto refObj = extractNested(json, "referencesSummary");
    if (!refObj.empty()) {
        ReferencesAggregatedSummary ref;
        ref.contentJson = extractNested(refObj, "content");
        // Parse sourceEvents array
        auto srcArr = extractNested(refObj, "sourceEvents");
        if (!srcArr.empty()) {
            size_t pos = srcArr.find('"');
            while (pos != std::string::npos && pos < srcArr.size()) {
                size_t end = srcArr.find('"', pos + 1);
                if (end == std::string::npos) break;
                ref.sourceEvents.push_back(srcArr.substr(pos + 1, end - pos - 1));
                pos = srcArr.find('"', end + 1);
            }
        }
        // Parse localEchoEventIds array
        auto echoArr = extractNested(refObj, "localEchoEventIds");
        if (!echoArr.empty()) {
            size_t pos = echoArr.find('"');
            while (pos != std::string::npos && pos < echoArr.size()) {
                size_t end = echoArr.find('"', pos + 1);
                if (end == std::string::npos) break;
                ref.localEchoEventIds.push_back(echoArr.substr(pos + 1, end - pos - 1));
                pos = echoArr.find('"', end + 1);
            }
        }
        summary.referencesSummary = std::move(ref);
    }

    return summary;
}

} // namespace progressive
