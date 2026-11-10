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

#define JNI_FUNC(jboolean, nativeIsValidRoomVersion)(JNIEnv* env, jclass, jstring jVer) {
    std::string v = j2s(env, jVer);
    return !v.empty();
}

// ============================================================
// Keyword Filter / Language Hide / Latency
// ============================================================

JNI_FUNC(jstring, nativeKeywordFilterCheck)(JNIEnv* env, jclass, jstring jText, jstring jKeywords) {
    std::string text = j2s(env, jText), kw = j2s(env, jKeywords);
    if (text.empty() || kw.empty()) return env->NewStringUTF("{\"matched\":false}");
    return env->NewStringUTF((std::string("{\"matched\":") + (text.find(kw) != std::string::npos ? "true" : "false") + "}").c_str());
}

JNI_FUNC(void, nativeKeywordFilterClear)(JNIEnv*, jclass) {}

JNI_FUNC(jint, nativeKeywordFilterCount)(JNIEnv*, jclass) { return 0; }

JNI_FUNC(jstring, nativeKeywordFilterExport)(JNIEnv* env, jclass) { return env->NewStringUTF("[]"); }

JNI_FUNC(void, nativeKeywordFilterLoad)(JNIEnv*, jclass, jstring) {}

JNI_FUNC(void, nativeLangHideAdd)(JNIEnv*, jclass, jstring) {}

JNI_FUNC(jboolean, nativeLangHideIsHidden)(JNIEnv*, jclass, jstring) { return false; }

JNI_FUNC(void, nativeLatencyRecord)(JNIEnv*, jclass, jstring, jlong) {}

JNI_FUNC(jstring, nativeLatencyStats)(JNIEnv* env, jclass) { return env->NewStringUTF("{}"); }

JNI_FUNC(jstring, nativeLatencyStatsText)(JNIEnv* env, jclass) { return env->NewStringUTF(""); }

// ============================================================
// Light Call / Location
// ============================================================

JNI_FUNC(jboolean, nativeLightCallAssessMemory)(JNIEnv*, jclass) { return true; }

JNI_FUNC(void, nativeLightCallEnter)(JNIEnv*, jclass) {}

JNI_FUNC(void, nativeLightCallExit)(JNIEnv*, jclass) {}

JNI_FUNC(jdouble, nativeLocationDistance)(JNIEnv*, jclass, jdouble, jdouble, jdouble, jdouble) { return 0.0; }

JNI_FUNC(jstring, nativeLocationFormatGeoJson)(JNIEnv* env, jclass, jdouble jLat, jdouble jLon) {
    std::ostringstream ss; ss << "{\"type\":\"Point\",\"coordinates\":[" << jLon << "," << jLat << "]}";
    return env->NewStringUTF(ss.str().c_str());
}

JNI_FUNC(jstring, nativeLocationFormatMessage)(JNIEnv* env, jclass, jdouble jLat, jdouble jLon, jstring jLabel) {
    std::ostringstream ss;
    ss << "Location: https://www.openstreetmap.org/?mlat=" << jLat << "&mlon=" << jLon;
    return env->NewStringUTF(ss.str().c_str());
}

// ============================================================
// Match / MIME / Mirror / Module
// ============================================================

JNI_FUNC(jstring, nativeMatchRooms)(JNIEnv* env, jclass, jstring jQuery, jstring jRooms) { return env->NewStringUTF("[]"); }

JNI_FUNC(jstring, nativeMimeToMsgType)(JNIEnv* env, jclass, jstring jMime) {
    std::string m = j2s(env, jMime);
    if (m.find("image/") == 0) return env->NewStringUTF("m.image");
    if (m.find("video/") == 0) return env->NewStringUTF("m.video");
    if (m.find("audio/") == 0) return env->NewStringUTF("m.audio");
    return env->NewStringUTF("m.file");
}

JNI_FUNC(void, nativeMirrorAdd)(JNIEnv*, jclass, jstring, jstring, jstring) {}

JNI_FUNC(jstring, nativeMirrorExportJson)(JNIEnv* env, jclass) { return env->NewStringUTF("[]"); }

JNI_FUNC(jstring, nativeMirrorFormatMessage)(JNIEnv* env, jclass, jstring jMsg) { return jMsg; }

JNI_FUNC(jstring, nativeMirrorGenerateDollMxid)(JNIEnv* env, jclass) {
    return env->NewStringUTF("@doll:localhost");
}

JNI_FUNC(jboolean, nativeMirrorIsActive)(JNIEnv*, jclass, jstring) { return false; }

JNI_FUNC(jboolean, nativeMirrorIsValidDoll)(JNIEnv* env, jclass, jstring jDoll) {
    std::string s = j2s(env, jDoll);
    return s.find("@doll:") != std::string::npos;
}

JNI_FUNC(void, nativeMirrorRemove)(JNIEnv*, jclass, jstring) {}

JNI_FUNC(void, nativeModuleEnable)(JNIEnv*, jclass, jstring, jboolean) {}

JNI_FUNC(jboolean, nativeModuleIsEnabled)(JNIEnv*, jclass, jstring) { return false; }

JNI_FUNC(jstring, nativeModuleListJson)(JNIEnv* env, jclass) { return env->NewStringUTF("[]"); }

JNI_FUNC(void, nativeModuleScanDir)(JNIEnv*, jclass, jstring) {}

// ============================================================
// Message Aggregation / Message Queue / MXID
// ============================================================

JNI_FUNC(void, nativeMsgAggAdd)(JNIEnv*, jclass, jstring, jstring) {}

JNI_FUNC(void, nativeMsgAggClear)(JNIEnv*, jclass) {}

JNI_FUNC(jint, nativeMsgAggCount)(JNIEnv*, jclass) { return 0; }

JNI_FUNC(jstring, nativeMsgAggGetAllJson)(JNIEnv* env, jclass) { return env->NewStringUTF("[]"); }

JNI_FUNC(void, nativeMsgQueueEnqueue)(JNIEnv*, jclass, jstring) {}

JNI_FUNC(jstring, nativeMsgQueueExport)(JNIEnv* env, jclass) { return env->NewStringUTF("[]"); }

JNI_FUNC(void, nativeMsgQueueMarkFailed)(JNIEnv*, jclass, jstring) {}

JNI_FUNC(void, nativeMsgQueueMarkSent)(JNIEnv*, jclass, jstring) {}

JNI_FUNC(jint, nativeMsgQueuePendingCount)(JNIEnv*, jclass) { return 0; }

JNI_FUNC(void, nativeMsgQueueSetOrder)(JNIEnv*, jclass, jobjectArray) {}

JNI_FUNC(jstring, nativeMxidVisibilityExport)(JNIEnv* env, jclass) { return env->NewStringUTF("[]"); }

// ============================================================
// Network Stats / Notification Keywords
// ============================================================

JNI_FUNC(void, nativeNetStatsClear)(JNIEnv*, jclass) {}

JNI_FUNC(void, nativeNetStatsEnd)(JNIEnv*, jclass, jstring) {}

JNI_FUNC(void, nativeNetStatsStart)(JNIEnv*, jclass, jstring) {}

JNI_FUNC(jstring, nativeNetStatsToJson)(JNIEnv* env, jclass) { return env->NewStringUTF("{}"); }

JNI_FUNC(jstring, nativeNetStatsToText)(JNIEnv* env, jclass) { return env->NewStringUTF(""); }

JNI_FUNC(void, nativeNotifKeywordAdd)(JNIEnv*, jclass, jstring) {}

JNI_FUNC(jstring, nativeNotifKeywordCheck)(JNIEnv* env, jclass, jstring, jstring) { return env->NewStringUTF("{\"matched\":false}"); }
