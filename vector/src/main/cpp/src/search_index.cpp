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

} // namespace progressive
