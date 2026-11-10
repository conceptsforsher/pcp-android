#include <jni.h>
#include <string>
#include <map>
#include <memory>
#include <mutex>
#include "pcp/reddit_client.hpp"

using namespace pcp::reddit;

// Global JVM and callback references
static JavaVM* gJvm = nullptr;
static jobject gCallbackObj = nullptr;
static jmethodID gOnPostsMethod = nullptr;
static jmethodID gOnCommentsMethod = nullptr;
static jmethodID gOnSubredditsMethod = nullptr;
static jmethodID gOnErrorMethod = nullptr;

static std::mutex gCallbackMutex;

// Convert jstring to std::string
static std::string jstr(JNIEnv* env, jstring js) {
    if (!js) return "";
    const char* s = env->GetStringUTFChars(js, nullptr);
    std::string result(s);
    env->ReleaseStringUTFChars(js, s);
    return result;
}

// Convert std::string to jstring
static jstring sjstr(JNIEnv* env, const std::string& s) {
    return env->NewStringUTF(s.c_str());
}

// ==== URL builder JNI methods ====

extern "C" JNIEXPORT jstring JNICALL
Java_chat_pcp_app_reddit_RedditNative_nativeFrontpageUrl(
    JNIEnv* env, jclass, jstring sort, jstring after, jint limit) {
    SortOrder s = SortOrder::Hot;
    std::string sortStr = jstr(env, sort);
    if (sortStr == "new") s = SortOrder::New;
    else if (sortStr == "top") s = SortOrder::Top;
    else if (sortStr == "rising") s = SortOrder::Rising;
    else if (sortStr == "controversial") s = SortOrder::Controversial;
    return sjstr(env, RedditClient::frontpageUrl(s, jstr(env, after), limit));
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_pcp_app_reddit_RedditNative_nativeSubredditUrl(
    JNIEnv* env, jclass, jstring sub, jstring sort, jstring after) {
    SortOrder s = SortOrder::Hot;
    std::string sortStr = jstr(env, sort);
    if (sortStr == "new") s = SortOrder::New;
    else if (sortStr == "top") s = SortOrder::Top;
    else if (sortStr == "rising") s = SortOrder::Rising;
    else if (sortStr == "controversial") s = SortOrder::Controversial;
    return sjstr(env, RedditClient::subredditUrl(jstr(env, sub), s, jstr(env, after)));
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_pcp_app_reddit_RedditNative_nativeCommentsUrl(
    JNIEnv* env, jclass, jstring sub, jstring postId) {
    return sjstr(env, RedditClient::commentsUrl(jstr(env, sub), jstr(env, postId)));
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_pcp_app_reddit_RedditNative_nativeSearchSubredditsUrl(
    JNIEnv* env, jclass, jstring query) {
    return sjstr(env, RedditClient::subredditSearchUrl(jstr(env, query)));
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_pcp_app_reddit_RedditNative_nativeSubredditAboutUrl(
    JNIEnv* env, jclass, jstring sub) {
    return sjstr(env, RedditClient::subredditAboutUrl(jstr(env, sub)));
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_pcp_app_reddit_RedditNative_nativeUserPostsUrl(
    JNIEnv* env, jclass, jstring user, jstring sort) {
    SortOrder s = SortOrder::New;
    std::string sortStr = jstr(env, sort);
    if (sortStr == "top") s = SortOrder::Top;
    else if (sortStr == "hot") s = SortOrder::Hot;
    return sjstr(env, RedditClient::userPostsUrl(jstr(env, user), s));
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_pcp_app_reddit_RedditNative_nativeVoteUrl(JNIEnv* env, jclass) {
    return sjstr(env, RedditClient::voteUrl());
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_pcp_app_reddit_RedditNative_nativeSaveUrl(JNIEnv* env, jclass, jboolean saveFlag) {
    return sjstr(env, RedditClient::saveUrl(saveFlag));
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_pcp_app_reddit_RedditNative_nativeHideUrl(JNIEnv* env, jclass) {
    return sjstr(env, RedditClient::hideUrl());
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_pcp_app_reddit_RedditNative_nativeCommentUrl(JNIEnv* env, jclass) {
    return sjstr(env, RedditClient::submitCommentUrl());
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_pcp_app_reddit_RedditNative_nativeBuildVoteBody(
    JNIEnv* env, jclass, jstring id, jint dir) {
    return sjstr(env, RedditClient::buildOAuthBody("vote", jstr(env, id), "dir", std::to_string(dir)));
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_pcp_app_reddit_RedditNative_nativeBuildSaveBody(
    JNIEnv* env, jclass, jstring id) {
    return sjstr(env, RedditClient::buildOAuthBody("save", jstr(env, id)));
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_pcp_app_reddit_RedditNative_nativeBuildHideBody(
    JNIEnv* env, jclass, jstring id) {
    return sjstr(env, RedditClient::buildOAuthBody("hide", jstr(env, id)));
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_pcp_app_reddit_RedditNative_nativeBuildCommentBody(
    JNIEnv* env, jclass, jstring parentId, jstring text) {
    return sjstr(env, RedditClient::buildOAuthBody("comment", jstr(env, parentId), "text", jstr(env, text)));
}

// ==== Data parsing JNI methods ====

extern "C" JNIEXPORT jstring JNICALL
Java_chat_pcp_app_reddit_RedditNative_nativeParsePosts(
    JNIEnv* env, jclass, jstring rawJson, jobject afterHolder) {
    std::string after;
    std::string result = RedditClient::parsePosts(jstr(env, rawJson), after);

    // Set after into the StringHolder
    if (afterHolder) {
        jclass cls = env->GetObjectClass(afterHolder);
        jfieldID fid = env->GetFieldID(cls, "value", "Ljava/lang/String;");
        env->SetObjectField(afterHolder, fid, sjstr(env, after));
        env->DeleteLocalRef(cls);
    }

    return sjstr(env, result);
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_pcp_app_reddit_RedditNative_nativeParseComments(
    JNIEnv* env, jclass, jstring rawJson) {
    return sjstr(env, RedditClient::parseComments(jstr(env, rawJson)));
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_pcp_app_reddit_RedditNative_nativeParseSubreddits(
    JNIEnv* env, jclass, jstring rawJson) {
    return sjstr(env, RedditClient::parseSubreddits(jstr(env, rawJson)));
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_pcp_app_reddit_RedditNative_nativeParseSubredditInfo(
    JNIEnv* env, jclass, jstring rawJson) {
    return sjstr(env, RedditClient::parseSubredditInfo(jstr(env, rawJson)));
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_pcp_app_reddit_RedditNative_nativeTokenUrl(JNIEnv* env, jclass) {
    return sjstr(env, RedditClient::tokenUrl());
}

// JNI_OnLoad
extern "C" JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void*) {
    gJvm = vm;
    return JNI_VERSION_1_6;
}
