#include "progressive/room_directory_manager.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

namespace progressive {

// ====== Enum conversions ======

const char* visibilityToString(RoomDirectoryVisibility v) {
    return v == RoomDirectoryVisibility::PUBLIC ? "public" : "private";
}

RoomDirectoryVisibility visibilityFromString(const std::string& s) {
    if (s == "public") return RoomDirectoryVisibility::PUBLIC;
    return RoomDirectoryVisibility::PRIVATE;
}

// ====== JSON helpers ======

std::string RoomDirectoryManager::extractStr(const std::string& json, const std::string& key) {
    auto pp = json.find("\"" + key + "\"");
    if (pp == std::string::npos) return "";
    pp = json.find('"', pp + key.size() + 2);
    if (pp == std::string::npos) return "";
    pp++;
    size_t e = pp;
    while (e < json.size() && json[e] != '"') e++;
    return json.substr(pp, e - pp);
}

int64_t RoomDirectoryManager::extractInt(const std::string& json, const std::string& key) {
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

bool RoomDirectoryManager::extractBool(const std::string& json, const std::string& key) {
    return json.find("\"" + key + "\":true") != std::string::npos;
}

// ====== Constructor ======

RoomDirectoryManager::RoomDirectoryManager() {}

// ====== Public Rooms Search ======
// Original: getPublicRooms(server, PublicRoomsParams)

std::string RoomDirectoryManager::buildPublicRoomsRequest(const PublicRoomsParams& params) const {
    std::ostringstream os;
    os << R"({"limit":)" << params.limit;

    if (!params.since.empty()) {
        os << R"(,"since":")" << params.since << R"(")";
    }

    if (!params.filter.searchTerm.empty()) {
        os << R"(,"filter":{"generic_search_term":")" << params.filter.searchTerm << R"("})";
    }

    if (params.includeAllNetworks) {
        os << R"(,"include_all_networks":true)";
    }

    if (!params.thirdPartyInstanceId.empty()) {
        os << R"(,"third_party_instance_id":")" << params.thirdPartyInstanceId << R"(")";
    }

    os << "}";
    return os.str();
}

// Original: PublicRoomsResponse — {"chunk":[{...}],"next_batch":"...","prev_batch":"...","total_room_count_estimate":42}
PublicRoomsResponse RoomDirectoryManager::parsePublicRoomsResponse(const std::string& json) const {
    PublicRoomsResponse resp;

    resp.nextBatch = extractStr(json, "next_batch");
    resp.prevBatch = extractStr(json, "prev_batch");
    resp.totalRoomCountEstimate = static_cast<int>(extractInt(json, "total_room_count_estimate"));
    resp.hasMore = !resp.nextBatch.empty();

    // Parse chunk array
    size_t pos = json.find("\"chunk\"");
    if (pos == std::string::npos) return resp;

    pos = json.find('[', pos);
    if (pos == std::string::npos) return resp;
    pos++;

    while (pos < json.size()) {
        while (pos < json.size() && (json[pos] == ' ' || json[pos] == ',' || json[pos] == '\n')) pos++;
        if (pos >= json.size() || json[pos] == ']') break;

        size_t objStart = pos;
        int depth = 0;
        while (pos < json.size()) {
            if (json[pos] == '{') depth++;
            else if (json[pos] == '}') depth--;
            if (depth == 0 && json[pos] == '}') { pos++; break; }
            pos++;
        }
        std::string roomJson = json.substr(objStart, pos - objStart);

        PublicRoom room;
        room.roomId = extractStr(roomJson, "room_id");
        room.name = extractStr(roomJson, "name");
        room.topic = extractStr(roomJson, "topic");
        room.numJoinedMembers = static_cast<int>(extractInt(roomJson, "num_joined_members"));
        room.avatarUrl = extractStr(roomJson, "avatar_url");
        room.canonicalAlias = extractStr(roomJson, "canonical_alias");
        room.worldReadable = extractBool(roomJson, "world_readable");
        room.guestCanJoin = extractBool(roomJson, "guest_can_join");

        // Parse m.federate (undocumented)
        room.isFederated = extractBool(roomJson, "m.federate");

        // Parse aliases array: ["#alias1:org","#alias2:org"]
        auto aliasesPos = roomJson.find("\"aliases\"");
        if (aliasesPos != std::string::npos) {
            aliasesPos = roomJson.find('[', aliasesPos);
            if (aliasesPos != std::string::npos) {
                aliasesPos++;
                while (aliasesPos < roomJson.size() && roomJson[aliasesPos] != ']') {
                    if (roomJson[aliasesPos] == '"') {
                        aliasesPos++;
                        size_t e = aliasesPos;
                        while (e < roomJson.size() && roomJson[e] != '"') e++;
                        room.aliases.push_back(roomJson.substr(aliasesPos, e - aliasesPos));
                        aliasesPos = e;
                    }
                    aliasesPos++;
                }
            }
        }

        room.valid = !room.roomId.empty();
        if (room.valid) resp.chunk.push_back(room);
    }

    resp.loadedCount = static_cast<int>(resp.chunk.size());
    return resp;
}

void RoomDirectoryManager::accumulateResults(PublicRoomsResponse& existing, const PublicRoomsResponse& nextPage) const {
    existing.chunk.insert(existing.chunk.end(), nextPage.chunk.begin(), nextPage.chunk.end());
    existing.nextBatch = nextPage.nextBatch;
    existing.prevBatch = nextPage.prevBatch;
    existing.hasMore = nextPage.hasMore;
    existing.loadedCount += nextPage.loadedCount;
}

// ====== Room Directory Visibility ======

std::string RoomDirectoryManager::buildVisibilityRequest(RoomDirectoryVisibility visibility) const {
    return std::string(R"({"visibility":")") + visibilityToString(visibility) + R"("})";
}

RoomDirectoryVisibility RoomDirectoryManager::parseVisibilityResponse(const std::string& json) const {
    auto vis = extractStr(json, "visibility");
    return visibilityFromString(vis);
}

// ====== Alias Check ======

std::string RoomDirectoryManager::buildAliasCheckRequest(const std::string& aliasLocalPart) const {
    return R"({"alias_localpart":")" + aliasLocalPart + R"("})";
}

AliasAvailabilityResult RoomDirectoryManager::parseAliasAvailability(const std::string& json, const std::string& aliasLocalPart) const {
    AliasAvailabilityResult result;
    result.available = extractBool(json, "available");
    result.alias = "#" + aliasLocalPart;
    auto err = extractStr(json, "errcode");
    if (!err.empty()) {
        result.error = err;
        result.available = false;
    }
    return result;
}

// ====== Room Preview ======

std::string RoomDirectoryManager::formatRoomPreview(const PublicRoom& room) const {
    std::ostringstream os;
    os << (room.name.empty() ? room.getPrimaryAlias() : room.name);
    if (!room.topic.empty()) {
        std::string topic = room.topic;
        if (topic.size() > 80) topic = topic.substr(0, 77) + "...";
        os << " — " << topic;
    }
    os << " (" << room.numJoinedMembers << " members)";
    return os.str();
}

std::string RoomDirectoryManager::buildRoomJoinUrl(const std::string& roomId, const std::string& viaServer) const {
    std::ostringstream url;
    url << "/join/" << roomId;
    if (!viaServer.empty()) url << "?via=" << viaServer;
    // URL encode
    std::string result = url.str();
    for (size_t i = 0; i < result.size(); i++) {
        if (result[i] == '#') {
            result.replace(i, 1, "%23");
            i += 2;
        }
    }
    return result;
}

std::string RoomDirectoryManager::buildRoomAvatarUrl(const std::string& avatarUrl, const std::string& homeServer) const {
    if (avatarUrl.empty()) return "";
    if (avatarUrl.rfind("mxc://", 0) == 0) {
        auto server = extractStr(avatarUrl, "");
        auto id = extractStr(avatarUrl, "");
        // Proper MXC resolution
        auto mxc = avatarUrl;
        if (mxc.compare(0, 6, "mxc://") == 0) {
            auto slash = mxc.find('/', 6);
            if (slash != std::string::npos) {
                auto serverName = mxc.substr(6, slash - 6);
                auto mediaId = mxc.substr(slash + 1);
                return homeServer + "/_matrix/media/r0/thumbnail/" + serverName + "/" + mediaId + "?width=96&height=96&method=crop";
            }
        }
    }
    return avatarUrl;
}

// ====== Filtering & Sorting ======

std::vector<PublicRoom> RoomDirectoryManager::filterRooms(const std::vector<PublicRoom>& rooms, const std::string& query) const {
    if (query.empty()) return rooms;

    std::string q;
    for (char c : query) q += static_cast<char>(std::tolower(c));

    std::vector<PublicRoom> result;
    for (const auto& room : rooms) {
        std::string name;
        for (char c : room.name) name += static_cast<char>(std::tolower(c));
        std::string topic;
        for (char c : room.topic) topic += static_cast<char>(std::tolower(c));
        std::string alias = room.getPrimaryAlias();

        if (name.find(q) != std::string::npos ||
            topic.find(q) != std::string::npos ||
            alias.find(q) != std::string::npos) {
            result.push_back(room);
        }
    }
    return result;
}

void RoomDirectoryManager::sortRoomsByPopularity(std::vector<PublicRoom>& rooms) const {
    std::sort(rooms.begin(), rooms.end(), [](const PublicRoom& a, const PublicRoom& b) {
        if (a.numJoinedMembers != b.numJoinedMembers) return a.numJoinedMembers > b.numJoinedMembers;
        return a.name < b.name;
    });
}

void RoomDirectoryManager::sortRoomsByName(std::vector<PublicRoom>& rooms) const {
    std::sort(rooms.begin(), rooms.end(), [](const PublicRoom& a, const PublicRoom& b) {
        return a.name < b.name;
    });
}

// ====== Serialization ======

std::string RoomDirectoryManager::roomToJson(const PublicRoom& room) const {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    std::ostringstream os;
    os << R"({"room_id":")" << esc(room.roomId)
       << R"(","name":")" << esc(room.name)
       << R"(","topic":")" << esc(room.topic)
       << R"(,"num_members":)" << room.numJoinedMembers
       << R"(,"avatar_url":")" << esc(room.avatarUrl)
       << R"(,"primary_alias":")" << esc(room.getPrimaryAlias())
       << R"(,"world_readable":)" << (room.worldReadable ? "true" : "false")
       << R"(,"guest_can_join":)" << (room.guestCanJoin ? "true" : "false")
       << R"(,"federated":)" << (room.isFederated ? "true" : "false")
       << R"(,"aliases":[)";
    for (size_t i = 0; i < room.aliases.size(); i++) {
        if (i > 0) os << ","; os << "\"" << esc(room.aliases[i]) << "\"";
    }
    os << R"(],"preview":")" << esc(formatRoomPreview(room)) << R"(")";
    os << "}";
    return os.str();
}

std::string RoomDirectoryManager::roomsToJson(const std::vector<PublicRoom>& rooms) const {
    std::ostringstream os; os << "[";
    for (size_t i = 0; i < rooms.size(); i++) {
        if (i > 0) os << ","; os << roomToJson(rooms[i]);
    }
    os << "]";
    return os.str();
}

std::string RoomDirectoryManager::responseToJson(const PublicRoomsResponse& resp) const {
    std::ostringstream os;
    os << R"({"rooms":)" << roomsToJson(resp.chunk)
       << R"(,"next_batch":")" << resp.nextBatch
       << R"(","has_more":)" << (resp.hasMore ? "true" : "false")
       << R"(,"total_estimate":)" << resp.totalRoomCountEstimate
       << R"(,"loaded_count":)" << resp.loadedCount
       << "}";
    return os.str();
}

std::string RoomDirectoryManager::aliasResultToJson(const AliasAvailabilityResult& result) const {
    std::ostringstream os;
    os << R"({"alias":")" << result.alias
       << R"(","available":)" << (result.available ? "true" : "false");
    if (!result.error.empty()) os << R"(,"error":")" << result.error << R"(")";
    os << "}";
    return os.str();
}

} // namespace progressive
