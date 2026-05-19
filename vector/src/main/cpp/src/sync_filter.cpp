#include "progressive/sync_filter.hpp"

#include <sstream>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <functional>

namespace progressive {

// ============================================================
//  Internal JSON Helper Functions
// ============================================================

namespace {

// Escape a string for JSON (handle \ " and control chars).
std::string jsonEscape(const std::string& s) {
    std::string out;
    out.reserve(s.size() + 2);
    for (char c : s) {
        switch (c) {
            case '"':  out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\n': out += "\\n";  break;
            case '\r': out += "\\r";  break;
            case '\t': out += "\\t";  break;
            default:
                if (static_cast<unsigned char>(c) < 0x20)
                    out += "\\u00" + std::string(1, "0123456789abcdef"[(c >> 4) & 0xf])
                                    + std::string(1, "0123456789abcdef"[c & 0xf]);
                else
                    out += c;
                break;
        }
    }
    return out;
}

// Write a JSON string entry: "key": "value"
void writeJsonStringEntry(std::ostringstream& json, bool& first,
                          const char* key, const std::string& value) {
    if (!first) json << ",";
    json << R"(")" << key << R"(":")" << jsonEscape(value) << R"(")";
    first = false;
}

// Write a JSON bool entry: "key": true|false
void writeJsonBoolEntry(std::ostringstream& json, bool& first,
                        const char* key, bool value) {
    if (!first) json << ",";
    json << R"(")" << key << R"(":)" << (value ? "true" : "false");
    first = false;
}

// Write a JSON int entry: "key": <n>
void writeJsonIntEntry(std::ostringstream& json, bool& first,
                       const char* key, int value) {
    if (!first) json << ",";
    json << R"(")" << key << R"(":)" << value;
    first = false;
}

// Write a JSON string array entry: "key": ["a", "b"]
void writeJsonStringArrayEntry(std::ostringstream& json, bool& first,
                               const char* key, const std::vector<std::string>& values) {
    if (!first) json << ",";
    json << R"(")" << key << R"(":[)";
    for (size_t i = 0; i < values.size(); ++i) {
        if (i > 0) json << ",";
        json << R"(")" << jsonEscape(values[i]) << R"(")";
    }
    json << "]";
    first = false;
}

// Check if an optional string vector has content (not nullopt and not empty).
bool stringVecHasData(const std::optional<std::vector<std::string>>& v) {
    return v.has_value() && !v->empty();
}

} // anonymous namespace

// ============================================================
//  EventFilter
// ============================================================
// Original Kotlin: EventFilter.kt

bool EventFilter::hasData() const {
    return limit.has_value() ||
           stringVecHasData(notSenders) ||
           stringVecHasData(senders) ||
           stringVecHasData(types) ||
           stringVecHasData(notTypes);
}

std::string EventFilter::toJson() const {
    if (!hasData()) return "{}";

    std::ostringstream json;
    json << "{";
    bool first = true;

    if (limit.has_value())
        writeJsonIntEntry(json, first, "limit", *limit);

    if (stringVecHasData(notSenders))
        writeJsonStringArrayEntry(json, first, "not_senders", *notSenders);

    if (stringVecHasData(senders))
        writeJsonStringArrayEntry(json, first, "senders", *senders);

    if (stringVecHasData(types))
        writeJsonStringArrayEntry(json, first, "types", *types);

    if (stringVecHasData(notTypes))
        writeJsonStringArrayEntry(json, first, "not_types", *notTypes);

    json << "}";
    return json.str();
}

// ============================================================
//  RoomEventFilter
// ============================================================
// Original Kotlin: RoomEventFilter.kt (105 lines)

bool RoomEventFilter::hasData() const {
    return limit.has_value() ||
           stringVecHasData(notSenders) ||
           stringVecHasData(senders) ||
           stringVecHasData(types) ||
           stringVecHasData(notTypes) ||
           stringVecHasData(rooms) ||
           stringVecHasData(notRooms) ||
           containsUrl.has_value() ||
           url.has_value() ||
           lazyLoadMembers.has_value() ||
           includeRedundantMembers.has_value() ||
           unreadThreadNotifications.has_value();
}

std::string RoomEventFilter::toJson() const {
    if (!hasData()) return "{}";

    std::ostringstream json;
    json << "{";
    bool first = true;

    if (limit.has_value())
        writeJsonIntEntry(json, first, "limit", *limit);

    if (stringVecHasData(notSenders))
        writeJsonStringArrayEntry(json, first, "not_senders", *notSenders);

    if (stringVecHasData(senders))
        writeJsonStringArrayEntry(json, first, "senders", *senders);

    if (stringVecHasData(types))
        writeJsonStringArrayEntry(json, first, "types", *types);

    if (stringVecHasData(notTypes))
        writeJsonStringArrayEntry(json, first, "not_types", *notTypes);

    if (stringVecHasData(rooms))
        writeJsonStringArrayEntry(json, first, "rooms", *rooms);

    if (stringVecHasData(notRooms))
        writeJsonStringArrayEntry(json, first, "not_rooms", *notRooms);

    if (containsUrl.has_value())
        writeJsonBoolEntry(json, first, "contains_url", *containsUrl);

    if (url.has_value() && !url->empty())
        writeJsonStringEntry(json, first, "url", *url);

    if (lazyLoadMembers.has_value())
        writeJsonBoolEntry(json, first, "lazy_load_members", *lazyLoadMembers);

    if (includeRedundantMembers.has_value())
        writeJsonBoolEntry(json, first, "include_redundant_members", *includeRedundantMembers);

    // Original Kotlin: enableUnreadThreadNotifications
    if (unreadThreadNotifications.has_value())
        writeJsonBoolEntry(json, first, "unread_thread_notifications", *unreadThreadNotifications);

    json << "}";
    return json.str();
}

// ============================================================
//  StateFilter
// ============================================================
// Original Kotlin: RoomEventFilter (state filter mirrors RoomEventFilter per spec)

bool StateFilter::hasData() const {
    return limit.has_value() ||
           stringVecHasData(notSenders) ||
           stringVecHasData(senders) ||
           stringVecHasData(types) ||
           stringVecHasData(notTypes) ||
           lazyLoadMembers.has_value() ||
           includeRedundantMembers.has_value() ||
           unreadThreadNotifications.has_value() ||
           stringVecHasData(rooms) ||
           stringVecHasData(notRooms) ||
           containsUrl.has_value();
}

std::string StateFilter::toJson() const {
    if (!hasData()) return "{}";

    std::ostringstream json;
    json << "{";
    bool first = true;

    if (limit.has_value())
        writeJsonIntEntry(json, first, "limit", *limit);

    if (stringVecHasData(notSenders))
        writeJsonStringArrayEntry(json, first, "not_senders", *notSenders);

    if (stringVecHasData(senders))
        writeJsonStringArrayEntry(json, first, "senders", *senders);

    if (stringVecHasData(types))
        writeJsonStringArrayEntry(json, first, "types", *types);

    if (stringVecHasData(notTypes))
        writeJsonStringArrayEntry(json, first, "not_types", *notTypes);

    if (lazyLoadMembers.has_value())
        writeJsonBoolEntry(json, first, "lazy_load_members", *lazyLoadMembers);

    if (includeRedundantMembers.has_value())
        writeJsonBoolEntry(json, first, "include_redundant_members", *includeRedundantMembers);

    if (unreadThreadNotifications.has_value())
        writeJsonBoolEntry(json, first, "unread_thread_notifications", *unreadThreadNotifications);

    if (stringVecHasData(rooms))
        writeJsonStringArrayEntry(json, first, "rooms", *rooms);

    if (stringVecHasData(notRooms))
        writeJsonStringArrayEntry(json, first, "not_rooms", *notRooms);

    if (containsUrl.has_value())
        writeJsonBoolEntry(json, first, "contains_url", *containsUrl);

    json << "}";
    return json.str();
}

// ============================================================
//  RoomFilter
// ============================================================
// Original Kotlin: RoomFilter.kt (70 lines)

bool RoomFilter::hasData() const {
    return stringVecHasData(rooms) ||
           stringVecHasData(notRooms) ||
           (ephemeral.has_value() && ephemeral->hasData()) ||
           (state.has_value() && state->hasData()) ||
           (timeline.has_value() && timeline->hasData()) ||
           (accountData.has_value() && accountData->hasData()) ||
           includeLeave.has_value() ||
           unreadThreadNotifications.has_value();
}

std::string RoomFilter::toJson() const {
    if (!hasData()) return "{}";

    std::ostringstream json;
    json << "{";
    bool first = true;

    if (stringVecHasData(rooms))
        writeJsonStringArrayEntry(json, first, "rooms", *rooms);

    if (stringVecHasData(notRooms))
        writeJsonStringArrayEntry(json, first, "not_rooms", *notRooms);

    // Original Kotlin: @Json(name = "ephemeral")
    if (ephemeral.has_value() && ephemeral->hasData()) {
        if (!first) json << ",";
        json << R"("ephemeral":)" << ephemeral->toJson();
        first = false;
    }

    // Original Kotlin: @Json(name = "state")
    if (state.has_value() && state->hasData()) {
        if (!first) json << ",";
        json << R"("state":)" << state->toJson();
        first = false;
    }

    // Original Kotlin: @Json(name = "timeline")
    if (timeline.has_value() && timeline->hasData()) {
        if (!first) json << ",";
        json << R"("timeline":)" << timeline->toJson();
        first = false;
    }

    // Original Kotlin: @Json(name = "account_data")
    if (accountData.has_value() && accountData->hasData()) {
        if (!first) json << ",";
        json << R"("account_data":)" << accountData->toJson();
        first = false;
    }

    if (includeLeave.has_value())
        writeJsonBoolEntry(json, first, "include_leave", *includeLeave);

    if (unreadThreadNotifications.has_value())
        writeJsonBoolEntry(json, first, "unread_thread_notifications", *unreadThreadNotifications);

    json << "}";
    return json.str();
}

// ============================================================
//  FilterBody
// ============================================================
// Original Kotlin: Filter.kt (58 lines)

bool FilterBody::hasData() const {
    return stringVecHasData(eventFields) ||
           (eventFormat.has_value() && !eventFormat->empty()) ||
           (presence.has_value() && presence->hasData()) ||
           (accountData.has_value() && accountData->hasData()) ||
           (room.has_value() && room->hasData());
}

std::string FilterBody::toJson() const {
    if (!hasData()) return "{}";

    std::ostringstream json;
    json << "{";
    bool first = true;

    // Original Kotlin: @Json(name = "event_fields")
    if (stringVecHasData(eventFields))
        writeJsonStringArrayEntry(json, first, "event_fields", *eventFields);

    // Original Kotlin: @Json(name = "event_format")
    if (eventFormat.has_value() && !eventFormat->empty())
        writeJsonStringEntry(json, first, "event_format", *eventFormat);

    // Original Kotlin: @Json(name = "presence")
    if (presence.has_value() && presence->hasData()) {
        if (!first) json << ",";
        json << R"("presence":)" << presence->toJson();
        first = false;
    }

    // Original Kotlin: @Json(name = "account_data")
    if (accountData.has_value() && accountData->hasData()) {
        if (!first) json << ",";
        json << R"("account_data":)" << accountData->toJson();
        first = false;
    }

    // Original Kotlin: @Json(name = "room")
    if (room.has_value() && room->hasData()) {
        if (!first) json << ",";
        json << R"("room":)" << room->toJson();
        first = false;
    }

    json << "}";
    return json.str();
}

// ============================================================
//  Filter Builders (JSON)
// ============================================================

std::string buildFilterBody(const FilterBody& filter) {
    // Original Kotlin: Filter.toJSONString()
    return filter.toJson();
}

std::string buildRoomFilter(const RoomFilter& filter) {
    return filter.toJson();
}

std::string buildRoomEventFilter(const RoomEventFilter& filter) {
    return filter.toJson();
}

std::string buildStateFilter(const StateFilter& filter) {
    return filter.toJson();
}

std::string buildEventFilter(const EventFilter& filter) {
    return filter.toJson();
}

std::string buildTimelineFilter(const RoomEventFilter& timelineFilter) {
    // Convenience: wraps RoomEventFilter in a FilterBody with only timeline.
    // Original Kotlin: equivalent to Filter(room = RoomFilter(timeline = ...))
    RoomFilter roomFilter;
    roomFilter.timeline = timelineFilter;

    FilterBody filter;
    filter.room = roomFilter;

    return buildFilterBody(filter);
}

// ============================================================
//  buildFilterBodyFromParams
// ============================================================
// Original Kotlin: SyncFilterBuilder.kt (119 lines)
//
//  fun build(homeServerCapabilities): Filter {
//      return Filter(room = buildRoomFilter(homeServerCapabilities))
//  }
//  private fun buildRoomFilter(caps): RoomFilter {
//      return RoomFilter(timeline = buildTimelineFilter(caps), state = buildStateFilter())
//  }

FilterBody buildFilterBodyFromParams(
    const SyncFilterParams& params,
    bool canUseThreadReadReceiptsAndNotifications
) {
    // Original Kotlin (SyncFilterBuilder.kt:70-79): buildTimelineFilter()
    //   val resolvedUseThreadNotifications = if (caps.canUseThread...)
    //       useThreadNotifications else null
    bool useThreads = canUseThreadReadReceiptsAndNotifications &&
                      params.useThreadNotifications.has_value() &&
                      *params.useThreadNotifications;

    RoomEventFilter timelineFilter;
    timelineFilter.lazyLoadMembers = params.lazyLoadMembersForMessageEvents;
    if (useThreads)
        timelineFilter.unreadThreadNotifications = true;
    if (!params.listOfSupportedEventTypes.empty())
        timelineFilter.types = params.listOfSupportedEventTypes;

    // Original Kotlin (SyncFilterBuilder.kt:82-86): buildStateFilter()
    //   RoomEventFilter(lazyLoadMembers = lazyLoadMembersForStateEvents,
    //                   types = listOfSupportedStateEventTypes).orNullIfEmpty()
    StateFilter stateFilter;
    stateFilter.lazyLoadMembers = params.lazyLoadMembersForStateEvents;
    if (!params.listOfSupportedStateEventTypes.empty())
        stateFilter.types = params.listOfSupportedStateEventTypes;

    RoomFilter roomFilter;
    if (timelineFilter.hasData()) roomFilter.timeline = timelineFilter;
    if (stateFilter.hasData()) roomFilter.state = stateFilter;

    FilterBody filter;
    // Original Kotlin: eventFormat defaults to "client"
    filter.eventFormat = EVENT_FORMAT_CLIENT;
    if (roomFilter.hasData()) filter.room = roomFilter;

    return filter;
}

// ============================================================
//  JSON Parsing Helpers
// ============================================================
// Minimal recursive-descent JSON parser for filter payloads.
// Handles: objects, arrays, strings, ints, bools, null.

namespace {

class FilterJsonParser {
public:
    explicit FilterJsonParser(const std::string& json) : json_(json), pos_(0) {}

    bool isDone() { skipWhitespace(); return pos_ >= json_.size(); }

    // ---- Value readers ----

    std::string readString() {
        skipWhitespace();
        expect('"');
        std::string result;
        while (pos_ < json_.size()) {
            char c = json_[pos_++];
            if (c == '"') return result;
            if (c == '\\') {
                if (pos_ >= json_.size()) break;
                char esc = json_[pos_++];
                switch (esc) {
                    case '"':  result += '"';  break;
                    case '\\': result += '\\'; break;
                    case '/':  result += '/';  break;
                    case 'n':  result += '\n'; break;
                    case 'r':  result += '\r'; break;
                    case 't':  result += '\t'; break;
                    case 'u': {
                        // Simple \uXXXX handler
                        if (pos_ + 4 <= json_.size()) {
                            std::string hex = json_.substr(pos_, 4);
                            pos_ += 4;
                            result += static_cast<char>(std::strtol(hex.c_str(), nullptr, 16));
                        }
                        break;
                    }
                    default: result += esc; break;
                }
            } else {
                result += c;
            }
        }
        return result;
    }

    int readInt() {
        skipWhitespace();
        bool neg = false;
        if (peek() == '-') { neg = true; pos_++; }
        int val = 0;
        while (pos_ < json_.size() && std::isdigit(static_cast<unsigned char>(json_[pos_]))) {
            val = val * 10 + (json_[pos_] - '0');
            pos_++;
        }
        return neg ? -val : val;
    }

    bool readBool() {
        skipWhitespace();
        if (match("true")) return true;
        if (match("false")) return false;
        throw std::runtime_error("Expected true or false");
    }

    void readNull() {
        skipWhitespace();
        if (!match("null"))
            throw std::runtime_error("Expected null");
    }

    std::vector<std::string> readStringArray() {
        skipWhitespace();
        expect('[');
        std::vector<std::string> result;
        skipWhitespace();
        if (peek() == ']') { pos_++; return result; }
        for (;;) {
            result.push_back(readString());
            skipWhitespace();
            if (peek() == ',') { pos_++; continue; }
            if (peek() == ']') { pos_++; break; }
        }
        return result;
    }

    // ---- Object readers ---

    // Enter an object: expects '{', reads keys until '}'
    // Calls keyHandler(key) for each key; caller must read the value.
    void readObject(const std::function<void(const std::string& key)>& keyHandler) {
        skipWhitespace();
        expect('{');
        skipWhitespace();
        if (peek() == '}') { pos_++; return; }
        for (;;) {
            std::string key = readString();
            skipWhitespace();
            expect(':');
            keyHandler(key);
            skipWhitespace();
            if (peek() == ',') { pos_++; skipWhitespace(); continue; }
            if (peek() == '}') { pos_++; break; }
        }
    }

    // Helpers to determine value type at current position
    bool peekNull() { skipWhitespace(); return peek(0) == 'n'; }
    bool peekBool() { skipWhitespace(); char c = peek(); return c == 't' || c == 'f'; }
    bool peekInt() { skipWhitespace(); char c = peek(); return std::isdigit(c) || c == '-'; }
    bool peekString() { skipWhitespace(); return peek() == '"'; }
    bool peekObject() { skipWhitespace(); return peek() == '{'; }
    bool peekArray() { skipWhitespace(); return peek() == '['; }

private:
    char peek(size_t offset = 0) const {
        size_t idx = pos_ + offset;
        return idx < json_.size() ? json_[idx] : '\0';
    }

    void skipWhitespace() {
        while (pos_ < json_.size() && std::isspace(static_cast<unsigned char>(json_[pos_])))
            pos_++;
    }

    void expect(char c) {
        skipWhitespace();
        if (pos_ >= json_.size() || json_[pos_] != c)
            throw std::runtime_error(std::string("Expected '") + c + "'");
        pos_++;
    }

    bool match(const char* s) {
        skipWhitespace();
        size_t len = std::strlen(s);
        if (pos_ + len <= json_.size() && json_.compare(pos_, len, s) == 0) {
            pos_ += len;
            return true;
        }
        return false;
    }

    const std::string& json_;
    size_t pos_;
};

// Read optional int from value position
void readOptionalInt(FilterJsonParser& p, std::optional<int>& out) {
    out = p.readInt();
}

// Read optional string from value position
void readOptionalString(FilterJsonParser& p, std::optional<std::string>& out) {
    out = p.readString();
}

// Read optional bool from value position
void readOptionalBool(FilterJsonParser& p, std::optional<bool>& out) {
    out = p.readBool();
}

// Read optional string array from value position
void readOptionalStringArray(FilterJsonParser& p, std::optional<std::vector<std::string>>& out) {
    auto arr = p.readStringArray();
    if (!arr.empty())
        out = std::move(arr);
}

// ---- Parse individual sub-filters from current JSON position ----

EventFilter parseJsonEventFilter(FilterJsonParser& p) {
    EventFilter f;
    p.readObject([&](const std::string& key) {
        if (key == "limit")          readOptionalInt(p, f.limit);
        else if (key == "not_senders") readOptionalStringArray(p, f.notSenders);
        else if (key == "senders")     readOptionalStringArray(p, f.senders);
        else if (key == "types")       readOptionalStringArray(p, f.types);
        else if (key == "not_types")   readOptionalStringArray(p, f.notTypes);
        else { p.readNull(); /* skip unknown */ }
    });
    return f;
}

RoomEventFilter parseJsonRoomEventFilter(FilterJsonParser& p) {
    RoomEventFilter f;
    p.readObject([&](const std::string& key) {
        if (key == "limit")                      readOptionalInt(p, f.limit);
        else if (key == "not_senders")           readOptionalStringArray(p, f.notSenders);
        else if (key == "senders")               readOptionalStringArray(p, f.senders);
        else if (key == "types")                 readOptionalStringArray(p, f.types);
        else if (key == "not_types")             readOptionalStringArray(p, f.notTypes);
        else if (key == "rooms")                 readOptionalStringArray(p, f.rooms);
        else if (key == "not_rooms")             readOptionalStringArray(p, f.notRooms);
        else if (key == "contains_url")          readOptionalBool(p, f.containsUrl);
        else if (key == "url")                   readOptionalString(p, f.url);
        else if (key == "lazy_load_members")     readOptionalBool(p, f.lazyLoadMembers);
        else if (key == "include_redundant_members") readOptionalBool(p, f.includeRedundantMembers);
        else if (key == "unread_thread_notifications") readOptionalBool(p, f.unreadThreadNotifications);
        else { p.readNull(); /* skip unknown */ }
    });
    return f;
}

StateFilter parseJsonStateFilter(FilterJsonParser& p) {
    StateFilter f;
    p.readObject([&](const std::string& key) {
        if (key == "limit")                      readOptionalInt(p, f.limit);
        else if (key == "not_senders")           readOptionalStringArray(p, f.notSenders);
        else if (key == "senders")               readOptionalStringArray(p, f.senders);
        else if (key == "types")                 readOptionalStringArray(p, f.types);
        else if (key == "not_types")             readOptionalStringArray(p, f.notTypes);
        else if (key == "lazy_load_members")     readOptionalBool(p, f.lazyLoadMembers);
        else if (key == "include_redundant_members") readOptionalBool(p, f.includeRedundantMembers);
        else if (key == "unread_thread_notifications") readOptionalBool(p, f.unreadThreadNotifications);
        else if (key == "rooms")                 readOptionalStringArray(p, f.rooms);
        else if (key == "not_rooms")             readOptionalStringArray(p, f.notRooms);
        else if (key == "contains_url")          readOptionalBool(p, f.containsUrl);
        else { p.readNull(); /* skip unknown */ }
    });
    return f;
}

RoomFilter parseJsonRoomFilter(FilterJsonParser& p) {
    RoomFilter f;
    p.readObject([&](const std::string& key) {
        if (key == "rooms")                      readOptionalStringArray(p, f.rooms);
        else if (key == "not_rooms")             readOptionalStringArray(p, f.notRooms);
        else if (key == "ephemeral") {
            f.ephemeral = parseJsonRoomEventFilter(p);
        }
        else if (key == "state") {
            f.state = parseJsonStateFilter(p);
        }
        else if (key == "timeline") {
            f.timeline = parseJsonRoomEventFilter(p);
        }
        else if (key == "account_data") {
            f.accountData = parseJsonRoomEventFilter(p);
        }
        else if (key == "include_leave")         readOptionalBool(p, f.includeLeave);
        else if (key == "unread_thread_notifications") readOptionalBool(p, f.unreadThreadNotifications);
        else { p.readNull(); /* skip unknown */ }
    });
    return f;
}

} // anonymous namespace

// ============================================================
//  Filter Parsers
// ============================================================

FilterResponse parseFilterResponse(const std::string& json) {
    // Original Kotlin: FilterResponse(filterId = ...)
    // Parses: {"filter_id": "abc123"}
    FilterResponse response;
    FilterJsonParser p(json);
    p.readObject([&](const std::string& key) {
        if (key == "filter_id")
            response.filterId = p.readString();
        else
            p.readNull(); // skip unknown
    });
    return response;
}

FilterBody parseFilterBody(const std::string& json) {
    // Original Kotlin: Moshi-parse of Filter JSON
    FilterBody filter;
    FilterJsonParser p(json);
    p.readObject([&](const std::string& key) {
        if (key == "event_fields")         readOptionalStringArray(p, filter.eventFields);
        else if (key == "event_format")    readOptionalString(p, filter.eventFormat);
        else if (key == "presence") {
            filter.presence = parseJsonEventFilter(p);
        }
        else if (key == "account_data") {
            filter.accountData = parseJsonEventFilter(p);
        }
        else if (key == "room") {
            filter.room = parseJsonRoomFilter(p);
        }
        else { p.readNull(); /* skip unknown */ }
    });
    return filter;
}

// ============================================================
//  Lazy Loading
// ============================================================
// Original Kotlin: FilterUtil.kt (111 lines)
//
//  fun enableLazyLoading(filter: Filter, useLazyLoading: Boolean): Filter {
//      if (useLazyLoading) {
//          return filter.copy(
//              room = filter.room?.copy(
//                  state = filter.room.state?.copy(lazyLoadMembers = true)
//                      ?: RoomEventFilter(lazyLoadMembers = true)
//              ) ?: RoomFilter(state = RoomEventFilter(lazyLoadMembers = true))
//          )
//      } else {
//          val newRoomEventFilter = filter.room?.state?.copy(lazyLoadMembers = null)
//              ?.takeIf { it.hasData() }
//          val newRoomFilter = filter.room?.copy(state = newRoomEventFilter)
//              ?.takeIf { it.hasData() }
//          return filter.copy(room = newRoomFilter)
//      }
//  }

FilterBody buildLazyLoadingFilter(bool useLazyLoading) {
    FilterBody filter;

    // Original Kotlin: event_format defaults to "client"
    filter.eventFormat = EVENT_FORMAT_CLIENT;

    if (useLazyLoading) {
        // Original Kotlin:
        //   room = RoomFilter(state = RoomEventFilter(lazyLoadMembers = true))
        StateFilter stateFilter;
        stateFilter.lazyLoadMembers = true;

        RoomFilter roomFilter;
        roomFilter.state = stateFilter;

        filter.room = roomFilter;
    }
    // else: empty filter (no lazy loading)

    return filter;
}

bool isLazyLoadingEnabled(const FilterBody& filter) {
    // Original Kotlin: checks filter.room?.state?.lazyLoadMembers
    if (!filter.room.has_value()) return false;
    if (!filter.room->state.has_value()) return false;
    return filter.room->state->lazyLoadMembers.has_value() &&
           *filter.room->state->lazyLoadMembers;
}

bool isLazyLoadingEnabled(const RoomEventFilter& filter) {
    // Original Kotlin: checks filter.lazyLoadMembers
    return filter.lazyLoadMembers.has_value() && *filter.lazyLoadMembers;
}

// ============================================================
//  Unread Thread Notifications (MSC3773)
// ============================================================
// Original Kotlin: enableUnreadThreadNotifications in RoomEventFilter

FilterBody buildUnreadThreadFilter(bool enable) {
    FilterBody filter;

    // Original Kotlin: event_format defaults to "client"
    filter.eventFormat = EVENT_FORMAT_CLIENT;

    // Original Kotlin:
    //   RoomEventFilter(enableUnreadThreadNotifications = enable)
    RoomEventFilter timelineFilter;
    timelineFilter.unreadThreadNotifications = enable;

    // Also set on the RoomFilter level for servers that support it there
    RoomFilter roomFilter;
    roomFilter.timeline = timelineFilter;
    if (enable)
        roomFilter.unreadThreadNotifications = enable;

    filter.room = roomFilter;
    return filter;
}

// ============================================================
//  Default Filters & Utilities
// ============================================================
// Original Kotlin: FilterFactory.kt (51 lines)

FilterBody getDefaultFilter() {
    // Original Kotlin:
    //   FilterUtil.enableLazyLoading(Filter(), true)
    return buildLazyLoadingFilter(true);
}

RoomEventFilter getDefaultRoomEventFilter() {
    // Original Kotlin: RoomEventFilter(lazyLoadMembers = true)
    RoomEventFilter filter;
    filter.lazyLoadMembers = true;
    return filter;
}

bool hasActiveFiltering(const FilterBody& filter) {
    // Original Kotlin: orNullIfEmpty() — returns true if filter has data
    return filter.hasData();
}

std::string filterBodyToJson(const FilterBody& filter) {
    return filter.toJson();
}

std::string filterResponseToJson(const FilterResponse& response) {
    // Original Kotlin: Moshi serialization of FilterResponse
    std::ostringstream json;
    json << R"({"filter_id":")" << jsonEscape(response.filterId) << R"("})";
    return json.str();
}

} // namespace progressive
