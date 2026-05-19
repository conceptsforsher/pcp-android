#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// ================================================================
// Content Reporting — reason codes, request bodies, validation
//
// Faithful port from Element Android original sources:
//   ReportContentBody.kt — score, reason
//   ReportRoomBody.kt — reason
//   ReportContentTask.kt — Params(roomId, eventId, score, reason)
//   ReportRoomTask.kt — Params(roomId, reason)
//   DefaultReportingService.kt — reportContent(), reportRoom()
//
// Matrix spec: POST /_matrix/client/v3/rooms/{roomId}/report/{eventId}
//              POST /_matrix/client/v3/rooms/{roomId}/report
// ================================================================

// ---- Report Reason Enum (from Kotlin: matrix spec reason codes) ----
// Original Kotlin: string-based reason codes passed to API

enum class ReportReasonCode {
    SPAM,               // m.spam — Unsolicited advertising
    INAPPROPRIATE,      // m.inappropriate — Not suitable for the room
    OFFENSIVE,          // m.offensive — Offensive or abusive content
    ILLEGAL,            // m.illegal — Illegal content
    VIOLENCE,           // m.violence — Violence or threat of violence
    HARASSMENT,         // m.harassment — Harassment or bullying
    SUICIDE,            // m.suicide — Suicide or self-harm content
    IMPERSONATION,      // m.impersonation — Impersonation of another user
    CHILD_ABUSE,        // m.child_abuse — Child abuse material
    TERRORISM,          // m.terrorism — Terrorism or violent extremism
    COPYRIGHT,          // m.copyright — Copyright infringement
    OTHER,              // m.other — User-defined reason (custom)
    CUSTOM,             // Fully custom reason string
};

// String constants for each reason code
namespace ReportReasonStr {
    constexpr const char* SPAM = "m.spam";
    constexpr const char* INAPPROPRIATE = "m.inappropriate";
    constexpr const char* OFFENSIVE = "m.offensive";
    constexpr const char* ILLEGAL = "m.illegal";
    constexpr const char* VIOLENCE = "m.violence";
    constexpr const char* HARASSMENT = "m.harassment";
    constexpr const char* SUICIDE = "m.suicide";
    constexpr const char* IMPERSONATION = "m.impersonation";
    constexpr const char* CHILD_ABUSE = "m.child_abuse";
    constexpr const char* TERRORISM = "m.terrorism";
    constexpr const char* COPYRIGHT = "m.copyright";
    constexpr const char* OTHER = "m.other";
}

// Convert enum to Matrix reason string.
const char* reportReasonCodeToString(ReportReasonCode code);

// Convert Matrix reason string to enum.
ReportReasonCode reportReasonCodeFromString(const std::string& code);

// ---- ReportReason (legacy struct, backward compatible) ----

struct ReportReason {
    std::string code;            // Matrix reason code
    std::string description;     // human-readable
    bool isCustom = false;       // user-provided reason
};

// ---- Report Content Body (from ReportContentBody.kt:23-32) ----
// Original Kotlin:
//   @JsonClass(generateAdapter = true)
//   internal data class ReportContentBody(
//       @Json(name = "score") val score: Int,
//       @Json(name = "reason") val reason: String
//   )
struct ReportContentBody {
    std::string reason;          // Reason code or custom text
    int score = -100;            // -100 (most offensive) to 0 (inoffensive)
    std::string eventId;         // The event being reported
    std::string roomId;          // The room containing the event
};

// ---- Report Room Body (from ReportRoomBody.kt:23-28) ----
// Original Kotlin:
//   @JsonClass(generateAdapter = true)
//   internal data class ReportRoomBody(
//       @Json(name = "reason") val reason: String
//   )
struct ReportRoomBody {
    std::string reason;          // Reason code or custom text
    int score = -100;            // -100 (most offensive) to 0 (inoffensive)
    std::string roomId;          // The room being reported
};

// ---- Report Request (legacy, for backward compatibility) ----

struct ReportRequest {
    std::string eventId;
    std::string roomId;
    std::string reason;           // reason code or custom text
    int score = -100;            // severity: -100 (offensive) to 0 (irrelevant)
};

// Get the list of standard Matrix report reasons.
std::vector<ReportReason> getReportReasons();

// Build content report request body JSON.
// Original Kotlin: reportContent(roomId, eventId, ReportContentBody(score, reason))
std::string buildReportContentBody(const ReportContentBody& body);

// Build room report request body JSON.
// Original Kotlin: reportRoom(roomId, ReportRoomBody(reason))
std::string buildReportRoomBody(const ReportRoomBody& body);

// Build content report request body JSON (legacy).
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

// ---- Server Notice Content ----
// Original Kotlin: server notice events are sent to m.server_notice rooms
// as admin messages. Fields derived from MatrixError.kt and server notice
// module spec.

struct ServerNoticeContent {
    std::string adminContact;        // admin_contact URI from error
    std::string serverNoticeType;    // m.server_notice or regular
    std::string limitType;           // monthly_active_user, hard, soft, etc.
    std::string body;                // Message body text
    std::string errorCode;           // Matrix error code (e.g., M_RESOURCE_LIMIT_EXCEEDED)
    std::string consentUri;          // For M_CONSENT_NOT_GIVEN
    int64_t retryAfterMs = 0;        // For M_LIMIT_EXCEEDED
    bool isResourceLimit = false;    // M_RESOURCE_LIMIT_EXCEEDED
    bool isConsentRequired = false;  // M_CONSENT_NOT_GIVEN
    bool isRateLimited = false;      // M_LIMIT_EXCEEDED
    bool isServerNotice = false;     // Is a server notice event
};

// Parse a server notice from event content JSON.
// Original Kotlin: extracts admin_contact, server_notice_type, limit_type,
//   body, error codes from the event content and embedded error data.
ServerNoticeContent parseServerNotice(const std::string& contentJson);

// Alias for spec compliance.
inline ServerNoticeContent parseServerNoticeContent(const std::string& contentJson) {
    return parseServerNotice(contentJson);
}

// Check if event content JSON is a server notice.
bool isServerNotice(const std::string& contentJson);

// Format server notice for user display.
std::string formatServerNoticeMessage(const ServerNoticeContent& notice);

// ================================================================
// Bug Report / Rageshake
// ================================================================

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

// ================================================================
// Rageshake Detection
// ================================================================

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

// ================================================================
// Expanded Report Types — categories, results, history, validation
//
// Original Kotlin: ReportCategory.kt, ReportResult.kt,
//   ReportHistory.kt, ReportConfig.kt
// ================================================================

// ---- Report Category ----
// Original Kotlin: ReportCategory enum (UI-facing categories)

enum class ReportCategory {
    SPAM = 0,
    HARASSMENT = 1,
    INAPPROPRIATE = 2,
    VIOLENCE = 3,
    SELF_HARM = 4,
    ILLEGAL = 5,
    IMPERSONATION = 6,
    COPYRIGHT = 7,
    CUSTOM = 8,
};

const char* reportCategoryToString(ReportCategory category);
ReportCategory reportCategoryFromString(const std::string& s);

// ---- Report Status ----
// Original Kotlin: ReportStatus enum

enum class ReportStatus {
    PENDING = 0,
    REVIEWED = 1,
    RESOLVED = 2,
    DISMISSED = 3,
};

const char* reportStatusToString(ReportStatus status);
ReportStatus reportStatusFromString(const std::string& s);

// ---- Report Result ----
// Original Kotlin: ReportResult data class (success/failure wrapper)

struct ReportResult {
    bool success = false;
    std::string reportId;
    std::string errorMessage;
    std::string errorCode;
};

// ---- Report Config ----
// Original Kotlin: ReportConfig — server-reported reporting configuration

struct ReportConfig {
    std::vector<std::string> availableReasons;
    bool requireDescription = false;
    int maxDescriptionLength = 500;
};

// ---- Report History Entry ----
// Original Kotlin: ReportHistoryEntry data class

struct ReportHistoryEntry {
    std::string reportId;
    std::string eventId;
    std::string roomId;
    std::string reason;
    int64_t timestamp = 0;       // Unix epoch millis
    ReportStatus status = ReportStatus::PENDING;
};

// ---- Report History ----
// Original Kotlin: ReportHistory wrapper

struct ReportHistory {
    std::vector<ReportHistoryEntry> reports;
    int totalCount = 0;
};

// ================================================================
// Expanded Report Functions
// ================================================================

// Validate a report request for submission.
// Original Kotlin: ReportValidator.validate(report)
bool validateReport(const ReportRequest& request, const ReportConfig& config);

// Format a report reason as a user-facing readable string.
// Original Kotlin: report reason -> display string
std::string formatReportReason(const std::string& reasonCode);

// Get the list of default Matrix specification report reasons.
// Original Kotlin: Matrix spec section 13.33.1 reason strings
std::vector<std::string> getDefaultReportReasons();

} // namespace progressive
