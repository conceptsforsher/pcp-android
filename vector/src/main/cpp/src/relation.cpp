#include "progressive/relation.hpp"
#include "progressive/json_parser.hpp"

namespace progressive {

RelationInfo parseRelation(const std::string& eventJson) {
    RelationInfo info;

    // Extract the "content" block first, then look for "m.relates_to"
    auto contentBlock = progressive::parseJsonStringValue(eventJson, "content");
    std::string searchIn;

    if (!contentBlock.empty()) {
        // The event has a top-level "content" key — parse within it
        searchIn = contentBlock;
    } else {
        // For events where m.relates_to is at the top level (some edge cases)
        searchIn = eventJson;
    }

    // Find "m.relates_to" nested block — it's a JSON object containing event_id and rel_type
    std::string searchKey = R"("m.relates_to")";
    auto pos = searchIn.find(searchKey);
    if (pos == std::string::npos) {
        // Try with "relates_to" (some APIs omit the m. prefix)
        searchKey = R"("relates_to")";
        pos = searchIn.find(searchKey);
    }

    if (pos == std::string::npos) return info;

    pos += searchKey.size();

    // Skip whitespace and colon
    while (pos < searchIn.size() && (searchIn[pos] == ' ' || searchIn[pos] == '\t' ||
           searchIn[pos] == '\n' || searchIn[pos] == '\r'))
        ++pos;
    if (pos >= searchIn.size() || searchIn[pos] != ':') return info;
    ++pos;

    // Extract the relates_to JSON object
    // We need to find the matching closing brace
    int depth = 0;
    auto start = pos;
    while (start < searchIn.size() && (searchIn[start] == ' ' || searchIn[start] == '\t' ||
           searchIn[start] == '\n' || searchIn[start] == '\r'))
        ++start;
    if (start >= searchIn.size() || searchIn[start] != '{') return info;

    auto objStart = start;
    ++start;
    ++depth;
    auto end = start;
    while (end < searchIn.size() && depth > 0) {
        if (searchIn[end] == '{') ++depth;
        else if (searchIn[end] == '}') --depth;
        ++end;
    }

    std::string relatesToBlock = searchIn.substr(objStart, end - objStart);

    // Extract event_id from the relates_to block
    info.sourceEventId = progressive::parseJsonStringValue(relatesToBlock, "event_id");
    info.relationType = progressive::parseJsonStringValue(relatesToBlock, "rel_type");

    info.isRelation = !info.sourceEventId.empty();
    return info;
}

bool isJumpableRelationType(const std::string& relationType) {
    return relationType == "m.annotation"       // reaction
        || relationType == "m.reference"        // reply
        || relationType == "m.replace"          // edit
        || relationType == "m.thread";          // thread root
}

bool isJumpableRelationType(const char* relationType) {
    return isJumpableRelationType(std::string(relationType));
}

} // namespace progressive
