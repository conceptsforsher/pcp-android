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

// ---- Scan State (mirrors Kotlin ScanState enum) ----
// Original Kotlin: enum class ScanState { TRUSTED, INFECTED, UNKNOWN, IN_PROGRESS }

enum class ScanState : int {
    TRUSTED = 0,
    INFECTED = 1,
    UNKNOWN = 2,
    IN_PROGRESS = 3
};

constexpr const char* scanStateToString(ScanState state) {
    switch (state) {
        case ScanState::TRUSTED:     return "TRUSTED";
        case ScanState::INFECTED:    return "INFECTED";
        case ScanState::UNKNOWN:     return "UNKNOWN";
        case ScanState::IN_PROGRESS: return "IN_PROGRESS";
        default:                      return "UNKNOWN";
    }
}

ScanState scanStateFromString(const std::string& str);

// Original Kotlin: data class ScanStatusInfo
struct ScanStatusInfo {
    ScanState state = ScanState::UNKNOWN;
    int64_t scanDateTimestamp = 0;
    std::string humanReadableMessage;
};

// ---- Content Scanner Error Types ----
// Original Kotlin: data class ContentScannerError

struct ContentScannerError {
    std::string info;
    std::string reason;
};

// Original Kotlin: ContentScannerError companion error reason constants
constexpr const char* REASON_MCS_MEDIA_REQUEST_FAILED = "MCS_MEDIA_REQUEST_FAILED";
// Original Kotlin: 400 - server failed to decrypt encrypted media
constexpr const char* REASON_MCS_MEDIA_FAILED_TO_DECRYPT = "MCS_MEDIA_FAILED_TO_DECRYPT";
// Original Kotlin: 403 - scan detected non-clean content
constexpr const char* REASON_MCS_MEDIA_NOT_CLEAN = "MCS_MEDIA_NOT_CLEAN";
// Original Kotlin: 403 - encrypted_body could not be decrypted (retry with new key)
constexpr const char* REASON_MCS_BAD_DECRYPTION = "MCS_BAD_DECRYPTION";
// Original Kotlin: 400 - malformed JSON in request body
constexpr const char* REASON_MCS_MALFORMED_JSON = "MCS_MALFORMED_JSON";

// ---- Server Public Key Response ----
// Original Kotlin: ServerPublicKeyResponse data class

struct ServerPublicKeyResponse {
    std::string publicKey;
};

// ---- Encrypted File Info (mirrors Kotlin crypto/attachments models) ----
// Original Kotlin: EncryptedFileKey (k, alg, keyOps, kty, ext)

struct EncryptedFileKey {
    std::string k;            // key
    std::string alg;          // algorithm, typically "A256CTR"
    std::string keyOps;       // key operations, comma-separated: "encrypt,decrypt"
    std::string kty;          // key type, typically "oct"
    bool ext = true;          // extractable
};

// Original Kotlin: EncryptedFileInfo (url, iv, hashes, key, v)

struct EncryptedFileInfo {
    std::string url;          // mxc URL
    std::string iv;           // initialization vector
    std::string sha256;       // SHA256 hash (from hashes map)
    EncryptedFileKey key;
    std::string v;            // version, e.g. "v2"
};

// Original Kotlin: EncryptedBody (ciphertext, mac, ephemeral)

struct EncryptedBody {
    std::string ciphertext;   // PK-encrypted content
    std::string mac;          // message authentication code
    std::string ephemeral;    // ephemeral key (curve25519)
};

// Original Kotlin: DownloadBody (file info + optional encrypted_body)

struct DownloadBody {
    EncryptedFileInfo file;         // unencrypted file metadata
    EncryptedBody encryptedBody;    // PK-encrypted payload (present when hasEncryptedBody)
    bool hasEncryptedBody = false;
};

// ---- Scanner Configuration ----
// Original Kotlin: ContentScannerInfoEntity (serverUrl, enabled)

struct ContentScannerConfig {
    std::string serverUrl;
    bool enabled = false;
};

// ---- API Paths ----
// Original Kotlin: NetworkConstants.URI_API_PREFIX_PATH_MEDIA_PROXY_UNSTABLE
//                   + ContentScannerApi endpoint paths

constexpr const char* MEDIA_PROXY_PATH = "_matrix/media_proxy/unstable/";
constexpr const char* PATH_SCAN_MEDIA = "_matrix/media_proxy/unstable/scan/";
constexpr const char* PATH_SCAN_ENCRYPTED = "_matrix/media_proxy/unstable/scan_encrypted";
constexpr const char* PATH_DOWNLOAD_ENCRYPTED = "_matrix/media_proxy/unstable/download_encrypted";
constexpr const char* PATH_PUBLIC_KEY = "_matrix/media_proxy/unstable/public_key";

// ==================== Expanded Scanner Functions ====================

// Original Kotlin: buildScanRequestBody extended — builds POST request JSON.
// For scan_encrypted: POST body is DownloadBody JSON with EncryptedFileInfo.
std::string buildScanRequest(const std::string& mxcUri,
                             const EncryptedFileInfo* fileInfo = nullptr);

// Original Kotlin: parseScanResponse — parses server scan response
// into ScanResult (handles both plain and encrypted scan responses).
ScanResult parseScanResponse(const std::string& apiResponseJson);

// Original Kotlin: ScanMediaTask — extracts human-readable info from scan.
std::string parseScanInfo(const std::string& apiResponseJson);

// ---- Scanner URL Builders ----
// Original Kotlin: ContentScannerApi URL patterns

// Build scan URL: {base}/_matrix/media_proxy/unstable/scan/{domain}/{mediaId}
std::string buildScanUrl(const std::string& scannerBaseUrl, const std::string& mxcUri);

// Build download-encrypted URL: {base}/_matrix/media_proxy/unstable/download_encrypted
std::string buildDownloadEncryptedUrl(const std::string& scannerBaseUrl);

// Build public key URL: {base}/_matrix/media_proxy/unstable/public_key
std::string buildPublicKeyUrl(const std::string& scannerBaseUrl);

// ---- Trust & Validation ----
// Original Kotlin: RealmContentScannerStore.isValidUrl() check plus TLS verification

// Validate scanner URL (must be https for trusted environments).
bool isScannerUrlValid(const std::string& url);

// Full scanner readiness check: URL is valid + scanner is enabled.
bool isScannerConfigured(const ContentScannerConfig& config);

// ---- Scan Result Utilities ----
// Original Kotlin: DefaultScanMediaTask — map clean bool to ScanState.
ScanState scanResponseToState(bool clean);

// ---- Encrypted Content JSON Builders ----
// Original Kotlin: DownloadBody.toJson() — full DownloadBody as JSON.
std::string buildDownloadBodyJson(const DownloadBody& body);

// Original Kotlin: DownloadBody.toCanonicalJson() — canonical (sorted-key) JSON.
std::string buildDownloadBodyCanonicalJson(const DownloadBody& body);

// Original Kotlin: parse server public key response JSON.
ServerPublicKeyResponse parseServerPublicKeyResponse(const std::string& json);

// Original Kotlin: build EncryptedBody JSON for request embedding.
std::string buildEncryptedBodyJson(const EncryptedBody& body);

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
