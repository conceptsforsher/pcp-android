#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>
#include "progressive/message_content.hpp"

namespace progressive {

// ==== MessageImageInfoContent Interface ====
//
// Original Kotlin (MessageImageInfoContent.kt:23-26):
//   interface MessageImageInfoContent : MessageWithAttachmentContent {
//       val info: ImageInfo?
//   }

struct MessageImageInfoContent : MessageImageContent {
    // Has info from base MessageImageContent
};

struct MessageStickerContent : MessageImageContent {
    // Same structure as MessageImageContent
    // Original Kotlin (MessageStickerContent.kt:28-54):
    //   @Transient override val msgType = MSGTYPE_STICKER_LOCAL
    //   body, info, url, relatesTo, newContent, encryptedFileInfo
    // Same fields already in MessageImageContent
};

// ==== Default Message (fallback for unknown msgtypes) ====
//
// Original Kotlin (MessageDefaultContent.kt:25-31):
//   data class MessageDefaultContent(
//       @Json(name="msgtype") msgType, @Json(name="body") body,
//       @Json(name="m.relates_to") relatesTo,
//       @Json(name="m.new_content") newContent
//   ) : EventMessageContent

struct MessageDefaultContent : EventMessageContent {
    // Same as base MessageContent — generic fallback
};

// ==== Location Asset ====
//
// Original Kotlin (LocationAssetType.kt, LocationAsset.kt):
//   Used in MSC3488 location sharing

namespace LocationAssetType {
    constexpr const char* SELF = "m.self";
    constexpr const char* PIN = "m.pin";
}

struct LocationAsset {
    std::string type;    // e.g. "m.self", "m.pin"
};

struct LocationInfo {
    std::string geoUri;     // geo:lat,lon;u=uncertainty
    std::string description;
};

// ==== Enhanced Location Message ====
//
// Original Kotlin (MessageLocationContent.kt:25-90):
//   Adds MSC3488 fields: locationInfo, timestampMillis, text, asset

struct MessageEnhancedLocationContent : EventMessageContent {
    std::string geoUri;                    // "geo_uri" key
    LocationInfo locationInfo;             // "m.location" / "org.matrix.msc3488.location"
    int64_t timestampMillis = 0;           // "m.ts" / "org.matrix.msc3488.ts"
    std::string text;                      // "m.text" / "org.matrix.msc1767.text"
    LocationAsset locationAsset;           // "m.asset" / "org.matrix.msc3488.asset"

    // Original Kotlin: getBestGeoUri() = getBestLocationInfo()?.geoUri ?: geoUri
    std::string getBestGeoUri() const {
        if (!locationInfo.geoUri.empty()) return locationInfo.geoUri;
        return geoUri;
    }

    // Original Kotlin: isSelfLocation()
    bool isSelfLocation() const {
        return locationAsset.type.empty() || locationAsset.type == "m.self";
    }
};

// ==== Beacon Info (Live Location Sharing) ====
//
// Original Kotlin (MessageBeaconInfoContent.kt:28-67):
//   State event content for starting a live location share session

struct MessageBeaconInfoContent : EventMessageContent {
    std::string description;               // "description" key
    int64_t timeout = 0;                   // "timeout" key — milliseconds until inactive
    bool isLive = false;                   // "live" key
    int64_t timestampMillis = 0;           // "m.ts" / "org.matrix.msc3488.ts"
    LocationAsset locationAsset;           // "m.asset" / "org.matrix.msc3488.asset"

    bool isActive(int64_t nowMillis) const {
        if (!isLive) return false;
        if (timeout <= 0) return true;
        return (nowMillis - timestampMillis) < timeout;
    }
};

// Original Kotlin (MessageBeaconLocationDataContent.kt:28-53):
//   Event content for each location update in a live share

struct MessageBeaconLocationDataContent : EventMessageContent {
    LocationInfo locationInfo;             // "m.location" / "org.matrix.msc3488.location"
    int64_t timestampMillis = 0;           // "m.ts" / "org.matrix.msc3488.ts"
};

// ==== Poll Types ====
//
// Original Kotlin (PollType.kt:24-43):
//   enum class PollType {
//       DISCLOSED_UNSTABLE, DISCLOSED,
//       UNDISCLOSED_UNSTABLE, UNDISCLOSED
//   }

enum class PollType {
    DISCLOSED_UNSTABLE = 0,    // "org.matrix.msc3381.poll.disclosed"
    DISCLOSED = 1,             // "m.poll.disclosed"
    UNDISCLOSED_UNSTABLE = 2,  // "org.matrix.msc3381.poll.undisclosed"
    UNDISCLOSED = 3            // "m.poll.undisclosed"
};

// Original Kotlin (PollQuestion.kt:24-31):
//   data class PollQuestion(
//       @Json(name="org.matrix.msc1767.text") unstableQuestion,
//       @Json(name="m.text") question
//   ) { fun getBestQuestion() = question ?: unstableQuestion }

struct PollQuestion {
    std::string question;          // "m.text" key
    std::string unstableQuestion;  // "org.matrix.msc1767.text" key (MSC3381 unstable)

    // Original Kotlin: getBestQuestion() = question ?: unstableQuestion
    std::string getBestQuestion() const {
        if (!question.empty()) return question;
        return unstableQuestion;
    }
};

// Original Kotlin (PollAnswer.kt:23-33):
//   data class PollAnswer(
//       @Json(name="id") id,
//       @Json(name="org.matrix.msc1767.text") unstableAnswer,
//       @Json(name="m.text") answer
//   ) { fun getBestAnswer() = answer ?: unstableAnswer }

struct PollAnswer {
    std::string id;                // "id" key
    std::string answer;            // "m.text" key
    std::string unstableAnswer;    // "org.matrix.msc1767.text" key

    // Original Kotlin: getBestAnswer() = answer ?: unstableAnswer
    std::string getBestAnswer() const {
        if (!answer.empty()) return answer;
        return unstableAnswer;
    }
};

// Original Kotlin (PollCreationInfo.kt:25-36):
//   data class PollCreationInfo(
//       @Json(name="question") question: PollQuestion?,
//       @Json(name="kind") kind: PollType? = DISCLOSED_UNSTABLE,
//       @Json(name="max_selections") maxSelections: Int = 1,
//       @Json(name="answers") answers: List<PollAnswer>?
//   ) { fun isUndisclosed() = kind in listOf(UNDISCLOSED_UNSTABLE, UNDISCLOSED) }

struct PollCreationInfo {
    PollQuestion question;
    PollType kind = PollType::DISCLOSED_UNSTABLE;
    int maxSelections = 1;
    std::vector<PollAnswer> answers;

    // Original Kotlin: isUndisclosed()
    bool isUndisclosed() const {
        return kind == PollType::UNDISCLOSED_UNSTABLE
            || kind == PollType::UNDISCLOSED;
    }
};

// Original Kotlin (PollResponse.kt:23-25):
//   data class PollResponse(@Json(name="answers") answers: List<String>?)

struct PollResponse {
    std::vector<std::string> answerIds;  // "answers" key — list of selected answer IDs
};

// ==== Poll Message Types ====
//
// Original Kotlin (MessagePollContent.kt:25-47):
//   data class MessagePollContent(
//       @Transient override msgType = MSGTYPE_POLL_START,
//       body, relatesTo, newContent,
//       unstablePollCreationInfo, pollCreationInfo
//   ) : EventMessageContent

struct MessagePollContent : EventMessageContent {
    PollCreationInfo pollCreationInfo;        // "m.poll.start" key
    PollCreationInfo unstablePollCreationInfo; // "org.matrix.msc3381.poll.start" key

    // Original Kotlin: getBestPollCreationInfo() = pollCreationInfo ?: unstablePollCreationInfo
    PollCreationInfo getBestPollCreationInfo() const {
        if (!pollCreationInfo.answers.empty() || !pollCreationInfo.question.getBestQuestion().empty())
            return pollCreationInfo;
        return unstablePollCreationInfo;
    }
};

// Original Kotlin (MessagePollResponseContent.kt:27-49):
//   data class MessagePollResponseContent(
//       @Transient override msgType = MSGTYPE_POLL_RESPONSE,
//       body, relatesTo, newContent,
//       unstableResponse, response
//   ) : EventMessageContent

struct MessagePollResponseContent : EventMessageContent {
    PollResponse response;             // "m.response" key
    PollResponse unstableResponse;     // "org.matrix.msc3381.poll.response" key

    // Original Kotlin: getBestResponse() = response ?: unstableResponse
    PollResponse getBestResponse() const {
        if (!response.answerIds.empty()) return response;
        return unstableResponse;
    }
};

// Original Kotlin (MessageEndPollContent.kt:27-40):
//   data class MessageEndPollContent(
//       @Transient override msgType = MSGTYPE_POLL_END,
//       body, newContent, relatesTo,
//       unstableText, text
//   ) : EventMessageContent

struct MessageEndPollContent : EventMessageContent {
    std::string text;                  // "m.text" key
    std::string unstableText;          // "org.matrix.msc1767.text" key

    // Original Kotlin: getBestText() = text ?: unstableText
    std::string getBestText() const {
        if (!text.empty()) return text;
        return unstableText;
    }
};

// ==== JSON Parsing ====

// Parse poll content from event JSON
MessagePollContent parsePollContent(const std::string& contentJson);
MessagePollResponseContent parsePollResponseContent(const std::string& contentJson);
MessageEndPollContent parseEndPollContent(const std::string& contentJson);

// Parse beacon content
MessageBeaconInfoContent parseBeaconInfoContent(const std::string& contentJson);
MessageBeaconLocationDataContent parseBeaconLocationDataContent(const std::string& contentJson);

// Parse location content with MSC3488 enhancements
MessageEnhancedLocationContent parseEnhancedLocationContent(const std::string& contentJson);

// Parse sticker content
MessageStickerContent parseStickerContent(const std::string& contentJson);

// Serialize poll types
std::string pollTypeToString(PollType type);
PollType pollTypeFromString(const std::string& s);

// Serialize polls to JSON
std::string pollContentToJson(const MessagePollContent& poll);
std::string pollResponseToJson(const MessagePollResponseContent& response);
std::string endPollContentToJson(const MessageEndPollContent& endPoll);

// ==== Verification Message Types ====
//
// Original Kotlin (MessageVerificationRequestContent.kt:28-43):
//   data class MessageVerificationRequestContent(
//       @Json(name="msgtype") msgType = MSGTYPE_VERIFICATION_REQUEST,
//       body, fromDevice, methods, toUserId, timestamp,
//       format, formattedBody, relatesTo, newContent, transactionId
//   ) : EventMessageContent

struct MessageVerificationRequestContent : EventMessageContent {
    std::string fromDevice;              // "from_device" key
    std::vector<std::string> methods;    // "methods" key — e.g. ["m.sas.v1"]
    std::string toUserId;                // "to" key
    int64_t timestamp = 0;               // "timestamp" key
    std::string format;
    std::string formattedBody;
    std::string transactionId;           // set from eventId, not in JSON
};

// Original Kotlin (MessageVerificationStartContent.kt:26-40):
//   data class MessageVerificationStartContent(
//       fromDevice, hashes, keyAgreementProtocols,
//       messageAuthenticationCodes, shortAuthenticationStrings,
//       method, relatesTo, sharedSecret
//   )
struct MessageVerificationStartContent {
    std::string fromDevice;              // "from_device" key
    std::vector<std::string> hashes;     // "hashes" key
    std::vector<std::string> keyAgreementProtocols;    // "key_agreement_protocols"
    std::vector<std::string> messageAuthenticationCodes; // "message_authentication_codes"
    std::vector<std::string> shortAuthenticationStrings; // "short_authentication_string"
    std::string method;                  // "method" key
    std::string sharedSecret;            // "secret" key (SAS shared secret)
    RelationDefaultContent relatesTo;
    std::string transactionId;           // relatesTo.eventId
};

// Original Kotlin (MessageVerificationReadyContent.kt:26-33):
//   data class MessageVerificationReadyContent(
//       fromDevice, methods, relatesTo
//   )
struct MessageVerificationReadyContent {
    std::string fromDevice;
    std::vector<std::string> methods;
    RelationDefaultContent relatesTo;
    std::string transactionId;
};

// Original Kotlin (MessageVerificationDoneContent.kt:26-32):
//   data class MessageVerificationDoneContent(relatesTo)
struct MessageVerificationDoneContent {
    RelationDefaultContent relatesTo;
    std::string transactionId;
};

// Original Kotlin (MessageVerificationCancelContent.kt:28-36):
//   data class MessageVerificationCancelContent(code, reason, relatesTo)
struct MessageVerificationCancelContent {
    std::string code;                    // "code" key — e.g. "m.user"
    std::string reason;                  // "reason" key
    RelationDefaultContent relatesTo;
    std::string transactionId;
};

// Original Kotlin (MessageVerificationMacContent.kt:27-35):
//   data class MessageVerificationMacContent(mac, keys, relatesTo)
struct MessageVerificationMacContent {
    std::unordered_map<std::string, std::string> mac;  // "mac" key
    std::string keys;                    // "keys" key
    RelationDefaultContent relatesTo;
    std::string transactionId;
};

// Original Kotlin (MessageVerificationKeyContent.kt:28-36):
//   data class MessageVerificationKeyContent(key, relatesTo)
struct MessageVerificationKeyContent {
    std::string key;                     // "key" key — device's ephemeral public key
    RelationDefaultContent relatesTo;
    std::string transactionId;
};

// ==== Element Call Notification ====
//
// Original Kotlin (ElementCallNotifyContent.kt:24-33):
//   data class ElementCallNotifyContent(
//       application, callId, mentions, notifyType
//   )

struct Mentions {
    bool room = false;                   // "room" key — true if @room
    std::vector<std::string> userIds;    // "user_ids" key
};

struct ElementCallNotifyContent {
    std::string application;             // "application" key
    std::string callId;                  // "call_id" key
    Mentions mentions;                   // "m.mentions" key
    std::string notifyType;              // "notify_type" key

    // Original Kotlin: fun isUserMentioned(userId: String): Boolean
    bool isUserMentioned(const std::string& userId) const {
        if (mentions.room) return true;
        for (const auto& uid : mentions.userIds) {
            if (uid == userId) return true;
        }
        return false;
    }
};

// ==== MessageAudioEvent Wrapper ====
//
// Original Kotlin (MessageAudioEvent.kt:27-48):
//   value class MessageAudioEvent(val root: Event) {
//       val content: MessageAudioContent
//           get() = root.getClearContent().toModel<MessageContent>() as MessageAudioContent
//   }
struct MessageAudioEvent {
    std::string eventId;
    std::string roomId;
    std::string sender;
    int64_t timestamp = 0;
    MessageAudioContent content;

    // Convenience fields extracted from nested content
    std::string audioUrl;       // content.getFileUrl()
    int64_t duration = 0;       // content.audioInfo.duration or content.duration
    std::vector<int> waveform;  // content.audioWaveform.waveform
    bool voiceMessage = false;  // content.isVoiceMessage
    int64_t fileSize = 0;       // content.audioInfo.size or content.size

    // Original Kotlin: populate convenience accessors from content
    void resolve() {
        audioUrl = content.getFileUrl();
        duration = content.audioInfo.duration > 0 ? content.audioInfo.duration : content.duration;
        waveform = content.audioWaveform.waveform;
        voiceMessage = content.isVoiceMessage;
        fileSize = content.audioInfo.size > 0 ? content.audioInfo.size : content.size;
        if (fileSize <= 0) fileSize = 0;
    }
};

// ==== MessageFormatInfo ====
//
// Original Kotlin (MessageFormat.kt combined with formatted_body handling):
//   Encapsulates the format + formattedBody pair used in HTML messages.
//   format: usually "org.matrix.custom.html"
//   formattedBody: the HTML content
struct MessageFormatInfo {
    std::string format;        // "format" key, e.g. "org.matrix.custom.html"
    std::string formattedBody; // "formatted_body" key

    bool hasFormat() const {
        return !format.empty() && !formattedBody.empty();
    }

    // Original Kotlin: checks if this is valid Matrix HTML formatting
    bool isMatrixHtml() const {
        return format == "org.matrix.custom.html" && !formattedBody.empty();
    }
};

// ==== Audio Waveform Constants ====
//
// Original Kotlin (VoiceMessageHelper.kt constants):
//   Minimum/maximum number of amplitude values for a waveform visualization
//   Maximum amplitude value for each sample
namespace AudioWaveformConstants {
    constexpr int kAudioWaveformMinValues = 30;
    constexpr int kAudioWaveformMaxValues = 120;
    constexpr int kAudioWaveformMaxAmplitude = 1024;
}

// Parse verification contents
MessageVerificationRequestContent parseVerificationRequest(const std::string& contentJson);
MessageVerificationStartContent parseVerificationStart(const std::string& contentJson);
MessageVerificationReadyContent parseVerificationReady(const std::string& contentJson);
MessageVerificationDoneContent parseVerificationDone(const std::string& contentJson);
MessageVerificationCancelContent parseVerificationCancel(const std::string& contentJson);
MessageVerificationMacContent parseVerificationMac(const std::string& contentJson);
MessageVerificationKeyContent parseVerificationKey(const std::string& contentJson);

// Parse Element Call notification
ElementCallNotifyContent parseCallNotifyContent(const std::string& contentJson);

// ==== Message Type Helpers ====

// Original Kotlin: Determine the msgtype from content JSON (reads "msgtype" key)
// Returns empty string if not found
std::string getMessageType(const std::string& contentJson);

// Original Kotlin: Type-check functions that inspect msgtype field
//   fun isVoiceMessage() = root.getClearType() == MSGTYPE_AUDIO
//       && root.contentToModel<MessageAudioContent>()?.isVoiceMessage == true
bool isVoiceMessage(const std::string& contentJson);
bool isVideoMessage(const std::string& contentJson);
bool isFileMessage(const std::string& contentJson);
bool isImageMessage(const std::string& contentJson);
bool isAudioMessage(const std::string& contentJson);

// ==== JSON Builders for Media Content ====

// Serialize video content to JSON (for sending m.video messages)
// Original Kotlin (MessageVideoContent.kt toContent()):
//   {"msgtype":"m.video","body":"...","url":"mxc://...",
//    "info":{"w":640,"h":480,"duration":5000,"mimetype":"video/mp4","size":123456}}
std::string buildVideoContent(const MessageVideoContent& video);

// Serialize audio content to JSON (for sending m.audio messages)
// Original Kotlin (MessageAudioContent.kt toContent()):
//   {"msgtype":"m.audio","body":"...","url":"mxc://...",
//    "info":{"duration":5000,"mimetype":"audio/aac","size":123456},
//    "org.matrix.msc3245.voice":{},"org.matrix.msc1767.audio":{"duration":5000,"waveform":[...]}}
std::string buildAudioContent(const MessageAudioContent& audio);

// Serialize file content to JSON (for sending m.file messages)
// Original Kotlin (MessageFileContent.kt toContent()):
//   {"msgtype":"m.file","body":"...","filename":"doc.pdf",
//    "url":"mxc://...","info":{"mimetype":"application/pdf","size":123456}}
std::string buildFileContent(const MessageFileContent& file);

// Serialize sticker content to JSON
// Original Kotlin (MessageStickerContent.kt toContent()):
//   {"body":"...","url":"mxc://...","info":{"w":128,"h":128,"mimetype":"image/png","size":1024}}
std::string buildStickerContent(const MessageStickerContent& sticker);

// Serialize default/fallback content to JSON
// Original Kotlin (MessageDefaultContent.kt toContent()):
//   {"msgtype":"...","body":"..."}
std::string buildDefaultContent(const MessageDefaultContent& def);

// ==== Manual Parsers for Media Content ====

// Parse video content from event content JSON
// Original Kotlin (MessageVideoContent.kt fromContent()):
//   Reads url, info{w,h,duration,mimetype,size,thumbnail_url,thumbnail_info}
MessageVideoContent parseVideoContent(const std::string& contentJson);

// Parse audio content from event content JSON
// Original Kotlin (MessageAudioContent.kt fromContent()):
//   Reads url, info{duration,mimetype,size}, voice indicator, waveform
MessageAudioContent parseAudioContent(const std::string& contentJson);

// Parse file content from event content JSON
// Original Kotlin (MessageFileContent.kt fromContent()):
//   Reads url, filename, info{mimetype,size,thumbnail_url,thumbnail_info}
MessageFileContent parseFileContent(const std::string& contentJson);

// Parse sticker content from event content JSON
// (re-declared for completeness; implementation exists below)
MessageStickerContent parseStickerContent(const std::string& contentJson);

// ==== Audio Waveform Utilities ====

// Original Kotlin (VoiceMessageHelper.formatWaveform()):
//   Normalize waveform data for display:
//   1. Input waveform values are scaled to [0, kAudioWaveformMaxAmplitude]
//   2. Resampled to fit within [kAudioWaveformMinValues, kAudioWaveformMaxValues]
//   3. Normalized relative to the maximum value
std::vector<int> formatWaveform(const std::vector<int>& rawWaveform);

} // namespace progressive
