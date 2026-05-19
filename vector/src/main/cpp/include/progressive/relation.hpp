#ifndef PROGRESSIVE_RELATION_HPP
#define PROGRESSIVE_RELATION_HPP

#include <string>
#include <optional>

namespace progressive {

struct RelationInfo {
    std::string sourceEventId;    // the event this one relates to
    std::string relationType;      // "m.annotation", "m.replace", "m.reference"
    bool isRelation = false;
};

// Parse a Matrix event's "m.relates_to" JSON to find the source event ID.
// Input: raw JSON of the event's content (or full event).
// Extracts: event_id from m.relates_to block.
RelationInfo parseRelation(const std::string& eventJson);

// Check if a relation type is one we support for jumptodate-like navigation.
bool isJumpableRelationType(const std::string& relationType);
bool isJumpableRelationType(const char* relationType);

} // namespace progressive

#endif // PROGRESSIVE_RELATION_HPP
