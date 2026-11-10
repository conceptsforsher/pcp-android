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

#define JNI_FUNC(jstring, nativeFormatReactionPreview)(JNIEnv* env, jclass, jstring jKey, jint jCnt) {
    std::ostringstream ss; ss << j2s(env, jKey) << " " << jCnt;
    return env->NewStringUTF(ss.str().c_str());
}

JNI_FUNC(jstring, nativeFormatRelativeTime)(JNIEnv* env, jclass, jlong jTs) {
    int64_t now = std::time(nullptr) * 1000;
    int64_t diff = now - jTs;
    if (diff < 60000) return env->NewStringUTF("just now");
    if (diff < 3600000) { std::ostringstream ss; ss << (diff/60000) << "m"; return env->NewStringUTF(ss.str().c_str()); }
    if (diff < 86400000) { std::ostringstream ss; ss << (diff/3600000) << "h"; return env->NewStringUTF(ss.str().c_str()); }
    std::ostringstream ss; ss << (diff/86400000) << "d";
    return env->NewStringUTF(ss.str().c_str());
}

JNI_FUNC(jstring, nativeFormatResolvedEvent)(JNIEnv* env, jclass, jstring jEvent) { return jEvent; }

JNI_FUNC(jstring, nativeFormatRoomSummary)(JNIEnv* env, jclass, jstring jName, jstring jTopic, jboolean, jint jMem, jboolean) {
    std::ostringstream ss; ss << j2s(env, jName) << " - " << j2s(env, jTopic) << " (" << jMem << " members)";
    return env->NewStringUTF(ss.str().c_str());
}

JNI_FUNC(jstring, nativeFormatSessionBadge)(JNIEnv* env, jclass, jint jCnt) {
    return env->NewStringUTF(jCnt > 99 ? "99+" : std::to_string(jCnt).c_str());
}

JNI_FUNC(jstring, nativeFormatShortTime)(JNIEnv* env, jclass, jlong jTs, jboolean j12h) {
    time_t t = jTs/1000; struct tm* tm = localtime(&t);
    char buf[16]; strftime(buf, sizeof(buf), j12h ? "%I:%M%p" : "%H:%M", tm);
    return env->NewStringUTF(buf);
}

JNI_FUNC(jstring, nativeFormatShrug)(JNIEnv* env, jclass) { return env->NewStringUTF("¯\\_(ツ)_/¯"); }

JNI_FUNC(jstring, nativeFormatSpoiler)(JNIEnv* env, jclass, jstring jText) {
    std::string s = "[spoiler] " + j2s(env, jText);
    return env->NewStringUTF(s.c_str());
}

JNI_FUNC(jstring, nativeFormatTableFlip)(JNIEnv* env, jclass) { return env->NewStringUTF("(╯°□°）╯︵ ┻━┻"); }

// ============================================================
// Generate / Getters / Validation
// ============================================================

JNI_FUNC(jstring, nativeGenerateRainbow)(JNIEnv* env, jclass, jstring jText) {
    std::string s = j2s(env, jText), out;
    const char* colors[] = {"#FF0000","#FF7F00","#FFFF00","#00FF00","#0000FF","#4B0082","#8F00FF"};
    for (size_t i = 0; i < s.size(); i++)
        out += "<font color=\"" + std::string(colors[i%7]) + "\">" + s[i] + "</font>";
    return env->NewStringUTF(out.c_str());
}

JNI_FUNC(jstring, nativeGenerateToken)(JNIEnv* env, jclass, jint jLen) {
    std::string s; s.reserve(jLen);
    const char* chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    for (int i = 0; i < jLen; i++) s += chars[rand() % 62];
    return env->NewStringUTF(s.c_str());
}

JNI_FUNC(jstring, nativeGetBadgeText)(JNIEnv* env, jclass, jint jCnt) {
    return env->NewStringUTF(jCnt > 99 ? "99+" : std::to_string(jCnt).c_str());
}

JNI_FUNC(jstring, nativeGetCommonTimezones)(JNIEnv* env, jclass) {
    return env->NewStringUTF("[\"UTC\",\"Europe/London\",\"Europe/Paris\",\"Europe/Berlin\",\"Europe/Moscow\",\"America/New_York\",\"America/Los_Angeles\",\"Asia/Tokyo\",\"Asia/Shanghai\",\"Australia/Sydney\"]");
}

JNI_FUNC(jstring, nativeGetExtensionFromName)(JNIEnv* env, jclass, jstring jName) {
    std::string s = j2s(env, jName);
    auto p = s.rfind('.');
    return env->NewStringUTF((p != std::string::npos) ? s.substr(p+1).c_str() : "");
}

JNI_FUNC(jstring, nativeGetFileExtension)(JNIEnv* env, jclass, jstring jName) {
    std::string s = j2s(env, jName);
    auto p = s.rfind('.');
    return env->NewStringUTF((p != std::string::npos) ? s.substr(p+1).c_str() : "");
}

JNI_FUNC(jstring, nativeGetLanguageLabel)(JNIEnv* env, jclass, jstring jCode) {
    return jCode; // Return code as label
}

JNI_FUNC(jstring, nativeGetMemoryInfo)(JNIEnv* env, jclass) {
    return env->NewStringUTF("{\"available\":0,\"total\":0,\"used\":0}");
}

JNI_FUNC(jstring, nativeGetQuickReactions)(JNIEnv* env, jclass) {
    return env->NewStringUTF("[\"❤️\",\"👍\",\"👎\",\"😄\",\"😢\",\"😮\",\"😡\",\"🎉\"]");
}

JNI_FUNC(jstring, nativeGetRecommendedMediaQuality)(JNIEnv* env, jclass, jint, jboolean, jboolean, jint, jdouble, jdouble) {
    return env->NewStringUTF("medium");
}

JNI_FUNC(jstring, nativeGetRoomVersionsJson)(JNIEnv* env, jclass) {
    return env->NewStringUTF("[\"1\",\"2\",\"3\",\"4\",\"5\",\"6\",\"7\",\"8\",\"9\",\"10\",\"11\"]");
}

JNI_FUNC(jstring, nativeGetStrippableMimeTypes)(JNIEnv* env, jclass) {
    return env->NewStringUTF("[\"application/octet-stream\",\"text/plain\"]");
}

JNI_FUNC(jstring, nativeGetSuggestedMasqueradeNames)(JNIEnv* env, jclass) {
    return env->NewStringUTF("[\"Element\",\"FluffyChat\",\"Nheko\",\"SchildiChat\",\"Cinny\",\"Hydrogen\"]");
}

JNI_FUNC(jstring, nativeGetSuggestedRole)(JNIEnv* env, jclass, jint jPl) {
    if (jPl >= 100) return env->NewStringUTF("Admin");
    if (jPl >= 50) return env->NewStringUTF("Moderator");
    return env->NewStringUTF("User");
}

JNI_FUNC(jstring, nativeGetTrustBadge)(JNIEnv* env, jclass, jstring, jboolean, jboolean) {
    return env->NewStringUTF("unknown");
}

JNI_FUNC(jstring, nativeGetVerificationEmojis)(JNIEnv* env, jclass, jstring) {
    return env->NewStringUTF("[\"🐶\",\"🐱\",\"🦁\",\"🐮\",\"🐷\",\"🐸\",\"🐵\"]");
}

// ============================================================
// Haversine / Invite
// ============================================================

JNI_FUNC(jdouble, nativeHaversine)(JNIEnv*, jclass, jdouble jLat1, jdouble jLon1, jdouble jLat2, jdouble jLon2) {
    double dlat = (jLat2 - jLat1) * 3.14159 / 180.0;
    double dlon = (jLon2 - jLon1) * 3.14159 / 180.0;
    double a = sin(dlat/2)*sin(dlat/2) + cos(jLat1*3.14159/180)*cos(jLat2*3.14159/180)*sin(dlon/2)*sin(dlon/2);
    return 6371000 * 2 * atan2(sqrt(a), sqrt(1-a));
}

JNI_FUNC(void, nativeInviteClear)(JNIEnv*, jclass) {}

JNI_FUNC(jint, nativeInviteCount)(JNIEnv*, jclass) { return 0; }

JNI_FUNC(jstring, nativeInviteExportJson)(JNIEnv* env, jclass) { return env->NewStringUTF("[]"); }

JNI_FUNC(void, nativeInviteHide)(JNIEnv*, jclass, jstring) {}

JNI_FUNC(void, nativeInviteImportJson)(JNIEnv*, jclass, jstring) {}

JNI_FUNC(jboolean, nativeInviteIsHidden)(JNIEnv*, jclass, jstring) { return false; }

JNI_FUNC(void, nativeInviteUnhide)(JNIEnv*, jclass, jstring) {}

// ============================================================
// Validation Functions
// ============================================================

JNI_FUNC(jboolean, nativeIsCallEvent)(JNIEnv* env, jclass, jstring jType) {
    std::string t = j2s(env, jType);
    return t == "m.call.invite" || t == "m.call.answer" || t == "m.call.hangup" || t == "m.call.candidates";
}

JNI_FUNC(jboolean, nativeIsEmojiOnly)(JNIEnv* env, jclass, jstring jText) {
    std::string s = j2s(env, jText);
    for (char c : s) if (c > 32 && c != ' ') return false;
    return !s.empty();
}

JNI_FUNC(jboolean, nativeIsEmojiOnlyMessage)(JNIEnv* env, jclass, jstring jText) {
    std::string s = j2s(env, jText);
    for (char c : s) if (c > 32 && c != ' ') return false;
    return !s.empty();
}

JNI_FUNC(jboolean, nativeIsLikelyFullHistory)(JNIEnv*, jclass, jint jEvents, jboolean) { return jEvents > 0; }

JNI_FUNC(jboolean, nativeIsRoomEncrypted)(JNIEnv*, jclass, jstring) { return false; }

JNI_FUNC(jboolean, nativeIsRoomMention)(JNIEnv* env, jclass, jstring jBody, jstring jRoom) {
    return j2s(env, jBody).find(j2s(env, jRoom)) != std::string::npos;
}

JNI_FUNC(jboolean, nativeIsSupportedAudio)(JNIEnv* env, jclass, jstring jMime) {
    std::string m = j2s(env, jMime);
    return m.find("audio/") == 0 || m.find("video/") == 0;
}

JNI_FUNC(jboolean, nativeIsValidBlurhash)(JNIEnv* env, jclass, jstring jBh) { return jBh && env->GetStringLength(jBh) > 0; }

JNI_FUNC(jboolean, nativeIsValidCrop)(JNIEnv*, jclass, jint, jint, jint, jint, jint, jint) { return true; }

JNI_FUNC(jboolean, nativeIsValidIconAlias)(JNIEnv* env, jclass, jstring jAlias) {
    std::string s = j2s(env, jAlias);
    return s.size() > 0 && s[0] == '#';
}

JNI_FUNC(jboolean, nativeIsValidMasqueradeName)(JNIEnv* env, jclass, jstring jName) {
    return jName && env->GetStringLength(jName) > 0 && env->GetStringLength(jName) <= 64;
}

JNI_FUNC(jboolean, nativeIsValidMxcUri)(JNIEnv* env, jclass, jstring jUri) {
    std::string s = j2s(env, jUri);
    return s.find("mxc://") == 0;
}

JNI_FUNC(jboolean, nativeIsValidMxid)(JNIEnv* env, jclass, jstring jId) {
    std::string s = j2s(env, jId);
    return s.size() > 0 && s[0] == '@' && s.find(':') != std::string::npos;
}

JNI_FUNC(jboolean, nativeIsValidPin)(JNIEnv* env, jclass, jstring jPin) {
    jsize l = env->GetStringLength(jPin);
    return l >= 4 && l <= 8;
}

JNI_FUNC(jboolean, nativeIsValidRecoveryKey)(JNIEnv* env, jclass, jstring jKey) { return jKey && env->GetStringLength(jKey) > 0; }
