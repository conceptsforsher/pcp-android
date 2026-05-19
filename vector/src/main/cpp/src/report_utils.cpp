#include "progressive/report_utils.hpp"
#include <sstream>
#include <algorithm>
#include <cmath>

namespace progressive {

std::vector<ReportReason> getReportReasons() {
    return {
        {"m.offensive", "Offensive or abusive content", false},
        {"m.spam", "Spam or unsolicited advertising", false},
        {"m.illegal", "Illegal content", false},
        {"m.violence", "Violence or threat of violence", false},
        {"m.harassment", "Harassment or bullying", false},
        {"m.suicide", "Suicide or self-harm content", false},
        {"m.impersonation", "Impersonation of another user", false},
        {"m.child_abuse", "Child abuse material", false},
        {"m.terrorism", "Terrorism or violent extremism", false},
        {"m.copyright", "Copyright infringement", false},
        {"m.other", "Other reason (please specify)", true},
    };
}

std::string buildReportBody(const ReportRequest& request) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"reason": ")" << esc(request.reason) << R"(")";
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

std::string buildBugReportBody(const BugReport& report) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << "{";
    json << R"("text": ")" << esc(report.description) << R"(")";
    json << R"(,"version": ")" << esc(report.version) << R"(")";
    json << R"(,"user_id": ")" << esc(report.userId) << R"(")";
    json << R"(,"can_contact": )" << (report.canContact ? "true" : "false");
    if (report.includeLogs && !report.logs.empty()) {
        json << R"(,"logs": ")" << esc(report.logs) << R"(")";
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
