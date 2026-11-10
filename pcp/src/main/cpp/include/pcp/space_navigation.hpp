#pragma once
#include <string>
#include <vector>

namespace pcp {

struct SpaceNode {
    std::string roomId;
    std::string name;
    std::string topic;
    std::string avatarUrl;
    std::string type;           // "m.space" or room type
    int depth = 0;
    bool isJoined = false;
    bool isSuggested = true;
    std::vector<std::string> children;
};

struct SpaceHierarchy {
    std::string rootId;
    std::vector<SpaceNode> spaces;
    std::vector<SpaceNode> rooms;
    int totalSpaces = 0;
    int totalRooms = 0;
};

// Parse space child event (m.space.child)
SpaceNode parseSpaceChild(const std::string& stateKey, const std::string& contentJson);

// Parse space parent event (m.space.parent)
std::string parseSpaceParent(const std::string& contentJson);

// Build space hierarchy from state events
SpaceHierarchy buildSpaceHierarchy(const std::string& spaceId,
                                     const std::vector<std::string>& childKeys,
                                     const std::vector<std::string>& childContents,
                                     const std::vector<std::string>& roomNames = {});

// Format space breadcrumb navigation
std::string formatSpaceBreadcrumb(const std::vector<std::string>& spaceNames);

// Format space suggestion text
std::string formatSpaceSuggestion(const SpaceNode& node);

// Check if a room is in a space
bool isRoomInSpace(const SpaceHierarchy& hierarchy, const std::string& roomId);

} // namespace pcp
