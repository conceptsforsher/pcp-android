#include "progressive/space_utils.hpp"
#include "progressive/json_parser.hpp"
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

} // namespace progressive
