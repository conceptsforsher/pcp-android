#include "pcp/room_preview.hpp"
#include <nlohmann/json.hpp>
#include <sstream>

namespace pcp {

static std::string extractField(const std::string& json, const std::string& key) {
    auto p = json.find("\"" + key + "\":\"");
    if (p == std::string::npos) return "";
    p += key.size() + 4;
    auto e = json.find('"', p);
    if (e == std::string::npos) return "";
    return json.substr(p, e - p);
}
static int extractInt(const std::string& json, const std::string& key) {
    auto p = json.find("\"" + key + "\":");
    if (p == std::string::npos) return 0;
    p += key.size() + 2;
    while (p < json.size() && json[p] == ' ') p++;
    try { return std::stoi(json.substr(p)); } catch(...) { return 0; }
}

RoomPreview parseRoomPreview(const std::string& json, const std::string& roomId) {
    RoomPreview p;
    p.roomId = roomId.empty() ? extractField(json, "room_id") : roomId;
    p.name = extractField(json, "name");
    p.topic = extractField(json, "topic");
    p.avatarUrl = extractField(json, "avatar_url");
    p.alias = extractField(json, "canonical_alias");
    p.memberCount = extractInt(json, "num_joined_members");
    p.joinRule = extractField(json, "join_rule");
    p.guestAccess = extractField(json, "guest_access");
    p.isPublic = p.joinRule == "public";
    p.isSpace = extractField(json, "type") == "m.space";
    p.canJoin = p.joinRule == "public" || p.joinRule == "knock";
    p.isEncrypted = json.find("\"algorithm\":\"m.megolm") != std::string::npos;
    return p;
}

RoomPreview parseRoomSummary(const std::string& roomId, const std::string& json) {
    RoomPreview p;
    p.roomId = roomId;
    p.name = extractField(json, "display_name");
    p.topic = extractField(json, "topic");
    p.avatarUrl = extractField(json, "avatar_url");
    p.isDirect = extractField(json, "is_direct") == "true";
    p.isEncrypted = json.find("\"algorithm\"") != std::string::npos;
    p.memberCount = extractInt(json, "joined_member_count");
    return p;
}

std::string formatRoomPreviewCard(const RoomPreview& p) {
    std::ostringstream os;
    os << "=== " << p.name << " ===\n";
    if (!p.topic.empty()) os << p.topic << "\n";
    os << p.memberCount << " members";
    if (p.isPublic) os << " • Public";
    if (p.isEncrypted) os << " • Encrypted";
    if (p.isSpace) os << " • Space";
    os << "\n";
    if (!p.alias.empty()) os << "Alias: " << p.alias << "\n";
    return os.str();
}

std::string roomPreviewToJson(const RoomPreview& p) {
    std::ostringstream os;
    os << R"({"roomId":")" << p.roomId << R"(")";
    os << R"(,"name":")" << p.name << R"(")";
    os << R"(,"topic":")" << p.topic << R"(")";
    os << R"(,"memberCount":)" << p.memberCount;
    os << R"(,"isPublic":)" << (p.isPublic ? "true" : "false");
    os << R"(,"isEncrypted":)" << (p.isEncrypted ? "true" : "false");
    os << R"(,"canJoin":)" << (p.canJoin ? "true" : "false");
    os << "}";
    return os.str();
}

} // namespace pcp
