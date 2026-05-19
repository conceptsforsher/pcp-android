#include "progressive/server_notice_manager.hpp"
#include "progressive/room_content.hpp"
#include <sstream>
#include <algorithm>
#include <ctime>
#include <unordered_set>

namespace progressive {

// ====== Enum conversions ======

const char* resourceLimitTypeToString(ResourceLimitType type) {
    switch (type) {
        case ResourceLimitType::MAU: return "monthly_active_user";
        case ResourceLimitType::HARD: return "hard";
        case ResourceLimitType::SOFT: return "soft";
        case ResourceLimitType::STORAGE: return "storage";
        default: return "unknown";
    }
}

ResourceLimitType resourceLimitTypeFromString(const std::string& s) {
    if (s == "monthly_active_user") return ResourceLimitType::MAU;
    if (s == "hard") return ResourceLimitType::HARD;
    if (s == "soft") return ResourceLimitType::SOFT;
    if (s == "storage") return ResourceLimitType::STORAGE;
    return ResourceLimitType::UNKNOWN;
}

// ====== JSON helpers ======

std::string ServerNoticeManager::extractStr(const std::string& json, const std::string& key) {
    auto pp = json.find("\"" + key + "\"");
    if (pp == std::string::npos) return "";
    pp = json.find('"', pp + key.size() + 2);
    if (pp == std::string::npos) return "";
    pp++;
    size_t e = pp;
    while (e < json.size() && json[e] != '"') e++;
    return json.substr(pp, e - pp);
}

int64_t ServerNoticeManager::extractInt(const std::string& json, const std::string& key) {
    auto pp = json.find("\"" + key + "\"");
    if (pp == std::string::npos) return 0;
    pp = json.find(':', pp);
    if (pp == std::string::npos) return 0;
    pp++;
    while (pp < json.size() && (json[pp] == ' ' || json[pp] == '\t')) pp++;
    int64_t v = 0;
    while (pp < json.size() && json[pp] >= '0' && json[pp] <= '9') { v=v*10+(json[pp]-'0'); pp++; }
    return v;
}

bool ServerNoticeManager::extractBool(const std::string& json, const std::string& key) {
    return json.find("\"" + key + "\":true") != std::string::npos;
}

// ====== Constructor ======

ServerNoticeManager::ServerNoticeManager() {}

// ====== Error Parsing ======
// Original: MatrixError.kt — {errcode, error, limit_type, admin_contact, consent_uri, retry_after_ms, soft_logout}

ServerNoticeInfo ServerNoticeManager::parseMatrixError(const std::string& errorJson) {
    ServerNoticeInfo info;

    auto code = extractStr(errorJson, "errcode");
    if (code.empty()) code = extractStr(errorJson, "error");
    info.noticeType = code;

    info.noticeBody = extractStr(errorJson, "error");
    if (info.noticeBody.empty()) info.noticeBody = extractStr(errorJson, "message");

    // Resource limit fields (M_RESOURCE_LIMIT_EXCEEDED)
    auto limitType = extractStr(errorJson, "limit_type");
    info.limitType = resourceLimitTypeFromString(limitType);
    info.isResourceLimit = (code == MatrixErrorCodes::RESOURCE_LIMIT_EXCEEDED);

    info.adminContact = extractStr(errorJson, "admin_contact");
    if (info.adminContact.empty()) info.adminContact = extractStr(errorJson, "admin_uri");

    // Consent URI (M_CONSENT_NOT_GIVEN)
    info.consentUri = extractStr(errorJson, "consent_uri");
    info.isConsentRequired = (code == MatrixErrorCodes::CONSENT_NOT_GIVEN);

    // Rate limit (M_LIMIT_EXCEEDED)
    info.retryAfterMs = extractInt(errorJson, "retry_after_ms");
    info.isRateLimited = (code == MatrixErrorCodes::LIMIT_EXCEEDED);

    // Soft logout
    info.mode = extractBool(errorJson, "soft_logout") ? ResourceLimitMode::SOFT : ResourceLimitMode::HARD;

    return info;
}

ServerNoticeInfo ServerNoticeManager::parseServerNoticeContent(const std::string& contentJson) {
    ServerNoticeInfo info;
    info.noticeType = extractStr(contentJson, "msgtype");

    if (info.noticeType.empty()) {
        // Check for server notice specific fields
        auto body = extractStr(contentJson, "body");
        if (!body.empty()) {
            info.noticeBody = body;
            info.noticeType = "m.server_notice";
        }
    }

    info.noticeBody = extractStr(contentJson, "body");
    info.adminContact = extractStr(contentJson, "admin_contact");

    // Check server notice type
    auto serverNoticeType = extractStr(contentJson, "server_notice_type");
    if (!serverNoticeType.empty()) {
        info.noticeType = serverNoticeType;
    }

    return info;
}

// ====== Room Detection ======
// Original: m.server_notice room tag

bool ServerNoticeManager::isServerNoticeRoom(const std::string& roomTagsJson) {
    return roomTagsJson.find("\"m.server_notice\"") != std::string::npos;
}

bool ServerNoticeManager::isServerNoticeTag(const std::string& tagName) {
    return tagName == "m.server_notice";
}

// ====== Resource Limit Formatting ======
// Original: ResourceLimitErrorFormatter.format(matrixError, mode, separator, clickable)

std::string ServerNoticeManager::formatResourceLimitError(const ServerNoticeInfo& info, ResourceLimitMode mode) {
    std::ostringstream os;

    bool isHard = (mode == ResourceLimitMode::HARD);

    if (info.limitType == ResourceLimitType::MAU) {
        if (isHard) {
            os << "This homeserver has exceeded its Monthly Active User limit. "
               << "Please contact your service administrator to get this limit increased.";
        } else {
            os << "This homeserver is approaching its Monthly Active User limit. "
               << "You may be blocked from sending messages when the limit is reached. "
               << "Please contact your service administrator.";
        }
    } else {
        if (isHard) {
            os << "This homeserver has exceeded one of its resource limits. "
               << "Please contact your service administrator to continue.";
        } else {
            os << "This homeserver is approaching one of its resource limits. "
               << "Please contact your service administrator.";
        }
    }

    // Add admin contact
    if (!info.adminContact.empty()) {
        os << " Contact: " << info.adminContact;
    }

    return os.str();
}

std::string ServerNoticeManager::formatAdminContactLink(const std::string& adminUri) {
    if (adminUri.empty()) return "";
    return "<a href=\"" + adminUri + "\">contact your service administrator</a>";
}

std::string ServerNoticeManager::formatAdminContactText(const std::string& adminUri) {
    if (adminUri.empty()) return "contact your service administrator";
    return adminUri;
}

// Original: M_CONSENT_NOT_GIVEN — consent_uri field
std::string ServerNoticeManager::formatConsentRequired(const ServerNoticeInfo& info) {
    std::ostringstream os;
    os << "Your server requires you to agree to new terms.";
    if (!info.consentUri.empty()) {
        os << " Please visit: " << info.consentUri;
    }
    os << " " << formatAdminContactText(info.adminContact);
    return os.str();
}

// Original: M_LIMIT_EXCEEDED — retry_after_ms
std::string ServerNoticeManager::formatRateLimitMessage(const ServerNoticeInfo& info) {
    std::ostringstream os;
    os << "Too many requests. ";
    if (info.retryAfterMs > 0) {
        os << "Please retry after " << formatDowntime(info.retryAfterMs) << ".";
    } else {
        os << "Please wait a while and try again.";
    }
    return os.str();
}

// ====== Error Classification ======
// Original: MatrixError companion object constants

std::string ServerNoticeManager::getErrorCodeDescription(const std::string& errorCode) {
    if (errorCode == MatrixErrorCodes::FORBIDDEN) return "Access forbidden";
    if (errorCode == MatrixErrorCodes::UNKNOWN_TOKEN) return "Unknown access token";
    if (errorCode == MatrixErrorCodes::MISSING_TOKEN) return "Missing access token";
    if (errorCode == MatrixErrorCodes::LIMIT_EXCEEDED) return "Rate limit exceeded";
    if (errorCode == MatrixErrorCodes::NOT_FOUND) return "Not found";
    if (errorCode == MatrixErrorCodes::UNKNOWN) return "Unknown error";
    if (errorCode == MatrixErrorCodes::RESOURCE_LIMIT_EXCEEDED) return "Server resource limit exceeded";
    if (errorCode == MatrixErrorCodes::CONSENT_NOT_GIVEN) return "Consent not given";
    if (errorCode == MatrixErrorCodes::CANNOT_LEAVE_SERVER_NOTICE_ROOM) return "Cannot leave server notice room";
    if (errorCode == MatrixErrorCodes::USER_DEACTIVATED) return "User account deactivated";
    if (errorCode == MatrixErrorCodes::TERMS_NOT_SIGNED) return "Terms of service not signed";
    if (errorCode == MatrixErrorCodes::EXPIRED_ACCOUNT) return "Account expired";
    if (errorCode == "M_WEAK_PASSWORD") return "Password too weak";
    if (errorCode == "M_BAD_JSON") return "Bad JSON format";
    if (errorCode == "M_NOT_JSON") return "Not JSON content";
    if (errorCode == "M_USER_IN_USE") return "User ID already in use";
    if (errorCode == "M_ROOM_IN_USE") return "Room alias already in use";
    if (errorCode == "M_UNAUTHORIZED") return "Not authorized";
    if (errorCode == "M_UNSUPPORTED_ROOM_VERSION") return "Unsupported room version";
    if (errorCode == "M_TOO_LARGE") return "Request entity too large";
    if (errorCode == "M_GUEST_ACCESS_FORBIDDEN") return "Guest access forbidden";
    if (errorCode == "M_CAPTCHA_NEEDED") return "Captcha required";
    if (errorCode == "M_CAPTCHA_INVALID") return "Invalid captcha";
    if (errorCode == "M_THREEPID_IN_USE") return "Email/phone already in use";
    if (errorCode == "M_THREEPID_NOT_FOUND") return "Email/phone not found";
    if (errorCode == "M_SERVER_NOT_TRUSTED") return "Server not trusted";
    if (errorCode == "M_INVALID_USERNAME") return "Invalid username";
    if (errorCode == "M_BAD_STATE") return "Invalid state transition";
    if (errorCode == "M_MISSING_PARAM") return "Missing parameter";
    if (errorCode == "M_INVALID_PARAM") return "Invalid parameter";
    if (errorCode == "M_EXCLUSIVE") return "Resource reserved by application service";
    if (errorCode == "M_PASSWORD_TOO_SHORT") return "Password too short";
    if (errorCode == "M_PASSWORD_NO_DIGIT") return "Password needs a digit";
    if (errorCode == "M_PASSWORD_NO_UPPERCASE") return "Password needs uppercase";
    if (errorCode == "M_PASSWORD_NO_LOWERCASE") return "Password needs lowercase";
    if (errorCode == "M_PASSWORD_NO_SYMBOL") return "Password needs a symbol";
    if (errorCode == "M_PASSWORD_IN_DICTIONARY") return "Password is too common";
    return "Error: " + errorCode;
}

bool ServerNoticeManager::isResourceLimitError(const std::string& errorCode) {
    return errorCode == MatrixErrorCodes::RESOURCE_LIMIT_EXCEEDED;
}

bool ServerNoticeManager::isRateLimitError(const std::string& errorCode) {
    return errorCode == MatrixErrorCodes::LIMIT_EXCEEDED;
}

bool ServerNoticeManager::isConsentError(const std::string& errorCode) {
    return errorCode == MatrixErrorCodes::CONSENT_NOT_GIVEN;
}

bool ServerNoticeManager::isLogoutError(const std::string& errorCode) {
    return errorCode == MatrixErrorCodes::UNKNOWN_TOKEN;
}

bool ServerNoticeManager::isUserDeactivatedError(const std::string& errorCode) {
    return errorCode == MatrixErrorCodes::USER_DEACTIVATED;
}

// ====== Display Formatting ======

std::string ServerNoticeManager::getBannerColor(const ServerNoticeInfo& info) {
    if (info.limitType == ResourceLimitType::MAU && info.mode == ResourceLimitMode::HARD) return "#FF4444";
    if (info.isResourceLimit) return "#FF8C00";
    if (info.isConsentRequired) return "#FF8C00";
    if (info.isRateLimited) return "#FFD700";
    return "#2196F3";
}

std::string ServerNoticeManager::formatDowntime(int64_t retryAfterMs) {
    if (retryAfterMs <= 0) return "a moment";

    int64_t secs = retryAfterMs / 1000;
    if (secs < 60) return std::to_string(secs) + " second" + (secs != 1 ? "s" : "");
    int64_t mins = secs / 60;
    if (mins < 60) return std::to_string(mins) + " minute" + (mins != 1 ? "s" : "");
    int64_t hours = mins / 60;
    if (hours < 24) return std::to_string(hours) + " hour" + (hours != 1 ? "s" : "");
    return std::to_string(hours / 24) + " day" + (hours / 24 != 1 ? "s" : "");
}

std::string ServerNoticeManager::formatServerNotice(const ServerNoticeInfo& info) {
    std::ostringstream os;
    os << "[Server Notice] " << info.noticeBody;
    if (!info.adminContact.empty()) {
        os << " (Contact: " << info.adminContact << ")";
    }
    return os.str();
}

// ====== Serialization ======

std::string ServerNoticeManager::serverNoticeToJson(const ServerNoticeInfo& info) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    std::ostringstream os;
    os << R"({"type":")" << esc(info.noticeType)
       << R"(","body":")" << esc(info.noticeBody)
       << R"(","is_resource_limit":)" << (info.isResourceLimit ? "true" : "false")
       << R"(,"is_consent":)" << (info.isConsentRequired ? "true" : "false")
       << R"(,"is_rate_limit":)" << (info.isRateLimited ? "true" : "false")
       << R"(,"limit_type":")" << resourceLimitTypeToString(info.limitType)
       << R"(","admin_contact":")" << esc(info.adminContact)
       << R"(","consent_uri":")" << esc(info.consentUri)
       << R"(,"retry_after_ms":)" << info.retryAfterMs
       << R"(,"banner_color":")" << getBannerColor(info) << R"(")"
       << "}";
    return os.str();
}

std::string ServerNoticeManager::resourceLimitToJson(const ServerNoticeInfo& info) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    std::ostringstream os;
    os << R"({"type":")" << resourceLimitTypeToString(info.limitType)
       << R"(","mode":")" << (info.mode == ResourceLimitMode::HARD ? "hard" : "soft")
       << R"(","message":")" << esc(formatResourceLimitError(info, info.mode))
       << R"(","admin_contact":")" << esc(formatAdminContactText(info.adminContact))
       << R"(","admin_link":")" << esc(formatAdminContactLink(info.adminContact))
       << R"(","banner_color":")" << getBannerColor(info) << R"(")"
       << "}";
    return os.str();
}

// ================================================================
// Expanded Server Notice — structured types, dismiss, consent
// ================================================================

// ====== Server Notice Type conversions ======
// Original Kotlin: ServerNotice.Type enum

const char* serverNoticeTypeToString(ServerNoticeType type) {
    switch (type) {
        case ServerNoticeType::MESSAGE: return "message";
        case ServerNoticeType::USAGE_LIMIT: return "usage_limit";
        case ServerNoticeType::POLICY_UPDATE: return "policy_update";
        case ServerNoticeType::MAINTENANCE: return "maintenance";
        case ServerNoticeType::VERSION_UPGRADE: return "version_upgrade";
        case ServerNoticeType::SECURITY: return "security";
    }
    return "message";
}

ServerNoticeType serverNoticeTypeFromString(const std::string& s) {
    if (s == "usage_limit") return ServerNoticeType::USAGE_LIMIT;
    if (s == "policy_update") return ServerNoticeType::POLICY_UPDATE;
    if (s == "maintenance") return ServerNoticeType::MAINTENANCE;
    if (s == "version_upgrade") return ServerNoticeType::VERSION_UPGRADE;
    if (s == "security") return ServerNoticeType::SECURITY;
    return ServerNoticeType::MESSAGE;
}

// ====== Server Notice Priority conversions ======
// Original Kotlin: ServerNotice.Priority enum

const char* serverNoticePriorityToString(ServerNoticePriority priority) {
    switch (priority) {
        case ServerNoticePriority::LOW: return "low";
        case ServerNoticePriority::NORMAL: return "normal";
        case ServerNoticePriority::HIGH: return "high";
        case ServerNoticePriority::CRITICAL: return "critical";
    }
    return "normal";
}

ServerNoticePriority serverNoticePriorityFromString(const std::string& s) {
    if (s == "low") return ServerNoticePriority::LOW;
    if (s == "high") return ServerNoticePriority::HIGH;
    if (s == "critical") return ServerNoticePriority::CRITICAL;
    return ServerNoticePriority::NORMAL;
}

// ====== Server Notice Action Type conversions ======
// Original Kotlin: ServerNoticeAction.Type enum

const char* serverNoticeActionTypeToString(ServerNoticeActionType type) {
    switch (type) {
        case ServerNoticeActionType::OPEN_URL: return "open_url";
        case ServerNoticeActionType::DISMISS: return "dismiss";
        case ServerNoticeActionType::SETTINGS: return "settings";
    }
    return "open_url";
}

ServerNoticeActionType serverNoticeActionTypeFromString(const std::string& s) {
    if (s == "dismiss") return ServerNoticeActionType::DISMISS;
    if (s == "settings") return ServerNoticeActionType::SETTINGS;
    return ServerNoticeActionType::OPEN_URL;
}

// ====== JSON escape helper ======

static std::string escStr(const std::string& s) {
    std::string out;
    for (char c : s) {
        if (c == '"' || c == '\\') out += '\\';
        out += c;
    }
    return out;
}

// File-scoped JSON extraction helpers (mirror ServerNoticeManager:: private statics).
// Needed because free functions cannot access private static members.

static std::string extractStr(const std::string& json, const std::string& key) {
    auto pp = json.find("\"" + key + "\"");
    if (pp == std::string::npos) return "";
    pp = json.find('"', pp + key.size() + 2);
    if (pp == std::string::npos) return "";
    pp++;
    size_t e = pp;
    while (e < json.size() && json[e] != '"') e++;
    return json.substr(pp, e - pp);
}

static int64_t extractInt(const std::string& json, const std::string& key, int64_t def = 0) {
    auto pp = json.find("\"" + key + "\"");
    if (pp == std::string::npos) return def;
    pp = json.find(':', pp);
    if (pp == std::string::npos) return def;
    pp++;
    while (pp < json.size() && (json[pp] == ' ' || json[pp] == '\t')) pp++;
    int64_t v = 0;
    while (pp < json.size() && json[pp] >= '0' && json[pp] <= '9') { v=v*10+(json[pp]-'0'); pp++; }
    return v;
}

static bool extractBool(const std::string& json, const std::string& key) {
    return json.find("\"" + key + "\":true") != std::string::npos;
}

// ====== Dismissal State ======
// Original Kotlin: local store for dismissed notice IDs

static std::unordered_set<std::string> g_dismissedNoticeIds;

// ====== Consent State ======
// Original Kotlin: local store for consent records

static std::unordered_map<std::string, ServerNoticeConsent> g_consentRecords;

// ====== Parse Server Notice Event ======
// Original Kotlin: parse m.server_notice event content JSON

ServerNotice parseServerNoticeEvent(const std::string& contentJson) {
    ServerNotice notice;

    notice.noticeId = extractStr(contentJson, "id");
    if (notice.noticeId.empty()) notice.noticeId = extractStr(contentJson, "notice_id");

    auto typeStr = extractStr(contentJson, "notice_type");
    if (typeStr.empty()) typeStr = extractStr(contentJson, "type");
    notice.type = serverNoticeTypeFromString(typeStr);

    auto priorityStr = extractStr(contentJson, "priority");
    notice.priority = serverNoticePriorityFromString(priorityStr);

    notice.title = extractStr(contentJson, "title");
    notice.message = extractStr(contentJson, "message");
    if (notice.message.empty()) notice.message = extractStr(contentJson, "body");

    notice.adminContact = extractStr(contentJson, "admin_contact");
    notice.actionUrl = extractStr(contentJson, "action_url");
    notice.actionLabel = extractStr(contentJson, "action_label");
    notice.dismissable = !extractBool(contentJson, "not_dismissable");

    notice.expiresAt = extractInt(contentJson, "expires_at");
    notice.createdAt = extractInt(contentJson, "created_at");

    if (notice.createdAt == 0) {
        // Fallback: use origin_server_ts
        notice.createdAt = extractInt(contentJson, "origin_server_ts");
    }

    return notice;
}

// ====== Format Server Notice ======
// Original Kotlin: ServerNotice.format() user-facing display

std::string formatServerNotice(const ServerNotice& notice) {
    std::ostringstream os;

    if (!notice.title.empty()) {
        os << "[" << notice.title << "] ";
    } else {
        // Original Kotlin: prefix based on type
        switch (notice.type) {
            case ServerNoticeType::SECURITY: os << "[Security Alert] "; break;
            case ServerNoticeType::MAINTENANCE: os << "[Maintenance] "; break;
            case ServerNoticeType::VERSION_UPGRADE: os << "[Upgrade Required] "; break;
            case ServerNoticeType::POLICY_UPDATE: os << "[Policy Update] "; break;
            case ServerNoticeType::USAGE_LIMIT: os << "[Usage Limit] "; break;
            default: os << "[Server Notice] "; break;
        }
    }

    os << notice.message;

    if (!notice.adminContact.empty()) {
        os << " (Contact: " << notice.adminContact << ")";
    }

    if (!notice.actionLabel.empty() && !notice.actionUrl.empty()) {
        os << " [" << notice.actionLabel << ": " << notice.actionUrl << "]";
    }

    return os.str();
}

// ====== Dismissal Functions ======
// Original Kotlin: persist dismissed notice IDs in local state

bool isServerNoticeDismissed(const std::string& noticeId) {
    return g_dismissedNoticeIds.find(noticeId) != g_dismissedNoticeIds.end();
}

void dismissServerNotice(const std::string& noticeId) {
    g_dismissedNoticeIds.insert(noticeId);
}

// ====== Get Active Notices ======
// Original Kotlin: filter non-dismissed, non-expired notices

ServerNoticeList getActiveNotices(const ServerNoticeList& list) {
    ServerNoticeList active;
    active.totalCount = list.totalCount;

    for (const auto& notice : list.notices) {
        // Check dismissed
        if (isServerNoticeDismissed(notice.noticeId)) continue;

        // Check expired
        if (notice.expiresAt > 0) {
            // Use time_t for epoch seconds comparison (simplified)
            int64_t nowMs = static_cast<int64_t>(std::time(nullptr)) * 1000;
            if (nowMs > notice.expiresAt) continue;
        }

        active.notices.push_back(notice);
        active.unreadCount++;
    }

    active.totalCount = static_cast<int>(active.notices.size());
    return active;
}

// ====== Get Notice Actions ======
// Original Kotlin: ServerNotice.getActions() — available action list

std::vector<ServerNoticeAction> getNoticeActions(const ServerNotice& notice) {
    std::vector<ServerNoticeAction> actions;

    // Always add dismiss action if dismissable
    if (notice.dismissable) {
        ServerNoticeAction dismissAction;
        dismissAction.label = "Dismiss";
        dismissAction.type = ServerNoticeActionType::DISMISS;
        actions.push_back(dismissAction);
    }

    // Add primary action if URL and label provided
    if (!notice.actionUrl.empty() && !notice.actionLabel.empty()) {
        ServerNoticeAction primaryAction;
        primaryAction.label = notice.actionLabel;
        primaryAction.url = notice.actionUrl;
        primaryAction.type = ServerNoticeActionType::OPEN_URL;
        actions.push_back(primaryAction);
    }

    // Add settings action if applicable
    if (notice.type == ServerNoticeType::VERSION_UPGRADE || notice.type == ServerNoticeType::POLICY_UPDATE) {
        ServerNoticeAction settingsAction;
        settingsAction.label = "Settings";
        settingsAction.type = ServerNoticeActionType::SETTINGS;
        actions.push_back(settingsAction);
    }

    return actions;
}

// ====== Build Server Notice Content ======
// Original Kotlin: build m.server_notice state event content JSON

std::string buildServerNoticeContent(const ServerNotice& notice) {
    std::ostringstream os;
    os << "{";
    os << R"("notice_id":")" << escStr(notice.noticeId) << R"(")";
    os << R"(,"notice_type":")" << serverNoticeTypeToString(notice.type) << R"(")";
    os << R"(,"priority":")" << serverNoticePriorityToString(notice.priority) << R"(")";

    if (!notice.title.empty()) {
        os << R"(,"title":")" << escStr(notice.title) << R"(")";
    }

    os << R"(,"message":")" << escStr(notice.message) << R"(")";

    if (!notice.adminContact.empty()) {
        os << R"(,"admin_contact":")" << escStr(notice.adminContact) << R"(")";
    }
    if (!notice.actionUrl.empty()) {
        os << R"(,"action_url":")" << escStr(notice.actionUrl) << R"(")";
    }
    if (!notice.actionLabel.empty()) {
        os << R"(,"action_label":")" << escStr(notice.actionLabel) << R"(")";
    }

    os << R"(,"dismissable":)" << (notice.dismissable ? "true" : "false");

    if (notice.expiresAt > 0) {
        os << R"(,"expires_at":)" << notice.expiresAt;
    }

    os << R"(,"created_at":)" << notice.createdAt;
    os << "}";
    return os.str();
}

// ====== Record Consent ======
// Original Kotlin: record user consent for policy/terms notice

void recordConsent(const std::string& noticeId, const std::string& consentVersion) {
    ServerNoticeConsent consent;
    consent.noticeId = noticeId;
    consent.consentGiven = true;
    consent.consentedAt = static_cast<int64_t>(std::time(nullptr)) * 1000;
    consent.consentVersion = consentVersion;
    g_consentRecords[noticeId] = consent;
}

} // namespace progressive
