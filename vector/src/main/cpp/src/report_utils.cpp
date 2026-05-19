#include "progressive/report_utils.hpp"
#include <sstream>
#include <algorithm>
#include <cmath>

namespace progressive {

// ====== Enum conversions ======

const char* reportReasonCodeToString(ReportReasonCode code) {
    switch (code) {
        case ReportReasonCode::SPAM: return ReportReasonStr::SPAM;
        case ReportReasonCode::INAPPROPRIATE: return ReportReasonStr::INAPPROPRIATE;
        case ReportReasonCode::OFFENSIVE: return ReportReasonStr::OFFENSIVE;
        case ReportReasonCode::ILLEGAL: return ReportReasonStr::ILLEGAL;
        case ReportReasonCode::VIOLENCE: return ReportReasonStr::VIOLENCE;
        case ReportReasonCode::HARASSMENT: return ReportReasonStr::HARASSMENT;
        case ReportReasonCode::SUICIDE: return ReportReasonStr::SUICIDE;
        case ReportReasonCode::IMPERSONATION: return ReportReasonStr::IMPERSONATION;
        case ReportReasonCode::CHILD_ABUSE: return ReportReasonStr::CHILD_ABUSE;
        case ReportReasonCode::TERRORISM: return ReportReasonStr::TERRORISM;
        case ReportReasonCode::COPYRIGHT: return ReportReasonStr::COPYRIGHT;
        case ReportReasonCode::OTHER: return ReportReasonStr::OTHER;
        case ReportReasonCode::CUSTOM: return "";
    }
    return "";
}

ReportReasonCode reportReasonCodeFromString(const std::string& code) {
    if (code == ReportReasonStr::SPAM) return ReportReasonCode::SPAM;
    if (code == ReportReasonStr::INAPPROPRIATE) return ReportReasonCode::INAPPROPRIATE;
    if (code == ReportReasonStr::OFFENSIVE) return ReportReasonCode::OFFENSIVE;
    if (code == ReportReasonStr::ILLEGAL) return ReportReasonCode::ILLEGAL;
    if (code == ReportReasonStr::VIOLENCE) return ReportReasonCode::VIOLENCE;
    if (code == ReportReasonStr::HARASSMENT) return ReportReasonCode::HARASSMENT;
    if (code == ReportReasonStr::SUICIDE) return ReportReasonCode::SUICIDE;
    if (code == ReportReasonStr::IMPERSONATION) return ReportReasonCode::IMPERSONATION;
    if (code == ReportReasonStr::CHILD_ABUSE) return ReportReasonCode::CHILD_ABUSE;
    if (code == ReportReasonStr::TERRORISM) return ReportReasonCode::TERRORISM;
    if (code == ReportReasonStr::COPYRIGHT) return ReportReasonCode::COPYRIGHT;
    if (code == ReportReasonStr::OTHER) return ReportReasonCode::OTHER;
    if (code.empty()) return ReportReasonCode::CUSTOM;
    return ReportReasonCode::CUSTOM;
}

// ====== Report Reasons ======

std::vector<ReportReason> getReportReasons() {
    return {
        {ReportReasonStr::SPAM, "Spam or unsolicited advertising", false},
        {ReportReasonStr::INAPPROPRIATE, "Inappropriate content", false},
        {ReportReasonStr::OFFENSIVE, "Offensive or abusive content", false},
        {ReportReasonStr::ILLEGAL, "Illegal content", false},
        {ReportReasonStr::VIOLENCE, "Violence or threat of violence", false},
        {ReportReasonStr::HARASSMENT, "Harassment or bullying", false},
        {ReportReasonStr::SUICIDE, "Suicide or self-harm content", false},
        {ReportReasonStr::IMPERSONATION, "Impersonation of another user", false},
        {ReportReasonStr::CHILD_ABUSE, "Child abuse material", false},
        {ReportReasonStr::TERRORISM, "Terrorism or violent extremism", false},
        {ReportReasonStr::COPYRIGHT, "Copyright infringement", false},
        {ReportReasonStr::OTHER, "Other reason (please specify)", true},
    };
}

// ====== JSON escape helper ======

namespace {
    std::string escJson(const std::string& s) {
        std::string out;
        for (char c : s) {
            if (c == '"') out += "\\\"";
            else if (c == '\\') out += "\\\\";
            else if (c == '\n') out += "\\n";
            else out += c;
        }
        return out;
    }
}

// ====== buildReportContentBody — from ReportContentBody.kt ======
// Original Kotlin:
//   @Json(name = "score") val score: Int
//   @Json(name = "reason") val reason: String
std::string buildReportContentBody(const ReportContentBody& body) {
    std::ostringstream json;
    json << R"({"reason":")" << escJson(body.reason) << R"(",)";
    json << R"("score":)" << body.score;
    json << "}";
    return json.str();
}

// ====== buildReportRoomBody — from ReportRoomBody.kt ======
// Original Kotlin:
//   @Json(name = "reason") val reason: String
std::string buildReportRoomBody(const ReportRoomBody& body) {
    std::ostringstream json;
    json << R"({"reason":")" << escJson(body.reason) << R"(")";
    if (body.score != -100) {
        json << R"(,"score":)" << body.score;
    }
    json << "}";
    return json.str();
}

// ====== buildReportBody (legacy) ======

std::string buildReportBody(const ReportRequest& request) {
    std::ostringstream json;
    json << R"({"reason": ")" << escJson(request.reason) << R"(")";
    if (request.score != -100) {
        json << R"(,"score": )" << request.score;
    }
    json << "}";
    return json.str();
}

bool isValidReportReason(const std::string& reason) {
    for (const auto& r : getReportReasons()) {
        if (r.code == reason) return true;
    }
    return !reason.empty(); // custom reasons are valid too
}

bool isStandardReason(const std::string& reason) {
    for (const auto& r : getReportReasons()) {
        if (r.code == reason && !r.isCustom) return true;
    }
    return false;
}

std::string getReasonDescription(const std::string& code) {
    for (const auto& r : getReportReasons()) {
        if (r.code == code) return r.description;
    }
    return code;
}

std::string formatReportConfirmation(const ReportRequest& request) {
    std::ostringstream out;
    out << "Report this message?\n";
    out << "Reason: " << getReasonDescription(request.reason) << "\n";
    out << "This will notify server administrators.";
    return out.str();
}

bool isOffensive(int score, int threshold) {
    return score < threshold;
}

// ====== Server Notice Parsing ======
// Original Kotlin: server notice events are sent to m.server_notice rooms.
//   Event content JSON contains: server_notice_type, body, admin_contact,
//   and possibly embedded error data (M_RESOURCE_LIMIT_EXCEEDED, etc.)

ServerNoticeContent parseServerNotice(const std::string& contentJson) {
    ServerNoticeContent notice;

    // Check for server_notice marker
    notice.isServerNotice = contentJson.find("\"server_notice\"") != std::string::npos
        || contentJson.find("server_notice") != std::string::npos;

    // Extract body
    auto extractStr = [&](const std::string& key) -> std::string {
        auto pp = contentJson.find("\"" + key + "\"");
        if (pp == std::string::npos) return "";
        pp = contentJson.find('"', pp + key.size() + 2);
        if (pp == std::string::npos) return "";
        pp++;
        size_t e = pp;
        while (e < contentJson.size() && contentJson[e] != '"') e++;
        return contentJson.substr(pp, e - pp);
    };

    // Extract integer
    auto extractInt = [&](const std::string& key) -> int64_t {
        auto pp = contentJson.find("\"" + key + "\"");
        if (pp == std::string::npos) return 0;
        pp = contentJson.find(':', pp);
        if (pp == std::string::npos) return 0;
        pp++;
        while (pp < contentJson.size() && (contentJson[pp] == ' ' || contentJson[pp] == '\t')) pp++;
        int64_t v = 0;
        while (pp < contentJson.size() && contentJson[pp] >= '0' && contentJson[pp] <= '9') { v=v*10+(contentJson[pp]-'0'); pp++; }
        return v;
    };

    notice.body = extractStr("body");
    notice.adminContact = extractStr("admin_contact");
    notice.serverNoticeType = extractStr("server_notice_type");
    notice.limitType = extractStr("limit_type");
    notice.consentUri = extractStr("consent_uri");
    notice.retryAfterMs = extractInt("retry_after_ms");

    // Extract error code from embedded error JSON if present
    auto errcode = extractStr("errcode");
    if (!errcode.empty()) notice.errorCode = errcode;
    else if (contentJson.find("M_RESOURCE_LIMIT_EXCEEDED") != std::string::npos) notice.errorCode = "M_RESOURCE_LIMIT_EXCEEDED";
    else if (contentJson.find("M_CONSENT_NOT_GIVEN") != std::string::npos) notice.errorCode = "M_CONSENT_NOT_GIVEN";
    else if (contentJson.find("M_LIMIT_EXCEEDED") != std::string::npos) notice.errorCode = "M_LIMIT_EXCEEDED";

    // Classify
    notice.isResourceLimit = (notice.errorCode == "M_RESOURCE_LIMIT_EXCEEDED");
    notice.isConsentRequired = (notice.errorCode == "M_CONSENT_NOT_GIVEN");
    notice.isRateLimited = (notice.errorCode == "M_LIMIT_EXCEEDED");

    return notice;
}

bool isServerNotice(const std::string& contentJson) {
    return contentJson.find("server_notice") != std::string::npos;
}

std::string formatServerNoticeMessage(const ServerNoticeContent& notice) {
    std::ostringstream out;
    if (!notice.body.empty()) {
        out << notice.body;
    }
    if (notice.isResourceLimit) {
        out << "\n\nThis server has exceeded a resource limit.";
        if (!notice.adminContact.empty()) {
            out << "\nContact: " << notice.adminContact;
        }
    } else if (notice.isConsentRequired) {
        out << "\n\nYou must accept the terms of service.";
        if (!notice.consentUri.empty()) {
            out << "\nConsent: " << notice.consentUri;
        }
    } else if (notice.isRateLimited) {
        out << "\n\nYou are being rate limited.";
        if (notice.retryAfterMs > 0) {
            out << "\nRetry after: " << notice.retryAfterMs << "ms";
        }
    }
    return out.str();
}

// ====== Bug Report ======

std::string buildBugReportBody(const BugReport& report) {
    std::ostringstream json;
    json << "{";
    json << R"("text": ")" << escJson(report.description) << R"(")";
    json << R"(,"version": ")" << escJson(report.version) << R"(")";
    json << R"(,"user_id": ")" << escJson(report.userId) << R"(")";
    json << R"(,"can_contact": )" << (report.canContact ? "true" : "false");
    if (report.includeLogs && !report.logs.empty()) {
        json << R"(,"logs": ")" << escJson(report.logs) << R"(")";
    }
    json << "}";
    return json.str();
}

std::string formatBugReport(const BugReport& report) {
    std::ostringstream out;
    out << "Bug Report\n==========\n";
    out << "Version: " << report.version << "\n";
    out << "User: " << report.userId << "\n";
    out << "Device: " << report.deviceInfo << "\n";
    out << "Description:\n" << report.description << "\n";
    if (report.includeLogs) out << "\n[Logs attached]\n";
    return out.str();
}

bool isValidBugReport(const BugReport& report) {
    return !report.description.empty();
}

std::string truncateReportDescription(const std::string& desc, int maxLen) {
    if (static_cast<int>(desc.size()) <= maxLen) return desc;
    return desc.substr(0, maxLen - 3) + "...";
}

// ====== Rageshake Detection ======

bool detectShake(const std::vector<RageshakeEvent>& events, double threshold,
    int requiredSamples, int windowMs) {
    int shakeCount = 0;
    int64_t windowStart = 0;

    for (size_t i = 0; i < events.size(); ++i) {
        const auto& e = events[i];
        double magnitude = std::sqrt(e.accelerometerX * e.accelerometerX +
                                     e.accelerometerY * e.accelerometerY +
                                     e.accelerometerZ * e.accelerometerZ);

        if (magnitude > threshold) {
            if (windowStart == 0) windowStart = e.timestampMs;
            shakeCount++;

            if (e.timestampMs - windowStart > windowMs) {
                // Window expired, reset
                shakeCount = 0;
                windowStart = 0;
            }

            if (shakeCount >= requiredSamples) return true;
        }
    }

    return false;
}

bool shouldTriggerRageshake(const std::vector<RageshakeEvent>& events) {
    // Trigger on 3 shakes within 500ms with >15 m/s² acceleration
    return detectShake(events, 15.0, 3, 500);
}

} // namespace progressive
