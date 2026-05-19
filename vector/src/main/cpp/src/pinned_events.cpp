#include "progressive/pinned_events.hpp"
#include <sstream>
#include <regex>

namespace progressive {

std::vector<std::string> parsePinnedEventIds(const std::string& stateContentJson) {
    std::vector<std::string> ids;

    // Extract "pinned": ["$ev1", "$ev2"]
    auto pinnedPos = stateContentJson.find("\"pinned\"");
    if (pinnedPos == std::string::npos) return ids;

    auto bracket = stateContentJson.find('[', pinnedPos);
    if (bracket == std::string::npos) return ids;

    auto end = stateContentJson.find(']', bracket);
    if (end == std::string::npos) return ids;

    std::string array = stateContentJson.substr(bracket + 1, end - bracket - 1);

    // Extract each "$eventId" manually (NDK 21 regex_iterator is buggy)
    size_t searchPos = 0;
    while (true) {
        auto dollarPos = array.find("$\"", searchPos);
        if (dollarPos == std::string::npos) break;
        auto start = dollarPos + 1; // skip $
        auto end = array.find('"', start);
        if (end == std::string::npos) break;
        ids.push_back(array.substr(start, end - start));
        searchPos = end + 1;
    }

    return ids;
}

std::string formatPinnedEventsText(const std::vector<PinnedEvent>& events) {
    std::ostringstream out;
    out << "Pinned Messages (" << events.size() << ")\n";
    out << "-----------------\n";
    for (size_t i = 0; i < events.size(); ++i) {
        const auto& e = events[i];
        out << (i + 1) << ". " << e.senderName << ": "
            << (e.body.size() > 60 ? e.body.substr(0, 57) + "..." : e.body)
            << "\n";
    }
    return out.str();
}

std::string pinnedEventsToJson(const std::vector<PinnedEvent>& events, const std::string& roomId) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };
    std::ostringstream json;
    json << R"({"roomId": ")" << esc(roomId) << R"(")";
    json << R"(,"pins": [)";
    for (size_t i = 0; i < events.size(); ++i) {
        if (i > 0) json << ",";
        const auto& e = events[i];
        json << R"({"eventId": ")" << esc(e.eventId) << R"(")";
        json << R"(,"pinnedBy": ")" << esc(e.pinnedBy) << R"(")";
        json << R"(,"pinnedAtMs": )" << e.pinnedAtMs;
        json << R"(,"body": ")" << esc(e.body) << R"(")";
        json << R"(,"senderName": ")" << esc(e.senderName) << R"(")";
        json << "}";
    }
    json << "]}";
    return json.str();
}

std::string buildPinnedEventsContent(const std::vector<std::string>& eventIds) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };
    std::ostringstream json;
    json << R"({"pinned": [)";
    for (size_t i = 0; i < eventIds.size(); ++i) {
        if (i > 0) json << ", ";
        json << R"(")" << esc(eventIds[i]) << R"(")";
    }
    json << "]}";
    return json.str();
}

bool canManagePins(int userPowerLevel, int requiredLevel) {
    return userPowerLevel >= requiredLevel;
}

// ================================================================
// Extended Pinned Events API
// ================================================================

// Original Kotlin: parsePinnedEventsContent()
PinnedEventsContent parsePinnedEventsContent(const std::string& stateContentJson) {
    PinnedEventsContent content;
    content.pinned = parsePinnedEventIds(stateContentJson);
    return content;
}

// Original Kotlin: isEventPinned()
bool isEventPinned(const std::string& eventId, const std::vector<std::string>& pinnedIds) {
    for (const auto& id : pinnedIds) {
        if (id == eventId) return true;
    }
    return false;
}

// Original Kotlin: pinEvent()
std::vector<std::string> pinEvent(const std::vector<std::string>& currentPins,
                                   const std::string& eventId) {
    if (isEventPinned(eventId, currentPins)) return currentPins;
    std::vector<std::string> result = currentPins;
    result.push_back(eventId);
    return result;
}

// Original Kotlin: unpinEvent()
std::vector<std::string> unpinEvent(const std::vector<std::string>& currentPins,
                                     const std::string& eventId) {
    std::vector<std::string> result;
    for (const auto& id : currentPins) {
        if (id != eventId) result.push_back(id);
    }
    return result;
}

// Original Kotlin: getPinnedEvents()
std::vector<PinnedEventInfo> getPinnedEvents(const std::string& stateContentJson) {
    auto ids = parsePinnedEventIds(stateContentJson);
    std::vector<PinnedEventInfo> result;
    for (const auto& id : ids) {
        PinnedEventInfo info;
        info.eventId = id;
        info.isPinned = true;
        result.push_back(std::move(info));
    }
    return result;
}

// Original Kotlin: formatPinnedEventsMessage()
std::string formatPinnedEventsMessage(const std::vector<PinnedEvent>& events) {
    return formatPinnedEventsText(events);
}

} // namespace progressive
