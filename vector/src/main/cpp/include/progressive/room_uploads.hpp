#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// Room Uploads — browse media/attachment events in a room.
//
// Original Kotlin (UploadsService.kt:19-30):
//   interface UploadsService {
//       suspend fun getUploads(numberOfEvents: Int,
//           since: String?): GetUploadsResult
//   }

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

} // namespace progressive
