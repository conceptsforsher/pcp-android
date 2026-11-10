#include "pcp/event_pagination.hpp"
#include <sstream>

namespace pcp {

std::string buildPaginationRequest(const PaginationToken& token) {
    std::ostringstream os;
    os << "{";
    if (!token.fromToken.empty()) os << R"("from":")" << token.fromToken << R"(",)";
    if (!token.toToken.empty()) os << R"("to":")" << token.toToken << R"(",)";
    os << R"("dir":")" << (token.direction == PaginationDirection::BACKWARD ? "b" : "f") << R"(",)";
    os << R"("limit":)" << token.limit;
    if (!token.filter.empty()) os << R"(,"filter":)" << token.filter;
    os << "}";
    return os.str();
}

PaginationResult parsePaginationResponse(const std::string& json) {
    PaginationResult r;
    
    // Parse chunk array
    auto chunkPos = json.find("\"chunk\"");
    if (chunkPos != std::string::npos) {
        auto arrStart = json.find('[', chunkPos);
        auto arrEnd = json.find(']', arrStart);
        if (arrStart != std::string::npos && arrEnd != std::string::npos) {
            std::string arr = json.substr(arrStart + 1, arrEnd - arrStart - 1);
            size_t pos = 0;
            while (pos < arr.size()) {
                auto evtId = arr.find("\"event_id\":\"", pos);
                if (evtId == std::string::npos) break;
                evtId += 12;
                auto evtEnd = arr.find('"', evtId);
                if (evtEnd == std::string::npos) break;
                r.eventIds.push_back(arr.substr(evtId, evtEnd - evtId));
                pos = evtEnd + 1;
            }
        }
    }
    
    // Parse tokens
    auto startPos = json.find("\"start\":\"");
    if (startPos != std::string::npos) {
        startPos += 9;
        auto end = json.find('"', startPos);
        if (end != std::string::npos) r.prevBatch = json.substr(startPos, end - startPos);
    }
    auto endPos = json.find("\"end\":\"");
    if (endPos != std::string::npos) {
        endPos += 7;
        auto end = json.find('"', endPos);
        if (end != std::string::npos) r.nextBatch = json.substr(endPos, end - endPos);
    }
    
    r.totalReturned = (int)r.eventIds.size();
    r.hasMore = !r.nextBatch.empty();
    
    return r;
}

bool hasMorePages(const PaginationResult& result, PaginationDirection direction) {
    if (direction == PaginationDirection::BACKWARD) return !result.nextBatch.empty();
    return !result.prevBatch.empty();
}

std::string formatPaginationProgress(int loaded, int total, PaginationDirection direction) {
    std::ostringstream os;
    os << "Loaded " << loaded << " of " << total << " events";
    return os.str();
}

} // namespace pcp
