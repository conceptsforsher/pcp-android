#include "progressive/sync_handler.hpp"
#include "progressive/presence_utils.hpp"
#include <sstream>
#include <algorithm>
#include <unordered_set>

namespace progressive {

// ==== JSON helpers (manual, local to this TU) ====

namespace {

std::string escJson(const std::string& s) {
    std::string out;
    out.reserve(s.size() + 8);
    for (char c : s) {
        switch (c) {
            case '"':  out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\n': out += "\\n";  break;
            case '\r': out += "\\r";  break;
            case '\t': out += "\\t";  break;
            default:   out += c;
        }
    }
    return out;
}

std::string extractStr(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return {};
    pos = json.find(':', pos);
    if (pos == std::string::npos) return {};
    ++pos;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n')) ++pos;
    if (pos >= json.size() || json[pos] != '"') return {};
    ++pos;
    size_t end = pos;
    while (end < json.size()) {
        if (json[end] == '"') break;
        if (json[end] == '\\') ++end;
        ++end;
    }
    if (end >= json.size()) return {};
    std::string val;
    val.reserve(end - pos);
    for (size_t i = pos; i < end; ++i) {
        if (json[i] == '\\' && i + 1 < end) {
            switch (json[i + 1]) {
                case '"':  val += '"';  ++i; break;
                case '\\': val += '\\'; ++i; break;
                case 'n':  val += '\n'; ++i; break;
                case 'r':  val += '\r'; ++i; break;
                case 't':  val += '\t'; ++i; break;
                default:   val += json[i];
            }
        } else {
            val += json[i];
        }
    }
    return val;
}

double extractDouble(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return 0.0;
    pos = json.find(':', pos);
    if (pos == std::string::npos) return 0.0;
    ++pos;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n')) ++pos;
    if (pos >= json.size()) return 0.0;
    size_t end = pos;
    while (end < json.size() && json[end] != ',' && json[end] != '}' && json[end] != ']' && !std::isspace(json[end])) ++end;
    return std::stod(json.substr(pos, end - pos));
}

std::string extractObj(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return {};
    pos = json.find(':', pos);
    if (pos == std::string::npos) return {};
    ++pos;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n')) ++pos;
    if (pos >= json.size() || json[pos] != '{') return {};
    int depth = 1;
    size_t start = pos;
    ++pos;
    while (pos < json.size() && depth > 0) {
        if (json[pos] == '{') ++depth;
        else if (json[pos] == '}') --depth;
        ++pos;
    }
    return json.substr(start, pos - start);
}

// Parse string keys from a JSON object (returns key-value pairs)
std::vector<std::pair<std::string, std::string>> parseStringMap(const std::string& json) {
    std::vector<std::pair<std::string, std::string>> result;
    size_t pos = 0;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n')) ++pos;
    if (pos >= json.size() || json[pos] != '{') return result;
    ++pos;
    while (pos < json.size()) {
        while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == ',' || json[pos] == '\n')) ++pos;
        if (pos >= json.size() || json[pos] == '}') break;
        if (json[pos] == '"') {
            ++pos;
            size_t kend = pos;
            while (kend < json.size() && json[kend] != '"') {
                if (json[kend] == '\\') ++kend;
                ++kend;
            }
            std::string key = json.substr(pos, kend - pos);
            pos = kend + 1;
            while (pos < json.size() && json[pos] != ':') ++pos;
            ++pos;
            while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n')) ++pos;
            if (pos < json.size() && json[pos] == '{') {
                int depth = 1;
                size_t start = pos;
                ++pos;
                while (pos < json.size() && depth > 0) {
                    if (json[pos] == '{') ++depth;
                    else if (json[pos] == '}') --depth;
                    ++pos;
                }
                result.emplace_back(key, json.substr(start, pos - start));
            } else if (pos < json.size() && json[pos] == '"') {
                ++pos;
                size_t vend = pos;
                while (vend < json.size() && json[vend] != '"') {
                    if (json[vend] == '\\') ++vend;
                    ++vend;
                }
                result.emplace_back(key, "\"" + json.substr(pos, vend - pos) + "\"");
                pos = vend + 1;
            } else {
                // Number or literal, skip to next delimiter
                while (pos < json.size() && json[pos] != ',' && json[pos] != '}') ++pos;
            }
        } else {
            ++pos;
        }
    }
    return result;
}

} // anonymous namespace

// ==== processSyncResponse ====

// Original Kotlin: SyncResponseHandler.handleResponse()
SyncHandlerResult processSyncResponse(const SyncResponse& response) {
    SyncHandlerResult result;

    // Process rooms: join
    for (const auto& [roomId, room] : response.rooms.join) {
        ++result.roomsProcessed;
        int roomEvents = static_cast<int>(room.state.events.size()) +
                         static_cast<int>(room.timeline.events.size());
        if (room.ephemeral.state == EphemeralState::PARSED) {
            roomEvents += static_cast<int>(room.ephemeral.parsed.events.size());
        }
        result.eventsProcessed += roomEvents;
    }

    // Process rooms: leave
    for (const auto& [roomId, room] : response.rooms.leave) {
        ++result.roomsProcessed;
        int roomEvents = static_cast<int>(room.state.events.size()) +
                         static_cast<int>(room.timeline.events.size());
        result.eventsProcessed += roomEvents;
    }

    // Process rooms: invite
    for (const auto& [roomId, room] : response.rooms.invite) {
        ++result.roomsProcessed;
        result.eventsProcessed += static_cast<int>(room.inviteState.events.size());
    }

    // Process presence
    result.eventsProcessed += static_cast<int>(response.presence.events.size());

    // Process account data
    result.eventsProcessed += static_cast<int>(response.accountData.events.size());

    // Process to-device
    result.eventsProcessed += static_cast<int>(response.toDevice.events.size());

    // Build aggregator JSON for post-treatment
    std::ostringstream agg;
    agg << "{";
    agg << "\"roomsProcessed\":" << result.roomsProcessed << ",";
    agg << "\"eventsProcessed\":" << result.eventsProcessed << ",";
    agg << "\"deviceChanged\":" << response.deviceLists.changed.size() << ",";
    agg << "\"deviceLeft\":" << response.deviceLists.left.size();
    agg << "}";
    result.aggregatorJson = agg.str();

    return result;
}

// ==== processAccountDataSync ====

// Original Kotlin: UserAccountDataSyncHandler.handle()
std::vector<AccountDataSyncResult> processAccountDataSync(
    const std::vector<UserAccountDataEvent>& events) {

    std::vector<AccountDataSyncResult> results;

    for (const auto& event : events) {
        AccountDataSyncResult r;
        r.type = event.type;
        r.totalEvents = 1;
        r.processedEvents = 1;

        if (event.type == "m.direct") {
            r.processedEvents = 1;
        } else if (event.type == "m.push_rules") {
            r.processedEvents = 1;
        } else if (event.type == "m.ignored_user_list") {
            r.processedEvents = 1;
        } else if (event.type == "m.breadcrumbs") {
            r.processedEvents = 1;
        }

        results.push_back(std::move(r));
    }

    return results;
}

// ==== processRoomAccountData ====

// Original Kotlin: RoomSyncAccountDataHandler.handle()
int processRoomAccountData(const std::string& roomId,
                            const std::vector<Event>& accountDataEvents) {
    int processed = 0;

    for (const auto& event : accountDataEvents) {
        if (event.type.empty()) continue;

        // Identify the event type (use clear type without namespace if possible)
        std::string eventType = event.type;
        if (eventType == "m.tag" || eventType.find("tag") != std::string::npos) {
            // Tags are handled separately via processRoomTags
            ++processed;
        } else if (eventType == "m.fully_read" || eventType.find("fully_read") != std::string::npos) {
            ++processed;
        } else {
            ++processed;
        }
    }

    return processed;
}

// ==== processRoomTags ====

// Original Kotlin: RoomTagHandler.handle()
std::vector<RoomTagInfo> processRoomTags(const std::string& roomId, const std::string& tagContentJson) {
    std::vector<RoomTagInfo> result;

    // Content format: {"tags":{"m.favourite":{"order":0.5},"m.lowpriority":{}}}
    auto tagsObj = extractObj(tagContentJson, "tags");
    if (tagsObj.empty()) return result;

    auto pairs = parseStringMap(tagsObj);
    for (const auto& [tagName, tagValueJson] : pairs) {
        RoomTagInfo info;
        info.roomId = roomId;
        info.tagName = tagName;
        info.tagOrder = extractDouble(tagValueJson, "order");
        result.push_back(std::move(info));
    }

    return result;
}

// ==== processFullyReadMarker ====

// Original Kotlin: RoomFullyReadHandler.handle()
std::string processFullyReadMarker(const std::string& fullyReadContentJson) {
    // Content format: {"event_id":"$someEventId"}
    return extractStr(fullyReadContentJson, "event_id");
}

// ==== processTypingUsers ====

// Original Kotlin: RoomTypingUsersHandler.handle()
std::vector<std::string> processTypingUsers(const std::string& roomId,
                                             const std::vector<std::string>& typingUserIds,
                                             const std::string& selfUserId,
                                             const std::vector<std::string>& ignoredUserIds) {
    std::unordered_set<std::string> filtered(ignoredUserIds.begin(), ignoredUserIds.end());
    filtered.insert(selfUserId);

    std::vector<std::string> result;
    for (const auto& userId : typingUserIds) {
        if (filtered.find(userId) == filtered.end()) {
            result.push_back(userId);
        }
    }

    return result;
}

// ==== computeSyncPostTreatment ====

// Original Kotlin: SyncResponsePostTreatmentAggregator
SyncPostTreatmentResult computeSyncPostTreatment(const SyncHandlerResult& handlerResult) {
    SyncPostTreatmentResult treatment;

    // Process aggregator data from handler result
    if (!handlerResult.aggregatorJson.empty()) {
        // Extract device change counts as shield update signals
        auto dc = [&]() -> int {
            auto pos = handlerResult.aggregatorJson.find("\"deviceChanged\":");
            if (pos == std::string::npos) return 0;
            pos += 17;
            int val = 0;
            while (pos < handlerResult.aggregatorJson.size() && handlerResult.aggregatorJson[pos] >= '0' && handlerResult.aggregatorJson[pos] <= '9') {
                val = val * 10 + (handlerResult.aggregatorJson[pos] - '0');
                ++pos;
            }
            return val;
        };
        treatment.shieldUpdates = dc();
    }

    // If there were errors during processing, flag summary updates
    if (!handlerResult.errors.empty()) {
        treatment.summaryUpdates = static_cast<int>(handlerResult.errors.size());
    }

    // Rooms processed with membership changes trigger space updates
    treatment.spaceUpdates = (handlerResult.roomsProcessed > 0) ? 1 : 0;

    return treatment;
}

} // namespace progressive
