#pragma once
#include <string>
#include <cstdint>

namespace pcp {

enum class ReportReason { SPAM, INAPPROPRIATE, CUSTOM, UNKNOWN };

struct UserReport {
    std::string eventId;
    std::string roomId;
    std::string userId;         // user being reported
    ReportReason reason = ReportReason::UNKNOWN;
    std::string customReason;   // for CUSTOM reason
    int64_t score = 0;          // server return: -100 to 0
};

// Parse report reason from string
ReportReason reportReasonFromString(const std::string& s);
std::string reportReasonToString(ReportReason r);

// Build JSON to report a specific event
std::string buildReportContent(const std::string& eventId, ReportReason reason,
                                 const std::string& customReason = "");

// Build JSON to report a server (not event-specific)
std::string buildReportServerNotice(const std::string& reason);

// Parse report response from server  
UserReport parseReportResponse(const std::string& json);

// Format report reason for display
std::string formatReportReason(ReportReason reason);

} // namespace pcp
