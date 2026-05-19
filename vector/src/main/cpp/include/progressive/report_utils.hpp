#ifndef PROGRESSIVE_REPORT_UTILS_HPP
#define PROGRESSIVE_REPORT_UTILS_HPP

#include <string>
#include <vector>

namespace progressive {

// ---- Content Reporting ----

struct ReportReason {
    std::string code;            // Matrix reason code
    std::string description;     // human-readable
    bool isCustom = false;       // user-provided reason
};

struct ReportRequest {
    std::string eventId;
    std::string roomId;
    std::string reason;           // reason code or custom text
    int score = -100;            // severity: -100 (offensive) to 0 (irrelevant)
};

// Get the list of standard Matrix report reasons.
std::vector<ReportReason> getReportReasons();

// Build content report request body JSON.
std::string buildReportBody(const ReportRequest& request);

// Validate a report reason.
bool isValidReportReason(const std::string& reason);

// Check if a reason is a standard Matrix reason code.
bool isStandardReason(const std::string& reason);

// Get reason description from code.
std::string getReasonDescription(const std::string& code);

// Format a report request for confirmation dialog.
std::string formatReportConfirmation(const ReportRequest& request);

// Check if a score indicates offensive content.
bool isOffensive(int score, int threshold = -50);

// ---- Bug Report / Rageshake ----

struct BugReport {
    std::string description;
    std::string logs;            // base64-encoded log content
    std::string version;
    std::string deviceInfo;
    std::string userId;
    bool includeLogs = true;
    bool canContact = true;
};

// Build bug report request body JSON.
std::string buildBugReportBody(const BugReport& report);

// Format bug report for GitHub issue or email.
std::string formatBugReport(const BugReport& report);

// Validate bug report (must have description).
bool isValidBugReport(const BugReport& report);

// Truncate bug report description to maximum length.
std::string truncateReportDescription(const std::string& desc, int maxLen = 2000);

// ---- Rageshake Detection ----

struct RageshakeEvent {
    int64_t timestampMs = 0;
    double accelerometerX = 0.0;
    double accelerometerY = 0.0;
    double accelerometerZ = 0.0;
    bool isShaking = false;
};

// Detect shake from accelerometer data.
bool detectShake(const std::vector<RageshakeEvent>& events, double threshold = 15.0,
    int requiredSamples = 3, int windowMs = 500);

// Check if recent shakes exceed threshold for rageshake trigger.
bool shouldTriggerRageshake(const std::vector<RageshakeEvent>& events);

} // namespace progressive

#endif // PROGRESSIVE_REPORT_UTILS_HPP
