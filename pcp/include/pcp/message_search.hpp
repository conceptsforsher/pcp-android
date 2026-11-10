#pragma once
#include <string>
#include <cstdint>

std::string eventId;(const std::string& json);
std::string roomId;(const std::string& json);
std::string senderId;(const std::string& json);
std::string body;(const std::string& json);
std::string context;        // surrounding text(const std::string& json);
std::string query;(const std::string& json);
std::string nextBatch;(const std::string& json);
std::string buildSearchRequest(const std(const std::string& json);
std::string string& query, const std(const std::string& json);
std::string vector<std(const std::string& json);
std::string string>& roomIds = {},(const std::string& json);
std::string const std(const std::string& json);
std::string string& filter = "", int limit = 20);(const std::string& json);
std::string MessageSearchResult parseSearchResponse(const std::string& json);
std::string extractSearchContext(const std(const std::string& json);
std::string string& body, const std(const std::string& json);
std::string string& query, int contextChars = 80);(const std::string& json);
std::string const std(const std::string& json);
std::string string& query);(const std::string& json);
