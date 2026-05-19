#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// ================================================================
// Event Relations Manager — full event relation handling
//
// Faithful port from Element Android original sources:
//   EventRelationType.kt — ANNOTATION, REPLACE, REFERENCE, THREAD, RESPONSE
//   RelationContent.kt — type, eventId, inReplyTo, option, isFallingBack
//   ReplyToContent.kt — eventId
//   RelationDefaultContent.kt — all fields, shouldRenderInThread(),
//     isReply() (checks inReplyTo.eventId != null)
//   EventEditValidator.kt — validate edit, same room/sender/type
//   SendService.kt — sendQuotedTextMessage, relatesTo parameter
//
// Covers:
//   1. Relation type detection (annotation/replace/reference/thread)
//   2. Reply detection and extraction
//   3. Edit (m.replace) detection and validation
//   4. Thread root detection (m.thread)
//   5. Reaction (annotation) detection
//   6. Build reply relation content
//   7. Build edit relation content
//   8. Build thread relation content
//   9. Build annotation/content relation
// ================================================================

// ---- Relation Type ----
// Original: EventRelationType.kt (ANNOTATION, REPLACE, REFERENCE, THREAD, RESPONSE)

enum class EventRelationType {
    UNKNOWN = 0,
    ANNOTATION = 1,       // m.annotation — reactions, etc.
    REPLACE = 2,          // m.replace — edit events
    REFERENCE = 3,        // m.reference — generic reference
    THREAD = 4,           // m.thread — thread replies
    RESPONSE = 5,         // org.matrix.response
};

const char* relationTypeToString(EventRelationType type);
EventRelationType relationTypeFromString(const std::string& s);

// ---- Reply-To Content ----
// Original: ReplyToContent.kt (event_id)

#ifndef PROGRESSIVE_REPLY_TO_DEFINED
#define PROGRESSIVE_REPLY_TO_DEFINED
struct ReplyToContent {
    std::string eventId;
    bool valid = false;
};
#endif

// ---- Relation Content ----
// Original: RelationContent.kt (type, eventId, inReplyTo, option, isFallingBack)
// Original: RelationDefaultContent.kt (all fields, shouldRenderInThread, isReply)

#ifndef PROGRESSIVE_RELATION_CONTENT_DEFINED
#define PROGRESSIVE_RELATION_CONTENT_DEFINED
struct RelationContent {
    EventRelationType type = EventRelationType::UNKNOWN;
    std::string typeString;          // Raw type string
    std::string eventId;             // Target event ID
    ReplyToContent inReplyTo;        // Reply target
    int option = -1;                 // For poll responses etc.
    bool isFallingBack = false;      // Render as reply fallback (when false)
    bool valid = false;

    // Original: shouldRenderInThread() — render in thread if NOT falling back
    bool shouldRenderInThread() const { return isFallingBack == false; }

    // Original: isReply() — checks inReplyTo.eventId != null
    bool isReply() const { return !inReplyTo.eventId.empty(); }

    // Check if this is an edit (m.replace).
    bool isEdit() const { return type == EventRelationType::REPLACE; }

    // Check if this is a thread relation.
    bool isThread() const { return type == EventRelationType::THREAD; }

    // Check if this is an annotation (reaction).
    bool isAnnotation() const { return type == EventRelationType::ANNOTATION; }

    // Check if this is a reference.
    bool isReference() const { return type == EventRelationType::REFERENCE; }
};
#endif

// ---- Event Relations Manager ----

class EventRelationsManager {
public:
    EventRelationsManager();

    // ====== Relation Parsing ======
    // Original: Parse m.relates_to from event content JSON

    // Parse relation content from event JSON.
    RelationContent parseRelation(const std::string& eventContentJson);

    // Parse reply-to content (m.in_reply_to) from event JSON.
    ReplyToContent parseReplyTo(const std::string& eventContentJson);

    // ====== Relation Detection ======
    // Original: EventRelationType constants

    // Check if an event has a relation of any type.
    bool hasRelation(const std::string& eventContentJson);

    // Check if an event is a reply (has m.in_reply_to with event_id).
    bool isEventReply(const std::string& eventContentJson);

    // Check if an event is an edit (has m.relates_to with rel_type=m.replace).
    bool isEventEdit(const std::string& eventContentJson);

    // Check if an event is a reaction (m.annotation).
    bool isEventReaction(const std::string& eventContentJson);

    // Check if an event is a thread root (m.thread rel_type pointing to self).
    bool isThreadRoot(const std::string& eventContentJson, const std::string& eventId);

    // Check if an event is a thread reply.
    bool isThreadReply(const std::string& eventContentJson);

    // Extract thread root event ID from content.
    std::string extractThreadRoot(const std::string& eventContentJson);

    // Extract edit source event ID.
    std::string extractEditSource(const std::string& eventContentJson);

    // Extract reply source event ID.
    std::string extractReplySource(const std::string& eventContentJson);

    // ====== Relation Building ======
    // Original: SendService sendQuotedTextMessage, sendMedia with relatesTo

    // Build m.in_reply_to content JSON.
    std::string buildReplyRelation(const std::string& eventId);

    // Build m.replace relation content JSON.
    std::string buildEditRelation(const std::string& eventId, bool isFallingBack = false);

    // Build m.thread relation content JSON.
    std::string buildThreadRelation(const std::string& eventId, bool isFallingBack = false,
                                     const std::string& replyToEventId = "");

    // Build m.annotation relation content JSON.
    std::string buildAnnotationRelation(const std::string& eventId, const std::string& key = "");

    // Build m.reference relation content JSON.
    std::string buildReferenceRelation(const std::string& eventId);

    // Build combined reply + thread relation.
    std::string buildReplyRelationWithThread(const std::string& replyToEventId,
                                              const std::string& threadRootEventId);

    // ====== Edit Validation ======
    // Original: EventEditValidator.kt — validate edit

    // Validate that an edit is allowed per Matrix spec:
    // 1. Same room_id
    // 2. Same sender
    // 3. Same event type
    // 4. Neither is a state event
    // 5. Original must not already be an edit
    // 6. Replacement must have m.new_content
    bool validateEdit(const std::string& originalEventJson, const std::string& replaceEventJson,
                       std::string& error);

    // ====== Content Building ======

    // Build m.new_content for an edit event.
    std::string buildNewContent(const std::string& body, const std::string& msgType,
                                 const std::string& formattedBody = "",
                                 const std::string& format = "");

    // ====== Serialization ======

    // Export relation content as JSON.
    std::string relationToJson(const RelationContent& rel) const;

private:
    static std::string extractStr(const std::string& json, const std::string& key);
    static int64_t extractInt(const std::string& json, const std::string& key);
    static bool extractBool(const std::string& json, const std::string& key);
};

} // namespace progressive
