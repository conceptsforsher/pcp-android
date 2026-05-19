#ifndef PROGRESSIVE_EVENTCACHE_HPP
#define PROGRESSIVE_EVENTCACHE_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>

namespace progressive {

struct CachedEvent {
    std::string eventId;
    std::string senderId;
    std::string senderName;
    std::string timestamp;
    std::string body;
    std::string msgType;
    std::string eventType;
    std::string relationType;
    std::string sourceEventId;
    std::string formattedBody;
    bool isEncrypted = false;
    bool sentByMe = false;
    bool hasFailed = false;
    int reactionCount = 0;
    bool hasBeenEdited = false;
};

class EventCache {
public:
    // Insert or update a cached event. Called when timeline loads events.
    void put(const CachedEvent& event);

    // Get a single event by ID. Returns nullptr if not cached.
    const CachedEvent* get(const std::string& eventId) const;

    // Get all events that react to / relate to a given source event.
    std::vector<const CachedEvent*> getRelations(const std::string& sourceEventId) const;

    // Get quick context menu data for a given eventId.
    // Returns JSON string with all fields needed for Stage 2 context menu.
    std::string getContextData(const std::string& eventId) const;

    // Clear all cached data.
    void clear();

    // Number of cached events.
    size_t size() const { return events_.size(); }

private:
    std::unordered_map<std::string, CachedEvent> events_;
    // Reverse index: source event → events that relate to it
    std::unordered_map<std::string, std::vector<std::string>> relationIndex_;
};

} // namespace progressive

#endif // PROGRESSIVE_EVENTCACHE_HPP
