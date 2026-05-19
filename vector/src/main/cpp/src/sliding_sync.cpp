#include "progressive/sliding_sync.hpp"
#include "progressive/http_client.hpp"
#include <sstream>
#include <algorithm>

namespace progressive {

// ==== SlidingSyncList → JSON ====

static const char* sortToString(SlidingSyncSort s) {
    switch (s) {
        case SlidingSyncSort::BY_RECENCY: return "by_recency";
        case SlidingSyncSort::BY_NAME: return "by_name";
        case SlidingSyncSort::BY_NOTIFICATION_LEVEL: return "by_notification_level";
        case SlidingSyncSort::BY_PRIORITY: return "by_priority";
    }
    return "by_recency";
}

std::string SlidingSyncList::toJson() const {
    std::ostringstream os;
    os << "{";
    os << R"("name":")" << name << R"(",)";
    // Range
    os << R"("ranges":[[)" << rangeStart << "," << rangeEnd << R"(]],)";
    // Sort
    os << R"("sort":[)";
    for (size_t i = 0; i < sort.size(); i++) {
        if (i > 0) os << ",";
        os << R"(")" << sortToString(sort[i]) << R"(")";
    }
    os << "],";
    // Required state
    os << R"("required_state":[)";
    for (size_t i = 0; i < requiredState.size(); i++) {
        if (i > 0) os << ",";
        os << "[\"" << requiredState[i].eventType << "\",\"" << requiredState[i].stateKey << "\"]";
    }
    os << "],";
    // Timeline limit
    os << R"("timeline_limit":20)";
    // Filters
    if (!includeOldRooms) {
        os << R"(,"filters":{"is_dm":false})";
    }
    os << "}";
    return os.str();
}

// ==== SlidingSyncRequest → JSON ====

std::string SlidingSyncRequest::toJson() const {
    std::ostringstream os;
    os << "{";

    // Lists
    os << R"("lists":{)";
    for (size_t i = 0; i < lists.size(); i++) {
        if (i > 0) os << ",";
        os << lists[i].toJson();
    }
    os << "},";

    // Room subscriptions
    os << R"("room_subscriptions":{)";
    for (size_t i = 0; i < roomSubscriptions.size(); i++) {
        if (i > 0) os << ",";
        os << R"(")" << roomSubscriptions[i] << R"(":{"required_state":[],"timeline_limit":20})";
    }
    os << "},";

    // Position
    os << R"("pos":")" << pos << R"(",)";

    // Extensions (request E2EE + to-device + account data)
    os << R"("extensions":{"e2ee":{"enabled":true},"to_device":{"enabled":true},"account_data":{"enabled":true}})";

    os << "}";
    return os.str();
}

// ==== API Call ====

SlidingSyncResponse slidingSync(const SlidingSyncRequest& request,
                                 const std::string& homeserverUrl,
                                 const std::string& accessToken) {
    SlidingSyncResponse response;

    std::string url = homeserverUrl;
    if (!url.empty() && url.back() == '/') url.pop_back();
    url += "/_matrix/client/unstable/org.matrix.msc3575/sync";
    if (request.timeout > 0) {
        url += "?timeout=" + std::to_string(request.timeout);
    }

    std::unordered_map<std::string, std::string> headers;
    if (!accessToken.empty()) headers["Authorization"] = "Bearer " + accessToken;

    auto httpResp = httpPost(url, request.toJson(), headers, request.timeout + 10000);

    if (!httpResp.isOk()) {
        response.errorMessage = "Sliding sync failed: HTTP " + std::to_string(httpResp.statusCode);
        return response;
    }

    return parseSlidingSyncResponse(httpResp.body);
}

// ==== JSON Parsing Helpers ====

static std::string extractJsonString(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return "";
    pos = json.find(':', pos);
    if (pos == std::string::npos) return "";
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '"')) pos++;
    size_t end = pos;
    while (end < json.size() && json[end] != '"') { if (json[end] == '\\') end++; end++; }
    return json.substr(pos, end - pos);
}

static int extractJsonInt(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return 0;
    pos = json.find(':', pos);
    if (pos == std::string::npos) return 0;
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    int val = 0;
    while (pos < json.size() && json[pos] >= '0' && json[pos] <= '9') { val = val * 10 + (json[pos]-'0'); pos++; }
    return val;
}

static bool extractJsonBool(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return false;
    pos = json.find(':', pos);
    if (pos == std::string::npos) return false;
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    return json.compare(pos, 4, "true") == 0;
}

static std::string extractJsonObject(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return "";
    pos = json.find(':', pos);
    if (pos == std::string::npos) return "";
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    if (pos >= json.size() || json[pos] != '{') return "";
    int depth = 1;
    size_t start = pos;
    pos++;
    while (pos < json.size() && depth > 0) { if (json[pos] == '{') depth++; else if (json[pos] == '}') depth--; pos++; }
    return json.substr(start, pos - start);
}

// ==== Response Parsing ====

SlidingSyncResponse parseSlidingSyncResponse(const std::string& json) {
    SlidingSyncResponse response;

    // Parse position
    response.pos = extractJsonString(json, "pos");
    response.success = !response.pos.empty();

    // Parse lists
    auto listsObj = extractJsonObject(json, "lists");
    if (!listsObj.empty()) {
        size_t pos = 1; // skip opening {
        while (pos < listsObj.size()) {
            while (pos < listsObj.size() && (listsObj[pos] == ' ' || listsObj[pos] == ',' || listsObj[pos] == '\n')) pos++;
            if (pos >= listsObj.size() || listsObj[pos] == '}') break;
            if (listsObj[pos] == '"') {
                pos++;
                size_t keyEnd = pos;
                while (keyEnd < listsObj.size() && listsObj[keyEnd] != '"') keyEnd++;
                std::string listName = listsObj.substr(pos, keyEnd - pos);
                pos = keyEnd + 1;
                while (pos < listsObj.size() && listsObj[pos] != ':') pos++;
                pos++;
                while (pos < listsObj.size() && (listsObj[pos] == ' ' || listsObj[pos] == '\n')) pos++;
                if (pos < listsObj.size() && listsObj[pos] == '{') {
                    int depth = 1;
                    size_t start = pos;
                    pos++;
                    while (pos < listsObj.size() && depth > 0) { if (listsObj[pos] == '{') depth++; else if (listsObj[pos] == '}') depth--; pos++; }
                    std::string listJson = listsObj.substr(start, pos - start);

                    SlidingSyncListResponse listResp;
                    listResp.count = extractJsonInt(listJson, "count");

                    // Parse ops array
                    auto opsPos = listJson.find("\"ops\"");
                    if (opsPos != std::string::npos) {
                        opsPos = listJson.find('[', opsPos);
                        if (opsPos != std::string::npos) {
                            opsPos++;
                            while (opsPos < listJson.size()) {
                                while (opsPos < listJson.size() && (listJson[opsPos] == ' ' || listJson[opsPos] == ',' || listJson[opsPos] == '\n')) opsPos++;
                                if (opsPos >= listJson.size() || listJson[opsPos] == ']') break;
                                if (listJson[opsPos] == '{') {
                                    int d = 1;
                                    size_t os = opsPos;
                                    opsPos++;
                                    while (opsPos < listJson.size() && d > 0) { if (listJson[opsPos] == '{') d++; else if (listJson[opsPos] == '}') d--; opsPos++; }
                                    std::string opJson = listJson.substr(os, opsPos - os);

                                    SlidingSyncListOp op;
                                    auto opStr = extractJsonString(opJson, "op");
                                    if (opStr == "SYNC") op.op = SlidingSyncOp::SYNC;
                                    else if (opStr == "INSERT") op.op = SlidingSyncOp::INSERT;
                                    else if (opStr == "DELETE") op.op = SlidingSyncOp::DELETE;
                                    else if (opStr == "INVALIDATE") op.op = SlidingSyncOp::INVALIDATE;

                                    // Parse range
                                    auto rangePos = opJson.find("\"range\"");
                                    if (rangePos != std::string::npos) {
                                        rangePos = opJson.find('[', rangePos);
                                        if (rangePos != std::string::npos) {
                                            rangePos++;
                                            while (rangePos < opJson.size() && opJson[rangePos] == ' ') rangePos++;
                                            op.rangeStart = 0;
                                            while (rangePos < opJson.size() && opJson[rangePos] >= '0' && opJson[rangePos] <= '9') { op.rangeStart = op.rangeStart * 10 + (opJson[rangePos]-'0'); rangePos++; }
                                            while (rangePos < opJson.size() && opJson[rangePos] != ',') rangePos++;
                                            rangePos++;
                                            op.rangeEnd = 0;
                                            while (rangePos < opJson.size() && opJson[rangePos] >= '0' && opJson[rangePos] <= '9') { op.rangeEnd = op.rangeEnd * 10 + (opJson[rangePos]-'0'); rangePos++; }
                                        }
                                    }

                                    // Parse room_ids array
                                    auto idsPos = opJson.find("\"room_ids\"");
                                    if (idsPos != std::string::npos) {
                                        idsPos = opJson.find('[', idsPos);
                                        if (idsPos != std::string::npos) {
                                            idsPos++;
                                            while (idsPos < opJson.size()) {
                                                while (idsPos < opJson.size() && (opJson[idsPos] == ' ' || opJson[idsPos] == ',' || opJson[idsPos] == '\n')) idsPos++;
                                                if (idsPos >= opJson.size() || opJson[idsPos] == ']') break;
                                                if (opJson[idsPos] == '"') {
                                                    idsPos++;
                                                    size_t idEnd = idsPos;
                                                    while (idEnd < opJson.size() && opJson[idEnd] != '"') idEnd++;
                                                    op.roomIds.push_back(opJson.substr(idsPos, idEnd - idsPos));
                                                    idsPos = idEnd + 1;
                                                }
                                            }
                                        }
                                    }

                                    listResp.ops.push_back(op);
                                }
                            }
                        }
                    }

                    response.lists[listName] = listResp;
                }
            }
        }
    }

    // Parse rooms
    auto roomsObj = extractJsonObject(json, "rooms");
    if (!roomsObj.empty()) {
        size_t pos = 1;
        while (pos < roomsObj.size()) {
            while (pos < roomsObj.size() && (roomsObj[pos] == ' ' || roomsObj[pos] == ',' || roomsObj[pos] == '\n')) pos++;
            if (pos >= roomsObj.size() || roomsObj[pos] == '}') break;
            if (roomsObj[pos] == '"') {
                pos++;
                size_t keyEnd = pos;
                while (keyEnd < roomsObj.size() && roomsObj[keyEnd] != '"') keyEnd++;
                std::string roomId = roomsObj.substr(pos, keyEnd - pos);
                pos = keyEnd + 1;
                while (pos < roomsObj.size() && roomsObj[pos] != ':') pos++;
                pos++;
                while (pos < roomsObj.size() && (roomsObj[pos] == ' ' || roomsObj[pos] == '\n')) pos++;
                if (pos < roomsObj.size() && roomsObj[pos] == '{') {
                    int depth = 1;
                    size_t start = pos;
                    pos++;
                    while (pos < roomsObj.size() && depth > 0) { if (roomsObj[pos] == '{') depth++; else if (roomsObj[pos] == '}') depth--; pos++; }
                    std::string roomJson = roomsObj.substr(start, pos - start);

                    SlidingSyncRoom room;
                    room.roomId = roomId;
                    room.name = extractJsonString(roomJson, "name");
                    room.avatarUrl = extractJsonString(roomJson, "avatar_url");
                    room.isDirect = extractJsonBool(roomJson, "is_dm");
                    room.initial = extractJsonBool(roomJson, "initial");
                    room.notificationCount = extractJsonInt(roomJson, "notification_count");
                    room.highlightCount = extractJsonInt(roomJson, "highlight_count");
                    room.isEncrypted = extractJsonBool(roomJson, "is_encrypted");

                    response.rooms[roomId] = room;
                }
            }
        }
    }

    return response;
}

// ==== Client State Management ====

void SlidingSyncState::scrollDown(int amount) {
    for (auto& list : lists) {
        list.rangeStart += amount;
        list.rangeEnd += amount;
    }
}

void SlidingSyncState::updateSubscriptions(const std::vector<std::string>& visibleRoomIds) {
    subscribedRooms = visibleRoomIds;
}

SlidingSyncRequest SlidingSyncState::buildRequest(int timeout) const {
    SlidingSyncRequest req;
    req.lists = lists;
    req.roomSubscriptions = subscribedRooms;
    req.pos = pos;
    req.timeout = timeout;
    return req;
}

} // namespace progressive
