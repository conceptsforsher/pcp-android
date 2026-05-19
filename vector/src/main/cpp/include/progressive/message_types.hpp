#pragma once

#include <string>
#include <cstdint>

namespace progressive {

// Matrix Client-Server API Message Types
//
// Ref: https://matrix.org/docs/spec/client_server/latest#m-room-message-msgtypes
//
// Original Kotlin (MessageType.kt:22-58):
//   object MessageType {
//       const val MSGTYPE_TEXT = "m.text"
//       const val MSGTYPE_EMOTE = "m.emote"
//       const val MSGTYPE_NOTICE = "m.notice"
//       const val MSGTYPE_IMAGE = "m.image"
//       const val MSGTYPE_AUDIO = "m.audio"
//       const val MSGTYPE_VIDEO = "m.video"
//       const val MSGTYPE_LOCATION = "m.location"
//       const val MSGTYPE_FILE = "m.file"
//       // ... fake types for sticker, poll, beacon, etc.
//   }

namespace MessageType {
    // Standard Matrix msgtypes
    constexpr const char* TEXT = "m.text";
    constexpr const char* EMOTE = "m.emote";
    constexpr const char* NOTICE = "m.notice";
    constexpr const char* IMAGE = "m.image";
    constexpr const char* AUDIO = "m.audio";
    constexpr const char* VIDEO = "m.video";
    constexpr const char* LOCATION = "m.location";
    constexpr const char* FILE = "m.file";

    // Verification (uses event type as msgtype)
    constexpr const char* VERIFICATION_REQUEST = "m.key.verification.request";

    // Local fake msgtypes (not in Matrix spec, used internally)
    constexpr const char* STICKER_LOCAL = "org.matrix.android.sdk.sticker";
    constexpr const char* POLL_START = "org.matrix.android.sdk.poll.start";
    constexpr const char* POLL_RESPONSE = "org.matrix.android.sdk.poll.response";
    constexpr const char* POLL_END = "org.matrix.android.sdk.poll.end";
    constexpr const char* BEACON_INFO = "org.matrix.android.sdk.beacon.info";
    constexpr const char* BEACON_LOCATION_DATA = "org.matrix.android.sdk.beacon.location.data";
    constexpr const char* VOICE_BROADCAST_INFO = "io.element.voicebroadcast.info";

    // Fun effects
    constexpr const char* CONFETTI = "nic.custom.confetti";
    constexpr const char* SNOWFALL = "io.element.effect.snowfall";
}

// Message format (HTML)
//
// Original Kotlin (MessageFormat.kt:21-23):
//   object MessageFormat { const val FORMAT_MATRIX_HTML = "org.matrix.custom.html" }
namespace MessageFormat {
    constexpr const char* MATRIX_HTML = "org.matrix.custom.html";
}

// JSON key for msgtype field
// Original Kotlin (MessageContent.kt:26): const val MSG_TYPE_JSON_KEY = "msgtype"
constexpr const char* MSG_TYPE_JSON_KEY = "msgtype";

} // namespace progressive
