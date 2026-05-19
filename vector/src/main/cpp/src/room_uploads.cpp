#include "progressive/room_uploads.hpp"

namespace progressive {

// ============================================================================
// UploadState — string conversion
// ============================================================================
//
// Original Kotlin (UploadState.kt):
//   enum class UploadState { UPLOADING, COMPLETED, FAILED, CANCELLED }

std::string uploadStateToString(UploadState state) {
    switch (state) {
        case UploadState::UPLOADING:  return "UPLOADING";
        case UploadState::COMPLETED:  return "COMPLETED";
        case UploadState::FAILED:     return "FAILED";
        case UploadState::CANCELLED:  return "CANCELLED";
        default:                      return "UNKNOWN";
    }
}

UploadState uploadStateFromString(const std::string& str) {
    if (str == "UPLOADING")  return UploadState::UPLOADING;
    if (str == "COMPLETED")  return UploadState::COMPLETED;
    if (str == "FAILED")     return UploadState::FAILED;
    if (str == "CANCELLED")  return UploadState::CANCELLED;
    return UploadState::FAILED;
}

// ============================================================================
// UploadProgress — JSON
// ============================================================================
//
// Original Kotlin (UploadProgress.kt):
//   data class UploadProgress(
//       val bytesUploaded: Long,
//       val totalBytes: Long,
//       val percentage: Int
//   )

std::string uploadProgressToJson(const UploadProgress& progress) {
    std::string json = "{";
    json += "\"bytesUploaded\":" + std::to_string(progress.bytesUploaded) + ",";
    json += "\"totalBytes\":" + std::to_string(progress.totalBytes) + ",";
    json += "\"percentage\":" + std::to_string(progress.percentage);
    json += "}";
    return json;
}

static int64_t extractInt64(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return 0;
    pos = json.find(':', pos);
    if (pos == std::string::npos) return 0;
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n')) pos++;
    if (pos >= json.size()) return 0;
    bool negative = false;
    if (json[pos] == '-') { negative = true; pos++; }
    int64_t val = 0;
    while (pos < json.size() && json[pos] >= '0' && json[pos] <= '9') {
        val = val * 10 + (json[pos] - '0');
        pos++;
    }
    return negative ? -val : val;
}

static int extractInt(const std::string& json, const std::string& key) {
    return static_cast<int>(extractInt64(json, key));
}

static std::string extractStr(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return "";
    pos = json.find(':', pos);
    if (pos == std::string::npos) return "";
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n')) pos++;
    if (pos >= json.size() || json[pos] != '"') return "";
    pos++;
    size_t end = pos;
    while (end < json.size() && json[end] != '"') {
        if (json[end] == '\\') end++;
        end++;
    }
    return json.substr(pos, end - pos);
}

static std::string extractObj(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return "";
    pos = json.find(':', pos);
    if (pos == std::string::npos) return "";
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n')) pos++;
    if (pos >= json.size() || json[pos] != '{') return "";
    int depth = 1;
    size_t start = pos;
    pos++;
    while (pos < json.size() && depth > 0) {
        if (json[pos] == '{') depth++;
        else if (json[pos] == '}') depth--;
        pos++;
    }
    return json.substr(start, pos - start);
}

UploadProgress parseUploadProgress(const std::string& json) {
    UploadProgress progress;
    progress.bytesUploaded = extractInt64(json, "bytesUploaded");
    progress.totalBytes = extractInt64(json, "totalBytes");
    progress.percentage = extractInt(json, "percentage");
    return progress;
}

// ============================================================================
// MxcUrl
// ============================================================================
//
// Original Kotlin (MxcUrl.kt):
//   data class MxcUrl(val serverName: String, val mediaId: String)

std::string MxcUrl::toMxcString() const {
    return std::string(kMxcPrefix) + serverName + "/" + mediaId;
}

std::string MxcUrl::toHttpUrl(const std::string& homeserverBase) const {
    std::string base = homeserverBase;
    if (!base.empty() && base.back() == '/') base.pop_back();
    return base + kMediaDownloadPath + "/" + serverName + "/" + mediaId;
}

std::string MxcUrl::toThumbnailUrl(const std::string& homeserverBase,
                                    int width, int height,
                                    const std::string& method) const {
    std::string base = homeserverBase;
    if (!base.empty() && base.back() == '/') base.pop_back();
    std::string url = base + kMediaThumbnailPath + "/" + serverName + "/" + mediaId;
    url += "?width=" + std::to_string(width);
    url += "&height=" + std::to_string(height);
    url += "&method=" + method;
    return url;
}

MxcUrl parseMxcUrl(const std::string& mxcUri) {
    MxcUrl result;
    if (mxcUri.compare(0, 6, "mxc://") != 0) return result;

    size_t serverStart = 6;
    auto slashPos = mxcUri.find('/', serverStart);
    if (slashPos == std::string::npos) return result;

    result.serverName = mxcUri.substr(serverStart, slashPos - serverStart);
    result.mediaId = mxcUri.substr(slashPos + 1);
    return result;
}

bool isValidMxcUrl(const std::string& mxcUri) {
    auto parsed = parseMxcUrl(mxcUri);
    return parsed.valid();
}

std::string getMxcServerName(const std::string& mxcUri) {
    return parseMxcUrl(mxcUri).serverName;
}

std::string getMxcMediaId(const std::string& mxcUri) {
    return parseMxcUrl(mxcUri).mediaId;
}

// ============================================================================
// ThumbnailInfo
// ============================================================================
//
// Original Kotlin (ThumbnailInfo.kt):
//   data class ThumbnailInfo(
//       val w: Int, val h: Int,
//       val mimetype: String?, val size: Long?
//   )

std::string thumbnailInfoToJson(const ThumbnailInfo& info) {
    std::string json = "{";
    json += "\"w\":" + std::to_string(info.w) + ",";
    json += "\"h\":" + std::to_string(info.h) + ",";
    json += "\"mimetype\":\"" + info.mimetype + "\",";
    json += "\"size\":" + std::to_string(info.size);
    json += "}";
    return json;
}

ThumbnailInfo parseThumbnailInfo(const std::string& json) {
    ThumbnailInfo info;
    info.w = extractInt(json, "w");
    info.h = extractInt(json, "h");
    info.mimetype = extractStr(json, "mimetype");
    info.size = extractInt64(json, "size");
    return info;
}

// ============================================================================
// MediaUploadResult
// ============================================================================
//
// Original Kotlin (MediaUploadResult.kt):
//   data class MediaUploadResult(...)

std::string mediaUploadResultToJson(const MediaUploadResult& result) {
    std::string json = "{";
    json += "\"contentUri\":\"" + result.contentUri + "\",";
    json += "\"mxcUrl\":\"" + result.mxcUrl + "\",";
    json += "\"fileName\":\"" + result.fileName + "\",";
    json += "\"mimeType\":\"" + result.mimeType + "\",";
    json += "\"size\":" + std::to_string(result.size) + ",";
    json += "\"width\":" + std::to_string(result.width) + ",";
    json += "\"height\":" + std::to_string(result.height) + ",";
    json += "\"thumbnailUrl\":\"" + result.thumbnailUrl + "\",";
    json += "\"thumbnailInfo\":" + thumbnailInfoToJson(result.thumbnailInfo) + ",";
    json += "\"state\":\"" + uploadStateToString(result.state) + "\"";
    json += "}";
    return json;
}

MediaUploadResult parseMediaUploadResult(const std::string& json) {
    MediaUploadResult result;
    result.contentUri = extractStr(json, "contentUri");
    result.mxcUrl = extractStr(json, "mxcUrl");
    result.fileName = extractStr(json, "fileName");
    result.mimeType = extractStr(json, "mimeType");
    result.size = extractInt64(json, "size");
    result.width = extractInt(json, "width");
    result.height = extractInt(json, "height");
    result.thumbnailUrl = extractStr(json, "thumbnailUrl");
    result.thumbnailInfo = parseThumbnailInfo(extractObj(json, "thumbnailInfo"));
    result.state = uploadStateFromString(extractStr(json, "state"));
    return result;
}

// ============================================================================
// Upload Request / Response
// ============================================================================
//
// Original Kotlin (MediaUploadAPI.kt):
//   POST /_matrix/media/v3/upload?filename=photo.jpg

std::string buildUploadUrl(const std::string& homeserverBase, const std::string& fileName) {
    std::string base = homeserverBase;
    if (!base.empty() && base.back() == '/') base.pop_back();
    std::string url = base + kMediaUploadPath;
    if (!fileName.empty()) {
        url += "?filename=" + fileName;
    }
    return url;
}

std::string buildUploadRequest(const std::string& fileName,
                                const std::string& mimeType,
                                int64_t fileSize,
                                bool encrypt) {
    std::string json = "{";
    json += "\"filename\":\"" + fileName + "\",";
    json += "\"content_type\":\"" + mimeType + "\",";
    json += "\"content_length\":" + std::to_string(fileSize);
    if (encrypt) {
        json += ",\"encrypt\":true";
    }
    json += "}";
    return json;
}

std::string buildUploadResponse(const std::string& contentUri) {
    std::string json = "{";
    json += "\"content_uri\":\"" + contentUri + "\"";
    json += "}";
    return json;
}

MediaUploadResult parseUploadResponse(const std::string& responseJson) {
    MediaUploadResult result;
    result.mxcUrl = extractStr(responseJson, "content_uri");
    if (!result.mxcUrl.empty()) {
        result.state = UploadState::COMPLETED;
    } else {
        result.state = UploadState::FAILED;
    }
    return result;
}

// ============================================================================
// Media Download URL builders
// ============================================================================
//
// Original Kotlin (ContentUrlResolver.kt):
//   fun buildDownloadUrl(homeserver: String, mxcUri: String): String

std::string buildMediaDownloadUrl(const std::string& homeserverBase,
                                   const std::string& mxcUri) {
    auto mxc = parseMxcUrl(mxcUri);
    if (!mxc.valid()) return "";
    return mxc.toHttpUrl(homeserverBase);
}

std::string buildThumbnailDownloadUrl(const std::string& homeserverBase,
                                       const std::string& mxcUri,
                                       int width, int height,
                                       const std::string& method) {
    auto mxc = parseMxcUrl(mxcUri);
    if (!mxc.valid()) return "";
    return mxc.toThumbnailUrl(homeserverBase, width, height, method);
}

std::string buildAuthenticatedDownloadUrl(const std::string& homeserverBase,
                                           const std::string& mxcUri,
                                           const std::string& accessToken) {
    auto url = buildMediaDownloadUrl(homeserverBase, mxcUri);
    if (url.empty()) return "";
    url += "?access_token=" + accessToken;
    return url;
}

// ============================================================================
// MXC URI utilities
// ============================================================================

std::string buildMxcUri(const std::string& serverName, const std::string& mediaId) {
    return std::string(kMxcPrefix) + serverName + "/" + mediaId;
}

bool isMxcUri(const std::string& uri) {
    return uri.compare(0, 6, kMxcPrefix) == 0;
}

std::string mxcToContentUri(const std::string& mxcUri) {
    return mxcUri;
}

// ============================================================================
// Upload cancellation
// ============================================================================

std::string buildCancellationRequestJson(const std::string& uploadId) {
    std::string json = "{";
    json += "\"uploadId\":\"" + uploadId + "\"";
    json += "}";
    return json;
}

bool parseCancellationResponse(const std::string& json) {
    auto status = extractStr(json, "status");
    return status == "cancelled" || status == "ok" || json.find("\"cancelled\":true") != std::string::npos;
}

// ============================================================================
// Sticker Detection (legacy)
// ============================================================================
//
// Original Kotlin (GetUploadsTask.kt:72):
//   .filter { it.getClearType() != EventType.STICKER }

bool isStickerEvent(const std::string& eventType) {
    return eventType == "m.sticker";
}

// ============================================================================
// Attachment URL Detection (legacy)
// ============================================================================
//
// Original Kotlin (GetUploadsTask.kt:68):
//   .like(EventEntityFields.DECRYPTION_RESULT_JSON,
//         TimelineEventFilter.DecryptedContent.URL)

bool hasAttachmentUrl(const std::string& decryptedContentJson) {
    auto pos = decryptedContentJson.find("\"url\"");
    if (pos == std::string::npos) return false;
    pos = decryptedContentJson.find("mxc://", pos);
    return pos != std::string::npos;
}

// ============================================================================
// Event Extraction (legacy)
// ============================================================================
//
// Original Kotlin (GetUploadsTask.kt:103-116):
//   val eventId = event.eventId
//   val messageContent = event.getClearContent()?.toModel<MessageContent>()
//   val messageWithAttachmentContent = (messageContent as? MessageWithAttachmentContent)

UploadEvent extractUploadEvent(
    const std::string& eventJson,
    const std::string& senderName,
    const std::string& senderAvatarUrl,
    bool isUniqueDisplayName)
{
    UploadEvent event;

    event.eventId = extractStr(eventJson, "event_id");
    event.senderId = extractStr(eventJson, "sender");

    event.senderName = senderName;
    event.senderAvatarUrl = senderAvatarUrl;

    auto contentJson = extractObj(eventJson, "content");
    if (!contentJson.empty()) {
        auto infoJson = extractObj(contentJson, "info");

        event.mxcUrl = extractStr(contentJson, "url");
        event.fileName = extractStr(contentJson, "filename");
        event.mimeType = extractStr(contentJson, "mimetype");

        if (!infoJson.empty()) {
            if (event.mimeType.empty()) event.mimeType = extractStr(infoJson, "mimetype");
            if (event.fileName.empty()) event.fileName = extractStr(infoJson, "filename");
            event.fileSize = extractInt64(infoJson, "size");
        }

        if (event.fileSize == 0) {
            event.fileSize = extractInt64(contentJson, "size");
        }
    }

    event.timestamp = extractInt64(eventJson, "origin_server_ts");

    return event;
}

// ============================================================================
// Uploads Filter (legacy)
// ============================================================================
//
// Original Kotlin (FilterFactory.createUploadsFilter(numberOfEvents)):
//   Creates a RoomEventFilter with types and not_types.

std::string createUploadsFilterJson(int numberOfEvents) {
    std::string json = "{";
    json += "\"room\":{";
    json += "\"timeline\":{";
    json += "\"limit\":" + std::to_string(numberOfEvents) + ",";
    json += "\"types\":[\"m.room.message\"],";
    json += "\"not_types\":[\"m.room.member\",\"m.sticker\"]";
    json += "}}}";
    return json;
}

// ============================================================================
// Serialization (legacy)
// ============================================================================

std::string getUploadsResultToJson(const GetUploadsResult& result) {
    std::string json = "{";
    json += "\"uploadEvents\":[";
    bool first = true;
    for (const auto& ev : result.uploadEvents) {
        if (!first) json += ",";
        first = false;
        json += "{";
        json += "\"eventId\":\"" + ev.eventId + "\",";
        json += "\"senderName\":\"" + ev.senderName + "\",";
        json += "\"mxcUrl\":\"" + ev.mxcUrl + "\",";
        json += "\"fileName\":\"" + ev.fileName + "\",";
        json += "\"mimeType\":\"" + ev.mimeType + "\",";
        json += "\"fileSize\":" + std::to_string(ev.fileSize) + ",";
        json += "\"timestamp\":" + std::to_string(ev.timestamp);
        json += "}";
    }
    json += "],";
    json += "\"nextToken\":\"" + result.nextToken + "\",";
    json += "\"hasMore\":" + std::string(result.hasMore ? "true" : "false");
    json += "}";
    return json;
}

} // namespace progressive
