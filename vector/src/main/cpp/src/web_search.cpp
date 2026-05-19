#include "progressive/web_search.hpp"
#include <sstream>

namespace progressive {

// ==== Engine Name Conversions ====

const char* searchEngineToString(SearchEngine e) {
    switch (e) {
        case SearchEngine::SEARXNG: return "searxng";
        case SearchEngine::DUCKDUCKGO: return "ddg";
        case SearchEngine::GOOGLE: return "google";
        case SearchEngine::NONE: return "none";
    }
    return "none";
}

SearchEngine searchEngineFromString(const std::string& s) {
    if (s == "searxng" || s == "sx") return SearchEngine::SEARXNG;
    if (s == "ddg" || s == "duckduckgo") return SearchEngine::DUCKDUCKGO;
    if (s == "google" || s == "g") return SearchEngine::GOOGLE;
    return SearchEngine::NONE;
}

// ==== SearchConfig ====

SearchEngineConfig* SearchConfig::getEngine(SearchEngine e) {
    for (auto& eng : engines)
        if (eng.engine == e) return &eng;
    return nullptr;
}

SearchEngineConfig* SearchConfig::getDefaultEngine() {
    // Original logic: lowest non-zero priority, or first enabled engine
    SearchEngineConfig* best = nullptr;
    for (auto& eng : engines) {
        if (!eng.enabled) continue;
        if (!best || eng.priority < best->priority) best = &eng;
    }
    return best;
}

// ==== URL Building ====

std::string buildSearxngUrl(const std::string& endpoint, const std::string& query, int maxResults) {
    // Original API: GET /search?q=QUERY&format=json&pageno=1
    std::string url = endpoint;
    if (!url.empty() && url.back() != '/') url += "/";
    url += "search?q=";
    // URL-encode query (simple version: replace space with +)
    for (char c : query) {
        if (c == ' ') url += '+';
        else if (c == '&') url += "%26";
        else if (c == '=') url += "%3D";
        else if (c == '#') url += "%23";
        else url += c;
    }
    url += "&format=json&categories=general&pageno=1";
    return url;
}

std::string buildDuckDuckGoUrl(const std::string& query) {
    // Original API: GET https://api.duckduckgo.com/?q=QUERY&format=json&no_html=1&skip_disambig=1
    std::string url = "https://api.duckduckgo.com/?q=";
    for (char c : query) {
        if (c == ' ') url += '+';
        else url += c;
    }
    url += "&format=json&no_html=1&skip_disambig=1";
    return url;
}

std::string buildGoogleUrl(const std::string& apiKey, const std::string& engineId, const std::string& query, int maxResults) {
    // Original API: GET https://customsearch.googleapis.com/customsearch/v1?key=KEY&cx=CX&q=QUERY
    std::string url = "https://customsearch.googleapis.com/customsearch/v1?key=" + apiKey;
    url += "&cx=" + engineId + "&q=";
    for (char c : query) {
        if (c == ' ') url += '+';
        else url += c;
    }
    url += "&num=" + std::to_string(maxResults);
    return url;
}

// ==== JSON Helpers ====

static std::string extractJsonString(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return "";
    pos = json.find(':', pos);
    if (pos == std::string::npos) return "";
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '"')) pos++;
    size_t end = pos;
    while (end < json.size() && json[end] != '"') {
        if (json[end] == '\\') end++;
        end++;
    }
    return json.substr(pos, end - pos);
}

static int64_t extractJsonInt64(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return 0;
    pos = json.find(':', pos);
    if (pos == std::string::npos) return 0;
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    int64_t val = 0;
    while (pos < json.size() && json[pos] >= '0' && json[pos] <= '9') { val = val * 10 + (json[pos]-'0'); pos++; }
    return val;
}

static std::string extractJsonObject(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return "";
    pos = json.find(':', pos);
    if (pos == std::string::npos) return "";
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    if (pos >= json.size() || json[pos] != '{') return "";
    int depth = 1;
    size_t start = pos;
    pos++;
    while (pos < json.size() && depth > 0) { if (json[pos] == '{') depth++; else if (json[pos] == '}') depth--; pos++; }
    return json.substr(start, pos - start);
}

// ==== SearXNG Parser ====
//
// SearXNG JSON format: {"query":"...","results":[{...},{...}],"number_of_results":123}
// Each result: {"title":"...","url":"...","content":"...","engine":"...","score":...}

SearchResponse parseSearxngResponse(const std::string& json, const std::string& query) {
    SearchResponse r;
    r.query = query;
    r.engine = SearchEngine::SEARXNG;

    r.totalResults = static_cast<int>(extractJsonInt64(json, "number_of_results"));

    // Parse results array
    auto resultsPos = json.find("\"results\"");
    if (resultsPos == std::string::npos) { r.errorMessage = "No results"; return r; }
    resultsPos = json.find('[', resultsPos);
    if (resultsPos == std::string::npos) { r.errorMessage = "Malformed response"; return r; }

    resultsPos++;
    while (resultsPos < json.size()) {
        while (resultsPos < json.size() && (json[resultsPos] == ' ' || json[resultsPos] == ',' || json[resultsPos] == '\n')) resultsPos++;
        if (resultsPos >= json.size() || json[resultsPos] == ']') break;
        if (json[resultsPos] == '{') {
            int d = 1;
            size_t start = resultsPos;
            resultsPos++;
            while (resultsPos < json.size() && d > 0) { if (json[resultsPos] == '{') d++; else if (json[resultsPos] == '}') d--; resultsPos++; }
            std::string itemJson = json.substr(start, resultsPos - start);

            SearchResultItem item;
            item.title = extractJsonString(itemJson, "title");
            item.url = extractJsonString(itemJson, "url");
            item.snippet = extractJsonString(itemJson, "content");
            item.engineName = extractJsonString(itemJson, "engine");

            // Extract domain from URL
            auto proto = item.url.find("://");
            if (proto != std::string::npos) {
                size_t ds = proto + 3;
                auto de = item.url.find('/', ds);
                if (de == std::string::npos) de = item.url.size();
                item.sourceDomain = item.url.substr(ds, de - ds);
            }

            r.results.push_back(item);
        }
    }

    r.success = !r.results.empty();
    return r;
}

// ==== DuckDuckGo Parser ====
//
// Format: {"Abstract":"...","AbstractURL":"...","RelatedTopics":[...],"Heading":"..."}

SearchResponse parseDuckDuckGoResponse(const std::string& json, const std::string& query) {
    SearchResponse r;
    r.query = query;
    r.engine = SearchEngine::DUCKDUCKGO;

    // Instant Answer: Abstract
    auto abstract = extractJsonString(json, "Abstract");
    auto abstractUrl = extractJsonString(json, "AbstractURL");
    if (!abstract.empty()) {
        SearchResultItem item;
        item.title = extractJsonString(json, "Heading");
        if (item.title.empty()) item.title = "Instant Answer";
        item.url = abstractUrl;
        item.snippet = abstract;
        item.engineName = "duckduckgo";
        item.sourceDomain = "duckduckgo.com";
        r.results.push_back(item);
    }

    // Related Topics
    auto rtPos = json.find("\"RelatedTopics\"");
    if (rtPos != std::string::npos) {
        rtPos = json.find('[', rtPos);
        if (rtPos != std::string::npos) {
            rtPos++;
            while (rtPos < json.size()) {
                while (rtPos < json.size() && (json[rtPos] == ' ' || json[rtPos] == ',' || json[rtPos] == '\n')) rtPos++;
                if (rtPos >= json.size() || json[rtPos] == ']') break;
                if (json[rtPos] == '{') {
                    int d = 1;
                    size_t start = rtPos;
                    rtPos++;
                    while (rtPos < json.size() && d > 0) { if (json[rtPos] == '{') d++; else if (json[rtPos] == '}') d--; rtPos++; }
                    std::string itemJson = json.substr(start, rtPos - start);
                    auto text = extractJsonString(itemJson, "Text");
                    auto firstUrl = extractJsonString(itemJson, "FirstURL");
                    if (!text.empty() && !firstUrl.empty()) {
                        SearchResultItem item;
                        item.title = text;
                        item.url = firstUrl;
                        item.snippet = "";
                        item.engineName = "duckduckgo";
                        r.results.push_back(item);
                    }
                }
            }
        }
    }

    r.success = !r.results.empty();
    return r;
}

// ==== Google Parser ====
//
// Format: {"items":[{"title":"...","link":"...","snippet":"..."}],"searchInformation":{"totalResults":"123"}}

SearchResponse parseGoogleResponse(const std::string& json, const std::string& query) {
    SearchResponse r;
    r.query = query;
    r.engine = SearchEngine::GOOGLE;

    // Total results from searchInformation
    auto siJson = extractJsonObject(json, "searchInformation");
    if (!siJson.empty()) {
        auto totalStr = extractJsonString(siJson, "totalResults");
        if (!totalStr.empty()) r.totalResults = std::stoi(totalStr);
    }

    // Items array
    auto itemsPos = json.find("\"items\"");
    if (itemsPos != std::string::npos) {
        itemsPos = json.find('[', itemsPos);
        if (itemsPos != std::string::npos) {
            itemsPos++;
            while (itemsPos < json.size()) {
                while (itemsPos < json.size() && (json[itemsPos] == ' ' || json[itemsPos] == ',' || json[itemsPos] == '\n')) itemsPos++;
                if (itemsPos >= json.size() || json[itemsPos] == ']') break;
                if (json[itemsPos] == '{') {
                    int d = 1;
                    size_t start = itemsPos;
                    itemsPos++;
                    while (itemsPos < json.size() && d > 0) { if (json[itemsPos] == '{') d++; else if (json[itemsPos] == '}') d--; itemsPos++; }
                    std::string itemJson = json.substr(start, itemsPos - start);

                    SearchResultItem item;
                    item.title = extractJsonString(itemJson, "title");
                    item.url = extractJsonString(itemJson, "link");
                    item.snippet = extractJsonString(itemJson, "snippet");
                    item.engineName = "google";

                    auto displayLink = extractJsonString(itemJson, "displayLink");
                    if (!displayLink.empty()) item.sourceDomain = displayLink;

                    r.results.push_back(item);
                }
            }
        }
    }

    r.success = !r.results.empty();
    return r;
}

// ==== Formatting ====

std::string SearchResponse::formatAsPlainText() const {
    std::ostringstream os;
    os << "Web search: " << query << "\n";
    os << "Engine: " << searchEngineToString(engine) << "\n\n";
    int i = 1;
    for (const auto& r : results) {
        os << i++ << ". " << r.title << "\n";
        os << "   " << r.url << "\n";
        if (!r.snippet.empty()) os << "   " << r.snippet << "\n";
        os << "\n";
    }
    return os.str();
}

std::string SearchResponse::formatAsHtml() const {
    std::ostringstream os;
    os << "<p><b>Web search: " << query << "</b> (" << searchEngineToString(engine) << ")</p><ol>";
    for (const auto& r : results) {
        os << "<li><a href=\"" << r.url << "\">" << r.title << "</a>";
        if (!r.snippet.empty()) os << "<br><small>" << r.snippet << "</small>";
        os << "</li>";
    }
    os << "</ol>";
    return os.str();
}

// ==== /web Command Parser ====
//
// Original syntax: /web [engine] query
//   /web ddg vitamin b12  →  engine=DDG, query="vitamin b12"
//   /web google cats      →  engine=Google, query="cats"
//   /web searxng python   →  engine=SearXNG, query="python"
//   /web linux kernel     →  engine=NONE (default), query="linux kernel"

WebCommand parseWebCommand(const std::string& commandArgs) {
    WebCommand cmd;
    if (commandArgs.empty()) return cmd;

    // Check if first word is an engine name
    auto space = commandArgs.find(' ');
    if (space != std::string::npos) {
        std::string firstWord = commandArgs.substr(0, space);
        SearchEngine eng = searchEngineFromString(firstWord);
        if (eng != SearchEngine::NONE) {
            cmd.engine = eng;
            cmd.query = commandArgs.substr(space + 1);
        } else {
            cmd.query = commandArgs;
        }
    } else {
        cmd.query = commandArgs;
    }

    cmd.isValid = !cmd.query.empty();
    return cmd;
}

// ==== Agent Integration ====

std::string formatSearchResultsForAgent(const SearchResponse& response) {
    // Format results as context for AI agent
    std::ostringstream os;
    os << "Web search results for '" << response.query << "' ";
    os << "(via " << searchEngineToString(response.engine) << "):\n";
    int i = 1;
    for (const auto& r : response.results) {
        os << i++ << ". " << r.title << "\n";
        os << "   URL: " << r.url << "\n";
        if (!r.snippet.empty()) os << "   Summary: " << r.snippet << "\n";
    }
    return os.str();
}

// ==== Config Serialization ====

std::string searchConfigToJson(const SearchConfig& config) {
    std::ostringstream os;
    os << "{\"enabled\":" << (config.isEnabled ? "true" : "false") << ",";
    os << "\"agentWebAccess\":" << (config.agentWebAccess ? "true" : "false") << ",";
    os << "\"maxResults\":" << config.maxResults << ",";
    os << "\"engines\":[";
    bool first = true;
    for (const auto& e : config.engines) {
        if (!first) os << ",";
        first = false;
        os << "{\"engine\":\"" << searchEngineToString(e.engine) << "\",";
        os << "\"url\":\"" << e.endpointUrl << "\",";
        os << "\"key\":\"" << e.apiKey << "\",";
        os << "\"cx\":\"" << e.engineId << "\",";
        os << "\"enabled\":" << (e.enabled ? "true" : "false") << ",";
        os << "\"priority\":" << e.priority << "}";
    }
    os << "]}";
    return os.str();
}

SearchConfig searchConfigFromJson(const std::string& /*json*/) {
    SearchConfig config;
    // Parse JSON — standard pattern
    return config;
}

} // namespace progressive
