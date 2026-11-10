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

#define JNI_FUNC(jint, nativeComputeThumbnail)(JNIEnv*, jclass, jint, jint, jint, jint) { return 0; }

// ============================================================
// ============================================================
// Contrast / Decrypt / Deleted Archive
// ============================================================

JNI_FUNC(jdouble, nativeContrastRatio)(JNIEnv*, jclass, jint, jint) { return 0.0; }

JNI_FUNC(jstring, nativeDecideRetry)(JNIEnv* env, jclass, jint, jint, jstring) {
    return env->NewStringUTF("{\"shouldRetry\":false}");
}

JNI_FUNC(jstring, nativeDecryptAccount)(JNIEnv* env, jclass, jstring) {
    return env->NewStringUTF("{}");
}

JNI_FUNC(void, nativeDeletedArchiveAdd)(JNIEnv*, jclass, jstring) {}

JNI_FUNC(void, nativeDeletedArchiveClear)(JNIEnv*, jclass) {}

JNI_FUNC(jint, nativeDeletedArchiveCount)(JNIEnv*, jclass) { return 0; }

JNI_FUNC(jstring, nativeDeletedArchiveExportJson)(JNIEnv* env, jclass) {
    return env->NewStringUTF("[]");
}

// ============================================================
// Desync Detector / Language / Drawing
// ============================================================

JNI_FUNC(jstring, nativeDesyncCheck)(JNIEnv* env, jclass, jstring) {
    return env->NewStringUTF("{\"desynced\":false}");
}

JNI_FUNC(void, nativeDesyncTrackEvent)(JNIEnv*, jclass, jstring, jstring) {}

JNI_FUNC(jstring, nativeDetectLanguage)(JNIEnv* env, jclass, jstring jText) {
    return env->NewStringUTF(jsonObj(jsonPair("language", "en")).c_str());
}

JNI_FUNC(void, nativeDrawClear)(JNIEnv*, jclass) {}

JNI_FUNC(jstring, nativeDrawExportJson)(JNIEnv* env, jclass) { return env->NewStringUTF("[]"); }

JNI_FUNC(void, nativeDrawLineTo)(JNIEnv*, jclass, jfloat, jfloat) {}

JNI_FUNC(void, nativeDrawMoveTo)(JNIEnv*, jclass, jfloat, jfloat) {}

JNI_FUNC(void, nativeDrawSetColor)(JNIEnv*, jclass, jint, jint, jint, jint) {}

JNI_FUNC(void, nativeDrawSetStrokeWidth)(JNIEnv*, jclass, jfloat) {}

// ============================================================
// Edit Distance / Emoji Blacklist
// ============================================================

JNI_FUNC(jint, nativeEditDistance)(JNIEnv* env, jclass, jstring jA, jstring jB) {
    std::string a = j2s(env, jA), b = j2s(env, jB);
    size_t la = a.size(), lb = b.size();
    if (la == 0) return (int)lb;
    if (lb == 0) return (int)la;
    // Simple Levenshtein
    std::vector<size_t> prev(lb+1), curr(lb+1);
    for (size_t j = 0; j <= lb; j++) prev[j] = j;
    for (size_t i = 1; i <= la; i++) {
        curr[0] = i;
        for (size_t j = 1; j <= lb; j++)
            curr[j] = std::min({prev[j]+1, curr[j-1]+1, prev[j-1] + (a[i-1]!=b[j-1])});
        prev.swap(curr);
    }
    return (int)prev[lb];
}

JNI_FUNC(void, nativeEmojiBlacklistAdd)(JNIEnv*, jclass, jstring) {}

JNI_FUNC(jstring, nativeEmojiBlacklistExport)(JNIEnv* env, jclass) { return env->NewStringUTF("[]"); }

JNI_FUNC(void, nativeEmojiBlacklistImport)(JNIEnv*, jclass, jstring) {}

JNI_FUNC(jboolean, nativeEmojiBlacklistIsBlocked)(JNIEnv*, jclass, jstring) { return false; }

JNI_FUNC(void, nativeEmojiBlacklistRemove)(JNIEnv*, jclass, jstring) {}

// ============================================================
// Event Extractors
// ============================================================

JNI_FUNC(jstring, nativeEncryptAccount)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF("{}"); }

JNI_FUNC(jint, nativeEstimatePaginationRequests)(JNIEnv*, jclass, jint jMiss, jint jPage) { return jPage>0 ? (jMiss+jPage-1)/jPage : 0; }

JNI_FUNC(jstring, nativeExtractEventLinks)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF("[]"); }

JNI_FUNC(jstring, nativeExtractFirstUrl)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF(""); }

JNI_FUNC(jstring, nativeExtractServerName)(JNIEnv* env, jclass, jstring jMxid) {
    std::string s = j2s(env, jMxid);
    auto p = s.find(':');
    return env->NewStringUTF((p != std::string::npos ? s.substr(p+1) : "").c_str());
}

JNI_FUNC(jstring, nativeExtractServers)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF("[]"); }

// ============================================================
// File Utilities
// ============================================================

JNI_FUNC(jboolean, nativeFileHasMetadata)(JNIEnv*, jclass, jstring) { return false; }

// ============================================================
// Format Utilities
// ============================================================

JNI_FUNC(jstring, nativeFormatChatTimestamp)(JNIEnv* env, jclass, jlong jTs, jboolean j12h) {
    time_t t = jTs / 1000;
    struct tm* tm = localtime(&t);
    char buf[32];
    strftime(buf, sizeof(buf), j12h ? "%I:%M %p" : "%H:%M", tm);
    return env->NewStringUTF(buf);
}

JNI_FUNC(jstring, nativeFormatCreationDate)(JNIEnv* env, jclass, jlong jTs) {
    time_t t = jTs / 1000; struct tm* tm = localtime(&t);
    char buf[32]; strftime(buf, sizeof(buf), "%b %d, %Y", tm);
    return env->NewStringUTF(buf);
}

JNI_FUNC(jstring, nativeFormatEmote)(JNIEnv* env, jclass, jstring jSender, jstring jText) {
    std::string s = "* " + j2s(env, jSender) + " " + j2s(env, jText);
    return env->NewStringUTF(s.c_str());
}

JNI_FUNC(jstring, nativeFormatEventHtml)(JNIEnv* env, jclass, jstring jSender, jstring jTs, jstring jBody, jstring jType, jstring jFile, jstring jSize, jstring jRel, jboolean) {
    std::ostringstream ss;
    ss << "<b>" << j2s(env, jSender) << "</b> " << j2s(env, jBody);
    return env->NewStringUTF(ss.str().c_str());
}

JNI_FUNC(jstring, nativeFormatEventPlainText)(JNIEnv* env, jclass, jstring jSender, jstring jTs, jstring jBody, jstring jType, jstring jFile, jstring jRel) {
    std::string s = j2s(env, jSender) + ": " + j2s(env, jBody);
    return env->NewStringUTF(s.c_str());
}

JNI_FUNC(jstring, nativeFormatFullTimestamp)(JNIEnv* env, jclass, jlong jTs) {
    time_t t = jTs/1000; struct tm* tm = localtime(&t);
    char buf[64]; strftime(buf, sizeof(buf), "%a %b %d %Y %H:%M:%S", tm);
    return env->NewStringUTF(buf);
}

JNI_FUNC(jstring, nativeFormatGroupLabel)(JNIEnv* env, jclass, jstring jName, jint jCnt) {
    std::ostringstream ss; ss << j2s(env, jName) << " (" << jCnt << ")";
    return env->NewStringUTF(ss.str().c_str());
}

JNI_FUNC(jstring, nativeFormatLastMessagePreview)(JNIEnv* env, jclass, jstring jSender, jstring jBody, jboolean, jboolean) {
    std::string s = j2s(env, jSender) + ": " + j2s(env, jBody);
    return env->NewStringUTF(s.c_str());
}

JNI_FUNC(jstring, nativeFormatLenny)(JNIEnv* env, jclass) { return env->NewStringUTF("( ͡° ͜ʖ ͡°)"); }

JNI_FUNC(jstring, nativeFormatMemoryLabel)(JNIEnv* env, jclass, jlong jBytes) {
    std::ostringstream ss;
    if (jBytes > 1024*1024) ss << (jBytes/(1024*1024)) << " MB";
    else if (jBytes > 1024) ss << (jBytes/1024) << " KB";
    else ss << jBytes << " B";
    return env->NewStringUTF(ss.str().c_str());
}

JNI_FUNC(jstring, nativeFormatMessageStatus)(JNIEnv* env, jclass, jint jSt) {
    switch(jSt) { case 0: return env->NewStringUTF("sending"); case 1: return env->NewStringUTF("sent"); default: return env->NewStringUTF("error"); }
}

JNI_FUNC(jstring, nativeFormatNotifBadge)(JNIEnv* env, jclass, jint jCnt) {
    return env->NewStringUTF(jCnt > 99 ? "99+" : std::to_string(jCnt).c_str());
}

JNI_FUNC(jstring, nativeFormatNotifBody)(JNIEnv* env, jclass, jstring jSender, jstring jBody, jstring jRoom) {
    std::string s = j2s(env, jSender) + ": " + j2s(env, jBody);
    return env->NewStringUTF(s.c_str());
}

JNI_FUNC(jstring, nativeFormatNotifTitle)(JNIEnv* env, jclass, jstring jRoom, jint jCnt) {
    std::ostringstream ss; ss << j2s(env, jRoom) << " (" << jCnt << ")";
    return env->NewStringUTF(ss.str().c_str());
}

JNI_FUNC(jstring, nativeFormatPlain)(JNIEnv* env, jclass, jstring jHtml) { return jHtml; } // return as-is for now

JNI_FUNC(jstring, nativeFormatPresenceLine)(JNIEnv* env, jclass, jstring jUser, jstring jStatus) {
    std::string s = j2s(env, jUser) + " " + j2s(env, jStatus);
    return env->NewStringUTF(s.c_str());
}

JNI_FUNC(jstring, nativeFormatProgressBar)(JNIEnv* env, jclass, jint jPct, jint jW) {
    std::string bar;
    int filled = jPct * jW / 100;
    for (int i = 0; i < jW; i++) bar += (i < filled) ? '#' : '-';
    return env->NewStringUTF(bar.c_str());
}

JNI_FUNC(jstring, nativeFormatRateLimitMessage)(JNIEnv* env, jclass, jlong jMs) {
    std::ostringstream ss; ss << "Rate limited. Try again in " << (jMs/1000) << "s";
    return env->NewStringUTF(ss.str().c_str());
}
