#pragma once
#include <string>
#include <vector>
#include <cstdint>

namespace pcp {

enum class PaginationDirection { BACKWARD = 0, FORWARD = 1 };

struct PaginationToken {
    std::string fromToken;      // pagination token (start/end)
    std::string toToken;        // exclusive end token (optional)
    PaginationDirection direction = PaginationDirection::BACKWARD;
    int limit = 20;
    std::string filter;         // JSON filter string
};

struct PaginationResult {
    std::vector<std::string> eventIds;
    std::string nextBatch;      // token for next page
    std::string prevBatch;      // token for previous page
    bool hasMore = false;
    int totalReturned = 0;
};

// Build pagination request parameters
std::string buildPaginationRequest(const PaginationToken& token);

// Parse pagination response
PaginationResult parsePaginationResponse(const std::string& json);

// Check if more pages are available
bool hasMorePages(const PaginationResult& result, PaginationDirection direction);

// Format pagination progress text
std::string formatPaginationProgress(int loaded, int total, PaginationDirection direction);

} // namespace pcp
