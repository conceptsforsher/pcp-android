#ifndef PROGRESSIVE_CONTENT_SCANNER_HPP
#define PROGRESSIVE_CONTENT_SCANNER_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// ---- Content Scanner (Antivirus Integration) ----

struct ScanResult {
    bool clean = true;
    bool scanned = false;
    std::string mxcUri;
    std::string serverName;      // which scanner server
    std::string threat;          // detected threat name
    std::string recommendation;  // "allow", "block", "warn"
};

// Parse scan result from Matrix content scanner API response.
ScanResult parseScanResult(const std::string& apiResponseJson);

// Build content scan request body.
std::string buildScanRequestBody(const std::string& mxcUri);

// Check if content scanning is available on the server.
bool isContentScannerAvailable(const std::string& serverCapabilitiesJson);

// Format scan result for display.
std::string formatScanResult(const ScanResult& result);

// ---- Server Notice Management ----

struct ServerNotice {
    std::string eventId;
    std::string body;
    std::string adminContact;
    std::string noticeType;      // "m.server_notice"
    int64_t timestampMs = 0;
    bool isRead = false;
    bool isDismissed = false;
};

// Parse server notice from Matrix event.
ServerNotice parseServerNotice(const std::string& eventContentJson, const std::string& eventId);

// Filter unread server notices.
std::vector<const ServerNotice*> getUnreadNotices(const std::vector<ServerNotice>& notices);

// Check if a message is a server notice.
bool isServerNotice(const std::string& eventContentJson);

// Format server notice for display.
std::string formatServerNotice(const ServerNotice& notice);

// ---- Terms of Service ----

struct TosInfo {
    std::string version;
    std::string url;
    bool accepted = false;
    bool pending = false;       // must accept before continuing
};

// Parse terms of service from login/register response.
TosInfo parseTosInfo(const std::string& responseJson);

// Check if user must accept ToS before proceeding.
bool mustAcceptTos(const std::string& responseJson);

// Build ToS acceptance request body.
std::string buildTosAcceptBody(const std::string& version);

} // namespace progressive

#endif // PROGRESSIVE_CONTENT_SCANNER_HPP
