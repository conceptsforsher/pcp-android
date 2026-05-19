#ifndef PROGRESSIVE_SEARCH_INDEX_HPP
#define PROGRESSIVE_SEARCH_INDEX_HPP

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

} // namespace progressive

#endif // PROGRESSIVE_SEARCH_INDEX_HPP
