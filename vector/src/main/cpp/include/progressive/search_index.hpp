#pragma once

#include <string>
#include <vector>
#include <unordered_map>

namespace progressive {

struct SearchHit {
    std::string eventId;
    std::string roomId;
    std::string roomName;
    std::string senderName;
    std::string body;           // matched body snippet
    std::string timestamp;
    int64_t originServerTs = 0;
    bool isEncrypted = false;
    float relevanceScore = 0.0f; // 0-1
};

struct SearchResult {
    std::string query;
    std::vector<SearchHit> hits;
    int totalHits = 0;
    int roomsSearched = 0;
    int64_t searchTimeMs = 0;
};

class SearchIndex {
public:
    // Index a message (plaintext or decrypted).
    void indexMessage(const std::string& eventId, const std::string& roomId,
                      const std::string& roomName, const std::string& senderName,
                      const std::string& body, int64_t timestamp,
                      bool isEncrypted);

    // Search indexed messages. Returns up to `limit` results sorted by relevance.
    SearchResult search(const std::string& query, int limit = 50) const;

    // Remove messages for a room.
    void removeRoom(const std::string& roomId);

    // Clear index.
    void clear();

    size_t indexedCount() const { return entries_.size(); }

    // Export search hits as JSON.
    static std::string hitsToJson(const std::vector<SearchHit>& hits);

    // Compute relevance score (simple TF-based).
    static float computeRelevance(const std::string& body, const std::string& query);

private:
    struct IndexedMessage {
        std::string eventId;
        std::string roomId;
        std::string roomName;
        std::string senderName;
        std::string body;
        int64_t timestamp = 0;
        bool isEncrypted = false;
    };

    std::vector<IndexedMessage> entries_;
    static std::string toLower(const std::string& s);
};

// ---- Matrix Search API models ----

// Original Kotlin: search categories: room_events, room_names, users
enum class SearchCategory { ROOM_MESSAGES, ROOM_NAMES, USERS };

inline const char* searchCategoryString(SearchCategory cat) {
    switch (cat) {
        case SearchCategory::ROOM_MESSAGES: return "room_events";
        case SearchCategory::ROOM_NAMES:    return "room_names";
        case SearchCategory::USERS:         return "user_directory";
    }
    return "room_events";
}

// Original Kotlin: (grouping for search results)
struct SearchGroup {
    std::string groupBy;
    std::string orderBy = "rank";
};

// Original Kotlin: SearchRequestEventContext.kt
struct EventContextSettings {
    int beforeLimit = 0;
    int afterLimit = 0;
    bool includeProfile = false;
};

// Original Kotlin: SearchTask.Params + SearchRequestRoomEvents.kt
struct SearchQuery {
    std::string searchTerm;
    std::vector<SearchCategory> categories;
    bool orderByRecency = false;
    bool includeProfile = false;
    std::string nextBatch;
    EventContextSettings eventContext;
    int limit = 10;
    std::string roomId; // optional room scope
};

// Original Kotlin: SearchRequestBody.kt wrapper
struct SearchBody {
    // Serialised as JSON: {"search_categories":{"room_events":...}}
    std::string searchCategoriesJson;
    // Optional room filter serialised within room_events
    std::string filterJson;
};

// Original Kotlin: SearchResponse.kt top-level
struct SearchResponse {
    // Parsed room_events category
    std::string nextBatch;
    int count = 0;
    std::vector<std::string> highlights;
    // Raw JSON for the room_events category block
    std::string roomEventsJson;
};

// Original Kotlin: SearchResponseItem.kt (renamed from SearchResult to avoid
// conflict with the local search-index SearchResult)
struct SearchResponseItem {
    double rank = 0.0;
    // Raw event JSON
    std::string resultJson;
    // Raw context JSON
    std::string contextJson;
    // Parsed convenience fields
    std::string eventId;
    std::string roomId;
    std::string senderId;
    int64_t originServerTs = 0;
};

// ---- Search JSON builders ----

// Original Kotlin: builds the search query object within room_events
std::string buildSearchQuery(const SearchQuery& query);

// Original Kotlin: builds the room_events search body subtree
std::string buildRoomEventsSearchBody(const SearchQuery& query);

// Original Kotlin: builds the full POST /search JSON body
std::string buildSearchBody(const SearchQuery& query);

// ---- Search JSON parsers ----

// Original Kotlin: parses the full /search API response
SearchResponse parseSearchResponse(const std::string& json);

// Original Kotlin: parses a single SearchResponseItem from its JSON object
SearchResponseItem parseSearchResponseItem(const std::string& json);

// Original Kotlin: parses an event context object (profile_info, events_before, events_after)
struct SearchEventContext {
    std::vector<std::string> eventsBefore;
    std::vector<std::string> eventsAfter;
    std::string start;
    std::string end;
    // userId -> { "displayname": "...", "avatar_url": "..." }
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> profileInfo;
};

SearchEventContext parseSearchContext(const std::string& json);

} // namespace progressive
