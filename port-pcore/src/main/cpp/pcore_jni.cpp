#include <jni.h>

extern "C" JNIEXPORT jstring JNICALL
Java_chat_pcp_app_pcore_PcoreBridge_ping(JNIEnv* env, jobject) {
    return env->NewStringUTF("ok");
}
