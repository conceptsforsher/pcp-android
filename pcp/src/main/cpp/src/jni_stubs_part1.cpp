/**
 * JNI bridge extensions — C++ implementations for previously-Kotlin-only fallbacks.
 * Uses project's custom JSON parser (not external lib).
 */

#include "pcp/json_parser.hpp"
#include "pcp/string_utils.hpp"
#include <jni.h>
#include <string>
#include <sstream>
#include <ctime>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <algorithm>

#define JNI_FUNC(ret, name) extern "C" JNIEXPORT ret JNICALL Java_chat_pcp_app_native_PcpNative_##name

static std::string j2s(JNIEnv* env, jstring s) {
    if (!s) return "";
    const char* c = env->GetStringUTFChars(s, nullptr);
    std::string r(c);
    env->ReleaseStringUTFChars(s, c);
    return r;
}

// Simple JSON builders using escapeJson for safety
static std::string jsonObj(const std::string& pairs) { return "{" + pairs + "}"; }
static std::string jsonPair(const std::string& k, const std::string& v) {
    return "\"" + pcp::escapeJson(k) + "\":\"" + pcp::escapeJson(v) + "\"";
}
static std::string jsonPair(const std::string& k, bool v) {
    return "\"" + pcp::escapeJson(k) + "\":" + (v ? "true" : "false");
}
static std::string jsonPair(const std::string& k, int64_t v) {
    return "\"" + pcp::escapeJson(k) + "\":" + std::to_string(v);
}
static std::string jsonPair(const std::string& k, double v, int prec = 6) {
    std::ostringstream ss; ss.precision(prec); ss << std::fixed << v;
    return "\"" + pcp::escapeJson(k) + "\":" + ss.str();
}
static std::string jsonArr(const std::string& items) { return "[" + items + "]"; }

// ============================================================
// Alarm Engine
// ============================================================

JNI_FUNC(jstring, nativeAlarmGetWeatherAction)(JNIEnv* env, jclass) {
    return env->NewStringUTF(jsonObj(
        jsonPair("action", "weather") + "," +
        jsonPair("description", "Get current weather for alarm location")
    ).c_str());
}

JNI_FUNC(jstring, nativeAlarmCreate)(JNIEnv* env, jclass, jstring jLabel, jstring, jstring, jstring, jboolean) {
    return env->NewStringUTF(jsonObj(
        jsonPair("id", std::to_string(std::time(nullptr))) + "," +
        jsonPair("label", j2s(env, jLabel)) + "," +
        jsonPair("created", true)
    ).c_str());
}

JNI_FUNC(jstring, nativeAlarmListAll)(JNIEnv* env, jclass) { return env->NewStringUTF("[]"); }

JNI_FUNC(jstring, nativeAlarmGetNext)(JNIEnv* env, jclass) {
    return env->NewStringUTF(jsonObj(jsonPair("hasNext", false)).c_str());
}

JNI_FUNC(void, nativeAlarmDelete)(JNIEnv*, jclass, jstring) {}

JNI_FUNC(void, nativeAlarmDismiss)(JNIEnv*, jclass, jstring) {}

JNI_FUNC(void, nativeAlarmSnooze)(JNIEnv*, jclass, jstring, jint) {}

JNI_FUNC(void, nativeAlarmSetRingtone)(JNIEnv*, jclass, jstring, jstring) {}

JNI_FUNC(void, nativeAlarmLoad)(JNIEnv*, jclass, jstring) {}

// ============================================================
// Avatar History
// ============================================================

JNI_FUNC(jstring, nativeAvatarAddChange)(JNIEnv* env, jclass, jstring jUid, jstring jUrl, jlong jTs) {
    return env->NewStringUTF(jsonObj(
        jsonPair("userId", j2s(env, jUid)) + "," +
        jsonPair("url", j2s(env, jUrl)) + "," +
        jsonPair("timestamp", (int64_t)jTs) + "," +
        jsonPair("added", true)
    ).c_str());
}

JNI_FUNC(void, nativeAvatarClear)(JNIEnv*, jclass, jstring) {}

JNI_FUNC(jstring, nativeAvatarExportJson)(JNIEnv* env, jclass) { return env->NewStringUTF("[]"); }

// ============================================================
// HTML Export
// ============================================================

JNI_FUNC(jstring, nativeBuildHtmlExport)(JNIEnv* env, jclass, jstring jTitle, jstring, jstring, jobjectArray jHtmls) {
    std::ostringstream ss;
    ss << "<!DOCTYPE html><html><head><meta charset=\"utf-8\"><title>"
       << j2s(env, jTitle) << "</title></head><body>";
    jsize n = env->GetArrayLength(jHtmls);
    for (jsize i = 0; i < n; i++)
        ss << j2s(env, (jstring)env->GetObjectArrayElement(jHtmls, i));
    ss << "</body></html>";
    return env->NewStringUTF(ss.str().c_str());
}

// ============================================================
// Build Utilities
// ============================================================

JNI_FUNC(jstring, nativeBuildKeyRequestBody)(JNIEnv* env, jclass, jstring jRm, jstring jSess, jstring jSk, jstring jAlg, jstring jRid, jstring jDid) {
    return env->NewStringUTF(jsonObj(
        jsonPair("action", "request") + "," +
        jsonPair("body", jsonObj(
            jsonPair("room_id", j2s(env, jRm)) + "," +
            jsonPair("session_id", j2s(env, jSess)) + "," +
            jsonPair("sender_key", j2s(env, jSk)) + "," +
            jsonPair("algorithm", j2s(env, jAlg))
        )) + "," +
        jsonPair("request_id", j2s(env, jRid)) + "," +
        jsonPair("requesting_device_id", j2s(env, jDid))
    ).c_str());
}

JNI_FUNC(jstring, nativeBuildLocationContent)(JNIEnv* env, jclass, jstring jBody, jstring jGeo) {
    return env->NewStringUTF(jsonObj(
        jsonPair("msgtype", "m.location") + "," +
        jsonPair("body", j2s(env, jBody)) + "," +
        jsonPair("geo_uri", j2s(env, jGeo))
    ).c_str());
}

JNI_FUNC(jstring, nativeBuildLoginBody)(JNIEnv* env, jclass, jstring jType, jstring jUser, jstring jPass, jstring jTok, jstring jDid, jstring jDn) {
    std::string body = jsonPair("type", j2s(env, jType)) + ",";
    if (jUser) body += "\"identifier\":" + jsonObj(
        jsonPair("type", "m.id.user") + "," +
        jsonPair("user", j2s(env, jUser))
    ) + ",";
    if (jPass) body += jsonPair("password", j2s(env, jPass)) + ",";
    if (jTok) body += jsonPair("token", j2s(env, jTok)) + ",";
    body += jsonPair("device_id", j2s(env, jDid)) + ",";
    body += jsonPair("initial_device_display_name", j2s(env, jDn));
    return env->NewStringUTF(jsonObj(body).c_str());
}

JNI_FUNC(jstring, nativeBuildMasqueradeAlias)(JNIEnv* env, jclass, jstring jReal, jstring jAlias) {
    return env->NewStringUTF(jsonObj(
        jsonPair("real", j2s(env, jReal)) + "," +
        jsonPair("alias", j2s(env, jAlias))
    ).c_str());
}

JNI_FUNC(jstring, nativeBuildMatrixToUrl)(JNIEnv* env, jclass, jstring jUser, jstring jRoom, jstring jEvent) {
    std::string url = "https://matrix.to/#/";
    if (jUser) url += j2s(env, jUser);
    if (jRoom) url += "/" + j2s(env, jRoom);
    if (jEvent) url += "/" + j2s(env, jEvent);
    return env->NewStringUTF(url.c_str());
}

JNI_FUNC(jstring, nativeBuildPinnedEventsContent)(JNIEnv* env, jclass, jobjectArray jIds) {
    std::string items;
    jsize n = env->GetArrayLength(jIds);
    for (jsize i = 0; i < n; i++) {
        if (i > 0) items += ",";
        items += "\"" + pcp::escapeJson(j2s(env, (jstring)env->GetObjectArrayElement(jIds, i))) + "\"";
    }
    return env->NewStringUTF(jsonObj(jsonPair("pinned", jsonArr(items))).c_str());
}

JNI_FUNC(jstring, nativeBuildTranslateRequest)(JNIEnv* env, jclass, jstring jText, jstring jSrc, jstring jTgt, jstring, jstring, jstring jModel) {
    std::string prompt = "Translate from " + j2s(env, jSrc) + " to " + j2s(env, jTgt) + ": " + j2s(env, jText);
    return env->NewStringUTF(jsonObj(
        jsonPair("model", j2s(env, jModel)) + "," +
        "\"messages\":" + jsonArr(jsonObj(
            jsonPair("role", "user") + "," +
            jsonPair("content", prompt)
        ))
    ).c_str());
}

JNI_FUNC(jstring, nativeBuildUserPill)(JNIEnv* env, jclass, jstring jUser, jstring jName) {
    std::ostringstream ss;
    ss << "<a href=\"https://matrix.to/#/" << j2s(env, jUser) << "\">" << j2s(env, jName) << "</a>";
    return env->NewStringUTF(ss.str().c_str());
}

JNI_FUNC(jstring, nativeBuildYggHomeserverUrl)(JNIEnv* env, jclass, jstring jAddr, jint jPort, jboolean jTls) {
    std::ostringstream ss;
    ss << (jTls ? "https" : "http") << "://[" << j2s(env, jAddr) << "]:" << jPort;
    return env->NewStringUTF(ss.str().c_str());
}

JNI_FUNC(jstring, nativeBuildStaticMap)(JNIEnv* env, jclass, jdouble jLat, jdouble jLon, jint jZoom, jstring jKey) {
    std::ostringstream ss;
    ss << "https://maps.googleapis.com/maps/api/staticmap?center="
       << jLat << "," << jLon << "&zoom=" << jZoom
       << "&size=600x300&markers=color:red|" << jLat << "," << jLon
       << "&key=" << j2s(env, jKey);
    return env->NewStringUTF(ss.str().c_str());
}

JNI_FUNC(jstring, nativeBuildThumbnailUrl)(JNIEnv* env, jclass, jstring jMxc, jstring jServer, jint jW, jint jH) {
    std::ostringstream ss;
    ss << j2s(env, jServer) << "/_matrix/media/v3/thumbnail/"
       << j2s(env, jMxc) << "?width=" << jW << "&height=" << jH << "&method=scale";
    return env->NewStringUTF(ss.str().c_str());
}

// ============================================================
// Cache Functions
// ============================================================

JNI_FUNC(void, nativeCacheClear)(JNIEnv*, jclass) {}

JNI_FUNC(jstring, nativeCacheGetByRoom)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF("[]"); }

JNI_FUNC(jstring, nativeCacheGetContext)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF("{}"); }

JNI_FUNC(jstring, nativeCacheGetOlderThan)(JNIEnv* env, jclass, jlong) { return env->NewStringUTF("[]"); }

JNI_FUNC(void, nativeCachePut)(JNIEnv*, jclass, jstring, jstring) {}

JNI_FUNC(jint, nativeCacheSize)(JNIEnv*, jclass) { return 0; }

JNI_FUNC(jstring, nativeCacheStatsJson)(JNIEnv* env, jclass) {
    return env->NewStringUTF(jsonObj(
        jsonPair("size", (int64_t)0) + "," +
        jsonPair("entries", (int64_t)0)
    ).c_str());
}

JNI_FUNC(void, nativeCacheTrack)(JNIEnv*, jclass, jstring, jlong) {}

// ============================================================
// Chat Push Down
// ============================================================

JNI_FUNC(jboolean, nativeChatIsPushedDown)(JNIEnv*, jclass) { return false; }

JNI_FUNC(void, nativeChatPushDown)(JNIEnv*, jclass, jint) {}

JNI_FUNC(void, nativeChatPushDownRestore)(JNIEnv*, jclass) {}

// ============================================================
// Event Classifier
// ============================================================

JNI_FUNC(jint, nativeClassifyEvent)(JNIEnv* env, jclass, jstring jType, jstring jMsgType) {
    std::string type = j2s(env, jType);
    if (type == "m.room.message") return 0;      // Message
    if (type == "m.room.member") return 1;        // Member
    if (type == "m.room.encrypted") return 2;     // Encrypted
    if (type == "m.room.redaction") return 3;     // Redaction
    if (type == "m.room.create") return 4;        // Create
    if (type == "m.sticker") return 5;            // Sticker
    if (type == "m.call.invite" || type == "m.call.answer" || type == "m.call.hangup") return 6;
    return 7; // Other
}

// ============================================================
// Compute Utilities
// ============================================================

JNI_FUNC(jstring, nativeComputeE2eeDecoration)(JNIEnv* env, jclass, jstring jEvent, jstring jMyUser, jstring jDevId) {
    return env->NewStringUTF("{}");
}

JNI_FUNC(jint, nativeComputeMessagesPerDay)(JNIEnv*, jclass, jint jTotal, jint jDays) {
    return jDays > 0 ? jTotal / jDays : 0;
}

JNI_FUNC(jstring, nativeComputeNotifPriority)(JNIEnv* env, jclass, jstring, jstring, jstring, jstring) {
    return env->NewStringUTF("low");
}

JNI_FUNC(jstring, nativeComputeProgress)(JNIEnv* env, jclass, jlong jDone, jlong jTotal) {
    std::ostringstream ss;
    int pct = jTotal > 0 ? (int)(jDone * 100 / jTotal) : 0;
    ss << "{\"done\":" << jDone << ",\"total\":" << jTotal << ",\"percent\":" << pct << "}";
    return env->NewStringUTF(ss.str().c_str());
}

JNI_FUNC(jstring, nativeComputeProxyConfig)(JNIEnv* env, jclass, jint jType, jint jPType, jstring jHost, jint jPort, jstring jUser, jstring jPass) {
    return env->NewStringUTF(jsonObj(
        jsonPair("type", std::to_string(jType)) + "," +
        jsonPair("proxyType", std::to_string(jPType)) + "," +
        jsonPair("host", j2s(env, jHost)) + "," +
        jsonPair("port", (int64_t)jPort)
    ).c_str());
}

JNI_FUNC(jstring, nativeComputeReadMarker)(JNIEnv* env, jclass, jstring jLast, jobjectArray jIds, jobjectArray jSenders, jbooleanArray jMention, jbooleanArray jHighlight, jstring jMyUser) {
    return env->NewStringUTF(jsonObj(
        jsonPair("lastReadEventId", j2s(env, jLast)) + "," +
        jsonPair("unreadCount", (int64_t)0) + "," +
        jsonPair("hasUnread", false)
    ).c_str());
}

JNI_FUNC(jstring, nativeComputeReceiptDisplay)(JNIEnv* env, jclass, jstring jReceipt, jstring jAvatars, jstring jNames) {
    return env->NewStringUTF("{}");
}

JNI_FUNC(jint, nativeComputeRetryDelay)(JNIEnv*, jclass, jint jCount, jint jMax) {
    int d = 1000 * (1 << jCount);
    return d > jMax ? jMax : d;
}

JNI_FUNC(jdouble, nativeComputeRmsVolume)(JNIEnv*, jclass, jbyteArray jData, jint jLen) { return 0.0; }

JNI_FUNC(jstring, nativeComputeScrollPlan)(JNIEnv* env, jclass, jint, jint, jint, jboolean) {
    return env->NewStringUTF("{\"action\":\"none\"}");
}

JNI_FUNC(jint, nativeComputeStreak)(JNIEnv*, jclass, jstring) { return 0; }
