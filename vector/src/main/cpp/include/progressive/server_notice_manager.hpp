#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// ================================================================
// Server Notice Handler — resource limits, admin messages
//
// Faithful port from Element Android original sources:
//   MatrixError.kt — error codes (M_RESOURCE_LIMIT_EXCEEDED,
//     M_CONSENT_NOT_GIVEN, M_CANNOT_LEAVE_SERVER_NOTICE_ROOM),
//     limitType (monthly_active_user), adminUri, consentUri, retryAfterMs
//   ResourceLimitErrorFormatter.kt — Mode (Hard/Soft), format error
//     with MAU-specific text and admin contact link
//   RoomTag.kt — m.server_notice tag for server notice rooms
//   Server Notice module spec — special room for admin messages
//
// Covers:
//   1. Server notice room detection (m.server_notice tag)
//   2. Resource limit errors (MAU hard/soft, generic limits)
//   3. Admin contact extraction
//   4. Error message formatting (user-facing strings)
//   5. Consent URI (M_CONSENT_NOT_GIVEN)
//   6. Retry-after handling (M_LIMIT_EXCEEDED)
//   7. Server notice message content parsing
// ================================================================

// ---- Resource Limit Type ----
// Original: MatrixError.LIMIT_TYPE_MAU = "monthly_active_user"

enum class ResourceLimitType {
    UNKNOWN = 0,
    MAU = 1,                 // monthly_active_user — Monthly Active User limit
    HARD = 2,                // User blocked entirely
    SOFT = 3,                // User can still read but not send
    STORAGE = 4,             // Storage limit exceeded
};

const char* resourceLimitTypeToString(ResourceLimitType type);
ResourceLimitType resourceLimitTypeFromString(const std::string& s);

// ---- Resource Limit Mode (Hard/Soft) ----
// Original: ResourceLimitErrorFormatter.Mode.Soft / Mode.Hard

enum class ResourceLimitMode {
    SOFT = 0,   // User can still send messages (warning)
    HARD = 1,   // User cannot send messages (blocked)
};

// ---- Server Notice Info ----

struct ServerNoticeInfo {
    std::string noticeType;          // "m.server_notice" or regular message
    std::string noticeBody;          // The message body
    std::string adminContact;        // admin_contact from error
    std::string consentUri;          // For M_CONSENT_NOT_GIVEN
    ResourceLimitType limitType = ResourceLimitType::UNKNOWN;
    ResourceLimitMode mode = ResourceLimitMode::HARD;
    int64_t retryAfterMs = 0;       // For M_LIMIT_EXCEEDED
    bool isServerNoticeRoom = false; // Room has m.server_notice tag
    bool isResourceLimit = false;    // M_RESOURCE_LIMIT_EXCEEDED
    bool isConsentRequired = false;  // M_CONSENT_NOT_GIVEN
    bool isRateLimited = false;      // M_LIMIT_EXCEEDED
    std::string formattedMessage;    // User-facing formatted error
    std::string adminContactLink;    // HTML link for admin contact
};

// ---- Matrix Error Codes (from spec) ----
// Original: MatrixError companion object constants

struct MatrixErrorCodes {
    static constexpr const char* FORBIDDEN = "M_FORBIDDEN";
    static constexpr const char* UNKNOWN_TOKEN = "M_UNKNOWN_TOKEN";
    static constexpr const char* MISSING_TOKEN = "M_MISSING_TOKEN";
    static constexpr const char* LIMIT_EXCEEDED = "M_LIMIT_EXCEEDED";
    static constexpr const char* NOT_FOUND = "M_NOT_FOUND";
    static constexpr const char* UNKNOWN = "M_UNKNOWN";
    static constexpr const char* RESOURCE_LIMIT_EXCEEDED = "M_RESOURCE_LIMIT_EXCEEDED";
    static constexpr const char* CONSENT_NOT_GIVEN = "M_CONSENT_NOT_GIVEN";
    static constexpr const char* CANNOT_LEAVE_SERVER_NOTICE_ROOM = "M_CANNOT_LEAVE_SERVER_NOTICE_ROOM";
    static constexpr const char* USER_DEACTIVATED = "M_USER_DEACTIVATED";
    static constexpr const char* TERMS_NOT_SIGNED = "M_TERMS_NOT_SIGNED";
    static constexpr const char* SOFT_LOGOUT = "M_UNKNOWN_TOKEN_SOFT_LOGOUT";
    static constexpr const char* EXPIRED_ACCOUNT = "ORG_MATRIX_EXPIRED_ACCOUNT";

    // Resource limit type values
    static constexpr const char* LIMIT_TYPE_MAU = "monthly_active_user";
};

// ---- Server Notice Manager ----

class ServerNoticeManager {
public:
    ServerNoticeManager();

    // ====== Error Parsing ======
    // Original: MatrixError.kt fields: code, message, limitType, adminUri, consentUri, retryAfterMs

    // Parse a Matrix error JSON response.
    // Returns a ServerNoticeInfo with all relevant fields extracted.
    ServerNoticeInfo parseMatrixError(const std::string& errorJson);

    // Parse server notice content from a state event.
    ServerNoticeInfo parseServerNoticeContent(const std::string& contentJson);

    // ====== Room Detection ======
    // Original: RoomTag.kt — m.server_notice

    // Check if a room is a server notice room (has m.server_notice tag).
    bool isServerNoticeRoom(const std::string& roomTagsJson);

    // Check if a room tag indicates a server notice room.
    bool isServerNoticeTag(const std::string& tagName);

    // ====== Resource Limit Handling ======
    // Original: ResourceLimitErrorFormatter.kt — Mode.Hard/Mode.Soft

    // Format a resource limit error for user display.
    // Combines error message with admin contact.
    std::string formatResourceLimitError(const ServerNoticeInfo& info, ResourceLimitMode mode);

    // Format admin contact as a clickable link (HTML).
    std::string formatAdminContactLink(const std::string& adminUri);

    // Format admin contact as plain text.
    std::string formatAdminContactText(const std::string& adminUri);

    // Format consent required message (M_CONSENT_NOT_GIVEN).
    std::string formatConsentRequired(const ServerNoticeInfo& info);

    // Format rate limit message with retry time.
    std::string formatRateLimitMessage(const ServerNoticeInfo& info);

    // ====== Error Classification ======
    // Original: MatrixError companion — all error codes

    // Get human-readable description of a Matrix error code.
    std::string getErrorCodeDescription(const std::string& errorCode);

    // Check if error is a resource limit.
    bool isResourceLimitError(const std::string& errorCode);

    // Check if error is a rate limit.
    bool isRateLimitError(const std::string& errorCode);

    // Check if error is a consent error.
    bool isConsentError(const std::string& errorCode);

    // Check if error is a logout error (token invalid).
    bool isLogoutError(const std::string& errorCode);

    // Check if error is a user deactivation.
    bool isUserDeactivatedError(const std::string& errorCode);

    // ====== Display Formatting ======

    // Get a banner color for a server notice severity.
    // Returns hex color string.
    std::string getBannerColor(const ServerNoticeInfo& info);

    // Format the downtime duration for display.
    std::string formatDowntime(int64_t retryAfterMs);

    // Format server notice for notification.
    std::string formatServerNotice(const ServerNoticeInfo& info);

    // ====== Serialization ======

    // Export server notice info as JSON.
    std::string serverNoticeToJson(const ServerNoticeInfo& info);

    // Export resource limit as JSON.
    std::string resourceLimitToJson(const ServerNoticeInfo& info);

private:
    static std::string extractStr(const std::string& json, const std::string& key);
    static int64_t extractInt(const std::string& json, const std::string& key);
    static bool extractBool(const std::string& json, const std::string& key);
};

// ================================================================
// Server Notice Types — structured notice model
//
// Original Kotlin: ServerNotice.kt, ServerNoticeAction.kt
// ================================================================

// ---- Server Notice Type ----
// Original Kotlin: ServerNotice.Type enum

enum class ServerNoticeType {
    MESSAGE = 0,         // Informational message from server admin
    USAGE_LIMIT = 1,     // Resource/usage limit warning or block
    POLICY_UPDATE = 2,   // Terms of service / policy changes
    MAINTENANCE = 3,     // Planned maintenance / downtime
    VERSION_UPGRADE = 4, // Client version upgrade required
    SECURITY = 5,        // Security notice or alert
};

const char* serverNoticeTypeToString(ServerNoticeType type);
ServerNoticeType serverNoticeTypeFromString(const std::string& s);

// ---- Server Notice Priority ----
// Original Kotlin: ServerNotice.Priority enum

enum class ServerNoticePriority {
    LOW = 0,
    NORMAL = 1,
    HIGH = 2,
    CRITICAL = 3,
};

const char* serverNoticePriorityToString(ServerNoticePriority priority);
ServerNoticePriority serverNoticePriorityFromString(const std::string& s);

// ---- Server Notice Action Type ----
// Original Kotlin: ServerNoticeAction.Type enum

enum class ServerNoticeActionType {
    OPEN_URL = 0,
    DISMISS = 1,
    SETTINGS = 2,
};

const char* serverNoticeActionTypeToString(ServerNoticeActionType type);
ServerNoticeActionType serverNoticeActionTypeFromString(const std::string& s);

// ---- Server Notice Action ----
// Original Kotlin: ServerNoticeAction data class

struct ServerNoticeAction {
    std::string label;
    std::string url;
    ServerNoticeActionType type = ServerNoticeActionType::OPEN_URL;
};

// ---- Server Notice ----
// Original Kotlin: ServerNotice data class

struct ServerNotice {
    std::string noticeId;
    ServerNoticeType type = ServerNoticeType::MESSAGE;
    ServerNoticePriority priority = ServerNoticePriority::NORMAL;
    std::string title;
    std::string message;
    std::string adminContact;
    std::string actionUrl;
    std::string actionLabel;
    bool dismissable = true;
    int64_t expiresAt = 0;           // Unix epoch millis, 0 = no expiry
    int64_t createdAt = 0;           // Unix epoch millis
};

// ---- Server Notice List ----
// Original Kotlin: ServerNoticeList wrapper

struct ServerNoticeList {
    std::vector<ServerNotice> notices;
    int totalCount = 0;
    int unreadCount = 0;
};

// ---- Server Notice Consent ----
// Original Kotlin: UserConsent data class

struct ServerNoticeConsent {
    std::string noticeId;
    bool consentGiven = false;
    int64_t consentedAt = 0;         // Unix epoch millis
    std::string consentVersion;
};

// ================================================================
// Server Notice free functions
// ================================================================

// Parse a server notice state event content JSON.
// Original Kotlin: parse m.server_notice event content
ServerNotice parseServerNoticeEvent(const std::string& contentJson);

// Format a structured ServerNotice for user display.
// Original Kotlin: ServerNotice.format() display string
std::string formatServerNotice(const ServerNotice& notice);

// Check if a notice has been dismissed by the user.
// Original Kotlin: persist dismissed notice IDs in local store
bool isServerNoticeDismissed(const std::string& noticeId);

// Mark a notice as dismissed by the user.
// Original Kotlin: dismiss notice — store in local state
void dismissServerNotice(const std::string& noticeId);

// Get all active (non-dismissed, non-expired) notices from a list.
// Original Kotlin: ServerNoticeManager.getActiveNotices()
ServerNoticeList getActiveNotices(const ServerNoticeList& list);

// Get the available actions for a server notice.
// Original Kotlin: ServerNotice.getActions()
std::vector<ServerNoticeAction> getNoticeActions(const ServerNotice& notice);

// Build JSON body for a server notice state event.
// Original Kotlin: build m.server_notice state event content
std::string buildServerNoticeContent(const ServerNotice& notice);

// Record user consent for a server notice.
// Original Kotlin: record consent for policy/terms notice
void recordConsent(const std::string& noticeId, const std::string& consentVersion);

} // namespace progressive
