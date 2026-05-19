#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// ==== Web Search Integration ====
//
// Supports multiple search backends:
//   - SearXNG (self-hosted):     GET /search?q=...&format=json
//   - DuckDuckGo Instant Answer: https://api.duckduckgo.com/?q=...&format=json
//   - Google Custom Search:      https://customsearch.googleapis.com/customsearch/v1?key=...&q=...
//
// All disabled by default. User configures endpoints and API keys in Labs settings.
// Slash command: /web [engine] query
//   /web ddg vitamin b12       → DuckDuckGo
//   /web google python async   → Google
//   /web vitamins              → default engine (first configured)

enum class SearchEngine {
    NONE = 0,
    SEARXNG = 1,     // Self-hosted SearXNG instance
    DUCKDUCKGO = 2,  // DuckDuckGo Instant Answer API
    GOOGLE = 3       // Google Custom Search API
};

const char* searchEngineToString(SearchEngine e);
SearchEngine searchEngineFromString(const std::string& s);

// Per-engine configuration (stored in preferences)
struct SearchEngineConfig {
    SearchEngine engine = SearchEngine::NONE;
    std::string endpointUrl;     // Base URL for API requests
    std::string apiKey;          // For Google: API key; DDG/SearXNG: empty
    std::string engineId;        // For Google: CX (custom search engine ID); DDG/SearXNG: empty
    bool enabled = false;        // Master on/off switch
    int priority = 0;            // Lower = higher priority (default engine)
};

// Set of all configured engines
struct SearchConfig {
    std::vector<SearchEngineConfig> engines;
    SearchEngine defaultEngine = SearchEngine::NONE;
    int maxResults = 10;         // Maximum results per search
    bool agentWebAccess = false;  // Allow /agent to search the web
    bool isEnabled = false;      // Master Labs toggle

    SearchEngineConfig* getEngine(SearchEngine e);
    SearchEngineConfig* getDefaultEngine();
};

// A single search result from any engine
struct SearchResultItem {
    std::string title;
    std::string url;
    std::string snippet;         // Description/summary
    std::string engineName;      // Which engine provided this result
    std::string sourceDomain;    // e.g. "wikipedia.org"
};

// Parsed search response
struct SearchResponse {
    std::string query;
    std::vector<SearchResultItem> results;
    int totalResults = 0;        // Total estimated results
    std::string errorMessage;
    bool success = false;
    SearchEngine engine = SearchEngine::NONE;

    // Format as Matrix message body (plain text or HTML)
    std::string formatAsPlainText() const;
    std::string formatAsHtml() const;
};

// ==== URL Building ====

// Build search URL for each engine type
std::string buildSearxngUrl(const std::string& endpoint, const std::string& query, int maxResults);
std::string buildDuckDuckGoUrl(const std::string& query);
std::string buildGoogleUrl(const std::string& apiKey, const std::string& engineId, const std::string& query, int maxResults);

// ==== Response Parsing ====

// Parse SearXNG JSON response (format=json)
SearchResponse parseSearxngResponse(const std::string& json, const std::string& query);

// Parse DuckDuckGo Instant Answer JSON
SearchResponse parseDuckDuckGoResponse(const std::string& json, const std::string& query);

// Parse Google Custom Search JSON
SearchResponse parseGoogleResponse(const std::string& json, const std::string& query);

// ==== Command Parsing ====

// Parse "/web [engine] query" command
// Returns engine (or NONE for default) and query string
struct WebCommand {
    SearchEngine engine = SearchEngine::NONE;
    std::string query;
    bool isValid = false;
};

WebCommand parseWebCommand(const std::string& commandArgs);

// ==== Agent Web Access ====
//
// When agentWebAccess is enabled, the AI executor can search the web.
// Search results are formatted and injected into the agent's system prompt.

// Format search results for injection into agent context
std::string formatSearchResultsForAgent(const SearchResponse& response);

// Serialize/deserialize search config for preferences storage
std::string searchConfigToJson(const SearchConfig& config);
SearchConfig searchConfigFromJson(const std::string& json);

} // namespace progressive
