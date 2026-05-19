#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// ============================================================================
// MXC URI Constants
// ============================================================================
//
// Matrix Content URIs identify media on a homeserver.
// Format: mxc://<server-name>/<media-id>
//
// Original Kotlin (ContentUrlRewriter.kt):
//   const val MXC_PREFIX = "mxc://"

constexpr const char* kMxcPrefix = "mxc://";
constexpr const char* kMediaDownloadPath = "/_matrix/media/v3/download";
constexpr const char* kMediaThumbnailPath = "/_matrix/media/v3/thumbnail";
constexpr const char* kMediaUploadPath = "/_matrix/media/v3/upload";

// ============================================================================
// UploadState — tracks the lifecycle of a media upload
// ============================================================================
//
// Original Kotlin (UploadState.kt):
//   enum class UploadState { UPLOADING, COMPLETED, FAILED, CANCELLED }

enum class UploadState {
    UPLOADING,
    COMPLETED,
    FAILED,
    CANCELLED
};

// Convert UploadState to/from string representations.
// Used for JSON serialization and JNI interop.
std::string uploadStateToString(UploadState state);
UploadState uploadStateFromString(const std::string& str);

// ============================================================================
// UploadProgress — tracks ongoing upload progress
// ============================================================================
//
// Original Kotlin (UploadProgress.kt):
//   data class UploadProgress(
//       val bytesUploaded: Long,
//       val totalBytes: Long,
//       val percentage: Int
//   )

struct UploadProgress {
    int64_t bytesUploaded = 0;
    int64_t totalBytes = 0;
    int percentage = 0;           // 0–100

    float fraction() const {
        if (totalBytes == 0) return 0.0f;
        return static_cast<float>(bytesUploaded) / static_cast<float>(totalBytes);
    }

    bool isComplete() const { return bytesUploaded >= totalBytes && totalBytes > 0; }
    bool isEmpty() const { return totalBytes == 0; }
};

// Build UploadProgress JSON for JNI bridge.
std::string uploadProgressToJson(const UploadProgress& progress);
UploadProgress parseUploadProgress(const std::string& json);

// ============================================================================
// MxcUrl — structured MXC URI with helper methods
// ============================================================================
//
// Original Kotlin (MxcUrl.kt):
//   data class MxcUrl(val serverName: String, val mediaId: String)

struct MxcUrl {
    std::string serverName;
    std::string mediaId;

    // Build an MXC string: "mxc://serverName/mediaId"
    std::string toMxcString() const;

    // Build an HTTP download URL from a base homeserver URL.
    // e.g. "https://matrix.org/_matrix/media/v3/download/matrix.org/abc123"
    std::string toHttpUrl(const std::string& homeserverBase) const;

    // Build an HTTP thumbnail URL.
    std::string toThumbnailUrl(const std::string& homeserverBase,
                               int width, int height,
                               const std::string& method = "scale") const;

    bool valid() const { return !serverName.empty() && !mediaId.empty(); }
};

// Parse an MXC URI into MxcUrl struct.
// "mxc://matrix.example.org/abc123" → {serverName="matrix.example.org", mediaId="abc123"}
MxcUrl parseMxcUrl(const std::string& mxcUri);

// Validate whether a string is a valid MXC URI.
bool isValidMxcUrl(const std::string& mxcUri);

// Extract server name from an MXC URI.
// "mxc://matrix.example.org/abc123" → "matrix.example.org"
std::string getMxcServerName(const std::string& mxcUri);

// Extract media ID from an MXC URI.
// "mxc://matrix.example.org/abc123" → "abc123"
std::string getMxcMediaId(const std::string& mxcUri);

// ============================================================================
// ThumbnailInfo — metadata for thumbnails
// ============================================================================
//
// Original Kotlin (ThumbnailInfo.kt):
//   data class ThumbnailInfo(
//       val w: Int, val h: Int,
//       val mimetype: String?, val size: Long?
//   )

struct ThumbnailInfo {
    int w = 0;
    int h = 0;
    std::string mimetype;
    int64_t size = 0;

    bool hasDimensions() const { return w > 0 && h > 0; }
    float aspectRatio() const {
        if (h == 0) return 0.0f;
        return static_cast<float>(w) / static_cast<float>(h);
    }
};

// Convert ThumbnailInfo to/from JSON.
std::string thumbnailInfoToJson(const ThumbnailInfo& info);
ThumbnailInfo parseThumbnailInfo(const std::string& json);

// ============================================================================
// MediaUploadResult — result of a successful upload
// ============================================================================
//
// Original Kotlin (MediaUploadResult.kt):
//   data class MediaUploadResult(
//       val contentUri: String?,
//       val mxcUrl: String?,
//       val fileName: String?,
//       val mimeType: String?,
//       val size: Long?,
//       val width: Int?,
//       val height: Int?,
//       val thumbnailUrl: String?,
//       val thumbnailInfo: ThumbnailInfo?
//   )

struct MediaUploadResult {
    std::string contentUri;       // Local URI on device
    std::string mxcUrl;           // The MXC URI returned by server
    std::string fileName;
    std::string mimeType;
    int64_t size = 0;
    int width = 0;
    int height = 0;
    std::string thumbnailUrl;     // MXC URI for thumbnail
    ThumbnailInfo thumbnailInfo;
    UploadState state = UploadState::UPLOADING;

    bool isSuccess() const { return state == UploadState::COMPLETED && !mxcUrl.empty(); }
    MxcUrl parsedMxcUrl() const { return parseMxcUrl(mxcUrl); }
};

// Convert MediaUploadResult to/from JSON.
std::string mediaUploadResultToJson(const MediaUploadResult& result);
MediaUploadResult parseMediaUploadResult(const std::string& json);

// ============================================================================
// Upload Request / Response
// ============================================================================
//
// Original Kotlin (MediaUploadAPI.kt):
//   POST /_matrix/media/v3/upload?filename=photo.jpg
//   Content-Type: application/octet-stream or multipart

// Build the upload endpoint URL for a given homeserver and optional filename.
std::string buildUploadUrl(const std::string& homeserverBase, const std::string& fileName = "");

// Build the multipart upload request metadata JSON.
// This is the JSON metadata sent alongside the file in a multipart upload.
std::string buildUploadRequest(const std::string& fileName,
                                const std::string& mimeType,
                                int64_t fileSize,
                                bool encrypt = false);

// Build the upload response JSON (for testing / JNI).
// Returns a JSON string matching the server's /upload response.
std::string buildUploadResponse(const std::string& contentUri);

// Parse the server's upload response JSON.
// Server returns: {"content_uri": "mxc://..."}
// This extracts the content_uri field.
MediaUploadResult parseUploadResponse(const std::string& responseJson);

// ============================================================================
// Media Download URL builders
// ============================================================================
//
// Original Kotlin (ContentUrlResolver.kt):
//   fun buildDownloadUrl(homeserver: String, mxcUri: String): String
//   fun buildThumbnailUrl(...)

// Build a media download URL from an MXC URI.
// e.g. "mxc://example.org/abc" + "https://matrix.org"
//   → "https://matrix.org/_matrix/media/v3/download/example.org/abc"
std::string buildMediaDownloadUrl(const std::string& homeserverBase,
                                   const std::string& mxcUri);

// Build a thumbnail download URL.
std::string buildThumbnailDownloadUrl(const std::string& homeserverBase,
                                       const std::string& mxcUri,
                                       int width, int height,
                                       const std::string& method = "scale");

// Build a download URL with authentication (for encrypted media).
std::string buildAuthenticatedDownloadUrl(const std::string& homeserverBase,
                                           const std::string& mxcUri,
                                           const std::string& accessToken);

// ============================================================================
// Utility: extract info from MXC URI by separate server / mediaId
// ============================================================================

// Build an MXC URI string from separate components.
std::string buildMxcUri(const std::string& serverName, const std::string& mediaId);

// Check if a string is an MXC URI (starts with "mxc://").
bool isMxcUri(const std::string& uri);

// Build a content URI from an MXC URI — essentially just returns the MXC.
// "mxc://example.org/abc" → "mxc://example.org/abc" (identity for upload results)
std::string mxcToContentUri(const std::string& mxcUri);

// ============================================================================
// Legacy UploadEvent / GetUploadsResult (preserved from original)
// ============================================================================

// Sender information for an upload event
//
// Original Kotlin (UploadEvent.kt:22-32):
//   data class UploadEvent(
//       val root: Event,
//       val eventId: String,
//       val contentWithAttachmentContent: MessageWithAttachmentContent,
//       val senderInfo: SenderInfo
//   )
struct UploadEvent {
    std::string eventId;
    std::string senderId;
    std::string senderName;
    std::string senderAvatarUrl;
    std::string mimeType;           // From attachment content
    std::string fileName;           // From attachment content
    std::string mxcUrl;             // From attachment content (decrypted URL)
    int64_t fileSize = 0;           // Size in bytes
    int64_t timestamp = 0;          // origin_server_ts
    bool isEncrypted = false;       // was the event encrypted
};

// Original Kotlin (GetUploadsResult.kt:19-26):
//   data class GetUploadsResult(
//       val uploadEvents: List<UploadEvent>,
//       val nextToken: String,
//       val hasMore: Boolean
//   )
struct GetUploadsResult {
    std::vector<UploadEvent> uploadEvents;
    std::string nextToken;          // pagination token
    bool hasMore = false;           // more events available

    bool empty() const { return uploadEvents.empty(); }
};

// Original Kotlin (GetUploadsTask.kt:30-37):
//   data class Params(
//       val roomId: String,
//       val isRoomEncrypted: Boolean,
//       val numberOfEvents: Int,
//       val since: String?
//   )
struct GetUploadsParams {
    std::string roomId;
    bool isRoomEncrypted = false;
    int numberOfEvents = 20;
    std::string since;             // pagination token, empty for first page
};

// Check if an event is a sticker (by clear type).
// Original Kotlin (GetUploadsTask.kt:72):
//   .filter { it.getClearType() != EventType.STICKER }
// Stickers have type "m.sticker"
bool isStickerEvent(const std::string& eventType);

// Check if an event has attachment URL content.
// For encrypted events: checks if decrypted content contains a URL.
// For unencrypted: checks message content is MessageWithAttachmentContent.
//
// Original Kotlin (GetUploadsTask.kt:68):
//   .like(EventEntityFields.DECRYPTION_RESULT_JSON,
//         TimelineEventFilter.DecryptedContent.URL)
//   and
//   val messageWithAttachmentContent =
//       (messageContent as? MessageWithAttachmentContent)
bool hasAttachmentUrl(const std::string& decryptedContentJson);

// Extract attachment metadata from event content JSON.
// Returns the MXC URL, filename, mime type, and file size.
//
// Original Kotlin (GetUploadsTask.kt:103-116):
//   event.getClearContent()?.toModel<MessageContent>()
//   (messageContent as? MessageWithAttachmentContent)
//   senderInfo from roomMemberHelper
UploadEvent extractUploadEvent(
    const std::string& eventJson,
    const std::string& senderName,
    const std::string& senderAvatarUrl,
    bool isUniqueDisplayName
);

// Create the uploads filter JSON for the Matrix API.
// The filter limits results to message types with attachments (image, video, audio, file).
//
// Original Kotlin (FilterFactory.createUploadsFilter(numberOfEvents)):
//   Creates a RoomEventFilter with types: m.room.message
//   and subtypes for image, video, audio, file content types.
std::string createUploadsFilterJson(int numberOfEvents);

// Serialize GetUploadsResult to JSON
std::string getUploadsResultToJson(const GetUploadsResult& result);

// ============================================================================
// Upload cancellation support
// ============================================================================

// Build a cancellation token / request identifier JSON.
// Used to correlate upload requests with their cancellations.
std::string buildCancellationRequestJson(const std::string& uploadId);

// Parse cancellation result.
bool parseCancellationResponse(const std::string& json);

} // namespace progressive
