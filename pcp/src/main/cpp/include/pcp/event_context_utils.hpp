#pragma once
#include <string>
#include <vector>

namespace pcp {

struct EventContext {
    std::string eventId;
    std::vector<std::string> beforeEvents;   // events before
    std::vector<std::string> afterEvents;    // events after
    std::string roomId;
};

// Parse event context API response
EventContext parseEventContext(const std::string& json);

// Build event context request
std::string buildContextRequest(int limit = 10);

// Format context summary
std::string formatContextSummary(const EventContext& ctx);

} // namespace pcp
