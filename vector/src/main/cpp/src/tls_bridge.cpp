#include "progressive/tls_bridge.hpp"
#include <cstring>
#include <jni.h>

namespace progressive {

// Cached JavaVM and method IDs for JNI callbacks
static JavaVM* gJvm = nullptr;
static jclass gBridgeClass = nullptr;
static jmethodID gRequestMethod = nullptr;

bool tlsBridgeInit(void* jniEnv) {
    JNIEnv* env = static_cast<JNIEnv*>(jniEnv);
    if (!env) return false;

    // Cache the JavaVM
    if (env->GetJavaVM(&gJvm) != JNI_OK) return false;

    // Find the bridge class — defined in ProgressiveNative.kt as companion
    jclass localClass = env->FindClass("im/vector/app/features/jumptodate/ProgressiveNative");
    if (!localClass) return false;

    gBridgeClass = static_cast<jclass>(env->NewGlobalRef(localClass));
    env->DeleteLocalRef(localClass);

    // Find the static method: nativeTlsRequest(String host, int port, String request, int timeoutMs) → String
    gRequestMethod = env->GetStaticMethodID(
        gBridgeClass,
        "nativeTlsRequest",
        "(Ljava/lang/String;ILjava/lang/String;I)Ljava/lang/String;"
    );

    return gRequestMethod != nullptr;
}

// Helper: get JNIEnv for current thread, attaching if needed
static JNIEnv* getJniEnv() {
    if (!gJvm) return nullptr;

    JNIEnv* env = nullptr;
    jint result = gJvm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);

    if (result == JNI_EDETACHED) {
        // Attach the current thread
        JavaVMAttachArgs args;
        args.version = JNI_VERSION_1_6;
        args.name = "ProgressiveTLS";
        args.group = nullptr;

        if (gJvm->AttachCurrentThread(&env, &args) == JNI_OK) {
            // Will be detached on thread exit
        }
    }

    return env;
}

bool tlsBridgeAvailable() {
    return gJvm != nullptr && gBridgeClass != nullptr && gRequestMethod != nullptr;
}

std::string tlsBridgeRequest(
    const std::string& host,
    int port,
    const std::string& request,
    int timeoutMs)
{
    JNIEnv* env = getJniEnv();
    if (!env) return "";

    // Convert C++ strings to Java strings
    jstring jHost = env->NewStringUTF(host.c_str());
    jstring jRequest = env->NewStringUTF(request.c_str());

    // Call static method: ProgressiveNative.nativeTlsRequest(host, port, request, timeoutMs)
    jstring jResult = static_cast<jstring>(env->CallStaticObjectMethod(
        gBridgeClass,
        gRequestMethod,
        jHost,
        static_cast<jint>(port),
        jRequest,
        static_cast<jint>(timeoutMs)
    ));

    // Check for Java exceptions
    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
        env->ExceptionClear();
        env->DeleteLocalRef(jHost);
        env->DeleteLocalRef(jRequest);
        return "";
    }

    // Convert result back to C++ string
    std::string result;
    if (jResult) {
        const char* chars = env->GetStringUTFChars(jResult, nullptr);
        if (chars) {
            result = chars;
            env->ReleaseStringUTFChars(jResult, chars);
        }
        env->DeleteLocalRef(jResult);
    }

    env->DeleteLocalRef(jHost);
    env->DeleteLocalRef(jRequest);

    return result;
}

} // namespace progressive
