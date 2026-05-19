#include "progressive/search_index.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <chrono>
#include <unordered_set>

namespace progressive {

std::string SearchIndex::toLower(const std::string& s) {
    std::string r = s;
    std::transform(r.begin(), r.end(), r.begin(), ::tolower);
    return r;
}

float SearchIndex::computeRelevance(const std::string& body, const std::string& query) {
    auto lowerBody = toLower(body);
    auto lowerQuery = toLower(query);

    // Split query into terms
    std::vector<std::string> terms;
    std::istringstream qs(lowerQuery);
    std::string term;
    while (qs >> term) terms.push_back(term);

    if (terms.empty()) return 0.0f;

    int matches = 0;
    int totalTerms = static_cast<int>(terms.size());

    for (const auto& t : terms) {
        if (lowerBody.find(t) != std::string::npos) {
            ++matches;
        }
    }

    // Base score: what fraction of query terms matched
    float baseScore = static_cast<float>(matches) / static_cast<float>(totalTerms);

    // Bonus: exact phrase match
    if (lowerBody.find(lowerQuery) != std::string::npos) {
        baseScore += 0.3f;
    }

    // Bonus: query appears early in body
    auto pos = lowerBody.find(terms[0]);
    if (pos != std::string::npos && pos < 50) {
        baseScore += 0.1f;
    }

    return std::min(1.0f, baseScore);
}

void SearchIndex::indexMessage(const std::string& eventId, const std::string& roomId,
                               const std::string& roomName, const std::string& senderName,
                               const std::string& body, int64_t timestamp,
                               bool isEncrypted) {
    // Remove old entry if exists
    entries_.erase(std::remove_if(entries_.begin(), entries_.end(),
        [&](const IndexedMessage& m) { return m.eventId == eventId; }
    ), entries_.end());

    entries_.push_back({eventId, roomId, roomName, senderName, body, timestamp, isEncrypted});
}

SearchResult SearchIndex::search(const std::string& query, int limit) const {
    auto startTime = std::chrono::steady_clock::now();

    SearchResult result;
    result.query = query;

    // Collect hits with relevance
    std::vector<std::pair<SearchHit, float>> scoredHits;
    std::unordered_set<std::string> roomsSearched;

    for (const auto& entry : entries_) {
        auto relevance = computeRelevance(entry.body, query);
        if (relevance > 0.0f) {
            SearchHit hit;
            hit.eventId = entry.eventId;
            hit.roomId = entry.roomId;
            hit.roomName = entry.roomName;
            hit.senderName = entry.senderName;
            hit.isEncrypted = entry.isEncrypted;
            hit.originServerTs = entry.timestamp;
            hit.relevanceScore = relevance;

            // Create body snippet (first 200 chars)
            auto pos = toLower(entry.body).find(toLower(query));
            int snippetStart = pos != std::string::npos ? std::max(0, static_cast<int>(pos) - 40) : 0;
            hit.body = entry.body.substr(snippetStart, 200);
            if (snippetStart > 0) hit.body = "..." + hit.body;
            if (entry.body.size() > static_cast<size_t>(snippetStart + 200)) hit.body += "...";

            scoredHits.push_back({hit, relevance});
            roomsSearched.insert(entry.roomId);
        }
    }

    // Sort by relevance (highest first), then by timestamp (newest first)
    std::sort(scoredHits.begin(), scoredHits.end(),
        [](const auto& a, const auto& b) {
            if (a.second != b.second) return a.second > b.second;
            return a.first.originServerTs > b.first.originServerTs;
        }
    );

    // Take top N
    auto end = scoredHits.size() > static_cast<size_t>(limit)
        ? scoredHits.begin() + limit : scoredHits.end();
    for (auto it = scoredHits.begin(); it != end; ++it) {
        result.hits.push_back(it->first);
    }

    result.totalHits = static_cast<int>(scoredHits.size());
    result.roomsSearched = static_cast<int>(roomsSearched.size());

    auto endTime = std::chrono::steady_clock::now();
    result.searchTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    return result;
}

void SearchIndex::removeRoom(const std::string& roomId) {
    entries_.erase(std::remove_if(entries_.begin(), entries_.end(),
        [&](const IndexedMessage& m) { return m.roomId == roomId; }
    ), entries_.end());
}

void SearchIndex::clear() {
    entries_.clear();
}

std::string SearchIndex::hitsToJson(const std::vector<SearchHit>& hits) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) {
            if (c == '"') out += "\\\"";
            else out += c;
        }
        return out;
    };

    std::ostringstream json;
    json << "[";
    for (size_t i = 0; i < hits.size(); ++i) {
        if (i > 0) json << ",";
        const auto& h = hits[i];
        json << R"({"eventId": ")" << esc(h.eventId) << R"(")";
        json << R"(,"roomId": ")" << esc(h.roomId) << R"(")";
        json << R"(,"roomName": ")" << esc(h.roomName) << R"(")";
        json << R"(,"senderName": ")" << esc(h.senderName) << R"(")";
        json << R"(,"body": ")" << esc(h.body) << R"(")";
        json << R"(,"relevance": )" << h.relevanceScore;
        json << R"(,"isEncrypted": )" << (h.isEncrypted ? "true" : "false") << "}";
    }
    json << "]";
    return json.str();
}

// ============ Search JSON builders ============

// ---- JSON escape helper ----
namespace {

std::string escJson(const std::string& s) {
    std::string out;
    out.reserve(s.size() + 8);
    for (char c : s) {
        switch (c) {
            case '"':  out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\n': out += "\\n"; break;
            case '\r': out += "\\r"; break;
            case '\t': out += "\\t"; break;
            default:   out += c;
        }
    }
    return out;
}

// Manual JSON value extractor (matches existing project patterns)
std::string extractJsonValue(const std::string& json, const std::string& key) {
    auto search = '"' + key + '"';
    auto pos = json.find(search);
    if (pos == std::string::npos) return {};
    pos = json.find(':', pos);
    if (pos == std::string::npos) return {};
    ++pos;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n')) ++pos;
    if (pos >= json.size()) return {};
    if (json[pos] == '"') {
        ++pos;
        auto end = pos;
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
    // Numeric or boolean or null value
    auto end = pos;
    while (end < json.size() && json[end] != ',' && json[end] != '}' && json[end] != ']' && !std::isspace(json[end])) ++end;
    return json.substr(pos, end - pos);
}

// Extract the raw JSON substring of an object key's value (including nested objects/arrays)
std::string extractJsonRaw(const std::string& json, const std::string& key) {
    auto search = '"' + key + '"';
    auto pos = json.find(search);
    if (pos == std::string::npos) return {};
    pos = json.find(':', pos);
    if (pos == std::string::npos) return {};
    ++pos;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n')) ++pos;
    if (pos >= json.size()) return {};

    if (json[pos] == '"') {
        // String value - return it quoted
        ++pos;
        auto end = pos;
        while (end < json.size()) {
            if (json[end] == '"') break;
            if (json[end] == '\\') ++end;
            ++end;
        }
        return json.substr(pos - 1, end - pos + 2);
    }
    if (json[pos] == '{') {
        int depth = 0;
        auto end = pos;
        while (end < json.size()) {
            if (json[end] == '{') ++depth;
            else if (json[end] == '}') { --depth; if (depth == 0) { ++end; break; } }
            ++end;
        }
        return json.substr(pos, end - pos);
    }
    if (json[pos] == '[') {
        int depth = 0;
        auto end = pos;
        while (end < json.size()) {
            if (json[end] == '[') ++depth;
            else if (json[end] == ']') { --depth; if (depth == 0) { ++end; break; } }
            else if (json[end] == '"') { ++end; while (end < json.size() && json[end] != '"') { if (json[end] == '\\') ++end; ++end; } }
            ++end;
        }
        return json.substr(pos, end - pos);
    }
    // Number, boolean, or null
    auto end = pos;
    while (end < json.size() && json[end] != ',' && json[end] != '}' && json[end] != ']' && !std::isspace(json[end])) ++end;
    return json.substr(pos, end - pos);
}

} // anonymous namespace

// Original Kotlin: builds the room_events search query JSON object
std::string buildSearchQuery(const SearchQuery& query) {
    std::ostringstream os;
    os << "{";
    os << "\"search_term\":\"" << escJson(query.searchTerm) << "\"";

    // order_by
    os << ",\"order_by\":\"" << (query.orderByRecency ? "recent" : "rank") << "\"";

    // filter
    bool hasFilter = !query.roomId.empty() || query.limit > 0;
    if (hasFilter) {
        os << ",\"filter\":{";
        bool first = true;
        if (query.limit > 0) {
            os << "\"limit\":" << query.limit;
            first = false;
        }
        if (!query.roomId.empty()) {
            if (!first) os << ",";
            os << "\"rooms\":[\"" << escJson(query.roomId) << "\"]";
        }
        os << "}";
    }

    // event_context
    const auto& ec = query.eventContext;
    bool hasContext = ec.beforeLimit > 0 || ec.afterLimit > 0 || ec.includeProfile;
    if (hasContext) {
        os << ",\"event_context\":{";
        bool first = true;
        if (ec.beforeLimit > 0) {
            os << "\"before_limit\":" << ec.beforeLimit;
            first = false;
        }
        if (ec.afterLimit > 0) {
            if (!first) os << ",";
            os << "\"after_limit\":" << ec.afterLimit;
            first = false;
        }
        if (ec.includeProfile) {
            if (!first) os << ",";
            os << "\"include_profile\":true";
        }
        os << "}";
    }

    if (query.includeProfile) {
        os << ",\"include_profile\":true";
    }

    os << "}";
    return os.str();
}

// Original Kotlin: builds the room_events subtree for the search body
std::string buildRoomEventsSearchBody(const SearchQuery& query) {
    std::ostringstream os;
    os << "\"room_events\":" << buildSearchQuery(query);
    return os.str();
}

// Original Kotlin: builds the full POST /search body
std::string buildSearchBody(const SearchQuery& query) {
    std::ostringstream os;
    os << "{\"search_categories\":{";
    // For each category the user requested
    bool firstCat = true;
    for (auto cat : query.categories) {
        if (!firstCat) os << ",";
        firstCat = false;
        const char* catName = searchCategoryString(cat);
        if (cat == SearchCategory::ROOM_MESSAGES) {
            os << "\"" << catName << "\":" << buildSearchQuery(query);
        } else {
            // ROOM_NAMES and USERS use a simpler body
            os << "\"" << catName << "\":{";
            os << "\"search_term\":\"" << escJson(query.searchTerm) << "\"";
            os << "}";
        }
    }
    os << "}}";
    return os.str();
}

// ============ Search JSON parsers ============

// Helper: clone JSON object string from the raw JSON
static std::string cloneJsonObject(const std::string& json, size_t start) {
    if (start >= json.size() || json[start] != '{') return {};
    int depth = 0;
    size_t end = start;
    while (end < json.size()) {
        if (json[end] == '{') ++depth;
        else if (json[end] == '}') { --depth; if (depth == 0) { ++end; break; } }
        ++end;
    }
    return json.substr(start, end - start);
}

// Original Kotlin: parses the full /search API response
SearchResponse parseSearchResponse(const std::string& json) {
    SearchResponse resp;

    // Navigate: search_categories → room_events
    auto roomEventsRaw = extractJsonRaw(json, "room_events");
    if (roomEventsRaw.empty()) {
        // Try inner path: search_categories.{room_events}
        auto scRaw = extractJsonRaw(json, "search_categories");
        if (!scRaw.empty() && scRaw[0] == '{') {
            roomEventsRaw = extractJsonRaw(scRaw, "room_events");
        }
    }

    resp.roomEventsJson = roomEventsRaw;

    if (!roomEventsRaw.empty()) {
        auto nextBatch = extractJsonValue(roomEventsRaw, "next_batch");
        resp.nextBatch = nextBatch;

        auto countStr = extractJsonValue(roomEventsRaw, "count");
        if (!countStr.empty()) resp.count = std::stoi(countStr);

        // highlights array
        auto highlightsRaw = extractJsonRaw(roomEventsRaw, "highlights");
        if (!highlightsRaw.empty() && highlightsRaw[0] == '[') {
            size_t pos = 1;
            while (pos < highlightsRaw.size()) {
                while (pos < highlightsRaw.size() && (highlightsRaw[pos] == ' ' || highlightsRaw[pos] == '\t' || highlightsRaw[pos] == ',' || highlightsRaw[pos] == '\n')) ++pos;
                if (pos >= highlightsRaw.size() || highlightsRaw[pos] == ']') break;
                if (highlightsRaw[pos] == '"') {
                    ++pos;
                    size_t end = pos;
                    while (end < highlightsRaw.size() && highlightsRaw[end] != '"') {
                        if (highlightsRaw[end] == '\\') ++end;
                        ++end;
                    }
                    resp.highlights.push_back(highlightsRaw.substr(pos, end - pos));
                    pos = end + 1;
                } else {
                    ++pos;
                }
            }
        }
    }

    return resp;
}

// Original Kotlin: parses a single SearchResponseItem from its JSON object
SearchResponseItem parseSearchResponseItem(const std::string& json) {
    SearchResponseItem item;

    auto rankStr = extractJsonValue(json, "rank");
    if (!rankStr.empty()) item.rank = std::stod(rankStr);

    // result (the event itself)
    auto resultRaw = extractJsonRaw(json, "result");
    item.resultJson = resultRaw;

    // context
    auto contextRaw = extractJsonRaw(json, "context");
    item.contextJson = contextRaw;

    // Parse convenience fields from the event
    if (!resultRaw.empty()) {
        item.eventId = extractJsonValue(resultRaw, "event_id");
        item.roomId = extractJsonValue(resultRaw, "room_id");
        item.senderId = extractJsonValue(resultRaw, "sender");

        auto tsStr = extractJsonValue(resultRaw, "origin_server_ts");
        if (!tsStr.empty()) item.originServerTs = std::stoll(tsStr);
    }

    return item;
}

// Original Kotlin: parses event context object
SearchEventContext parseSearchContext(const std::string& json) {
    SearchEventContext ctx;

    // events_before array
    auto beforeRaw = extractJsonRaw(json, "events_before");
    if (!beforeRaw.empty() && beforeRaw[0] == '[') {
        size_t pos = 1;
        while (pos < beforeRaw.size()) {
            while (pos < beforeRaw.size() && (beforeRaw[pos] == ' ' || beforeRaw[pos] == '\t' || beforeRaw[pos] == ',' || beforeRaw[pos] == '\n')) ++pos;
            if (pos >= beforeRaw.size() || beforeRaw[pos] == ']') break;
            if (beforeRaw[pos] == '{') {
                auto objJson = cloneJsonObject(beforeRaw, pos);
                ctx.eventsBefore.push_back(objJson);
                pos += objJson.size();
            } else {
                ++pos;
            }
        }
    }

    // events_after array
    auto afterRaw = extractJsonRaw(json, "events_after");
    if (!afterRaw.empty() && afterRaw[0] == '[') {
        size_t pos = 1;
        while (pos < afterRaw.size()) {
            while (pos < afterRaw.size() && (afterRaw[pos] == ' ' || afterRaw[pos] == '\t' || afterRaw[pos] == ',' || afterRaw[pos] == '\n')) ++pos;
            if (pos >= afterRaw.size() || afterRaw[pos] == ']') break;
            if (afterRaw[pos] == '{') {
                auto objJson = cloneJsonObject(afterRaw, pos);
                ctx.eventsAfter.push_back(objJson);
                pos += objJson.size();
            } else {
                ++pos;
            }
        }
    }

    ctx.start = extractJsonValue(json, "start");
    ctx.end = extractJsonValue(json, "end");

    // profile_info: {userId: {displayname: ..., avatar_url: ...}}
    auto profileRaw = extractJsonRaw(json, "profile_info");
    if (!profileRaw.empty() && profileRaw[0] == '{') {
        size_t pos = 1;
        while (pos < profileRaw.size()) {
            while (pos < profileRaw.size() && (profileRaw[pos] == ' ' || profileRaw[pos] == '\t' || profileRaw[pos] == ',' || profileRaw[pos] == '\n')) ++pos;
            if (pos >= profileRaw.size() || profileRaw[pos] == '}') break;
            if (profileRaw[pos] == '"') {
                // userId key
                ++pos;
                size_t keyEnd = pos;
                while (keyEnd < profileRaw.size() && profileRaw[keyEnd] != '"') ++keyEnd;
                std::string userId = profileRaw.substr(pos, keyEnd - pos);
                pos = keyEnd + 1;
                while (pos < profileRaw.size() && profileRaw[pos] != ':') ++pos;
                ++pos;
                while (pos < profileRaw.size() && (profileRaw[pos] == ' ' || profileRaw[pos] == '\t' || profileRaw[pos] == '\n')) ++pos;
                if (pos < profileRaw.size() && profileRaw[pos] == '{') {
                    auto innerObj = cloneJsonObject(profileRaw, pos);
                    std::unordered_map<std::string, std::string> fields;
                    auto dn = extractJsonValue(innerObj, "displayname");
                    if (!dn.empty()) fields["displayname"] = dn;
                    auto av = extractJsonValue(innerObj, "avatar_url");
                    if (!av.empty()) fields["avatar_url"] = av;
                    ctx.profileInfo[userId] = std::move(fields);
                    pos += innerObj.size();
                }
            } else {
                ++pos;
            }
        }
    }

    return ctx;
}

} // namespace progressive
