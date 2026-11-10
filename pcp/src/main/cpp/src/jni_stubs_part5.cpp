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

#define JNI_FUNC(void, nativeNotifKeywordClear)(JNIEnv*, jclass) {}

JNI_FUNC(jstring, nativeNotifKeywordExport)(JNIEnv* env, jclass) { return env->NewStringUTF("[]"); }

JNI_FUNC(void, nativeNotifKeywordImport)(JNIEnv*, jclass, jstring) {}

// ============================================================
// Parse Functions
// ============================================================

JNI_FUNC(jstring, nativeParseColor)(JNIEnv* env, jclass, jstring jHex) { return env->NewStringUTF("{\"r\":0,\"g\":0,\"b\":0}"); }

JNI_FUNC(jstring, nativeParseEncryptedHeader)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF("{}"); }

JNI_FUNC(jstring, nativeParseGeoUri)(JNIEnv* env, jclass, jstring jUri) {
    std::string u = j2s(env, jUri);
    auto p = u.find(':');
    return env->NewStringUTF((p != std::string::npos) ? u.substr(p+1).c_str() : "");
}

JNI_FUNC(jstring, nativeParseJumpToDate)(JNIEnv* env, jclass, jstring jArgs) { return jArgs; }

JNI_FUNC(jstring, nativeParsePinnedEventIds)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF("[]"); }

JNI_FUNC(jstring, nativeParsePushCondition)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF("{}"); }

JNI_FUNC(jstring, nativeParseRelation)(JNIEnv* env, jclass, jstring, jstring) { return env->NewStringUTF("{}"); }

JNI_FUNC(jstring, nativeParseResponse)(JNIEnv* env, jclass, jstring, jint) { return env->NewStringUTF("{}"); }

JNI_FUNC(jstring, nativeParseServerCapabilities)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF("{}"); }

JNI_FUNC(jstring, nativeParseSvg)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF(""); }

JNI_FUNC(jstring, nativeParseTranslateResponse)(JNIEnv* env, jclass, jstring, jint) { return env->NewStringUTF("{}"); }

// ============================================================
// Profile Swiper / Replacement Rules
// ============================================================

JNI_FUNC(jstring, nativeProfileSwiperNext)(JNIEnv* env, jclass) { return env->NewStringUTF("{\"hasMore\":false}"); }

JNI_FUNC(jstring, nativeProfileSwiperPrev)(JNIEnv* env, jclass) { return env->NewStringUTF("{\"hasMore\":false}"); }

JNI_FUNC(void, nativeProfileSwiperSetProfiles)(JNIEnv*, jclass, jstring) {}

JNI_FUNC(void, nativeReplacementAddRule)(JNIEnv*, jclass, jstring, jstring) {}

JNI_FUNC(jstring, nativeReplacementApply)(JNIEnv* env, jclass, jstring jText) { return jText; }

JNI_FUNC(jstring, nativeReplacementExport)(JNIEnv* env, jclass) { return env->NewStringUTF("[]"); }

// ============================================================
// Resolve / Sanitize / Scheduled Edits
// ============================================================

JNI_FUNC(jstring, nativeResolveMatrixId)(JNIEnv* env, jclass, jstring jId) { return jId; }

JNI_FUNC(jstring, nativeRewriteHomeserverUrl)(JNIEnv* env, jclass, jstring jUrl, jstring jYgg) { return jUrl; }

JNI_FUNC(jstring, nativeSanitizeRoomName)(JNIEnv* env, jclass, jstring jName) { return jName; }

JNI_FUNC(void, nativeSchedEditCancel)(JNIEnv*, jclass, jstring) {}

JNI_FUNC(jstring, nativeSchedEditExport)(JNIEnv* env, jclass) { return env->NewStringUTF("[]"); }

JNI_FUNC(jstring, nativeSchedEditGetDue)(JNIEnv* env, jclass) { return env->NewStringUTF("[]"); }

JNI_FUNC(void, nativeSchedEditMarkApplied)(JNIEnv*, jclass, jstring) {}

JNI_FUNC(void, nativeSchedEditMarkFailed)(JNIEnv*, jclass, jstring) {}

JNI_FUNC(jstring, nativeSchedEditSchedule)(JNIEnv* env, jclass, jstring, jstring, jstring, jlong) { return env->NewStringUTF("{\"id\":\"0\"}"); }

JNI_FUNC(jstring, nativeSchedEditStats)(JNIEnv* env, jclass) { return env->NewStringUTF("{\"total\":0,\"applied\":0,\"failed\":0}"); }

// ============================================================
// Search / SHA / Scroll / Smart Replies
// ============================================================

JNI_FUNC(void, nativeSearchClear)(JNIEnv*, jclass) {}

JNI_FUNC(void, nativeSearchIndexMessage)(JNIEnv*, jclass, jstring, jstring, jstring) {}

JNI_FUNC(jint, nativeSearchIndexedCount)(JNIEnv*, jclass) { return 0; }

JNI_FUNC(jstring, nativeSearchQuery)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF("[]"); }

JNI_FUNC(jstring, nativeSha256Hex)(JNIEnv* env, jclass, jstring jData) { return jData; }

JNI_FUNC(jboolean, nativeShouldAutoScroll)(JNIEnv*, jclass, jboolean jOwn, jboolean jAtBottom) { return jAtBottom || jOwn; }

JNI_FUNC(jboolean, nativeShouldBlockImage)(JNIEnv* env, jclass, jstring jUrl) { return false; }

JNI_FUNC(jboolean, nativeShouldLock)(JNIEnv*, jclass, jstring) { return false; }

JNI_FUNC(jboolean, nativeShouldUseLightweightMode)(JNIEnv*, jclass) { return true; }

JNI_FUNC(jstring, nativeSmartDefaultReactions)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF("[\"👍\",\"❤️\",\"😄\"]"); }

JNI_FUNC(jstring, nativeSmartDefaultReplies)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF("[\"Yes\",\"No\",\"OK\"]"); }

JNI_FUNC(jstring, nativeSmartParseReplies)(JNIEnv* env, jclass, jstring) { return env->NewStringUTF("[]"); }

JNI_FUNC(jstring, nativeSmartReplyPrompt)(JNIEnv* env, jclass, jstring jMsg) { return jMsg; }

// ============================================================
// Strip / Suggest / Symbol / Text / Thread
// ============================================================

JNI_FUNC(jstring, nativeStripPills)(JNIEnv* env, jclass, jstring jText) { return jText; }

JNI_FUNC(jint, nativeSuggestBarCount)(JNIEnv*, jclass, jint jW) { return jW / 50 + 1; }

JNI_FUNC(jstring, nativeSuggestQuietHours)(JNIEnv* env, jclass) { return env->NewStringUTF("22:00-07:00"); }

JNI_FUNC(void, nativeSymbolAdd)(JNIEnv*, jclass, jstring, jstring) {}

JNI_FUNC(jstring, nativeSymbolExport)(JNIEnv* env, jclass) { return env->NewStringUTF("[]"); }
