#include <jni.h>
#include <string>
#include "progressive/lemmy_api.hpp"

using namespace progressive;

static std::string jstringToString(JNIEnv* env, jstring str) {
    if (!str) return "";
    const char* utf = env->GetStringUTFChars(str, nullptr);
    std::string result(utf);
    env->ReleaseStringUTFChars(str, utf);
    return result;
}

// ==== Instance ====

extern "C" JNIEXPORT void JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmySetInstance(JNIEnv* env, jclass, jstring url) {
    setLemmyInstance(jstringToString(env, url));
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmyGetInstance(JNIEnv* env, jclass) {
    return env->NewStringUTF(getLemmyInstance().c_str());
}

// ==== Site ====

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmyGetSite(JNIEnv* env, jclass, jstring jAuth) {
    auto r = lemmyGetSite(jstringToString(env, jAuth));
    return env->NewStringUTF(r.c_str());
}

// ==== Auth ====

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmyLogin(JNIEnv* env, jclass,
        jstring jUser, jstring jPass, jstring jTotp) {
    auto r = lemmyLogin(jstringToString(env, jUser), jstringToString(env, jPass),
                        jstringToString(env, jTotp));
    return env->NewStringUTF(r.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmyRegister(JNIEnv* env, jclass,
        jstring jUser, jstring jPass, jstring jEmail,
        jstring jCaptchaUuid, jstring jCaptchaAnswer, jstring jAnswer, jboolean jShowNsfw) {
    auto r = lemmyRegister(jstringToString(env, jUser), jstringToString(env, jPass),
                           jstringToString(env, jEmail), jstringToString(env, jCaptchaUuid),
                           jstringToString(env, jCaptchaAnswer), jstringToString(env, jAnswer),
                           jShowNsfw);
    return env->NewStringUTF(r.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmyGetCaptcha(JNIEnv* env, jclass, jstring jAuth) {
    auto r = lemmyGetCaptcha(jstringToString(env, jAuth));
    return env->NewStringUTF(r.c_str());
}

// ==== Community ====

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmyListCommunities(JNIEnv* env, jclass,
        jstring jAuth, jstring jType, jstring jSort, jint jPage, jint jLimit, jboolean jShowNsfw) {
    auto r = lemmyListCommunities(jstringToString(env, jAuth), jstringToString(env, jType),
                                   jstringToString(env, jSort), jPage, jLimit, jShowNsfw);
    return env->NewStringUTF(r.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmyGetCommunity(JNIEnv* env, jclass,
        jstring jAuth, jint jId, jstring jName) {
    auto r = lemmyGetCommunity(jstringToString(env, jAuth), jId, jstringToString(env, jName));
    return env->NewStringUTF(r.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmyFollowCommunity(JNIEnv* env, jclass,
        jstring jAuth, jint jCommunityId, jboolean jFollow) {
    auto r = lemmyFollowCommunity(jstringToString(env, jAuth), jCommunityId, jFollow);
    return env->NewStringUTF(r.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmyBlockCommunity(JNIEnv* env, jclass,
        jstring jAuth, jint jCommunityId, jboolean jBlock) {
    auto r = lemmyBlockCommunity(jstringToString(env, jAuth), jCommunityId, jBlock);
    return env->NewStringUTF(r.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmyHideCommunity(JNIEnv* env, jclass,
        jstring jAuth, jint jCommunityId, jboolean jHide, jstring jReason) {
    auto r = lemmyHideCommunity(jstringToString(env, jAuth), jCommunityId, jHide,
                                jstringToString(env, jReason));
    return env->NewStringUTF(r.c_str());
}

// ==== Post ====

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmyListPosts(JNIEnv* env, jclass,
        jstring jAuth, jstring jType, jstring jSort, jint jPage, jint jLimit,
        jint jCommunityId, jstring jCommunityName,
        jboolean jSavedOnly, jboolean jLikedOnly, jboolean jDislikedOnly, jboolean jShowNsfw) {
    auto r = lemmyListPosts(jstringToString(env, jAuth), jstringToString(env, jType),
                             jstringToString(env, jSort), jPage, jLimit,
                             jCommunityId, jstringToString(env, jCommunityName),
                             jSavedOnly, jLikedOnly, jDislikedOnly, jShowNsfw);
    return env->NewStringUTF(r.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmyGetPost(JNIEnv* env, jclass,
        jstring jAuth, jint jPostId) {
    auto r = lemmyGetPost(jstringToString(env, jAuth), jPostId);
    return env->NewStringUTF(r.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmyCreatePost(JNIEnv* env, jclass,
        jstring jAuth, jint jCommunityId, jstring jName,
        jstring jBody, jstring jUrl, jboolean jNsfw, jint jLanguageId) {
    auto r = lemmyCreatePost(jstringToString(env, jAuth), jCommunityId,
                              jstringToString(env, jName), jstringToString(env, jBody),
                              jstringToString(env, jUrl), jNsfw, jLanguageId);
    return env->NewStringUTF(r.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmyEditPost(JNIEnv* env, jclass,
        jstring jAuth, jint jPostId, jstring jName,
        jstring jBody, jstring jUrl, jboolean jNsfw, jint jLanguageId) {
    auto r = lemmyEditPost(jstringToString(env, jAuth), jPostId,
                            jstringToString(env, jName), jstringToString(env, jBody),
                            jstringToString(env, jUrl), jNsfw, jLanguageId);
    return env->NewStringUTF(r.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmyDeletePost(JNIEnv* env, jclass,
        jstring jAuth, jint jPostId, jboolean jDeleted) {
    auto r = lemmyDeletePost(jstringToString(env, jAuth), jPostId, jDeleted);
    return env->NewStringUTF(r.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmyRemovePost(JNIEnv* env, jclass,
        jstring jAuth, jint jPostId, jboolean jRemoved, jstring jReason) {
    auto r = lemmyRemovePost(jstringToString(env, jAuth), jPostId, jRemoved,
                              jstringToString(env, jReason));
    return env->NewStringUTF(r.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmyLikePost(JNIEnv* env, jclass,
        jstring jAuth, jint jPostId, jint jScore) {
    auto r = lemmyLikePost(jstringToString(env, jAuth), jPostId, jScore);
    return env->NewStringUTF(r.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmySavePost(JNIEnv* env, jclass,
        jstring jAuth, jint jPostId, jboolean jSave) {
    auto r = lemmySavePost(jstringToString(env, jAuth), jPostId, jSave);
    return env->NewStringUTF(r.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmyReportPost(JNIEnv* env, jclass,
        jstring jAuth, jint jPostId, jstring jReason) {
    auto r = lemmyReportPost(jstringToString(env, jAuth), jPostId,
                              jstringToString(env, jReason));
    return env->NewStringUTF(r.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmyLockPost(JNIEnv* env, jclass,
        jstring jAuth, jint jPostId, jboolean jLocked) {
    auto r = lemmyLockPost(jstringToString(env, jAuth), jPostId, jLocked);
    return env->NewStringUTF(r.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmyFeaturePost(JNIEnv* env, jclass,
        jstring jAuth, jint jPostId, jboolean jFeatured, jstring jFeatureType) {
    auto r = lemmyFeaturePost(jstringToString(env, jAuth), jPostId, jFeatured,
                               jstringToString(env, jFeatureType));
    return env->NewStringUTF(r.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmyMarkPostRead(JNIEnv* env, jclass,
        jstring jAuth, jint jPostId, jboolean jRead) {
    auto r = lemmyMarkPostRead(jstringToString(env, jAuth), jPostId, jRead);
    return env->NewStringUTF(r.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmyGetSiteMetadata(JNIEnv* env, jclass,
        jstring jAuth, jstring jUrl) {
    auto r = lemmyGetSiteMetadata(jstringToString(env, jAuth), jstringToString(env, jUrl));
    return env->NewStringUTF(r.c_str());
}

// ==== Comment ====

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmyListComments(JNIEnv* env, jclass,
        jstring jAuth, jint jPostId, jstring jSort, jint jMaxDepth, jint jLimit, jint jPage,
        jboolean jSavedOnly, jboolean jLikedOnly, jboolean jDislikedOnly) {
    auto r = lemmyListComments(jstringToString(env, jAuth), jPostId,
                                jstringToString(env, jSort), jMaxDepth, jLimit, jPage,
                                jSavedOnly, jLikedOnly, jDislikedOnly);
    return env->NewStringUTF(r.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmyGetComment(JNIEnv* env, jclass,
        jstring jAuth, jint jCommentId) {
    auto r = lemmyGetComment(jstringToString(env, jAuth), jCommentId);
    return env->NewStringUTF(r.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmyCreateComment(JNIEnv* env, jclass,
        jstring jAuth, jint jPostId, jstring jContent, jint jParentId, jint jLanguageId) {
    auto r = lemmyCreateComment(jstringToString(env, jAuth), jPostId,
                                 jstringToString(env, jContent), jParentId, jLanguageId);
    return env->NewStringUTF(r.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmyEditComment(JNIEnv* env, jclass,
        jstring jAuth, jint jCommentId, jstring jContent, jint jLanguageId) {
    auto r = lemmyEditComment(jstringToString(env, jAuth), jCommentId,
                               jstringToString(env, jContent), jLanguageId);
    return env->NewStringUTF(r.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmyDeleteComment(JNIEnv* env, jclass,
        jstring jAuth, jint jCommentId, jboolean jDeleted) {
    auto r = lemmyDeleteComment(jstringToString(env, jAuth), jCommentId, jDeleted);
    return env->NewStringUTF(r.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmyRemoveComment(JNIEnv* env, jclass,
        jstring jAuth, jint jCommentId, jboolean jRemoved, jstring jReason) {
    auto r = lemmyRemoveComment(jstringToString(env, jAuth), jCommentId, jRemoved,
                                 jstringToString(env, jReason));
    return env->NewStringUTF(r.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmyLikeComment(JNIEnv* env, jclass,
        jstring jAuth, jint jCommentId, jint jScore) {
    auto r = lemmyLikeComment(jstringToString(env, jAuth), jCommentId, jScore);
    return env->NewStringUTF(r.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmySaveComment(JNIEnv* env, jclass,
        jstring jAuth, jint jCommentId, jboolean jSave) {
    auto r = lemmySaveComment(jstringToString(env, jAuth), jCommentId, jSave);
    return env->NewStringUTF(r.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmyReportComment(JNIEnv* env, jclass,
        jstring jAuth, jint jCommentId, jstring jReason) {
    auto r = lemmyReportComment(jstringToString(env, jAuth), jCommentId,
                                 jstringToString(env, jReason));
    return env->NewStringUTF(r.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmyDistinguishComment(JNIEnv* env, jclass,
        jstring jAuth, jint jCommentId, jboolean jDistinguished) {
    auto r = lemmyDistinguishComment(jstringToString(env, jAuth), jCommentId, jDistinguished);
    return env->NewStringUTF(r.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmyMarkCommentRead(JNIEnv* env, jclass,
        jstring jAuth, jint jCommentId, jboolean jRead) {
    auto r = lemmyMarkCommentRead(jstringToString(env, jAuth), jCommentId, jRead);
    return env->NewStringUTF(r.c_str());
}

// ==== User ====

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmyGetUser(JNIEnv* env, jclass,
        jstring jAuth, jstring jUsername, jint jPersonId, jstring jSort,
        jint jPage, jint jLimit, jint jCommunityId, jboolean jSavedOnly) {
    auto r = lemmyGetUser(jstringToString(env, jAuth), jstringToString(env, jUsername),
                           jPersonId, jstringToString(env, jSort), jPage, jLimit,
                           jCommunityId, jSavedOnly);
    return env->NewStringUTF(r.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmyBlockUser(JNIEnv* env, jclass,
        jstring jAuth, jint jPersonId, jboolean jBlock) {
    auto r = lemmyBlockUser(jstringToString(env, jAuth), jPersonId, jBlock);
    return env->NewStringUTF(r.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmyGetMentions(JNIEnv* env, jclass,
        jstring jAuth, jstring jSort, jint jPage, jint jLimit, jboolean jUnreadOnly) {
    auto r = lemmyGetUserMentions(jstringToString(env, jAuth), jstringToString(env, jSort),
                                   jPage, jLimit, jUnreadOnly);
    return env->NewStringUTF(r.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmyMarkMentionRead(JNIEnv* env, jclass,
        jstring jAuth, jint jMentionId, jboolean jRead) {
    auto r = lemmyMarkMentionRead(jstringToString(env, jAuth), jMentionId, jRead);
    return env->NewStringUTF(r.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmyGetReplies(JNIEnv* env, jclass,
        jstring jAuth, jstring jSort, jint jPage, jint jLimit, jboolean jUnreadOnly) {
    auto r = lemmyGetUserReplies(jstringToString(env, jAuth), jstringToString(env, jSort),
                                  jPage, jLimit, jUnreadOnly);
    return env->NewStringUTF(r.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmyMarkAllAsRead(JNIEnv* env, jclass,
        jstring jAuth) {
    auto r = lemmyMarkAllAsRead(jstringToString(env, jAuth));
    return env->NewStringUTF(r.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmyGetUnreadCount(JNIEnv* env, jclass,
        jstring jAuth) {
    auto r = lemmyGetUnreadCount(jstringToString(env, jAuth));
    return env->NewStringUTF(r.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmySaveUserSettings(JNIEnv* env, jclass,
        jstring jAuth, jstring jSettingsJson) {
    auto r = lemmySaveUserSettings(jstringToString(env, jAuth),
                                    jstringToString(env, jSettingsJson));
    return env->NewStringUTF(r.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmyDeleteAccount(JNIEnv* env, jclass,
        jstring jAuth, jstring jPassword, jboolean jDeleteContent) {
    auto r = lemmyDeleteAccount(jstringToString(env, jAuth),
                                 jstringToString(env, jPassword), jDeleteContent);
    return env->NewStringUTF(r.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmyChangePassword(JNIEnv* env, jclass,
        jstring jAuth, jstring jNewPass, jstring jNewPassVerify, jstring jOldPass) {
    auto r = lemmyChangePassword(jstringToString(env, jAuth), jstringToString(env, jNewPass),
                                  jstringToString(env, jNewPassVerify),
                                  jstringToString(env, jOldPass));
    return env->NewStringUTF(r.c_str());
}

// ==== Private Messages ====

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmyListPrivateMessages(JNIEnv* env, jclass,
        jstring jAuth, jint jPage, jint jLimit, jboolean jUnreadOnly) {
    auto r = lemmyListPrivateMessages(jstringToString(env, jAuth), jPage, jLimit, jUnreadOnly);
    return env->NewStringUTF(r.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmySendPrivateMessage(JNIEnv* env, jclass,
        jstring jAuth, jint jRecipientId, jstring jContent) {
    auto r = lemmySendPrivateMessage(jstringToString(env, jAuth), jRecipientId,
                                      jstringToString(env, jContent));
    return env->NewStringUTF(r.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmyEditPrivateMessage(JNIEnv* env, jclass,
        jstring jAuth, jint jMessageId, jstring jContent) {
    auto r = lemmyEditPrivateMessage(jstringToString(env, jAuth), jMessageId,
                                      jstringToString(env, jContent));
    return env->NewStringUTF(r.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmyDeletePrivateMessage(JNIEnv* env, jclass,
        jstring jAuth, jint jMessageId, jboolean jDeleted) {
    auto r = lemmyDeletePrivateMessage(jstringToString(env, jAuth), jMessageId, jDeleted);
    return env->NewStringUTF(r.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmyMarkPrivateMessageRead(JNIEnv* env, jclass,
        jstring jAuth, jint jMessageId, jboolean jRead) {
    auto r = lemmyMarkPrivateMessageRead(jstringToString(env, jAuth), jMessageId, jRead);
    return env->NewStringUTF(r.c_str());
}

// ==== Search ====

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmySearch(JNIEnv* env, jclass,
        jstring jAuth, jstring jQuery, jstring jType, jstring jSort,
        jstring jListingType, jint jPage, jint jLimit,
        jint jCommunityId, jstring jCommunityName, jint jCreatorId) {
    auto r = lemmySearch(jstringToString(env, jAuth), jstringToString(env, jQuery),
                          jstringToString(env, jType), jstringToString(env, jSort),
                          jstringToString(env, jListingType), jPage, jLimit,
                          jCommunityId, jstringToString(env, jCommunityName), jCreatorId);
    return env->NewStringUTF(r.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmyResolveObject(JNIEnv* env, jclass,
        jstring jAuth, jstring jQuery) {
    auto r = lemmyResolveObject(jstringToString(env, jAuth), jstringToString(env, jQuery));
    return env->NewStringUTF(r.c_str());
}

// ==== Moderation ====

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmyGetModlog(JNIEnv* env, jclass,
        jstring jAuth, jint jModPersonId, jint jCommunityId,
        jint jPage, jint jLimit, jstring jType, jint jOtherPersonId) {
    auto r = lemmyGetModlog(jstringToString(env, jAuth), jModPersonId, jCommunityId,
                             jPage, jLimit, jstringToString(env, jType), jOtherPersonId);
    return env->NewStringUTF(r.c_str());
}

// ==== Admin ====

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmyAddAdmin(JNIEnv* env, jclass,
        jstring jAuth, jint jPersonId, jboolean jAdded) {
    auto r = lemmyAddAdmin(jstringToString(env, jAuth), jPersonId, jAdded);
    return env->NewStringUTF(r.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmyListRegistrationApplications(JNIEnv* env, jclass,
        jstring jAuth, jboolean jUnreadOnly, jint jPage, jint jLimit) {
    auto r = lemmyListRegistrationApplications(jstringToString(env, jAuth), jUnreadOnly,
                                                jPage, jLimit);
    return env->NewStringUTF(r.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmyApproveRegistration(JNIEnv* env, jclass,
        jstring jAuth, jint jId, jboolean jApprove, jstring jDenyReason) {
    auto r = lemmyApproveRegistration(jstringToString(env, jAuth), jId, jApprove,
                                       jstringToString(env, jDenyReason));
    return env->NewStringUTF(r.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmyBlockInstance(JNIEnv* env, jclass,
        jstring jAuth, jint jInstanceId, jboolean jBlock) {
    auto r = lemmyBlockInstance(jstringToString(env, jAuth), jInstanceId, jBlock);
    return env->NewStringUTF(r.c_str());
}

// ==== Image Upload ====

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmyUploadImage(JNIEnv* env, jclass,
        jstring jAuth, jbyteArray jImageData, jstring jFilename, jstring jMimeType) {
    auto auth = jstringToString(env, jAuth);
    auto filename = jstringToString(env, jFilename);
    auto mime = jstringToString(env, jMimeType);

    jsize len = env->GetArrayLength(jImageData);
    jbyte* bytes = env->GetByteArrayElements(jImageData, nullptr);
    std::string imageData(reinterpret_cast<char*>(bytes), static_cast<size_t>(len));
    env->ReleaseByteArrayElements(jImageData, bytes, JNI_ABORT);

    auto r = lemmyUploadImage(auth, imageData, filename, mime);
    return env->NewStringUTF(r.c_str());
}

// ==== Utility ====

extern "C" JNIEXPORT jstring JNICALL
Java_chat_progressive_app_lemmy_LemmyNative_nativeLemmyBuildJson(JNIEnv* env, jclass,
        jstring jKeys, jstring jValues) {
    // keys and values are comma-separated strings
    std::string keysStr = jstringToString(env, jKeys);
    std::string valuesStr = jstringToString(env, jValues);
    std::vector<std::pair<std::string, std::string>> fields;

    // Parse comma-separated pairs
    size_t kp = 0, vp = 0;
    while (kp < keysStr.size() && vp < valuesStr.size()) {
        auto kn = keysStr.find(',', kp);
        auto vn = valuesStr.find(',', vp);
        std::string key = keysStr.substr(kp, kn == std::string::npos ? std::string::npos : kn - kp);
        std::string val = valuesStr.substr(vp, vn == std::string::npos ? std::string::npos : vn - vp);
        fields.push_back({key, val});
        if (kn == std::string::npos || vn == std::string::npos) break;
        kp = kn + 1; vp = vn + 1;
    }
    auto r = lemmyBuildJson(fields);
    return env->NewStringUTF(r.c_str());
}
