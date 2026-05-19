#include "progressive/space_utils.hpp"
#include "progressive/json_parser.hpp"
#include "progressive/space_graph.hpp"   // For SpaceChildWithOrder
#include <sstream>
#include <algorithm>

namespace progressive {

SpaceInfo parseSpaceInfo(const std::string& roomId, const std::string& stateEventsJson) {
    SpaceInfo info;
    info.spaceId = roomId;

    // Parse from state events (m.room.name, m.room.topic, etc.)
    info.name  = parseJsonStringValue(stateEventsJson, "name");
    info.topic = parseJsonStringValue(stateEventsJson, "topic");

    auto joinRule = parseJsonStringValue(stateEventsJson, "join_rule");
    info.isPublic = (joinRule == "public");

    auto avatarUrl = parseJsonStringValue(stateEventsJson, "url");
    if (!avatarUrl.empty()) info.avatarUrl = avatarUrl;

    return info;
}

std::vector<SpaceChild> parseSpaceChildren(const std::string& stateEventsJson) {
    std::vector<SpaceChild> children;

    size_t pos = 0;
    while (true) {
        pos = stateEventsJson.find("\"m.space.child\"", pos);
        if (pos == std::string::npos) break;

        // Find the state event object
        auto objStart = stateEventsJson.rfind('{', pos);
        if (objStart == std::string::npos) break;

        int depth = 0;
        auto objEnd = objStart;
        while (objEnd < stateEventsJson.size()) {
            if (stateEventsJson[objEnd] == '{') ++depth;
            else if (stateEventsJson[objEnd] == '}') --depth;
            if (depth == 0) break;
            ++objEnd;
        }
        if (objEnd >= stateEventsJson.size()) break;

        std::string obj = stateEventsJson.substr(objStart, objEnd - objStart + 1);

        SpaceChild child;
        child.childId = parseJsonStringValue(obj, "state_key");
        child.isRoom = (child.childId[0] == '!');

        auto content = parseJsonStringValue(obj, "content");
        if (!content.empty()) {
            std::string wrapped = "{" + content + "}";
            child.order       = parseJsonStringValue(wrapped, "order");
            child.isSuggested = (parseJsonStringValue(wrapped, "suggested") == "true");
            child.name        = parseJsonStringValue(wrapped, "name");
        }

        if (!child.childId.empty()) children.push_back(child);
        pos = objEnd + 1;
    }

    return children;
}

void sortSpaceChildren(std::vector<SpaceChild>& children) {
    std::sort(children.begin(), children.end(), [](const SpaceChild& a, const SpaceChild& b) {
        if (!a.order.empty() && !b.order.empty()) return a.order < b.order;
        if (!a.order.empty()) return true;  // ordered items first
        if (!b.order.empty()) return false;
        return a.name < b.name;
    });
}

std::vector<SpaceChild> filterSpaceChildren(const std::vector<SpaceChild>& children,
    bool roomsOnly, bool spacesOnly, bool suggestedOnly) {
    std::vector<SpaceChild> result;
    for (const auto& c : children) {
        if (roomsOnly && !c.isRoom) continue;
        if (spacesOnly && c.isRoom) continue;
        if (suggestedOnly && !c.isSuggested) continue;
        result.push_back(c);
    }
    return result;
}

std::vector<SpaceChild> searchSpaceChildren(const std::vector<SpaceChild>& children,
    const std::string& query) {
    if (query.empty()) return children;
    auto lowerQuery = query;
    std::transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);

    std::vector<SpaceChild> result;
    for (const auto& c : children) {
        auto lowerName = c.name;
        auto lowerId = c.childId;
        std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
        std::transform(lowerId.begin(), lowerId.end(), lowerId.begin(), ::tolower);
        if (lowerName.find(lowerQuery) != std::string::npos ||
            lowerId.find(lowerQuery) != std::string::npos) {
            result.push_back(c);
        }
    }
    return result;
}

std::string formatSpaceInfo(const SpaceInfo& info) {
    std::ostringstream out;
    out << info.name << (info.isPublic ? " (Public)" : " (Private)") << "\n";
    out << "Rooms: " << info.childRoomCount << " | Sub-spaces: " << info.childSpaceCount << "\n";
    if (!info.topic.empty()) out << info.topic << "\n";
    return out.str();
}

std::string formatSpaceTree(const SpaceTree& tree) {
    std::ostringstream out;
    out << "Space: " << tree.rootSpaceName << "\n";
    out << "Total spaces: " << tree.totalSpaces << "\n";
    out << "Total rooms: " << tree.totalRooms << "\n";
    if (!tree.orphanRooms.empty()) {
        out << "Rooms not in any space: " << tree.orphanRooms.size() << "\n";
    }
    return out.str();
}

std::string buildSpaceChildContent(bool suggested, const std::string& order,
    bool autoJoin, bool canonical) {
    std::ostringstream json;
    json << "{";
    json << R"("via": [],)";
    if (suggested) json << R"("suggested": true,)";
    if (!order.empty()) json << R"("order": ")" << order << R"(",)";
    if (autoJoin) json << R"("auto_join": true,)";
    if (canonical) json << R"("canonical": true,)";
    // Remove trailing comma
    std::string result = json.str();
    if (result.back() == ',') result.pop_back();
    result += "}";
    return result;
}

std::string buildSpaceParentContent(const std::string& parentSpaceId, bool canonical) {
    std::ostringstream json;
    json << "{";
    json << R"("via": [],)";
    if (canonical) json << R"("canonical": true,)";
    json << R"("parent": ")" << parentSpaceId << R"(")";
    json << "}";
    return json.str();
}

// ================================================================
// Room filter helpers
// Original Kotlin: SpaceFilter.kt, SpaceListViewModel filtering
// ================================================================

std::vector<std::string> filterRoomsBySpace(
    const std::vector<std::string>& roomIds,
    const SpaceRoomFilter& filter,
    const std::unordered_map<std::string, std::vector<std::string>>& flattenParentIds)
{
    // Original Kotlin: spaceFilterActiveSpace → filter by flattenParentIds containing spaceId
    // Original Kotlin: spaceFilterExcludeSpace → exclude those containing spaceId
    // Original Kotlin: spaceFilterOrphanRooms → rooms with empty flattenParentIds
    if (filter.type == SpaceRoomFilter::NONE) return roomIds;

    std::vector<std::string> result;

    for (const auto& roomId : roomIds) {
        auto it = flattenParentIds.find(roomId);
        const auto& parents = (it != flattenParentIds.end()) ? it->second
            : std::vector<std::string>{};

        switch (filter.type) {
            case SpaceRoomFilter::ACTIVE_SPACE: {
                // Include if the specific space is in the room's flattenParentIds
                if (std::find(parents.begin(), parents.end(), filter.spaceId) != parents.end()) {
                    result.push_back(roomId);
                }
                break;
            }
            case SpaceRoomFilter::EXCLUDE_SPACE: {
                // Include if the specific space is NOT in the room's flattenParentIds
                if (std::find(parents.begin(), parents.end(), filter.spaceId) == parents.end()) {
                    result.push_back(roomId);
                }
                break;
            }
            case SpaceRoomFilter::ORPHAN: {
                // Include only rooms with no flattenParentIds (not in any space)
                if (parents.empty()) {
                    result.push_back(roomId);
                }
                break;
            }
            case SpaceRoomFilter::NONE:
            default:
                result.push_back(roomId);
                break;
        }
    }

    return result;
}

// ================================================================
// Suggested rooms / orphaned rooms
// Original Kotlin: SpaceListViewModel, RoomListViewModel filtering
// ================================================================

std::vector<std::string> getSuggestedRooms(
    const std::unordered_map<std::string, std::vector<SpaceChildWithOrder>>& spaceChildren)
{
    // Original Kotlin: collect all childRoomIds where suggested=true
    std::vector<std::string> result;
    for (const auto& [spaceId, children] : spaceChildren) {
        for (const auto& child : children) {
            if (child.suggested) {
                // De-duplicate
                if (std::find(result.begin(), result.end(), child.childRoomId) == result.end()) {
                    result.push_back(child.childRoomId);
                }
            }
        }
    }
    return result;
}

std::vector<std::string> getOrphanedRooms(
    const std::unordered_map<std::string, std::vector<std::string>>& allFlattenParentIds,
    const std::unordered_set<std::string>& spaceRoomIds)
{
    // Original Kotlin: rooms that have no parent space
    // (i.e., flattenParentIds is empty or doesn't exist, and it's not a space itself)
    std::vector<std::string> result;
    for (const auto& [roomId, parentIds] : allFlattenParentIds) {
        if (parentIds.empty() && spaceRoomIds.find(roomId) == spaceRoomIds.end()) {
            result.push_back(roomId);
        }
    }
    return result;
}

// ================================================================
// Space child / parent event builders — full event JSON
// Original Kotlin: Space.kt addChildren/removeChildren, DefaultSpaceService.kt setSpaceParent
// ================================================================

std::string buildSpaceChildEvent(const std::string& stateKey,
                                 bool suggested,
                                 const std::string& order,
                                 const std::vector<std::string>& via)
{
    // Original Kotlin: SpaceChildContent → toContent() → sendStateEvent
    // PUT /_matrix/client/r0/rooms/{roomId}/state/m.space.child/{stateKey}
    // Event JSON: {"type":"m.space.child","state_key":"!child:org","content":{...}}
    std::ostringstream json;
    json << R"({"type":"m.space.child","state_key":")";
    // Escape stateKey
    for (char c : stateKey) {
        if (c == '"') json << "\\\"";
        else if (c == '\\') json << "\\\\";
        else json << c;
    }
    json << R"(","content":{)";

    // via
    json << R"("via":[)";
    for (size_t i = 0; i < via.size(); i++) {
        if (i > 0) json << ",";
        json << R"(")";
        for (char c : via[i]) {
            if (c == '"') json << "\\\"";
            else json << c;
        }
        json << R"(")";
    }
    json << "]";

    // suggested
    if (suggested) {
        json << R"(,"suggested":true)";
    }

    // order (validated: ASCII \x20-\x7E, max 50 chars)
    if (!order.empty()) {
        std::string validOrder = parseSpaceChildOrder(order);
        if (!validOrder.empty()) {
            json << R"(,"order":")";
            for (char c : validOrder) {
                if (c == '"') json << "\\\"";
                else if (c == '\\') json << "\\\\";
                else json << c;
            }
            json << R"(")";
        }
    }

    json << "}}";
    return json.str();
}

std::string buildSpaceParentEvent(const std::string& parentSpaceId,
                                  bool canonical,
                                  const std::vector<std::string>& via)
{
    // Original Kotlin: SpaceParentContent → toContent() → sendStateEvent
    // PUT /_matrix/client/r0/rooms/{roomId}/state/m.space.parent/{parentSpaceId}
    std::ostringstream json;
    json << R"({"type":"m.space.parent","state_key":")";
    for (char c : parentSpaceId) {
        if (c == '"') json << "\\\"";
        else if (c == '\\') json << "\\\\";
        else json << c;
    }
    json << R"(","content":{)";

    // via
    json << R"("via":[)";
    for (size_t i = 0; i < via.size(); i++) {
        if (i > 0) json << ",";
        json << R"(")";
        for (char c : via[i]) {
            if (c == '"') json << "\\\"";
            else json << c;
        }
        json << R"(")";
    }
    json << "]";

    // canonical
    if (canonical) {
        json << R"(,"canonical":true)";
    }

    json << "}}";
    return json.str();
}

// ================================================================
// Space child / parent event parsers — full event JSON → structured data
// Original Kotlin: SpaceChildSummaryEvent.kt, SpaceChildContent.kt, SpaceParentContent.kt
// ================================================================

SpaceChildWithOrder parseSpaceChildEvent(const std::string& eventJson) {
    // Original Kotlin: SpaceChildSummaryEvent → content.toModel<SpaceChildContent>()
    // Event JSON: {"type":"m.space.child","state_key":"!room:org","content":{...},"sender":"@user:org",...}
    SpaceChildWithOrder result;

    result.childRoomId = parseJsonStringValue(eventJson, "state_key");
    result.order = parseSpaceChildOrder(parseJsonStringValue(eventJson, "order"));
    result.suggested = parseJsonBoolValue(eventJson, "suggested", false);

    // Parse via array from content
    // Look for "via":[ ... ] in the JSON
    size_t viaPos = eventJson.find("\"via\"");
    if (viaPos != std::string::npos) {
        size_t bracket = eventJson.find('[', viaPos);
        if (bracket != std::string::npos) {
            bracket++;
            while (bracket < eventJson.size()) {
                while (bracket < eventJson.size()
                    && (eventJson[bracket] == ' ' || eventJson[bracket] == ',' || eventJson[bracket] == '\n')) {
                    bracket++;
                }
                if (bracket >= eventJson.size() || eventJson[bracket] == ']') break;
                if (eventJson[bracket] == '"') {
                    bracket++;
                    size_t endQuote = bracket;
                    while (endQuote < eventJson.size() && eventJson[endQuote] != '"') endQuote++;
                    result.viaServers.push_back(eventJson.substr(bracket, endQuote - bracket));
                    bracket = endQuote + 1;
                } else {
                    // Skip non-string values
                    while (bracket < eventJson.size()
                        && eventJson[bracket] != ',' && eventJson[bracket] != ']') bracket++;
                }
            }
        }
    }

    return result;
}

std::pair<std::string, SpaceParentEntry> parseSpaceParentEvent(const std::string& eventJson) {
    // Original Kotlin: stateKey = parent space ID, content → SpaceParentContent
    // Event JSON: {"type":"m.space.parent","state_key":"!space:org","content":{...},"sender":"@user:org",...}
    std::string spaceId = parseJsonStringValue(eventJson, "state_key");

    SpaceParentEntry entry;
    entry.spaceId = spaceId;
    entry.canonical = parseJsonBoolValue(eventJson, "canonical", false);

    // Parse via array from content
    size_t viaPos = eventJson.find("\"via\"");
    if (viaPos != std::string::npos) {
        size_t bracket = eventJson.find('[', viaPos);
        if (bracket != std::string::npos) {
            bracket++;
            while (bracket < eventJson.size()) {
                while (bracket < eventJson.size()
                    && (eventJson[bracket] == ' ' || eventJson[bracket] == ',' || eventJson[bracket] == '\n')) {
                    bracket++;
                }
                if (bracket >= eventJson.size() || eventJson[bracket] == ']') break;
                if (eventJson[bracket] == '"') {
                    bracket++;
                    size_t endQuote = bracket;
                    while (endQuote < eventJson.size() && eventJson[endQuote] != '"') endQuote++;
                    entry.via.push_back(eventJson.substr(bracket, endQuote - bracket));
                    bracket = endQuote + 1;
                } else {
                    while (bracket < eventJson.size()
                        && eventJson[bracket] != ',' && eventJson[bracket] != ']') bracket++;
                }
            }
        }
    }

    return {spaceId, entry};
}

} // namespace progressive
