#include "progressive/content_scanner.hpp"
#include "progressive/json_parser.hpp"
#include <sstream>

namespace progressive {

ScanResult parseScanResult(const std::string& apiResponseJson) {
    ScanResult result;
    result.scanned = true;

    auto info = parseJsonStringValue(apiResponseJson, "info");
    if (!info.empty()) {
        std::string wrapped = "{" + info + "}";
        result.threat = parseJsonStringValue(wrapped, "description");
        result.recommendation = parseJsonStringValue(wrapped, "recommendation");
    }

    result.clean = parseJsonStringValue(apiResponseJson, "clean") == "true";
    result.serverName = parseJsonStringValue(apiResponseJson, "server");

    return result;
}

std::string buildScanRequestBody(const std::string& mxcUri) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    return R"({"url": ")" + esc(mxcUri) + R"("})";
}

bool isContentScannerAvailable(const std::string& serverCapabilitiesJson) {
    return serverCapabilitiesJson.find("m.content_scanner") != std::string::npos ||
           serverCapabilitiesJson.find("content_scanner") != std::string::npos;
}

std::string formatScanResult(const ScanResult& result) {
    if (!result.scanned) return "Not yet scanned.";
    if (result.clean) return "No threats detected.";
    std::ostringstream out;
    out << "Threat detected: " << result.threat;
    if (!result.recommendation.empty())
        out << " (recommendation: " << result.recommendation << ")";
    return out.str();
}

ServerNotice parseServerNotice(const std::string& eventContentJson, const std::string& eventId) {
    ServerNotice notice;
    notice.eventId = eventId;
    notice.body = parseJsonStringValue(eventContentJson, "body");
    notice.adminContact = parseJsonStringValue(eventContentJson, "admin_contact");
    notice.noticeType = parseJsonStringValue(eventContentJson, "server_notice_type");
    return notice;
}

std::vector<const ServerNotice*> getUnreadNotices(const std::vector<ServerNotice>& notices) {
    std::vector<const ServerNotice*> result;
    for (const auto& n : notices) {
        if (!n.isRead && !n.isDismissed) result.push_back(&n);
    }
    return result;
}

bool isServerNotice(const std::string& eventContentJson) {
    return eventContentJson.find("m.server_notice") != std::string::npos ||
           eventContentJson.find("server_notice") != std::string::npos;
}

std::string formatServerNotice(const ServerNotice& notice) {
    std::ostringstream out;
    out << "[Server Notice] " << notice.body;
    if (!notice.adminContact.empty()) {
        out << "\nContact: " << notice.adminContact;
    }
    return out.str();
}

TosInfo parseTosInfo(const std::string& responseJson) {
    TosInfo tos;

    auto params = parseJsonStringValue(responseJson, "params");
    if (params.empty()) return tos;

    std::string wrapped = "{" + params + "}";
    tos.version = parseJsonStringValue(wrapped, "version");
    tos.url     = parseJsonStringValue(wrapped, "url");

    // Check if it's in the login flows
    tos.pending = (responseJson.find("m.login.terms") != std::string::npos);

    return tos;
}

bool mustAcceptTos(const std::string& responseJson) {
    return responseJson.find("m.login.terms") != std::string::npos;
}

std::string buildTosAcceptBody(const std::string& version) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    return R"({"m.login.terms": {"version": ")" + esc(version) + R"("}})";
}

} // namespace progressive
