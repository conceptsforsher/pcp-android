#ifndef PROGRESSIVE_EVENT_RELATIONS_HPP
#define PROGRESSIVE_EVENT_RELATIONS_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <optional>
#include <cstdint>

namespace progressive {

// ---- Event Relations (Threads, Replies, Edits) ----

struct EventRelationInfo {
    std::string relType;           // "m.thread", "m.reference", "m.replace", "m.annotation"
    std::string eventId;           // the event this relation points to
    std::string key;               // for m.annotation: the emoji key
    std::string fallback;          // for m.replace: new body text
    int count = 0;                 // for m.annotation: reaction count
    int currentIndex = 0;          // for threaded relations
    bool isFallingBack = false;    // for m.replace: show old body?
};

// Parse relation info from event content JSON.
EventRelationInfo parseEventRelation(const std::string& contentJson);

// Check if an event is part of a thread.
bool isThreadRoot(const std::string& contentJson);

// Check if an event is a reply (m.reference).
bool isReply(const std::string& contentJson);

// Check if an event is an edit (m.replace).
bool isEdit(const std::string& contentJson);

// Check if an event is a reaction (m.annotation).
bool isReaction(const std::string& contentJson);

// Extract the thread root event ID from content.
std::string extractThreadRoot(const std::string& contentJson);

// Extract the reply source event ID from content.
std::string extractReplySource(const std::string& contentJson);

// Extract the edit source event ID from content.
std::string extractEditSource(const std::string& contentJson);

// Build relation content for a reply.
std::string buildReplyRelationWithThread(const std::string& eventId, const std::string& threadRoot = "");

// Build relation content for a thread reply.
std::string buildThreadRelation(const std::string& threadRoot);

// Build relation content for an edit.
std::string buildEditRelation(const std::string& eventId);

// Format relation as a human-readable description.
std::string formatRelationDescription(const EventRelationInfo& relation);

// ---- Thread Summary ----

struct ThreadSummary {
    std::string rootEventId;
    std::string rootMessageBody;       // first message in thread
    std::string rootSender;
    int replyCount = 0;
    std::string lastReplyBody;
    std::string lastReplySender;
    int64_t lastReplyTs = 0;
    int participantCount = 0;
    bool unread = false;
};

// Compute a thread summary from event data.
ThreadSummary computeThreadSummary(
    const std::string& rootEventId,
    const std::string& rootBody,
    const std::string& rootSender,
    const std::vector<std::string>& replyBodies,
    const std::vector<std::string>& replySenders,
    const std::vector<int64_t>& replyTimestamps,
    bool hasUnread
);

// Format thread summary as JSON.
std::string threadSummaryToJson(const ThreadSummary& summary);

// Build thread list from a JSON array of events.
std::string buildThreadListJson(const std::string& eventsJson);

// ==== Thread Unread Counter ====

struct ThreadUnreadCount {
    int totalReplies = 0;
    int unreadReplies = 0;       // replies after read receipt
    int highlightReplies = 0;    // replies with @mention
    bool hasUnread = false;
};

// Compute unread counts for a thread.
// eventIds: ordered list of event IDs in the thread (oldest first)
// readReceiptEventId: the last event the user has read (empty = none read)
// highlightIds: event IDs that should be highlighted (mentions)
ThreadUnreadCount computeThreadUnreadCount(
    const std::vector<std::string>& eventIds,
    const std::string& readReceiptEventId,
    const std::vector<std::string>& highlightIds);

// ==== Expanded Relation Model (ported from Kotlin SDK) ====

// Original Kotlin: Minimal event data for aggregation
struct MinimalEvent {
    std::string eventId;
    std::string type;            // "m.room.message", "m.reaction", etc.
    std::string senderId;
    int64_t originServerTs = 0;
    std::string contentJson;    // raw JSON content string
    bool isLocalEcho = false;
};

// ---- Server-side Aggregation Structs ----

// Original Kotlin: AggregatedAnnotation (reaction count chunk from server)
struct AggregatedAnnotation {
    std::string key;                           // reaction emoji key ("👍")
    int count = 0;                             // count for this key
    std::vector<std::string> sourceEvents;     // event IDs of reactions
};

// Original Kotlin: AggregatedReplace (latest edit info from server)
struct AggregatedReplace {
    std::string eventId;                       // latest replacement event ID
    int64_t originServerTs = 0;                // timestamp of latest replacement
    std::string senderId;                      // sender of latest replacement
};

// Original Kotlin: reference event info
struct AggregatedReference {
    std::string eventId;
    int64_t originServerTs = 0;
    std::string senderId;
    std::string type;                          // event type of the reference
};

// Original Kotlin: AggregatedRelationsData (client-side full aggregation)
struct AggregatedRelationsData {
    std::unordered_map<std::string, AggregatedAnnotation> annotations; // key -> annotation
    std::optional<AggregatedReplace> replace;
    std::vector<AggregatedReference> references;
};

// ---- Relation Content (expanded from Kotlin) ----

// Original Kotlin: RelationDefaultContent + ReactionInfo
struct RelationDefaultContent {
    std::string type;                          // rel_type: "m.annotation", "m.replace", "m.reference", "m.thread"
    std::string eventId;                       // target event ID
    std::string key;                           // for m.annotation: the reaction key
    std::string inReplyToEventId;              // m.in_reply_to.event_id (for replies)
    int option = 0;                            // optional integer (e.g. for polls)
    bool isFallingBack = false;                // whether this is a fallback render
};

// ---- Verification State (for references aggregation) ----

// Original Kotlin: VerificationState enum
enum class VerificationState {
    REQUEST,
    WAITING,
    CANCELED_BY_ME,
    CANCELED_BY_OTHER,
    DONE
};

// ---- Edit Aggregation ----

// Original Kotlin: EditionOfEvent (single edition entry)
struct EditionOfEvent {
    std::string eventId;
    int64_t timestamp = 0;
    bool isLocalEcho = false;
};

// Original Kotlin: EditAggregatedSummary
struct EditAggregationInfo {
    std::string latestEditEventId;             // event_id of the latest edit
    std::string latestEditSenderId;            // sender of the latest edit
    int64_t lastEditTs = 0;                   // timestamp of the latest edit
    int editCount = 0;                         // total number of edits
    std::vector<EditionOfEvent> editions;     // all edition entries
};

// ---- Reaction Aggregation ----

// Original Kotlin: ReactionAggregatedSummary
struct ReactionAggregationInfo {
    std::string key;                           // reaction emoji key ("👍")
    int count = 0;                             // total count for this key
    bool addedByMe = false;                    // did the current user add this reaction?
    int64_t firstTimestamp = 0;                // timestamp of the first reaction
    std::vector<std::string> sourceEvents;    // event IDs (remote reactions)
    std::vector<std::string> localEchoEvents; // local echo event IDs / txIds
};

// ---- References Aggregation (MSC3912) ----

// Original Kotlin: ReferencesAggregatedContent
struct ReferencesAggregatedContent {
    VerificationState verificationState = VerificationState::REQUEST;
};

// Original Kotlin: ReferencesAggregatedSummary
struct ReferencesAggregatedSummary {
    std::string contentJson;                   // serialized aggregated content
    std::vector<std::string> sourceEvents;    // event IDs (remote references)
    std::vector<std::string> localEchoEventIds; // local echo event IDs / txIds
};

// ---- Event Annotations Summary (full aggregation snapshot) ----

// Original Kotlin: EventAnnotationsSummary
struct EventAnnotationsSummary {
    std::vector<ReactionAggregationInfo> reactionsSummary;
    std::optional<EditAggregationInfo> editSummary;
    std::optional<ReferencesAggregatedSummary> referencesSummary;
};

// ---- Relation Aggregation Functions ----

// Original Kotlin: aggregateRelations — compute all relation types from a list of events
AggregatedRelationsData aggregateRelations(
    const std::vector<MinimalEvent>& relatedEvents,
    const std::string& currentUserId = ""
);

// Original Kotlin: computeAggregatedAnnotations — compute reaction counts per key
std::unordered_map<std::string, AggregatedAnnotation> computeAggregatedAnnotations(
    const std::vector<MinimalEvent>& events
);

// Original Kotlin: computeAggregatedReplace — find latest edit in chain
// Sorted by originServerTs then eventId (lexicographically largest = most recent in tie)
std::optional<AggregatedReplace> computeAggregatedReplace(
    const std::vector<MinimalEvent>& events
);

// Original Kotlin: computeAggregatedReferences — collect m.reference events
std::vector<AggregatedReference> computeAggregatedReferences(
    const std::vector<MinimalEvent>& events
);

// ---- Relation Content Parsing / Building ----

// Original Kotlin: getRelationContent() -> RelationDefaultContent
// Extract m.relates_to fields from event content JSON.
RelationDefaultContent parseRelationContent(const std::string& contentJson);

// Original Kotlin: builds m.relates_to content JSON for various relation types
std::string buildRelationContent(
    const std::string& relType,
    const std::string& eventId,
    const std::string& key = "",
    const std::string& inReplyToEventId = "",
    bool isFallingBack = false
);

// ---- Event Edit / Reaction Processing ----

// Original Kotlin: computeEditAggregation — from EditAggregatedSummary.kt logic
// Finds the latest edit by comparing originServerTs then eventId.
EditAggregationInfo computeEditAggregation(
    const std::vector<MinimalEvent>& editEvents
);

// Original Kotlin: computeReactionAggregation — from ReactionAggregatedSummary.kt logic
// Groups reactions by key, tracks counts, addedByMe, source events.
std::vector<ReactionAggregationInfo> computeReactionAggregation(
    const std::vector<MinimalEvent>& reactionEvents,
    const std::string& currentUserId = ""
);

// ---- Event Annotations Summary Build / Parse ----

// Original Kotlin: buildEventAnnotationsSummary — construct from components
EventAnnotationsSummary buildEventAnnotationsSummary(
    const std::vector<ReactionAggregationInfo>& reactions,
    const std::optional<EditAggregationInfo>& edit,
    const std::optional<ReferencesAggregatedSummary>& refs
);

// Serialize EventAnnotationsSummary to JSON
std::string eventAnnotationsSummaryToJson(const EventAnnotationsSummary& summary);

// Parse EventAnnotationsSummary from JSON
EventAnnotationsSummary parseEventAnnotationsSummary(const std::string& json);

// ---- References Aggregation (MSC3912) ----

// Original Kotlin: parseReferencesAggregatedContent — parse verification state
ReferencesAggregatedContent parseReferencesAggregatedContent(const std::string& contentJson);

} // namespace progressive

#endif // PROGRESSIVE_EVENT_RELATIONS_HPP
