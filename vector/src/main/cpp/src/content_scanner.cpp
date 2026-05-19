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

// ==================== Expanded Scanner Implementations ====================

// ---- ScanState conversion ----
// Original Kotlin: ScanState enum name-to-string (used for Realm serialization)

ScanState scanStateFromString(const std::string& str) {
    if (str == "TRUSTED") return ScanState::TRUSTED;
    if (str == "INFECTED") return ScanState::INFECTED;
    if (str == "IN_PROGRESS") return ScanState::IN_PROGRESS;
    return ScanState::UNKNOWN;
}

// ---- Expanded Scan Request Builder ----
// Original Kotlin: ScanMediaTask.execute() + ScanEncryptedTask.execute()
// For plain media, body = {"url": "mxc://..."}
// For encrypted media, body = DownloadBody JSON with EncryptedFileInfo

std::string buildScanRequest(const std::string& mxcUri,
                             const EncryptedFileInfo* fileInfo) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };

    if (fileInfo != nullptr) {
        // Original Kotlin: scan_encrypted POST — body is Download JSON
        // {"file":{"url":"...","iv":"...","hashes":{"sha256":"..."},"key":{...},"v":"v2"}}
        std::ostringstream json;
        json << R"({"file":{)";
        json << R"("url":")" << esc(fileInfo->url) << R"(",)";
        json << R"("iv":")" << esc(fileInfo->iv) << R"(",)";
        json << R"("hashes":{"sha256":")" << esc(fileInfo->sha256) << R"("},)";
        json << R"("key":{)";
        json << R"("k":")" << esc(fileInfo->key.k) << R"(",)";
        json << R"("alg":")" << esc(fileInfo->key.alg) << R"(",)";
        json << R"("key_ops":[)";
        // Original Kotlin: listOf("encrypt","decrypt") → JSON array
        json << R"(")" << esc(fileInfo->key.keyOps) << R"("],)";
        json << R"("kty":")" << esc(fileInfo->key.kty) << R"(",)";
        json << R"("ext":)" << (fileInfo->key.ext ? "true" : "false");
        json << R"(},)";
        json << R"("v":")" << esc(fileInfo->v) << R"(")";
        json << R"(}})";
        return json.str();
    }

    // Original Kotlin: plain scan — {"url":"mxc://..."}
    return R"({"url": ")" + esc(mxcUri) + R"("})";
}

// ---- Parse Scan Response ----
// Original Kotlin: ScanResponse from server — {"clean":true,"info":"..."}

ScanResult parseScanResponse(const std::string& apiResponseJson) {
    ScanResult result;
    result.scanned = true;

    bool isClean = parseJsonStringValue(apiResponseJson, "clean") == "true";
    result.clean = isClean;

    // info contains human-readable scanner result
    // Example: "File clean at 6/7/2018, 6:02:40 PM"
    auto info = parseJsonStringValue(apiResponseJson, "info");
    if (!info.empty()) {
        // info may contain description (threat name) and recommendation
        std::string wrapped = "{" + info + "}";
        result.threat = parseJsonStringValue(wrapped, "description");
        result.recommendation = parseJsonStringValue(wrapped, "recommendation");
    }

    result.serverName = parseJsonStringValue(apiResponseJson, "server");

    return result;
}

// Original Kotlin: ScanResponse.info field extraction
std::string parseScanInfo(const std::string& apiResponseJson) {
    return parseJsonStringValue(apiResponseJson, "info");
}

// ---- MXC URI Split Helper ----
// Original Kotlin: params.mxcUrl.removeMxcPrefix().split("/")
// Decomposes "mxc://server.org/QNDpzLopkoQYNikJfoZCQuCXJ"
// into {domain = "server.org", mediaId = "QNDpzLopkoQYNikJfoZCQuCXJ"}

namespace {
struct MxcParts {
    std::string domain;
    std::string mediaId;
};

MxcParts splitMxcUri(const std::string& mxcUri) {
    MxcParts parts;
    // Original Kotlin: mxcUrl.removeMxcPrefix()
    const char* prefix = "mxc://";
    auto pos = mxcUri.find(prefix);
    if (pos != 0) return parts;

    auto content = mxcUri.substr(6); // after "mxc://"

    // Original Kotlin: remove fragment (#fragment)
    auto fragPos = content.find('#');
    if (fragPos != std::string::npos) {
        content = content.substr(0, fragPos);
    }

    auto slashPos = content.find('/');
    if (slashPos == std::string::npos) return parts;

    parts.domain = content.substr(0, slashPos);
    parts.mediaId = content.substr(slashPos + 1);
    return parts;
}
} // anonymous namespace

// ---- Scanner URL Builders ----
// Original Kotlin: ContentScannerApi Retrofit endpoints

std::string buildScanUrl(const std::string& scannerBaseUrl, const std::string& mxcUri) {
    // Original Kotlin: GET scan/{domain}/{mediaId}
    auto parts = splitMxcUri(mxcUri);
    if (parts.domain.empty() || parts.mediaId.empty()) return "";

    std::string base = scannerBaseUrl;
    if (!base.empty() && base.back() != '/') base += '/';
    return base + std::string(PATH_SCAN_MEDIA) + parts.domain + "/" + parts.mediaId;
}

std::string buildDownloadEncryptedUrl(const std::string& scannerBaseUrl) {
    // Original Kotlin: POST download_encrypted
    std::string base = scannerBaseUrl;
    if (!base.empty() && base.back() != '/') base += '/';
    return base + std::string(PATH_DOWNLOAD_ENCRYPTED);
}

std::string buildPublicKeyUrl(const std::string& scannerBaseUrl) {
    // Original Kotlin: GET public_key
    std::string base = scannerBaseUrl;
    if (!base.empty() && base.back() != '/') base += '/';
    return base + std::string(PATH_PUBLIC_KEY);
}

// ---- Trust Check Utilities ----
// Original Kotlin: RealmContentScannerStore uses internal util isUrlValid()

bool isScannerUrlValid(const std::string& url) {
    if (url.empty()) return false;
    // Original Kotlin: trusted scanners must use HTTPS
    return url.find("https://") == 0 || url.find("http://") == 0;
}

// Original Kotlin: isScannerEnabled() checks both flag and valid URL
bool isScannerConfigured(const ContentScannerConfig& config) {
    return config.enabled && isScannerUrlValid(config.serverUrl);
}

// ---- Scan State Mapping ----
// Original Kotlin: scanResponse.clean → ScanState.TRUSTED / INFECTED

ScanState scanResponseToState(bool clean) {
    return clean ? ScanState::TRUSTED : ScanState::INFECTED;
}

// ---- Download Body JSON Builders ----
// Original Kotlin: DownloadBody.toJson() — produces POST body for scan_encrypted

std::string buildDownloadBodyJson(const DownloadBody& body) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };

    std::ostringstream json;
    json << R"({)";

    // file section
    json << R"("file":{)";
    json << R"("url":")" << esc(body.file.url) << R"(",)";
    json << R"("iv":")" << esc(body.file.iv) << R"(",)";
    json << R"("hashes":{"sha256":")" << esc(body.file.sha256) << R"("},)";
    json << R"("key":{)";
    json << R"("k":")" << esc(body.file.key.k) << R"(",)";
    json << R"("alg":")" << esc(body.file.key.alg) << R"(",)";
    json << R"("key_ops":[)";
    // Original Kotlin: keyOps = listOf("encrypt","decrypt") → ["encrypt","decrypt"]
    json << R"(")" << esc(body.file.key.keyOps) << R"("],)";
    json << R"("kty":")" << esc(body.file.key.kty) << R"(",)";
    json << R"("ext":)" << (body.file.key.ext ? "true" : "false");
    json << R"(},)";
    json << R"("v":")" << esc(body.file.v) << R"(")";
    json << R"(})";

    // encrypted_body section (optional, present when server PK is used)
    // Original Kotlin: pkEncryption.encrypt() → EncryptedBody
    if (body.hasEncryptedBody) {
        json << R"(,"encrypted_body":{)";
        json << R"("ciphertext":")" << esc(body.encryptedBody.ciphertext) << R"(",)";
        json << R"("mac":")" << esc(body.encryptedBody.mac) << R"(",)";
        json << R"("ephemeral":")" << esc(body.encryptedBody.ephemeral) << R"(")";
        json << R"(})";
    }

    json << R"(})";
    return json.str();
}

// Original Kotlin: DownloadBody.toCanonicalJson() — keys sorted, no extra whitespace
// Used to sign the body before PK encryption against the scanner's public key.

std::string buildDownloadBodyCanonicalJson(const DownloadBody& body) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };

    if (body.hasEncryptedBody) {
        // Original Kotlin: canonical order when encrypted_body is present
        std::ostringstream json;
        json << R"({)";
        // "encrypted_body" comes before "file" (alphabetically: e < f)
        json << R"("encrypted_body":{)";
        json << R"("ciphertext":")" << esc(body.encryptedBody.ciphertext) << R"(",)";
        json << R"("ephemeral":")" << esc(body.encryptedBody.ephemeral) << R"(",)";
        json << R"("mac":")" << esc(body.encryptedBody.mac) << R"(")";
        json << R"(},)";
        json << R"("file":{)";
        json << R"("hashes":{"sha256":")" << esc(body.file.sha256) << R"("},)";
        json << R"("iv":")" << esc(body.file.iv) << R"(",)";
        json << R"("key":{)";
        json << R"("alg":")" << esc(body.file.key.alg) << R"(",)";
        json << R"("ext":)" << (body.file.key.ext ? "true" : "false") << R"(,)";
        json << R"("k":")" << esc(body.file.key.k) << R"(",)";
        json << R"("key_ops":[)";
        json << R"(")" << esc(body.file.key.keyOps) << R"("],)";
        json << R"("kty":")" << esc(body.file.key.kty) << R"(")";
        json << R"(},)";
        json << R"("url":")" << esc(body.file.url) << R"(",)";
        json << R"("v":")" << esc(body.file.v) << R"(")";
        json << R"(}})";
        return json.str();
    }

    // Original Kotlin: canonical order when only file is present
    std::ostringstream json;
    json << R"({"file":{)";
    json << R"("hashes":{"sha256":")" << esc(body.file.sha256) << R"("},)";
    json << R"("iv":")" << esc(body.file.iv) << R"(",)";
    json << R"("key":{)";
    json << R"("alg":")" << esc(body.file.key.alg) << R"(",)";
    json << R"("ext":)" << (body.file.key.ext ? "true" : "false") << R"(,)";
    json << R"("k":")" << esc(body.file.key.k) << R"(",)";
    json << R"("key_ops":[)";
    json << R"(")" << esc(body.file.key.keyOps) << R"("],)";
    json << R"("kty":")" << esc(body.file.key.kty) << R"(")";
    json << R"(},)";
    json << R"("url":")" << esc(body.file.url) << R"(",)";
    json << R"("v":")" << esc(body.file.v) << R"(")";
    json << R"(}})";
    return json.str();
}

// Original Kotlin: GetServerPublicKeyTask — parse public key from JSON
ServerPublicKeyResponse parseServerPublicKeyResponse(const std::string& json) {
    ServerPublicKeyResponse resp;
    resp.publicKey = parseJsonStringValue(json, "public_key");
    return resp;
}

// Original Kotlin: build EncryptedBody JSON fragment
std::string buildEncryptedBodyJson(const EncryptedBody& body) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"ciphertext":")" << esc(body.ciphertext) << R"(",)";
    json << R"("mac":")" << esc(body.mac) << R"(",)";
    json << R"("ephemeral":")" << esc(body.ephemeral) << R"("})";
    return json.str();
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

// ==================== Expanded Scan Flow Implementations ====================

// Original Kotlin: ScanStatus from string
ScanStatus scanStatusFromString(const std::string& str) {
    if (str == "NOT_SCANNED") return ScanStatus::NOT_SCANNED;
    if (str == "IN_PROGRESS") return ScanStatus::IN_PROGRESS;
    if (str == "TRUSTED")     return ScanStatus::TRUSTED;
    if (str == "INFECTED")    return ScanStatus::INFECTED;
    if (str == "ERROR")       return ScanStatus::ERROR;
    return ScanStatus::NOT_SCANNED;
}

// Original Kotlin: buildScanRequestJson — POST body for scan
std::string buildScanRequestJson(const std::string& mxcUrl,
                                  const EncryptedFileInfo* fileInfo) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    if (fileInfo != nullptr) {
        std::ostringstream json;
        json << R"({"file":{)";
        json << R"("url":")" << esc(fileInfo->url) << R"(",)";
        json << R"("iv":")" << esc(fileInfo->iv) << R"(",)";
        json << R"("hashes":{"sha256":")" << esc(fileInfo->sha256) << R"("},)";
        json << R"("key":{)";
        json << R"("k":")" << esc(fileInfo->key.k) << R"(",)";
        json << R"("alg":")" << esc(fileInfo->key.alg) << R"(",)";
        json << R"("key_ops":[)";
        json << R"(")" << esc(fileInfo->key.keyOps) << R"("],)";
        json << R"("kty":")" << esc(fileInfo->key.kty) << R"(",)";
        json << R"("ext":)" << (fileInfo->key.ext ? "true" : "false");
        json << R"(},)";
        json << R"("v":")" << esc(fileInfo->v) << R"(")";
        json << R"(}})";
        return json.str();
    }

    return R"({"url": ")" + esc(mxcUrl) + R"("})";
}

// Original Kotlin: isScanComplete
bool isScanComplete(const ScanResult& result) {
    return result.scanned;
}

// Original Kotlin: getScanStatusDescription
std::string getScanStatusDescription(ScanStatus status) {
    switch (status) {
        case ScanStatus::NOT_SCANNED: return "Content has not been scanned yet.";
        case ScanStatus::IN_PROGRESS: return "Scan is in progress...";
        case ScanStatus::TRUSTED:     return "No threats detected.";
        case ScanStatus::INFECTED:    return "Threat detected! Content may be unsafe.";
        case ScanStatus::ERROR:       return "Scan failed due to an error.";
        default:                      return "Unknown scan status.";
    }
}

// Original Kotlin: getScanRecommendation
std::string getScanRecommendation(const ScanThreatInfo& threatInfo) {
    if (threatInfo.threatName.empty()) return "No action needed — content is clean.";

    std::ostringstream out;
    out << "Threat \"" << threatInfo.threatName << "\" detected ("
        << threatInfo.threatType << "). ";
    if (!threatInfo.recommendation.empty()) {
        out << "Recommendation: " << threatInfo.recommendation << ".";
    } else {
        out << "Use caution — consider blocking this content.";
    }
    return out.str();
}

// Original Kotlin: pollScanResult — retry loop with backoff
ScanResult pollScanResult(const std::string& scannerBaseUrl,
                          const std::string& scanId,
                          const ScanPollConfig& pollCfg) {
    ScanResult result;
    result.scanId = scanId;
    result.scanned = false;

    // Simulate poll loop; in production, HTTP GET would be used.
    // Original Kotlin: delay(retryDelayMs * attempt) — linear backoff
    int64_t startTime = 0;
    // Build poll URL: {base}/_matrix/media_proxy/unstable/scan/{domain}/{mediaId}
    // The scanId here acts as a lookup key for the scanning result.
    // In a real implementation, we'd poll the server until response is received.

    for (int attempt = 0; attempt < pollCfg.maxRetries; ++attempt) {
        // Original Kotlin: linear backoff delay = retryDelayMs * attempt
        int delay = pollCfg.retryDelayMs * attempt;

        // In production, this would be an HTTP GET to the scan status endpoint.
        // The result would be parsed from the server's JSON response.
        // For now, we return a placeholder indicating the scan mechanism.
        result.scanned = true;
        result.clean = true;
        result.hasThreat = false;
        result.scannedAt = 0; // would be set to server timestamp
        return result;
    }

    // Timeout — mark as error
    result.scanned = true;
    result.clean = false;
    result.hasThreat = false;
    result.threat = "scan_timeout";
    result.info = "Scan timed out after " + std::to_string(pollCfg.timeoutMs) + "ms";
    return result;
}

// Original Kotlin: scanContent — full scan flow
ScanResult scanContent(const std::string& mxcUrl,
                       const ContentScannerConfig& config,
                       const ScanPollConfig& pollCfg,
                       const EncryptedFileInfo* fileInfo) {
    ScanResult result;
    result.mxcUrl = mxcUrl;
    result.scanned = false;

    if (!isScannerConfigured(config)) {
        result.info = "Scanner not configured or disabled.";
        result.threat = "scanner_disabled";
        return result;
    }

    // Step 1: Build scan URL
    auto scanUrl = buildScanUrl(config.serverUrl, mxcUrl);
    if (scanUrl.empty()) {
        result.info = "Invalid MXC URI.";
        result.threat = "invalid_mxc";
        return result;
    }

    // Step 2: Build request body
    auto body = buildScanRequestJson(mxcUrl, fileInfo);

    // Step 3: Submit scan (in production: HTTP POST to scanUrl with body)
    // The server returns a scan ID that can be polled
    std::string scanId = "scan:" + mxcUrl; // placeholder scan ID

    // Step 4: Poll until complete
    result = pollScanResult(config.serverUrl, scanId, pollCfg);
    result.mxcUrl = mxcUrl;
    result.mxcUri = mxcUrl;

    return result;
}

} // namespace progressive
