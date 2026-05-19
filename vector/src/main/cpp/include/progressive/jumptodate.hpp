#ifndef PROGRESSIVE_JUMPTODATE_HPP
#define PROGRESSIVE_JUMPTODATE_HPP

#include <string>
#include <optional>
#include <cstdint>

namespace progressive {

struct JumpToDateRequest {
    std::string roomId;
    std::string dateString;       // YYYY-MM-DD
    std::string serverBaseUrl;    // e.g. "https://matrix.example.com"
    std::string accessToken;
    std::string errorMessage;
    int64_t originServerTs = 0;  // computed
};

struct JumpToDateResult {
    bool success = false;
    std::string eventId;
    std::string errorMessage;
    int statusCode = 0;
};

// Validate date format (YYYY-MM-DD) and compute Unix timestamp
bool validateAndComputeTimestamp(JumpToDateRequest& request);

// Build the MSC3030 /timestamp_to_event endpoint URL
// GET /_matrix/client/unstable/org.matrix.msc3030/rooms/{roomId}/timestamp_to_event?ts=<ts>&dir=f
std::string buildMsc3030Url(const JumpToDateRequest& request);

// Parse the JSON response from the server
JumpToDateResult parseTimestampToEventResponse(const std::string& responseBody, int httpStatus);

// Compute Unix timestamp from date parts
int64_t dateToUnixTimestamp(int year, int month, int day);

} // namespace progressive

#endif // PROGRESSIVE_JUMPTODATE_HPP
