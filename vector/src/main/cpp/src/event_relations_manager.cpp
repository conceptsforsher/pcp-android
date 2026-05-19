#include "progressive/event_relations_manager.hpp"
#include <sstream>

namespace progressive {

// ====== Enum conversions ======

const char* relationTypeToString(EventRelationType type) {
    switch (type) {
        case EventRelationType::ANNOTATION: return "m.annotation";
        case EventRelationType::REPLACE: return "m.replace";
        case EventRelationType::REFERENCE: return "m.reference";
        case EventRelationType::THREAD: return "m.thread";
        case EventRelationType::RESPONSE: return "org.matrix.response";
        default: return "unknown";
    }
}

EventRelationType relationTypeFromString(const std::string& s) {
    if (s == "m.annotation") return EventRelationType::ANNOTATION;
    if (s == "m.replace") return EventRelationType::REPLACE;
    if (s == "m.reference") return EventRelationType::REFERENCE;
    if (s == "m.thread") return EventRelationType::THREAD;
    if (s == "org.matrix.response") return EventRelationType::RESPONSE;
    return EventRelationType::UNKNOWN;
}

// ====== JSON helpers ======

std::string EventRelationsManager::extractStr(const std::string& json, const std::string& key) {
    auto pp = json.find("\"" + key + "\"");
    if (pp == std::string::npos) return "";
    pp = json.find('"', pp + key.size() + 2);
    if (pp == std::string::npos) return "";
    pp++;
    size_t e = pp;
    while (e < json.size() && json[e] != '"') e++;
    return json.substr(pp, e - pp);
}

int64_t EventRelationsManager::extractInt(const std::string& json, const std::string& key) {
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

bool EventRelationsManager::extractBool(const std::string& json, const std::string& key) {
    return json.find("\"" + key + "\":true") != std::string::npos;
}

// ====== Constructor ======

EventRelationsManager::EventRelationsManager() {}

// ====== Relation Parsing ======
// Original: Parse m.relates_to block from event content

RelationContent EventRelationsManager::parseRelation(const std::string& eventContentJson) {
    RelationContent rel;

    // Find m.relates_to block
    size_t pos = eventContentJson.find("\"m.relates_to\"");
    if (pos == std::string::npos) return rel;

    pos = eventContentJson.find('{', pos);
    if (pos == std::string::npos) return rel;

    int depth = 0;
    size_t start = pos;
    pos++;
    while (pos < eventContentJson.size()) {
        if (eventContentJson[pos] == '{') depth++;
        else if (eventContentJson[pos] == '}') depth--;
        if (depth == -1) break;
        pos++;
    }
    std::string relJson = eventContentJson.substr(start, pos - start);

    rel.typeString = extractStr(relJson, "rel_type");
    rel.type = relationTypeFromString(rel.typeString);
    rel.eventId = extractStr(relJson, "event_id");
    rel.option = static_cast<int>(extractInt(relJson, "option"));
    rel.isFallingBack = extractBool(relJson, "is_falling_back");

    // Parse in_reply_to
    auto replyTo = parseReplyTo(eventContentJson);
    if (replyTo.valid) rel.inReplyTo = replyTo;

    rel.valid = !rel.eventId.empty();
    return rel;
}

ReplyToContent EventRelationsManager::parseReplyTo(const std::string& eventContentJson) {
    ReplyToContent reply;

    // Find m.in_reply_to block
    size_t pos = eventContentJson.find("\"m.in_reply_to\"");
    if (pos == std::string::npos) return reply;

    pos = eventContentJson.find('{', pos);
    if (pos == std::string::npos) return reply;

    int depth = 0;
    size_t start = pos;
    pos++;
    while (pos < eventContentJson.size()) {
        if (eventContentJson[pos] == '{') depth++;
        else if (eventContentJson[pos] == '}') depth--;
        if (depth == -1) break;
        pos++;
    }
    std::string replyJson = eventContentJson.substr(start, pos - start);

    reply.eventId = extractStr(replyJson, "event_id");
    reply.valid = !reply.eventId.empty();
    return reply;
}

// ====== Relation Detection ======

bool EventRelationsManager::hasRelation(const std::string& eventContentJson) {
    return eventContentJson.find("\"m.relates_to\"") != std::string::npos ||
           eventContentJson.find("\"m.in_reply_to\"") != std::string::npos;
}

bool EventRelationsManager::isEventReply(const std::string& eventContentJson) {
    auto reply = parseReplyTo(eventContentJson);
    return reply.valid;
}

bool EventRelationsManager::isEventEdit(const std::string& eventContentJson) {
    auto rel = parseRelation(eventContentJson);
    return rel.valid && rel.type == EventRelationType::REPLACE;
}

bool EventRelationsManager::isEventReaction(const std::string& eventContentJson) {
    auto rel = parseRelation(eventContentJson);
    return rel.valid && rel.type == EventRelationType::ANNOTATION;
}

bool EventRelationsManager::isThreadRoot(const std::string& eventContentJson, const std::string& eventId) {
    auto rel = parseRelation(eventContentJson);
    return rel.valid && rel.type == EventRelationType::THREAD && rel.eventId == eventId;
}

bool EventRelationsManager::isThreadReply(const std::string& eventContentJson) {
    auto rel = parseRelation(eventContentJson);
    return rel.valid && rel.type == EventRelationType::THREAD &&
           rel.eventId != extractStr(eventContentJson, "event_id"); // Not pointing to self
}

std::string EventRelationsManager::extractThreadRoot(const std::string& eventContentJson) {
    auto rel = parseRelation(eventContentJson);
    if (rel.valid && rel.type == EventRelationType::THREAD) return rel.eventId;
    return "";
}

std::string EventRelationsManager::extractEditSource(const std::string& eventContentJson) {
    auto rel = parseRelation(eventContentJson);
    if (rel.valid && rel.type == EventRelationType::REPLACE) return rel.eventId;
    return "";
}

std::string EventRelationsManager::extractReplySource(const std::string& eventContentJson) {
    auto reply = parseReplyTo(eventContentJson);
    return reply.eventId;
}

// ====== Relation Building ======

std::string EventRelationsManager::buildReplyRelation(const std::string& eventId) {
    return R"({"m.in_reply_to":{"event_id":")" + eventId + R"("}})";
}

std::string EventRelationsManager::buildEditRelation(const std::string& eventId, bool isFallingBack) {
    std::ostringstream os;
    os << R"({"m.relates_to":{)"
       << R"("rel_type":"m.replace")"
       << R"(,"event_id":")" << eventId << R"(")";
    if (isFallingBack) os << R"(,"is_falling_back":true)";
    os << "}}";
    return os.str();
}

std::string EventRelationsManager::buildThreadRelation(const std::string& eventId, bool isFallingBack,
                                                        const std::string& replyToEventId) {
    std::ostringstream os;
    os << R"({"m.relates_to":{)"
       << R"("rel_type":"m.thread")"
       << R"(,"event_id":")" << eventId << R"(")";
    if (isFallingBack) os << R"(,"is_falling_back":true)";

    // Thread replies should also include m.in_reply_to for fallback rendering
    if (!replyToEventId.empty()) {
        os << R"(,"m.in_reply_to":{"event_id":")" << replyToEventId << R"("})";
    } else {
        os << R"(,"m.in_reply_to":{"event_id":")" << eventId << R"("})";
    }
    os << "}}";
    return os.str();
}

std::string EventRelationsManager::buildAnnotationRelation(const std::string& eventId, const std::string& key) {
    std::ostringstream os;
    os << R"({"m.relates_to":{)"
       << R"("rel_type":"m.annotation")"
       << R"(,"event_id":")" << eventId << R"(")";
    if (!key.empty()) os << R"(,"key":")" << key << R"(")";
    os << "}}";
    return os.str();
}

std::string EventRelationsManager::buildReferenceRelation(const std::string& eventId) {
    return R"({"m.relates_to":{"rel_type":"m.reference","event_id":")" + eventId + R"("}})";
}

std::string EventRelationsManager::buildReplyRelationWithThread(const std::string& replyToEventId,
                                                                 const std::string& threadRootEventId) {
    std::ostringstream os;
    os << R"({"m.relates_to":{)"
       << R"("rel_type":"m.thread")"
       << R"(,"event_id":")" << threadRootEventId << R"(")";
    os << R"(,"is_falling_back":true)";
    os << R"(,"m.in_reply_to":{"event_id":")" << replyToEventId << R"("})";
    os << "}}";
    return os.str();
}

// ====== Edit Validation ======
// Original: EventEditValidator.kt

bool EventRelationsManager::validateEdit(const std::string& originalEventJson, const std::string& replaceEventJson,
                                          std::string& error) {
    // Rule 1: Same room_id
    auto origRoom = extractStr(originalEventJson, "room_id");
    auto replRoom = extractStr(replaceEventJson, "room_id");
    if (!origRoom.empty() && !replRoom.empty() && origRoom != replRoom) {
        error = "Original and replacement must have same room_id";
        return false;
    }

    // Rule 2: Same sender
    auto origSender = extractStr(originalEventJson, "sender");
    auto replSender = extractStr(replaceEventJson, "sender");
    if (!origSender.empty() && !replSender.empty() && origSender != replSender) {
        error = "Original and replacement must have same sender";
        return false;
    }

    // Rule 3: Same event type
    auto origType = extractStr(originalEventJson, "type");
    auto replType = extractStr(replaceEventJson, "type");
    if (!origType.empty() && !replType.empty() && origType != replType) {
        error = "Replacement must have same type as original";
        return false;
    }

    // Rule 4: Neither is a state event (state events have empty string state_key or non-null)
    bool origState = originalEventJson.find("\"state_key\"") != std::string::npos;
    bool replState = replaceEventJson.find("\"state_key\"") != std::string::npos;
    if (origState || replState) {
        error = "Cannot edit state events";
        return false;
    }

    // Rule 5: Original must not already be an edit
    if (isEventEdit(originalEventJson)) {
        error = "Cannot edit an edit";
        return false;
    }

    // Rule 6: Replacement must have m.new_content
    if (replaceEventJson.find("\"m.new_content\"") == std::string::npos &&
        replaceEventJson.find("m.new_content") == std::string::npos) {
        error = "Replacement must have m.new_content";
        return false;
    }

    return true;
}

// ====== Content Building ======

std::string EventRelationsManager::buildNewContent(const std::string& body, const std::string& msgType,
                                                    const std::string& formattedBody, const std::string& format) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    std::ostringstream os;
    os << R"({"m.new_content":{)";
    os << R"("msgtype":")" << esc(msgType) << R"(")";
    os << R"(,"body":")" << esc(body) << R"(")";
    if (!formattedBody.empty()) {
        os << R"(,"formatted_body":")" << esc(formattedBody) << R"(")";
        os << R"(,"format":")" << (format.empty() ? "org.matrix.custom.html" : format) << R"(")";
    }
    os << "}}";
    return os.str();
}

// ====== Serialization ======

std::string EventRelationsManager::relationToJson(const RelationContent& rel) const {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    std::ostringstream os;
    os << R"({"type":")" << rel.typeString
       << R"(","rel_type":")" << relationTypeToString(rel.type)
       << R"(","event_id":")" << esc(rel.eventId)
       << R"(","is_reply":)" << (rel.isReply() ? "true" : "false")
       << R"(,"is_edit":)" << (rel.isEdit() ? "true" : "false")
       << R"(,"is_thread":)" << (rel.isThread() ? "true" : "false")
       << R"(,"is_annotation":)" << (rel.isAnnotation() ? "true" : "false");
    if (rel.inReplyTo.valid) os << R"(,"reply_to":")" << esc(rel.inReplyTo.eventId) << R"(")";
    if (rel.option >= 0) os << R"(,"option":)" << rel.option;
    os << "}";
    return os.str();
}

} // namespace progressive
